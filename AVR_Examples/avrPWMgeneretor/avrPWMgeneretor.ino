/**********************************************************/
/* AVR Atmega328P PWM generator                           */
/**********************************************************/
#include <avr/io.h>
#include <FlexiTimer2.h>

extern "C"
{
}

/**********************************************************/
/* prototypes                                             */
/**********************************************************/

/**********************************************************/
/* valiables                                              */
/**********************************************************/
#define  PWMA_PIN     9  // PB1:OC1A
#define  PWMB_PIN     10  // PB2:OC1B
#define  Voffset_PIN  11
#define  SWITCH_PIN   12
#define  OUTPUT_PIN   13
#define  TIMER_COUNTER_CLOCK  (1000000)
#define  PHASE_SHIFT          (-60.0f)
/* 50hz */
#define  FREQUENCY            (50.0f)
#define  RESOLUTION           (50)  /* 16000000hz = 50hz * 256 * 50 * 25 */
/* 60hz */
//#define  FREQUENCY            (60.0f)  /* 16000000hz = 60hz * 256 * 40 * 26.0416667 */
//#define  RESOLUTION           (40)

int scale = (256 / 2) - 1;  /* 1000000 is timer clock */
int offset = (256 / 2) - 0;  /* center position */
int waveTableCounter;
float channelPhase = (2 * M_PI * PHASE_SHIFT / 360.0f);
unsigned int waveTableA[ RESOLUTION ];
unsigned int waveTableB[ RESOLUTION ];

/**********************************************************/
/* setup                                                  */
/**********************************************************/
void setup()
{
  Serial.begin( 115200UL );
  Serial.println( "AVR Atmega328P PWM generator." );
  digitalWrite( PWMA_PIN, LOW );
  digitalWrite( PWMB_PIN, LOW );
  pinMode( PWMA_PIN, OUTPUT );
  pinMode( PWMB_PIN, OUTPUT );

  digitalWrite( OUTPUT_PIN, HIGH );  /* relay off */
  digitalWrite( Voffset_PIN, LOW );
  pinMode( SWITCH_PIN, INPUT );
  pinMode( OUTPUT_PIN, OUTPUT );
  pinMode( Voffset_PIN, OUTPUT );

  /* generate wave data */
  for( int i = 0; i < RESOLUTION; i++ )
  {
    int tempI = (int)(sin(2 * M_PI * i / RESOLUTION) * scale);
    waveTableA[ i ] = (unsigned int)tempI + offset;
    tempI = (int)(sin((2 * M_PI * i / RESOLUTION) + channelPhase) * scale);
    waveTableB[ i ] = (unsigned int)tempI + offset;
  }

  /* display wave data A */
  Serial.println( "WAVE DATA A" );
  for( int i = 0; i < RESOLUTION; i++ )
  {
    Serial.print( waveTableA[ i ], DEC );
    Serial.print( "," );
    if( (i % 10) == 9 ) Serial.println();
  }
  Serial.println();

  /* display wave data B */
  Serial.println( "WAVE DATA B" );
  for( int i = 0; i < RESOLUTION; i++ )
  {
    Serial.print( waveTableB[ i ], DEC );
    Serial.print( "," );
    if( (i % 10) == 9 ) Serial.println();
  }
  Serial.println();

  /* timer counter initialize. */
  TCCR1B = 0x00;  //  timer1 stop
  TCNT1 = 0x0000;  // 16bit counter clear
  TIMSK1 = 0x00;  // interrupt disable
  TCCR1A = 0b10100001;  //  0b10 10 00 01
    // OC1A : compare match goes OC1A to clear and top goes OC1A to set.
    // OC1B : compare match goes OC1B to clear and top goes OC1B to set.
    // fast 8bit PWM and top is 0x00FF. WGM = 0b0101
  TCCR1B = 0b00001010;  //  0b0 0 0 01 010
    // fast 8bit PWM and top is 0x00FF.
    // clkIO / 8 priscaller.
  OCR1A = waveTableA[ 0 ];
  OCR1B = waveTableB[ 0 ];
  TIMSK1 = 0x00;  // interrupt disable
  digitalWrite( Voffset_PIN, HIGH );  // always HIGH
  digitalWrite( OUTPUT_PIN, LOW );  /* relay on */

  waveTableCounter = 1;

  FlexiTimer2::set( 1, 1.0f / (FREQUENCY * RESOLUTION), pwmUpdateHandler );
  FlexiTimer2::start();

  /* Infinite loop */
  while(1) {}
}

/**********************************************************/
/* loop ※使われない                                      */
/**********************************************************/
void loop()
{
  delay( 200UL );
  digitalWrite( OUTPUT_PIN, HIGH );
  delay( 200UL );
  digitalWrite( OUTPUT_PIN, LOW );
}

/**********************************************************/
/* pwm update handler                                     */
/**********************************************************/
void pwmUpdateHandler()
{
  OCR1A = waveTableA[ waveTableCounter ];
  OCR1B = waveTableB[ waveTableCounter ];
  if( ++waveTableCounter >= RESOLUTION )
  {
    waveTableCounter = 0;
  }
}
