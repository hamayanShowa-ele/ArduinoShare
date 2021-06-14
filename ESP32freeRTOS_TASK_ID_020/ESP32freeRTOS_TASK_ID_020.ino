void setup()
{
  Serial.begin( 115200 );
  Serial.println( "FreRTOS Test." );

  /* configure take task1. */
  xTaskCreatePinnedToCore(
    lowerPriorityTask,  /* task name */
    "",     /* task name string */
    1024,   /* stack size */
    NULL,   /* execute parameter */
    2,      /* task priority : 0 to 24. 0 is lowest priority. */
    NULL,   /* task handle pointer */
    1       /* core ID */
  );

  /* configure take task2. */
  xTaskCreatePinnedToCore(
    upperPriorityTask,  /* task name */
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

void lowerPriorityTask( void *execParam )
{
  int count = 1;
  portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
  while( 1 )
  {
    taskENTER_CRITICAL( &mux ) ;
    Serial.print( "lowerPriorityTask count = " ); Serial.println( count++,DEC );
    taskEXIT_CRITICAL( &mux );

    vTaskDelay( pdMS_TO_TICKS( 1 ) );
  }
}

void upperPriorityTask( void *execParam )
{
  int count = 1;
  while( 1 )
  {
    Serial.print( "upperPriorityTask count = " ); Serial.println( count++,DEC );

    vTaskDelay( pdMS_TO_TICKS( 10 ) );
  }
}
