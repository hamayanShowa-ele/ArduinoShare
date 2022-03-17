
HardwareSerial Serial1( PA10, PA9 );  // rxd pin, txd pin
//HardwareSerial Serial2( PA3, PA2 );  // rxd pin, txd pin

//#define  stdout  Serial
#define  stdout  Serial1
//#define  stdout  Serial2

uint32_t messageTime;
void setup()
{
  stdout.begin( 115200UL );
  delay( 20UL );
  messageTime = millis();
}

void loop()
{
  if( (millis() - messageTime) >= 1000UL )
  {
    messageTime = millis();
    stdout.println( "hello world." );
  }
  if( stdout.available() )
  {
    int c = stdout.read();
    stdout.write( c );
  }
}
