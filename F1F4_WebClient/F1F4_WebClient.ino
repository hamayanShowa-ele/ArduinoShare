/*
  Web client

 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe, based on work by Adrian McEwen

 */

#include <SPI.h>
#include <Wire.h>
#include <Ethernet2.h>
#include <rom24aa025.h>

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
TwoWire i2c1;
EEPROM_24AA025 eeprom( &i2c1 );

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
//byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte mac[6];
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
//char server[] = "www.google.com";    // name address for Google (using DNS)
char server[] = "www.chiseki.go.jp";    // name address for kokudo koutsusyou (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress constantIP(192, 168, 100, 177);
IPAddress myIP; // self IP address

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

/*
  setup.
*/
void setup()
{
  // Open serial communications and wait for port to open:
  // initialize digital pin LED_BUILTIN as an output.
  pinMode( LED_BUILTIN, OUTPUT );
  digitalWrite( LED_BUILTIN, LOW );

  // initialize usb power on pin USB_POWER_ON as an output.
  pinMode( USB_POWER_ON, OUTPUT );
  digitalWrite( USB_POWER_ON, USB_POWER_ON_LEVEL );

  // initialize usb CDC class.
  Serial.begin( 115200UL );
  while( !Serial ) { delay( 20UL ); }
  Serial.println( "Board 2102 start up message from USB serial." );
  Serial.println( "Web client example." );

  // I2C1 initialize.
  i2c1.setSDA( SDA1 );  /* SDA */
  i2c1.setSCL( SCL1 );  /* SCL */
  i2c1.begin();
  
  // read the mac address from the i2c eeprom.
  eeprom.read( MAC_ADDRESS_IN_24AA025E48, mac, (int)sizeof(mac) );
  Serial.printf( "mac %02X:%02X:%02X:%02X:%02X:%02X\n",
    mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]  );
  Serial.println();
  
  // SPI connected with w5500 initialize.
  SPI.setMISO( SPI2_MISO );
  SPI.setMOSI( SPI2_MOSI );
  SPI.setSCLK( SPI2_SCK );
  Ethernet.init( W5500_CS );

  // reset W5500.
  pinMode( W5500_RST, OUTPUT );
  digitalWrite( W5500_RST, LOW );
  delay( 10UL );
  digitalWrite( W5500_RST, HIGH );
  delay( 500UL );

  // start the Ethernet connection:
  if( Ethernet.begin( mac ) == 0 )
  {
    Serial.println( "Failed to configure Ethernet using DHCP" );
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin( mac, constantIP );
  }
  myIP = Ethernet.localIP();
  Serial.print( "\r\nIP number assigned by DHCP is " ); Serial.println( myIP );

  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.print( "connecting......" );
  // if you get a connection, report back via serial:
  if( client.connect(server, 80) )
  {
    Serial.println( "connected" );
    // Make a HTTP request:
    // connect to http://www.chiseki.go.jp/library/index.html
//    client.println("GET /search?q=arduino HTTP/1.1");
    client.println("GET /library/index.html HTTP/1.1");
//    client.println("Host: www.google.com");
    client.println("Host: www.chiseki.go.jp");
    client.println("Connection: close");
    client.println();
  }
  else
  {
    // if you didn't get a connection to the server:
    Serial.println( "failed" );
  }
}

/*
  loop.
*/
int count = 0;
void loop()
{
  // if there are incoming bytes available
  // from the server, read them and print them:
  if( client.available() )
  {
    char c = client.read();
    Serial.print(c);
    if( ++count >= 6000 )
    {
      client.stop();
      Serial.println(); Serial.println( "force disconnecting." );
      while (true);
    }
  }

  // if the server's disconnected, stop the client:
  if( !client.connected() )
  {
    Serial.println();
    Serial.println( "disconnecting." );
    client.stop();
    Serial.print( "web site contents size : " ); Serial.print( count, DEC );
    Serial.println( "bytes.");

    // do nothing forevermore:
    while( true )
    {
      delay( 50UL );
      digitalWrite( LED_BUILTIN, (digitalRead(LED_BUILTIN) == LOW) ? HIGH : LOW );
    }
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
#if  defined( STM32F1xx )
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

#elif  defined( STM32F4xx )
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
#endif  /* STM32F4xx */
}

extern "C"
{
}  /* extern "C" */
