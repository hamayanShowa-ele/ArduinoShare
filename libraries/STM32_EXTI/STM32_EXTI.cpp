/**
  ******************************************************************************
  * @file           : STM32_EXTI.cpp
  * @brief          : STM32_EXTI code
    Copyright (c) 2022 Kazuaki Hamahara
    Released under the MIT license
    https://github.com/YukinobuKurata/YouTubeMagicBuyButton/blob/master/MIT-LICENSE.txt
  */
/* Includes ------------------------------------------------------------------*/
#include "STM32_EXTI.h"

extern "C"
{
uint8_t exti_update[ EXTI_INTERRUPT_NUMBERS ];

/**
  * @brief EXTI interrupt.
  * @retval none.
  */
static void cbEXTI_00( void ) { exti_update[0]++; }
static void cbEXTI_01( void ) { exti_update[1]++; }
static void cbEXTI_02( void ) { exti_update[2]++; }
static void cbEXTI_03( void ) { exti_update[3]++; }
static void cbEXTI_04( void ) { exti_update[4]++; }
static void cbEXTI_05( void ) { exti_update[5]++; }
static void cbEXTI_06( void ) { exti_update[6]++; }
static void cbEXTI_07( void ) { exti_update[7]++; }
static void cbEXTI_08( void ) { exti_update[8]++; }
static void cbEXTI_09( void ) { exti_update[9]++; }
static void cbEXTI_10( void ) { exti_update[10]++; }
static void cbEXTI_11( void ) { exti_update[11]++; }
static void cbEXTI_12( void ) { exti_update[12]++; }
static void cbEXTI_13( void ) { exti_update[13]++; }
static void cbEXTI_14( void ) { exti_update[14]++; }
static void cbEXTI_15( void ) { exti_update[15]++; }

static void (*cbEXTI[EXTI_INTERRUPT_NUMBERS])(void) =
{
  cbEXTI_00,  // exti 0
  cbEXTI_01,  // exti 1
  cbEXTI_02,  // exti 2
  cbEXTI_03,  // exti 3
  cbEXTI_04,  // exti 4
  cbEXTI_05,  // exti 5
  cbEXTI_06,  // exti 6
  cbEXTI_07,  // exti 7
  cbEXTI_08,  // exti 8
  cbEXTI_09,  // exti 9
  cbEXTI_10,  // exti 10
  cbEXTI_11,  // exti 11
  cbEXTI_12,  // exti 12
  cbEXTI_13,  // exti 13
  cbEXTI_14,  // exti 14
  cbEXTI_15,  // exti 15
};

}  /* extern "C" */

/**
  * @brief attach EXTI interrupt.
  * @parameter select mode from CHANGE or FALLING or RISING.
  * @retval none.
  */
void attachEXTI( uint32_t pin, uint32_t md )
{
  attachInterrupt( pin, cbEXTI[ getPortPinIndex(pin) % 16 ], md );
}
void attachEXTI( uint32_t pin, void (*cb)(void), uint32_t md )
{
  attachInterrupt( pin, cb, md );
}

/**
  * @brief detach EXTI interrupt.
  * @retval none.
  */
void detachEXTI( uint32_t pin )
{
  detachInterrupt( pin );
}
