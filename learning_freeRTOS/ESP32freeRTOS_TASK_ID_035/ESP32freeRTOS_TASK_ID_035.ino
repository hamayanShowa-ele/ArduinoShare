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
// hello_aquestalk.ino - AquesTalk pico for ESP32 サンプルプログラム

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "driver/i2s.h"
#include <aquestalk.h>

/*************************************************************************/
/*  instances and valiables.                                             */
/*************************************************************************/
IPAddress myIP; // self IP address
static unsigned long monitorPrintTim;
volatile xQueueHandle que;

/*************************************************************************/
/*  task prototype defines.                                              */
/*************************************************************************/
xTaskHandle taskHandle[ 2 ];
void blynkTask( void *execParam );
void aquesTalkTask( void *execParam );

/*************************************************************************/
/*  Wi-Fi defines.                                                       */
/*************************************************************************/
const char ssid[] = "ap ssid";
const char pass[] = "ap password";

/*************************************************************************/
/*  blynk defines.                                                       */
/*************************************************************************/
const char auth[] = "your blynk token";

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

  // Create data queue.
  que = xQueueCreate( 10, sizeof(char *) );

  /* active the blynk demoTask. */
  xTaskCreatePinnedToCore(
    blynkTask, "",
    configMINIMAL_STACK_SIZE * 4,
    NULL, 1, &taskHandle[ 0 ], 1 );

  /* active the aquesTalk task. */
  xTaskCreatePinnedToCore(
    aquesTalkTask, "",
    (configMINIMAL_STACK_SIZE * 2) + (sizeof(uint32_t) * AQ_SIZE_WORKBUF),
    NULL, 2, &taskHandle[1], 1 );
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
  while( 1 )
  {
    Blynk.run();
//    vTaskDelay( pdMS_TO_TICKS( 10 ) );
  }
}

// Attach virtual serial terminal to Virtual Pin V4
WidgetTerminal terminal(V4);

/*************************************************************************/
/*  blynk virtual pin 0 function.                                        */
/*************************************************************************/
BLYNK_WRITE( V0 )
{ 
  static const char *talk = "konnnichiwa.";
  int value = param.asInt();
  if( value )
  {
    xQueueSendToBack( que, (const void *)&talk, 0 );
    terminal.println( talk );
    terminal.flush();
  }
}

/*************************************************************************/
/*  blynk virtual pin 1 function.                                        */
/*************************************************************************/
BLYNK_WRITE( V1 )
{ 
  static const char *talk = "korewa;te'_sutode_su.";
  int value = param.asInt();
  if( value )
  {
    xQueueSendToBack( que, (const void *)&talk, 0 );
    terminal.println( talk );
    terminal.flush();
  }
}

/*************************************************************************/
/*  blynk virtual pin 2 function.                                        */
/*************************************************************************/
BLYNK_WRITE( V2 )
{ 
  static const char *talk = "sa'nngatsu/<NUMK VAL=17 COUNTER=nichi> <NUMK VAL=12 COUNTER=ji>/<NUMK VAL=23 COUNTER=funn>.";
  int value = param.asInt();
  if( value )
  {
    xQueueSendToBack( que, (const void *)&talk, 0 );
    terminal.println( talk );
    terminal.flush();
  }
}

/*************************************************************************/
/*  blynk virtual pin 3 function.                                        */
/*************************************************************************/
BLYNK_WRITE( V3 )
{ 
  static const char *talk = "yukkuri_siteittene?";
  int value = param.asInt();
  if( value )
  {
    xQueueSendToBack( que, (const void *)&talk, 0 );
    terminal.println( talk );
    terminal.flush();
  }
}

/*************************************************************************/
/*  blynk virtual pin 4 function.                                        */
/*************************************************************************/
BLYNK_WRITE( V4 )
{ 
  char *talk = (char *)param.getBuffer();
  int length = param.getLength();
  xQueueSendToBack( que, (const void *)&talk, 0 );
  terminal.write( talk, length );
  terminal.println();
  terminal.flush();
}

/*************************************************************************/
/*  aquesTalkTask                                                        */
/*************************************************************************/
void aquesTalkTask( void *execParam )
{
  #define LEN_FRAME 32
  uint32_t workbuf [ AQ_SIZE_WORKBUF ];

  Serial.println( "Initialize AquesTalk" );
  int iret = CAqTkPicoF_Init(workbuf, LEN_FRAME, NULL);  // CAqTkPicoF_Init(workbuf, LEN_FRAME, "licence code");
  if(iret) Serial.println("ERR:CAqTkPicoF_Init");

  DAC_Create();
  Serial.println("D/A start");

  while( 1 )
  {
    char *talk;
    xQueueReceive( que, (void *)&talk, portMAX_DELAY );
    Play( talk );
  }
  DAC_Release();
  Serial.println("D/A stop");
  // vTaskDelete( NULL );  /* delete aquesTalkTask. if you want. */
}


// 一文の音声出力（同期型）
void Play( const char *koe )
{
  Serial.print( "Play:" );
  Serial.println( koe );

  int iret = CAqTkPicoF_SetKoe( (const uint8_t*)koe, 100, 0xffffU );
  if( iret )  Serial.println( "ERR:CAqTkPicoF_SetKoe" );

  for(;;)
  {
    int16_t wav[ LEN_FRAME ];
    uint16_t len;
    iret = CAqTkPicoF_SyntheFrame( wav, &len );
    if( iret ) break; // EOD
    
    DAC_Write( (int)len, wav );
  }
}

////////////////////////////////
//i2s configuration 
const int i2s_num = 0; // i2s port number
i2s_config_t i2s_config =
{
  .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
  .sample_rate = 24000,
  .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
  .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
  .communication_format = (i2s_comm_format_t)I2S_COMM_FORMAT_I2S_MSB,
  .intr_alloc_flags = 0,
  .dma_buf_count = 4,
  .dma_buf_len = 384,
  .use_apll = 0
};

void DAC_Create()
{
  AqResample_Reset();

  i2s_driver_install((i2s_port_t)i2s_num, &i2s_config, 0, NULL);
  i2s_set_pin((i2s_port_t)i2s_num, NULL);
}

void DAC_Release()
{
  i2s_driver_uninstall((i2s_port_t)i2s_num); //stop & destroy i2s driver 
}

// upsampling & write to I2S
int DAC_Write( int len, int16_t *wav )
{
  int i = 0;
  for( i = 0; i < len; i++ )
  {
    // upsampling x3
    int16_t wav3[ 3 ];
    AqResample_Conv( wav[ i ], wav3 );
    // write to I2S DMA buffer
    for(int k = 0; k < 3; k++ )
    {
      uint16_t sample[ 2 ];
      uint16_t us = ((uint16_t)wav3[ k ])^0x8000U;  // signed -> unsigned data 内蔵DA Only
      sample[ 0 ] = sample[ 1 ] = us; // mono -> stereo
      int iret = i2s_push_sample((i2s_port_t)i2s_num, (const char *)sample, 100);
      if( iret < 0 ) return iret; // -1:ESP_FAIL
      if( iret == 0 ) break;  //  0:TIMEOUT
    }
  }
  return i;
}
