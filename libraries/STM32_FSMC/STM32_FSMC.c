/**
  ******************************************************************************
  * @file           : STM32_FSMC.c
  * @brief          : fsmc code
    Copyright (c) 2022 Kazuaki Hamahara
    This source code retains the following license.
  ******************************************************************************
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
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "STM32_FSMC.h"

extern HAL_StatusTypeDef HAL_SRAM_Init(SRAM_HandleTypeDef *hsram, FMC_NORSRAM_TimingTypeDef *Timing, FMC_NORSRAM_TimingTypeDef *ExtTiming);

/* GPIO alternate initialization function. */
static void alterPin( GPIO_TypeDef *GPIOx, uint16_t pin )
{
  if( GPIOx == GPIOA ) __HAL_RCC_GPIOA_CLK_ENABLE();
  else if( GPIOx == GPIOB ) __HAL_RCC_GPIOB_CLK_ENABLE();
  else if( GPIOx == GPIOC ) __HAL_RCC_GPIOC_CLK_ENABLE();
  else if( GPIOx == GPIOD ) __HAL_RCC_GPIOD_CLK_ENABLE();
  else if( GPIOx == GPIOE ) __HAL_RCC_GPIOE_CLK_ENABLE();
  else if( GPIOx == GPIOF ) __HAL_RCC_GPIOF_CLK_ENABLE();
  else if( GPIOx == GPIOG ) __HAL_RCC_GPIOG_CLK_ENABLE();
#if defined( STM32F4xx )
else if( GPIOx == GPIOH ) __HAL_RCC_GPIOH_CLK_ENABLE();
#endif /* defined( STM32F4xx ) */
  else return;

  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
#if defined( STM32F4xx )
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
#elif defined( STM32F1xx )
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
#endif /* defined( STM32F4xx ) */
  HAL_GPIO_Init( GPIOx, &GPIO_InitStruct );
}

/* FSMC initialization function for address bit. */
int fsmc_address( int bit )
{
  if( bit < 1 || bit > 26 ) return 0;

  // A0
  alterPin( GPIOF, GPIO_PIN_0 ); if( bit == 1 ) return bit;
  // A1
  alterPin( GPIOF, GPIO_PIN_1 ); if( bit == 2 ) return bit;
  // A2
  alterPin( GPIOF, GPIO_PIN_2 ); if( bit == 3 ) return bit;
  // A3
  alterPin( GPIOF, GPIO_PIN_3 ); if( bit == 4 ) return bit;
  // A4
  alterPin( GPIOF, GPIO_PIN_4 ); if( bit == 5 ) return bit;
  // A5
  alterPin( GPIOF, GPIO_PIN_5 ); if( bit == 6 ) return bit;
  // A6
  alterPin( GPIOF, GPIO_PIN_12 ); if( bit == 7 ) return bit;
  // A7
  alterPin( GPIOF, GPIO_PIN_13 ); if( bit == 8 ) return bit;
  // A8
  alterPin( GPIOF, GPIO_PIN_14 ); if( bit == 9 ) return bit;
  // A9
  alterPin( GPIOF, GPIO_PIN_15 ); if( bit == 10 ) return bit;

  // A10
  alterPin( GPIOG, GPIO_PIN_0 ); if( bit == 11 ) return bit;
  // A11
  alterPin( GPIOG, GPIO_PIN_1 ); if( bit == 12 ) return bit;
  // A12
  alterPin( GPIOG, GPIO_PIN_2 ); if( bit == 13 ) return bit;
  // A13
  alterPin( GPIOG, GPIO_PIN_3 ); if( bit == 14 ) return bit;
  // A14
  alterPin( GPIOG, GPIO_PIN_4 ); if( bit == 15 ) return bit;
  // A15
  alterPin( GPIOG, GPIO_PIN_5 ); if( bit == 16 ) return bit;

  // A16
  alterPin( GPIOD, GPIO_PIN_11 ); if( bit == 17 ) return bit;
  // A17
  alterPin( GPIOD, GPIO_PIN_12 ); if( bit == 18 ) return bit;
  // A18
  alterPin( GPIOD, GPIO_PIN_13 ); if( bit == 19 ) return bit;

  // A19
  alterPin( GPIOE, GPIO_PIN_3 ); if( bit == 20 ) return bit;
  // A20
  alterPin( GPIOE, GPIO_PIN_4 ); if( bit == 21 ) return bit;
  // A21
  alterPin( GPIOE, GPIO_PIN_5 ); if( bit == 22 ) return bit;
  // A22
  alterPin( GPIOE, GPIO_PIN_6 ); if( bit == 23 ) return bit;
  // A23
  alterPin( GPIOE, GPIO_PIN_2 ); if( bit == 24 ) return bit;

  // A24
  alterPin( GPIOG, GPIO_PIN_13 ); if( bit == 25 ) return bit;
  // A25
  alterPin( GPIOG, GPIO_PIN_14 ); if( bit == 26 ) return bit;

  return 0;
}

/* FSMC initialization function for data bit. */
int fsmc_data( int bit )
{
  if( bit < 1 || bit > 16 ) return 0;

  // D0
  alterPin( GPIOD, GPIO_PIN_14 ); if( bit == 1 ) return bit;
  // D1
  alterPin( GPIOD, GPIO_PIN_15 ); if( bit == 2 ) return bit;
  // D2
  alterPin( GPIOD, GPIO_PIN_0 ); if( bit == 3 ) return bit;
  // D3
  alterPin( GPIOD, GPIO_PIN_1 ); if( bit == 4 ) return bit;

  // D4
  alterPin( GPIOE, GPIO_PIN_7 ); if( bit == 5 ) return bit;
  // D5
  alterPin( GPIOE, GPIO_PIN_8 ); if( bit == 6 ) return bit;
  // D6
  alterPin( GPIOE, GPIO_PIN_9 ); if( bit == 7 ) return bit;
  // D7
  alterPin( GPIOE, GPIO_PIN_10 ); if( bit == 8 ) return bit;
  // D8
  alterPin( GPIOE, GPIO_PIN_11 ); if( bit == 9 ) return bit;
  // D9
  alterPin( GPIOE, GPIO_PIN_12 ); if( bit == 10 ) return bit;
  // D10
  alterPin( GPIOE, GPIO_PIN_13 ); if( bit == 11 ) return bit;
  // D11
  alterPin( GPIOE, GPIO_PIN_14 ); if( bit == 12 ) return bit;
  // D12
  alterPin( GPIOE, GPIO_PIN_15 ); if( bit == 13 ) return bit;

  // D13
  alterPin( GPIOD, GPIO_PIN_8 ); if( bit == 14 ) return bit;
  // D14
  alterPin( GPIOD, GPIO_PIN_9 ); if( bit == 15 ) return bit;
  // D15
  alterPin( GPIOD, GPIO_PIN_10 ); if( bit == 16 ) return bit;

  return 0;
}

/* FSMC initialization function for sram mode. */
/*
  uint32_t ne,  // bank
  uint32_t width,  // bus width
  uint32_t rAS,  // read address setup time max=15
  uint32_t rDS,  // read data setup time max=255
  uint32_t wAS,  // write address setup time max=15
  uint32_t wDS,  // write data setup time max=255
  uint8_t wait  // If this value is not 0, the WAIT pin is enabled.
*/
void fsmc_sram(
  uint32_t ne, uint32_t width,
  uint32_t rAS, uint32_t rDS, uint32_t wAS, uint32_t wDS,
  uint8_t wait )
{
  if( rAS > 15 ) rAS = 15;
  if( wAS > 15 ) wAS = 15;
  if( rDS > 255 ) rDS = 255;
  if( wDS > 255 ) wDS = 255;

  __HAL_RCC_FSMC_CLK_ENABLE();
  // NOE,NWE
  alterPin( GPIOD, GPIO_PIN_4 );  // NOE
  alterPin( GPIOD, GPIO_PIN_5 );  // NWE
  // NBL0,NBL1
  alterPin( GPIOE, GPIO_PIN_0 );  // NBL0
  alterPin( GPIOE, GPIO_PIN_1 );  // NBL1
  // WAIT
  if( wait != 0 ) alterPin( GPIOD, GPIO_PIN_6 );  // WAIT

  if( ne == FSMC_NORSRAM_BANK1 )  // NE1
  {
    alterPin( GPIOD, GPIO_PIN_7 );
  }
  else if( ne == FSMC_NORSRAM_BANK2 )  // NE2
  {
    alterPin( GPIOG, GPIO_PIN_9 );
  }
  else if( ne == FSMC_NORSRAM_BANK3 )  // NE3
  {
    alterPin( GPIOG, GPIO_PIN_10 );
  }
  else if( ne == FSMC_NORSRAM_BANK4 )  // NE4
  {
    alterPin( GPIOG, GPIO_PIN_12 );
  }
  else return;

  /** Perform the SRAM? memory initialization sequence */
  SRAM_HandleTypeDef hsram;
//  HAL_SRAM_DeInit( &hsram );
  hsram.Instance = FSMC_NORSRAM_DEVICE;
  hsram.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram.Init */
  hsram.Init.NSBank = ne;
  hsram.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hsram.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  hsram.Init.MemoryDataWidth = width;
  hsram.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  hsram.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  hsram.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
  hsram.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  hsram.Init.ExtendedMode = FSMC_EXTENDED_MODE_ENABLE;
  hsram.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
  hsram.Init.PageSize = FSMC_PAGE_SIZE_NONE;

  /* Timing */
  FSMC_NORSRAM_TimingTypeDef Timing;
  Timing.AddressSetupTime = rAS;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = rDS;
  Timing.BusTurnAroundDuration = 15;
  Timing.CLKDivision = 15;
  Timing.DataLatency = 15;
  Timing.AccessMode = FSMC_ACCESS_MODE_A;
  /* ExtTiming */
  FSMC_NORSRAM_TimingTypeDef ExtTiming;
  ExtTiming.AddressSetupTime = wAS;
  ExtTiming.AddressHoldTime = 15;
  ExtTiming.DataSetupTime = wDS;
  ExtTiming.BusTurnAroundDuration = 15;
  ExtTiming.CLKDivision = 15;
  ExtTiming.DataLatency = 15;
  ExtTiming.AccessMode = FSMC_ACCESS_MODE_A;

  if (HAL_SRAM_Init(&hsram, &Timing, &ExtTiming) != HAL_OK)
  {
    user_Error_Handler( );
  }
}
