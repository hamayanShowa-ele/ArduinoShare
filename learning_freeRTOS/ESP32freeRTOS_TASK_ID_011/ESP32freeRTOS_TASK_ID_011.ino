const int ledPin = 2;
hw_timer_t * timer = NULL;
volatile SemaphoreHandle_t timerSemaphore;

void IRAM_ATTR onTimer()
{
  // Give a semaphore that we can check in the loop
  xSemaphoreGiveFromISR( timerSemaphore, NULL );
}

void setup()
{
  Serial.begin( 115200 );
  Serial.println( "FreRTOS Test." );
  pinMode( ledPin, OUTPUT );
  digitalWrite( ledPin, LOW );  /* turn off led. */

  // Create semaphore to inform us when the timer has fired
  timerSemaphore = xSemaphoreCreateBinary();

  timer = timerBegin( 0, 80, true );
  timerAttachInterrupt( timer, &onTimer, true );
  timerAlarmWrite( timer, 100 * 1000UL, true );
  timerAlarmEnable( timer );

  /* configure led blink task. */
  xTaskCreatePinnedToCore(
    task1,  /* task name */
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

void task1( void *execParam )
{
  while( 1 )
  {
    if( xSemaphoreTake(timerSemaphore, 0) == pdTRUE )
    {
      digitalWrite( ledPin, ( digitalRead(ledPin) ) ? LOW : HIGH );  /* toggle led. */
    }
  }
}
