/*
  BOARD 1401A setup.
  Board type : Generic STM32F4 Generic F407ZETx
*/

#ifndef	 __BD1401A_H__
#define	 __BD1401A_H__

#define  INT_SRAM_BASE_ADDRESS      0x60000000
#define  INT_ETHERNET_BASE_ADDRESS  0x64000000
#define  INT_ARCNET_BASE_ADDRESS    0x68000000
#define  EXT_BASE_ADR               0x6C000000
#define  INT_SRAM_SIZE              (524288UL * 2)  // SRAM
//#define  INT_SRAM_SIZE              (524288UL * 1)  // MRAM
#define  W5300_MR_ADDRESS      (volatile uint16_t *)(INT_ETHERNET_BASE_ADDRESS + (0 * 2))
#define  W5300_IDM_AR_ADDRESS  (volatile uint16_t *)(INT_ETHERNET_BASE_ADDRESS + (2 * 2))
#define  W5300_IDM_DR_ADDRESS  (volatile uint16_t *)(INT_ETHERNET_BASE_ADDRESS + (4 * 2))

#define  BOOT1     PB2
#define  SWDIO     PA13
#define  SWCLK     PA14
#define  OSC32_IN  PC14
#define  OSC32_OUT PC15
#define  OSC_IN    PH0
#define  OSC_OUT   PH1

#define  USBDM  PA11
#define  USBDP  PA12
#define  USBON  PC0
#define  USB_POWER_ON_LEVEL LOW

#define  USART1_TX  PA9
#define  USART1_RX  PA10
#define  USART2_TX  PA2
#define  USART2_RX  PA3
#define  UART4_TX   PC10
#define  UART4_RX   PC11
#define  UART4_MS   PC12

#define  SCL1  PB8
#define  SDA1  PB9

#define  LED_RED    PF8
#define  LED_BLUE   PF9
#define  LED_GREEN  PF10
#undef   LED_BUILTIN
#define  ACTLED     PG11
#define  LED_BUILTIN  ACTLED

#define  BUS_A0   PF0
#define  BUS_A1   PF1
#define  BUS_A2   PF2
#define  BUS_A3   PF3
#define  BUS_A4   PF4
#define  BUS_A5   PF5
#define  BUS_A6   PF12
#define  BUS_A7   PF13
#define  BUS_A8   PF14
#define  BUS_A9   PF15
#define  BUS_A10  PG0
#define  BUS_A11  PG1
#define  BUS_A12  PG2
#define  BUS_A13  PG3
#define  BUS_A14  PG4
#define  BUS_A15  PG5
#define  BUS_A16  PD11
#define  BUS_A17  PD12
#define  BUS_A18  PD13

#define  BUS_D0   PD14
#define  BUS_D1   PD15
#define  BUS_D2   PD0
#define  BUS_D3   PD1
#define  BUS_D4   PE7
#define  BUS_D5   PE8
#define  BUS_D6   PE9
#define  BUS_D7   PE10
#define  BUS_D8   PE11
#define  BUS_D9   PE12
#define  BUS_D10  PE13
#define  BUS_D11  PE14
#define  BUS_D12  PE15
#define  BUS_D13  PD8
#define  BUS_D14  PD9
#define  BUS_D15  PD10

#define  MRAM_CS  PD7
#define  ETH_CS   PG9
#define  ARC_CS   PG10
#define  EXT_CS   PG12
#define  BUS_RD   PD4
#define  BUS_WR   PD5
#define  BLE      PE0
#define  BHE      PE1
#define  IO_RST   PG13

#define  TAMPER    PC13
#define  ETH_INT   PG6
#define  ARC_INT   PG7
#define  EXT_INT0  PG8
#define  EXT_INT1  PC9

#define  GCLK        PA15
#define  GPS_1PPS    PA1
#define  FCLK        PB_0_ALT1  //PB0
#define  TRIGGER1    PD3
#define  TRIGGER2    PD2
#define  CONVERT_MS  PG15
#define  CONVERT     PB_3  //PB3

#define  AR_A0   PA0
#define  AR_A1   PC3
#define  AR_A2   PC2
#define  AR_A3   PC1
#define  AR_D2   PC5
#define  AR_D3   PB7
#define  AR_D4   PC4
#define  AR_D5   PA6
#define  AR_D6   PA7
#define  AR_D7   PB6
#define  AR_D8   PD6
#define  AR_D9   PA4
#define  AR_D10  PA5
#define  AR_D11  PB5
#define  AR_D12  PB4
#define  AR_D13  PG14

#endif	 /* __BD1401A_H__ */
