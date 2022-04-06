/*
  Arduino IDE setup.
  Board type : Ncleo-64. NUCLEO-F446RE
  U(S)ART support : Enable (No generic Serial).
  USB support : CDC (generic Serial supersede U(S)ART).
  USB speed : Low/Full Speed.
  Optimize : ANY.
  C Runtime Library : ANY.
  Update method : STM32CubeProgrammer (SWD).
  Serial port : ANY.

  NUCLEO-F446RE setup.
    R34,R36 : OPEN
    SB50 : OPEN
    SB48,SB49,SB55 : SHORT

  Configurable baud rate : 4800,9600,19200,38400,57600,
                           115200,230400,460800,921600bps
*/
/**
  * @brief Add the following definition to the beginning of HardwareSerial.h in advance.
          #define SERIAL_TX_BUFFER_SIZE 256
          #define SERIAL_RX_BUFFER_SIZE 256
  */
#include  <HardwareSerial.h>
#include  <SPI.h>
#include  <Ethernet2.h>  // https://www.arduino.cc/reference/en/libraries/ethernet2/
#include  <Adafruit_SSD1306.h>  // https://github.com/adafruit/Adafruit_SSD1306
#include  <Adafruit_GFX.h>  // https://github.com/adafruit/Adafruit-GFX-Library
#include  <splash.h>  / https://github.com/adafruit/Adafruit_SSD1306
#include  <STM32F_CPU_Identity.h>  // https://github.com/hamayanShowa-ele/ArduinoShare/tree/main/libraries/STM32F_CPU_Identity
#include  <mul_tsk.h>  // https://github.com/hamayanShowa-ele/ArduinoShare/tree/main/libraries/mul_tsk
#include  <MY_STREAM.h>  // https://github.com/hamayanShowa-ele/ArduinoShare/tree/main/libraries/MY_STREAM
#include  <rom24aa025.h>  // https://github.com/hamayanShowa-ele/ArduinoShare/tree/main/libraries/rom24aa025

enum TSK_ID_NUMBERS
{
  ID_server1_Task,
  ID_server2_Task,
  ID_server3_Task,
  ID_server4_Task,
  ID_server5_Task,
  ID_server6_Task,
  ID_pcIF_Task,
  ID_stack_monitor,
  TSKID_END,
};

/**
  * @brief defines.
  */
#define  USBDM  PA11
#define  USBDP  PA12
#define  USBON  PC5
#define  USB_POWER_ON_LEVEL HIGH

#define  OLED_RESET   PB2 // Reset pin # (or -1 if sharing Arduino reset pin)

#define  TXD1  PA_9
#define  RXD1  PA_10
#define  TXD2  PA_2
#define  RXD2  PA_3
#define  TXD3  PC_10_ALT1  // PC10
#define  RXD3  PC_11_ALT1  // PC11
#define  TXD4  PA_0
#define  RXD4  PA_1
#define  TXD5  PC_12
#define  RXD5  PD_2
#define  TXD6  PC_6
#define  RXD6  PC_7

#define  SDA1  PB9
#define  SCL1  PB8

#define  SPI2_SCK   PB13
#define  SPI2_MISO  PB14
#define  SPI2_MOSI  PB15

#define  SD_CD  PB1
#define  SD_CS  PB12

#define  SW_IN1   PA15
#define  SW_IN2   PB7
#define  BR_SEL0  PC13
#define  BR_SEL1  PC14
#define  BR_SEL2  PC15
#define  CH_SEL0  PH0
#define  CH_SEL1  PH1
#define  CH_SEL2  PC2

#define  ACTLED  PC3
#define  REDLED  PC4

#define  TIM3_CH3  PC8
#define  TIM8_CH4  PC9
#define  BEEP1  TIM3_CH3
#define  BEEP2  TIM8_CH4

//#define  TELNET  23  // telnet port number.
#define  TELNET  5001  // telnet port number.
#define  W5500_RST  PA4  // A2
#define  W5500_MOSI PA7  // 11
#define  W5500_MISO PA6  // 12
#define  W5500_SCK  PA5  // 13
#define  W5500_SSEL PB6  // 10

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an stm32 nucleo64:    PB9(SDA), PB8(SCL)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define NUMFLAKES     10 // Number of snowflakes in the animation example
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

/**
  * @brief prototypes.
  */

void tsk_ini( void );
void stackMonitor( void );
void pcIF_Task( void );
void server_Task( void );
static void oledPrint( String msg, int x = 0, int y = 0 );
static void oledPrint( const char *msg, int x = 0, int y = 0 );
static void seriOutput( const char *str );
static void transparentsSerial( HardwareSerial *ser );
static void initSSD1306();
static void initGPIO();
static void getMACaddress( uint8_t *adr );
static uint8_t chSelectorRead();
static uint8_t brSelectorRead();

/**
  * @brief instance and global variables.
  */
String ip_port_msg;
STM32F_CPU_IDENTITY cpu_id;

HardwareSerial Serial1( RXD1, TXD1 );  // rxd pin, txd pin
HardwareSerial Serial2( RXD2, TXD2 );  // rxd pin, txd pin
HardwareSerial Serial3( RXD3, TXD3 );  // rxd pin, txd pin
HardwareSerial Serial4( RXD4, TXD4 );  // rxd pin, txd pin
HardwareSerial Serial5( RXD5, TXD5 );  // rxd pin, txd pin
HardwareSerial Serial6( RXD6, TXD6 );  // rxd pin, txd pin

// Enter a MAC address for your controller below.
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 };  // 0xED
// If you do not obtain an IP address via DHCP, prepare a fixed IP address as follows.
IPAddress constantIP(192, 168, 100, 250);
IPAddress myIP;

TwoWire i2c1;
//Adafruit_SSD1306 oled( SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET );
Adafruit_SSD1306 oled( SCREEN_WIDTH, SCREEN_HEIGHT, &i2c1, OLED_RESET );
EEPROM_24AA025  eeprom( &i2c1 );

MY_STREAM pcIF_tx_Fifo(256);
MY_STREAM pcIF_rx_Fifo(256);
static uint32_t  baudRate[6] =
{
  38400UL,  // 1
  115200UL,  // 2
  38400UL,  // 3
  38400UL,  // 4
  38400UL,  // 5
  38400UL,  // 6
};

static const uint32_t configurable_baud_rate[] =
{
  4800UL,  // 0
  9600UL,  // 1
  19200UL,  // 2
  38400UL,  // 3
  57600UL,  // 4
  115200UL,  // 5
  230400UL,  // 6
  460800UL,  // 7
//  921600UL,  // 8
};


static uint8_t tsk0_stk[ 128 * 8 ];
static uint8_t tsk1_stk[ 128 * 8 + 1536 ];
static uint8_t tsk2_stk[ 128 * 8 + 1536 ];
static uint8_t tsk3_stk[ 128 * 8 + 1536 ];
static uint8_t tsk4_stk[ 128 * 8 + 1536 ];
static uint8_t tsk5_stk[ 128 * 8 + 1536 ];
static uint8_t tsk6_stk[ 128 * 8 + 1536 ];
static uint8_t tsk7_stk[ 128 * 5 ];

/**
  * @brief task initialize.
  * @retval none.
  */
void tsk_ini( void )
{
  cfg_sys( TSKID_END );

  reg_tsk( ID_stack_monitor, (void *)stackMonitor, (void *)tsk0_stk, sizeof(tsk0_stk), 0,0,0,0 );
  reg_tsk( ID_server1_Task, (void *)server_Task, (void *)tsk1_stk, sizeof(tsk1_stk),
    TELNET + 0, (VP_INT)&Serial1, 1, 38400UL );
  reg_tsk( ID_server2_Task, (void *)server_Task, (void *)tsk2_stk, sizeof(tsk2_stk),
    TELNET + 1, (VP_INT)&Serial2, 2, 115200UL );
  reg_tsk( ID_server3_Task, (void *)server_Task, (void *)tsk3_stk, sizeof(tsk3_stk),
    TELNET + 2, (VP_INT)&Serial3, 3, 38400UL );
  reg_tsk( ID_server4_Task, (void *)server_Task, (void *)tsk4_stk, sizeof(tsk4_stk),
    TELNET + 3, (VP_INT)&Serial4, 4, 38400UL );
  reg_tsk( ID_server5_Task, (void *)server_Task, (void *)tsk5_stk, sizeof(tsk5_stk),
    TELNET + 4, (VP_INT)&Serial5, 5, 38400UL );
  reg_tsk( ID_server6_Task, (void *)server_Task, (void *)tsk6_stk, sizeof(tsk6_stk),
    TELNET + 5, (VP_INT)&Serial6, 6, 38400UL );
  reg_tsk( ID_pcIF_Task, (void *)pcIF_Task, (void *)tsk7_stk, sizeof(tsk7_stk), 0,0,0,0 );

  sta_tsk( ID_stack_monitor );
}

/**
  * @brief setup.
  * @retval none.
  */
void setup()
{
  // GPIO initialize.
  initGPIO();

  // USB/Serial initialize.
  Serial.begin( 115200UL );
//  while( !Serial ) { delay( 20 ); }
  delay( 20 );
  Serial.println( "STM32 Serial/Ethernet converter." );
  delay( 100UL );

  // get cpu id.
  Serial.println( cpu_id.sysClockString() );
  Serial.println( cpu_id.uIDtring() );

  // I2C1 initialize.
  i2c1.setSDA( SDA1 );  /* SDA */
  i2c1.setSCL( SCL1 );  /* SCL */
  i2c1.begin();

  // SSD1306 initialize.
  initSSD1306();

  // read the mac address from the i2c eeprom.
  getMACaddress( mac );

  // start the Ethernet connection:
  Ethernet.init( W5500_SSEL );
  if( Ethernet.begin(mac) == 0 )
  {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, constantIP);
  }
  myIP = Ethernet.localIP();
  ip_port_msg = "";
  ip_port_msg += String( myIP[0], DEC ) + "." + String( myIP[1], DEC ) + "."
    + String( myIP[2], DEC ) + "." + String( myIP[3], DEC ) + ":" + String( TELNET, DEC );
  Serial.println( ip_port_msg );
  oledPrint( ip_port_msg, 0, 0 );

  tsk_ini();  // Initialize Tasks
  sta_rdq( ID_stack_monitor );  // Round robin starts. The task starts here.

  /* Infinite loop */
  while(1) {}
}

/**
  * @brief loop.
  * @retval none.
  */
void loop() {}

/**
  * @brief stack monitor task.
  * @retval none.
  */
static uint32_t RemainStack( void *stk, uint32_t sz )
{
  uint32_t i;
  char *ptr = (char *)stk;
  for( i = 0; i < sz; i++ ) { if( *ptr++ != 0 ) break; }
  return sz - i;
}

void stackMonitor( void )
{
  sta_tsk( ID_pcIF_Task );
  sta_tsk( ID_server1_Task );
  sta_tsk( ID_server2_Task );
  sta_tsk( ID_server3_Task );
  sta_tsk( ID_server4_Task );
  sta_tsk( ID_server5_Task );
  sta_tsk( ID_server6_Task );

  uint8_t brrBit = 5;  // deafult 115200bps
  uint8_t chBit = 2;  // default Serial2
  uint8_t sw4_state = 0;
  uint32_t reportTim = millis();
  uint32_t swScanTim = millis();
  uint32_t blinkTim = millis();
  HardwareSerial *ser;
  while( 1 )
  {
    /*stack report.*/
    if( (millis() - reportTim) >= 5 * 1000UL )
    {
      reportTim = millis();
#if 1
      char msg[64];
      sprintf( msg, "  SERVER1:%d/%d", RemainStack( tsk1_stk, sizeof(tsk1_stk) ), sizeof(tsk1_stk) );
      seriOutput( (const char *)msg );

      sprintf( msg, "  SERVER2:%d/%d", RemainStack( tsk2_stk, sizeof(tsk2_stk) ), sizeof(tsk2_stk) );
      seriOutput( (const char *)msg );

      sprintf( msg, "  SERVER3:%d/%d", RemainStack( tsk3_stk, sizeof(tsk3_stk) ), sizeof(tsk3_stk) );
      seriOutput( (const char *)msg );

      sprintf( msg, "  SERVER4:%d/%d", RemainStack( tsk4_stk, sizeof(tsk4_stk) ), sizeof(tsk4_stk) );
      seriOutput( (const char *)msg );

      sprintf( msg, "  SERVER5:%d/%d", RemainStack( tsk5_stk, sizeof(tsk5_stk) ), sizeof(tsk5_stk) );
      seriOutput( (const char *)msg );

      sprintf( msg, "  SERVER6:%d/%d", RemainStack( tsk6_stk, sizeof(tsk6_stk) ), sizeof(tsk6_stk) );
      seriOutput( (const char *)msg );

      sprintf( msg, "  PC IF:%d/%d", RemainStack( tsk7_stk, sizeof(tsk7_stk) ), sizeof(tsk7_stk) );
      seriOutput( (const char *)msg );

      sprintf( msg, "  MONITOR:%d/%d", RemainStack( tsk0_stk, sizeof(tsk0_stk) ), sizeof(tsk0_stk) );
      seriOutput( (const char *)msg );

      seriOutput( "" );
#endif
    }

    /*switch scan.*/
    if( (millis() - swScanTim) >= 50UL )
    {
      swScanTim = millis();
      // chack SW4(setup switch).
      sw4_state <<= 1;
      sw4_state |= ( digitalRead(SW_IN2) == LOW ) ? 0x01 : 0x00;
      if( (sw4_state & 0x07) == 0x03 )  // When the switch is pressed.
      {
        uint8_t chBitNew = chSelectorRead();  // 1 or 2 or 3 or 4 or 5 or 6
        if( chBitNew < 1 || chBitNew > 6 ) { chBitNew = 2; }
        if( chBitNew != chBit )
        {
          chBit = chBitNew;
          switch( chBit )
          {
            case 1 : ser = &Serial1; break;
            case 3 : ser = &Serial3; break;
            case 4 : ser = &Serial4; break;
            case 5 : ser = &Serial5; break;
            case 6 : ser = &Serial6; break;
            default :
              ser = &Serial2;
              chBit = 2;
              break;
          }
        }
        uint8_t brrBitNew = brSelectorRead() >> 0;  // 9600 or 19200 or 38400 or 115200bps and any more.
        if( brrBitNew != brrBit )  // When there is a change in baud rate.
        {
          brrBit = brrBitNew;
        }
        uint32_t brr = configurable_baud_rate[ brrBit ];
        baudRate[ chBit - 1 ] = brr;
        // text display
        oled.clearDisplay();
        oledPrint( ip_port_msg, 0, 0 );
        String msg = "Select Serial = " + String( chBit, DEC ) + ".";
        oledPrint( msg, 0, 8 );
        msg = "baud = " + String( brr, DEC ) + "bps.";
        oledPrint( msg, 0, 16 );
        // Change of baud rate.
        ser->end();
        ser->begin( brr );
        while( !*ser ) { dly_tsk( 5UL ); }
      }
    }

    /*led blink.*/
    if( (millis() - blinkTim) >= 500UL )
    {
      blinkTim = millis();
      digitalWrite( ACTLED, (digitalRead(ACTLED) == LOW) ? HIGH : LOW );
    }
    rot_rdq();  // round robin.
  }
}

/**
  * @brief serial ethernet interface task.
  * @retval none.
  */
void server_Task( void )
{
  // Get startup arguments.
  VP_INT exinf[4];
  get_par( exinf, sizeof(exinf) / sizeof(exinf[0]) );
  uint16_t portNumber = exinf[0];  // server port number
  HardwareSerial *ser = (HardwareSerial *)exinf[1];  // serial interface
  int serialNumber = exinf[2];  // serial interface number.
  uint32_t baud = exinf[3];  // baud rate

  ser->begin( baud );
  while( !*ser ) { dly_tsk( 20UL ); }

#if 0
  char buf[64];
  sprintf( buf, "server port=%u serial%d baud=%u", portNumber, serialNumber, baud );
  seriOutput( (const char *)buf );
#endif
  uint8_t rcv[ 1536 ];
  while( 1 )
  {
    EthernetServer server( portNumber );
    // listen and accept
    server.begin();
    while( 1 )
    {
      int rcvCount;
      EthernetClient client = server.available();
      if( client )
      {
        rcvCount = client.available();
        rcvCount = ( rcvCount < ser->availableForWrite() ) ? rcvCount : ser->availableForWrite();
        for( int i = 0; i < rcvCount; i++ ) { rcv[ i ] = client.read(); /* rot_rdq(); */ }
        ser->write( (const uint8_t *)rcv, rcvCount );
      }
      rcvCount = ser->available();
      if( rcvCount )
      {
        for( int i = 0; i < rcvCount; i++ ) { rcv[ i ] = ser->read(); /* rot_rdq(); */ }
        server.write( (const uint8_t *)rcv, rcvCount );
      }
      rot_rdq();
    }
    rot_rdq();
  }
}

/**
  * @brief usb/serial interface task.
  * @retval none.
  */
void pcIF_Task( void )
{
  while( 1 )
  {
    int c;
    c = pcIF_tx_Fifo.read();
    if( c >= 0 )
    {
      while( Serial.availableForWrite() == 0 ) { rot_rdq(); }
      Serial.write( c );
    }
    if( Serial.available() )
    {
      c = Serial.read();
      pcIF_rx_Fifo.write( (uint8_t)c );
    }
    rot_rdq();
  }
}

/**
  * @brief OLED Messae.
  * @retval none.
  */
static void oledPrint( String msg, int x, int y )
{
  oled.setCursor( x, y );
  oled.print( msg );
  oled.display(); // actually display all of the above
}

static void oledPrint( const char *msg, int x, int y )
{
  oled.setCursor( x, y );
  oled.print( msg );
  oled.display(); // actually display all of the above
}

/**
  * @brief serial output by USB/Serial.
  * @retval none.
  */
static void seriOutput( const char *str )
{
  while( *str ) { pcIF_tx_Fifo.write( *str++ ); }
  pcIF_tx_Fifo.write( '\r' );
  pcIF_tx_Fifo.write( '\n' );
}

/**
  * @brief read the baud rate selector.
  * @retval read value.
  */
static uint8_t brSelectorRead()
{
  uint8_t tempUC = 0;
  if( digitalRead(BR_SEL0) == HIGH ) tempUC |= 0x01;  // BR_SEL0:PC13
  if( digitalRead(BR_SEL1) == HIGH ) tempUC |= 0x02;  // BR_SEL1:PC14
  if( digitalRead(BR_SEL2) == HIGH ) tempUC |= 0x04;  // BR_SEL2:PC15
  return tempUC;
}

/**
  * @brief read the channel selector.
  * @retval read value.
  */
static uint8_t chSelectorRead()
{
  uint8_t tempUC = 0;
  if( digitalRead(CH_SEL0) == HIGH ) tempUC |= 0x01;  // CH_SEL0:PH0
  if( digitalRead(CH_SEL1) == HIGH ) tempUC |= 0x02;  // CH_SEL1:PH1
  if( digitalRead(CH_SEL2) == HIGH ) tempUC |= 0x04;  // CH_SEL2:PC2
  return tempUC;
}

/**
  * @brief transparents serial converter.
  * @retval none.
  */
static void transparentsSerial( HardwareSerial *ser )
{
  while( 1 )
  {
    if( ser->available() )
    {
      int c = ser->read();
      Serial.write( c );
    }
    if( Serial.available() )
    {
      int c = Serial.read();
      ser->write( c );
    }
  }
}

/**
  * @brief GPIO initialize.
  * @retval none.
  */
static void initGPIO()
{
  pinMode( LED_BUILTIN, OUTPUT );
  pinMode( ACTLED, OUTPUT );
  pinMode( REDLED, OUTPUT );
  digitalWrite( LED_BUILTIN, HIGH );
  digitalWrite( ACTLED, HIGH );
  digitalWrite( REDLED, LOW );
  pinMode( SW_IN1, INPUT_PULLUP );
  pinMode( SW_IN2, INPUT_PULLUP );
  pinMode( BR_SEL0, INPUT_PULLUP );
  pinMode( BR_SEL1, INPUT_PULLUP );
  pinMode( BR_SEL2, INPUT_PULLUP );
  pinMode( CH_SEL0, INPUT_PULLUP );
  pinMode( CH_SEL1, INPUT_PULLUP );
  pinMode( CH_SEL2, INPUT_PULLUP );
  // initialize usb power on pin USBON as an output.
  pinMode( USBON, OUTPUT );
  digitalWrite( USBON, USB_POWER_ON_LEVEL );
  // setup SPI for the W5500.
  pinMode( W5500_RST, OUTPUT );
  digitalWrite( W5500_RST, LOW );
  delay( 10UL );
  digitalWrite( W5500_RST, HIGH );
}

/**
  * @brief SSD1306 initialize.
  * @retval none.
  */
static void initSSD1306()
{
  static const uint8_t PROGMEM logo_bmp[] =
  {
    0b00000000, 0b11000000,
    0b00000001, 0b11000000,
    0b00000001, 0b11000000,
    0b00000011, 0b11100000,
    0b11110011, 0b11100000,
    0b11111110, 0b11111000,
    0b01111110, 0b11111111,
    0b00110011, 0b10011111,
    0b00011111, 0b11111100,
    0b00001101, 0b01110000,
    0b00011011, 0b10100000,
    0b00111111, 0b11100000,
    0b00111111, 0b11110000,
    0b01111100, 0b11110000,
    0b01110000, 0b01110000,
    0b00000000, 0b00110000
  };

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if( !oled.begin( SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS ) )
  {
    Serial.println(F( "SSD1306 allocation failed" ));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  oled.display();
  delay( 2000 ); // Pause for 2 seconds

  // Clear the buffer
  oled.clearDisplay();
  oled.display();

  // text display tests
  oled.setTextSize(1);  // 6 x 8 font size.
  oled.setTextColor( SSD1306_WHITE );
  oledPrint( "STM32 Serial/Ethernet converter." );
  delay( 2000 );
  oled.clearDisplay();
}

/**
  * @brief Obtain the MAC ADDRESS.
  * @retval none.
  */
static void getMACaddress( uint8_t *adr )
{
  eeprom.read( MAC_ADDRESS_IN_24AA025E48, adr, 6 );
  Serial.printf( "\r\nMAC:%02X:%02X:%02X:%02X:%02X:%02X\r\n",
    adr[0],adr[1],adr[2],adr[3],adr[4],adr[5]  );
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    user_Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    user_Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    user_Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.PLLSAI.PLLSAIM = 16;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
  PeriphClkInitStruct.PLLSAI.PLLSAIQ = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV4;
  PeriphClkInitStruct.PLLSAIDivQ = 1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48CLKSOURCE_PLLSAIP;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    user_Error_Handler();
  }
}

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
