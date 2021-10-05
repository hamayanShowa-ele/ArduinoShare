/*
  W5500 RAW MODE librarly.

  Copyright (C) 2014 hamayan All Rights Reserved.
*/
#ifndef  __WIZCHIP_RAW_MODE_SPI_H__
#define  __WIZCHIP_RAW_MODE_SPI_H__

#include  <Arduino.h>
#include  <Ethernet2.h>

extern uint16_t hton( uint16_t ab );
extern uint32_t hton( uint32_t abcd );
extern void dump( const uint8_t *data, uint32_t sz );
extern char* int2Hex02( char *dest, uint8_t num );

#define  SPI2_PCLK  (uint32_t)(pClk / 2)

extern "C"
{
  #include  <string.h>
  #include  <limits.h>

  uint32_t crc32Left( int n, uint8_t c[] );  // left shifted.
  uint32_t crc32Right( int n, uint8_t c[]) ;  // right shifted.
}

#define  MAC_BYTE_COUNT   6
#define  IPV4_BYTE_COUNT  4

enum  SOCKET_NUMBERS {SOCKET_0,SOCKET_1,SOCKET_2,SOCKET_3,SOCKET_4,SOCKET_5,SOCKET_6,SOCKET_7,SOCKET_END};

typedef struct ETHERNET2_HEADER
{
  uint8_t dst[MAC_BYTE_COUNT];  // desitination hardware address
  uint8_t src[MAC_BYTE_COUNT];  // source hardware address
  uint8_t typ[2];  // The value will always be 0x0800 or higher.
} t_ether2_header;

typedef struct ETHERNET2_FRAME
{
  t_ether2_header header;
  uint8_t payload[46];  //  uint8_t payload[MIN 46, MAX 1500];
//  uint32_t fcs;  // frame check sequence
} t_ether2_frame;

typedef struct IEEE802_3_HEADER
{
  uint8_t dst[MAC_BYTE_COUNT];  // desitination hardware address
  uint8_t src[MAC_BYTE_COUNT];  // source hardware address
  uint8_t len[2];  // The value will always be 0x0800 or higher.
} t_ieee802_3_header;

typedef struct IEEE802_3_FRAME
{
  t_ieee802_3_header header;
  uint8_t llc[3];  // logical link control
  uint8_t snap[5];  // Subnetwork Access Protocol
  uint8_t payload[38];  //  uint8_t payload[MIN 38, MAX 1492];
//  uint32_t fcs;  // frame check sequence
} t_ieee802_3_frame;

/* ethernet ether2 type field. */
#define  ETHER2_TYPE_TEST01           (uint16_t)0xF000  /* for testing */
#define  ETHER2_TYPE_IPV4             (uint16_t)0x0800
#define  ETHER2_TYPE_ARP              (uint16_t)0x0806
#define  ETHER2_TYPE_RARP             (uint16_t)0x8035
#define  ETHER2_TYPE_WAKE_ON_LAN      (uint16_t)0x8042
#define  ETHER2_TYPE_VMTP             (uint16_t)0x805B
#define  ETHER2_TYPE_APPLE_TALK       (uint16_t)0x809B
#define  ETHER2_TYPE_AARP             (uint16_t)0x80F3
#define  ETHER2_TYPE_IPX              (uint16_t)0x8137
#define  ETHER2_TYPE_SNMP             (uint16_t)0x814C
#define  ETHER2_TYPE_NetBEUI          (uint16_t)0x8191
#define  ETHER2_TYPE_XTP              (uint16_t)0x817D
#define  ETHER2_TYPE_IPV6             (uint16_t)0x86DD
#define  ETHER2_TYPE_PPPoE_DISCOVERY  (uint16_t)0x8863
#define  ETHER2_TYPE_PPPoE_SESSION    (uint16_t)0x8864
#define  ETHER2_TYPE_LOOPBACK         (uint16_t)0x9000

#define  ETHER2_FROM_BUFFALO_HUB  (uint16_t)0x8899  // Loop detection function of BUFFALO hub and more...
/* and any more... */

/* address phase */
#define  MR_ADDRESS         (uint16_t)0x0000
#define  GAR0_ADDRESS       (uint16_t)0x0001
#define  GAR1_ADDRESS       (uint16_t)0x0002
#define  GAR2_ADDRESS       (uint16_t)0x0003
#define  GAR3_ADDRESS       (uint16_t)0x0004
#define  SUBR0_ADDRESS      (uint16_t)0x0005
#define  SUBR1_ADDRESS      (uint16_t)0x0006
#define  SUBR2_ADDRESS      (uint16_t)0x0007
#define  SUBR3_ADDRESS      (uint16_t)0x0008
#define  SHAR0_ADDRESS      (uint16_t)0x0009
#define  SHAR1_ADDRESS      (uint16_t)0x000A
#define  SHAR2_ADDRESS      (uint16_t)0x000B
#define  SHAR3_ADDRESS      (uint16_t)0x000C
#define  SHAR4_ADDRESS      (uint16_t)0x000D
#define  SHAR5_ADDRESS      (uint16_t)0x000E
#define  SIPR0_ADDRESS      (uint16_t)0x000F
#define  SIPR1_ADDRESS      (uint16_t)0x0010
#define  SIPR2_ADDRESS      (uint16_t)0x0011
#define  SIPR3_ADDRESS      (uint16_t)0x0012
#define  INTLEVEL0_ADDRESS  (uint16_t)0x0013
#define  INTLEVEL1_ADDRESS  (uint16_t)0x0014
#define  IR_ADDRESS         (uint16_t)0x0015
#define  IMR_ADDRESS        (uint16_t)0x0016
#define  SIR_ADDRESS        (uint16_t)0x0017
#define  SIMR_ADDRESS       (uint16_t)0x0018
#define  RTR0_ADDRESS       (uint16_t)0x0019
#define  RTR1_ADDRESS       (uint16_t)0x001A
#define  RCR_ADDRESS        (uint16_t)0x001B
#define  PTIMER_ADDRESS     (uint16_t)0x001C
#define  PMAGIC_ADDRESS     (uint16_t)0x001D
#define  PHAR0_ADDRESS      (uint16_t)0x001E
#define  PHAR1_ADDRESS      (uint16_t)0x001F
#define  PHAR2_ADDRESS      (uint16_t)0x0020
#define  PHAR3_ADDRESS      (uint16_t)0x0021
#define  PHAR4_ADDRESS      (uint16_t)0x0022
#define  PHAR5_ADDRESS      (uint16_t)0x0023
#define  PSID0_ADDRESS      (uint16_t)0x0024
#define  PSID1_ADDRESS      (uint16_t)0x0025
#define  PMRU0_ADDRESS      (uint16_t)0x0026
#define  PMRU1_ADDRESS      (uint16_t)0x0027
#define  UIPR0_ADDRESS      (uint16_t)0x0028
#define  UIPR1_ADDRESS      (uint16_t)0x0029
#define  UIPR2_ADDRESS      (uint16_t)0x002A
#define  UIPR3_ADDRESS      (uint16_t)0x002B
#define  UPORT0_ADDRESS     (uint16_t)0x002C
#define  UPORT1_ADDRESS     (uint16_t)0x002D
#define  PHYCFGR_ADDRESS    (uint16_t)0x002E
/* from 0x002F to 0x0038 are reserved.*/
#define  VERSION_ADDRESS    (uint16_t)0x0039

/* control phase */
#define  BLOCK_SELECT_COMMON            (uint8_t)(0 << 3)
#define  BLOCK_SELECT_SOCKET0_REGISTER  (uint8_t)((4 * 0 + 1) << 3)
#define  BLOCK_SELECT_SOCKET0_TX_BUFFER (uint8_t)((4 * 0 + 2) << 3)
#define  BLOCK_SELECT_SOCKET0_RX_BUFFER (uint8_t)((4 * 0 + 3) << 3)
#define  BLOCK_SELECT_SOCKET1_REGISTER  (uint8_t)((4 * 1 + 1) << 3)
#define  BLOCK_SELECT_SOCKET1_TX_BUFFER (uint8_t)((4 * 1 + 2) << 3)
#define  BLOCK_SELECT_SOCKET1_RX_BUFFER (uint8_t)((4 * 1 + 3) << 3)
#define  BLOCK_SELECT_SOCKET2_REGISTER  (uint8_t)((4 * 2 + 1) << 3)
#define  BLOCK_SELECT_SOCKET2_TX_BUFFER (uint8_t)((4 * 2 + 2) << 3)
#define  BLOCK_SELECT_SOCKET2_RX_BUFFER (uint8_t)((4 * 2 + 3) << 3)
#define  BLOCK_SELECT_SOCKET3_REGISTER  (uint8_t)((4 * 3 + 1) << 3)
#define  BLOCK_SELECT_SOCKET3_TX_BUFFER (uint8_t)((4 * 3 + 2) << 3)
#define  BLOCK_SELECT_SOCKET3_RX_BUFFER (uint8_t)((4 * 3 + 3) << 3)
#define  BLOCK_SELECT_SOCKET4_REGISTER  (uint8_t)((4 * 4 + 1) << 3)
#define  BLOCK_SELECT_SOCKET4_TX_BUFFER (uint8_t)((4 * 4 + 2) << 3)
#define  BLOCK_SELECT_SOCKET4_RX_BUFFER (uint8_t)((4 * 4 + 3) << 3)
#define  BLOCK_SELECT_SOCKET5_REGISTER  (uint8_t)((4 * 5 + 1) << 3)
#define  BLOCK_SELECT_SOCKET5_TX_BUFFER (uint8_t)((4 * 5 + 2) << 3)
#define  BLOCK_SELECT_SOCKET5_RX_BUFFER (uint8_t)((4 * 5 + 3) << 3)
#define  BLOCK_SELECT_SOCKET6_REGISTER  (uint8_t)((4 * 6 + 1) << 3)
#define  BLOCK_SELECT_SOCKET6_TX_BUFFER (uint8_t)((4 * 6 + 2) << 3)
#define  BLOCK_SELECT_SOCKET6_RX_BUFFER (uint8_t)((4 * 6 + 3) << 3)
#define  BLOCK_SELECT_SOCKET7_REGISTER  (uint8_t)((4 * 7 + 1) << 3)
#define  BLOCK_SELECT_SOCKET7_TX_BUFFER (uint8_t)((4 * 7 + 2) << 3)
#define  BLOCK_SELECT_SOCKET7_RX_BUFFER (uint8_t)((4 * 7 + 3) << 3)
#define  RWB_READ   (uint8_t)(0 << 2)
#define  RWB_WRITE  (uint8_t)(1 << 2)
#define  RWB_MASK   (uint8_t)(1 << 2)
#define  OM_VDM     (uint8_t)(0 << 0)
#define  OM_FDM_1   (uint8_t)(1 << 0)
#define  OM_FDM_2   (uint8_t)(2 << 0)
#define  OM_FDM_4   (uint8_t)(3 << 0)
#define  OM_MASK    (uint8_t)(3 << 0)

/* common register bit define. */
/* MR register bit define. */
#define  COMMON_MR_RST_bit   (uint8_t)(1 << 7)
#define  COMMON_MR_WOL_bit   (uint8_t)(1 << 5)
#define  COMMON_MR_PB_bit    (uint8_t)(1 << 4)
#define  COMMON_MR_PPPoE_bit (uint8_t)(1 << 3)
#define  COMMON_MR_FARP_bit  (uint8_t)(1 << 1)
/* IR register bit define. */
#define  COMMON_IR_CONFLICT_bit (uint8_t)(1 << 7)
#define  COMMON_IR_UNREACH_bit  (uint8_t)(1 << 6)
#define  COMMON_IR_PPPoE_bit    (uint8_t)(1 << 5)
#define  COMMON_IR_MP_bit       (uint8_t)(1 << 4)
/* IMR register bit define. */
#define  COMMON_IMR_CONFLICT_bit (uint8_t)(1 << 7)
#define  COMMON_IMR_UNREACH_bit  (uint8_t)(1 << 6)
#define  COMMON_IMR_PPPoE_bit    (uint8_t)(1 << 5)
#define  COMMON_IMR_MP_bit       (uint8_t)(1 << 4)
/* SIR register bit define. */
#define  COMMON_SIR_SOCKET7_bit (uint8_t)(1 << 7)
#define  COMMON_SIR_SOCKET6_bit (uint8_t)(1 << 6)
#define  COMMON_SIR_SOCKET5_bit (uint8_t)(1 << 5)
#define  COMMON_SIR_SOCKET4_bit (uint8_t)(1 << 4)
#define  COMMON_SIR_SOCKET3_bit (uint8_t)(1 << 3)
#define  COMMON_SIR_SOCKET2_bit (uint8_t)(1 << 2)
#define  COMMON_SIR_SOCKET1_bit (uint8_t)(1 << 1)
#define  COMMON_SIR_SOCKET0_bit (uint8_t)(1 << 0)
/* SIMR register bit define. */
#define  COMMON_SIMR_SOCKET7_bit (uint8_t)(1 << 7)
#define  COMMON_SIMR_SOCKET6_bit (uint8_t)(1 << 6)
#define  COMMON_SIMR_SOCKET5_bit (uint8_t)(1 << 5)
#define  COMMON_SIMR_SOCKET4_bit (uint8_t)(1 << 4)
#define  COMMON_SIMR_SOCKET3_bit (uint8_t)(1 << 3)
#define  COMMON_SIMR_SOCKET2_bit (uint8_t)(1 << 2)
#define  COMMON_SIMR_SOCKET1_bit (uint8_t)(1 << 1)
#define  COMMON_SIMR_SOCKET0_bit (uint8_t)(1 << 0)
/* PHYCFGR register bit define. */
#define  COMMON_PHYCFGR_RST_bit   (uint8_t)(1 << 7)
#define  COMMON_PHYCFGR_OPMD_bit  (uint8_t)(1 << 6)
#define  COMMON_PHYCFGR_OPMDC_bit (uint8_t)(7 << 3)
#define  COMMON_PHYCFGR_DPX_bit   (uint8_t)(1 << 2)
#define  COMMON_PHYCFGR_SPD_bit   (uint8_t)(1 << 1)
#define  COMMON_PHYCFGR_LNK_bit   (uint8_t)(1 << 0)

/* socket registers */
#define  Sn_MR          (uint16_t)0x0000
#define  Sn_CR          (uint16_t)0x0001
#define  Sn_IR          (uint16_t)0x0002
#define  Sn_SR          (uint16_t)0x0003
#define  Sn_PORT0       (uint16_t)0x0004
#define  Sn_PORT1       (uint16_t)0x0005
#define  Sn_DHAR0       (uint16_t)0x0006
#define  Sn_DHAR1       (uint16_t)0x0007
#define  Sn_DHAR2       (uint16_t)0x0008
#define  Sn_DHAR3       (uint16_t)0x0009
#define  Sn_DHAR4       (uint16_t)0x000A
#define  Sn_DHAR5       (uint16_t)0x000B
#define  Sn_DIPR0       (uint16_t)0x000C
#define  Sn_DIPR1       (uint16_t)0x000D
#define  Sn_DIPR2       (uint16_t)0x000E
#define  Sn_DIPF3       (uint16_t)0x000F
#define  Sn_DPORT0      (uint16_t)0x0010
#define  Sn_DPORT1      (uint16_t)0x0011
#define  Sn_MSSR0       (uint16_t)0x0012
#define  Sn_MSSR1       (uint16_t)0x0013
/* 0x0014 reserved */
#define  Sn_TOS         (uint16_t)0x0015
#define  Sn_TTL         (uint16_t)0x0016
/* from 0x0017 to 0x001D reserved. */
#define  Sn_RXBUF_SIZE  (uint16_t)0x001E
#define  Sn_TXBUF_SIZE  (uint16_t)0x001F
#define  Sn_TX_FSR0     (uint16_t)0x0020
#define  Sn_TX_FSR1     (uint16_t)0x0021
#define  Sn_TX_RD0      (uint16_t)0x0022
#define  Sn_TX_RD1      (uint16_t)0x0023
#define  Sn_TX_WR0      (uint16_t)0x0024
#define  Sn_TX_WR1      (uint16_t)0x0025
#define  Sn_RX_RSR0     (uint16_t)0x0026
#define  Sn_RX_RSR1     (uint16_t)0x0027
#define  Sn_RX_RD0      (uint16_t)0x0028
#define  Sn_RX_RD1      (uint16_t)0x0029
#define  Sn_RX_WR0      (uint16_t)0x002A
#define  Sn_RX_WR1      (uint16_t)0x002B
#define  Sn_IMR         (uint16_t)0x002C
#define  Sn_FRAG0       (uint16_t)0x002D
#define  Sn_FRAG1       (uint16_t)0x002E
#define  Sn_KPALVTR     (uint16_t)0x002F
/* from 0x0030 to 0xFFFF reserved. */

/* socket registers */
/* MR register in socket registers */
#define  Sn_MR_MULTI_MFEN_bit      (uint8_t)(1 << 7)
#define  Sn_MR_BCASTB_bit          (uint8_t)(1 << 6)
#define  Sn_MR_ND_MC_MMB_bit       (uint8_t)(1 << 5)
#define  Sn_MR_UCASTB_MIP6B_bit    (uint8_t)(1 << 4)
#define  Sn_MR_PROTOCOL_CLOSED_bit (uint8_t)(0 << 0)
#define  Sn_MR_PROTOCOL_TCP_bit    (uint8_t)(1 << 0)
#define  Sn_MR_PROTOCOL_UDP_bit    (uint8_t)(2 << 0)
#define  Sn_MR_PROTOCOL_MACRAW_bit (uint8_t)(4 << 0)
#define  Sn_MR_PROTOCOL_MASK_bit   (uint8_t)(0xF << 0)
/* CR register in socket registers */
#define  Sn_CR_OPEN      (uint8_t)(0x01)
#define  Sn_CR_LISTEN    (uint8_t)(0x02)
#define  Sn_CR_CONNECT   (uint8_t)(0x04)
#define  Sn_CR_DISCON    (uint8_t)(0x08)
#define  Sn_CR_CLOSE     (uint8_t)(0x10)
#define  Sn_CR_SEND      (uint8_t)(0x20)
#define  Sn_CR_SEND_MAC  (uint8_t)(0x21)
#define  Sn_CR_SEND_KEEP (uint8_t)(0x22)
#define  Sn_CR_RECV      (uint8_t)(0x40)
#define  Sn_CR_MASK      (uint8_t)(0xFF)
/* IR register in socket registers */
#define  Sn_IR_SEND_OK_bit (uint8_t)(1 << 4)
#define  Sn_IR_TIMEOUT_bit (uint8_t)(1 << 3)
#define  Sn_IR_RECV_bit    (uint8_t)(1 << 2)
#define  Sn_IR_DISCON_bit  (uint8_t)(1 << 1)
#define  Sn_IR_CON_bit     (uint8_t)(1 << 0)
#define  Sn_IR_MASK_bit    (uint8_t)(0x1F << 0)
/* SR register in socket registers */
#define  Sn_SR_SOCK_CLOSE       (uint8_t)(0x00)
#define  Sn_SR_SOCK_INIT        (uint8_t)(0x13)
#define  Sn_SR_SOCK_LISTEN      (uint8_t)(0x14)
#define  Sn_SR_SOCK_ESTABLISHED (uint8_t)(0x17)
#define  Sn_SR_SOCK_CLOSE_WAIT  (uint8_t)(0x1C)
#define  Sn_SR_SOCK_UDP         (uint8_t)(0x22)
#define  Sn_SR_SOCK_MACRAW      (uint8_t)(0x42)
#define  Sn_SR_SOCK_SYNSENT     (uint8_t)(0x15)
#define  Sn_SR_SOCK_SYNRECV     (uint8_t)(0x16)
#define  Sn_SR_SOCK_FIN_WAIT    (uint8_t)(0x18)
#define  Sn_SR_SOCK_CLOSING     (uint8_t)(0x1A)
#define  Sn_SR_SOCK_TIME_WAIT   (uint8_t)(0x1B)
#define  Sn_SR_SOCK_LAST_ACK    (uint8_t)(0x1D)
/* IMR register in socket registers */
#define  Sn_IMR_SENDOK_bit  (uint8_t)(1 << 4)
#define  Sn_IMR_TIMOUT_bit  (uint8_t)(1 << 3)
#define  Sn_IMR_RECV_bit    (uint8_t)(1 << 2)
#define  Sn_IMR_DISCON_bit  (uint8_t)(1 << 1)
#define  Sn_IMR_CON_bit     (uint8_t)(1 << 0)
#define  Sn_IMR_MASK_bit    (uint8_t)(0x1F << 0)

/*
 * function macros.
 */
/* common registers. */
#define  setMR(mr)  writeCom(BLOCK_SELECT_COMMON,(uint8_t)mr)
#define  getMR()    readCom(BLOCK_SELECT_COMMON)

#define  setGAR(gw)  writeCom(GAR0_ADDRESS,(const uint8_t *)gw,IPV4_BYTE_COUNT)
#define  getGAR(gw)  readCom(GAR0_ADDRESS,(uint8_t *)gw,IPV4_BYTE_COUNT)

#define  setSUBR(mask)  writeCom(SUBR0_ADDRESS,(const uint8_t *)mask,IPV4_BYTE_COUNT)
#define  getSUBR(mask)  readCom(SUBR0_ADDRESS,(uint8_t *)mask,IPV4_BYTE_COUNT)

#define  setSHAR(mac)  writeCom(SHAR0_ADDRESS,(const uint8_t *)mac,MAC_BYTE_COUNT)
#define  getSHAR(mac)  readCom(SHAR0_ADDRESS,(uint8_t *)mac,MAC_BYTE_COUNT)

#define  setSIPR(ip)  writeCom(SIPR0_ADDRESS,(const uint8_t *)ip,IPV4_BYTE_COUNT)
#define  getSIPR(ip)  readCom(SIPR0_ADDRESS,(uint8_t *)ip,IPV4_BYTE_COUNT)

#define  setINTLEVEL(lv)  writeCom(INTLEVEL0_ADDRESS,(uint16_t)lv)
#define  getINTLEVEL()    readCom(INTLEVEL0_ADDRESS,(uint16_t *)nullptr)

#define  setIR(ir) writeCom(IR_ADDRESS,(uint8_t)ir)
#define  getIR()   readCom(IR_ADDRESS)

#define  setIMR(imr) writeCom(IMR_ADDRESS,(uint8_t)imr)
#define  getIMR()    readCom(IMR_ADDRESS)

#define  setSIR(sir) writeCom(SIR_ADDRESS,(uint8_t)sir)
#define  getSIR()    readCom(SIR_ADDRESS)

#define  setSIMR(simr) writeCom(SIMR_ADDRESS,(uint8_t)simr)
#define  getSIMR()     readCom(SIMR_ADDRESS)

#define  setRTR(rtr)  writeCom(RTR0_ADDRESS,(uint16_t)rtr)
#define  getRTR()     readCom(RTR0_ADDRESS,(uint16_t *)nullptr)

#define  setRCR(rcr) writeCom(RCR_ADDRESS,(uint8_t)rcr)
#define  getRCR()    readCom(RCR_ADDRESS)

#define  setPTIMER(tim)  writeCom(PTIMER_ADDRESS,(uint8_t)tim)
#define  getPTIMER()     readCom(PTIMER_ADDRESS)

#define  setPMAGIC(mag)  writeCom(PMAGIC_ADDRESS,(uint8_t)mag)
#define  getPMAGIC()     readCom(PMAGIC_ADDRESS)

#define  setPHAR(mac)  writeCom(PHAR0_ADDRESS,(const uint8_t *)mac,MAC_BYTE_COUNT)
#define  getPHAR(mac)  readCom(PHAR0_ADDRESS,(uint8_t *)mac,MAC_BYTE_COUNT)

#define  setPSID(id)  writeCom(PSID0_ADDRESS,(uint16_t)id)
#define  getPSID()    readCom(PSID0_ADDRESS,(uint16_t *)nullptr)

#define  setPMRU(mru)  writeCom(PMRU0_ADDRESS,(uint16_t)mru)
#define  getPMRU()     readCom(PMRU0_ADDRESS,(uint16_t *)nullptr)

#define  getUIPR(uip)  readCom(UIPR0_ADDRESS,uip,IPV4_BYTE_COUNT)
#define  getUPORTR()  readCom(UPORT0_ADDRESS,(uint16_t *)nullptr)

#define  setPHYCFGR(cfg)  writeCom(PHYCFGR_ADDRESS,(uint8_t)cfg)
#define  getPHYCFGR()     readCom(PHYCFGR_ADDRESS)

#define  getVERSIONR()  readCom(VERSION_ADDRESS)


/* socket registers. */
#define  setSnMR(sn,md)     writeSnReg(sn,Sn_MR,(uint8_t)md)
#define  getSnMR(sn)        readSnReg(sn,Sn_MR)

#define  setSnCR(sn,cmd)  writeSnReg(sn,Sn_CR,(uint8_t)cmd)
#define  getSnCR(sn)      readSnReg(sn,Sn_CR)

#define  SnOpen(sn)       setSnCR(sn,Sn_CR_OPEN)
#define  SnListen(sn)     setSnCR(sn,Sn_CR_LISTEN)
#define  SnConnect(sn)    setSnCR(sn,Sn_CR_CONNECT)
#define  SnDiscon(sn)     setSnCR(sn,Sn_CR_DISCON)
#define  SnClose(sn)      setSnCR(sn,Sn_CR_CLOSE)
#define  SnSend(sn)       setSnCR(sn,Sn_CR_SEND)
#define  SnSend_mac(sn)   setSnCR(sn,Sn_CR_SEND_MAC)
#define  SnSend_keep(sn)  setSnCR(sn,Sn_CR_SEND_KEEP)
#define  SnRecv(sn)       setSnCR(sn,Sn_CR_RECV)

#define  setSnIR(sn,ir)  writeSnReg(sn,Sn_IR,(uint8_t)ir)
#define  getSnIR(sn)     readSnReg(sn,Sn_IR)

#define  getSnSR(sn)  readSnReg(sn,Sn_SR)

#define  setSnSPORT(sn,sp)  writeSnReg(sn,Sn_PORT0,(uint16_t)sp)
#define  getSnSPORT(sn)     readSnReg(sn,Sn_PORT0,(uint16_t *)nullptr)

#define  setSnDHAR(sn,dha)  writeSnReg(sn,Sn_DHAR0,(const uint8_t *)dha,MAC_BYTE_COUNT)
#define  getSnDHAR(sn,dha)  readSnReg(sn,Sn_DHAR0,(uint8_t *)dha,MAC_BYTE_COUNT)

#define  setSnDIPR(sn,dip)  writeSnReg(sn,Sn_DIPR0,(const uint8_t *)dip,IPV4_BYTE_COUNT)
#define  getSnDIPR(sn,dip)  readSnReg(sn,Sn_DIPR0,(uint8_t *)dip,IPV4_BYTE_COUNT)

#define  setSnDPORT(sn,dp)  writeSnReg(sn,Sn_DPORT0,(uint16_t)dp)
#define  getSnDPORT(sn)     readSnReg(sn,Sn_DPORT0,(uint16_t *)nullptr)

#define  setSnMSSR(sn,mss)  writeSnReg(sn,Sn_MSSR0,(uint16_t)mss)
#define  getSnMSS(sn)       readSnReg(sn,Sn_MSSR0)

#define  setSnTOS(sn,tos)  writeSnReg(sn,Sn_TOS,(uint8_t)tos)
#define  getSnTOS(sn)      readSnReg(sn,Sn_TOS)

#define  setSnTTL(sn,ttl)  writeSnReg(sn,Sn_TTL,(uint8_t)ttl)
#define  getSnTTL(sn)      readSnReg(sn,Sn_TTL)

#define  setSnRxBufSize(sn,sz)  writeSnReg(sn,Sn_RXBUF_SIZE,(uint8_t)sz)
#define  getSnRxBufSize(sn)     readSnReg(sn,Sn_RXBUF_SIZE)

#define  setSnTxBufSize(sn,sz)  writeSnReg(sn,Sn_TXBUF_SIZE,(uint8_t)sz)
#define  getSnTxBufSize(sn)     readSnReg(sn,Sn_TXBUF_SIZE)

#define  getSnTxFSR(sn)  readSnReg(sn,Sn_TX_FSR0,(uint16_t *)nullptr)

#define  setSnTxRD(sn,p)  writeSnReg(sn,Sn_TX_RD0,(uint16_t)p)
#define  getSnTxRD(sn)    readSnReg(sn,Sn_TX_RD0,(uint16_t *)nullptr)

#define  setSnTxWR(sn,p)  writeSnReg(sn,Sn_TX_WR0,(uint16_t)p)
#define  getSnTxWR(sn)    readSnReg(sn,Sn_TX_WR0,(uint16_t *)nullptr)

#define  getSnRxRSR(sn)  readSnReg(sn,Sn_RX_RSR0,(uint16_t *)nullptr)

#define  setSnRxRD(sn,p)  writeSnReg(sn,Sn_RX_RD0,(uint16_t)p)
#define  getSnRxRD(sn)    readSnReg(sn,Sn_RX_RD0,(uint16_t *)nullptr)

#define  setSnRxWR(sn,p)  writeSnReg(sn,Sn_RX_WR0,(uint16_t)p)
#define  getSnRxWR(sn)    readSnReg(sn,Sn_RX_WR0,(uint16_t *)nullptr)

#define  setSnIMR(sn,imr)  writeSnReg(sn,Sn_IMR,(uint8_t)imr)
#define  getSnIMR(sn)      readSnReg(sn,Sn_IMR)

#define  setSnFRAG(sn,frg)  writeSnReg(sn,Sn_FRAG0,(uint16_t)frg)
#define  getSnFRAG(sn)      readSnReg(sn,Sn_FRAG0)

#define  setSnKPALVTR(sn,tim) writeSnReg(sn,Sn_KPALVTR,(uint8_t)tim)
#define  getSnKPALVTR(sn)     readSnReg(sn,Sn_KPALVTR)

class WIZCHIP_RAW_MODE // : public W5500Class
{
private :
  SPIClass *spi;
  uint8_t csPin;
  uint32_t pClk;

  void setCS() { digitalWrite( csPin, LOW ); }
  void resetCS() { digitalWrite( csPin, HIGH ); }
  void initCS() { pinMode( csPin, OUTPUT ); resetCS(); }
  int softReset( uint32_t tmout = 1000UL );
  /*
   * get messages
   */
  const char* get_opmdc_message( int num );
  const char* get_dpx_message( int num );
  const char* get_spd_message( int num );
  const char* get_lnk_message( int num );

  /*
   * write register.
   */
  void writeReg( uint16_t addr, uint8_t ctrl, uint8_t data );
  void writeReg( uint16_t addr, uint8_t ctrl, uint16_t data );
  void writeReg( uint16_t addr, uint8_t ctrl, uint32_t data );
  void writeReg( uint16_t addr, uint8_t ctrl, const uint8_t *data, int sz );

  /*
   * reead register.
   */
  uint8_t readReg( uint16_t addr, uint8_t ctrl );
  uint16_t readReg( uint16_t addr, uint8_t ctrl, uint16_t *data );
  uint32_t readReg( uint16_t addr, uint8_t ctrl, uint32_t *data );
  void readReg( uint16_t addr, uint8_t ctrl, uint8_t *data, int sz );

public :
  WIZCHIP_RAW_MODE( SPIClass *_spi, uint8_t _pin ) { spi = _spi; csPin = _pin; pClk = 42000000UL; }
  void setPclk( uint32_t _clk ) { pClk = _clk; }

  int initW5500( const uint8_t sz[2][MAX_SOCK_NUM] = nullptr, uint32_t tmout = 5000UL );
  bool linked() { return ( readCom( PHYCFGR_ADDRESS ) & COMMON_PHYCFGR_LNK_bit ) ? true : false; }
  String linkMessage();
  String socketStatusMessage( uint8_t val );
  uint16_t snRxAvailable( uint8_t sn );
  uint16_t writeEther( uint8_t sn, const uint8_t *dst, uint16_t typ, const uint8_t *payload, int sz );
  uint16_t readEther( uint8_t sn, uint8_t *dst, uint16_t *typ, uint8_t *payload, int sz );

  /*
   * write common register.
   */
  void writeCom( uint16_t addr, uint8_t data ) { writeReg( addr, BLOCK_SELECT_COMMON, data ); }
  void writeCom( uint16_t addr, uint16_t data ) { writeReg( addr, BLOCK_SELECT_COMMON, data ); }
  void writeCom( uint16_t addr, uint32_t data ) { writeReg( addr, BLOCK_SELECT_COMMON, data ); }
  void writeCom( uint16_t addr, const uint8_t *data, int sz ) { writeReg( addr, BLOCK_SELECT_COMMON, data, sz ); }

  /*
   * read common register.
   */
  uint8_t readCom( uint16_t addr ) { return readReg( addr, BLOCK_SELECT_COMMON ); }
  uint16_t readCom( uint16_t addr, uint16_t *data ) { return readReg( addr, BLOCK_SELECT_COMMON, data ); }
  uint32_t readCom( uint16_t addr, uint32_t *data ) { return readReg( addr, BLOCK_SELECT_COMMON, data ); }
  void readCom( uint16_t addr, uint8_t *data, int sz ) { readReg( addr, BLOCK_SELECT_COMMON, data, sz ); }

  /*
   * write socket n register.
   */
  void writeSnReg( uint8_t sn, uint16_t addr, uint8_t data ) { writeReg( addr, (((4 * sn) + 1) << 3), data ); }
  void writeSnReg( uint8_t sn, uint16_t addr, uint16_t data ) { writeReg( addr, (((4 * sn) + 1) << 3), data ); }
  void writeSnReg( uint8_t sn, uint16_t addr, uint32_t data ) { writeReg( addr, (((4 * sn) + 1) << 3), data ); }
  void writeSnReg( uint8_t sn, uint16_t addr, const uint8_t *data, int sz ) { writeReg( addr, (((4 * sn) + 1) << 3), data, sz ); }

  /*
   * read socket n register.
   */
  uint8_t readSnReg( uint8_t sn, uint16_t addr ) { return readReg( addr, (((4 * sn) + 1) << 3) ); }
  uint16_t readSnReg( uint8_t sn, uint16_t addr, uint16_t *data ) { return readReg( addr, (((4 * sn) + 1) << 3), data ); }
  uint16_t readSnReg( uint8_t sn, uint16_t addr, uint32_t *data ) { return readReg( addr, (((4 * sn) + 1) << 3), data ); }
  void readSnReg( uint8_t sn, uint16_t addr, uint8_t *data, int sz ) { readReg( addr, (((4 * sn) + 1) << 3), data, sz ); }

  /*
   * read / write socket n tx buffer / rx buffer.
   */
  void writeSnTxData( uint8_t sn, const uint8_t *data, int sz )
  {
    uint16_t wr = getSnTxWR( sn );
    writeReg( wr, (((4 * sn) + 2) << 3), data, sz );
    wr += sz;
    setSnTxWR( sn, wr );
  }
  void readSnTxData( uint8_t sn, uint16_t addr, uint8_t *data, int sz )
  {
    readReg( addr, (((4 * sn) + 2) << 3), data, sz );
  }

  void writeSnRxData( uint8_t sn, uint16_t addr, const uint8_t *data, int sz )
  {
    writeReg( addr, (((4 * sn) + 3) << 3), data, sz );
  }
  void readSnRxData( uint8_t sn, uint8_t *data, int sz )
  {
    uint16_t rd = getSnRxRD( sn );
    readReg( rd, (((4 * sn) + 3) << 3), data, sz );
    rd += (uint16_t)sz;
    setSnRxRD( sn, rd );
    SnRecv( sn );
  }
};

#endif  /*  __WIZCHIP_RAW_MODE_SPI_H__ */

/*
  Copyright (C) 2014 hamayan All Rights Reserved.
*/
