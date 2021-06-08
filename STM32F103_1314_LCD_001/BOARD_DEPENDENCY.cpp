/*
  1314 board driven by stm32f103vet code.
    Copyright (C) 2014 hamayan All Rights Reserved.
                       hamayan.contact@gmail.com
*/
#include  "BOARD_DEPENDENCY.h"

extern "C"
{

/*
  wait for 140ns,,, perhaps.
*/
static void wait140ns( void )
{
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
}

/*
  wait for 200ns,,, perhaps.
*/
static void wait200ns( void )
{
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
}

/*
  wait for 450ns,,, perhaps.
*/
static void wait450ns( void )
{
  wait200ns();
  wait200ns();
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
  __NOP();  /* It takes 14ns per cpu clock. */
}

}  /* end of extern C */

/*
  constructor,destructor
*/


/*
  data bus direction will be write mode.
*/
void SG12864_IF::busWriteMode()
{
  GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init( LCD_DATA_PORT, &GPIO_Init );
}


/*
  data bus direction will be rwad mode.
*/
void SG12864_IF::busReadMode()
{
  GPIO_Init.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init( LCD_DATA_PORT, &GPIO_Init );
}


/*
  sg12864 begin.
*/
void SG12864_IF::begin()
{
  GPIO_Init.Pin =
    GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |GPIO_PIN_11 |
    GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_Init.Speed = GPIO_SPEED_FREQ_HIGH;

  digitalWrite( pins.e, LOW );
  digitalWrite( pins.di, LOW );
  digitalWrite( pins.rw, LOW );
  digitalWrite( pins.cs1, LOW );
  digitalWrite( pins.cs2, LOW );
  digitalWrite( pins.rst, LOW );

  pinMode( pins.e, OUTPUT );
  pinMode( pins.di, OUTPUT );
  pinMode( pins.rw, OUTPUT );
  pinMode( pins.cs1, OUTPUT );
  pinMode( pins.cs2, OUTPUT );
  pinMode( pins.rst, OUTPUT );

  pinMode( pins.d0, INPUT );
  pinMode( pins.d1, INPUT );
  pinMode( pins.d2, INPUT );
  pinMode( pins.d3, INPUT );
  pinMode( pins.d4, INPUT );
  pinMode( pins.d5, INPUT );
  pinMode( pins.d6, INPUT );
  pinMode( pins.d7, INPUT );
  busReadMode();

  digitalWrite( pins.bl, LOW );
  pinMode( pins.bl, OUTPUT );

  reset();
}


/*
  sg12864 byte write.
*/
void SG12864_IF::byteWrite( uint8_t dat )
{
//  busWriteMode();
  LCD_RW_IS_0;  /* LOW is write mode. */
  LCD_E_IS_0;
  wait140ns();
  LCD_E_IS_1;  /* 31ns step */
#if 0
  LCD_DATA_PORT->BSRR = ( dat & 0x80 ) ? (LCD_DATA_D7_PIN << 0) : (LCD_DATA_D7_PIN << 16);  /* shift 0 is HIGH, shift 16 is LOW. */
  LCD_DATA_PORT->BSRR = ( dat & 0x40 ) ? (LCD_DATA_D6_PIN << 0) : (LCD_DATA_D6_PIN << 16);  /* shift 0 is HIGH, shift 16 is LOW. */
  LCD_DATA_PORT->BSRR = ( dat & 0x20 ) ? (LCD_DATA_D5_PIN << 0) : (LCD_DATA_D5_PIN << 16);  /* shift 0 is HIGH, shift 16 is LOW. */
  LCD_DATA_PORT->BSRR = ( dat & 0x10 ) ? (LCD_DATA_D4_PIN << 0) : (LCD_DATA_D4_PIN << 16);  /* shift 0 is HIGH, shift 16 is LOW. */
  LCD_DATA_PORT->BSRR = ( dat & 0x08 ) ? (LCD_DATA_D3_PIN << 0) : (LCD_DATA_D3_PIN << 16);  /* shift 0 is HIGH, shift 16 is LOW. */
  LCD_DATA_PORT->BSRR = ( dat & 0x04 ) ? (LCD_DATA_D2_PIN << 0) : (LCD_DATA_D2_PIN << 16);  /* shift 0 is HIGH, shift 16 is LOW. */
  LCD_DATA_PORT->BSRR = ( dat & 0x02 ) ? (LCD_DATA_D1_PIN << 0) : (LCD_DATA_D1_PIN << 16);  /* shift 0 is HIGH, shift 16 is LOW. */
  LCD_DATA_PORT->BSRR = ( dat & 0x01 ) ? (LCD_DATA_D0_PIN << 0) : (LCD_DATA_D0_PIN << 16);  /* shift 0 is HIGH, shift 16 is LOW. */
#else
  __disable_irq();  /*disable interrupt*/
  volatile uint32_t temp = LCD_DATA_PORT->ODR;
  temp &= 0xffff00ff;
  temp |= (uint32_t)dat << 8;
  LCD_DATA_PORT->ODR = temp;
  __enable_irq();  /*enable interrupt*/
#endif
  wait450ns();
  LCD_E_IS_0;
//  busReadMode();
}


/*
  sg12864 byte read.
*/
uint8_t SG12864_IF::byteRead()
{
  LCD_RW_IS_1;  /* HIGH is read mode. */
  LCD_E_IS_0;  /* 31ns step */
  wait140ns();

  LCD_E_IS_1;  /* 31ns step */
  wait140ns();
  wait200ns();
  uint32_t dat = LCD_DATA_PORT->IDR;

  wait140ns();
  LCD_E_IS_0;

  return (uint8_t)(dat >> 8);
}


/*
  sg12864 command write.
*/
void SG12864_IF::cmdWrite( uint8_t cmd )
{
  busWriteMode();
  LCD_DI_IS_0;  /* LOW is instructions mode. */
  byteWrite( cmd );
  busReadMode();
}


/*
  sg12864 cs1 command write.
*/
void SG12864_IF::cs1cmdWrite( uint8_t cmd )
{
  LCD_CS1_IS_1;  /* HIGH is enable. */
  cmdWrite( cmd );
  LCD_CS1_IS_0;
}


/*
  sg12864 cs2 command write.
*/
void SG12864_IF::cs2cmdWrite( uint8_t cmd )
{
  LCD_CS2_IS_1;  /* HIGH is enable. */
  cmdWrite( cmd );
  LCD_CS2_IS_0;
}


/*
  sg12864 status read.
*/
uint8_t SG12864_IF::stsRead()
{
  LCD_DI_IS_0;  /* LOW is instructions mode. */
  uint8_t sts = byteRead();

  return sts;
}


/*
  sg12864 cs1 status read.
*/
uint8_t SG12864_IF::cs1stsRead()
{
  LCD_CS1_IS_1;  /* HIGH is enable. */
  uint8_t sts = stsRead();
  LCD_CS1_IS_0;

  return sts;
}


/*
  sg12864 cs2 status read.
*/
uint8_t SG12864_IF::cs2stsRead()
{
  LCD_CS2_IS_1;  /* HIGH is enable. */
  uint8_t sts = stsRead();
  LCD_CS2_IS_0;

  return sts;
}


/*
  sg12864 data write.
*/
void SG12864_IF::dataWrite( uint8_t dat )
{
  busWriteMode();
  LCD_DI_IS_1;  /* HIGH is data mode. */
  byteWrite( dat );
  busReadMode();
}

void SG12864_IF::dataWrite( const uint8_t *dat, int size )
{
  busWriteMode();
  LCD_DI_IS_1;  /* HIGH is data mode. */
  for( int i = 0; i < size; i++ )
  {
    byteWrite( *dat++ );
    delayMicroseconds( 5UL );
    rot_rdq();
  }
  busReadMode();
}


/*
  sg12864 cs1 data write.
*/
void SG12864_IF::cs1DataWrite( uint8_t dat )
{
  LCD_CS1_IS_1;  /* HIGH is enable. */
  dataWrite( dat );
  LCD_CS1_IS_0;
}

void SG12864_IF::cs1DataWrite( const uint8_t *dat, int size )
{
  LCD_CS1_IS_1;  /* HIGH is enable. */
  dataWrite( dat, size );
  LCD_CS1_IS_0;
}

/*
  sg12864 cs2 data write.
*/
void SG12864_IF::cs2DataWrite( uint8_t dat )
{
  LCD_CS2_IS_1;  /* HIGH is enable. */
  dataWrite( dat );
  LCD_CS2_IS_0;
}

void SG12864_IF::cs2DataWrite( const uint8_t *dat, int size )
{
  LCD_CS2_IS_1;  /* HIGH is enable. */
  dataWrite( dat, size );
  LCD_CS2_IS_0;
}

/*
  sg12864 data read.
*/
uint8_t SG12864_IF::dataRead()
{
  LCD_DI_IS_1;  /* HIGH is data mode. */
  uint8_t dat = byteRead();

  return dat;
}

int SG12864_IF::dataRead( uint8_t *dat, int size )
{
  LCD_DI_IS_1;  /* HIGH is data mode. */
  for( int i = 0; i < size; i++ )
  {
    delayMicroseconds( 5UL );
    *dat++ = byteRead();
    rot_rdq();
  }

  return size;
}

/*
  sg12864 cs1 data read.
*/
uint8_t SG12864_IF::cs1DataRead()
{
  LCD_CS1_IS_1;  /* HIGH is enable. */
  uint8_t dat = dataRead();
  LCD_CS1_IS_0;

  return dat;
}

int SG12864_IF::cs1DataRead( uint8_t *dat, int size )
{
  LCD_CS1_IS_1;  /* HIGH is enable. */
  int ret = dataRead( dat, size );
  LCD_CS1_IS_0;

  return ret;
}

/*
  sg12864 cs2 data read.
*/
uint8_t SG12864_IF::cs2DataRead()
{
  LCD_CS2_IS_1;  /* HIGH is enable. */
  uint8_t dat = dataRead();
  LCD_CS2_IS_0;

  return dat;
}

int SG12864_IF::cs2DataRead( uint8_t *dat, int size )
{
  LCD_CS2_IS_1;  /* HIGH is enable. */
  int ret = dataRead( dat, size );
  LCD_CS2_IS_0;

  return ret;
}
