TaskHandle_t taskHandle[1];  /* typedef void * TaskHandle_t; */
const int ledPin = 2;

void setup()
{
  Serial.begin( 115200 );

  /* configure led blink task. */
  unsigned long executeParameter = 1000UL;  /* delay time */
  xTaskCreatePinnedToCore(
    ledBlinkTask,  /* task name */
    "ledBlinkTask",   /* task name string */
    1024,           /* stack size */
    &executeParameter, /* execute parameter */
    2,             /* task priority : 0 to 24. 0 is lowest priority. */
    &taskHandle[0],   /* タスクハンドルポインタ */
    1              /* core ID */
  );
}

void loop()
{
  Serial.println( "hello world." );
  delay( 1000 );
}

/* led blink */
void ledBlinkTask( void *execParam )
{
  unsigned long dly = *(unsigned long *)execParam;

  pinMode( ledPin, OUTPUT );
  digitalWrite( ledPin, LOW );  /* turn off led. */
  while( 1 )
  {
    digitalWrite( ledPin, ( digitalRead(ledPin) ) ? LOW : HIGH );  /* toggle led. */
    delay( dly );
  }
}
