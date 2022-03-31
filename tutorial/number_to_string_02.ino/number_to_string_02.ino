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

  // However, sprintf may not work if the buffer size is insufficient.
  char buf[64],asc[8];
  itoa( count, asc, 10 );
  sprintf( buf, "Example 1, Display a decimal number using the itoa. %s\r\n", asc );
  Serial.print( buf );

  itoa( count, asc, 8 );
  sprintf( buf, "Example 2, Display a octal number using the itoa. %s\r\n", asc );
  Serial.print( buf );

  itoa( count, asc, 16 );
  sprintf( buf, "Example 3, Display a hex-decimal number using the itoa. %s\r\n", asc );
  Serial.print( buf );

  String msg = "Example 4, Display a decimal number using the String type. ";
  msg += String( count, DEC );
  Serial.println( msg );

  msg = "Example 5, Display a octal number using the String type. ";
  msg += String( count, OCT );
  Serial.println( msg );

  msg = "Example 6, Display a hex-decimal number using the String type. ";
  msg += String( count, HEX );
  Serial.println( msg );

  Serial.println();
  count++;
}
