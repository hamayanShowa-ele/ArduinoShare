/**********************************************************/
/* Example of using a external pin interrupt on           */
/*   an STM32 microcontroller.                            */
/**********************************************************/

/**********************************************************/
/* prototypes                                             */
/**********************************************************/
void cbExternalPinChange( void );

/**********************************************************/
/* valiables                                              */
/**********************************************************/
uint32_t intCounter;
uint32_t baseTim;

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
  // interrupt pin initialize.
  pinMode( PA10, INPUT_PULLUP );  // D2:PA10

  Serial.begin( 115200UL );
  while( !Serial ) { delay( 20 ); }
  Serial.println( "STM32 External Pin Interrupt Demo." );
  delay( 100UL );

  baseTim = millis();
  // attach interrupt initialize.
  attachInterrupt( PA10, cbExternalPinChange, FALLING );  // D2:PA10 LOW or CHANGE or RISING or FALLING
}

/**********************************************************/
/* loop                                                   */
/**********************************************************/
void loop()
{
  if( (millis() - baseTim) >= 1000UL )
  {
    baseTim = millis();
    Serial.print( "intCounter = " );
    Serial.println( intCounter, DEC );
  }
}

/**********************************************************/
/* timer interrupt call back routine.                     */
/**********************************************************/
void cbExternalPinChange( void )
{
  intCounter++;
//  Serial.write( '*' );
}
