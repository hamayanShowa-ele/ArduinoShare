/*
  Arduino IDE setup.
  Board type : Arduino UNO.
  U(S)ART support : yes generic Serial.
  USB support : none.
  USB speed : LOw/Full Speed.
  Optimize : ANY.
  C Runtime Library : ANY.
  Update method : USB/Serial.
  Serial port : ANY.
*/
/**
  * @brief Example of converting a number to a string.
  */

void setup()
{
  Serial.begin( 115200UL );
  while( !Serial ) { delay( 20UL ); }
  Serial.println( "hello this is example of converting a number to a string." );
}

void loop()
{
  static int count = 0;

  delay( 1000UL );

  String msg = "Example 1, Display a decimal number using the String type. ";
  msg += String( count, DEC );
  Serial.println( msg );

//  Serial.printf( "Example 2, Display a decimal number using the printf. %d\r\n", count );
  // However, printf cannot be used with Arduino UNO or other AVRs.

  char buf[64];
  sprintf( buf, "Example 3, Display a decimal number using the sprintf. %d\r\n", count );
  Serial.print( buf );
  // However, sprintf may not work if the buffer size is insufficient.

  Serial.println();
  count++;
}
