/*
  2113 board testing program.

  Acquire 16 channels of analog data and 16 channels of pulse data from the 2113 board.
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

#include  <MY_STREAM.h>  /* https://github.com/hamayanShowa-ele/ArduinoShare/tree/main/libraries/MY_STREAM */
#include <STM32_FSMC.h>
#include  <bd1401a.h>  /* https://github.com/hamayanShowa-ele/ArduinoShare/tree/main/libraries/board_header */
#include  <bd2113.h>  /* https://github.com/hamayanShowa-ele/ArduinoShare/tree/main/libraries/board_header */

#define  FREQ_OF_SIGNAL  50  /* Frequency of signal. */
#define  ADC_MAX_CHANNEL_NUMBERS  16
#define  ADC_MULTIPLE_SAMPLE_RATE 32
#define  PLS_MAX_CHANNEL_NUMBERS  1
#define  PLS_MULTIPLE_SAMPLE_RATE 32
#define  NUMBER_OF_SAMPLES_PER_SEC  (FREQ_OF_SIGNAL * ADC_MULTIPLE_SAMPLE_RATE)  /* Number of samples per second. */

static void cb_ADC_Interrupt( void );

BD2113 adc1( (volatile uint16_t *)(EXT_BASE_ADR + BD2113_ADC1_ADDRESS) );
BD2113 adc2( (volatile uint16_t *)(EXT_BASE_ADR + BD2113_ADC2_ADDRESS) );
BD2113 pulse( (volatile uint16_t *)(EXT_BASE_ADR + BD2113_PULSE_ADDRESS) );
MY_STREAM adFifo( ADC_MAX_CHANNEL_NUMBERS * ADC_MULTIPLE_SAMPLE_RATE, sizeof(uint16_t) );  /* analog data fifo. */
MY_STREAM plFifo( PLS_MAX_CHANNEL_NUMBERS * PLS_MULTIPLE_SAMPLE_RATE, sizeof(uint16_t) );  /* pulse data fifo. */
HardwareTimer Timer2( TIM2 );  /* CONVERT */
HardwareTimer Timer3( TIM3 );  /* FCLK */

volatile uint32_t adc_interrupt_count;
uint32_t adc_interrupt_count_Base;

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
  Serial.begin( 115200UL );
  while( !Serial ) { delay( 20 ); }
  Serial.println( "1401A 2113 board testing program from USB Serial." );
  delay( 100UL );

   /* MAX7401 low pass filter clock initialize and output. timer3 ch3 */
  Timer3.pause();  // Pause counter and all output channels
  Timer3.setPWM( 3, FCLK, 100 * 1000UL, 50UL );  /* timer3 ch3 */
  Timer3.resume(); // Resume counter and all output channels

  // External CPU BUS initialize.
  fsmc_address( 19 );  // set address bus size
  fsmc_data( 16 );  // set data bus size.
  fsmc_sram( FSMC_NORSRAM_BANK4, FSMC_NORSRAM_MEM_BUS_WIDTH_16, 1,12,1,9, 0 );

   /* ADC initialize. */
   adc1.write( 0x00FF );
   adc2.write( 0x00FF );

   /* AD Convert clock initialize and output. timer2 ch2 */
  Timer2.pause();  // Pause counter and all output channels
  Timer2.setPWM( 2, CONVERT, FREQ_OF_SIGNAL * ADC_MULTIPLE_SAMPLE_RATE, 90UL );
  Timer2.resume(); // Resume counter and all output channels

  adc_interrupt_count_Base = adc_interrupt_count;

 /* attach TxINT interrupt handler. */
  attachInterrupt( EXT_INT0, cb_ADC_Interrupt, FALLING );
}

/**
  * @brief  loop
  * @retval none
  */
void loop()
{
  static uint32_t loopCount = 0UL;
  static uint16_t adcBuffer[ADC_MAX_CHANNEL_NUMBERS][ADC_MULTIPLE_SAMPLE_RATE];
  static uint16_t plsBuffer[PLS_MAX_CHANNEL_NUMBERS][PLS_MULTIPLE_SAMPLE_RATE];

  if( adc_interrupt_count != adc_interrupt_count_Base )
  {
    adc_interrupt_count_Base++;
    for( int i = 0; i < ADC_MAX_CHANNEL_NUMBERS; i++ )
    {
      adcBuffer[ i ][ loopCount % ADC_MULTIPLE_SAMPLE_RATE ] = adFifo.read();
    }
    volatile uint16_t pluseData = plFifo.read();

    if( (loopCount % NUMBER_OF_SAMPLES_PER_SEC) == (NUMBER_OF_SAMPLES_PER_SEC - 1) )
    {
      String csv = "";
      for( int i = 0; i < ADC_MULTIPLE_SAMPLE_RATE; i++ )
      {
        for( int j = 0; j < ADC_MAX_CHANNEL_NUMBERS; j++ )
        {
          csv += String( adcBuffer[ j ][ i ], DEC ) + ",";
        }
        Serial.println( csv );
      }
    }
    loopCount++;
  }
}

/**
  * @brief  call back handler for 2113 ADC interrupt.
  * @retval none
  */
static void cb_ADC_Interrupt( void )
{
  volatile uint16_t tempUS;
  for( int i = 0; i < ADC_MAX_CHANNEL_NUMBERS / 2; i++ )
  {
    tempUS = adc1.read();
    adFifo.write( tempUS, 0UL );
  }
  for( int i = 0; i < ADC_MAX_CHANNEL_NUMBERS / 2; i++ )
  {
    tempUS = adc2.read();
    adFifo.write( tempUS, 0UL );
  }
  tempUS = pulse.read();
  plFifo.write( tempUS, 0UL );

  adc_interrupt_count++;
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
