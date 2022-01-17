/**
  ******************************************************************************
  * @file           : rom24aa025.h
  * @brief          : SERIAL EEPROM 24AA025 librarly.
    Copyright (c) 2022 Kazuaki Hamahara
    Released under the MIT license
    https://github.com/YukinobuKurata/YouTubeMagicBuyButton/blob/master/MIT-LICENSE.txt
  ******************************************************************************
  */

#ifndef  __EEPROM_24AA025__
#define  __EEPROM_24AA025__

#include  <Arduino.h>
#include  <Wire.h>

extern "C" {
}

#define  I2C_ADR_24AA025E48         0x50  /* 24AA025E48 mac address i2c eeprom */
#define  MAC_ADDRESS_IN_24AA025E48  0xFA  /* 00:1E:C0 or 80:1F:12 or 00:04:A3 */
                                          /* is Microchip Technology Inc. vender code. */

class EEPROM_24AA025 // : public hoge
{
private:
  TwoWire *wire;
  uint8_t i2cAddress;

public:
  EEPROM_24AA025( TwoWire *_w, uint8_t _a = I2C_ADR_24AA025E48 ) { wire = _w; i2cAddress = _a; }

  void read( uint8_t adr, uint8_t *dest );
  void read( uint8_t adr, uint8_t *dest, int len );
};

 
#endif  /*  __EEPROM_24AA025__ */
