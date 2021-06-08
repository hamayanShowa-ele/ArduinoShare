/****************************************************************************/
/* Low cost graphic LCD SG12864 header                                      */
/*                         Copyright (C) 2014 hamayan All Rights Reserved.  */
/*                                                hamayan.contact@gmail.com */
/****************************************************************************/
#ifndef __SG12864_H__
#define __SG12864_H__

#include  <Arduino.h>
#include  "BOARD_DEPENDENCY.h"

extern "C"
{
}

#define  DISPLAY_ON          0x3f
#define  DISPLAY_OFF         0x3e
#define  DISPLAY_START_LINE  0xc0
#define  PAGE_SET            0xb8
#define  COLUMN_SET          0x40

#define  CLR_SCREEN()     fillAll(0x00)
#define  BORDER_SCREEN()  fillAll(0xCC)

class GRAPHIC
{
private:
  SG12864_IF *iF;
  int posX,posY;

  void cmdWrite( uint8_t cmd );
  int  pageWrite( int column, int page, const uint8_t *data, int size );
  int  pageRead( int column, int page, uint8_t *data, int size );

public:

  GRAPHIC( SG12864_IF *_iF ) { iF = _iF; }
  void begin();
  void fillAll( uint8_t pattern );
  void clear() { CLR_SCREEN(); }
  int  write( uint8_t data );
  int  write( int x, int y, uint8_t data );

  void pset( int x, int y, int dot = 1 );
  int  locate( int x, int y );

  int  rectangleWrite( const uint8_t *data, int fromX, int fromY, int toX, int toY );
  int  rectangleRead( uint8_t *data, int fromX, int fromY, int toX, int toY );

  void clearLine( int y );
  void clearLine( int y, int fromX, int toX );
};

class GRAPHIC_FONT // : public GRAPHIC
{
private:
  int posX,posY;

  int asciiWidth,asciiHeight,asciiBmpWidth;
  const uint8_t *asciiBmp;

  int kanjiWidth,kanjiHeight,kanjiBmpWidth;
  const uint8_t *kanjiBmp;

  GRAPHIC *gfx;

  void pickupFont( char c, uint8_t *buf );
  int  ascii( char c );


public:
  GRAPHIC_FONT( GRAPHIC *g, int w, int h, int bw, const uint8_t *b )
  {
    gfx = g;
    asciiWidth = w; asciiHeight = h; asciiBmpWidth = bw;
    asciiBmp = b;

    kanjiWidth = kanjiHeight = kanjiBmpWidth = 0;
    kanjiBmp = nullptr;

    posX = posY = 0;
  }
  void kanji( int w, int h, int bw, const uint8_t *b )
  {
    kanjiWidth = w; kanjiHeight = h; kanjiBmpWidth = bw;
    kanjiBmp = b;
  }

  int  putchar( char c );
  int  puts( char *str );
};


#endif  /* __SG12864_H__ */

/****************************************************************************/
/*                         Copyright (C) 2014 hamayan All Rights Reserved.  */
/*                                                hamayan.contact@gmail.com */
/****************************************************************************/
