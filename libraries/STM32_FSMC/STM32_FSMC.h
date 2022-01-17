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
