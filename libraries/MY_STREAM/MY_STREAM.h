/**
  ******************************************************************************
  * @file           : MY_STREAM.h
  * @brief          : original stream class
  * Copyright (c) 2022 Kazuaki Hamahara
  *  Released under the MIT license
  *  https://github.com/YukinobuKurata/YouTubeMagicBuyButton/blob/master/MIT-LICENSE.txt
  *
  ******************************************************************************
  */
#ifndef  __MY_STREAM_H__
#define  __MY_STREAM_H__

#include  <Arduino.h>

#ifndef wait_ms
  #define  wait_ms(dly)  delay(dly)
#endif  /* wait_ms */

class MY_STREAM
{
private:
//  uint8_t *dataArray;
  uint8_t width;
  void *dataArray;
  uint32_t dataArraySize;
  uint32_t dataWptr,dataRptr;

public:
  MY_STREAM() { dataArray = nullptr; }
  MY_STREAM( uint32_t s, uint8_t w = 1 ) { config( s, w ); }
  ~MY_STREAM() { end(); }

  void end()
  {
    if(dataArray != nullptr)
    {
      if( width == 1 ) { uint8_t *a = (uint8_t *)dataArray; delete [] a; }
      else if( width == 2 ) { uint16_t *a = (uint16_t *)dataArray; delete [] a; }
    }
  }

  void config( uint32_t s, uint8_t w = 1 )
  {
    width = w;
    dataWptr = dataRptr = 0UL;
    dataArraySize = s;
    if( width == 1 )
    {
      uint8_t *ptr = new uint8_t[ dataArraySize ];
      dataArray = ptr;
    }
    else if( width == 2 )
    {
      uint16_t *ptr = new uint16_t[ dataArraySize ];
      dataArray = ptr;
    }
  }

  int write( uint8_t dat, uint32_t dly = 10UL );
  int write( const uint8_t *dat, int size, uint32_t dly = 10UL );
  int write( uint16_t dat, uint32_t dly );
  int write( const uint16_t *dat, int size, uint32_t dly );
  int read();
  uint32_t available()
  {
    return (dataWptr == dataRptr) ? 0UL :
           (dataWptr > dataRptr) ? dataWptr - dataRptr :
           dataArraySize + dataWptr - dataRptr;
  }
  uint32_t remain()
  {
    return dataArraySize - available();
  }

  void purge() { dataRptr = dataWptr; }
};

#endif  /* __MY_STREAM_H__ */
