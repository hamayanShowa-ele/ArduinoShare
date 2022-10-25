/*
  BOARD 2113 setup.
  Board type : Generic STM32F4 Generic F407ZETx
*/

#ifndef  __BD2113_H__
#define  __BD2113_H__

                                     /* Example of description in Class. */
#define  BD2113_ADC1_ADDRESS   0x20  /* BD2113 adc1( (volatile uint16_t *)(EXT_BASE_ADR + BD2113_ADC1_ADDRESS) ); */
#define  BD2113_ADC2_ADDRESS   0x26  /* BD2113 adc2( (volatile uint16_t *)(EXT_BASE_ADR + BD2113_ADC2_ADDRESS) ); */
#define  BD2113_PULSE_ADDRESS  0x24  /* BD2113 pulse( (volatile uint16_t *)(EXT_BASE_ADR + BD2113_PULSE_ADDRESS) ); */
#define  BD2113_ADC_MASK  0x3FFF  /* Example of reading a conversion value from ADC1. adc1.read() & BD2113_ADC_MASK */
#define  BD2113_PPS_MASK  0x8000
#define  BD2113_M_S_MASK  0x4000  /* Example of Master/Slave determination. (adc1.read() & BD2113_M_S_MASK) ? "MASTER" : "SLAVE" */

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
