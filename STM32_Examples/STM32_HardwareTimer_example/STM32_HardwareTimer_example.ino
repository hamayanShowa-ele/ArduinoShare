/**********************************************************/
/* Example of using a hardware timer on                   */
/*   an STM32 microcontroller.                            */
/**********************************************************/
#include  <HardwareTimer.h>

/**********************************************************/
/* prototypes                                             */
/**********************************************************/
void updateSystemTimer();

/**********************************************************/
/* valiables                                              */
/**********************************************************/
HardwareTimer timer( TIM2 );
uint32_t intCount;

/**********************************************************/
/* setup                                                  */
/**********************************************************/
void setup()
{
  /* Operation of the port for the USB to be recognized by the PC. */
//  pinMode( PC13, OUTPUT );  // 
//  digitalWrite( PC13, LOW );  // 
//  pinMode( LED_BUILTIN, OUTPUT );
//  digitalWrite( LED_BUILTIN, LOW );

  Serial.begin( 115200UL );
  while( !Serial ) { delay( 20 ); }
  Serial.println( "STM32 Hardware Timer Demo." );
  delay( 100UL );

  // timer initialize.
  timer.pause();
  timer.setPrescaleFactor( 90UL );
    /* 1Mhz timer clock. F103=72,F401=84,F446=90,L053=32 */
  timer.setOverflow( 1 * 1000UL );  /* 1ms interrupt */
  timer.attachInterrupt( updateSystemTimer );
  timer.refresh();
  timer.resume();

  uint32_t baseTime = millis();
  while( 1 )
  {
    if( (millis() - baseTime) >= 1000UL )
    {
      baseTime = millis();
      Serial.printf( "intCount = %d\r\n", intCount );
    }
  }
}

/**********************************************************/
/* loop                                                   */
/**********************************************************/
void loop() {}

/**********************************************************/
/* timer interrupt call back routine.                     */
/**********************************************************/
void updateSystemTimer()
{
  intCount++;
}
