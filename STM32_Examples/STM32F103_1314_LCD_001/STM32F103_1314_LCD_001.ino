#include  "BOARD_DEPENDENCY.h"
#include  <sg12864.h>
//#include  "ascii8x14_font.h"  /* If you have put the font data in a local folder, enable it here. */
#include  <ascii8x14_font.h>  /* If you have put the font data in a libralies folder, enable it here. */

static const SG12864_PINS pins =
{
  PB1,  /* enable */
  PC5,  /* data(HIGH) or instruction(LOW) */
  PB0,  /* read(HIGH) or write(LOW) */
  PB10,  /* chip select 1 (active HIGH) */
  PB11,  /* chip select 2 (active HIGH) */
  PE7,  /* reset */
  PE8,  /* data bit 0 */
  PE9,  /* data bit 1 */
  PE10,  /* data bit 2 */
  PE11,  /* data bit 3 */
  PE12,  /* data bit 4 */
  PE13,  /* data bit 5 */
  PE14,  /* data bit 6 */
  PE15,  /* data bit 7 */
  PC4,  /* back light */
};

SG12864_IF lcdIF( &pins );
GRAPHIC lcd( &lcdIF );
GRAPHIC_FONT letter(
  &lcd,
  ASCII8x14_CharWidth, ASCII8x14_PixHeight, ASCII8x14_PixWidth,
  ASCII8x14_GraphicBMPTable );

static const uint8_t PROGMEM logo_bmp[] =
{
  B11000000,
  B11000000,
  B11000000,
  B11100000,
  B11100000,
  B11111000,
  B11111111,
  B10011111,
  B11111100,
  B01110000,
  B10100000,
  B11100000,
  B11110000,
  B11110000,
  B01110000,
  B00110000,
  B00000000,
  B00000001,
  B00000001,
  B00000011,
  B11110011,
  B11111110,
  B01111110,
  B00110011,
  B00011111,
  B00001101,
  B00011011,
  B00111111,
  B00111111,
  B01111100,
  B01110000,
  B00000000,              
};

HardwareSerial Serial1( PA10, PA9 );  // rxd pin, txd pin

void setup()
{
  Serial1.begin( 115200UL );
  delay( 20UL );
  Serial1.println( "  THIS IS 1314B main board.\n    designed by hamayan." );
  
  lcdIF.begin();
  lcdIF.backLight( true );
  lcd.begin();

  lcd.rectangleWrite( logo_bmp, 0, 0, 16, 16 );
  delay( 10 * 1000UL );

  lcd.clear();
  letter.putchar( 'A' );
  letter.putchar( 'B' );
  letter.putchar( 'C' );
  letter.putchar( 'D' );
  letter.putchar( 'E' );
  letter.putchar( 'F' );
  letter.putchar( 'G' );
  letter.putchar( 'H' );

  letter.putchar( 'I' );
  letter.putchar( 'J' );
  letter.putchar( 'K' );
  letter.putchar( 'L' );
  letter.putchar( 'M' );
  letter.putchar( 'N' );
  letter.putchar( 'O' );
  letter.putchar( 'P' );

  letter.puts( "abcdefghijklmnopqrstuvwxyz0123456789!\"#$%&'()=~|+*<>?" );
}

void loop()
{
}
