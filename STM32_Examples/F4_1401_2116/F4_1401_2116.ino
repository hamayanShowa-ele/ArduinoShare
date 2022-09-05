/*
  2116 board testing program.

  Obtain 64 channels of pulse information from the 2116 board.
 */
/*
  Arduino IDE setup.
  Board type : Generic STM32F4 Generic F407ZETx
  U(S)ART support : Enable (No generic Serial).
  USB support : CDC (generic Serial supersede U(S)ART).
  USB speed : Low/Full Speed.
  Optimize : ANY.
  C Runtime Library : ANY.
  Update method : STM32CubeProgrammer (SWD).
  Serial port : ANY.
*/
/**
  * @brief Add the following definition to the beginning of HardwareSerial.h in advance.
          #define SERIAL_TX_BUFFER_SIZE 256
          #define SERIAL_RX_BUFFER_SIZE 256
  */

#include  <STM32_FSMC.h>
#include  <bd1401a.h>  /* https://github.com/hamayanShowa-ele/ArduinoShare/blob/main/libraries/board_header/bd1401a.h */
#include  <bd2116.h>  /* https://github.com/hamayanShowa-ele/ArduinoShare/blob/main/libraries/board_header/bd2116.h */

#define  TXD1       PA9
#define  RXD1       PA10

BD2116 pulse1( (volatile uint16_t *)(EXT_BASE_ADR + BD2116_PULSE1_ADDRESS) );
BD2116 pulse2( (volatile uint16_t *)(EXT_BASE_ADR + BD2116_PULSE2_ADDRESS) );
BD2116 pulse3( (volatile uint16_t *)(EXT_BASE_ADR + BD2116_PULSE3_ADDRESS) );
BD2116 pulse4( (volatile uint16_t *)(EXT_BASE_ADR + BD2116_PULSE4_ADDRESS) );
HardwareSerial Serial1( RXD1, TXD1 ); 

/**
  * @brief GPIO initialize.
  * @retval none.
  */
static void initGPIO()
{
  pinMode( LED_RED, OUTPUT );
  pinMode( LED_BLUE, OUTPUT );
  pinMode( LED_GREEN, OUTPUT );
  pinMode( LED_BUILTIN, OUTPUT );
  // initialize usb power on pin USBON as an output.
  pinMode( USBON, OUTPUT );
  digitalWrite( USBON, USB_POWER_ON_LEVEL );
  // reset i/o.
  pinMode( IO_RST, OUTPUT );
  digitalWrite( IO_RST, LOW );
  delay( 10UL );
  digitalWrite( IO_RST, HIGH );
}

/**
  * @brief  setup
  * @retval none
  */
void setup()
{
  // GPIO initialize.
  initGPIO();

  // Open serial communications and wait for port to open:
  // USB/Serial initialize.
  Serial1.begin( 115200UL );
  while( !Serial1 ) { delay( 20 ); }
  Serial1.println( "1401A 2116 board testing program from USB Serial." );
  delay( 100UL );

  // External CPU BUS initialize.
  fsmc_address( 19 );  // set address bus size
  fsmc_data( 16 );  // set data bus size.
  fsmc_sram( FSMC_NORSRAM_BANK4, FSMC_NORSRAM_MEM_BUS_WIDTH_16, 1,12,1,9, 0 );
}

/**
  * @brief  loop
  * @retval none
  */
void loop()
{
  delay( 1000UL );
  Serial1.printf( "%04X %04X %04X %04X\r\n",
    pulse1.read(), pulse2.read(), pulse3.read(), pulse4.read() );
}

/**
  * @brief System Clock Configuration
  * @retval None
  * @attention
  * Be sure to refer to this site to make changes to stm32yyxx_hal_conf.h.
  * https://www.stm32duino.com/viewtopic.php?p=8819
  * 
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
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
  while (1) { }
  /* USER CODE END Error_Handler_Debug */
}
