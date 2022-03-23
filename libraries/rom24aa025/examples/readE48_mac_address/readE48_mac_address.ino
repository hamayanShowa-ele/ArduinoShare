/*
  Arduino IDE setup.
  Board type : Arduino UNO
*/
#include  <rom24aa025.h>

/**
  * @brief instance and global variables.
  */
byte mac[6];
EEPROM_24AA025 eeprom( &Wire );

/**
  * @brief setup.
  * @retval none.
  */
void setup()
{
  Serial.begin( 115200UL );
  while( !Serial ) { delay( 20UL ); }
  Wire.begin();

  // read the mac address from the i2c eeprom.
  eeprom.read( MAC_ADDRESS_IN_24AA025E48, mac, (int)sizeof(mac) );
  char msg[32];
  sprintf( msg, "\r\nMAC:%02X:%02X:%02X:%02X:%02X:%02X\r\n",
    mac[0],mac[1],mac[2],mac[3],mac[4],mac[5] );
  Serial.print( msg );
}

/**
  * @brief loop.
  * @retval none.
  */
void loop() {}
