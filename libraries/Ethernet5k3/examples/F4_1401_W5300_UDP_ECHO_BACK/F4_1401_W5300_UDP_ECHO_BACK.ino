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

//#include  <HardwareSerial.h>
#include  <DEBUG_UTIL.h>  // https://github.com/hamayanShowa-ele/ArduinoShare/tree/main/libraries/DEBUG_UTIL
#include  <STM32F_CPU_Identity.h>  // https://github.com/hamayanShowa-ele/ArduinoShare/tree/main/libraries/STM32F_CPU_Identity
#include  <STM32_FSMC.h>
#include  <Ethernet5k3.h>
#include  <socket.h>
#include  <bd1401a.h>

static const byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01, 0x01 };
static const IPAddress local_ip( 192,168,100,128 );
IPAddress myIP;
static const uint16_t localPort = 7;  // local port to listen for UDP packets
static const SOCKET sn = 7;

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

  // USB/Serial initialize.
  Serial.begin( 115200UL );
  while( !Serial ) { delay( 20 ); }
  Serial.println( "1401A W5300 make the Arduino library from USB Serial." );
  delay( 100UL );

  // External CPU BUS initialize.
  fsmc_address( 19 );  // set address bus size
  fsmc_data( 16 );  // set data bus size.
  fsmc_sram( FSMC_NORSRAM_BANK2, FSMC_NORSRAM_MEM_BUS_WIDTH_16, 0,12,0,9, 0 );

  // Ethernet initialize.
  Ethernet.init( W5300_MR_ADDRESS, W5300_IDM_AR_ADDRESS, W5300_IDM_DR_ADDRESS );
  Ethernet.begin( mac, local_ip );
  myIP = Ethernet.localIP();
  Serial.print( "\r\nIP number assigned by DHCP is " ); Serial.println( myIP );

  socket( sn, SnMR::UDP, localPort, 0 ); // Opens a socket(TCP or UDP or IP_RAW mode)
}

/**
  * @brief  loop
  * @retval none
  */
void loop()
{
  uint8_t *buf = new uint8_t[1460];
  uint8_t addr[4];
  uint16_t port;
  uint32_t rcvLen = recvfrom( sn, buf, 1460, addr, &port );
  if( rcvLen )
  {
    Serial.printf( "rcvLen=%u from %d.%d.%d.%d:%d\r\n",
      rcvLen,
      addr[0], addr[1], addr[2], addr[3], port );
    dump( (const uint8_t *)buf, 16 );
    sendto( sn, (const uint8_t *)buf, rcvLen, addr, port );
  }
  delete [] buf;
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
