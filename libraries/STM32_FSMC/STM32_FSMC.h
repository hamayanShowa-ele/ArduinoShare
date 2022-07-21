/**
  ******************************************************************************
  * @file           : STM32_FSMC.h
  * @brief          : fsmc header
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
#ifndef  __STM32Fx_FSMC_H__
#define  __STM32Fx_FSMC_H__

#ifdef __cplusplus
extern "C" {
#endif

#if defined( STM32F4xx )
#include  <stm32f4xx_hal.h>
#include  <stm32f4xx_hal_sram.h>
#include  <stm32f4xx_hal_gpio.h>
#elif defined( STM32F1xx )
#include  <stm32f1xx_hal.h>
#include  <stm32f1xx_hal_sram.h>
#include  <stm32f1xx_hal_gpio.h>
#endif /* defined( STM32F4xx ) */

#ifdef __STM32F4xx_LL_FMC_H
#define  FSMC_NORSRAM_BANK1  FMC_NORSRAM_BANK1
#define  FSMC_NORSRAM_BANK2  FMC_NORSRAM_BANK2
#define  FSMC_NORSRAM_BANK3  FMC_NORSRAM_BANK3
#define  FSMC_NORSRAM_BANK4  FMC_NORSRAM_BANK4

#define  FSMC_NORSRAM_MEM_BUS_WIDTH_8   FMC_NORSRAM_MEM_BUS_WIDTH_8
#define  FSMC_NORSRAM_MEM_BUS_WIDTH_16  FMC_NORSRAM_MEM_BUS_WIDTH_16
#define  FSMC_NORSRAM_MEM_BUS_WIDTH_32  FMC_NORSRAM_MEM_BUS_WIDTH_32

#define  FSMC_NORSRAM_DEVICE           FMC_NORSRAM_DEVICE
#define  FSMC_NORSRAM_EXTENDED_DEVICE  FMC_NORSRAM_EXTENDED_DEVICE
#define  FSMC_NORSRAM_TimingTypeDef    FMC_NORSRAM_TimingTypeDef

#define  FSMC_ACCESS_MODE_A              FMC_ACCESS_MODE_A
#define  FSMC_MEMORY_TYPE_SRAM           FMC_MEMORY_TYPE_SRAM
#define  FSMC_WRAP_MODE_DISABLE          FMC_WRAP_MODE_DISABLE
#define  FSMC_EXTENDED_MODE_ENABLE       FMC_EXTENDED_MODE_ENABLE
#define  FSMC_DATA_ADDRESS_MUX_DISABLE   FMC_DATA_ADDRESS_MUX_DISABLE
#define  FSMC_BURST_ACCESS_MODE_DISABLE  FMC_BURST_ACCESS_MODE_DISABLE

#define  FSMC_WAIT_SIGNAL_DISABLE       FMC_WAIT_SIGNAL_DISABLE
#define  FSMC_WAIT_TIMING_BEFORE_WS     FMC_WAIT_TIMING_BEFORE_WS
#define  FSMC_WAIT_SIGNAL_POLARITY_LOW  FMC_WAIT_SIGNAL_POLARITY_LOW

#define  FSMC_WRITE_BURST_DISABLE     FMC_WRITE_BURST_DISABLE
#define  FSMC_WRITE_OPERATION_ENABLE  FMC_WRITE_OPERATION_ENABLE

#define  FSMC_PAGE_SIZE_NONE  FMC_PAGE_SIZE_NONE

#define  FSMC_ASYNCHRONOUS_WAIT_DISABLE  FMC_ASYNCHRONOUS_WAIT_DISABLE

#define  __HAL_RCC_FSMC_CLK_ENABLE  __HAL_RCC_FMC_CLK_ENABLE

#define  GPIO_AF12_FSMC  GPIO_AF12_FMC
#endif /* __STM32F4xx_LL_FMC_H */

/* Exported functions prototypes ---------------------------------------------*/
void user_Error_Handler(void);
int  fsmc_address( int bit );
int  fsmc_data( int bit );
void fsmc_sram(
  uint32_t ne, uint32_t width,  // ne:FSMC_NORSRAM_BANK1,2,3,4 width:FSMC_NORSRAM_MEM_BUS_WIDTH_8,16,32
  uint32_t rAS, uint32_t rDS, uint32_t wAS, uint32_t wDS,
  uint8_t wait );

#ifdef __cplusplus
}
#endif

#endif  /* __STM32Fx_FSMC_H__ */
