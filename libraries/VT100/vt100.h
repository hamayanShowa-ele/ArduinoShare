/**
  * @brief Files specific for VT100 emulation.
  * https://www.mm2d.net/main/prog/c/console-01.html
  * https://www.mm2d.net/main/prog/c/console-02.html
  */
#ifndef  __vt100_h__
#define  __vt100_h__

#include  <Arduino.h>
#include  <HardwareSerial.h>

extern "C"
{
}

#define  ASCII_ESC   0x1B
#define  ASCII_BEEP  0x07
#define  ASCII_BS    0x08
#define  ASCII_FF    0x0C
#define  ASCII_LF    0x0A
#define  ASCII_CR    0x0D
#define  ASCII_HTAB  0x09
#define  ASCII_VTAB  0x0B
#define  ASCII_DEL   0x20
#define  ASCII_SPACE 0x20

class VT100
{
private:
  HardwareSerial *serial;
  void write( const char *str )
  {
    while( *str ) { serial->write( *str++ ); }
  }

public:
  VT100() { serial = &Serial; }
  VT100( HardwareSerial *s ) { serial = s; }

  void beep() { serial->write( ASCII_BEEP ); }  // beep
  void bs() { serial->write( ASCII_BS ); }  // back space
  void ff() { serial->write( ASCII_FF ); }  // front feed
  void lf() { serial->write( ASCII_LF ); }  // line feed
  void cr() { serial->write( ASCII_CR ); }  // carriage return
  void htab() { serial->write( ASCII_HTAB ); }  // horizontal tabulation
  void vtab() { serial->write( ASCII_VTAB ); }  // vertical tabulation
  void del() { serial->write( ASCII_SPACE ); }  // delete

  void home() { write( "\x1b[0;0H" ); }  // cursor home.
  void cls() { write( "\x1b[2J" ); }  // clear screen.
  void delScrBack() { write( "\x1b[0J" ); }  // Erase behind the cursor.
  void delScrFront() { write( "\x1b[1J" ); }  // Erase before the cursor.
  void delLine() { write( "\x1b[2K" ); }  // Delete cursor line.
  void delLineBack() { write( "\x1b[0K" ); }  // Delete after the cursor on the cursor line.
  void delLineFront() { write( "\x1b[1K" ); }  // Delete before the cursor on the cursor line.

  void relCur( int x, int y )  // Move the cursor to a relative position.
  {
    // x position.
    if( x != 0 )
    {
      String esc = "\x1b[";
      if( x > 0 ) { esc += (String)x; esc += "C"; }
      else { esc += (String)abs(x); esc += "D"; }
      write( (const char *)esc.c_str() );
    }
    // y position.
    if( y != 0 )
    {
      String esc = "\x1b[";
      if( y > 0 ) { esc += (String)y; esc += "B"; }
      else { esc += (String)abs(y); esc += "A"; }
      write( (const char *)esc.c_str() );
    }
  }

  void relY_Top( int y )  // Moves the cursor to the beginning of the line at the specified relative position.
  {
    if( y != 0 )
    {
      String esc = "\x1b[";
      if( y > 0 ) { esc += (String)y; esc += "E"; }
      else { esc += (String)abs(y); esc += "F"; }
      write( (const char *)esc.c_str() );
    }
  }

  void relCurMove( int y )  // Scroll the console to the next y lines.
  {
    if( y != 0 )
    {
      String esc = "\x1b[";
      if( y > 0 ) { esc += (String)y; esc += "S"; }
      else { esc += (String)abs(y); esc += "T"; }
      write( (const char *)esc.c_str() );
    }
  }

  void absY( int y )  // Move the cursor to a location y from the left edge, regardless of the current horizontal position.
  {
    if( y < 0 ) return;
    String esc = "\x1b[" + (String)y + "G";
    write( (const char *)esc.c_str() );
  }

  void absCur( int x, int y )  // Move the cursor to the absolute position.
  {
    if( x < 0 || y < 0 ) return;
    String esc = "\x1b[" + (String)x + ";" + (String)y + "H";
    write( (const char *)esc.c_str() );
  }

  void attributes( int code )
  {
    String esc = "\x1b[" + (String)code + "m";
    write( (const char *)esc.c_str() );
  }

  void undo() { attributes( 0 ); }  // Undo Character Attributes.
  void bold() { attributes( 1 ); }  // Bold text attributes.
  void dark() { attributes( 2 ); }  // Darken character attributes.
  void italic() { attributes( 3 ); }  // Italicize the character attributes.
  void under() { attributes( 4 ); }  // Underlining.
  void blink() { attributes( 5 ); }  // blinking.
  void reversal() { attributes( 7 ); }  // reversal.
  void hidden() { attributes( 8 ); }  // hidden.
  void cancel() { attributes( 9 ); }  // cancel.
  // The letters in the 90s are brighter than those in the 30s.
  void black() { attributes( 30 + 60 ); }  // Make the foreground color black.
  void red() { attributes( 31 + 60 ); }  // Make the foreground color red.
  void green() { attributes( 32 + 60 ); }  // Make the foreground color green.
  void yellow() { attributes( 33 + 60 ); }  // Make the foreground color yellow.
  void blue() { attributes( 34 + 60 ); }  // Make the foreground color blue.
  void magenta() { attributes( 35 + 60 ); }  // Make the foreground color magenta.
  void cyan() { attributes( 36 + 60 ); }  // Make the foreground color cyan.
  void white() { attributes( 37 + 60 ); }  // Make the foreground color white.
  void defaultColor() { attributes( 39 ); }  // Restore foreground color to default.
  // The background color of the 100s is lighter than that of the 40s.
  void bgBlack() { attributes( 40 ); }  // Make the background color black.
  void bgRed() { attributes( 41 ); }  // Make the background color red.
  void bgGreen() { attributes( 42 ); }  // Make the background color green.
  void bgYellow() { attributes( 43 ); }  // Make the background color yellow.
  void bgBlue() { attributes( 44 ); }  // Make the background color blue.
  void bgMagenta() { attributes( 45 ); }  // Make the background color magenta.
  void bgCyan() { attributes( 46 ); }  // Make the background color cyan.
  void bgWhite() { attributes( 47 ); }  // Make the background color white.
  void bgDefaultColor() { attributes( 49 ); }  // Restore background color to default.
};  /* VT100 */

#endif  /* __vt100_h__ */
