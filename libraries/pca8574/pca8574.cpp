/* ----------------------------------------
  pca8574 utilities
  for STMicroelectronics SPL library

  Copyright (c) 2020 hamayan (hamayan@showa-ele.jp).
  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Created 2020 by hamayan (hamayan@showa-ele.jp)
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
