/*
  Arduino IDE setup.
  Board type : Generic STM32F4 or STM32F1 series.
  U(S)ART support : enable (generic Serial).
  USB support : CDC (generic Serial supersede U(S)ART).
  USB speed : LOw/Full Speed.
  Optimize : ANY.
  C Runtime Library : ANY.
  Update method : STM32CubeProgrammer (SWD).
  Serial port : ANY.
*/

#include  <STM32F_CPU_Identity.h>

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

/*
  setup.
*/
void setup()
{
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
  String msg = "systic:"; msg += String( cpu_id.sysload() );
  msg += " hclk:"; msg += String( cpu_id.hclk() );
  msg += " pclk1:"; msg += String( cpu_id.pclk1() );
  msg += " pclk2:"; msg += String( cpu_id.pclk2() );
  msg += " sysclock:"; msg += String( cpu_id.sysclk() );
  Serial.println( msg );
  uint32_t uid[3];
  cpu_id.uID( uid );
  msg = "UID0:0x"; msg += String( uid[0], HEX );
  msg += " UID1:0x"; msg += String( uid[1], HEX );
  msg += " UID2:0x"; msg += String( uid[2], HEX );
  Serial.println( msg );
}

/*
  loop.
*/
void loop()
{
  delay( 500UL );
  Serial.println( "hello world." );
  digitalWrite( LED_BUILTIN, (digitalRead(LED_BUILTIN) == LOW) ? HIGH : LOW );
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
