// hello_aquestalk.ino - AquesTalk pico for ESP32 サンプルプログラム
#include "driver/i2s.h"
#include <aquestalk.h>

/*************************************************************************/
/*  instances and valiables.                                             */
/*************************************************************************/
static unsigned long monitorPrintTim;

/*************************************************************************/
/*  task prototype defines.                                              */
/*************************************************************************/
xTaskHandle taskHandle[ 1 ];

void aquesTalkTask( void *execParam );

/*************************************************************************/
/*  setup.                                                               */
/*************************************************************************/
void setup()
{
  Serial.begin(115200);

  /* configure aquesTalk task. */
  xTaskCreatePinnedToCore(
    aquesTalkTask, "",
    (configMINIMAL_STACK_SIZE * 2) + (sizeof(uint32_t) * AQ_SIZE_WORKBUF),
    NULL, 2, &taskHandle[0], 1 );
}

/*************************************************************************/
/*  loop.                                                                */
/*************************************************************************/
void loop()
{
  /* stack monitor. */
  if( (millis() - monitorPrintTim) >= (5 * 1000UL) )
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
  String msg = "  Task"; msg += String( count ); msg += " remains stack size = "; msg += String( stzckSize );
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
    Play("konnnichiwa.");
    vTaskDelay( pdMS_TO_TICKS( 100 ) );

    Play("korewa;te'_sutode_su.");
    vTaskDelay( pdMS_TO_TICKS( 100 ) );

    Play("sa'nngatsu/<NUMK VAL=17 COUNTER=nichi> <NUMK VAL=12 COUNTER=ji>/<NUMK VAL=23 COUNTER=funn>.");
    vTaskDelay( pdMS_TO_TICKS( 100 ) );

    Play("yukkuri_siteittene?");
    vTaskDelay( pdMS_TO_TICKS( 100 ) );

    Play("korewa onse-ki'go-desu.");

    vTaskDelay( pdMS_TO_TICKS( 5000 ) );
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
