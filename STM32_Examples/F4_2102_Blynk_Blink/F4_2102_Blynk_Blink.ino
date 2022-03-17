/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  You ??? need:
   - Blynk App (download from AppStore or Google Play)
   - Arduino Uno board
   - Decide how to connect to Blynk
     (USB, Ethernet, Wi-Fi, Bluetooth, ...)

  There is a bunch of great example sketches included to show you how to get
  started. Think of them as LEGO bricks  and combine them as you wish.
  For example, take the Ethernet Shield sketch and combine it with the
  Servo example, or choose a USB sketch and add a code from SendData
  example.
 *************************************************************/

/* Comment this out to disable prints and save space */
#define  BLYNK_PRINT Serial1
#include <SPI.h>
#include <Ethernet2.h>
#include <BlynkSimpleEthernet2.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).

// Remark "#include author.h" and write the Blynk token
//  that you got yourself as follows instead.
#include  "author.h"
#ifndef  __AUTHORED__
char auth[] = "type your blynk token.";  // token
#endif  /* __AUTHORED__ */

#define  ACT_LED    PB4
#define  RED_LED    PB3

#define  TXD6       PC6
#define  RXD6       PC7
#define  RTS6       PC8
#define  TXD1       PA9
#define  RXD1       PA10
#define  USBDM      PA11
#define  USBDP      PA12
#define  USBON      PA15
#define  SCL1       PB6
#define  SDA1       PB7

#define  SPI2_MOSI  PB15
#define  SPI2_MISO  PB14
#define  SPI2_SCK   PB13
#define  W5500_CS   PB12
#define  W5500_RST  PA8
#define  W5500_INT  PC9

#define  DIO0       PB9
#define  DIO1       PC13
#define  DIO2       PC14
#define  DIO3       PC15

#define  IN00       PC0
#define  IN01       PC1
#define  IN02       PC2
#define  IN03       PC3
#define  IN04       PA0
#define  IN05       PA1
#define  IN06       PA2
#define  IN07       PA3

#define  OUT00      PA4
#define  OUT01      PA5
#define  OUT02      PA6
#define  OUT03      PA7
#define  OUT04      PC4
#define  OUT05      PC5
#define  OUT06      PB0
#define  OUT07      PB1

#define  LCD_RES    PB5

HardwareSerial Serial1( RXD1, TXD1 );  // rxd pin, txd pin

void setup()
{
  // Debug console
  Serial1.begin( 115200UL );
  while( !Serial1 ) delay( 20UL );
  Serial1.println( "\r\n\r\nSTM32F4 2102 BLYNK test." );

  pinMode( ACT_LED, OUTPUT );
  digitalWrite( ACT_LED, LOW );
  pinMode( W5500_RST, OUTPUT );
  digitalWrite( W5500_RST, LOW );
  delay( 10UL );
  digitalWrite( W5500_RST, HIGH );
  delay( 500UL );

  // SPI connected with w5500 initialize.
  SPI.setMISO( SPI2_MISO );
  SPI.setMOSI( SPI2_MOSI );
  SPI.setSCLK( SPI2_SCK );

//  SPI.setSSEL( W5500_CS );
  // The BLYNK library sets the CS pin of the Ethernet IC (W5500) as the default SPI pin, and does not call the EthernetClass init (uint8_t cspin), so we need to set the CS pin here.
  Ethernet.init( W5500_CS );

  // You can also specify server:
  Blynk.begin( auth );  // dhcp using and An appropriate MAC address.
  // Blynk.begin( auth, BLYNK_DEFAULT_DOMAIN, BLYNK_SERVER_PORT, const byte mac[] );  // dhcp using and A valid MAC address.
  // Blynk.begin(auth, "blynk-cloud.com", 80);
  // Blynk.begin(auth, IPAddress(192,168,100,250), 8080);
}

void loop()
{
  Blynk.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}

/*
  blynk virtual pin 0 function.
    connected with act led.
*/
BLYNK_WRITE( V0 ) //
{ 
  int value = param.asInt();
  if( value == 0 ) digitalWrite( ACT_LED, LOW );
  else  digitalWrite( ACT_LED, HIGH );
}
