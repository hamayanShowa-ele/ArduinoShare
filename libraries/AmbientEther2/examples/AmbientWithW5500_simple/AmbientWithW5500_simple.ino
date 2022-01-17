/*
 * Send something to Ambient.
 * Copyright (c) 2022 Kazuaki Hamahara
 *  Released under the MIT license
 *  https://github.com/YukinobuKurata/YouTubeMagicBuyButton/blob/master/MIT-LICENSE.txt
 */

#include <SPI.h>
#include <Ethernet2.h>
#include <AmbientEther2.h>

unsigned long channelId = 01234; // Ambient channel ID
unsigned long boradID = 56789;  // board ID if you need.
const char *writeKey = "write your write key."; // write key
const char *readKey = "write your read key.";  // read key if you need.
const char *userKey = "write your user key.";  // user key if you need.
const char *deviceKey = "write your device key.";  // device key if you need.

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
//char server[] = "www.google.com";    // name address for Google (using DNS)
char server[] = "www.chiseki.go.jp";    // name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress constantIP(192, 168, 1, 250);
IPAddress myIP;

// Initialize the Ethernet client library
EthernetClient client;
AmbientEther2 ambient;

#define  W5500_RST  A0
#define  W5500_MOSI 11
#define  W5500_MISO 12
#define  W5500_SCK  13
#define  W5500_SSEL 10

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin( 115200UL );
  while ( !Serial )
  {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.println( "Board W5500 sheild start up message from Serial." );

  // setup SPI for the W5500.
  pinMode( W5500_RST, OUTPUT );
  digitalWrite( W5500_RST, LOW );
  delay( 10UL );
  digitalWrite( W5500_RST, HIGH );

  // start the Ethernet connection:
  Ethernet.init( W5500_SSEL );
  if( Ethernet.begin(mac) == 0 )
  {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, constantIP);
  }
  myIP = Ethernet.localIP();
  Serial.print( "\r\nIP number assigned by DHCP is " ); Serial.println( myIP );

  bool result = ambient.begin( channelId, writeKey, &client ); // Initialize Ambient by specifying the channel ID and light key.
  if( result == false )
  {
    Serial.println( "ambient connection error." );
  }

#if 0
  ambient.delete_data( userKey );
  unsigned long cID;
  char wKey[32];
  ambient.getchannel( userKey, deviceKey, cID, wKey, (int)sizeof(wKey), &client );
  String s = "channel id = "; s += cID;
  s += " write key = "; s += wKey;
  Serial.println( s );
  while( 1 ) delay( 1 );
#endif
}

void loop()
{
  static double count = 0.0;

  Serial.print( "update data to ambient. " );
  Serial.println( count );
  ambient.set( 1, count ); //
  ambient.set( 2, count + 0.1 ); //
  ambient.set( 3, count + 0.2 ); //
  ambient.set( 4, count + 0.3 ); //
  ambient.set( 5, count + 0.4 ); //
  ambient.set( 6, count + 0.5 ); //
  ambient.set( 7, count + 0.6 ); //
  ambient.set( 8, count + 0.7 ); //
  ambient.setcmnt( "this is arduino uno3 with ethernet shield." ); //
  count += 0.1;
  if( count > 1.0 ) count = 0.0;
  bool result = ambient.send(); // Send data to Ambient.
  if( result == false )
  {
    Serial.println( "ambient data transmit error." );
  }

  delay( 30 * 1000UL );
}
