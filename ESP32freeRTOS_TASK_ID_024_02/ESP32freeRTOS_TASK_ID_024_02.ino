volatile xSemaphoreHandle mux;

void setup()
{
  Serial.begin( 115200 );
  Serial.println( "FreRTOS Test." );

  // Create mutex
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
    middlePriorityTask,  /* task name */
    "",     /* task name string */
    configMINIMAL_STACK_SIZE,   /* stack size */
    NULL,   /* execute parameter */
    3,      /* task priority : 0 to 24. 0 is lowest priority. */
    NULL,   /* task handle pointer */
    1       /* core ID */
  );

  /* configure take task3. */
  xTaskCreatePinnedToCore(
    upperPriorityTask,  /* task name */
    "",     /* task name string */
    configMINIMAL_STACK_SIZE,   /* stack size */
    NULL,   /* execute parameter */
    4,      /* task priority : 0 to 24. 0 is lowest priority. */
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
  while( 1 )
  {
    xSemaphoreTake( mux, portMAX_DELAY );
    Serial.print( "lowerPriorityTask time = " ); Serial.println( millis(), DEC );
    xSemaphoreGive( mux );

    vTaskDelay( pdMS_TO_TICKS( 1 ) );
  }
}

void middlePriorityTask( void *execParam )
{
  while( 1 )
  {
    vTaskDelay( pdMS_TO_TICKS( 20 ) );
//    Serial.print( "middlePriorityTask time = " ); Serial.println( millis(), DEC );
    unsigned long baseMillis = millis();
    while( (millis() - baseMillis) < 100UL ) {}  /* never enter block state. */
  }
}

void upperPriorityTask( void *execParam )
{
  int count = 1;
  while( 1 )
  {
    xSemaphoreTake( mux, portMAX_DELAY );
    Serial.print( "upperPriorityTask time = " ); Serial.println( millis() ,DEC );
    xSemaphoreGive( mux );

    vTaskDelay( pdMS_TO_TICKS( 10 ) );
  }
}
