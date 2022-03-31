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
  static uint32_t count = 0x00000000;

  delay( 200UL );

  // In itoa and String types, alphabetic characters are converted to lowercase.
  // Also, the number of digits in the string to be converted depends on the number of digits in the current number.
  char buf[64 + 32],asc[10];
  bin2hex( (uint8_t)count, asc );
  sprintf( buf, "Example 1, Display a uint8_t hex-decimal number using the Original conversion functions. %s\r\n", asc );
  Serial.print( buf );

  bin2hex( (uint16_t)count, asc );
  sprintf( buf, "Example 2, Display a uint16_t hex-decimal number using the Original conversion functions. %s\r\n", asc );
  Serial.print( buf );

  bin2hex( (uint32_t)count, asc );
  sprintf( buf, "Example 3, Display a uint32_t hex-decimal number using the Original conversion functions. %s\r\n", asc );
  Serial.print( buf );

  Serial.println();
  count++;
}

static const char hex[] = "0123456789ABCDEF";
/**
  * @brief Convert a number of type uint8_t to a string.
  * It can be from 00 to FF, so it should be a 2-digit number.
  */
char *bin2hex( uint8_t num, char *buf )
{
  buf[0] = hex[num / 0x10];
  num %= 0x10;
  buf[1] = hex[num];
  buf[2] = '\0';

  return buf;
}

/**
  * @brief Convert a number of type uint16_t to a string.
  * It can be from 0000 to FFFF, so it should be a 4-digit number.
  */
char *bin2hex( uint16_t num, char *buf )
{
  buf[0] = hex[num / 0x1000];
  num %= 0x1000;
  buf[1] = hex[num / 0x100];
  num %= 0x100;
  buf[2] = hex[num / 0x10];
  num %= 0x10;
  buf[3] = hex[num];
  buf[4] = '\0';

  return buf;
}

/**
  * @brief Convert a number of type uint32_t to a string.
  * It can be from 00000000 to FFFFFFFF, so it should be a 8-digit number.
  */
char *bin2hex( uint32_t num, char *buf )
{
  buf[0] = hex[num / 0x10000000];
  num %= 0x10000000;
  buf[1] = hex[num / 0x1000000];
  num %= 0x1000000;
  buf[2] = hex[num / 0x100000];
  num %= 0x100000;
  buf[3] = hex[num / 0x10000];
  num %= 0x10000;
  buf[4] = hex[num / 0x1000];
  num %= 0x1000;
  buf[5] = hex[num / 0x100];
  num %= 0x100;
  buf[6] = hex[num / 0x10];
  num %= 0x10;
  buf[7] = hex[num];
  buf[8] = '\0';

  return buf;
}
