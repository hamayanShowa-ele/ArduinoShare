#include  <STM32F_CPU_Identity.h>
#include  <STM32F_TIM.h>

/* USART1 TXD RXD */
#define  TXD1       PA9
#define  RXD1       PA10

#define  PWM5_PIN   PC6
#define  PWM6_PIN   PC7
#define  PWM7_PIN   PC8
#define  PWM8_PIN   PC9
#define  PD7_PORT   GPIOD
#define  PD7_PIN    GPIO_PIN_7

/* Private variables ---------------------------------------------------------*/
STM32F_CPU_IDENTITY cpu_id;
HardwareSerial Serial1( RXD1, TXD1 );  // rxd pin, txd pin
HardwareTimer Timer3( TIM3 );
STM32F_TIM pwm5,pwm7,pwm8;

#define  PWM_ELEMENTS    100
uint16_t pwm_table[ PWM_ELEMENTS ];
volatile uint32_t xferCount;
uint32_t baseTim;

/* Private function prototypes -----------------------------------------------*/
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);

void setup()
{
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();

  pinMode( PD7, OUTPUT );
  digitalWrite( PD7, LOW );
  /* Serial initialize and output start up message. */
  Serial1.begin( 115200UL );
  while( !Serial1 ) delay( 20UL );
  Serial1.println( "\r\n\r\nSTM32F1 SINE WAVE output by PWM." );

  /* get cpu id, sysclock,hclk,pclk,systic. */
  String msg = "systic:"; msg += String( cpu_id.sysload() );
  msg += " hclk:"; msg += String( cpu_id.hclk() );
  msg += " pclk1:"; msg += String( cpu_id.pclk1() );
  msg += " pclk2:"; msg += String( cpu_id.pclk2() );
  msg += " sysclock:"; msg += String( cpu_id.sysclk() );
  Serial1.println( msg );
  uint32_t uid[3];
  cpu_id.uID( uid );
  msg = "UID0:0x"; msg += String( uid[0], HEX );
  msg += " UID1:0x"; msg += String( uid[1], HEX );
  msg += " UID2:0x"; msg += String( uid[2], HEX );
  Serial1.println( msg );

  /* Timer initialize. */
  Timer3.pause();
  int overFlow = stm32f_Timer_frequency( &Timer3, 100 * 1000UL );

  for( int i = 0; i < PWM_ELEMENTS; i++ )
  {
    double tempD = (overFlow * 0.45) * sin( (2 * M_PI * i) / PWM_ELEMENTS );
    pwm_table[ i ] = (uint16_t)(tempD + (overFlow / 2.0));
  }

  /* PWM5 initialize. */
  pwm5.begin( &Timer3, PWM5_CHANNEL, PWM5_PIN, overFlow / 2 );
  pwm5.setScaleAndOffset( overFlow, overFlow / 2, overFlow / 2 );
  Serial1.printf( "pwm5 limit = %d scale = %d offset = %d.\n", pwm5.limit, pwm5.scale, pwm5.offset );
  pwm5.configDMA();
  pwm5.setSource( pwm_table, PWM_ELEMENTS );
  pwm5.startDMA();

  /* PWM7 initialize. */
  pwm7.begin( &Timer3, PWM7_CHANNEL, PWM7_PIN, overFlow / 2 );
  pwm7.setScaleAndOffset( overFlow, overFlow / 2, overFlow / 2 );
  Serial1.printf( "pwm7 limit = %d scale = %d offset = %d.\n", pwm7.limit, pwm7.scale, pwm7.offset );
  pwm7.configDMA();
  pwm7.setSource( pwm_table, PWM_ELEMENTS );
  pwm7.startDMA();

  /* PWM8 initialize. */
  pwm8.begin( &Timer3, PWM8_CHANNEL, PWM8_PIN, overFlow / 2 );
  pwm8.setScaleAndOffset( overFlow, overFlow / 2, overFlow / 2 );
  Serial1.printf( "pwm8 limit = %d scale = %d offset = %d.\n", pwm8.limit, pwm8.scale, pwm8.offset );
  pwm8.configDMA();
  pwm8.setSource( pwm_table, PWM_ELEMENTS );
  pwm8.startDMA();

  /* restart timer. */
  Timer3.resume();

  baseTim = millis();
}

void loop()
{
#if 1
  if( (millis() - baseTim) >= 1 * 1000UL )
  {
    baseTim = millis();
    Serial1.printf( "xferCount = %d\n", xferCount );
  }
#endif
#if 0
  delay( 10 * 1000UL );
  Serial1.println( "stop dma" );
  disTimChannelDMA( TIM3, 1 );
  disTimChannelDMA( TIM3, 3 );
  disTimChannelDMA( TIM3, 4 );

  delay( 10 * 1000UL );
  Serial1.println( "start dma" );
  enaTimChannelDMA( TIM3, 1 );
  enaTimChannelDMA( TIM3, 3 );
  enaTimChannelDMA( TIM3, 4 );
#endif
}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}


extern "C"
{
/**
  * DMA1 channel 6 interrupt handler for PWM5.
  */
void DMA1_Channel6_IRQHandler( void )
{
  volatile uint32_t iFlag = DMA1->IFCR;
  DMA1->IFCR |= 0x0000000F << ((4 * (6 - 1)) + 0);
  PD7_PORT->ODR ^= PD7_PIN;
  xferCount++;
}

/**
  * DMA1 channel 2 interrupt handler for PWM7.
  */
void DMA1_Channel2_IRQHandler( void )
{
  volatile uint32_t iFlag = DMA1->IFCR;
  DMA1->IFCR |= 0x0000000F << ((4 * (2 - 1)) + 0);
}

/**
  * DMA1 channel 3 interrupt handler for PWM8.
  */
void DMA1_Channel3_IRQHandler( void )
{
  volatile uint32_t iFlag = DMA1->IFCR;
  DMA1->IFCR |= 0x0000000F << ((4 * (3 - 1)) + 0);
}

}  /* extern "C" */
