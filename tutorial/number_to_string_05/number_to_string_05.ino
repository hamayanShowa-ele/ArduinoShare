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
  * @brief Example of memory dump.
  */

void setup()
{
  Serial.begin( 115200UL );
  while( !Serial ) { delay( 20UL ); }
  Serial.println( "hello this is example of memory dump." );
}

//uint8_t buffer[48];
void loop()
{
  static int increment_data = 0;
  delay( 1000UL );

  uint8_t buffer[48];
  // Generate appropriate data.
  for( int i = 0; i < (int)sizeof(buffer); i++ )  { buffer[i] = (uint8_t)increment_data++; }
  // dump memory.
  dump( (const uint8_t *)buffer, sizeof(buffer) );

  Serial.println();
}

void dump( const uint8_t *dat, int size )
{
  char asc[10];
  for( int i = 0; i < size; i += 16 )
  {
    const uint8_t *ptr = dat;
    String line = "0x";
    line += bin2hex( (uint32_t)ptr, asc ); line += " : ";
    for( int j = 0; j < 16; j++ )
    {
      if( j == 8 ) { line += " "; }
      line += bin2hex( *((uint8_t *)ptr), asc ); line += ",";
      ptr++;
    }

    ptr = dat;
    line += "    ";
    for( int j = 0; j < 16; j++ )
    {
      if( j == 8 ) { line += " "; }
      char c = *((char *)ptr);
      if( isprint( c ) ) { line += String( c ); }
      else { line += "."; }
      ptr++;
    }

    Serial.println( line );
    dat += 16;
  }
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
