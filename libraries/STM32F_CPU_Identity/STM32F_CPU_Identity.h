/*
  STM32Fxxx CPU Identity
    Copyright (C) 2014 hamayan All Rights Reserved.
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
  STM32Fxxx_CPU_IDENTITY() {}

  uint32_t systicLoad() { return SysTick->LOAD; }
  uint32_t hclk() { return HAL_RCC_GetHCLKFreq(); }
  uint32_t pclk1() { return HAL_RCC_GetPCLK1Freq(); }
  uint32_t pclk2() { return HAL_RCC_GetPCLK2Freq(); }
  void uid( uint32_t *id )
  {
    *(id + 0) = *((uint32_t *)(UID_BASE_ADDRESS + 0)); 
    *(id + 1) = *((uint32_t *)(UID_BASE_ADDRESS + 4)); 
    *(id + 2) = *((uint32_t *)(UID_BASE_ADDRESS + 8)); 
  }
};


#endif  /* __STM32Fxxx_CPU_IDENTITY_H__ */

/*
  STM32Fxxx CPU Identity
    Copyright (C) 2014 hamayan All Rights Reserved.
*/
