/*
  BOARD 2113 setup.
  Board type : Generic STM32F4 Generic F407ZETx
*/

#ifndef  __BD2113_H__
#define  __BD2113_H__

#define  BD2113_ADC1_ADDRESS   0x20
#define  BD2113_ADC2_ADDRESS   0x26
#define  BD2113_PULSE_ADDRESS  0x24
#define  BD2113_ADC_MASK  0x3FFF
#define  BD2113_PPS_MASK  0x8000

class BD2113 // public hoge
{
private:
  volatile uint16_t *adr;

public:
  BD2113( volatile uint16_t *a ) { adr = a; }
  int write( uint16_t dat ) { *adr = dat; return 1; }
  uint16_t read() { return *adr; }
};

#endif	 /* __BD2113_H__ */
