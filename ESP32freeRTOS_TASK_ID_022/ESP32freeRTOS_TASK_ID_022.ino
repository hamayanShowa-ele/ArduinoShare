xSemaphoreHandle mux;

void setup()
{
  Serial.begin( 115200 );
  Serial.println( "FreRTOS Test." );

  mux = xSemaphoreCreateMutex();
  /* configure take task1. */
  xTaskCreatePinnedToCore(
    lowerPriorityTask,  /* task name */
    "",     /* task name string */
    configMINIMAL_STACK_SIZE,   /* stack size */
    NULL,   /* execute parameter */
    2,      /* task priority : 0 to 24. 0 is lowest priority. */
    NULL,   /* task handle pointer */
    1       /* core ID */
  );

  /* configure take task2. */
  xTaskCreatePinnedToCore(
    upperPriorityTask,  /* task name */
    "",     /* task name string */
    configMINIMAL_STACK_SIZE,   /* stack size */
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

void lowerPriorityTask( void *execParam )
{
  int count = 1;
  while( 1 )
  {
    xSemaphoreTake( mux, portMAX_DELAY );
    Serial.print( "lowerPriorityTask count = " ); Serial.println( count++,DEC );
    xSemaphoreGive( mux );

    vTaskDelay( pdMS_TO_TICKS( 1 ) );
  }
}

void upperPriorityTask( void *execParam )
{
  int count = 1;
  while( 1 )
  {
    xSemaphoreTake( mux, portMAX_DELAY );
    Serial.print( "upperPriorityTask count = " ); Serial.println( count++,DEC );
    xSemaphoreGive( mux );

    vTaskDelay( pdMS_TO_TICKS( 10 ) );
  }
}
