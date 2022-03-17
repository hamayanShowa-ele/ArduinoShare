xQueueHandle que;
xTaskHandle taskHandle[ 3 ];

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
    &taskHandle[ 0 ],   /* task handle pointer */
    1       /* core ID */
  );

  /* configure take task2. */
  xTaskCreatePinnedToCore(
    task2,  /* task name */
    "",     /* task name string */
    configMINIMAL_STACK_SIZE,   /* stack size */
    NULL,   /* execute parameter */
    2,      /* task priority : 0 to 24. 0 is lowest priority. */
    &taskHandle[ 1 ],   /* task handle pointer */
    1       /* core ID */
  );

  /* configure take task3. */
  xTaskCreatePinnedToCore(
    task3,  /* task name */
    "",     /* task name string */
    configMINIMAL_STACK_SIZE,   /* stack size */
    NULL,   /* execute parameter */
    3,      /* task priority : 0 to 24. 0 is lowest priority. */
    &taskHandle[ 2 ],   /* task handle pointer */
    1       /* core ID */
  );
}

void loop()
{
  unsigned portBASE_TYPE stzckSize;
  stzckSize = uxTaskGetStackHighWaterMark( taskHandle[ 0 ] );
  Serial.print( "task1 remain stack size = " ); Serial.println( stzckSize, DEC );

  stzckSize = uxTaskGetStackHighWaterMark( taskHandle[ 1 ] );
  Serial.print( "task2 remain stack size = " ); Serial.println( stzckSize, DEC );

  stzckSize = uxTaskGetStackHighWaterMark( taskHandle[ 2 ] );
  Serial.print( "task3 remain stack size = " ); Serial.println( stzckSize, DEC );

  TaskHandle_t loopTaskHandle = xTaskGetCurrentTaskHandle();
  stzckSize = uxTaskGetStackHighWaterMark( loopTaskHandle );
  Serial.print( "loopTask remain stack size = " ); Serial.println( stzckSize, DEC );

  vTaskDelay( pdMS_TO_TICKS( 1000 ) );
}

void gateKeeperTask( void *execParam )
{
  char buffer[ 128 ];
  while( 1 )
  {
    char *pvBuffer;
    xQueueReceive( que, (void *)&pvBuffer, portMAX_DELAY );
    strcpy( buffer, pvBuffer );
    Serial.print( buffer );
//    Serial.print( pvBuffer );
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
