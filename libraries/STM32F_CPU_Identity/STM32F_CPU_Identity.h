/*
  STM32Fxxx CPU Identity

  Copyright (c) 2022 Kazuaki Hamahara
    Released under the MIT license
    https://github.com/YukinobuKurata/YouTubeMagicBuyButton/blob/master/MIT-LICENSE.txt
*/

#ifndef __STM32F_CPU_IDENTITY_H__
#define __STM32F_CPU_IDENTITY_H__

#include  <Arduino.h>

extern "C"
{
}

#define  UID_BASE_ADDRESS  UID_BASE

class STM32F_CPU_IDENTITY
{
private:

public:
  STM32F_CPU_IDENTITY() {}

  HAL_StatusTypeDef rccDeinit() { HAL_RCC_DeInit(); }
  HAL_StatusTypeDef oscConfig( RCC_OscInitTypeDef *RCC_OscInitStruct )
  {
    return HAL_RCC_OscConfig( RCC_OscInitStruct );
  }
  HAL_StatusTypeDef clkConfig( RCC_ClkInitTypeDef *RCC_ClkInitStruct, uint32_t FLatency )
  {
    return HAL_RCC_ClockConfig( RCC_ClkInitStruct, FLatency );
  }

  uint32_t sysload() { return SysTick->LOAD; }
  uint32_t sysclk() { return HAL_RCC_GetSysClockFreq(); }
  uint32_t hclk() { return HAL_RCC_GetHCLKFreq(); }
  uint32_t pclk1() { return HAL_RCC_GetPCLK1Freq(); }
  uint32_t pclk2() { return HAL_RCC_GetPCLK2Freq(); }
  void uID( uint32_t *id )
  {
    *(id + 0) = *((uint32_t *)(UID_BASE_ADDRESS + 0)); 
    *(id + 1) = *((uint32_t *)(UID_BASE_ADDRESS + 4)); 
    *(id + 2) = *((uint32_t *)(UID_BASE_ADDRESS + 8)); 
  }

  void mccConfig( uint32_t RCC_MCOx, uint32_t RCC_MCOSource, uint32_t RCC_MCODiv )
  {
    HAL_RCC_MCOConfig( RCC_MCOx,  RCC_MCOSource,  RCC_MCODiv );
  }

  void getOscConfig( RCC_OscInitTypeDef *RCC_OscInitStruct )
  {
    HAL_RCC_GetOscConfig( RCC_OscInitStruct);
  }
  void getClkConfig( RCC_ClkInitTypeDef *RCC_ClkInitStruct, uint32_t *pFLatency )
  {
    HAL_RCC_GetClockConfig( RCC_ClkInitStruct, pFLatency );
  }
};

#endif  /* __STM32Fxxx_CPU_IDENTITY_H__ */
