/*
  Arduino IDE setup.
  Board type : Ncleo-64. NUCLEO-F446RE
  U(S)ART support : Enaable (No generic Serial).
  USB support : CDC (generic Serial supersede U(S)ART).
  USB speed : LOw/Full Speed.
  Optimize : ANY.
  C Runtime Library : ANY.
  Update method : STM32CubeProgrammer (SWD).
  Serial port : ANY.
*/
/**
  * @brief Example of using a SSD1306(AL12832AWWB-H-U02) library
  *         on an STM32 microcontroller.
  */
#include  <HardwareSerial.h>
#include  <Adafruit_SSD1306.h>
#include  <Adafruit_GFX.h>
#include  <splash.h>
#include  <STM32F_CPU_Identity.h>
#include  <mul_tsk.h>
#include  <MY_STREAM.h>

enum TSK_ID_NUMBERS
{
  ID_serial1_Task,
  ID_serial3_Task,
  ID_serial4_Task,
  ID_serial5_Task,
  ID_serial6_Task,
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

#define  TXD1  PA9
#define  RXD1  PA10
#define  TXD2  PA2
#define  RXD2  PA3
#define  TXD3  PC_10_ALT1  // PC10
#define  RXD3  PC_11_ALT1  // PC11
#define  TXD4  PA0
#define  RXD4  PA1
#define  TXD5  PC12
#define  RXD5  PD2
#define  TXD6  PC6
#define  RXD6  PC7

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

void oledPrint( String msg, int x = 0, int y = 0 );
void tsk_ini( void );
void stackMonitor( void );
void serial_Task( void );
void pcIF_Task( void );
static void seriOutput( const char *str );
static void transparentsSerial( HardwareSerial *ser );

/**
  * @brief instance and global variables.
  */
HardwareSerial Serial1( RXD1, TXD1 );  // rxd pin, txd pin
HardwareSerial Serial2( RXD2, TXD2 );  // rxd pin, txd pin
HardwareSerial Serial3( RXD3, TXD3 );  // rxd pin, txd pin
HardwareSerial Serial4( RXD4, TXD4 );  // rxd pin, txd pin
HardwareSerial Serial5( RXD5, TXD5 );  // rxd pin, txd pin
HardwareSerial Serial6( RXD6, TXD6 );  // rxd pin, txd pin

Adafruit_SSD1306 oled( SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET );
MY_STREAM pcIF_tx_Fifo(256);
MY_STREAM pcIF_rx_Fifo(256);
STM32F_CPU_IDENTITY cpu_id;

static uint8_t tsk0_stk[ 128 * 5 ];
static uint8_t tsk1_stk[ 128 * 5 ];
static uint8_t tsk7_stk[ 128 * 5 ];

static const unsigned char PROGMEM logo_bmp[] =
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

/**
  * @brief task initialize.
  * @retval none.
  */
void tsk_ini( void )
{
  cfg_sys( TSKID_END );

  ID_serial1_Task,
  ID_serial3_Task,
  ID_serial4_Task,
  ID_serial5_Task,
  ID_serial6_Task,
  ID_stack_monitor,

  reg_tsk( ID_stack_monitor, (void *)stackMonitor, (void *)tsk0_stk, sizeof(tsk0_stk), 0,0,0,0 );
  reg_tsk( ID_serial1_Task, (void *)serial_Task, (void *)tsk1_stk, sizeof(tsk1_stk), 0,0,0,0 );
  reg_tsk( ID_pcIF_Task, (void *)pcIF_Task, (void *)tsk7_stk, sizeof(tsk7_stk), 0,0,0,0 );

  sta_tsk( ID_stack_monitor );
//  sta_tsk( ID_serial1_Task );
}

/**
  * @brief setup.
  * @retval none.
  */
void setup()
{
  /* Operation of the port for the USB to be recognized by the PC. */
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

  Serial.begin( 115200UL );
  while( !Serial ) { delay( 20 ); }
  Serial.println( "STM32 SSD1306(AL12832AWWB-H-U02) Demo." );
  delay( 100UL );

  // get cpu id.
  Serial.println( cpu_id.sysClockString() );
  Serial.println( cpu_id.uIDtring() );

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
  oled.setTextSize(1);
  oled.setTextColor( SSD1306_WHITE );
  oledPrint( "STM32 SSD1306(AL12832AWWB-H-U02) Demo." );
  delay( 2000 );
  oled.clearDisplay();

  // hardware serial initialize.
  Serial1.begin( 38400UL );
  Serial2.begin( 115200UL );
  Serial3.begin( 38400UL );
  Serial4.begin( 38400UL );
  Serial5.begin( 38400UL );
  Serial6.begin( 38400UL );
  while( !Serial1 ) { dly_tsk( 20UL ); }
  while( !Serial2 ) { dly_tsk( 20UL ); }
  while( !Serial3 ) { dly_tsk( 20UL ); }
  while( !Serial4 ) { dly_tsk( 20UL ); }
  while( !Serial5 ) { dly_tsk( 20UL ); }
  while( !Serial6 ) { dly_tsk( 20UL ); }
//  transparentsSerial( &Serial6 );

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
  * @brief OLED Messae.
  * @retval none.
  */
void oledPrint( String msg, int x, int y )
{
  oled.setCursor( x, y );
  oled.print( msg );
  oled.display(); // actually display all of the above
}
void oledPrint( const char *msg, int x, int y )
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
  sta_tsk( ID_serial1_Task );

  while( 1 )
  {
    /*stack report.*/
    dly_tsk( 10 * 1000UL );
#if 0
    char msg[64];
    sprintf( msg, "  SERIAL1:%d/%d", RemainStack( tsk1_stk, sizeof(tsk1_stk) ), sizeof(tsk1_stk) );
    seriOutput( (const char *)msg );

    sprintf( msg, "  PC IF:%d/%d", RemainStack( tsk7_stk, sizeof(tsk7_stk) ), sizeof(tsk7_stk) );
    seriOutput( (const char *)msg );

    sprintf( msg, "  MONITOR:%d/%d", RemainStack( tsk0_stk, sizeof(tsk0_stk) ), sizeof(tsk0_stk) );
    seriOutput( (const char *)msg );

    seriOutput( "" );
#endif
  }
}

/**
  * @brief read the baud rate selector.
  * @retval read value.
  */
//static uint8_t brSelectorRead()
static uint8_t chSelectorRead()
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
//static uint8_t chSelectorRead()
static uint8_t brSelectorRead()
{
  uint8_t tempUC = 0;
  if( digitalRead(CH_SEL0) == HIGH ) tempUC |= 0x01;  // CH_SEL0:PH0
  if( digitalRead(CH_SEL1) == HIGH ) tempUC |= 0x02;  // CH_SEL1:PH1
  if( digitalRead(CH_SEL2) == HIGH ) tempUC |= 0x04;  // CH_SEL2:PC2
  return tempUC;
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
  * @brief serial interface task.
  * @retval none.
  */
void serial_Task( void )
{
  // Get startup arguments.
//  VP_INT exinf[4];
//  get_par( exinf, sizeof(exinf) / sizeof(exinf[0]) );
  HardwareSerial *seri = &Serial2;
  uint8_t brrBit = 3;  // deafult 115200bps
  uint8_t chBit = 2;  // default Serial2
  uint32_t swScanTim = millis();
  uint8_t sw4_state = 0;
  while( 1 )
  {
    if( (millis() - swScanTim) >= 50UL )
    {
      swScanTim = millis();

      // chack SW4(setup switch).
      sw4_state <<= 1;
      sw4_state |= ( digitalRead(SW_IN2) == LOW ) ? 0x01 : 0x00;
      if( (sw4_state & 0x07) == 0x03 )  // When the switch is pressed.
      {
        uint8_t chBitNew = chSelectorRead();  // 1 or 2 or 3 or 4 or 5 or 6
        if( chBitNew != chBit )
        {
          chBit = chBitNew;
          switch( chBit )
          {
            case 1 : seri = &Serial1; break;
            case 3 : seri = &Serial3; break;
            case 4 : seri = &Serial4; break;
            case 5 : seri = &Serial5; break;
            case 6 : seri = &Serial6; break;
            default :
              seri = &Serial2;
              chBit = 2;
              break;
          }
        }

        uint32_t brr = (brrBit == 0) ? 9600UL : 
                       (brrBit == 1) ? 19200UL : 
                       (brrBit == 2) ? 38400UL : 115200UL;
        uint8_t brrBitNew = brSelectorRead() >> 1;  // 9600 or 19200 or 38400 or 115200bps
        if( brrBitNew != brrBit )  // When there is a change in baud rate.
        {
          brrBit = brrBitNew;
          brr = (brrBit == 0) ? 9600UL : 
                (brrBit == 1) ? 19200UL : 
                (brrBit == 2) ? 38400UL : 115200UL;
          seri->end();
          seri->begin( brr );
          dly_tsk( 20UL );
        }

        // text display
        oled.clearDisplay();
        String msg = "Select channel = " + String( chBit, DEC ) + ".";
        oledPrint( msg, 0, 0 );
        msg = "baud = " + String( brr, DEC ) + "bps.";
        oledPrint( msg, 0, 8 );
      }
    }

    // When incoming data is sent from the PC side.
    int c = pcIF_rx_Fifo.read();
    if( c >= 0 )
    {
      seri->write( c );
    }

    // When receive data is sent from the serial side.
    if( seri->available() )
    {
      c = seri->read();
      pcIF_tx_Fifo.write( (uint8_t)c );
    }

    rot_rdq();
  }
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
