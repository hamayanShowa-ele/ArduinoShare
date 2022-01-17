/**
  ******************************************************************************
  * @file           : rom24aa025.cpp
  * @brief          : SERIAL EEPROM 24AA025 librarly.
    Copyright (c) 2022 Kazuaki Hamahara
    Released under the MIT license
    https://github.com/YukinobuKurata/YouTubeMagicBuyButton/blob/master/MIT-LICENSE.txt
  ******************************************************************************
  */

#include  <rom24aa025.h>

extern "C" {
}

/*
  read from eeprom
*/
void EEPROM_24AA025::read( uint8_t adr, uint8_t *dest )
{
  read( adr, dest, 1 );
}


void EEPROM_24AA025::read( uint8_t adr, uint8_t *dest, int len )
{
  /* Read protection, don't read beyond memory limits */
  wire->beginTransmission( i2cAddress );
  wire->write( (uint8_t)(adr & 0xFF) ); // LSB
  wire->endTransmission();

  wire->requestFrom( (int)i2cAddress, len );
  for( int c = 0; c < len; c++ )
  {
    if( wire->available() )
    {
      *dest++ = wire->read();
    }
  }
}
