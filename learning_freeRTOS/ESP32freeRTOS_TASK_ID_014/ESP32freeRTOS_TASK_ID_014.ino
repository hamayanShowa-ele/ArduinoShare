const int ledPin = 2;
hw_timer_t * timer = NULL;
volatile SemaphoreHandle_t timerSemaphore;

#define  GPIObit0To31OutReg   *((volatile unsigned long *)GPIO_OUT_REG)
#define  GPIObit0To31Set      *((volatile unsigned long *)GPIO_OUT_W1TS_REG)
#define  GPIObit0To31Reset    *((volatile unsigned long *)GPIO_OUT_W1TC_REG)

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

  // Use 1st timer of 4 (counted from zero).
  // Set 80 divider for prescaler (see ESP32 Technical Reference Manual for more
  // info).
  timer = timerBegin( 0, 80, true );
  // Attach onTimer function to our timer.
  timerAttachInterrupt( timer, &onTimer, true );
  // Set alarm to call onTimer function every second (value in microseconds).
  // Repeat the alarm (third parameter)
  timerAlarmWrite( timer, 100 * 1000UL, true );
  // Start an alarm
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
//      digitalWrite( ledPin, ( digitalRead(ledPin) ) ? LOW : HIGH );  /* toggle led. */
        if( GPIObit0To31OutReg & (1UL << ledPin) ) GPIObit0To31Reset = (1UL << ledPin);
        else GPIObit0To31Set   = (1UL << ledPin);
    }
  }
}
