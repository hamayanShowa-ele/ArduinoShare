/*
  BOARD 2116 setup.
  Board type : Generic STM32F4 Generic F407ZETx
*/

#ifndef  __BD2116_H__
#define  __BD2116_H__

#define  BD2116_PULSE1_ADDRESS   0x28
#define  BD2116_PULSE2_ADDRESS   0x2A
#define  BD2116_PULSE3_ADDRESS   0x2C
#define  BD2116_PULSE4_ADDRESS   0x2E

class BD2116 // public hoge
{
private:
  volatile uint16_t *adr;

public:
  BD2116( volatile uint16_t *a ) { adr = a; }
  int write( uint16_t dat ) { *adr = dat; return 1; }
  uint16_t read() { return *adr; }
};

#endif	 /* __BD2116_H__ */
