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

  You’ll need:
   - Blynk App (download from AppStore or Google Play)
   - ESP32 board
   - Decide how to connect to Blynk
     (USB, Ethernet, Wi-Fi, Bluetooth, ...)

  There is a bunch of great example sketches included to show you how to get
  started. Think of them as LEGO bricks  and combine them as you wish.
  For example, take the Ethernet Shield sketch and combine it with the
  Servo example, or choose a USB sketch and add a code from SendData
  example.
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

/*************************************************************************/
/*  instances and valiables.                                             */
/*************************************************************************/
IPAddress myIP; // self IP address
static unsigned long monitorPrintTim;

/*************************************************************************/
/*  task prototype defines.                                              */
/*************************************************************************/
xTaskHandle taskHandle[ 1 ];
void blynkTask( void *execParam );

/*************************************************************************/
/*  Wi-Fi defines.                                                       */
/*************************************************************************/
const char ssid[] = "your ap ssid";
const char pass[] = "your ap password";

/*************************************************************************/
/*  blynk defines.                                                       */
/*************************************************************************/
const char auth[] = "your auther token";

/*************************************************************************/
/*  setup.                                                               */
/*************************************************************************/
void setup()
{
  // Debug console
  Serial.begin( 115200 );
  Serial.print( "FreeRTOS Test. " ); Serial.printf( "ESP32 Chip Revision: %d\r\n\r\n" , ESP.getChipRevision() );

  WiFi.disconnect();
  WiFi.begin( ssid, pass );
  while( WiFi.status() != WL_CONNECTED )
  {
     Serial.print( "." );
     vTaskDelay( pdMS_TO_TICKS( 100UL ) );
  }
  myIP = WiFi.localIP();
  Serial.print( "\r\nIP number assigned by DHCP is " );
  Serial.println( myIP );

  /* active the blynk demoTask. */
  xTaskCreatePinnedToCore( blynkTask, "", configMINIMAL_STACK_SIZE * 4, NULL, 2, &taskHandle[ 0 ], 1 );
}

/*************************************************************************/
/*  loop.                                                                */
/*************************************************************************/
void loop()
{
  /* stack monitor. */
  if( (millis() - monitorPrintTim) >= (10 * 1000UL) )
  {
    monitorPrintTim = millis();
    stackMonitor();
  }
}

/*************************************************************************/
/*  stack monitor                                                        */
/*************************************************************************/
static void reportStackSize( TaskHandle_t taskHdl, int count )
{
  unsigned portBASE_TYPE stzckSize = uxTaskGetStackHighWaterMark( taskHdl );
  String msg = "  Task" + String( count ) + " remains stack size = " + String( stzckSize );
  Serial.println( msg );
}

static void stackMonitor()
{
  int i;
  for( i = 0; i < sizeof(taskHandle) / sizeof(taskHandle[ 0 ]); i++ )
  {
    reportStackSize( taskHandle[ i ], i );
  }

  TaskHandle_t loopTaskHandle = xTaskGetCurrentTaskHandle();
  reportStackSize( loopTaskHandle, i );
  Serial.println();
}

/*************************************************************************/
/*  blynk task.                                                          */
/*************************************************************************/
void blynkTask( void *execParam )
{
  Blynk.config( auth );
  //Blynk.config( auth, BLYNK_DEFAULT_DOMAIN, BLYNK_DEFAULT_PORT );
  //Blynk.begin( auth, ssid, pass );
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8442);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8442);
  pinMode( 2, OUTPUT );

  while( 1 )
  {
    Blynk.run();
    vTaskDelay( pdMS_TO_TICKS( 10 ) );
  }
}

/*************************************************************************/
/*  blynk virtual pin 0 function.                                        */
/*************************************************************************/
BLYNK_WRITE( V0 ) //
{ 
  int value = param.asInt();
  if( value == 0 ) digitalWrite( 2, LOW );
  else  digitalWrite( 2, HIGH );
}
