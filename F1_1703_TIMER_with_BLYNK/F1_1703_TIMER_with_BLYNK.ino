/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest
  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.
    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app
  Blynk library is licensed under MIT license
  This example code is in public domain.
 *************************************************************
  You’ll need:
   - Blynk App (download from AppStore or Google Play)
   - Arduino Uno board
   - Decide how to connect to Blynk
     (USB, Ethernet, Wi-Fi, Bluetooth, ...)
  There is a bunch of great example sketches included to show you how to get
  started. Think of them as LEGO bricks  and combine them as you wish.
  For example, take the Ethernet Shield sketch and combine it with the
  Servo example, or choose a USB sketch and add a code from SendData
  example.
 *************************************************************/

/* Comment this out to disable prints and save space */
#define   BLYNK_PRINT Serial1
#include  <STM32F_CPU_Identity.h>
#include  <SPI.h>
#include  <Wire.h>
#include  <Ethernet2.h>
#include  <BlynkSimpleEthernet2.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).

// Remark "#include author.h" and write the Blynk token
//  that you got yourself as follows instead.
#include  "author.h"
#ifndef  __AUTHORED__
char auth[] = "type your blynk token.";  // token
#endif  /* __AUTHORED__ */


/*
  defines.
*/
#define  SET_OVERFLOW   (720UL)
#define  CURRENT_PERIOD (1000UL)  // ms
#define  CURRENT_SETUP_PERIOD  (20UL)  // ms
#define  CURRENT_HOLD_PERIOD   (20UL)  // ms
#define  TRIGGER_SETUP_PERIOD  (20UL + 100UL)  // ms
#define  TRIGGER_HOLD_PERIOD   (100UL)  // ms

/* GPIO pin defines. */
#define  ACT_LED    PC13
#define  DIO0       PC0
#define  DIO1       PC1
#define  DIO2       PC2
#define  DIO3       PC3

#define  PWM1_PIN   PA0
#define  PWM2_PIN   PA1
#define  PWM3_PIN   PA2
#define  PWM4_PIN   PA3
#define  PWM5_PIN   PA6
#define  PWM6_PIN   PA_7_ALT1  // PA7
#define  PWM7_PIN   PB_0_ALT1  // PB0
#define  PWM8_PIN   PB_1_ALT1  // PB1

#define  PWM1_CHANNEL  1
#define  PWM2_CHANNEL  2
#define  PWM3_CHANNEL  3
#define  PWM4_CHANNEL  4
#define  PWM5_CHANNEL  1
#define  PWM6_CHANNEL  2
#define  PWM7_CHANNEL  3
#define  PWM8_CHANNEL  4

/* USART1 TXD RXD */
#define  TXD1       PA9
#define  RXD1       PA10
#define  USART1_TXD_PORT      GPIOA
#define  USART1_RXD_PORT      GPIOA
#define  USART1_TXD_PIN       GPIO_Pin_9
#define  USART1_RXD_PIN       GPIO_Pin_10

/* I2C1,I2C2 */
#define  SCL2       PB10
#define  SDA2       PB11
#define  BOARD_1703_IC2_ADR  0x20  /* PCA8574PW */
#define  BOARD_1703_IC3_ADR  0x21  /* PCA8574PW */
#define  BOARD_1703_IC4_ADR  0x22  /* PCA8574PW */
#define  BOARD_1703_U1_ADR   0x76  /* BME280 */
#define  BOARD_1703_IC7_ADR  0x50  /* 24AA025E48 mac address i2c eeprom */

/* W5500 */
#define  W5500_CS       PC7   /* output */
#define  W5500_INT      PC5   /* input */
#define  W5500_RESET    PC8   /* output */
#define  W5500_CS_PORT  GPIOC
#define  W5500_CS_PIN   GPIO_Pin_7
#define  W5500_CS_IS_0  W5500_CS_PORT->BSRR = (W5500_CS_PIN << 16)
#define  W5500_CS_IS_1  W5500_CS_PORT->BSRR = (W5500_CS_PIN << 0)
#define  SPI2_MOSI  PB15
#define  SPI2_MISO  PB14
#define  SPI2_SCK   PB13

/*
  instance and global variables.
*/
STM32F_CPU_IDENTITY cpu_id;
HardwareSerial Serial1( RXD1, TXD1 );  // rxd pin, txd pin
TwoWire i2c2( SDA2, SCL2 );

HardwareTimer Timer5( TIM5 );
HardwareTimer Timer3( TIM3 );
uint32_t tim5Scale,tim5PwmPulse[4];
uint32_t tim3Scale,tim3PwmPulse[2];

IPAddress myip( 192,168,100,231 );
IPAddress dnsip( 192,168,100,1 );

const byte mac[] = {0x00, 0x1E, 0xC0, 0xF7, 0xEE, 0x67 };

bool currentOutput = false;
bool blynkLinked = false;
uint32_t ledBlynkTime;
uint32_t currentSetupTimer, currentHoldTimer;
uint32_t triggerSetupTimer, triggerHoldTimer;

/*
  setup.
*/
void setup()
{
  /* GPIO initialize and reset w5500. */
  pinMode( ACT_LED, OUTPUT );
  pinMode( DIO0, OUTPUT );
  pinMode( DIO1, OUTPUT );
  pinMode( DIO2, OUTPUT );
  pinMode( DIO3, OUTPUT );
  digitalWrite( DIO0, LOW );
  digitalWrite( DIO1, LOW );
  digitalWrite( DIO2, LOW );
  digitalWrite( DIO3, LOW );
 
  pinMode( W5500_CS, OUTPUT );
  digitalWrite( W5500_CS, HIGH );
  pinMode( W5500_RESET, OUTPUT );
  digitalWrite( W5500_RESET, LOW );
  delay( 10UL );
  digitalWrite( W5500_RESET, HIGH );
  delay( 500UL );

  /* Serial initialize and output start up message. */
  Serial1.begin( 115200UL );
  while( !Serial1 ) delay( 20UL );
  Serial1.println( "\r\n\r\nSTM32F1 Timer Test with BLYNK." );
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

  /* TIMER5 initialize and output zero level. */
  Timer5.pause();  // Pause counter and all output channels
  // set prescaler register (which is factor value - 1)
  Timer5.setPrescaleFactor( 1UL );
  // set AutoReload register depending on format provided
  Timer5.setOverflow( SET_OVERFLOW );
  tim5Scale = Timer5.getOverflow() / 2UL;
  for( int i = 0; i < (int)(sizeof(tim5PwmPulse) / sizeof(tim5PwmPulse[0])); i++ )
  {
    tim5PwmPulse[i] = tim5Scale;
    Serial1.print( "tim5PwmPulse = " ); Serial1.println( String( tim5PwmPulse[i] ) );
  }

  // Configure timer channel with specified mode on specified pin if available#endif
  Timer5.setMode( PWM1_CHANNEL, TIMER_OUTPUT_COMPARE_PWM1, PWM1_PIN );
  Timer5.setMode( PWM2_CHANNEL, TIMER_OUTPUT_COMPARE_PWM1, PWM2_PIN );
  Timer5.setMode( PWM3_CHANNEL, TIMER_OUTPUT_COMPARE_PWM1, PWM3_PIN );
  Timer5.setMode( PWM4_CHANNEL, TIMER_OUTPUT_COMPARE_PWM1, PWM4_PIN );
  // set Compare register value of specified channel depending on format provided  Timer5.resume(); // Resume counter and all output channels
  Timer5.setCaptureCompare( PWM1_CHANNEL, tim5PwmPulse[0] );
  Timer5.setCaptureCompare( PWM2_CHANNEL, tim5PwmPulse[1] );
  Timer5.setCaptureCompare( PWM3_CHANNEL, tim5PwmPulse[2] );
  Timer5.setCaptureCompare( PWM4_CHANNEL, tim5PwmPulse[3] );

//  Timer5.attachInterrupt( timer5OverFlowCB );
  Timer5.resume(); // Resume counter and all output channels

  /* TIMER3 initialize and output zero level. */
  Timer3.pause();  // Pause counter and all output channels
  // set prescaler register (which is factor value - 1)
  Timer3.setPrescaleFactor( 1UL );
  // set AutoReload register depending on format provided
  Timer3.setOverflow( SET_OVERFLOW );
  tim3Scale = Timer3.getOverflow() / 2UL;
  for( int i = 0; i < (int)(sizeof(tim3PwmPulse) / sizeof(tim3PwmPulse[0])); i++ )
  {
    tim3PwmPulse[i] = tim3Scale;
    Serial1.print( "tim3PwmPulse = " ); Serial1.println( String( tim3PwmPulse[i] ) );
  }

  // Configure timer channel with specified mode on specified pin if available#endif
  Timer3.setMode( PWM5_CHANNEL, TIMER_OUTPUT_COMPARE_PWM1, PWM5_PIN );
  Timer3.setMode( PWM6_CHANNEL, TIMER_OUTPUT_COMPARE_PWM1, PWM6_PIN );
  // set Compare register value of specified channel depending on format provided  Timer5.resume(); // Resume counter and all output channels
  Timer3.setCaptureCompare( PWM5_CHANNEL, tim3PwmPulse[0] );
  Timer3.setCaptureCompare( PWM6_CHANNEL, tim3PwmPulse[1] );

//  Timer3.attachInterrupt( timer5OverFlowCB );
  Timer3.resume(); // Resume counter and all output channels

  // SPI connected with w5500 initialize.
  SPI.setMISO( SPI2_MISO );
  SPI.setMOSI( SPI2_MOSI );
  SPI.setSCLK( SPI2_SCK );
//  SPI.setSSEL( W5500_CS );

  // The BLYNK library sets the CS pin of the Ethernet IC (W5500) as the default SPI pin, and does not call the EthernetClass init (uint8_t cspin), so we need to set the CS pin here.
  Ethernet.init( W5500_CS );
//  Blynk.begin( auth );  // dhcp using and An appropriate MAC address.
  Blynk.begin( auth, BLYNK_DEFAULT_DOMAIN, BLYNK_SERVER_PORT, mac );  // dhcp using and A valid MAC address.
//  Blynk.begin( auth, BLYNK_DEFAULT_DOMAIN, BLYNK_SERVER_PORT, myip, dnsip, mac );

  myip = Ethernet.localIP();
  Serial1.print( "myip " ); Serial1.println( myip );

  // blynk default value initialize.
 
  // any timers initialize.
  blynkLinked = true;
  ledBlynkTime = millis();
  currentSetupTimer = currentHoldTimer = 0xFFFFFFFF;
  triggerSetupTimer = triggerHoldTimer = 0xFFFFFFFF;
}

/*
  loop.
*/
void loop()
{
  if( blynkLinked && (millis() - ledBlynkTime) >= 250UL )
  {
    ledBlynkTime = millis();
    digitalToggle( ACT_LED );
  }

  /* current output control. */
  if( millis() >= currentSetupTimer )
  {
    currentOutput = true;
#if 0
    Timer5.setCaptureCompare( PWM1_CHANNEL, tim5PwmPulse[0] );
    Timer5.setCaptureCompare( PWM2_CHANNEL, tim5PwmPulse[1] );
    Timer5.setCaptureCompare( PWM3_CHANNEL, tim5PwmPulse[2] );
    Timer5.setCaptureCompare( PWM4_CHANNEL, tim5PwmPulse[3] );
    Timer3.setCaptureCompare( PWM5_CHANNEL, tim3PwmPulse[0] );
    Timer3.setCaptureCompare( PWM6_CHANNEL, tim3PwmPulse[1] );
#else
    TIM5->CCR1 = (uint16_t)tim5PwmPulse[0];
    TIM5->CCR2 = (uint16_t)tim5PwmPulse[1];
    TIM5->CCR3 = (uint16_t)tim5PwmPulse[2];
    TIM5->CCR4 = (uint16_t)tim5PwmPulse[3];
    TIM3->CCR1 = (uint16_t)tim3PwmPulse[0];
    TIM3->CCR2 = (uint16_t)tim3PwmPulse[1];
#endif

    currentSetupTimer = 0xFFFFFFFF;
    currentHoldTimer = millis() + CURRENT_HOLD_PERIOD;
Serial1.println( "CURRENT turn ON." );
  }

  if( currentOutput && (millis() >= currentHoldTimer) )
  {
    currentOutput = false;
#if 0
    Timer5.setCaptureCompare( PWM1_CHANNEL, tim5Scale );
    Timer5.setCaptureCompare( PWM2_CHANNEL, tim5Scale );
    Timer5.setCaptureCompare( PWM3_CHANNEL, tim5Scale );
    Timer5.setCaptureCompare( PWM4_CHANNEL, tim5Scale );
    Timer3.setCaptureCompare( PWM5_CHANNEL, tim3Scale );
    Timer3.setCaptureCompare( PWM6_CHANNEL, tim3Scale );
#else
    TIM5->CCR1 = (uint16_t)tim5Scale;
    TIM5->CCR2 = (uint16_t)tim5Scale;
    TIM5->CCR3 = (uint16_t)tim5Scale;
    TIM5->CCR4 = (uint16_t)tim5Scale;
    TIM3->CCR1 = (uint16_t)tim3Scale;
    TIM3->CCR2 = (uint16_t)tim3Scale;
#endif
    currentHoldTimer = 0xFFFFFFFF;
Serial1.println( "CURRENT turn OFF." );
  }

  /* trigger output control. */
  if( millis() >= triggerSetupTimer )
  {
    triggerSetupTimer = 0xFFFFFFFF;
    triggerHoldTimer = millis() + TRIGGER_HOLD_PERIOD;
    digitalWrite( DIO0, HIGH );
    Serial1.println( "TRIGGER turn ON." );
  }

  if( millis() >= triggerHoldTimer )
  {
    triggerHoldTimer = 0xFFFFFFFF;
    digitalWrite( DIO0, LOW );
    Serial1.println( "TRIGGER turn OFF." );
  }

  /* check blynk link status. */
  if( Blynk.connected() ) { blynkLinked = true; }
  else
  {
    blynkLinked = false;
    Blynk.disconnect();
    Blynk.connect();
  }

  /* execute blynk. */
//  if( blynkLinked ) { Blynk.run(); }
  Blynk.run();
}

/*
  blynk virtual pin 0 function.
    current output control.
*/
BLYNK_WRITE( V0 ) //
{ 
  if( !currentOutput )
  {
    if( param.asInt() )
    {
      Timer5.setCaptureCompare( PWM1_CHANNEL, tim5PwmPulse[0] );
      Timer5.setCaptureCompare( PWM2_CHANNEL, tim5PwmPulse[1] );
      Timer5.setCaptureCompare( PWM3_CHANNEL, tim5PwmPulse[2] );
      Timer5.setCaptureCompare( PWM4_CHANNEL, tim5PwmPulse[3] );
      Timer3.setCaptureCompare( PWM5_CHANNEL, tim3PwmPulse[0] );
      Timer3.setCaptureCompare( PWM6_CHANNEL, tim3PwmPulse[1] );
    }
    else
    {
      Timer5.setCaptureCompare( PWM1_CHANNEL, tim5Scale );
      Timer5.setCaptureCompare( PWM2_CHANNEL, tim5Scale );
      Timer5.setCaptureCompare( PWM3_CHANNEL, tim5Scale );
      Timer5.setCaptureCompare( PWM4_CHANNEL, tim5Scale );
      Timer3.setCaptureCompare( PWM5_CHANNEL, tim3Scale );
      Timer3.setCaptureCompare( PWM6_CHANNEL, tim3Scale );
    }
  }
//  Serial1.println( "V0" );
}

/*
  blynk virtual pin 8 function.
    current output control.
*/
BLYNK_WRITE( V8 ) //
{ 
  if( !currentOutput && param.asInt() )
  {
    currentSetupTimer = millis() + CURRENT_SETUP_PERIOD;  // limit timer set.
    triggerSetupTimer = millis() + TRIGGER_SETUP_PERIOD;  // limit timer set.
  }
//  Serial1.println( "V8" );
}

/*
  blynk virtual pin 7 function.
    manual trigger output.
*/
BLYNK_WRITE( V7 ) //
{ 
  if( param.asInt() )
  {
    digitalWrite( DIO0, HIGH );
  }
  else
  {
    digitalWrite( DIO0, LOW );
  }
//  Serial1.println( "V7" );
}

/*
  blynk virtual pin 1 function.
    set up PWM1~6.
*/
BLYNK_WRITE( V1 ) // PWM1
{ 
  float value = param.asFloat();
  int32_t tempUL = (int32_t)tim5Scale;
  tempUL = (int32_t)(tempUL * value / 10.0F);
  tim5PwmPulse[0] = tim5Scale - tempUL;
  Serial1.print( "PWM1:" ); Serial1.print( String( value ) );
  Serial1.print( " WIDTH:" ); Serial1.println( String( tim5PwmPulse[0] ) );
}

BLYNK_WRITE( V2 ) // PWM2
{ 
  float value = param.asFloat();
  int32_t tempUL = (int32_t)tim5Scale;
  tempUL = (int32_t)(tempUL * value / 10.0F);
  tim5PwmPulse[1] = tim5Scale - tempUL;
  Serial1.print( "PWM2:" ); Serial1.print( String( value ) );
  Serial1.print( " WIDTH:" ); Serial1.println( String( tim5PwmPulse[1] ) );
}

BLYNK_WRITE( V3 ) // PWM3
{ 
  float value = param.asFloat();
  int32_t tempUL = (int32_t)tim5Scale;
  tempUL = (int32_t)(tempUL * value / 10.0F);
  tim5PwmPulse[2] = tim5Scale - tempUL;
  Serial1.print( "PWM3:" ); Serial1.print( String( value ) );
  Serial1.print( " WIDTH:" ); Serial1.println( String( tim5PwmPulse[2] ) );
}

BLYNK_WRITE( V4 ) // PWM4
{ 
  float value = param.asFloat();
  int32_t tempUL = (int32_t)tim5Scale;
  tempUL = (int32_t)(tempUL * value / 10.0F);
  tim5PwmPulse[3] = tim5Scale - tempUL;
  Serial1.print( "PWM4:" ); Serial1.print( String( value ) );
  Serial1.print( " WIDTH:" ); Serial1.println( String( tim5PwmPulse[3] ) );
}

BLYNK_WRITE( V5 ) // PWM5
{ 
  float value = param.asFloat();
  int32_t tempUL = (int32_t)tim3Scale;
  tempUL = (int32_t)(tempUL * value / 10.0F);
  tim3PwmPulse[0] = tim3Scale - tempUL;
  Serial1.print( "PWM5:" ); Serial1.print( String( value ) );
  Serial1.print( " WIDTH:" ); Serial1.println( String( tim3PwmPulse[0] ) );
}

BLYNK_WRITE( V6 ) // PWM6
{ 
  float value = param.asFloat();
  int32_t tempUL = (int32_t)tim3Scale;
  tempUL = (int32_t)(tempUL * value / 10.0F);
  tim3PwmPulse[1] = tim3Scale - tempUL;
  Serial1.print( "PWM6:" ); Serial1.print( String( value ) );
  Serial1.print( " WIDTH:" ); Serial1.println( String( tim3PwmPulse[1] ) );
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

/* interrupt handlers. */
void timer5OverFlowCB( void )
{
#if 0
    count++;
    if( (count % COUNT_LIMIT) == COUNT_CENTER )  // lower side
    {
      TIM5->CCR1 = (uint16_t)((tim5Scale * 20UL) / 20UL);
    }
    else if( (count % COUNT_LIMIT) == COUNT_UPPER )  // upper side
    {
      TIM5->CCR1 = (uint16_t)((tim5Scale * 0UL) / 20UL);
    }
#endif
}


extern "C"
{
}  /* extern "C" */
