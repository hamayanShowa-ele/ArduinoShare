/*
  STM32F407ZET named virgo fsmc test
  Copyright (c) 2013 Kazuaki Hamahara
  This source code is released under the MIT license, but there are some different licenses.
  https://github.com/YukinobuKurata/YouTubeMagicBuyButton/blob/master/MIT-LICENSE.txt
*/

/* Includes ------------------------------------------------------------------*/
#include  <STM32_FSMC.h>

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#undef   LED_BUILTIN
#define  LED_RED      PF8
#define  LED_GREEN    PF10
#define  LED_BLUE     PF9
#define  ACTLED       PG11
#define  LED_BUILTIN  ACTLED
#define  USBON        PC0

#define  INT_SRAM_BASE_ADDRESS      0x60000000
#define  INT_ETHERNET_BASE_ADDRESS  0x64000000
#define  INT_ARCNET_BASE_ADDRESS    0x68000000
#define  EXT_BASE_ADR               0x6C000000

#define  EXT_2116_IN_ADR_1  (EXT_BASE_ADR + 0x00000028)
#define  EXT_2116_IN_ADR_2  (EXT_BASE_ADR + 0x0000002A)
#define  EXT_2116_IN_ADR_3  (EXT_BASE_ADR + 0x0000002C)
#define  EXT_2116_IN_ADR_4  (EXT_BASE_ADR + 0x0000002E)

#define  EXT_2116_OUT_ADR_1  (EXT_BASE_ADR + 0x00000028)
#define  EXT_2116_OUT_ADR_2  (EXT_BASE_ADR + 0x0000002C)
#define  EXT_2116_OUT_ADR_3  (EXT_BASE_ADR + 0x0000002A)
#define  EXT_2116_OUT_ADR_4  (EXT_BASE_ADR + 0x0000002E)

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/**
  * @brief  setup
  * @retval none
  */
void setup()
{
  pinMode( LED_BUILTIN, OUTPUT );
  pinMode( LED_RED, OUTPUT );
  pinMode( LED_GREEN, OUTPUT );
  pinMode( LED_BLUE, OUTPUT );
  digitalWrite( LED_RED, HIGH );
  digitalWrite( LED_GREEN, HIGH );
  digitalWrite( LED_BLUE, HIGH );
  
  /*
    Arduino ide settings
      U(S)ART support enabled(no generic Serial)
      USB support CDC (generic Serial supersede U(S)ART)
      USB speed (LOW/FULL speed)
  */
  pinMode( USBON, OUTPUT );
  digitalWrite( USBON, LOW );
  Serial.begin( 115200UL );
  while( !Serial ){ delay(20UL); }
  Serial.println( "hello serial output from USB." );

  fsmc_address( 19 );  // set address bus size
  fsmc_data( 16 );  // set data bus size.
//  fsmc_sram( FSMC_NORSRAM_BANK4, FSMC_NORSRAM_MEM_BUS_WIDTH_16, 1,12,1,19, 0 );  // ne:FSMC_NORSRAM_BANK1,2,3,4 width:FSMC_NORSRAM_MEM_BUS_WIDTH_8,16,32
  /* read width:88ns, write width:127ns, read address setup time:55ns, write address setup time:93ns, write address setup time:115ns */
  fsmc_sram( FSMC_NORSRAM_BANK4, FSMC_NORSRAM_MEM_BUS_WIDTH_16, 1,8,1,8, 0 );  // ne:FSMC_NORSRAM_BANK1,2,3,4 width:FSMC_NORSRAM_MEM_BUS_WIDTH_8,16,32
  /* read width:52ns, write width:59ns, read address setup time:55ns, write address setup time:93ns, write address setup time:67ns */

  volatile uint16_t *pulseI = (volatile uint16_t *)EXT_2116_IN_ADR_1;  // EXT_2116_IN_ADR_1 or _2 or _3 or _4
  volatile uint16_t *pulseO = (volatile uint16_t *)EXT_2116_OUT_ADR_1;  // EXT_2116_OUT_ADR_1 or _2 or _3 or _4
  volatile uint16_t WtempUS = 0;
  volatile uint16_t RtempUS = 0;
  int loop = 0;
  while( 1 )
  {
    for( int i = 0; i < 65536; i++ )
    {
      *pulseO = WtempUS;
      RtempUS = *pulseI;
      // If the address is EXT_2116_IN_ADR_1, this process is enabled.
      if( pulseI == (volatile uint16_t *)EXT_2116_IN_ADR_1 )
      {
        uint16_t tempUS = RtempUS;
        if( RtempUS & 0x0040 ) tempUS |= 0x0080;
        else tempUS &= ~0x0080;
        if( RtempUS & 0x0080 ) tempUS |= 0x0040;
        else tempUS &= ~0x0040;
        RtempUS = tempUS;
      }

      if( RtempUS != WtempUS )
      {
        digitalWrite( LED_GREEN, HIGH );
        String errMsg = "error! write:"; errMsg += WtempUS; errMsg += " read:"; errMsg += RtempUS; 
        Serial.println( errMsg );
        while( 1 )
        {
          delay( 100UL );
          digitalWrite( LED_RED, LOW );
          delay( 100UL );
          digitalWrite( LED_RED, HIGH );
//          break;
        }
      }
      WtempUS++;
      digitalWrite( LED_GREEN, (digitalRead( LED_GREEN ) == HIGH) ? LOW : HIGH );
    }
    loop++;
    String msg = "loop count : "; msg += loop; Serial.println( msg );
  }
}

/**
  * @brief  loop
  * @retval none
  */
void loop()
{
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
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
