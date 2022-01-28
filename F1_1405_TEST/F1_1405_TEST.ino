/**
  * @brief board type 1405.
  * Test program to send data to the 1415 board with the 1405 board.
  * 
  * Arduino IDE settings
  *   Generic STM32F1 series.
  *   Generic F103RBTx
  *   U(S)ART support enabled(generic Serial)
  * 
  *   FIFO(on the board 2104) size is 512 * 18bit
  */

#include  <STM32_EXTI.h>
#include  <STM32F_CPU_Identity.h>
#include  <DEBUG_UTIL.h>

#define  ALVC7804_WORD_SIZE  (512 - 0)  /* 512 * 18bits. */

/*
 defines for gpio pins.
*/
/* ID SW */
#define  IDSW_PORT  GPIOC
#define  IDSW_0     (IDSW_PORT->IDR & GPIO_PIN_13)
#define  IDSW_1     (IDSW_PORT->IDR & GPIO_PIN_14)
#define  IDSW_2     (IDSW_PORT->IDR & GPIO_PIN_15)
#define  IDSW       (((IDSW_PORT->IDR & (GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15)) >> 13) & 0x0007)

/* analog input. */
#define  ADC123_IN10  PC0   /* analog input */
#define  ADC123_IN11  PC1   /* analog input */
#define  ADC123_IN12  PC2   /* analog input */
#define  ADC123_IN13  PC3   /* analog input */
#define  ADC123_IN0   PA0   /* analog input */
#define  ADC123_IN1   PA1   /* analog input */
#define  ADC123_IN2   PA2   /* analog input */
#define  ADC123_IN3   PA3   /* analog input */
#define  ADC12_IN4    PA4   /* analog input */
#define  ADC12_IN5    PA5   /* analog input */
#define  V2R5         PA6   /* analog input (ADC12_IN6) */

/* DATA BUS */
#define  FIFO_D0      PB0   /* fifo data bus bit 0 */
#define  FIFO_D1      PB1   /* fifo data bus bit 1 */
#define  FIFO_D2      PB2   /* fifo data bus bit 2 */
#define  FIFO_D3      PB3   /* fifo data bus bit 3 */
#define  FIFO_D4      PB4   /* fifo data bus bit 4 */
#define  FIFO_D5      PB5   /* fifo data bus bit 5 */
#define  FIFO_D6      PB6   /* fifo data bus bit 6 */
#define  FIFO_D7      PB7   /* fifo data bus bit 7 */
#define  BUS_PORT  GPIOB
#define  BUS_PIN   (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7)

#define  INT2HOST     PC10   /* interrupt request to the host. */
#define  CONVERT      PC11   /* adc convert timing input. */
#define  FIFOWR1      PC12   /* fifo write timing 1. */
#define  FIFOWR0      PD2    /* fifo write timing 0. */

/* I2C2 */
#define  SCL2  PB10
#define  SDA2  PB11

/* PA11 :ACTLED */
#undef   LED_BUILTIN
#define  LED_BUILTIN  PA11
#define  ACTLED_PORT   GPIOA
#define  ACTLED_PIN    GPIO_PIN_11
#define  ACTLED_IS_0  GPIO_PIN_IS_0(ACTLED_PORT,ACTLED_PIN)
#define  ACTLED_IS_1  GPIO_PIN_IS_1(ACTLED_PORT,ACTLED_PIN)

/* PC10 :INT2HOST */
#define  INT2HOST_PORT  GPIOC
#define  INT2HOST_PIN   GPIO_PIN_10
#define  INT2HOST_IS_0  GPIO_PIN_IS_0(INT2HOST_PORT,INT2HOST_PIN)
#define  INT2HOST_IS_1  GPIO_PIN_IS_1(INT2HOST_PORT,INT2HOST_PIN)

/* PD2 :FIFOWR0 */
#define  FIFOWR0_PORT  GPIOD
#define  FIFOWR0_PIN   GPIO_PIN_2
#define  FIFOWR0_IS_0  GPIO_PIN_IS_0(FIFOWR0_PORT,FIFOWR0_PIN)
#define  FIFOWR0_IS_1  GPIO_PIN_IS_1(FIFOWR0_PORT,FIFOWR0_PIN)

/* PC12 :FIFOWR1 */
#define  FIFOWR1_PORT  GPIOC
#define  FIFOWR1_PIN   GPIO_PIN_12
#define  FIFOWR1_IS_0  GPIO_PIN_IS_0(FIFOWR1_PORT,FIFOWR1_PIN)
#define  FIFOWR1_IS_1  GPIO_PIN_IS_1(FIFOWR1_PORT,FIFOWR1_PIN)

/* PC11 : convert timing for ADC. */
#define  CONVERT       PC11
#define  CONVERT_PORT  GPIOC
#define  CONVERT_PIN   GPIO_PIN_11
#define  CONVERT_IS    (CONVERT_PORT->IDR & CONVERT_PIN)

#define  USART1_Tx  PA9
#define  USART1_Rx  PA10

STM32F_CPU_IDENTITY cpu;
HardwareSerial Serial1( USART1_Rx, USART1_Tx );  // rxd pin, txd pin
uint8_t board_id;
uint8_t convert_update,convert_update_Base;
uint16_t counter;
uint32_t blink_led_time;
int loopCount;

void setup()
{
  /* gpio initialize. */
  gpioInit();

  /*
    Arduino ide settings
      U(S)ART support enabled(generic Serial)
  */
  Serial1.begin( 115200UL );
  while( !Serial1 ) delay( 20UL );
  Serial1.println( "hello from serial1 on the 1405." );
//  dump( (const uint32_t *)0x08000000, 256, &Serial1 );
  String msg = "CPU CLOCK=" + String( cpu.sysclk(), DEC ) + "Hz";
  msg += " H CLOCK=" + String( cpu.hclk(), DEC ) + "Hz";
  msg += " P1 CLOCK=" + String( cpu.pclk1(), DEC ) + "Hz";
  msg += " P2 CLOCK=" + String( cpu.pclk2(), DEC ) + "Hz";
  Serial1.println( msg );
  board_id = IDSW * 2;
  msg = "BOARD ID = " + String( board_id, DEC ); Serial1.println( msg );
  counter = board_id;

  /* attach TxINT interrupt handler. */
  convert_update_Base = convert_update;
  attachEXTI( CONVERT, cbCONVERT, RISING );  // FALLING or RISING or CHANGE

  blink_led_time = millis();
}

void loop()
{
  if( convert_update != convert_update_Base )
  {
    convert_update_Base = convert_update;
    INT2HOST_IS_1;
    Serial1.println( "hello from 1415." );
    const int blocks = 32;
    if( (loopCount % 10) == 9 )
    {
//      counter++;
//      for( int i = 0; i < blocks; i++ ) { fifoWrite( counter++ ); }
    }
    for( int i = 0; i < blocks; i++ ) { fifoWrite( counter++ ); }
    loopCount++;
    INT2HOST_IS_0;
  }

  if( (millis() - blink_led_time) >= 100UL )
  {
    blink_led_time = millis();
    LED_TOGGLE( LED_BUILTIN );
  }
}

/**
  * @brief 1405 board gpio initialize.
  * @retval none.
  */
void gpioInit()
{
  GPIOA->ODR = 0x0000;
  /* PA0: ADC123_IN0 */
  /* PA1: ADC123_IN1 */
  /* PA2: ADC123_IN2 */
  /* PA3: ADC123_IN3 */
  /* PA4: ADC12_IN4 */
  /* PA5: ADC12_IN5 */
  /* PA6: ADC12_IN6 (2.5V reference) */
  /* PA7:  not connection. */
  /* PA8: not connection. */
  /* PA9: USART1 TXD:Alternate function */
  /* PA10: USART1 RXD:Alternate function */
  /* PA11: ACT LED output */
  /* PA12: not connection. */
  /* PA13: SWDIO */
  /* PA14: SWDCLK */
  /* PA15: JTAG JTDI but not use. */

  GPIOB->ODR = 0x0000;
  /* PB0: FIFO D0 output. */
  /* PB1: FIFO D1 output. */
  /* PB2: FIFO D2 output. (BOOT1) */
  /* PB3: JTAG TDO but use for FIFO D3 output.(need remap).*/
  /* PB4: JTAG TRST but use for FIFO D4 output.(need remap).*/
  /* PB5: FIFO D5 output. */
  /* PB6: FIFO D6 output. */
  /* PB7: FIFO D7 output. */
  /* PB8: not connection. */
  /* PB9: not connection. */
  /* PB10: not connection. */
  /* PB11: not connection. */
  /* PB12: not connection. */
  /* PB13: not connection. */
  /* PB14: not connection. */
  /* PB15: not connection. */

  GPIOC->ODR = 0x1000;  /* 0001 0000 0000 0000 */
  /* PC0: ADC123_IN10 */
  /* PC1: ADC123_IN11 */
  /* PC2: ADC123_IN12 */
  /* PC3: ADC123_IN13 */
  /* PC4:  not connection. */
  /* PC5:  not connection. */
  /* PC6: not connection. */
  /* PC7: not connection. */
  /* PC8: not connection. */
  /* PC9: not connection. */
  /* PC10: INT to HOST(V50) output. */
  /* PC11: CONVERT input. */
  /* PC12: FIFOWR1 output. */
  /* PC13: IDSW0 input. */
  /* PC14: IDSW1 input. */
  /* PC15: IDSW2 input. */

  GPIOD->ODR = 0x0004;  /* 0000 0000 0000 0100 */
  /* PD0:  oscillator input. */
  /* PD1:  oscillator output. */
  /* PD2: FIFOWR0 output. */
  /* PD3:  not used. */
  /* PD4:  not used. */
  /* PD5:  not used. */
  /* PD6:  not used. */
  /* PD7:  not used. */
  /* PD8: not used. */
  /* PD9: not used. */
  /* PD10: not used. */
  /* PD11: not used. */
  /* PD12: not used. */
  /* PD13: not used. */
  /* PD14: not used. */
  /* PD15: not used. */

  pinMode( FIFO_D0, OUTPUT );
  pinMode( FIFO_D1, OUTPUT );
  pinMode( FIFO_D2, OUTPUT );
  pinMode( FIFO_D3, OUTPUT );
  pinMode( FIFO_D4, OUTPUT );
  pinMode( FIFO_D5, OUTPUT );
  pinMode( FIFO_D6, OUTPUT );
  pinMode( FIFO_D7, OUTPUT );

  FIFOWR0_IS_1;
  FIFOWR1_IS_1;
  pinMode( FIFOWR0, OUTPUT );
  pinMode( FIFOWR1, OUTPUT );

  INT2HOST_IS_0;
  pinMode( INT2HOST, OUTPUT_OPEN_DRAIN );

  /* act led enabled. */
  pinMode( LED_BUILTIN, OUTPUT );
}

/**
  * @brief the data write to fifo function.
  * @retval none.
  */
void fifoWrite( uint16_t data )
{
  volatile unsigned short pb;

  /* lower side. */
  FIFOWR0_IS_1;
  noInterrupts();
  pb = BUS_PORT->ODR;
  pb &= ~BUS_PIN;
  pb |= data & 0x00ff;
  BUS_PORT->ODR = pb;  /* lower side. */
  interrupts();
  FIFOWR0_IS_0;  // about 120ns
  FIFOWR0_IS_0;  // about 120ns
  FIFOWR0_IS_1;  // about 120ns
  FIFOWR0_IS_1;  // about 120ns

  /* upper side. */
//  FIFOWR1_IS_1;  // If you set it to 1, extra data will be added at the first read.
  FIFOWR1_IS_0;  // Zero will work better.
  noInterrupts();
  pb = BUS_PORT->ODR;
  pb &= ~BUS_PIN;
  pb |= (data >> 8) & 0x00ff;  /* upper side. */
  BUS_PORT->ODR = pb;
  interrupts();
  FIFOWR1_IS_0;  // about 120ns
  FIFOWR1_IS_0;  // about 120ns
  FIFOWR1_IS_1;  // about 120ns
  FIFOWR1_IS_1;  // about 120ns
}

void fifoWrite( uint16_t *data, size_t size )
{
  for( int i = 0; i < (int)size; i++ ) fifoWrite( *data++ );
}

/**
  * @brief CONVERT call back function.
  * @retval none.
  */
void cbCONVERT( void )
{
  convert_update++;
}
