/* ----------------------------------------
  pca8574 utilities
  for STMicroelectronics SPL library

  Copyright (c) 2022 Kazuaki Hamahara
  Released under the MIT license
  https://github.com/YukinobuKurata/YouTubeMagicBuyButton/blob/master/MIT-LICENSE.txt
---------------------------------------- */

#include  "pca8574.h"

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

/* ----------------------------------------
    constructor destructor
---------------------------------------- */

/* ----------------------------------------
    write
---------------------------------------- */
int PCA8574::write( uint8_t data )
{
  byteData = data;
  i2c->beginTransmission( slave );
  int ret = i2c->write( byteData );
  i2c->endTransmission();
  return ret;
}


/* ----------------------------------------
    read
---------------------------------------- */
int PCA8574::read()
{
  int ret = i2c->requestFrom( slave, 1 );
  if( ret != 1 )
  {
    i2c->endTransmission();
    return ret;
  }
  ret = i2c->read();
  i2c->endTransmission();
  return ret;
}

/* ----------------------------------------
    bit set, bit reset.
---------------------------------------- */
int PCA8574::bitSetReset( uint8_t data )
{
  i2c->beginTransmission( slave );
  int ret = i2c->write( data );
  i2c->endTransmission();
  return ret;
}

int PCA8574::set( int b )
{
  uint8_t tempUC = 0x01;
  tempUC <<= b;
  byteData |= tempUC;
  return bitSetReset( byteData );
}

int PCA8574::reset( int b )
{
  uint8_t tempUC = 0x01;
  tempUC <<= b;
  byteData &= ~tempUC;
  return bitSetReset( byteData );
}

int PCA8574::toggle( int b )
{
  uint8_t tempUC = 0x01;
  tempUC <<= b;
  byteData ^= tempUC;
  return bitSetReset( byteData );
}
