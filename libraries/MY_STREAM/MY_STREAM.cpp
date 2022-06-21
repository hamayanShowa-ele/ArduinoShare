/**
  ******************************************************************************
  * @file           : MY_STREAM.cpp
  * @brief          : original stream class
  * Copyright (c) 2022 Kazuaki Hamahara
  *  Released under the MIT license
  *  https://github.com/YukinobuKurata/YouTubeMagicBuyButton/blob/master/MIT-LICENSE.txt
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "MY_STREAM.h"

int MY_STREAM::write( uint8_t dat, uint32_t dly )
{
  uint32_t next = dataWptr + 1;
  if( next >= dataArraySize ) next = 0;
  uint32_t baseTim = millis();
  if( dly != 0UL )
  {
    while( next == dataRptr )
    {
      if( (millis() - baseTim) >= dly ) return (-1);
      wait_ms( 1UL );
    }
  }
  *( (uint8_t *)dataArray + next) = dat;
  dataWptr = next;
  return 1;
}

int MY_STREAM::write( uint16_t dat, uint32_t dly )
{
  uint32_t next = dataWptr + 1;
  if( next >= dataArraySize ) next = 0;
  uint32_t baseTim = millis();
  if( dly != 0UL )
  {
    while( next == dataRptr )
    {
      if( (millis() - baseTim) >= dly ) return (-1);
      wait_ms( 1UL );
    }
  }
  *( (uint16_t *)dataArray + next) = dat;
  dataWptr = next;
  return 1;
}

int MY_STREAM::write( const uint8_t *dat, int size, uint32_t dly )
{
  for( int i = 0; i < size; i++ )
  {
    int ret = write( *dat++, dly );
    if( ret <= 0 ) return ret;
  }
  return size;
}

int MY_STREAM::write( const uint16_t *dat, int size, uint32_t dly )
{
  for( int i = 0; i < size; i++ )
  {
    int ret = write( *dat++, dly );
    if( ret <= 0 ) return ret;
  }
  return size;
}

int MY_STREAM::read()
{
  if( dataWptr == dataRptr ) return (-1);
  uint32_t next = dataRptr + 1;
  if( next >= dataArraySize ) next = 0;
  int dat;
  if( width == 1 ) { dat = *((uint8_t *)dataArray + next) & 0x000000FF; }
  else if( width == 2 ) { dat = *((uint16_t *)dataArray + next) & 0x0000FFFF; }
  else { return (-1); }
  dataRptr = next;
  return dat;
}
