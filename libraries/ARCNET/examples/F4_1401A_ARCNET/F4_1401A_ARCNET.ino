/*
  STM32F407ZET named virgo(1401A) arcnet test
  Copyright (c) 2013 Kazuaki Hamahara
  This source code is released under the MIT license, but there are some different licenses.
  https://github.com/YukinobuKurata/YouTubeMagicBuyButton/blob/master/MIT-LICENSE.txt
*/
/*
  Arduino ide settings
    U(S)ART support enabled(no generic Serial)
    USB support CDC (generic Serial supersede U(S)ART)
    USB speed (LOW/FULL speed)
*/

//#define  BD_TYPE_1401
#define  BD_TYPE_1415

/* Includes ------------------------------------------------------------------*/
#include  <STM32F_CPU_Identity.h>  /* https://github.com/hamayanShowa-ele/ArduinoShare/tree/main/libraries/STM32F_CPU_Identity */
#include  <STM32_FSMC.h>  /* https://github.com/hamayanShowa-ele/ArduinoShare/tree/main/libraries/STM32_FSMC */
#include  <ARCNET.h>  /* https://github.com/hamayanShowa-ele/ArduinoShare/tree/main/libraries/ARCNET */
#include  <DEBUG_UTIL.h>  /* https://github.com/hamayanShowa-ele/ArduinoShare/tree/main/libraries/DEBUG_UTIL */

#if defined( BD_TYPE_1401 )
  #include  <pca8574.h>  /* https://github.com/hamayanShowa-ele/ArduinoShare/tree/main/libraries/pca8574 */
  #include  <bd1401a.h>  /* https://github.com/hamayanShowa-ele/ArduinoShare/blob/main/libraries/board_header/bd1401a.h */
#elif defined( BD_TYPE_1415 )
  #include  <bd1415a.h>  /* https://github.com/hamayanShowa-ele/ArduinoShare/blob/main/libraries/board_header/bd1415a.h */
#endif /* defined( BD_TYPE_1401 ) */

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if defined( BD_TYPE_1401 )
  TwoWire i2c1;
  PCA8574 ic11( &i2c1, IC11_I2C_ADDRESS );
#elif defined( BD_TYPE_1415 )
#endif /* defined( BD_TYPE_1401 ) */

STM32F_CPU_IDENTITY cpu_id;
ARCNET arcnet( (volatile void *)INT_ARCNET_BASE_ADDRESS, ARC_INT, true, false );
volatile uint8_t bdSW;
uint32_t reportTime,transmitTime;

/**
  * @brief  setup
  * @retval none
  */
void setup()
{
  pinMode( LED_BUILTIN, OUTPUT );

  /* arcnet reset start. */
  pinMode( ARC_RST, OUTPUT );  // IO_RST
  digitalWrite( ARC_RST, LOW );  // IO_RST
  delay( 10 );
  /* arcnet reset end. */
  digitalWrite( ARC_RST, HIGH );  // IO_RST

  /* bus configuration. */
#if defined( BD_TYPE_1401 )
  fsmc_address( 19 );  // set address bus size
  fsmc_data( 16 );  // set data bus size.
  fsmc_sram( FSMC_NORSRAM_BANK1, FSMC_NORSRAM_MEM_BUS_WIDTH_16, 2,12,2,12, 0 );
  fsmc_sram( FSMC_NORSRAM_BANK2, FSMC_NORSRAM_MEM_BUS_WIDTH_16, 2,12,2,12, 0 );
  fsmc_sram( FSMC_NORSRAM_BANK3, FSMC_NORSRAM_MEM_BUS_WIDTH_16, 4,12,4,12, 0 );
  /* read cycle:209ns, read address setup time:80ns, read width:86ns */
  /* write cycle:210ns, write address setup time:87ns, write width:71ns */
  fsmc_sram( FSMC_NORSRAM_BANK4, FSMC_NORSRAM_MEM_BUS_WIDTH_16, 8,8,8,8, 0 );
  /* read cycle:193ns, read address setup time:99ns, read width:48ns */
  /* write cycle:220ns, write address setup time:99ns, write width:46ns */
#elif defined( BD_TYPE_1415 )
  fsmc_address( 19 );  // set address bus size
  fsmc_data( 16 );  // set data bus size.
  fsmc_sram( FSMC_NORSRAM_BANK1, FSMC_NORSRAM_MEM_BUS_WIDTH_16, 1,4,1,4, 0 );
  fsmc_sram( FSMC_NORSRAM_BANK2, FSMC_NORSRAM_MEM_BUS_WIDTH_16, 1,4,1,4, 0 );
  fsmc_sram( FSMC_NORSRAM_BANK3, FSMC_NORSRAM_MEM_BUS_WIDTH_16, 3,6,3,6, 0 );
  /* read cycle:209ns, read address setup time:80ns, read width:86ns */
  /* write cycle:210ns, write address setup time:87ns, write width:71ns */
  fsmc_sram( FSMC_NORSRAM_BANK4, FSMC_NORSRAM_MEM_BUS_WIDTH_16, 10,13,10,15, 0 );
  /* read cycle:193ns, read address setup time:99ns, read width:48ns */
  /* write cycle:220ns, write address setup time:99ns, write width:46ns */
#endif /* defined( BD_TYPE_1401 ) */

  pinMode( USBON, OUTPUT );
  digitalWrite( USBON, USB_POWER_ON_LEVEL );
  Serial.begin( 115200UL );
  while( !Serial ){ delay(20UL); }
  Serial.println();
  Serial.println( "1401 or 1415 board testing program from USB Serial." );
  Serial.println( "1401 or 1415 Arcnet demo." );
  // cpu id and system clock.
  String msg = "systic:"; msg += String( cpu_id.sysload() );
  msg += " hclk:"; msg += String( cpu_id.hclk() );
  msg += " pclk1:"; msg += String( cpu_id.pclk1() );
  msg += " pclk2:"; msg += String( cpu_id.pclk2() );
  msg += " sysclock:"; msg += String( cpu_id.sysclk() );
  Serial.println( msg );

#if defined( BD_TYPE_1401 )
  /* i2c initialize start. */
  i2c1.setSDA( SDA1 );  /* SDA */
  i2c1.setSCL( SCL1 );  /* SCL */
  i2c1.begin();

  /* read board number. */
  ic11.write( 0xFF );  // initialize for read.
  bdSW = ic11.read();
  uint8_t tempUC = bdSW >> 4;
  bdSW &= 0x0F; bdSW *= 10;
  bdSW += tempUC;
  if( bdSW == 0 ) bdSW = 255;
#elif defined( BD_TYPE_1415 )
  bdSW = 255;
#endif /* defined( BD_TYPE_1401 ) */
  Serial.print( "BOARD SWITCH = " ); Serial.println( bdSW, DEC );

  /* Write access to the sram area and let the ARCNET controller determine the bus type. */
  uint32_t ret = memCheck( (uint8_t *)INT_SRAM_BASE_ADDRESS, INT_SRAM_SIZE );
  if( ret == 0 ) { Serial.println( "RAM is fine." ); }
  else { Serial.print( "There was a problem with the RAM at the following address. 0x" ); Serial.println( ret, HEX ); }
  //  dump( (const uint8_t *)INT_SRAM_BASE_ADDRESS, 256 );

  /* start arcnet controller. */
  Serial.println();
  int result;
  if( (result = arcnet.begin( bdSW )) != 0 )
  {
    Serial.print( "ARCNET INITIALIZE ERROR code = " ); Serial.println( result, DEC );
    while( 1 ) delay( 1000 );
  }
  /* start arcnet interrupt. */
  arcnet.regCBfunction( cbARCNET_INT );
  arcnet.enaInterrupt();
  Serial.println( "ARCNET INITIALIZE Done..." );

  Serial.print( "ARCNET is attempting to join the network." );
  while( 1 )
  {
    if( arcnet.joined() && arcnet.nextid() != 0 ) { break; }
    Serial.print( '.' );
    delay( 250 );
  }
  Serial.println( " joined." );

  reportTime = millis();
  transmitTime = millis();
}

/**
  * @brief  loop
  * @retval none
  */
void loop()
{
  if( (millis() - reportTime) >= 5 * 1000UL )
  {
    reportTime = millis();
    String msg = "";
    msg += "IMASK : 0x"; msg += String(arcnet.imask,HEX);
    msg += " TRANSMIT : "; msg += String(arcnet.transmit_count,DEC);  // transmit_count
    msg += " RECIEVE : "; msg += String(arcnet.recieve_complete_count,DEC);
    msg += " RECONFIG : "; msg += String(arcnet.reconfig_count,DEC);
    msg += " NEXT ID : "; msg += String(arcnet.nextid(),DEC);

    Serial.println( msg );
  }

  if( arcnet.available() )
  {
    uint8_t sid;
    const int size = 512;
    uint8_t *dat = new uint8_t[size];
    int len = arcnet.recv( &sid, dat, size );
    if( len > 0 )
    {
      String msg = "RECIEVE DATA from ID : "; msg += String( sid, DEC );
      msg += " len = "; msg += String( len, DEC ); Serial.print( msg );
      for( int i = 0; i < len; i++ )
      {
        if( (uint8_t)(i ^ sid) != dat[i] )
        {
          Serial.println( " but DATA is error." );
          dump( dat, len );
          break;
        }
      }
      Serial.println( " Done..." );
    }
    else { Serial.println( "RECIEVE DATA something error." ); }
    delete [] dat;
  }

  if( (millis() - transmitTime) >= 1 * 1000UL )
  {
    transmitTime = millis();
    uint8_t selfID = arcnet.nodeid();
    uint8_t did = arcnet.nextid();
    const int size = 253;
    uint8_t *dat = new uint8_t[size];
    for( int i = 0; i < size; i++ ) { dat[i] = (uint8_t)i ^ selfID; }
    int ret = arcnet.send( did, (const uint8_t *)dat, size );
    delete [] dat;
    String msg;
    if( ret == size )
    {
      msg = "TRANSMIT for ID:"; msg += String( did, DEC );
    }
    else
    {
      msg = "TRANSMIT error result:"; msg += String( ret, DEC );
    }
    Serial.println( msg );
  }
}

/**
  * @brief arcnet interrupt callback routine.
  * @retval none.
  */
void cbARCNET_INT( void )
{
  arcnet.intHandler();
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
#if defined( BD_TYPE_1401 )
#if defined( ARDUINO_GENERIC_F407ZETX )
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
#elif defined( ARDUINO_GENERIC_F446ZETX )
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  PeriphClkInitStruct.PLLSAI.PLLSAIM = 8;
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
#endif  /* defined( ARDUINO_GENERIC_F407ZETX ) */
#elif defined( BD_TYPE_1415 )
void SystemClock_Config(void)
{
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
}
#endif /* defined( BD_TYPE_1401 ) */

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
