volatile SemaphoreHandle_t xSemaphore;

void setup()
{
  Serial.begin( 115200 );
  Serial.println( "FreRTOS Test." );

  // Create semaphore to inform us when the timer has fired
  xSemaphore = xSemaphoreCreateBinary();

  /* configure take task. */
  xTaskCreatePinnedToCore(
    takeTask,  /* task name */
    "",     /* task name string */
    1024,   /* stack size */
    NULL,   /* execute parameter */
    2,      /* task priority : 0 to 24. 0 is lowest priority. */
    NULL,   /* task handle pointer */
    1       /* core ID */
  );

  /* configure give task. */
  xTaskCreatePinnedToCore(
    giveTask,  /* task name */
    "",     /* task name string */
    1024,   /* stack size */
    NULL,   /* execute parameter */
    3,      /* task priority : 0 to 24. 0 is lowest priority. */
    NULL,   /* task handle pointer */
    1       /* core ID */
  );

  vTaskDelete( NULL );  /* delete loopTask. */
}

void loop()
{
}

void giveTask( void *execParam )
{
  int count = 1;
  while( 1 )
  {
    vTaskDelay( pdMS_TO_TICKS( 500 ) );
    Serial.print( "give count =" ); Serial.println( count, DEC );
    xSemaphoreGive( xSemaphore );
    if( ++count > 50 ) vTaskDelete( NULL );
  }
}

void takeTask( void *execParam )
{
  int count = 1;
  while( 1 )
  {
    if( xSemaphoreTake( xSemaphore, 0 ) == pdTRUE )
    {
      Serial.print( "take count =" ); Serial.println( count++, DEC );
    }
    vTaskDelay( pdMS_TO_TICKS( 550 ) );
  }
}
