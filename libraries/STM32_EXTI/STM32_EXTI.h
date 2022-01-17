/**
  ******************************************************************************
  * @file           : STM32_EXTI.h
  * @brief          : EXTI header
  ******************************************************************************
  */
#ifndef  __STM32x_EXTI_H__
#define  __STM32x_EXTI_H__

#include  <Arduino.h>
#include  <STM32_GPIO.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined( STM32F4xx )
#include  <stm32f4xx_hal.h>
//#include  <stm32f4xx_hal_gpio.h>
#elif defined( STM32F1xx )
#include  <stm32f1xx_hal.h>
//#include  <stm32f1xx_hal_gpio.h>
#endif /* defined( STM32F4xx ) */

#ifdef __cplusplus
}
#endif

#define EXTI_INTERRUPT_NUMBERS  16

void attachEXTI( uint32_t pin, uint32_t md );
void attachEXTI( uint32_t pin, void (*cb)(void), uint32_t md );
void detachEXTI( uint32_t pin );

#endif  /* __STM32x_EXTI_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
