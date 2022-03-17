/*
  1314 board driven by stm32f103vet header.
    Copyright (C) 2014 hamayan All Rights Reserved.
                       hamayan.contact@gmail.com
*/
#ifndef __BOARD_DEPENDENCY_H__
#define __BOARD_DEPENDENCY_H__

#include  <Arduino.h>

extern "C"
{
}

#ifndef _MULTITASK_H_
  #define  rot_rdq()
  #define  dly_tsk(dly)  delay(dly)
#endif  /* _MULTITASK_H_ */


/* LCD BACKLIGHT : PC4 */
#define  BACKLIGHT_PORT GPIOC
#define  BACKLIGHT_PIN  GPIO_PIN_4
#define  BACKLIGHT_IS_0 BACKLIGHT_PORT->BSRR = (BACKLIGHT_PIN << 16)
#define  BACKLIGHT_IS_1 BACKLIGHT_PORT->BSRR = (BACKLIGHT_PIN << 0)
/* LCD DI : PC5 */
#define  LCD_DI_PORT    GPIOC
#define  LCD_DI_PIN     GPIO_PIN_5
#define  LCD_DI_IS_0    LCD_DI_PORT->BSRR = (LCD_DI_PIN << 16)
#define  LCD_DI_IS_1    LCD_DI_PORT->BSRR = (LCD_DI_PIN << 0)
/* LCD RW : PB0 */
#define  LCD_RW_PORT    GPIOB
#define  LCD_RW_PIN     GPIO_PIN_0
#define  LCD_RW_IS_0    LCD_RW_PORT->BSRR = (LCD_RW_PIN << 16)
#define  LCD_RW_IS_1    LCD_RW_PORT->BSRR = (LCD_RW_PIN << 0)
/* LCD E : PB1 */
#define  LCD_E_PORT     GPIOB
#define  LCD_E_PIN      GPIO_PIN_1
#define  LCD_E_IS_0     LCD_E_PORT->BSRR = (LCD_E_PIN << 16)
#define  LCD_E_IS_1     LCD_E_PORT->BSRR = (LCD_E_PIN << 0)
/* LCD CS1 : PB10 */
#define  LCD_CS1_PORT   GPIOB
#define  LCD_CS1_PIN    GPIO_PIN_10
#define  LCD_CS1_IS_0   LCD_CS1_PORT->BSRR = (LCD_CS1_PIN << 16)
#define  LCD_CS1_IS_1   LCD_CS1_PORT->BSRR = (LCD_CS1_PIN << 0)
/* LCD CS2 : PB11 */
#define  LCD_CS2_PORT   GPIOB
#define  LCD_CS2_PIN    GPIO_PIN_11
#define  LCD_CS2_IS_0   LCD_CS2_PORT->BSRR = (LCD_CS2_PIN << 16)
#define  LCD_CS2_IS_1   LCD_CS2_PORT->BSRR = (LCD_CS2_PIN << 0)
/* LCD RST : PE7 */
#define  LCD_RST_PORT   GPIOE
#define  LCD_RST_PIN    GPIO_PIN_7
#define  LCD_RST_IS_0   LCD_RST_PORT->BSRR = (LCD_RST_PIN << 16)
#define  LCD_RST_IS_1   LCD_RST_PORT->BSRR = (LCD_RST_PIN << 0)
/* LCD DATA BUS : PE8~15 */
#define  LCD_DATA_PORT  GPIOE
#define  LCD_DATA_D0_PIN  GPIO_PIN_8
#define  LCD_DATA_D1_PIN  GPIO_PIN_9
#define  LCD_DATA_D2_PIN  GPIO_PIN_10
#define  LCD_DATA_D3_PIN  GPIO_PIN_11
#define  LCD_DATA_D4_PIN  GPIO_PIN_12
#define  LCD_DATA_D5_PIN  GPIO_PIN_13
#define  LCD_DATA_D6_PIN  GPIO_PIN_14
#define  LCD_DATA_D7_PIN  GPIO_PIN_15

/* PE1 :USBON */
#define  USBON_PORT     GPIOE
#define  USBON_PIN      GPIO_PIN_1
#define  USBON_IS_0     USBON_PORT->BSRR = (USBON_PIN << 16)
#define  USBON_IS_1     USBON_PORT->BSRR = (USBON_PIN << 0)

/* Mac address eeprom */
#if 1
#define  I2C1_PORT      GPIOB
#define  I2C1_SCL_PIN   GPIO_PIN_8
#define  I2C1_SDA_PIN   GPIO_PIN_9
#define  I2C1_SCL_IS_0  I2C1_PORT->BSRR = (I2C1_SCL_PIN << 16)
#define  I2C1_SCL_IS_1  I2C1_PORT->BSRR = (I2C1_SCL_PIN << 0)
#define  I2C1_SDA_IS_0  I2C1_PORT->BSRR = (I2C1_SCL_PIN << 16)
#define  I2C1_SDA_IS_1  I2C1_PORT->BSRR = (I2C1_SCL_PIN << 0)
#else
#define  I2C2_PORT      GPIOB
#define  I2C2_SCL_PIN   GPIO_PIN_10
#define  I2C2_SDA_PIN   GPIO_PIN_11
#define  I2C2_SCL_IS_0  I2C2_PORT->BSRR = (I2C2_SCL_PIN << 16)
#define  I2C2_SCL_IS_1  I2C2_PORT->BSRR = (I2C2_SCL_PIN << 0)
#define  I2C2_SDA_IS_0  I2C2_PORT->BSRR = (I2C2_SCL_PIN << 16)
#define  I2C2_SDA_IS_1  I2C2_PORT->BSRR = (I2C2_SCL_PIN << 0)
#endif

/* ETH1,2 RESET : PD5*/
#define  ETH_RST_PORT   GPIOD
#define  ETH_RST_PIN    GPIO_PIN_5
#define  ETH_RST_IS_0   ETH_RST_PORT->BSRR = (ETH_RST_PIN << 16)
#define  ETH_RST_IS_1   ETH_RST_PORT->BSRR = (ETH_RST_PIN << 0)

/* ETH1 CS : PD7*/
#define  ETH1_CS_PORT   GPIOD
#define  ETH1_CS_PIN    GPIO_PIN_7
#define  ETH1_CS_IS_0   ETH1_CS_PORT->BSRR = (ETH1_CS_PIN << 16)
#define  ETH1_CS_IS_1   ETH1_CS_PORT->BSRR = (ETH1_CS_PIN << 0)

/* ETH2 CS : PD6*/
#define  ETH2_CS_PORT   GPIOD
#define  ETH2_CS_PIN    GPIO_PIN_6
#define  ETH2_CS_IS_0   ETH2_CS_PORT->BSRR = (ETH2_CS_PIN << 16)
#define  ETH2_CS_IS_1   ETH2_CS_PORT->BSRR = (ETH2_CS_PIN << 0)

/* ACT LED : PE0 */
#define  ACTLED_PORT    GPIOE
#define  ACTLED_PIN     GPIO_PIN_0
#define  ACTLED_IS_0    ACTLED_PORT->BSRR = (ACTLED_PIN << 16)
#define  ACTLED_IS_1    ACTLED_PORT->BSRR = (ACTLED_PIN << 0)

typedef struct
{
  uint8_t e;   /* enable */
  uint8_t di;  /* data(HIGH) or instruction(LOW) */
  uint8_t rw;  /* read(HIGH) or write(LOW) */
  uint8_t cs1;  /* chip select 1 (active HIGH) */
  uint8_t cs2;  /* chip select 2 (active HIGH) */
  uint8_t rst;  /* reset */
  uint8_t d0;   /* data bit 0 */
  uint8_t d1;   /* data bit 1 */
  uint8_t d2;   /* data bit 2 */
  uint8_t d3;   /* data bit 3 */
  uint8_t d4;   /* data bit 4 */
  uint8_t d5;   /* data bit 5 */
  uint8_t d6;   /* data bit 6 */
  uint8_t d7;   /* data bit 7 */
  uint8_t bl;   /* back light */
} SG12864_PINS;


class SG12864_IF
{
private:
  SG12864_PINS pins;
//  uint8_t blPin;  /* back light pin. active HIGH */
  GPIO_InitTypeDef GPIO_Init;

  void busWriteMode();
  void busReadMode();

  void byteWrite( uint8_t dat );
  uint8_t byteRead();

  void cmdWrite( uint8_t cmd );
  uint8_t stsRead();

  void dataWrite( uint8_t dat );
  void dataWrite( const uint8_t *dat, int size );
  uint8_t dataRead();
  int  dataRead( uint8_t *dat, int size );

public:
  SG12864_IF() {}
  SG12864_IF( const SG12864_PINS *_pins ) { pins = *_pins; }

  void reset() { pinMode( pins.rst, OUTPUT ); digitalWrite( pins.rst, LOW ); delay( 2UL ); digitalWrite( pins.rst, HIGH ); }

  void begin();
  void backLight( bool onOff ) { BACKLIGHT_PORT->BSRR = ( onOff ) ? (BACKLIGHT_PIN << 0) : (BACKLIGHT_PIN << 16); }


  void cs1cmdWrite( uint8_t cmd );
  void cs2cmdWrite( uint8_t cmd );
  uint8_t cs1stsRead();
  uint8_t cs2stsRead();

  void cs1DataWrite( uint8_t dat );
  void cs1DataWrite( const uint8_t *dat, int size );
  void cs2DataWrite( uint8_t dat );
  void cs2DataWrite( const uint8_t *dat, int size );
  uint8_t cs1DataRead();
  int  cs1DataRead( uint8_t *dat, int size );
  uint8_t cs2DataRead();
  int  cs2DataRead( uint8_t *dat, int size );
};


#endif  /* __BOARD_DEPENDENCY_H__ */

/****************************************************************************/
/*                         Copyright (C) 2014 hamayan All Rights Reserved.  */
/*                                                hamayan.contact@gmail.com */
/****************************************************************************/
