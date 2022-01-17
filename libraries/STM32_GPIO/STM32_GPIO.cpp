/**
  ******************************************************************************
  * @file           : STM32_GPIO.cpp
  * @brief          : STM32_GPIO code
  */
/* Includes ------------------------------------------------------------------*/
#include "STM32_GPIO.h"

extern "C"
{

static const uint16_t port_numbers[] =
{
  #if defined(PA15)
  PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PA8,PA9,PA10,PA11,PA12,PA13,PA14,PA15,
  #endif
  #if defined(PB15)
  PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7,PB8,PB9,PB10,PB11,PB12,PB13,PB14,PB15,
  #endif
  #if defined(PC15)
  PC0,PC1,PC2,PC3,PC4,PC5,PC6,PC7,PC8,PC9,PC10,PC11,PC12,PC13,PC14,PC15,
  #endif
  #if defined(PD15)
  PD0,PD1,PD2,PD3,PD4,PD5,PD6,PD7,PD8,PD9,PD10,PD11,PD12,PD13,PD14,PD15,
  #endif
  #if defined(PE15)
  PE0,PE1,PE2,PE3,PE4,PE5,PE6,PE7,PE8,PE9,PE10,PE11,PE12,PE13,PE14,PE15,
  #endif
  #if defined(PF15)
  PF0,PF1,PF2,PF3,PF4,PF5,PF6,PF7,PF8,PF9,PF10,PF11,PF12,PF13,PF14,PF15,
  #endif
  #if defined(PG15)
  PG0,PG1,PG2,PG3,PG4,PG5,PG6,PG7,PG8,PG9,PG10,PG11,PG12,PG13,PG14,PG15,
  #endif
  #if defined(PH0)
  PH0,
  #endif
  #if defined(PH1)
  PH1,
  #endif
};

/**
  * @brief search bit order from table.
  * @retval bit order from LSB.
  */
int getPortPinIndex( uint32_t p )
{
  int ret = (-1);
  for( int i = 0; i < (int)(sizeof(port_numbers) / sizeof(port_numbers[0])); i++ )
  {
    if( port_numbers[i] == (uint16_t)p ) { ret = i; break; }
  }
  return ret;
}

/**
  * @brief search port instance from table.
  * @retval port instance.
  */
GPIO_TypeDef* getGPIOx( uint32_t p )
{
  int num = getPortPinIndex( p );
  #if defined(PA0)
  if( num <= (16 * 1) - 1 ) return GPIOA;
  #endif
  #if defined(PB0)
  else if( num <= (16 * 2) - 1 ) return GPIOB;
  #endif
  #if defined(PC0)
  else if( num <= (16 * 3) - 1 ) return GPIOC;
  #endif
  #if defined(PD0)
  else if( num <= (16 * 4) - 1 ) return GPIOD;
  #endif
  #if defined(PE0)
  else if( num <= (16 * 5) - 1 ) return GPIOE;
  #endif
  #if defined(PF0)
  else if( num <= (16 * 6) - 1 ) return GPIOF;
  #endif
  #if defined(PG0)
  else if( num <= (16 * 7) - 1 ) return GPIOG;
  #endif
  #if defined(PH0)
  else if( num <= (16 * 8) - 1 ) return GPIOH;
  #endif
  else return 0;
}

/**
  * @brief search port pin from table.
  * @retval port instance.
  */
uint32_t getGPIO_PINx( uint32_t p )
{
  int num = getPortPinIndex( p );
  num %= 16;
  return 0x00000001 << num;
}

}  /* extern "C" */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
