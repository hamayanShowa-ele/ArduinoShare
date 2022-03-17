/**
  * @brief board type 1415.
  * Test program on the 1415 board to read the data sent from the 1405 board.
  * 
  * Arduino IDE settings
  *   Generic STM32F1 series.
  *   Generic F103ZETx
  *   U(S)ART support enabled(generic Serial)
  * 
  *   FIFO(on the board 2104) size is 512 * 18bit
  */

#include  <STM32_EXTI.h>
#include  <STM32_FSMC.h>
#include  <STM32F_CPU_Identity.h>
#include  <DEBUG_UTIL.h>

/*
 defines for memory address.
*/
#define  INT_SRAM_BASE_ADDRESS      0x60000000
#define  INT_ETHERNET_BASE_ADDRESS  0x64000000
#define  INT_ARCNET_BASE_ADDRESS    0x68000000
#define  CBUS_BASE_ADR              0x6C000000
#define  CBUS_IO_ADR   (CBUS_BASE_ADR + 0x00000000)
#define  CBUS_MEM_ADR  (CBUS_BASE_ADR + 0x00800000)

#define  INT_SRAM_SIZE    (524288UL * 1)  // MRAM or SRAM
#define  ALVC7804_WORD_SIZE  (512 - 0)  /* 512 * 18bits. */

#define  MULTIPLY  1
#define  BD1405_BASE_ADR  (CBUS_IO_ADR + (0x0700 * MULTIPLY))
#define  BD1405_ID_00     (0x00 * MULTIPLY)
#define  BD1405_ID_02     (0x02 * MULTIPLY)
#define  BD1405_ID_04     (0x04 * MULTIPLY)
#define  BD1405_ID_06     (0x06 * MULTIPLY)
#define  BD1405_ID_08     (0x08 * MULTIPLY)
#define  BD1405_ID_0A     (0x0A * MULTIPLY)
#define  BD1405_ID_0C     (0x0C * MULTIPLY)

/*
 defines for gpio pins.
*/
#define  USART1_Tx  PA9  /* USART1 */
#define  USART1_Rx  PA10  /* USART1 */
#define  EX_SCL1    PB6
#define  EX_SDA1    PB7
#define  DFU        PB8
#define  MRAM_CS    PD7
#define  SRAM_CS    MRAM_CS
#define  ETH_CS     PG9
#define  ARC_CS     PG10
#define  CBUS_CS    PG12

#define  USBON      PA8
#define  USBON_ACTIVE HIGH
#define  USBON_IS_0  GPIO_PIN_IS_0(GPIOA,GPIO_PIN_8)
#define  USBON_IS_1  GPIO_PIN_IS_1(GPIOA,GPIO_PIN_8)

#undef   LED_BUILTIN
#define  LED_BUILTIN PB9
#define  LED_BUILTIN_IS_0  GPIO_PIN_IS_0(GPIOB,GPIO_PIN_9)
#define  LED_BUILTIN_IS_1  GPIO_PIN_IS_1(GPIOB,GPIO_PIN_9)

#define  ETH_RST    PC2
#define  ETH_RST_IS_0  GPIO_PIN_IS_0(GPIOC,GPIO_PIN_2)
#define  ETH_RST_IS_1  GPIO_PIN_IS_1(GPIOC,GPIO_PIN_2)

#define  ARC_RST    PC3
#define  ARC_RST_IS_0  GPIO_PIN_IS_0(GPIOC,GPIO_PIN_3)
#define  ARC_RST_IS_1  GPIO_PIN_IS_1(GPIOC,GPIO_PIN_3)

#define  CBUS_RST   PD3
#define  CBUS_RST_IS_0  GPIO_PIN_IS_0(GPIOD,GPIO_PIN_3)
#define  CBUS_RST_IS_1  GPIO_PIN_IS_1(GPIOD,GPIO_PIN_3)

#define  CBUS_ENA   PF11
#define  CBUS_ENA_IS_0  GPIO_PIN_IS_0(GPIOF,GPIO_PIN_11)
#define  CBUS_ENA_IS_1  GPIO_PIN_IS_1(GPIOF,GPIO_PIN_11)

#define  ETH_INT    PC0
#define  ARC_INT    PC1
#define  EXT_INT0   PC4
#define  EXT_INT1   PC5
#define  EXT_INT2   PF6
#define  EXT_INT3   PF7
#define  EXT_INT4   PF8
#define  EXT_INT5   PF9
#define  EXT_INT6   PF10

class BOARD_1405 // : public hoge
{
private:
  bool seqFault;
  volatile uint16_t *ioAddress;
  volatile uint16_t wptr,rptr;
  uint16_t incValue;

public:

  BOARD_1405( volatile uint16_t *adr )
  {
    ioAddress = adr;
    wptr = rptr = 0;
    seqFault = true;
  }

  /**
    * @brief knock 1405.
    * @retval none.
    */
  void knock1405() { *ioAddress = 0; delayMicroseconds( 2UL ); *ioAddress = 1; }

  /**
    * @brief 1405 fifo read.
    * @retval none.
    */
  uint16_t read() { return *ioAddress; }
  void read( uint16_t *dat, int size )
  {
    size /= sizeof(uint16_t);
    for( int i = 0; i < size; i++ ) { *dat++ = read(); }
  }

  /**
    * @brief 1405 fifo dummy read.
    * @retval none.
    */
  void fifoDummyRead( int size )
  {
    for( int i = 0; i < size; i++ )
    {
      volatile uint16_t tempUS = *ioAddress;
      (void)tempUS;
    }
  }

  /**
    * @brief available.
    * @retval wptr minus rptr.
    */
  uint16_t available()
  {
    uint16_t a = wptr - rptr;
    if( a == 0 ) return 0;
//    rptr++;
    rptr = wptr;
    return a;
  }

  /**
    * @brief check data.
    * @retval If it succeeds, it returns 0, and if it fails, it returns a negative number.
    */
   int check( const uint16_t *dat, int size )
   {
     for( int i = 0; i < size; i++ )
     {
       if( seqFault == true ) { seqFault = false; incValue = *dat; }
       if( *dat != incValue ) { seqFault = true; return (-1); }
       dat++;
       incValue++;
     }
     return 0;
   }

  /**
    * @brief interrupt handler.
    * @retval none.
    */
  void intHandler() { wptr++; }

};  /* class BOARD_1405 */

STM32F_CPU_IDENTITY cpu;
HardwareSerial Serial1( USART1_Rx, USART1_Tx );  // rxd pin, txd pin
BOARD_1405 bd1405[7] =
{
  { (volatile uint16_t *)(BD1405_BASE_ADR + BD1405_ID_00) },
  { (volatile uint16_t *)(BD1405_BASE_ADR + BD1405_ID_02) },
  { (volatile uint16_t *)(BD1405_BASE_ADR + BD1405_ID_04) },
  { (volatile uint16_t *)(BD1405_BASE_ADR + BD1405_ID_06) },
  { (volatile uint16_t *)(BD1405_BASE_ADR + BD1405_ID_08) },
  { (volatile uint16_t *)(BD1405_BASE_ADR + BD1405_ID_0A) },
  { (volatile uint16_t *)(BD1405_BASE_ADR + BD1405_ID_0C) },
};

uint32_t blink_led_time;
uint32_t call_1405_time;
uint32_t counterW,counterR;

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
  Serial1.println( "hello from serial1 on the 1415." );
//  dump( (const uint32_t *)0x08000000, 256, &Serial1 );
  String msg = "CPU CLOCK=" + String( cpu.sysclk(), DEC ) + "Hz";
  msg += " H CLOCK=" + String( cpu.hclk(), DEC ) + "Hz";
  msg += " P1 CLOCK=" + String( cpu.pclk1(), DEC ) + "Hz";
  msg += " P2 CLOCK=" + String( cpu.pclk2(), DEC ) + "Hz";
  Serial1.println( msg );

  /* cpu bus initialize. */
  fsmc_address( 23 );  // set address bus size
  fsmc_data( 16 );  // set data bus size.
  fsmc_sram( FSMC_NORSRAM_BANK4, FSMC_NORSRAM_MEM_BUS_WIDTH_16, 10,13,10,15, 0 );
  /* read cycle:193ns, read address setup time:99ns, read width:48ns */
  /* write cycle:220ns, write address setup time:99ns, write width:46ns */
  delay( 50UL );
  CBUS_RST_IS_1;  /* release c-bus reset. */
  CBUS_ENA_IS_1;  /* enable c-bus. */
  delay( 1 * 1000UL );  /* Wait for the 1405 board to complete startup. */

  /* attach INT0~INT6 interrupt handler. */
//  attachEXTI( EXT_INT0, cb1405_ID00, FALLING );  // FALLING or RISING or CHANGE

  blink_led_time = millis();
  call_1405_time = blink_led_time;
}

void loop()
{
  if( (millis() - blink_led_time) >= 100UL )
  {
    blink_led_time = millis();
    LED_TOGGLE( LED_BUILTIN );
  }

  if( (millis() - call_1405_time) >= 1000UL )
  {
    call_1405_time = millis();
    bd1405[0].knock1405();
    attachEXTI( EXT_INT0, cb1405_ID00, FALLING );  // FALLING or RISING or CHANGE
    Serial1.println( "1405 ID:00 knocked." );
    counterW++;
  }

  if( bd1405[0].available() != 0 )
  {
    detachEXTI( EXT_INT0 );
    counterR++;
    Serial1.print( "1405 ID:00 data request. W = " );
    Serial1.print( counterW, DEC );
    Serial1.print( " R = " );
    Serial1.println( counterR, DEC );

    uint16_t buffer[32];
    bd1405[0].read( buffer, sizeof(buffer) );
    if( bd1405[0].check( (const uint16_t *)buffer, (int)(sizeof(buffer) / sizeof(buffer[0])) ) != 0 )
    {
      Serial1.println( "An abnormality was found in the sequential data. Perform a dummy read." );
//      bd1405[0].fifoDummyRead( ALVC7804_WORD_SIZE );
//      Serial1.println( "An abnormality was found in the sequential data. Perform a c-bus reset." );
//      cbusReset();
    }
    else
    {
      Serial1.println( "Sequential data was fine." );
    }
    dump( buffer, sizeof(buffer), &Serial1 );
  }
}

/**
  * @brief force C-BUS reset.
  * @retval none.
  */
void cbusReset()
{
  CBUS_RST_IS_0;  /* c-bus reset. */
  delay( 50UL );
  CBUS_RST_IS_1;  /* release c-bus reset. */
  delay( 1 * 100UL );
}

/**
  * @brief 1405 board gpio initialize.
  * @retval none.
  */
void gpioInit()
{
  GPIOA->ODR = 0x0000;
  /* PA0: USART2 CTS */
  /* PA1: USART2 RTS */
  /* PA2: USART2 TXD */
  /* PA3: USART2 RXD */
  /* PA4: SPI1 NSS */
  /* PA5: SPI1 SCK */
  /* PA6: SPI1 MISO */
  /* PA7: SPI1 MOSI */
  /* PA8: USBON */
  /* PA9: USART1 TXD:Alternate function */
  /* PA10: USART1 RXD:Alternate function */
  /* PA11: USBDM */
  /* PA12: USBDP */
  /* PA13: SWDIO */
  /* PA14: SWDCLK */
  /* PA15: JTAG JTDI but not use. */

  GPIOB->ODR = 0x0000;
  /* PB0: EEPCS2 */
  /* PB1: not connected. */
  /* PB2: BOOT1 */
  /* PB3: JTAG TDO but use for FIFO D3 output.(need remap).*/
  /* PB4: JTAG TRST but use for FIFO D4 output.(need remap).*/
  /* PB5: EX I2C sema */
  /* PB6: EX SCL1 */
  /* PB7: EX SDA1 */
  /* PB8: DFU */
  /* PB9: ACT LED */
  /* PB10: USART3 TXD:Alternate function */
  /* PB11: USART3 RXD:Alternate function */
  /* PB12: USART3 CD */
  /* PB13: USART3 CTS */
  /* PB14: USART3 RTS */
  /* PB15: USART3 DSR */

  GPIOC->ODR = 0x0000;
  /* PC0: ETHERNET INT */
  /* PC1: ARCNET INT */
  /* PC2: ETHERNET RESET */
  /* PC3: ARCNET RESET */
  /* PC4: INT0 */
  /* PC5: INT1 */
  /* PC6: USART3 RI */
  /* PC7: not connected. */
  /* PC8: not connected. */
  /* PC9: not connected. */
  /* PC10: UART4 TXD */
  /* PC11: UART4 RXD */
  /* PC12: not connected. */
  /* PC13: NMI */
  /* PC14: OSC32 IN */
  /* PC15: OSC32 OUT */

  GPIOD->ODR = 0x0000;
  /* PD0: DATA BUS bit2 */
  /* PD1: DATA BUS bit3 */
  /* PD2: not connected. */
  /* PD3: C-BUS RESET */
  /* PD4: RD */
  /* PD5: WR */
  /* PD6: WAIT */
  /* PD7: SRAM CS */
  /* PD8: DATA BUS bit13 */
  /* PD9: DATA BUS bit14 */
  /* PD10: DATA BUS bit15 */
  /* PD11: ADDRESS BUS bit16 */
  /* PD12: ADDRESS BUS bit17 */
  /* PD13: ADDRESS BUS bit18 */
  /* PD14: DATA BUS bit0 */
  /* PD15: DATA BUS bit1 */

  GPIOE->ODR = 0x0000;
  /* PE0: BLE */
  /* PE1: BHE */
  /* PE2: not connected. */
  /* PE3: not connected. */
  /* PE4: not connected. */
  /* PE5: not connected. */
  /* PE6: ADDRESS BUS bit21 */
  /* PE7: DATA BUS bit4 */
  /* PE8: DATA BUS bit5 */
  /* PE9: DATA BUS bit6 */
  /* PE10: DATA BUS bit7 */
  /* PE11: DATA BUS bit8 */
  /* PE12: DATA BUS bit9 */
  /* PE13: DATA BUS bit10 */
  /* PE14: DATA BUS bit11 */
  /* PE15: DATA BUS bit12 */

  GPIOF->ODR = 0x0000;
  /* PF0: ADDRESS BUS bit0 */
  /* PF1: ADDRESS BUS bit1 */
  /* PF2: ADDRESS BUS bit2 */
  /* PF3: ADDRESS BUS bit3 */
  /* PF4: ADDRESS BUS bit4 */
  /* PF5: ADDRESS BUS bit5 */
  /* PF6: INT2 */
  /* PF7: INT3 */
  /* PF8: INT4 */
  /* PF9: INT5 */
  /* PF10: INT6 */
  /* PF11: C-BUS ENABLE */
  /* PF12: ADDRESS BUS bit6 */
  /* PF13: ADDRESS BUS bit7 */
  /* PF14: ADDRESS BUS bit8 */
  /* PF15: ADDRESS BUS bit9 */

  GPIOG->ODR = 0x0000;
  /* PG0: ADDRESS BUS bit10 */
  /* PG1: ADDRESS BUS bit11 */
  /* PG2: ADDRESS BUS bit12 */
  /* PG3: ADDRESS BUS bit13 */
  /* PG4: ADDRESS BUS bit14 */
  /* PG5: ADDRESS BUS bit15 */
  /* PG6: not connected. */
  /* PG7: not connected. */
  /* PG8: USART3 DTR */
  /* PG9: ETHERNET CS */
  /* PG10: ARCNET CS */
  /* PG11: not connected. */
  /* PG12: C-BUS CS */
  /* PG13: not connected. */
  /* PG14: not connected. */
  /* PG15: EX I2C INT */

  pinMode( LED_BUILTIN, OUTPUT );
  pinMode( ETH_RST, OUTPUT ); digitalWrite( ETH_RST, LOW );
  pinMode( ARC_RST, OUTPUT ); digitalWrite( ARC_RST, LOW );
  pinMode( CBUS_RST, OUTPUT ); digitalWrite( CBUS_RST, LOW );
  pinMode( CBUS_ENA, OUTPUT ); digitalWrite( CBUS_ENA, LOW );
  pinMode( EXT_INT0, INPUT_PULLUP );
  pinMode( EXT_INT1, INPUT_PULLUP );
  pinMode( EXT_INT2, INPUT_PULLUP );
  pinMode( EXT_INT3, INPUT_PULLUP );
  pinMode( EXT_INT4, INPUT_PULLUP );
  pinMode( EXT_INT5, INPUT_PULLUP );
  pinMode( EXT_INT6, INPUT_PULLUP );
}

/**
  * @brief 1405 interrupt call back functions.
  * @retval none.
  */
void cb1405_ID00( void ) { bd1405[0].intHandler(); }
void cb1405_ID02( void ) { bd1405[1].intHandler(); }
void cb1405_ID04( void ) { bd1405[2].intHandler(); }
void cb1405_ID06( void ) { bd1405[3].intHandler(); }
void cb1405_ID08( void ) { bd1405[4].intHandler(); }
void cb1405_ID0A( void ) { bd1405[5].intHandler(); }
void cb1405_ID0C( void ) { bd1405[6].intHandler(); }

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void user_Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
