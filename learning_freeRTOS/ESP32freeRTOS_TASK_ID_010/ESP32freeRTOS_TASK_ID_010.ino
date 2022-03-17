volatile SemaphoreHandle_t sema;
volatile xQueueHandle que;

struct DATA_QUEUE_STRING_MESAGE
{
  const char *msg;
  unsigned long size;
};

void setup()
{
  Serial.begin( 115200 );
  Serial.print( "FreRTOS Test. " ); Serial.printf( "ESP32 Chip Revision: %d\r\n\r\n" , ESP.getChipRevision() );

  // Create semaphore.
  sema = xSemaphoreCreateBinary();  //  xSemaphoreGive( semaPhotoShot );  /* give the count for semaPhotoShot  */
  // Create data queue.
  que = xQueueCreate( 5, sizeof(struct DATA_QUEUE_STRING_MESAGE) );

  /*  */
  xTaskCreatePinnedToCore( recieveTask, "", configMINIMAL_STACK_SIZE * 2, NULL, 2, NULL, 1 );
}

void loop()
{
  static int count = 0;
  /* wait semafor */
  xSemaphoreTake( sema, portMAX_DELAY );
  
  /* transmit message through data queue. */
  struct DATA_QUEUE_STRING_MESAGE *queMsg = new struct DATA_QUEUE_STRING_MESAGE;
  String str = "data queue transmit/recieve test. " + String( count++ ) + "\r\n";
  char *msg = new char[ str.length() ];
  memcpy( msg, str.c_str(), str.length() );
  queMsg->msg = msg;
  queMsg->size = str.length();
  xQueueSendToBack( que, (const void *)&queMsg, 0 );

  vTaskDelay( pdMS_TO_TICKS( 1 * 1000UL ) );  /* about 1 seconds. */
}

/*************************************************************************/
/*  task                                                                 */
/*************************************************************************/
void recieveTask( void *execParam )
{
  while( 1 )
  {
    xSemaphoreGive( sema );

    struct DATA_QUEUE_STRING_MESAGE *queMsg;
    xQueueReceive( que, (void *)&queMsg, portMAX_DELAY );
    Serial.write( (const uint8_t *)queMsg->msg, queMsg->size );
    delete[] queMsg->msg;
    delete queMsg;
  }
}
