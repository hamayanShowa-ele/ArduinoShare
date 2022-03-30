/**
  * @brief Files specific for VT100 emulation.
  * https://www.mm2d.net/main/prog/c/console-01.html
  * https://www.mm2d.net/main/prog/c/console-02.html
  */
#ifndef  __vt100_usb_serial_h__
#define  __vt100_usb_serial_h__

#include  <vt100.h>
#include  <USBSerial.h>
extern "C"
{
}

class VT100_USB_SERIAL : public VT100
{
private:
  USBSerial *serial;

  void write( String str )
  {
    if( serial == nullptr ) return;
    serial->write( str.c_str(), str.length() );
  }

  void write( const char *str )
  {
    String s = str;
    write( s );
  }

public:
  VT100_USB_SERIAL() { serial = nullptr; }
  VT100_USB_SERIAL( USBSerial *s ) { config( s ); }

  void config( USBSerial *s ) { serial = s; }
  
  void beep() { serial->write( ASCII_BEEP ); }  // beep
  void bs() { serial->write( ASCII_BS ); }  // back space
  void ff() { serial->write( ASCII_FF ); }  // front feed
  void lf() { serial->write( ASCII_LF ); }  // line feed
  void cr() { serial->write( ASCII_CR ); }  // carriage return
  void htab() { serial->write( ASCII_HTAB ); }  // horizontal tabulation
  void vtab() { serial->write( ASCII_VTAB ); }  // vertical tabulation
  void del() { serial->write( ASCII_SPACE ); }  // delete

  void home() { write( VT100::home() ); }  // cursor home.
  void cls() { write( VT100::cls() ); }  // clear screen.
  void delScrBack() { write( VT100::delScrBack() ); }  // Erase behind the cursor.
  void delScrFront() { write( VT100::delScrFront() ); }  // Erase before the cursor.
  void delLine() { write( VT100::delLine() ); }  // Delete cursor line.
  void delLineBack() { write( VT100::delLineBack() ); }  // Delete after the cursor on the cursor line.
  void delLineFront() { write( VT100::delLineFront() ); }  // Delete before the cursor on the cursor line.

  void relCur( int x, int y )  // Move the cursor to a relative position.
  {
    write( VT100::relCur( x, y ) );
  }

  void relY_Top( int y )  // Moves the cursor to the beginning of the line at the specified relative position.
  {
    write( VT100::relY_Top( y ) );
  }

  void relCurMove( int y )  // Scroll the console to the next y lines.
  {
    write( VT100::relCurMove( y ) );
  }

  void absY( int y )  // Move the cursor to a location y from the left edge, regardless of the current horizontal position.
  {
    write( VT100::absY( y ) );
  }

  void absCur( int x, int y )  // Move the cursor to the absolute position.
  {
    write( VT100::absCur( x, y ) );
  }

  void attributes( int code )
  {
    write( VT100::attributes( code ) );
  }

  void extend( uint8_t code )
  {
    write( VT100::extend( code ) );
  }

  void bgExtend( uint8_t code )
  {
    write( VT100::bgExtend( code ) );
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
};  /* VT100_USB_SERIAL */

#endif  /* __vt100_usb_serial_h__ */
