/*
  BOARD 1415A setup.
  Board type : Generic STM32F4 Generic F103ZETx
*/

#ifndef	 __BD1415A_H__
#define	 __BD1415A_H__

#define  INT_SRAM_BASE_ADDRESS      0x60000000
#define  INT_ETHERNET_BASE_ADDRESS  0x64000000
#define  INT_ARCNET_BASE_ADDRESS    0x68000000
#define  EXT_BASE_ADR               0x6C000000
#define  INT_SRAM_SIZE              (524288UL * 1)  // MRAM
#define  W5300_MR_ADDRESS      (volatile uint16_t *)(INT_ETHERNET_BASE_ADDRESS + (0 * 2))
#define  W5300_IDM_AR_ADDRESS  (volatile uint16_t *)(INT_ETHERNET_BASE_ADDRESS + (2 * 2))
#define  W5300_IDM_DR_ADDRESS  (volatile uint16_t *)(INT_ETHERNET_BASE_ADDRESS + (4 * 2))

#undef   LED_BUILTIN
#define  ACTLED       PB9
#define  LED_BUILTIN  ACTLED

#define  USBDM  PA11
#define  USBDP  PA12
#define  USBON  PA8
#define  DFU    PB8
#define  USB_POWER_ON_LEVEL HIGH
#define  USBON_ACTIVE  USB_POWER_ON_LEVEL

#define  BOOT1     PB2
#define  SWDIO     PA13
#define  JTAG_TMS  SWDIO
#define  SWCLK     PA14
#define  JTAG_TCK  SWCLK
#define  JTAG_TDI  PA15
#define  JTAG_TDI  PB3
#define  JTAG_TRST PB4

#define  OSC32_IN  PC14
#define  OSC32_OUT PC15
//#define  OSC_IN    PH0
//#define  OSC_OUT   PH1

#define  USART1_TX   PA9
#define  USART1_RX   PA10
#define  USART2_TX   PA2
#define  USART2_RX   PA3
#define  USART2_CTS  PA0
#define  USART2_RTS  PA1
#define  USART3_TX   PB10
#define  USART3_RX   PB11
#define  USART3_CD   PB12
#define  USART3_CTS  PB13
#define  USART3_RTS  PB14
#define  USART3_DSR  PB15
#define  USART3_DTR  PG8
#define  USART3_RI   PC6
#define  UART4_TX    PC10
#define  UART4_RX    PC11

#define  SPI1_NSS   PA4
#define  SPI1_SCK   PA5
#define  SPI1_MISO  PA6
#define  SPI1_MOSI  PA7
#define  EEPCS2     PB0

#define  SRAM_CS   PD7
#define  MRAM_CS   MRAM_CS
#define  ETH_CS    PG9
#define  ARC_CS    PG10
#define  EXT_CS    PG12
#define  CBUS_CS   EXT_CS
#define  BUS_RD    PD4
#define  BUS_WR    PD5
#define  BLE       PE0
#define  BHE       PE1
#define  WAIT      PD6
#define  ETH_RST   PC2
#define  ARC_RST   PC3
#define  C_RST     PD3
#define  IO_RST    C_RST
#define  BUS_ENB   PF11
#define  CBUS_ENB  BUS_ENB

#define  NMI_TAMPER  PC13
#define  ETH_INT   PC0
#define  ARC_INT   PC1
#define  EXT_INT0  PC4
#define  EXT_INT1  PC5
#define  EXT_INT2  PF6
#define  EXT_INT3  PF7
#define  EXT_INT4  PF8
#define  EXT_INT5  PF9
#define  EXT_INT6  PF10
#define  INT0  EXT_INT0
#define  INT1  EXT_INT1
#define  INT2  EXT_INT2
#define  INT3  EXT_INT3
#define  INT4  EXT_INT4
#define  INT5  EXT_INT5
#define  INT6  EXT_INT6

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
#define  BUS_A21  PE6

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

#define  EXSCL     PB6
#define  EXSDA     PB7
#define  EXI2C_INT PG15
#define  EXI2C_SEM PB5
#define  SCL1      EXSCL
#define  SDA1      EXSDA

#endif	 /* __BD1415A_H__ */
