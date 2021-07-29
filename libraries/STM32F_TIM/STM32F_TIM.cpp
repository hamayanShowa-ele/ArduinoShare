/*
  STM32F TImer extention librarly.

  Copyright (C) 2014 hamayan All Rights Reserved.
*/

#include  <STM32F_TIM.h>

extern HardwareSerial Serial1;

static const struct
{
  TIM_TypeDef *timx;
  DMA_Channel_TypeDef *instance;
  IRQn_Type irq;
  volatile uint32_t *ccrx;
} DMA_RESOURCE[] =
{
  { TIM5, DMA2_Channel5, DMA2_Channel4_5_IRQn, &TIM5->CCR1 },  // PWM1
  { TIM5, DMA2_Channel4, DMA2_Channel4_5_IRQn, &TIM5->CCR2 },  // PWM2
  { TIM5, DMA2_Channel2, DMA2_Channel2_IRQn,   &TIM5->CCR3 },  // PWM3
  { TIM5, DMA2_Channel1, DMA2_Channel1_IRQn,   &TIM5->CCR4 },  // PWM4

  { TIM3, DMA1_Channel6, DMA1_Channel6_IRQn,   &TIM3->CCR1 },  // PWM5
  { TIM3, nullptr, (IRQn_Type)0, &TIM3->CCR2 },  // PWM6
  { TIM3, DMA1_Channel2, DMA1_Channel2_IRQn,   &TIM3->CCR3 },  // PWM7
  { TIM3, DMA1_Channel3, DMA1_Channel3_IRQn,   &TIM3->CCR4 },  // PWM8

  { TIM1, DMA1_Channel2, DMA1_Channel2_IRQn,   &TIM1->CCR1 },  // PWM9
  { TIM1, DMA1_Channel3, DMA1_Channel3_IRQn,   &TIM1->CCR2 },  // PWM10
  { TIM1, DMA1_Channel6, DMA1_Channel6_IRQn,   &TIM1->CCR3 },  // PWM11
  { TIM1, DMA1_Channel4, DMA1_Channel4_IRQn,   &TIM1->CCR4 },  // PWM12

  { TIM2, DMA1_Channel5, DMA1_Channel5_IRQn,   &TIM2->CCR1 },  // PWM13
  { TIM2, DMA1_Channel7, DMA1_Channel7_IRQn,   &TIM2->CCR2 },  // PWM14
  { TIM2, DMA1_Channel1, DMA1_Channel1_IRQn,   &TIM2->CCR3 },  // PWM15
  { TIM2, DMA1_Channel7, DMA1_Channel7_IRQn,   &TIM2->CCR4 },  // PWM16

  { TIM4, DMA1_Channel1, DMA1_Channel1_IRQn,   &TIM4->CCR1 },  // PWM17
  { TIM4, DMA1_Channel4, DMA1_Channel4_IRQn,   &TIM4->CCR2 },  // PWM18
  { TIM4, DMA1_Channel5, DMA1_Channel5_IRQn,   &TIM4->CCR3 },  // PWM19
  { TIM4, nullptr, (IRQn_Type)0, &TIM4->CCR4 },  // PWM20

  { TIM8, DMA2_Channel3, DMA2_Channel3_IRQn,   &TIM8->CCR1 },  // PWM21
  { TIM8, DMA2_Channel5, DMA2_Channel4_5_IRQn, &TIM8->CCR2 },  // PWM22
  { TIM8, DMA2_Channel1, DMA2_Channel1_IRQn,   &TIM8->CCR3 },  // PWM23
  { TIM8, DMA2_Channel2, DMA2_Channel2_IRQn,   &TIM8->CCR4 },  // PWM24
};


/*
 *  get index number from channel number.
 */
int STM32F_TIM::channel2Index()
{
  for( int index = 0; index < (int)(sizeof(DMA_RESOURCE) / sizeof(DMA_RESOURCE[0])); index++ )
  {
    if( h_tim->Instance == DMA_RESOURCE[ index ].timx )
    {
      if( DMA_RESOURCE[ index ].instance != nullptr ) return index + channel - 1;
      else return (-1);
    }
  }
  return (-1);
}

/*
 *  enable timer dma request.
 */
void STM32F_TIM::enaTimChDMA()
{
  noInterrupts();
  uint16_t tempUS = DMA_RESOURCE[ index ].timx->DIER;
  tempUS |= 1 << ((9 - 1) + channel);  // CC?DE = 1 enable dma request.
  DMA_RESOURCE[ index ].timx->DIER = tempUS;
  interrupts();
}

/*
 *  disable timer dma request.
 */
void STM32F_TIM::disTimChDMA()
{
  noInterrupts();
  uint16_t tempUS = DMA_RESOURCE[ index ].timx->DIER;
  tempUS &= ~(1 << ((9 - 1) + channel));  // CC?DE = 1 enable dma request.
  DMA_RESOURCE[ index ].timx->DIER = tempUS;
  interrupts();
}

/*
 *  configure dma channel.
 *  _ins : DMA_Channel_TypeDef
 *  _mode : DMA_CIRCULAR or DMA_NORMAL
 */
void STM32F_TIM::configDMA( uint32_t _mode, uint8_t pri, uint8_t sub )
{
  h_dma.Instance = DMA_RESOURCE[ index ].instance;
  HAL_DMA_DeInit( &h_dma );
  h_dma.Init.Direction = DMA_MEMORY_TO_PERIPH;
  h_dma.Init.PeriphInc = DMA_PINC_DISABLE;
  h_dma.Init.MemInc = DMA_MINC_ENABLE;
  h_dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  h_dma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  h_dma.Init.Mode = _mode;  // DMA_CIRCULAR or DMA_NORMAL
  h_dma.Init.Priority = DMA_PRIORITY_LOW;  // LOW or MEDIUM or HIGH or VERY_HIGH
  HAL_DMA_Init( &h_dma );

  /* enable transmit complete interrupt. */
  noInterrupts();
  volatile uint32_t tempUL = h_dma.Instance->CCR;
  tempUL &= ~DMA_CCRX_HTIE;
  tempUL |= DMA_CCRX_EN | DMA_CCRX_TCIE | DMA_CCRX_TEIE;
  h_dma.Instance->CCR = tempUL;
  interrupts();
  HAL_NVIC_SetPriority( DMA_RESOURCE[ index ].irq, pri, sub );
  HAL_NVIC_EnableIRQ( DMA_RESOURCE[ index ].irq );
}

/*
 *  start dma channel.
 */
void STM32F_TIM::startDMA()
{
  int index = channel2Index();
  HAL_DMA_Start( &h_dma, (uint32_t)src, (uint32_t)DMA_RESOURCE[ index ].ccrx, elementSize );
  enaTimChDMA();
}

/*
 *  abort dma channel.
 */
void STM32F_TIM::abortDMA()
{
  int index = channel2Index();
  disTimChDMA();
  HAL_DMA_Abort( &h_dma );
}



extern "C"
{
/*
  stm32f timer frequency set.
*/
int stm32f_Timer_frequency( HardwareTimer *Timer, uint32_t freq )
{
  uint32_t sysclk = HAL_RCC_GetSysClockFreq();
  uint32_t pre,over;
  for( pre = 1UL; pre < 65536UL; pre++ )
  {
    over = sysclk / ( pre * freq );
    if( over < 65536UL ) break;
  }
  if( pre == 65536UL ) return (-1);

  // set prescaler register (which is factor value - 1)
  Timer->setPrescaleFactor( pre );
  // set AutoReload register depending on format provided
  Timer->setOverflow( over );  // ex. 72000000 / Prescale / Overflow = frequency

  return (int)Timer->getOverflow();
}

/*
 *  enable dma Channel.
 */
void enaDMA( DMA_Channel_TypeDef *DMAxChannely )
{
  noInterrupts();
  DMAxChannely->CCR |= DMA_CCRX_EN;
  interrupts();
}

/*
 *  disable dma Channel.
 */
void disDMA( DMA_Channel_TypeDef *DMAxChannely )
{
  noInterrupts();
  DMAxChannely->CCR &= ~DMA_CCRX_EN;
  interrupts();
}

/*
 *  enable timer channel dma request.
 */
void enaTimChannelDMA( TIM_TypeDef *TIMx, int ch )
{
  noInterrupts();
  TIMx->DIER |= (1 << ((9 - 1) + ch));
  interrupts();
}

/*
 *  disable timer channel dma request.
 */
void disTimChannelDMA( TIM_TypeDef *TIMx, int ch )
{
  noInterrupts();
  TIMx->DIER &= ~(1 << ((9 - 1) + ch));
  interrupts();
}

}  /* extern "C" */

/*
  Copyright (C) 2014 hamayan All Rights Reserved.
*/
