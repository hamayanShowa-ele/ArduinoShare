/*
  W5500 RAW MODE librarly test code.

  Copyright (C) 2014 hamayan All Rights Reserved.
*/

#include <SPI.h>
#include <Wire.h>
#include <Ethernet2.h>
#include <rom24aa025.h>  // https://github.com/hamayanShowa-ele/ArduinoShare/tree/main/libraries/rom24aa025
#include <STM32F_CPU_Identity.h>  // https://github.com/hamayanShowa-ele/ArduinoShare/tree/main/libraries/STM32F_CPU_Identity
#include <WIZCHIP_RAW_MODE_SPI.h>  // https://github.com/hamayanShowa-ele/ArduinoShare/tree/main/libraries/WIZCHIP_RAW_MODE_SPI

/*
  Board types.
*/
#define  BOARD_TYPE_2102_F4
//#define  BOARD_TYPE_2102_F1
//#define  BOARD_TYPE_1303_F1
//#define  BOARD_TYPE_MB-STM32F103_F1

/*
  GPIO pin defines.
*/
#if  defined( BOARD_TYPE_2102_F1 )  // 2102 STM32F103RET
  #undef  LED_BUILTIN
  #define  LED_BUILTIN  PB4
  #define  ACT_LED    PB4
  #define  RED_LED    PB3
  #define  DIO0       PB9
  #define  DIO1       PC13
  #define  DIO2       PC14
  #define  DIO3       PC15
  #define  IN00       PC0
  #define  IN01       PC1
  #define  IN02       PC2
  #define  IN03       PC3
  #define  IN04       PA0
  #define  IN05       PA1
  #define  IN06       PA2
  #define  IN07       PA3
  #define  OUT00      PA4
  #define  OUT01      PA5
  #define  OUT02      PA6
  #define  OUT03      PA7
  #define  OUT04      PC4
  #define  OUT05      PC5
  #define  OUT06      PB0
  #define  OUT07      PB1
  #define  W5500_CS   PB12
  #define  SPI2_SCK   PB13
  #define  SPI2_MISO  PB14
  #define  SPI2_MOSI  PB15
  #define  W5500_INT  PC6
  #define  W5500_RST  PC7
  #define  USB_POWER_ON PA8
  #define  USB_POWER_ON_LEVEL HIGH
  #define  TXD1       PA9
  #define  RXD1       PA10
  #define  USBDM      PA11
  #define  USBDP      PA12
  #define  RTS4       PA15
  #define  TXD4       PC10
  #define  RXD4       PC11
  #define  LCD_RES    PB5
  #define  SCL1       PB6
  #define  SDA1       PB7
#elif  defined( BOARD_TYPE_2102_F4 )  // STM32F401RCT
  #undef  LED_BUILTIN
  #define  LED_BUILTIN  PB4
  #define  ACT_LED    PB4
  #define  RED_LED    PB3
  #define  DIO0       PB9
  #define  DIO1       PC13
  #define  DIO2       PC14
  #define  DIO3       PC15
  #define  IN00       PC0
  #define  IN01       PC1
  #define  IN02       PC2
  #define  IN03       PC3
  #define  IN04       PA0
  #define  IN05       PA1
  #define  IN06       PA2
  #define  IN07       PA3
  #define  OUT00      PA4
  #define  OUT01      PA5
  #define  OUT02      PA6
  #define  OUT03      PA7
  #define  OUT04      PC4
  #define  OUT05      PC5
  #define  OUT06      PB0
  #define  OUT07      PB1
  #define  W5500_CS   PB12
  #define  SPI2_SCK   PB13
  #define  SPI2_MISO  PB14
  #define  SPI2_MOSI  PB15
  #define  TXD6       PC6
  #define  RXD6       PC7
  #define  RTS6       PC8
  #define  W5500_INT  PC9
  #define  W5500_RST  PA8
  #define  TXD1       PA9
  #define  RXD1       PA10
  #define  USBDM      PA11
  #define  USBDP      PA12
  #define  USB_POWER_ON PA15
  #define  USB_POWER_ON_LEVEL HIGH
  #define  LCD_RES    PB5
  #define  SCL1       PB6
  #define  SDA1       PB7
#elif  defined( BOARD_TYPE_1303_F1 )  // 1303 PIXIS
  #undef  LED_BUILTIN
  #define  LED_BUILTIN  PB7
  #define  USB_POWER_ON PE1
  #define  USB_POWER_ON_LEVEL HIGH
#elif  defined( BOARD_TYPE_MB-STM32F103_F1 )  // MB-STM32F103
  #undef  LED_BUILTIN
  #define  LED_BUILTIN  PB7
  #define  USB_POWER_ON PC13
  #define  USB_POWER_ON_LEVEL LOW
#endif  /* defined( BOARD_TYPE_2102_F1 ) */

/*
  instance and global variables.
*/
STM32F_CPU_IDENTITY cpu_id;
WIZCHIP_RAW_MODE wizchip( &SPI, W5500_CS );
TwoWire i2c1;
EEPROM_24AA025 eeprom( &i2c1 );

uint8_t mac[6];
static const uint8_t bdcast_mac[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
static const uint8_t bdcast_ip[] = {0xFF,0xFF,0xFF,0xFF};
const uint8_t sip[4] = {192,168,1,177};  // IPAddress myIP(192, 168, 1, 177);;
const uint8_t dnsip[4] = {192,168,1,1};  // IPAddress dnsIP(192, 168, 1, 1);;
const uint8_t gwip[4] = {192,168,1,1};  // IPAddress gwIP(192, 168, 1, 1);;
const uint8_t mask[4] = {255,255,255,0};  // IPAddress snMask(255, 255, 255, 0);;

volatile int w5500InterruptCount;

/*
  setup.
*/
void setup()
{
  /* gpio initialize. */
  pinMode( LED_BUILTIN, OUTPUT );
  digitalWrite( LED_BUILTIN, LOW );
  // initialize usb power on pin USB_POWER_ON as an output.
  pinMode( USB_POWER_ON, OUTPUT );
  digitalWrite( USB_POWER_ON, USB_POWER_ON_LEVEL );

  // initialize usb CDC class.
  Serial.begin( 115200UL );
  while( !Serial ) { delay( 20UL ); }
  Serial.println();
  Serial.println( "Board 2102 start up message from USB serial." );
  Serial.println( "W5500 RAW PACKET example." );
  Serial.println();
  String msg = "systic:"; msg += String( cpu_id.sysload() );
  msg += " hclk:"; msg += String( cpu_id.hclk() );
  msg += " pclk1:"; msg += String( cpu_id.pclk1() );
  msg += " pclk2:"; msg += String( cpu_id.pclk2() );
  msg += " sysclock:"; msg += String( cpu_id.sysclk() );
  Serial.println( msg );
  Serial.println();

  // I2C1 initialize.
  i2c1.setSDA( SDA1 );  /* SDA */
  i2c1.setSCL( SCL1 );  /* SCL */
  i2c1.begin();

  // read the mac address from the i2c eeprom.
  eeprom.read( MAC_ADDRESS_IN_24AA025E48, mac, (int)sizeof(mac) );
  Serial.printf( "mac %02X:%02X:%02X:%02X:%02X:%02X\r\n",
    mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]  );

  // reset W5500.
  pinMode( W5500_RST, OUTPUT );
  digitalWrite( W5500_RST, LOW );
  delay( 2UL );
  digitalWrite( W5500_RST, HIGH );
  delay( 500UL );

  // SPI connected with w5500 initialize.
  wizchip.setPclk( cpu_id.pclk1() );
  SPI.setMISO( SPI2_MISO );
  SPI.setMOSI( SPI2_MOSI );
  SPI.setSCLK( SPI2_SCK );
  SPI.begin();

  static const uint8_t TxRxBufferSize[2][MAX_SOCK_NUM] =  // w5500 tx and rx buffer size
  {
    {2,2,2,2,2,2,2,2,},  // Tx buffer size
    {2,2,2,2,2,2,2,2,},  // Rx buffer size
//    {4,4,4,4,0,0,0,0,},  // Tx buffer size
//    {4,4,4,4,0,0,0,0,},  // Rx buffer size
//    {8,8,0,0,0,0,0,0,},  // Tx buffer size
//    {8,8,0,0,0,0,0,0,},  // Rx buffer size
//    {16,0,0,0,0,0,0,0,},  // Tx buffer size
//    {16,0,0,0,0,0,0,0,},  // Rx buffer size
  };
  if( wizchip.initW5500( TxRxBufferSize ) ) { IamStuck( 50UL ); }

  /* print chip version. */
  Serial.printf( "chip version : %02X\r\n", wizchip.getVERSIONR() );
  /* print link status. */
  Serial.println( wizchip.linkMessage() );

  /* setup and print source hardware address */
  uint8_t rmac[6];
  wizchip.setSHAR( (const uint8_t *)mac );
  wizchip.getSHAR( rmac );
  Serial.printf( "source mac address : %02X:%02X:%02X:%02X:%02X:%02X\r\n",
    rmac[0],rmac[1],rmac[2],rmac[3],rmac[4],rmac[5] );

  uint8_t rip[4];
  /* setup and print source ip */
  wizchip.setSIPR( sip );
  wizchip.getSIPR( rip );
  Serial.printf( "source address : %d.%d.%d.%d\r\n", rip[0],rip[1],rip[2],rip[3] );
  /* setup and print gateway ip */
  wizchip.setGAR( gwip );
  wizchip.getGAR( rip );
  Serial.printf( "gateway address : %d.%d.%d.%d\r\n", rip[0],rip[1],rip[2],rip[3] );
  /* setup and print subnet mask */
  wizchip.setSUBR( mask );
  wizchip.getSUBR( rip );
  Serial.printf( "subnet mask : %d.%d.%d.%d\r\n", rip[0],rip[1],rip[2],rip[3] );

  /* socket 0 open. */
  uint8_t s0MR = wizchip.getSnMR( SOCKET_0 );
  s0MR &= ~(Sn_MR_PROTOCOL_MASK_bit);
  s0MR |= Sn_MR_PROTOCOL_MACRAW_bit | Sn_MR_MULTI_MFEN_bit
       | Sn_MR_ND_MC_MMB_bit | Sn_MR_UCASTB_MIP6B_bit;
  wizchip.setSnMR( SOCKET_0, s0MR );
  wizchip.SnClose( SOCKET_0 );
  wizchip.SnOpen( SOCKET_0 );
  /* print socket 0 tx,rx buffer size. */
  uint8_t s0txSize = wizchip.getSnTxBufSize( SOCKET_0 );
  uint8_t s0rxSize = wizchip.getSnRxBufSize( SOCKET_0 );
  Serial.printf( "tx buf = %d rx buf = %d\r\n", s0txSize, s0rxSize );
  /* print socket 0 tx fsr size,rx rsr size. */
  uint16_t s0TxFree = wizchip.getSnTxFSR( SOCKET_0 );
  uint16_t s0RxRemain = wizchip.getSnRxRSR( SOCKET_0 );
  Serial.printf( "tx free = %d rx remain = %d\r\n", s0TxFree, s0RxRemain );
  /* print socket 0 tx,rx read/write pointers. */
  uint16_t s0txRD = wizchip.getSnTxRD( SOCKET_0 );
  uint16_t s0txWR = wizchip.getSnTxWR( SOCKET_0 );
  uint16_t s0rxRD = wizchip.getSnRxRD( SOCKET_0 );
  uint16_t s0rxWR = wizchip.getSnRxWR( SOCKET_0 );
  Serial.printf( "tx rd,wr %d,%d rx rd,wr %d,%d\r\n", s0txRD, s0txWR, s0rxRD, s0rxWR );
  /* print socket 0 interrupt mask bit. */
  uint8_t s0IMR = wizchip.getSnIMR( SOCKET_0 );
  Serial.printf( "imr : %02X\r\n", s0IMR );
  /* print socket 0 status register. */
  uint8_t s0Status = wizchip.getSnSR( SOCKET_0 );
  Serial.print( "status : "); Serial.println( wizchip.socketStatusMessage( s0Status ) );
  /* setup and print source port number */
  wizchip.setSnSPORT( SOCKET_0, 5000 );
  Serial.printf( "source port : %d\r\n", wizchip.getSnSPORT( SOCKET_0 ) );

  uint8_t testData[64];
  for( int i = 0; i < (int)sizeof(testData); i++ ) { testData[i] = (uint8_t)i; }
  Serial.println( "packet transmitting." );
  wizchip.writeEther(
    SOCKET_0, bdcast_mac, ETHER2_TYPE_TEST01,
    (const uint8_t *)testData, (int)sizeof(testData) );
  Serial.println( "packet transmitted." );

  /* print socket 0 tx,rx read/write pointers. */
  s0txRD = wizchip.getSnTxRD( SOCKET_0 );
  s0txWR = wizchip.getSnTxWR( SOCKET_0 );
  s0rxRD = wizchip.getSnRxRD( SOCKET_0 );
  s0rxWR = wizchip.getSnRxWR( SOCKET_0 );
  Serial.printf( "tx rd,wr %d,%d rx rd,wr %d,%d\r\n", s0txRD, s0txWR, s0rxRD, s0rxWR );


  /* attach interrupt */
//  attachInterrupt( W5500_INT, wizchipInterrupt, FALLING );  //enable interrupt for w5500 interrupt.
//  wizchip.setIMR( (uint8_t)(COMMON_IMR_CONFLICT_bit | COMMON_IMR_UNREACH_bit) );
//  wizchip.setSIMR( (uint8_t)COMMON_SIR_SOCKET0_bit );
}

/*
  loop.
*/
void loop()
{
  static uint32_t ledBlinkTiming = 0UL;
  static int w5500InterruptCountBase = 0;

  uint8_t ir = wizchip.getSnIR( SOCKET_0 );
  if( ir & Sn_IR_SEND_OK_bit )
  {
    wizchip.setSnIR( SOCKET_0, Sn_IR_SEND_OK_bit );
  }

  uint16_t len = wizchip.snRxAvailable( SOCKET_0 );
  if( len > (int)sizeof(uint16_t) )
  {
    wizchip.setSnIR( SOCKET_0, Sn_IR_RECV_bit );

    len -= sizeof(uint16_t);
    if( len > 14 )
    {
      len -= 14;
      uint8_t *buf = new uint8_t[len];
      uint8_t dst[6];
      uint16_t typ;
      wizchip.readEther( SOCKET_0, dst, &typ, buf, len );
      Serial.printf( "%02X:%02X:%02X:%02X:%02X:%02X TYPE:%04X\r\n",
        dst[0],dst[1],dst[2],dst[3],dst[4],dst[5],typ );
      dump( buf, len );
      delete [] buf;
    }
    uint16_t rxrd = wizchip.getSnRxRD( SOCKET_0 );
    uint16_t rxwr = wizchip.getSnRxWR( SOCKET_0 );
    Serial.printf( "rx rd,wr %d,%d\r\n", rxrd, rxwr );


    while( false ) delay( 1000 );
  }

#if 0
  if( w5500InterruptCount != w5500InterruptCountBase )
  {
//    w5500InterruptCountBase = w5500InterruptCount;
    w5500InterruptCountBase++;
    wizchipISR();
  }
#endif

  if( (millis() - ledBlinkTiming) >= 500UL )
  {
    ledBlinkTiming = millis();
    digitalWrite( LED_BUILTIN, (digitalRead( LED_BUILTIN ) == HIGH) ? LOW : HIGH );
  }
}

#if 0
/*
  interrupt service routine.
*/
void wizchipISR()
{
  uint8_t ir = wizchip.getIR();
  uint8_t sir = wizchip.getSIR();
  wizchip.setIR( ir );

  if( ir & COMMON_IR_CONFLICT_bit )
  {
    Serial.println( "ip address conflict!" );
  }

  if( ir & COMMON_IR_UNREACH_bit )
  {
    uint8_t uip[4];
    wizchip.getUIPR( uip );
    uint16_t uport = wizchip.getUPORTR();
    Serial.printf( "unreachable : %d.%d.%d.%d : %d\r\n",
      uip[0],uip[1],uip[2],uip[3], uport );
  }

  if( ir & COMMON_IR_PPPoE_bit ) {}
  if( ir & COMMON_IR_MP_bit ) {}

  if( sir )
  {
    uint8_t bit = COMMON_SIR_SOCKET0_bit;
    for( int i = 0; i < 8; i++ )
    {
      bit <<= 1;
    }
  }
}

/*
  interrupt handlers.
*/
void wizchipInterrupt()
{
  w5500InterruptCount++;
}
#endif

/*
  blink led infinity loop.
*/
void IamStuck( uint32_t dly )
{
  while( true )
  {
    delay( dly );
    digitalWrite( LED_BUILTIN, (digitalRead( LED_BUILTIN ) == HIGH) ? LOW : HIGH );
  }
}

