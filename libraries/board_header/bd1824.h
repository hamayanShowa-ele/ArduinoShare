/*
  BOARD 1824 setup.
  Board type : Generic STM32F4 Generic F407ZETx
*/

#ifndef  __BD1824_H__
#define  __BD1824_H__

#define  DEFAULT_BD1824_ADC1_ADDRESS  0x20
#define  DEFAULT_BD1824_ADC2_ADDRESS  0x26
#define  BD1824_ADC_MASK  0x3FFF
#define  BD1824_PPS_MASK  0x8000

class BD1824 // public hoge
{
private:
  volatile uint16_t *adr;

public:
  BD1824( volatile uint16_t *a ) { adr = a; }
  int write( uint16_t dat ) { *adr = dat; return 1; }
  uint16_t read() { return *adr; }
};

#endif	 /* __BD1824_H__ */
