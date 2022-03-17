/*
  STM32F USB Serial demo.
*/
//#include <USBSerial.h>

//#define  BOARD_TYPE_2102_F1
#define  BOARD_TYPE_2102_F4
//#define  BOARD_TYPE_1303_F1
//#define  BOARD_TYPE_MB-STM32F103_F1

#if  defined( BOARD_TYPE_2102_F1 )  // 2102 STM32F103RET
  #undef  LED_BUILTIN
  #define  LED_BUILTIN  PB4
  #define  USB_POWER_ON PA8
  #define  USB_POWER_ON_LEVEL HIGH
#elif  defined( BOARD_TYPE_2102_F4 )  // STM32F401RCT
  #undef  LED_BUILTIN
  #define  LED_BUILTIN  PB4
  #define  USB_POWER_ON PA15
  #define  USB_POWER_ON_LEVEL HIGH
#elif  defined( BOARD_TYPE_1303_F1 )  // 1303 PIXIS
  #undef  LED_BUILTIN
  #define  LED_BUILTIN  PB7
  #define  USB_POWER_ON PE1
  #define  USB_POWER_ON_LEVEL HIGH
#elif  defined( BOARD_TYPE_MB-STM32F103_F1 )  // MB-STM32F103
  #undef  LED_BUILTIN
  #define  LED_BUILTIN  PB7
  #define  USB_POWER_ON PC13
  #define  USB_POWER_ON_LEVEL LOW
#endif  /* defined( BOARD_TYPE_2102_F1 ) */

//#define  stdout  SerialUSB
#define  stdout  Serial

uint32_t blinkTime,messageTime;

void setup()
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode( LED_BUILTIN, OUTPUT );
  digitalWrite( LED_BUILTIN, LOW );

  pinMode( USB_POWER_ON, OUTPUT );
  digitalWrite( USB_POWER_ON, USB_POWER_ON_LEVEL );

  stdout.begin();
  digitalWrite( LED_BUILTIN, HIGH );
  delay( 1 * 1000UL );
  stdout.println( "  STM32Duino running on ???? board." );

  blinkTime = millis();
  messageTime = millis();
}

int count;
void loop()
{
  if( (millis() - blinkTime) >= 500UL )
  {
    blinkTime = millis();
    digitalWrite( LED_BUILTIN, digitalRead( LED_BUILTIN ) == HIGH ? LOW : HIGH );
  }

#if 1
  if( (millis() - messageTime) >= 1000UL )
  {
    messageTime = millis();
    stdout.print( "  hello world. count = " );
    stdout.println( count++, DEC );
  }
#endif

  int num;
  if( (num = stdout.available()) > 0  )
  {
    uint8_t buf[ num ];
    stdout.readBytes( (char *)buf, num );
    stdout.write( (const uint8_t *)buf, num );
    stdout.flush();
  }
}
