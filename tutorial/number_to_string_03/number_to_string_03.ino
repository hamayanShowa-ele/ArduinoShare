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
  static uint32_t count = 0UL;

  delay( 200UL );

  // In itoa and String types, alphabetic characters are converted to lowercase.
  // Also, the number of digits in the string to be converted depends on the number of digits in the current number.
  char buf[64 + 32],asc[8];
  bin2dec( (uint8_t)count, asc );
  sprintf( buf, "Example 1, Display a uint8_t decimal number using the Original conversion functions. %s\r\n", asc );
  Serial.print( buf );

  bin2dec( (uint16_t)count, asc );
  sprintf( buf, "Example 2, Display a uint16_t decimal number using the Original conversion functions. %s\r\n", asc );
  Serial.print( buf );

  Serial.println();
  count++;
}

/**
  * @brief Convert a number of type uint8_t to a string.
  * It can be from 0 to 255, so it should be a 3-digit number.
  */
char *bin2dec( uint8_t num, char *buf )
{
  int value = num;
  buf[0] = ' '; buf[1] = ' '; buf[2] = ' '; buf[3] = '\0';

  if( value >= 100 ) { buf[0] = '0' + (value / 100); }
  value %= 100;

  if( value >= 10 ) { buf[1] = '0' + (value / 10); }
  else if( buf[0] != ' ' ) { buf[1] = '0'; }
  value %= 10;

  buf[2] = '0' + value;

  return buf;
}

/**
  * @brief Convert a number of type uint16_t to a string.
  * It can be from 0 to 65535, so it should be a 5-digit number.
  */
char *bin2dec( uint16_t num, char *buf )
{
  uint32_t value = num;
  buf[0] = ' '; buf[1] = ' '; buf[2] = ' '; buf[3] = ' '; buf[4] = ' '; buf[5] = '\0';

  if( value >= 10000 ) { buf[0] = '0' + (value / 10000); }
  value %= 10000;
 
  if( value >= 1000 ) { buf[1] = '0' + (value / 1000); }
  else if( buf[0] != ' ' ) { buf[1] = '0'; }
  value %= 1000;

  if( value >= 100 ) { buf[2] = '0' + (value / 100); }
  else if( buf[1] != ' ' ) { buf[2] = '0'; }
  value %= 100;

  if( value >= 10 ) { buf[3] = '0' + (value / 10); }
  else if( buf[2] != ' ' ) { buf[3] = '0'; }
  value %= 10;

  buf[4] = '0' + value;

  return buf;
}
