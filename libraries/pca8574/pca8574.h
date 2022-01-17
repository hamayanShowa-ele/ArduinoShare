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

#ifndef  __PCA8574_H__
#define  __PCA8574_H__

#include  <Wire.h>
extern "C"
{
}

#define  PCA8574_bit0  0x01
#define  PCA8574_bit1  0x02
#define  PCA8574_bit2  0x04
#define  PCA8574_bit3  0x08
#define  PCA8574_bit4  0x10
#define  PCA8574_bit5  0x20
#define  PCA8574_bit6  0x40
#define  PCA8574_bit7  0x80

/* ----------------------------------------
    prototypes 
---------------------------------------- */

/* ----------------------------------------
    instances or global variables
---------------------------------------- */

class PCA8574 //: public hoge
{
private:
  TwoWire *i2c;
  uint8_t slave;
  uint8_t byteData;

  int bitSetReset( uint8_t data );

public:
  PCA8574( TwoWire *_i2c, uint8_t adr )
  {
    i2c = _i2c;
    slave = adr;
    byteData = 0;
  }
//  ~PCA8574() {}

  int write( uint8_t data );
  int read();
  int set( int b );
  int reset( int b );
  int toggle( int b );
};


#endif  /* __PCA8574_H__ */

