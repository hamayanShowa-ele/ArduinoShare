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
static void generateWaveData();

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
#define  FREQUENCY            (10.0f)  // 50.0f
#define  RESOLUTION           (50 * 10)  /* 16000000hz = 50hz * 256 * 50 * 25 */
#define  RESOLUTION_F         (50.0F * 10.0F / 2.0F)  /* 16000000hz = 50hz * 256 * 50 * 25 */
/* 60hz */
//#define  FREQUENCY            (60.0f)  /* 16000000hz = 60hz * 256 * 40 * 26.0416667 */
//#define  RESOLUTION           (40)

//int scale = (256 / 2) - 1;  /* 1000000 is timer clock */
int scale = (int)((128.0 * 2.0 / 6.0) + 0.5);  /* 1000000 is timer clock */
int offset = (256 / 2) - 0;  /* center position */
int ledVF = (int)(-128.0 * (3.6 + 1.0) / 6.0);
int waveTableCounter;
float channelPhase = (2 * M_PI * PHASE_SHIFT / 360.0f);
uint16_t waveTableA[ RESOLUTION ];
uint32_t retry_time,switch_sample_time;

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
  generateWaveData();

  /* display wave data A */
  Serial.println( "WAVE DATA A" );
  for( int i = 0; i < RESOLUTION; i++ )
  {
    Serial.print( waveTableA[ i ], DEC );
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
  waveTableCounter = RESOLUTION - 1;
  OCR1A = waveTableA[ waveTableCounter ];
  TIMSK1 = 0x00;  // interrupt disable
  digitalWrite( Voffset_PIN, HIGH );  // always HIGH
  digitalWrite( OUTPUT_PIN, LOW );  /* relay on */
  FlexiTimer2::set( 1, 1.0f / (FREQUENCY * RESOLUTION), pwmUpdateHandler );
  FlexiTimer2::start();

  retry_time = switch_sample_time = millis();

  /* Infinite loop */
  while(0) {}
}

/**********************************************************/
/* loop ※使われない                                      */
/**********************************************************/
void loop()
{
  /* When a waveform is output when a switch is pressed. */
  static uint16_t swbit = 0;
  if( (millis() - switch_sample_time) >= 20UL )
  {
    switch_sample_time = millis();
    swbit <<= 1;
    if( digitalRead( SWITCH_PIN ) == LOW ) { swbit |= 0x0001; }
    if( swbit == 0x000F ) { waveTableCounter = 0; retry_time = millis(); }
  }

  /* When periodically outputting waveforms. */
  if( (millis() - retry_time) >= 1000UL )
  {
    retry_time = millis();
    if( waveTableCounter == RESOLUTION - 1 ) { waveTableCounter = 0; }
  }
}

/**********************************************************/
/* generate wave data                                     */
/**********************************************************/
static void generateWaveData()
{
#if 0
  /* sine wave */
  for( int i = 0; i < RESOLUTION; i++ )
  {
    int tempI = (int)(sin(2 * M_PI * i / RESOLUTION) * scale);
    tempI += offset + ledVF;
    if( tempI < 0 ) { tempI = 0; }
    if( tempI >= 128 ) { tempI = 127; }
    waveTableA[ i ] = (uint16_t)tempI;
//    waveTableA[ i ] = 0;
  }
#else
  /* x^2 wave */
  int X = -RESOLUTION / 2;
  float scale_root = powf( scale, 0.5F );
  for( int i = 0; i < RESOLUTION; i++ )
  {
    float Y = X * scale_root / RESOLUTION_F; X++;
    Y = powf( Y, 2.0F );
    int tempI = (int)(Y + 0.5F);
    tempI -= scale / 2;
    tempI += offset + ledVF;
    if( tempI < 0 ) { tempI = 0; }
    if( tempI >= 128 ) { tempI = 127; }
    waveTableA[ i ] = (uint16_t)tempI;
//    waveTableA[ i ] = 0;
  }
#endif
}

/**********************************************************/
/* pwm update handler                                     */
/**********************************************************/
void pwmUpdateHandler()
{
  OCR1A = waveTableA[ waveTableCounter ];
  if( ++waveTableCounter >= RESOLUTION )
  {
//    waveTableCounter = 0;  // When outputting waveforms continuously.
    waveTableCounter = RESOLUTION - 1;  // When outputting a waveform in a single shot.
  }
}
