/**
  ******************************************************************************
  * @file           : sg12864.cpp
  * @brief          : Low cost graphic LCD SG12864 header
    Copyright (c) 2022 Kazuaki Hamahara
    Released under the MIT license
    https://github.com/YukinobuKurata/YouTubeMagicBuyButton/blob/master/MIT-LICENSE.txt
  ******************************************************************************
  */﻿

#include  "sg12864.h"

extern "C"{
}

extern HardwareSerial Serial1;


/*
  constructor,destructor
*/


void GRAPHIC::cmdWrite( uint8_t cmd )
{
  iF->cs1cmdWrite( cmd );
  iF->cs2cmdWrite( cmd );
}


/*
  sg12864 begin.
*/
void GRAPHIC::begin()
{
  cmdWrite( DISPLAY_START_LINE | 0 );
  cmdWrite( DISPLAY_ON );
  CLR_SCREEN();
//  BORDER_SCREEN();

  volatile uint8_t sts;
  sts = iF->cs1stsRead();
  sts = iF->cs2stsRead();

  posX = posY = 0;
}


/*
  sg12864 fill all by pattern.
*/
void GRAPHIC::fillAll( uint8_t pattern )
{
  uint8_t area[64];
  memset( area, pattern, 64 );

  while( iF->cs1stsRead() & 0x80 ) rot_rdq();  /* wait for busy cleard. */
  while( iF->cs2stsRead() & 0x80 ) rot_rdq();  /* wait for busy cleard. */
  for( int page = 0; page < 8; page++ )
  {
    iF->cs1cmdWrite( PAGE_SET | (uint8_t)page );
    iF->cs1cmdWrite( COLUMN_SET | 0 );
    iF->cs1DataWrite( area, sizeof(area) );

    iF->cs2cmdWrite( PAGE_SET | (uint8_t)page );
    iF->cs2cmdWrite( COLUMN_SET | 0 );
    iF->cs2DataWrite( area, sizeof(area) );
  }
}


/*
  sg12864 pset.

  Draw a point on 128 horizontal and 64 vertical pixels.
  x can be reflected in the column data as it is. y is converted to page
   by dividing by 8.
  The bit position can be determined by the surplus.
  First, get the data for the corresponding column and page, and then
   change the corresponding bit and draw it back.

  NOTE! The column address is automatically incremented.
*/
void GRAPHIC::pset( int x, int y, int dot )
{
  volatile uint8_t data;

  int column = x;
  int page = y / 8;
  int bit = y % 8;

  data = iF->cs1DataRead();  /*dummy read*/
  data = iF->cs2DataRead();  /*dummy read*/
  if(column < 64)  /*cs1 area*/
  {
    while( iF->cs1stsRead() & 0x80 ) rot_rdq();  /* wait for busy cleard. */
    iF->cs1cmdWrite( PAGE_SET | (uint8_t)page );
    iF->cs1cmdWrite( COLUMN_SET | (uint8_t)(column - 0) );
    data = iF->cs1DataRead();
  }
  else  /*cs2 area*/
  {
    while( iF->cs2stsRead() & 0x80) rot_rdq();  /* wait for busy cleard. */
    iF->cs2cmdWrite( PAGE_SET | (uint8_t)page );
    iF->cs2cmdWrite( COLUMN_SET | (uint8_t)(column - 64) );
    data = iF->cs2DataRead();
  }

  if(dot == 0) data &= ~(0x01 << bit);  /* delete */
  else if(dot == (-1)) data ^= (0x01 << bit);  /* invert */
  else data |= (0x01 << bit);  /* draw */

  if(column < 64)  /*cs1 area*/
  {
//    while( iF->cs1stsRead() & 0x80 ) rot_rdq();  /* wait for busy cleard. */
    iF->cs1cmdWrite( PAGE_SET | (uint8_t)page );
    iF->cs1cmdWrite( COLUMN_SET | (uint8_t)(column - 0) );
    iF->cs1DataWrite( data );
  }
  else  /*cs2 area*/
  {
//    while( iF->cs2stsRead() & 0x80) rot_rdq();  /* wait for busy cleard. */
    iF->cs2cmdWrite( PAGE_SET | (uint8_t)page );
    iF->cs2cmdWrite( COLUMN_SET | (uint8_t)(column - 64) );
    iF->cs2DataWrite( data );
  }
}


/*
  sg12864 locate.

  set x and y positions.
*/
int GRAPHIC::locate( int x, int y )
{
  if( x >= 128 ) return (-1);
  if( y >= 64 ) return (-1);
  posX = x;
  if( y >= 56 ) posY = 56;
}



/*
  sg12864 write.
*/
int GRAPHIC::write( uint8_t data )
{
  int column = posX;
  int page = posY / 8;

  if( column < 64 )  /*cs1 area*/
  {
    while( iF->cs1stsRead() & 0x80 ) rot_rdq();  /* wait for busy cleard. */
    iF->cs1cmdWrite( PAGE_SET | (uint8_t)page );
    iF->cs1cmdWrite( COLUMN_SET | (uint8_t)(column - 0) );
    iF->cs1DataWrite( data );
  }
  else  /*cs2 area*/
  {
    while( iF->cs2stsRead() & 0x80) rot_rdq();  /* wait for busy cleard. */
    iF->cs2cmdWrite( PAGE_SET | (uint8_t)page );
    iF->cs2cmdWrite( COLUMN_SET | (uint8_t)(column - 64) );
    iF->cs2DataWrite( data );
  }

  if( ++posX >= 128 )
  {
    posX = 0;
    posY += 8;
    if( posY >= 56 )
    {
      posY = 56;
    }
  }

  return 1;
}


int GRAPHIC::write( int x, int y, uint8_t data )
{
  int column = x;
  int page = y / 8;

  if( column < 64 )  /*cs1 area*/
  {
    while( iF->cs1stsRead() & 0x80 ) rot_rdq();  /* wait for busy cleard. */
    iF->cs1cmdWrite( PAGE_SET | (uint8_t)page );
    iF->cs1cmdWrite( COLUMN_SET | (uint8_t)(column - 0) );
    iF->cs1DataWrite( data );
  }
  else  /*cs2 area*/
  {
    while( iF->cs2stsRead() & 0x80) rot_rdq();  /* wait for busy cleard. */
    iF->cs2cmdWrite( PAGE_SET | (uint8_t)page );
    iF->cs2cmdWrite( COLUMN_SET | (uint8_t)(column - 64) );
    iF->cs2DataWrite( data );
  }

  return 1;
}


int GRAPHIC::pageWrite( int column, int page, const uint8_t *data, int size )
{
  if( (column + size) > 128 ) size = 128 - column;

  if( column < 64 )
    while( iF->cs1stsRead() & 0x80 ) rot_rdq();  /* wait for busy cleard. */

  if( (column + size) >= 64 )
    while( iF->cs2stsRead() & 0x80 ) rot_rdq();  /* wait for busy cleard. */

  if( column < 64 )
  {
    int limit = ( (column + size) >= 64 ) ? 64 : column + size;

    iF->cs1cmdWrite( PAGE_SET | (uint8_t)page );
    iF->cs1cmdWrite( COLUMN_SET | (uint8_t)column );
    iF->cs1DataWrite( data, limit - column );
    data += limit - column;
  }

  if( (column + size) >= 64 )
  {
    int limit = column + size;
    if( column < 64 ) column = 0;
    else column -= 64;

    iF->cs2cmdWrite( PAGE_SET | (uint8_t)page );
    iF->cs2cmdWrite( COLUMN_SET | (uint8_t)column );
    iF->cs2DataWrite( data, limit - column - 64 );
  }

  return size;
}


int GRAPHIC::rectangleWrite( const uint8_t *data, int fromX, int fromY, int toX, int toY )
{
  if( toX < fromX ) { int temp = toX; toX = fromX; fromX = temp; }
  if( toY < fromY ) { int temp = toY; toY = fromY; fromY = temp; }

  for( int y = fromY; y < toY; y += 8 )
  {
    int ret = pageWrite( fromX, y / 8, data, toX - fromX );
    if( ret <= 0 ) return ret;
    data += ret;
  }
  
  return (toX - fromX) * (toY - fromY);
}


void GRAPHIC::clearLine( int y )
{
  uint8_t buf[ 128 * 1 ];
  memset( buf, 0, sizeof(buf) );
  rectangleWrite( (const uint8_t *)buf, 0, y, 128, y + 8 );
}

void GRAPHIC::clearLine( int y, int fromX, int toX )
{
  uint8_t buf[ 128 * 1 ];
  memset( buf, 0, sizeof(buf) );
  rectangleWrite( (const uint8_t *)buf, fromX, y, toX, y + 8 );
}


/*
  sg12864 page data read.
*/
int GRAPHIC::pageRead( int column, int page, uint8_t *data, int size )
{
  if( (column + size) > 128 ) size = 128 - column;

  if( column < 64 )
    while( iF->cs1stsRead() & 0x80 ) rot_rdq();  /* wait for busy cleard. */

  if( (column + size) >= 64 )
    while( iF->cs2stsRead() & 0x80 ) rot_rdq();  /* wait for busy cleard. */

  if( column < 64 )
  {
    int limit = ( (column + size) >= 64 ) ? 64 : column + size;

    iF->cs1cmdWrite( PAGE_SET | (uint8_t)page );
    iF->cs1cmdWrite( COLUMN_SET | (uint8_t)column );
    volatile uint8_t dummy = iF->cs1DataRead();
    iF->cs1DataRead( data, limit - column );
    data += limit - column;
  }

  if( (column + size) >= 64 )
  {
    int limit = column + size;
    if( column < 64 ) column = 0;
    else column -= 64;

    iF->cs2cmdWrite( PAGE_SET | (uint8_t)page );
    iF->cs2cmdWrite( COLUMN_SET | (uint8_t)column );
    volatile uint8_t dummy = iF->cs2DataRead();
    iF->cs2DataRead( data, limit - column - 64 );
  }

  return size;
}


int GRAPHIC::rectangleRead( uint8_t *data, int fromX, int fromY, int toX, int toY )
{
  if( toX < fromX ) { int temp = toX; toX = fromX; fromX = temp; }
  if( toY < fromY ) { int temp = toY; toY = fromY; fromY = temp; }


  for( int y = fromY; y < toY; y += 8 )
  {
    int ret = pageRead( fromX, y / 8, data, toX - fromX );
    if( ret <= 0 ) return ret;
    data += ret;
  }

  return (toX - fromX) * (toY - fromY);
}


/*
  pickupFont.

  pick up from font data to buffer.
*/
void GRAPHIC_FONT::pickupFont( char c, uint8_t *buf )
{
  for( int i = 0; i < (16 - asciiHeight); i++ ) *buf++ = 0;  /* padding. */
  int code = c - ' ';
  for( int i = 0; i < asciiHeight; i++ )
  {
    *buf++ = asciiBmp[ code + ((asciiBmpWidth / asciiWidth) * i) ];
  }
}


/*
  ascii character.

  Draws 8x14 pixel font data.

  Since the original font data flows from the upper left origin to
   the upper right, one down and left to right, it needs to be orthogonally
   converted.
*/
int GRAPHIC_FONT::ascii( char c )
{
  int cnt;
  uint8_t fnt[16],buf[asciiWidth];

  pickupFont( c, fnt );

  int x = posX; int y = posY;
  /* Each character consists of 16 bytes. Half of it is processed first. */
  for( int row = 0, cnt = 0; cnt < asciiWidth; cnt++, row++ )
  {
    uint8_t c0 = fnt[ cnt ];  /* Extracting a line of data. */
    /* It is evaluated bit by bit. This means that the data is placed from
        the MSB of the original data to the LSB of the buffer.*/
    for( int col = 7; col >= 0; col-- )
    {
      if( c0 & (0x01 << col) ) buf[7 - col] |= 0x01 << row;
      else buf[7 - col] &= ~(0x01 << row);
    }
  }
  uint8_t *ptr = buf;
  for( cnt = 0; cnt < asciiWidth; cnt++ )
  {
    gfx->write( x++, y, *ptr++ );
  }
  x = posX; y += 8;

  /* Each character consists of 16 bytes. Half of it is processed first. */
  for( int row = 0, cnt = asciiWidth; cnt < (asciiWidth * 2); cnt++, row++ )
  {
    uint8_t c0 = fnt[ cnt ];  /* Extracting a line of data. */
    /* It is evaluated bit by bit. This means that the data is placed from
        the MSB of the original data to the LSB of the buffer.*/
    for( int col = 7; col >= 0; col-- )
    {
      if( c0 & (0x01 << col) ) buf[7 - col] |= 0x01 << row;
      else buf[7 - col] &= ~(0x01 << row);
    }
  }
  ptr = buf;
  for( cnt = 0,ptr = buf; cnt < asciiWidth; cnt++ )
  {
    gfx->write( x++, y, *ptr++ );
  }

  return c;
}


/*
  putchar
  put character.
*/
int GRAPHIC_FONT::putchar( char c )
{
  int ret = 0;
  if( !(c & 0x80) )  /* ascii code */
  {
    ret = ascii( c );
    posX += asciiWidth;
    if( posX >= 128 )
    {
      posX = 0;
      posY += 16;
      if( posY >= 64 )
      {
        posY = 0;
      }
    }
  }
  else  /* multi byte code? */
  {
  }
  return ret;
}

/*
  puts
  put strings.
*/
int GRAPHIC_FONT::puts( char *str )
{
  while( *str ) putchar( *str++ );
  return strlen( str );
}

