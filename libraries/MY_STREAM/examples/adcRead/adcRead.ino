/**********************************************************/
/* try the stream buffer on AVR micro computer.           */
/**********************************************************/
#include  <HardwareTimer.h>
#include  <MY_STREAM.h>

/**********************************************************/
/* prototypes                                             */
/**********************************************************/
void updateSystemTimer();

/**********************************************************/
/* valiables                                              */
/**********************************************************/
MY_STREAM adcFifo( 32, sizeof(uint16_t) );
HardwareTimer timer( TIM5 );
uint32_t intCount;

/**********************************************************/
/* setup                                                  */
/**********************************************************/
void setup()
{
  /* Operation of the port for the USB to be recognized by the PC. */
  pinMode( PC13, OUTPUT );  // 
  digitalWrite( PC13, LOW );  // 
  pinMode( LED_BUILTIN, OUTPUT );
  digitalWrite( LED_BUILTIN, LOW );

  Serial.begin( 115200UL );
  while( !Serial ) { delay( 20 ); }
  Serial.println( "STM32 Stream buffer Demo." );
  delay( 1000UL );

  // timer initialize.
  timer.pause();
  timer.setPrescaleFactor( 90UL );
    /* 1Mhz timer clock. F103=72,F401=84,F446=90,L053=32 */
  timer.setOverflow( 20 * 1000UL );  /* 20ms interrupt */
  timer.attachInterrupt( updateSystemTimer );
  timer.refresh();
  timer.resume();

  uint32_t baseTime = millis();
  while( 0 )
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
void loop()
{
  static int index;
  static uint16_t adcBuffer[4][50];
  if( adcFifo.available() >= 4 )
  {
    for( int i = 0; i < 4; i++ )
    {
      adcBuffer[ i ][ index ] = adcFifo.read();
    }
    if( ++index >= 50 )
    {
      index = 0;
      uint32_t ave[4] = {0UL,0UL,0UL,0UL};
      for( int i = 0; i < 50; i++ )
      {
        ave[0] += adcBuffer[0][ i ];
        ave[1] += adcBuffer[1][ i ];
        ave[2] += adcBuffer[2][ i ];
        ave[3] += adcBuffer[3][ i ];
      }
      ave[0] /= 50UL;
      ave[1] /= 50UL;
      ave[2] /= 50UL;
      ave[3] /= 50UL;

      String str = "A0=" + String( ave[0], DEC );
      str += " A1=" + String( ave[1], DEC );
      str += " A2=" + String( ave[2], DEC );
      str += " A3=" + String( ave[3], DEC );
      Serial.println( str );
    }
  }
}

/**********************************************************/
/* timer interrupt call back routine.                     */
/**********************************************************/
void updateSystemTimer()
{
  intCount++;

  uint16_t a0Value = analogRead( 0 );  // PA0
  uint16_t a1Value = analogRead( 1 );  // PA1
  uint16_t a2Value = analogRead( 2 );  // PA4
  uint16_t a3Value = analogRead( 3 );  // PB0
  adcFifo.write( a0Value, 0UL );
  adcFifo.write( a1Value, 0UL );
  adcFifo.write( a2Value, 0UL );
  adcFifo.write( a3Value, 0UL );
}
