/**
  * @brief Files specific for VT100 emulation.
  * https://www.mm2d.net/main/prog/c/console-01.html
  * https://www.mm2d.net/main/prog/c/console-02.html
  */
#ifndef  __vt100_h__
#define  __vt100_h__

#include  <Arduino.h>
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
public:
  String home() { return "\x1b[0;0H"; }  // cursor home.
  String cls() { return "\x1b[2J"; }  // clear screen.
  String delScrBack() { return "\x1b[0J"; }  // Erase behind the cursor.
  String delScrFront() { return "\x1b[1J"; }  // Erase before the cursor.
  String delLine() { return "\x1b[2K"; }  // Delete cursor line.
  String delLineBack() { return "\x1b[0K"; }  // Delete after the cursor on the cursor line.
  String delLineFront() { return "\x1b[1K"; }  // Delete before the cursor on the cursor line.

  String relCur( int x, int y )  // Move the cursor to a relative position.
  {
    // x position.
    if( x != 0 )
    {
      String esc = "\x1b[";
      if( x > 0 ) { esc += String(x,DEC); esc += "C"; }
      else { esc += String( abs(x), DEC ); esc += "D"; }
      return esc;
    }
    // y position.
    if( y != 0 )
    {
      String esc = "\x1b[";
      if( y > 0 ) { esc += String(y,DEC); esc += "B"; }
      else { esc += String( abs(y), DEC ); esc += "A"; }
      return esc;
    }
  }

  String relY_Top( int y )  // Moves the cursor to the beginning of the line at the specified relative position.
  {
    if( y != 0 )
    {
      String esc = "\x1b[";
      if( y > 0 ) { esc += String(y,DEC); esc += "E"; }
      else { esc += String( abs(y), DEC ); esc += "F"; }
      return esc;
    }
  }

  String relCurMove( int y )  // Scroll the console to the next y lines.
  {
    if( y != 0 )
    {
      String esc = "\x1b[";
      if( y > 0 ) { esc += String(y,DEC); esc += "S"; }
      else { esc += String( abs(y), DEC ); esc += "T"; }
      return esc;
    }
  }

  String absY( int y )  // Move the cursor to a location y from the left edge, regardless of the current horizontal position.
  {
    if( y < 0 ) return "";
    String esc = "\x1b[" + String(y,DEC) + "G";
    return esc;
  }

  String absCur( int x, int y )  // Move the cursor to the absolute position.
  {
    if( x < 0 || y < 0 ) return "";
    String esc = "\x1b[" + String(x,DEC) + ";" + String(y,DEC) + "H";
    return esc;
  }

  String attributes( int code )
  {
    String esc = "\x1b[" + String(code,DEC) + "m";
    return esc;
  }

  String extend( uint8_t code )
  {
    String esc = "\x1b[38;5;" + String(code,DEC) + "m";
    return esc;
  }

  String bgExtend( uint8_t code )
  {
    String esc = "\x1b[48;5;" + String(code,DEC) + "m";
    return esc;
  }

  String undo() { return attributes( 0 ); }  // Undo Character Attributes.
  String bold() { return attributes( 1 ); }  // Bold text attributes.
  String dark() { return attributes( 2 ); }  // Darken character attributes.
  String italic() { return attributes( 3 ); }  // Italicize the character attributes.
  String under() { return attributes( 4 ); }  // Underlining.
  String blink() { return attributes( 5 ); }  // blinking.
  String reversal() { return attributes( 7 ); }  // reversal.
  String hidden() { return attributes( 8 ); }  // hidden.
  String cancel() { return attributes( 9 ); }  // cancel.
  // The letters in the 90s are brighter than those in the 30s.
  String black() { return attributes( 30 + 60 ); }  // Make the foreground color black.
  String red() { return attributes( 31 + 60 ); }  // Make the foreground color red.
  String green() { return attributes( 32 + 60 ); }  // Make the foreground color green.
  String yellow() { return attributes( 33 + 60 ); }  // Make the foreground color yellow.
  String blue() { return attributes( 34 + 60 ); }  // Make the foreground color blue.
  String magenta() { return attributes( 35 + 60 ); }  // Make the foreground color magenta.
  String cyan() { return attributes( 36 + 60 ); }  // Make the foreground color cyan.
  String white() { return attributes( 37 + 60 ); }  // Make the foreground color white.
  String defaultColor() { return attributes( 39 ); }  // Restore foreground color to default.
  // The background color of the 100s is lighter than that of the 40s.
  String bgBlack() { return attributes( 40 ); }  // Make the background color black.
  String bgRed() { return attributes( 41 ); }  // Make the background color red.
  String bgGreen() { return attributes( 42 ); }  // Make the background color green.
  String bgYellow() { return attributes( 43 ); }  // Make the background color yellow.
  String bgBlue() { return attributes( 44 ); }  // Make the background color blue.
  String bgMagenta() { return attributes( 45 ); }  // Make the background color magenta.
  String bgCyan() { return attributes( 46 ); }  // Make the background color cyan.
  String bgWhite() { return attributes( 47 ); }  // Make the background color white.
  String bgDefaultColor() { return attributes( 49 ); }  // Restore background color to default.
};  /* VT100 */

#endif  /* __vt100_h__ */
