/**
  ******************************************************************************
  * @file           : STM32_GPIO.h
  * @brief          : GPIO header
  ******************************************************************************
  */
#ifndef  __STM32x_GPIO_H__
#define  __STM32x_GPIO_H__

#include  <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined( STM32F4xx )
#include  <stm32f4xx_hal.h>
#include  <stm32f4xx_hal_gpio.h>
#elif defined( STM32F1xx )
#include  <stm32f1xx_hal.h>
#include  <stm32f1xx_hal_gpio.h>
#endif /* defined( STM32F4xx ) */

int getPortPinIndex( uint32_t p );
GPIO_TypeDef* getGPIOx( uint32_t p );
uint32_t getGPIO_PINx( uint32_t p );

#if defined( STM32F4xx )
//#define  GPIO_PIN_IS_0(G,P)  do{G->BSRRH = P << 0;}while(0)
//#define  GPIO_PIN_IS_1(G,P)  do{G->BSRRL = P << 0;}while(0)
#define  GPIO_PIN_IS_0(G,P)  do{G->BSRR = P << 16;}while(0)
#define  GPIO_PIN_IS_1(G,P)  do{G->BSRR = P << 0;}while(0)
#elif defined( STM32F1xx )
#define  GPIO_PIN_IS_0(G,P)  do{G->BSRR = P << 16;}while(0)
#define  GPIO_PIN_IS_1(G,P)  do{G->BSRR = P << 0;}while(0)
#endif /* defined( STM32F4xx ) */

#ifdef __cplusplus
}
#endif

#endif  /* __STM32x_GPIO_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
