/**
  ******************************************************************************
  * @file           : DEBUG_UTIL.h
  * @brief          : DEBUG_UTIL header

    Copyright (c) 2022 Kazuaki Hamahara
    Released under the MIT license
    https://github.com/YukinobuKurata/YouTubeMagicBuyButton/blob/master/MIT-LICENSE.txt
  ******************************************************************************
  */
#ifndef  __DEBUG_UTIL_H__
#define  __DEBUG_UTIL_H__

#include  <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined( STM32F4xx )
#elif defined( STM32F1xx )
#endif /* defined( STM32F4xx ) */

#ifdef __cplusplus
}
#endif

void dump( const uint8_t *dat, int size );
void dump( const uint16_t *dat, int size );
void dump( const uint32_t *dat, int size );
uint32_t memCheck( uint8_t *dat, uint32_t size );
uint32_t memCheck( uint16_t *dat, uint32_t size );
uint32_t memCheck( uint32_t *dat, uint32_t size );

#endif  /* __DEBUG_UTIL_H__ */
