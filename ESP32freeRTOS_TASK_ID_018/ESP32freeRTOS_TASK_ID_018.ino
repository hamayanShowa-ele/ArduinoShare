volatile SemaphoreHandle_t xSemaphore;

void setup()
{
  Serial.begin( 115200 );
  Serial.println( "FreRTOS Test." );

  xSemaphore = xSemaphoreCreateCounting( 100, 100 );

  /* configure take task1. */
  xTaskCreatePinnedToCore(
    takeTask1,  /* task name */
    "",     /* task name string */
    1024,   /* stack size */
    NULL,   /* execute parameter */
    2,      /* task priority : 0 to 24. 0 is lowest priority. */
    NULL,   /* task handle pointer */
    1       /* core ID */
  );

  /* configure take task2. */
  xTaskCreatePinnedToCore(
    takeTask2,  /* task name */
    "",     /* task name string */
    1024,   /* stack size */
    NULL,   /* execute parameter */
    2,      /* task priority : 0 to 24. 0 is lowest priority. */
    NULL,   /* task handle pointer */
    1       /* core ID */
  );

  vTaskDelete( NULL );  /* delete loopTask. */
}

void loop()
{
}

void takeTask1( void *execParam )
{
  int count = 1;
  while( 1 )
  {
    if( xSemaphoreTake( xSemaphore, 0 ) == pdTRUE )
    {
      Serial.print( "take semaphore 1 : " ); Serial.println( count++,DEC );
    }
    vTaskDelay( pdMS_TO_TICKS( 100 ) );
  }
}

void takeTask2( void *execParam )
{
  int count = 1;
  while( 1 )
  {
    if( xSemaphoreTake( xSemaphore, 0 ) == pdTRUE )
    {
      Serial.print( "take semaphore 2 : " ); Serial.println( count++,DEC );
    }
    vTaskDelay( pdMS_TO_TICKS( 100 ) );
  }
}
