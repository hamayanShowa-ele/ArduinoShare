/*
  BOARD __BD_SERIAL_COM_SELECTOR_H__ setup.
  Board type : NUCLEO-F446RE
*/

#ifndef	 __BD_SERIAL_COM_SELECTOR_H__
#define	 __BD_SERIAL_COM_SELECTOR_H__

#define  USBDM  PA11
#define  USBDP  PA12
#define  USBON  PC5
#define  USB_POWER_ON_LEVEL HIGH

#define  OLED_RESET   PB2 // Reset pin # (or -1 if sharing Arduino reset pin)

#define  TXD1  PA_9
#define  RXD1  PA_10
#define  TXD2  PA_2
#define  RXD2  PA_3
#define  TXD3  PC_10_ALT1  // PC10
#define  RXD3  PC_11_ALT1  // PC11
#define  TXD4  PA_0
#define  RXD4  PA_1
#define  TXD5  PC_12
#define  RXD5  PD_2
#define  TXD6  PC_6
#define  RXD6  PC_7

#define  SDA1  PB9
#define  SCL1  PB8

#define  SPI2_SCK   PB13
#define  SPI2_MISO  PB14
#define  SPI2_MOSI  PB15

#define  SD_CD  PB1
#define  SD_CS  PB12

#define  SW_IN1   PA15
#define  SW_IN2   PB7
#define  BR_SEL0  PC13
#define  BR_SEL1  PC14
#define  BR_SEL2  PC15
#define  CH_SEL0  PH0
#define  CH_SEL1  PH1
#define  CH_SEL2  PC2

#define  ACTLED  PC3
#define  REDLED  PC4

#define  TIM3_CH3  PC8
#define  TIM8_CH4  PC9
#define  BEEP1  TIM3_CH3
#define  BEEP2  TIM8_CH4

#define  W5500_RST  PA4  // A2
#define  W5500_MOSI PA7  // 11
#define  W5500_MISO PA6  // 12
#define  W5500_SCK  PA5  // 13
#define  W5500_SSEL PB6  // 10

#endif	 /* __BD_SERIAL_COM_SELECTOR_H__ */
