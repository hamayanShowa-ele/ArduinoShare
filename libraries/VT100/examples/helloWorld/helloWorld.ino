/*
  VT100 example.
*/

#include  <vt100.h>

/**
  * @brief instance and global variables.
  */

VT100 vt100( &Serial );  // When using Serial, no arguments need to be set.

/**
  * @brief setup.
  * @retval none.
  */
void setup()
{
  Serial.begin( 115200UL );
  while( !Serial ) { delay( 20UL ); }
  for( int i = 0; i < 3; i++ ) { vt100.beep(); delay( 500 ); }
  vt100.cls();
  vt100.home();
  vt100.bold();
  vt100.italic();
  vt100.absCur(20,20);

  vt100.red(); Serial.write( 'H' );
  vt100.green(); Serial.write( 'e' );
  vt100.yellow(); Serial.write( 'l' );
  vt100.blue(); Serial.write( 'l' );
  vt100.magenta(); Serial.write( 'o' );
  vt100.black(); Serial.write( ' ' );
  vt100.cyan(); Serial.write( 'w' );
  vt100.white(); Serial.write( 'o' );
  vt100.bgRed(); Serial.write( 'r' );
  vt100.bgGreen(); Serial.write( 'l' );
  vt100.bgYellow(); Serial.write( 'd' );
  vt100.bgCyan(); Serial.write( '.' );
  vt100.cr();
  vt100.lf();
  vt100.undo();
}

/**
  * @brief loop.
  * @retval none.
  */
void loop()
{
}



extern "C"
{
}  /* extern "C" */
