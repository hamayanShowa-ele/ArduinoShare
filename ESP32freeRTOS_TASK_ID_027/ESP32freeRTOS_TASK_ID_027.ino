xQueueHandle que;

void setup()
{
  Serial.begin( 115200 );
  Serial.println( "FreRTOS Test." );

  /* create data queue */
  que = xQueueCreate( 10, sizeof(char *) );

  /* configure take task1. */
  xTaskCreatePinnedToCore(
    gateKeeperTask,  /* task name */
    "",     /* task name string */
    configMINIMAL_STACK_SIZE,   /* stack size */
    NULL,   /* execute parameter */
    1,      /* task priority : 0 to 24. 0 is lowest priority. */
    NULL,   /* task handle pointer */
    1       /* core ID */
  );

  /* configure take task2. */
  xTaskCreatePinnedToCore(
    task2,  /* task name */
    "",     /* task name string */
    configMINIMAL_STACK_SIZE,   /* stack size */
    NULL,   /* execute parameter */
    2,      /* task priority : 0 to 24. 0 is lowest priority. */
    NULL,   /* task handle pointer */
    1       /* core ID */
  );

  /* configure take task3. */
  xTaskCreatePinnedToCore(
    task3,  /* task name */
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

void gateKeeperTask( void *execParam )
{
  while( 1 )
  {
    char *pvBuffer;
    xQueueReceive( que, (void *)&pvBuffer, portMAX_DELAY );
    Serial.print( pvBuffer );
  }
}

void task2( void *execParam )
{
  static const char *msg = "this is task2.\r\n";
  while( 1 )
  {
    xQueueSendToBack( que, (const void *)&msg, portMAX_DELAY );
    vTaskDelay( pdMS_TO_TICKS( 250 ) );
  }
}

void task3( void *execParam )
{
  static const char *msg = "this is task3.\r\n";
  while( 1 )
  {
    xQueueSendToBack( que, (const void *)&msg, portMAX_DELAY );
    vTaskDelay( pdMS_TO_TICKS( 500 ) );
  }
}
