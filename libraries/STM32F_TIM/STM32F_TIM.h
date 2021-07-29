/*
  STM32F TImer extention librarly.

  Copyright (C) 2014 hamayan All Rights Reserved.
*/
#ifndef  __STM32F_TIM__
#define  __STM32F_TIM__

#include  <Arduino.h>
#include  <HardwareTimer.h>

#define  TIM5_CCR1(x)  do{TIM5->CCR1=(uint16_t)x;}while(0)
#define  TIM5_CCR2(x)  do{TIM5->CCR2=(uint16_t)x;}while(0)
#define  TIM5_CCR3(x)  do{TIM5->CCR3=(uint16_t)x;}while(0)
#define  TIM5_CCR4(x)  do{TIM5->CCR4=(uint16_t)x;}while(0)

#define  TIM3_CCR1(x)  do{TIM3->CCR1=(uint16_t)x;}while(0)
#define  TIM3_CCR2(x)  do{TIM3->CCR2=(uint16_t)x;}while(0)
#define  TIM3_CCR3(x)  do{TIM3->CCR3=(uint16_t)x;}while(0)
#define  TIM3_CCR4(x)  do{TIM3->CCR4=(uint16_t)x;}while(0)

#define  TIM1_CCR1(x)  do{TIM1->CCR1=(uint16_t)x;}while(0)
#define  TIM1_CCR2(x)  do{TIM1->CCR2=(uint16_t)x;}while(0)
#define  TIM1_CCR3(x)  do{TIM1->CCR3=(uint16_t)x;}while(0)
#define  TIM1_CCR4(x)  do{TIM1->CCR4=(uint16_t)x;}while(0)

#define  TIM2_CCR1(x)  do{TIM2->CCR1=(uint16_t)x;}while(0)
#define  TIM2_CCR2(x)  do{TIM2->CCR2=(uint16_t)x;}while(0)
#define  TIM2_CCR3(x)  do{TIM2->CCR3=(uint16_t)x;}while(0)
#define  TIM2_CCR4(x)  do{TIM2->CCR4=(uint16_t)x;}while(0)

#define  TIM4_CCR1(x)  do{TIM4->CCR1=(uint16_t)x;}while(0)
#define  TIM4_CCR2(x)  do{TIM4->CCR2=(uint16_t)x;}while(0)
#define  TIM4_CCR3(x)  do{TIM4->CCR3=(uint16_t)x;}while(0)
#define  TIM4_CCR4(x)  do{TIM4->CCR4=(uint16_t)x;}while(0)

#define  TIM8_CCR1(x)  do{TIM8->CCR1=(uint16_t)x;}while(0)
#define  TIM8_CCR2(x)  do{TIM8->CCR2=(uint16_t)x;}while(0)
#define  TIM8_CCR3(x)  do{TIM8->CCR3=(uint16_t)x;}while(0)
#define  TIM8_CCR4(x)  do{TIM8->CCR4=(uint16_t)x;}while(0)

#define  PWM1_CHANNEL  1  // tim5 ch1
#define  PWM2_CHANNEL  2  // tim5 ch2
#define  PWM3_CHANNEL  3  // tim5 ch3
#define  PWM4_CHANNEL  4  // tim5 ch4

#define  PWM5_CHANNEL  1  // tim3 ch1
#define  PWM6_CHANNEL  2  // tim3 ch2
#define  PWM7_CHANNEL  3  // tim3 ch3
#define  PWM8_CHANNEL  4  // tim3 ch4

#define  PWM9_CHANNEL   1  // tim1 ch1
#define  PWM10_CHANNEL  2  // tim1 ch2
#define  PWM11_CHANNEL  3  // tim1 ch3
#define  PWM12_CHANNEL  4  // tim1 ch4

#define  PWM13_CHANNEL  1  // tim2 ch1
#define  PWM14_CHANNEL  2  // tim2 ch2
#define  PWM15_CHANNEL  3  // tim2 ch3
#define  PWM16_CHANNEL  4  // tim2 ch4

#define  PWM17_CHANNEL  1  // tim4 ch1
#define  PWM18_CHANNEL  2  // tim4 ch2
#define  PWM19_CHANNEL  3  // tim4 ch3
#define  PWM20_CHANNEL  4  // tim4 ch4

#define  PWM21_CHANNEL  1  // tim8 ch1
#define  PWM22_CHANNEL  2  // tim8 ch2
#define  PWM23_CHANNEL  3  // tim8 ch3
#define  PWM24_CHANNEL  4  // tim8 ch4

#define  DMA_CCRX_EN    (1UL << 0)  // channel enable. set is enable.
#define  DMA_CCRX_TCIE  (1UL << 1)  // TC(transmit complete) interrupt request enable. set is enable.
#define  DMA_CCRX_HTIE  (1UL << 2)  // HT(half transmit) interrupt request enable. set is enable.
#define  DMA_CCRX_TEIE  (1UL << 3)  // TE(transmit error) interrupt request enable. set is enable.
#define  DMA_CCRX_DIR   (1UL << 4)  // data direction. set is from memory to peripheral.
#define  DMA_CCRX_CIRC  (1UL << 5)  // Circulation mode. set is enable.
#define  DMA_CCRX_PINC  (1UL << 6)  // peripheral increment mode. set is peripheral increment enable.
#define  DMA_CCRX_MINC  (1UL << 7)  // Memory increment mode. set is Memory increment enable.
#define  DMA_CCRX_PSIZE_8BIT (0UL << 8)    // peripheral data size = 8bit.
#define  DMA_CCRX_PSIZE_16BIT (1UL << 8)   // peripheral data size = 16bit.
#define  DMA_CCRX_PSIZE_32BIT (2UL << 8)   // peripheral data size = 32bit.
#define  DMA_CCRX_MSIZE_8BIT (0UL << 10)   // memory data size = 8bit.
#define  DMA_CCRX_MSIZE_16BIT (1UL << 10)  // memory data size = 16bit.
#define  DMA_CCRX_MSIZE_32BIT (2UL << 10)  // memory data size = 32bit.
#define  DMA_CCRX_PL_LOW (0UL << 12)       // priority level is low.
#define  DMA_CCRX_PL_MIDDLE (1UL << 12)    // priority level is middle.
#define  DMA_CCRX_PL_HIGH (2UL << 12)      // priority level is high.
#define  DMA_CCRX_PL_VERY_HIGH (3UL << 12) // priority level is very high.
#define  DMA_CCRX_MEM2MEM (1UL << 14)  // memory to memory transfer enable. set is enable.

/*
  Typedef enum { HAL_OK = 0x00, HAL_ERROR = 0x01, HAL_BUSY = 0x02, HAL_TIMEOUT = 0x03
  } HAL_StatusTypeDef;
*/

/*
typedef struct
{
  TIM_TypeDef                        *Instance;  // Register base address
  TIM_Base_InitTypeDef               Init;       // TIM Time Base required parameters
  HAL_TIM_ActiveChannel              Channel;    // Active channel
  DMA_HandleTypeDef                  *hdma[7];   // DMA Handlers array This array is accessed by a @ref DMA_Handle_index
  HAL_LockTypeDef                    Lock;       // Locking object
  __IO HAL_TIM_StateTypeDef          State;      // TIM operation state
  __IO HAL_TIM_ChannelStateTypeDef   ChannelState[4];  // TIM channel operation state
  __IO HAL_TIM_ChannelStateTypeDef   ChannelNState[4]; // TIM complementary channel operation state
  __IO HAL_TIM_DMABurstStateTypeDef  DMABurstState;    // DMA burst operation state
} TIM_HandleTypeDef;
*/

/*
typedef struct
{
  uint32_t Direction;  // Specifies if the data will be transferred from memory to peripheral, 
                       // from memory to memory or from peripheral to memory.
                       // This parameter can be a value of @ref DMA_Data_transfer_direction
  uint32_t PeriphInc;  // Specifies whether the Peripheral address register should be incremented or not.
                       // This parameter can be a value of @ref DMA_Peripheral_incremented_mode
  uint32_t MemInc;     // Specifies whether the memory address register should be incremented or not.
                       // This parameter can be a value of @ref DMA_Memory_incremented_mode
  uint32_t PeriphDataAlignment;  // Specifies the Peripheral data width.
                                 // This parameter can be a value of @ref DMA_Peripheral_data_size
  uint32_t MemDataAlignment;     // Specifies the Memory data width.
                                 // This parameter can be a value of @ref DMA_Memory_data_size
  uint32_t Mode;      // Specifies the operation mode of the DMAy Channelx.
                      // This parameter can be a value of @ref DMA_mode
                      // @note The circular buffer mode cannot be used if the memory-to-memory
                      // data transfer is configured on the selected Channel
  uint32_t Priority;  // Specifies the software priority for the DMAy Channelx.
                      // This parameter can be a value of @ref DMA_Priority_level
} DMA_InitTypeDef;
*/

/*
typedef struct
{
  DMA_Channel_TypeDef   *Instance;  // Register base address
  DMA_InitTypeDef       Init;       // DMA communication parameters
  HAL_LockTypeDef       Lock;       // DMA locking object
  HAL_DMA_StateTypeDef  State;      // DMA transfer state
  void                  *Parent;    // Parent object state
  void                  (* XferCpltCallback)( struct __DMA_HandleTypeDef * hdma);     // DMA transfer complete callback
  void                  (* XferHalfCpltCallback)( struct __DMA_HandleTypeDef * hdma); // DMA Half transfer complete callback
  void                  (* XferErrorCallback)( struct __DMA_HandleTypeDef * hdma);    // DMA transfer error callback
  void                  (* XferAbortCallback)( struct __DMA_HandleTypeDef * hdma);    // DMA transfer abort callback
  __IO uint32_t         ErrorCode;                                                    // DMA Error code
  DMA_TypeDef            *DmaBaseAddress;                                             // DMA Channel Base Address
  uint32_t               ChannelIndex;                                                // DMA Channel Index
} DMA_HandleTypeDef;
*/

extern DMA_HandleTypeDef *exhdma;
extern "C"
{
  int  stm32f_Timer_frequency( HardwareTimer *Timer, uint32_t freq );
  void enaDMA( DMA_Channel_TypeDef *DMAxChannely );
  void disDMA( DMA_Channel_TypeDef *DMAxChannely );
  void enaTimChannelDMA( TIM_TypeDef *TIMx, int ch );
  void disTimChannelDMA( TIM_TypeDef *TIMx, int ch );
}



class STM32F_TIM // : public hoge,hage,hige
{
private:
  HardwareTimer *Timer;  // HardwareTimer instance. ex.Timer3
  TIM_HandleTypeDef *h_tim;  // TIM Time Base Handle Structure
  DMA_HandleTypeDef h_dma;  // DMA handle Structure
  uint32_t pinNumber;  // stm32 pin number.  ex. PC6,PC7,PC8,PC9,
  uint32_t value;  // compare match value. ex. 100UL
  void *src;  // memory address
  uint16_t elementSize;  // transmit element size

/*
 *  set the output compare match mode to pwm1.
 */
  void setPWM1() { Timer->setMode( channel, TIMER_OUTPUT_COMPARE_PWM1, pinNumber ); }
  int  channel2Index();
  void enaTimChDMA();
  void disTimChDMA();

public:
  int limit;  // timer auto reload register value
  int scale;  // swing scale
  int offset;  // center offset
  int index;
  uint32_t channel;  // compare match channel. ex. 1,2,3,4

  STM32F_TIM() {}
  STM32F_TIM( HardwareTimer *_t, uint32_t _ch, uint32_t _pin, uint32_t _val = 0UL )
  {
    begin( _t, _pin, _ch, _val );
  }
  ~STM32F_TIM() { end(); }

/*
 *  begin and end
 */
  void begin( HardwareTimer *_t, uint32_t _ch, uint32_t _pin, uint32_t _val = 0UL )
  {
    Timer = _t;
    channel = _ch;
    pinNumber = _pin;
    h_tim = Timer->getHandle();  // return the handle address for HAL related configuration
    index = channel2Index();
    setCompareMatch( _val );
    setPWM1();
 }

  void end() {}
  void configDMA( uint32_t _mode = DMA_CIRCULAR, uint8_t pri = 4, uint8_t sub = 0 );
  void startDMA();
  void abortDMA();

/*
 *  set the output compare value.
 */
  void setCompareMatch( uint32_t _val )
  {
    value = _val;  // compare match value.
    Timer->setCaptureCompare( channel, value );
  }

/*
 *  set arr and scale and offset.
 */
  void setScaleAndOffset( int _arr, int _s, int _o )
  {
    limit = _arr;
    scale = _s;
    offset = _o;
  };

/*
 *  set memory address and element size.
 */
  void setSource( void *_src, uint16_t _sz )
  {
    src = _src;  // memory address
    elementSize = _sz;  // transmit element size
  }
};


#endif  /*  __STM32F_TIM__ */

/*
  Copyright (C) 2014 hamayan All Rights Reserved.
*/
