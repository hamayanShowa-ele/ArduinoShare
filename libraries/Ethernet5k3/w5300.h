/*
* Copyright (c) 2010 by WIZnet <support@wiznet.co.kr>
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 *
 * - 10 Apr. 2015
 * Added support for Arduino Ethernet Shield 2
 * by Arduino.org team
 */

#ifndef	W5300_H_INCLUDED
#define	W5300_H_INCLUDED

#include  <Arduino.h>
//#include  <MY_STREAM.h>
extern "C"
{
  #include  <stdio.h>
  #include  <string.h>
}

enum SOCKET_NUMBERS
{
  SOCKET_0, SOCKET_1, SOCKET_2, SOCKET_3,
  SOCKET_4, SOCKET_5, SOCKET_6, SOCKET_7,
  SOCKET_END
};
#define  MAX_SOCK_NUM  SOCKET_END

typedef struct 
{
  uint8_t dst[6];
  uint8_t src[6];
  uint8_t type[2];
  uint8_t data[64];
} ETHER2_FRAME;

typedef struct 
{
  uint8_t  dip[4];
  uint16_t dport;
  uint16_t size;
} UDP_RECIEVE_HEADER;

#define  IPV4_SIZE  4

// common registers address
#define  W5300_IR         0x0002
#define  W5300_IMR        0x0004
#define  W5300_SHAR       0x0008
#define  W5300_SHAR2      0x000A
#define  W5300_SHAR4      0x000C
#define  W5300_GAR        0x0010
#define  W5300_GAR2       0x0012
#define  W5300_SUBR       0x0014
#define  W5300_SUBR2      0x0016
#define  W5300_SIPR       0x0018
#define  W5300_SIPR2      0x001A
#define  W5300_RTR        0x001C
#define  W5300_RCR        0x001E
#define  W5300_TMS01R     0x0020
#define  W5300_TMS23R     0x0022
#define  W5300_TMS45R     0x0024
#define  W5300_TMS67R     0x0026
#define  W5300_RMS01R     0x0028
#define  W5300_RMS23R     0x002A
#define  W5300_RMS45R     0x002C
#define  W5300_RMS67R     0x002E
#define  W5300_MTYPER     0x0030
#define  W5300_PATR       0x0032
#define  W5300_PTIMER     0x0036
#define  W5300_PMAGICR    0x0038
#define  W5300_PSIDR      0x003C
#define  W5300_PDHAR      0x0040
#define  W5300_PDHAR2     0x0042
#define  W5300_PDHAR4     0x0044
#define  W5300_UIPR       0x0048
#define  W5300_UIPR2      0x004A
#define  W5300_UPORTR     0x004C
#define  W5300_FMTUR      0x004E
#define  W5300_P0_BRDYR   0x0060
#define  W5300_P0_BDPTHR  0x0062
#define  W5300_P1_BRDYR   0x0064
#define  W5300_P1_BDPTHR  0x0066
#define  W5300_P2_BRDYR   0x0068
#define  W5300_P2_BDPTHR  0x006A
#define  W5300_P3_BRDYR   0x006C
#define  W5300_P3_BDPTHR  0x006E
#define  W5300_IDR        0x00FE
#define  W5300_VERSIONR   W5300_IDR

//----------------------------- W5300 SOCKET Registers -----------------------------
// socket registers address
#define  _W5300_IO_BASE_    0x0000
#define  _WIZCHIP_SN_BASE_  (0x0200)
#define  _WIZCHIP_SN_SIZE_  (0x0040)
#define  WIZCHIP_SREG_BLOCK(N)  (_WIZCHIP_SN_BASE_+ (_WIZCHIP_SN_SIZE_ * N))  //< Socket N register block

/**
 * @ingroup Socket_register_group_W5300
 * @brief Socket Mode register(R/W)
 * @details @ref Sn_MR configures the option or protocol type of Socket n.\n\n
 * Each bit of @ref Sn_MR defined as the following.
 * <table>
 * 		<tr>  <td>15</td> <td>14</td> <td>13</td> <td>12</td> <td>11</td> <td>10</td> <td>9</td> <td>8</td>   </tr>
 * 		<tr>  <td colspan=7> Reserved. Read as 0 </td> <td>ALIGN</td> </tr>
 * 		<tr>  <td>7</td> <td>6</td> <td>5</td> <td>4</td> <td>3</td> <td>2</td> <td>1</td> <td>0</td>   </tr>
 * 		<tr>  <td>MULTI</td> <td>MF</td> <td>ND/IGMPv</td> <td>Reserved</td> <td colspan=4>PROTOCOL[3:0]</td> </tr>
 * </table>
 * - @ref Sn_MR_ALIGN   : Alignment bit of Sn_MR, Only valid in \ref Sn_MR_TCP. (C0 : Include TCP PACK_INFO, 1 : Not include TCP PACK_INFO)
 * - @ref Sn_MR_MULTI	: Support UDP Multicasting
 * - @ref Sn_MR_MF	   : Enable MAC Filter (0 : Disable, 1 - Enable), When enabled, W5300 can receive only both own and broadcast packet.
 * - @ref Sn_MR_ND		: No Delayed Ack(TCP) flag
 * - @ref Sn_MR_IGMPv 	: IGMP version used <b>in UDP mulitcasting</b>. (0 : Version 2, 1 : Version 2)
 * - <b>PROTOCOL[3:0]</b>
 * <table>
 * 		<tr>   <td><b>Protocol[3]</b></td> <td><b>Protocol[2]</b></td> <td><b>Protocol[1]</b></td> <td><b>Protocol[0]</b></td> <td>@b Meaning</td>   </tr>
 * 		<tr>   <td>0</td> <td>0</td> <td>0</td> <td>0</td> <td>Closed</td>   </tr>
 * 		<tr>   <td>0</td> <td>0</td> <td>0</td> <td>1</td> <td>TCP</td>   </tr>
 * 		<tr>   <td>0</td> <td>0</td> <td>1</td> <td>0</td> <td>UDP</td>   </tr>
 * 		<tr>   <td>0</td> <td>0</td> <td>1</td> <td>1</td> <td>IPCRAW</td>   </tr>
 *       <tr>   <td>0</td> <td>1</td> <td>0</td> <td>0</td> <td>MACRAW</td>   </tr>
 *       <tr>   <td>0</td> <td>1</td> <td>0</td> <td>1</td> <td>PPPoE</td>   </tr>
 * </table>
 *
 *  - @ref Sn_MR_PPPoE  : PPPoE 
 *	 - @ref Sn_MR_MACRAW	: MAC LAYER RAW SOCK
 *  - @ref Sn_MR_IPRAW  : IP LAYER RAW SOCK 
 *  - @ref Sn_MR_UDP		: UDP
 *  - @ref Sn_MR_TCP		: TCP
 *  - @ref Sn_MR_CLOSE	: Unused socket
 *  @note MACRAW mode should be only used in Socket 0.
 */
#define  Sn_MR(n)  (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x00)

/**
 * @ingroup Socket_register_group_W5300
 * @brief Socket command register(R/W)
 * @details This is used to set the command for Socket n such as OPEN, CLOSE, CONNECT, LISTEN, SEND, and RECEIVE.\n
 * After W5500 accepts the command, the @ref Sn_CR register is automatically cleared to 0x00.
 * Even though @ref Sn_CR is cleared to 0x00, the command is still being processed.\n
 * To check whether the command is completed or not, please check the @ref Sn_IR or @ref Sn_SR.
 * - @ref Sn_CR_OPEN 		: Initialize or open socket.
 * - @ref Sn_CR_LISTEN 		: Wait connection request in TCP mode(<b>Server mode</b>)
 * - @ref Sn_CR_CONNECT 	: Send connection request in TCP mode(<b>Client mode</b>)
 * - @ref Sn_CR_DISCON 		: Send closing request in TCP mode.
 * - @ref Sn_CR_CLOSE   	: Close socket.
 * - @ref Sn_CR_SEND    	: Update TX buffer pointer and send data.
 * - @ref Sn_CR_SEND_MAC	: Send data with MAC address, so without ARP process.
 * - @ref Sn_CR_SEND_KEEP 	: Send keep alive message.
 * - @ref Sn_CR_RECV		   : Update RX buffer pointer and receive data.
 * - @ref Sn_CR_PCON       : PPPoE connection begins by transmitting PPPoE discovery packet.
 * - @ref Sn_CR_PDISCON    : Closes PPPoE connection.
 * - @ref Sn_CR_PCR        : In each phase, it transmits REQ message.
 * - @ref Sn_CR_PCN        : In each phase, it transmits NAK message.
 * - @ref Sn_CR_PCJ        : In each phase, it transmits REJECT message.
 */
#define  Sn_CR(n)  (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x02)

/**
 * @ingroup Socket_register_group_W5300
 * @brief socket interrupt mask register(R)
 * @details @ref Sn_IMR masks the interrupt of Socket n.
 * Each bit corresponds to each bit of @ref Sn_IR. When a Socket n Interrupt is occurred and the corresponding bit of @ref Sn_IMR is 
 * the corresponding bit of @ref Sn_IR becomes  When both the corresponding bit of @ref Sn_IMR and @ref Sn_IR are and the n-th bit of @ref IR is 
 * Host is interrupted by asserted INTn PIN to low.
 */
#define  Sn_IMR(n)  (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x04)

/**
 * @ingroup Socket_register_group_W5300
 * @brief Socket interrupt register(R)
 * @details @ref Sn_IR indicates the status of Socket Interrupt such as establishment, termination, receiving data, timeout).\n
 * When an interrupt occurs and the corresponding bit of @ref Sn_IMR is  the corresponding bit of @ref Sn_IR becomes \n
 * In order to clear the @ref Sn_IR bit, the host should write the bit to \n
 * <table>
 * 		<tr>  <td>15</td> <td>14</td> <td>13</td> <td>12</td> <td>11</td> <td>10</td> <td>9</td> <td>8</td>   </tr>
 * 		<tr>  <td colspan=8> Reserved. Read as 0</td> </tr>
 * 		<tr>  <td>7</td> <td>6</td> <td>5</td> <td>4</td> <td>3</td> <td>2</td> <td>1</td> <td>0</td>   </tr>
 * 		<tr>  <td>PRECV</td> <td>PFAIL</td> <td>PNEXT</td> <td>SENDOK</td> <td>TIMEOUT</td> <td>RECV</td> <td>DISCON</td> <td>CON</td> </tr>
 * </table>
 * - \ref Sn_IR_PRECV   : PPP receive
 * - \ref Sn_IR_PFAIL   : PPP fail
 * - \ref Sn_IR_PNEXT   : PPP next phase
 * - \ref Sn_IR_SENDOK  : SENDOK 
 * - \ref Sn_IR_TIMEOUT : TIMEOUT
 * - \ref Sn_IR_RECV    : RECV 
 * - \ref Sn_IR_DISCON  : DISCON
 * - \ref Sn_IR_CON     : CON
 */
#define  Sn_IR(n)  (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x06)

/**
 * @ingroup Socket_register_group_W5300
 * @brief Socket status register(R)
 * @details @ref Sn_SSR indicates the status of Socket n.\n
 * The status of Socket n is changed by @ref Sn_CR or some special control packet as SYN, FIN packet in TCP.
 * @par Normal status
 * - @ref SOCK_CLOSED 		: Closed
 * - @ref SOCK_INIT   		: Initiate state
 * - @ref SOCK_LISTEN    	: Listen state
 * - @ref SOCK_ESTABLISHED : Success to connect
 * - @ref SOCK_CLOSE_WAIT  : Closing state
 * - @ref SOCK_UDP   		: UDP socket
 * - @ref SOCK_IPRAW       : IPRAW socket
 * - @ref SOCK_MACRAW  		: MAC raw mode socket
 * - @ref SOCK_PPPoE       : PPPoE mode Socket
 *@par Temporary status during changing the status of Socket n.
 * - @ref SOCK_SYNSENT   	: This indicates Socket n sent the connect-request packet (SYN packet) to a peer.
 * - @ref SOCK_SYNRECV    	: It indicates Socket n successfully received the connect-request packet (SYN packet) from a peer.
 * - @ref SOCK_FIN_WAIT		: Connection state
 * - @ref SOCK_CLOSING		: Closing state
 * - @ref SOCK_TIME_WAIT	: Closing state
 * - @ref SOCK_LAST_ACK 	: Closing state
 * - @ref SOCK_ARP         : ARP request state 
 */
#define  Sn_SSR(n)  (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x08)
#define  Sn_SR(n)   Sn_SSR(n)   ///< For Compatible ioLibrary. Refer to @ref Sn_SSR(n)

/**
 * @ingroup Socket_register_group_W5300
 * @brief source port register(R/W)
 * @details @ref Sn_PORTR configures the source port number of Socket n.
 * It is valid when Socket n is used in TCP/UPD mode. It should be set before OPEN command is ordered.
 */
#define  Sn_PORTR(n)  (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x0A)
#define  Sn_PORT(n)   Sn_PORTR(n)       ///< For compatible ioLibrary. Refer to @ref Sn_PORTR(n).

/**
 * @ingroup Socket_register_group_W5300
 * @brief Peer MAC register address(R/W)
 * @details @ref Sn_DHAR configures the destination hardware address of Socket n when using SEND_MAC command in UDP mode or
 * it indicates that it is acquired in ARP-process by CONNECT/SEND command.
 */
#define  Sn_DHAR(n)  (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x0C)

/**
 * @ingroup Socket_register_group_W5300
 * @brief Peer port register address(R/W)
 * @details @ref Sn_DPORTR configures or indicates the destination port number of Socket n. It is valid when Socket n is used in TCP/UDP mode.
 * In TCP clientmode, it configures the listen port number of TCP serverbefore CONNECT command.
 * In TCP Servermode, it indicates the port number of TCP client after successfully establishing connection.
 * In UDP mode, it configures the port number of peer to be transmitted the UDP packet by SEND/SEND_MAC command.
 */
#define  Sn_DPORTR(n)  (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x12)
#define  Sn_DPORT(n)   Sn_DPORTR(n)    ///< For compatible ioLibrary. Refer to \ref Sn_DPORTR.

/**
 * @ingroup Socket_register_group_W5300
 * @brief Peer IP register address(R/W)
 * @details @ref Sn_DIPR configures or indicates the destination IP address of Socket n. It is valid when Socket n is used in TCP/UDP mode.
 * In TCP client mode, it configures an IP address of TCP serverbefore CONNECT command.
 * In TCP server mode, it indicates an IP address of TCP clientafter successfully establishing connection.
 * In UDP mode, it configures an IP address of peer to be received the UDP packet by SEND or SEND_MAC command.
 */
 #define  Sn_DIPR(n)  (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x14)

/**
 * @ingroup Socket_register_group_W5300
 * @brief Maximum Segment Size(Sn_MSSR0) register address(R/W)
 * @details @ref Sn_MSSR configures or indicates the MTU(Maximum Transfer Unit) of Socket n.
 */
#define  Sn_MSSR(n)  (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x18)

/**
 * @ingroup Socket_register_group_W5300
 * @brief Keep Alive Timer register(R/W)
 * @details @ref Sn_KPALVTR configures the transmitting timer of KEEP ALIVE(KA)packet of SOCKETn. It is valid only in TCP mode,
 * and ignored in other modes. The time unit is 5s.
 * KA packet is transmittable after @ref Sn_SR is changed to SOCK_ESTABLISHED and after the data is transmitted or received to/from a peer at least once.
 * In case of '@ref Sn_KPALVTR > 0', W5500 automatically transmits KA packet after time-period for checking the TCP connection (Auto-keepalive-process).
 * In case of '@ref Sn_KPALVTR = 0', Auto-keep-alive-process will not operate,
 * and KA packet can be transmitted by SEND_KEEP command by the host (Manual-keep-alive-process).
 * Manual-keep-alive-process is ignored in case of '@ref Sn_KPALVTR > 0'.
 */
#define  Sn_KPALVTR(n)  (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x1A)

/**
 * @ingroup Socket_register_group_W5300
 * @brief IP Protocol(PROTO) Register(R/W)
 * @details \ref Sn_PROTO that sets the protocol number field of the IP header at the IP layer. It is
 * valid only in IPRAW mode, and ignored in other modes.
 */
#define  Sn_PROTOR(n)  Sn_KPALVTR(n)

/**
 * @ingroup Socket_register_group_W5300
 * @brief IP Type of Service(TOS) Register(R/W)
 * @details @ref Sn_TOSR configures the TOS(Type Of Service field in IP Header) of Socket n.
 * It is set before OPEN command.
 */
#define  Sn_TOSR(n)  (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x1C)
#define  Sn_TOS(n)   Sn_TOSR(n)  ///< For compatible ioLibrary. Refer to Sn_TOSR

/**
 * @ingroup Socket_register_group_W5300
 * @brief IP Time to live(TTL) Register(R/W)
 * @details @ref Sn_TTLR configures the TTL(Time To Live field in IP header) of Socket n.
 * It is set before OPEN command.
 */
#define  Sn_TTLR(n)  (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x1E)
#define  Sn_TTL(n)   Sn_TTLR(n)  ///< For compatible ioLibrary. Refer to Sn_TTLR

/**
 * @ingroup Socket_register_group_W5300
 * @brief  SOCKETn TX write size register(R/W)
 * @details It sets the byte size of the data written in internal TX memory through @ref Sn_TX_FIFOR.
 * It is set before SEND or SEND_MAC command, and can't be bigger than internal TX memory
 * size set by TMSR such as @ref TMS01R, TMS23R and etc.
 */
#define  Sn_TX_WRSR(n)  (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x20)

/**
 * @ingroup Socket_register_group_W5300
 * @brief Transmit free memory size register(R)
 * @details Sn_TX_FSR indicates the free size of Socket n TX Buffer Block. It is initialized to the configured size by TMSR such as @ref TMS01SR.
 * Data bigger than Sn_TX_FSR should not be saved in the Socket n TX Buffer because the bigger data overwrites the previous saved data not yet sent.
 * Therefore, check before saving the data to the Socket n TX Buffer, and if data is equal or smaller than its checked size,
 * transmit the data with SEND/SEND_MAC command after saving the data in Socket n TX buffer. But, if data is bigger than its checked size,
 * transmit the data after dividing into the checked size and saving in the Socket n TX buffer.
 */
#define  Sn_TX_FSR(n)  (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x0024)

/**
 * @ingroup Socket_register_group_w5300
 * @brief Received data size register(R)
 * @details @ref Sn_RX_RSR indicates the data size received and saved in Socket n RX Buffer.
 * @ref Sn_RX_RSR does not exceed the RMSR such as @ref RMS01SR  and is calculated as the difference between
 * ?Socket n RX Write Pointer (@ref Sn_RX_WR)and Socket n RX Read Pointer (@ref Sn_RX_RD)
 */
#define  Sn_RX_RSR(n)  (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x0028)

/**
 * @ingroup Socket_register_group_W5300
 * @brief Fragment field value in IP header register(R/W)
 * @details @ref Sn_FRAGR configures the FRAG(Fragment field in IP header).
 */
#define  Sn_FRAGR(n)  (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x002C)
#define  Sn_FRAG(n)   Sn_FRAGR(n)

/**
 * @ingroup Socket_register_group_W5300
 * @brief SOCKET n TX FIFO regsiter
 * @details It indirectly accesses internal TX memory of SOCKETn.
 * The internal TX memory can't be accessed directly by the host, but can be accessed through Sn_TX_FIFOR. 
 * If @ref MR(MT) = '0', only the Host-Write of internal TX memory is allowed through Sn_TX_FIFOR. 
 * But if @ref MR(MT) is '1', both of Host-Read and Host-Write are allowed.
 */
#define  Sn_TX_FIFOR(n)  (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x2E)
/**

 * @ingroup Socket_register_group_W5300
 * @brief SOCKET n RX FIFO register
 * @details It indirectly accesses to internal RX memory of SOCKETn.
 * The internal RX memory can't be directly accessed by the host, but can be accessed through Sn_RX_FIFOR. 
 * If MR(MT) = '0', only the Host-Read of internal RX memory is allowed through Sn_RX_FIFOR. 
 * But if MR(MT) is '1', both of Host-Read and Host-Write are allowed. 
 */
#define  Sn_RX_FIFOR(n)  (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x30)

//#define  Sn_TX_SADR(n)  (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x32)
//#define  Sn_RX_SADR(n)  (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x34)
//#define  Sn_TX_RD(n)    (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x36)
//#define  Sn_TX_WR(n)    (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x38)
//#define  Sn_TX_ACK(n)   (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x3A)
//#define  Sn_RX_RD(n)    (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x3C)
//#define  Sn_RX_WR(n)    (_W5300_IO_BASE_ + WIZCHIP_SREG_BLOCK(n) + 0x3E)

/************************************/
/* The bit of MR regsiter defintion */
/************************************/
#define  MR_DBW     (1 << 15)  /**< Data bus width bit of \ref MR. Read Only. (0 : 8Bit, 1 : 16Bit)*/
#define  MR_MPF     (1 << 14)  /**< Mac layer pause frame bit of \ref MR. (0 : Disable, 1 : Enable)*/
#define  MR_WDF(X)  ((X & 0x07) << 11)  /**< Write data fetch time bit of  \ref MR. Fetch Data from DATA bus after PLL_CLK * MR_WDF[2:0]*/
#define  MR_RDH     (1 << 10)  /**< Read data hold time bit of \ref MR. Hold Data on DATA bus during 2 * PLL_CLK after CS high*/
#define  MR_FS      (1 << 8)   /**< FIFO swap bit of \ref MR. Swap MSB & LSB of \ref Sn_TX_FIFOR & Sn_RX_FIFOR (0 : No swap, 1 : Swap) */
#define  MR_RST     (1 << 7)   /**< S/W reset bit of \ref MR. (0 : Normal Operation,  1 : Reset (automatically clear after reset))*/
#define  MR_MT      (1 << 5)   /**< Memory test bit of \ref MR. (0 : Normal, 1 : Internal Socket memory write & read Test)*/
#define  MR_PB      (1 << 4)   /**< Ping block bit of \ref MR. (0 : Unblock, 1 : Block)*/
#define  MR_PPPoE   (1 << 3)   /**< PPPoE bit of \ref MR. (0 : No use PPPoE, 1: Use PPPoE)*/
#define  MR_DBS     (1 << 2)   /**< Data bus swap of \ref MR. Valid only 16bit mode (0 : No swap, 1 : Swap)*/
#define  MR_IND     (1 << 0)   /**< Indirect mode bit of \ref MR. (0 : Direct mode, 1 : Indirect mode) */

/************************************/ 
/* The bit of IR regsiter definition */ 
/************************************/ 
#define  IR_IPCF      (1 << 7)  /**< IP conflict bit of \ref IR. To clear, Write the bit to '1'. */
#define  IR_DPUR      (1 << 6)  /**< Destination port unreachable bit of \ref IR. To clear, Write the bit to '1'. */
#define  IR_PPPT      (1 << 5)  /**< PPPoE terminate bit of \ref IR. To clear, Write the bit to '1'. */
#define  IR_FMTU      (1 << 4)  /**< Fragment MTU bit of IR. To clear, Write the bit to '1'. */
#define  IR_SnINT(n)  (0x01 << n)  /**< SOCKETn interrupt occurrence bit of \ref IR. To clear, Clear \ref Sn_IR*/

/*****************************************/ 
/* The bit of Pn_BRDYR regsiter definition*/ 
/*****************************************/ 
#define  Pn_PEN    (1 << 7)  /**< PIN 'BRDYn' enable bit of Pn_BRDYR. */
#define  Pn_MT     (1 << 6)  /**< PIN memory type bit of Pn_BRDYR. */
#define  Pn_PPL    (1 << 5)  /**< PIN Polarity bit of Pn_BRDYR. */
#define  Pn_SN(n)  ((n & 0x07) << 0)  /**< What socket to monitor. */

/***************************************/ 
/* The bit of Sn_MR regsiter definition */ 
/***************************************/ 
/**
 * @brief Alignment bit of \ref Sn_MR. 
 * @details It is valid only in the TCP (\ref Sn_MR_TCP) with TCP communication, 
 * when every the received DATA packet size is of even number and set as '1', 
 * data receiving performance can be improved by removing PACKET-INFO(data size) that is attached to every the received DATA packet.
 */
#define  Sn_MR_ALIGN  (1 << 8)     

/**
 * @brief Multicasting bit of \ref Sn_MR
 * @details It is valid only in UDP (\ref Sn_MR_UDP).
 * In order to implement multicasting, set the IP address and port number in @ref Sn_DIPR and @ref Sn_DPORTR respectively before "OPEN" command(@ref Sn_CR_OPEN).\n
 * 0 : Disable, 1 : Enable
 */
#define  Sn_MR_MULTI  (1 << 7)     

/**
 * @brief  MAC filter bit of \ref Sn_MR
 * @details It is valid in MACRAW(@ref Sn_MR_MACRAW).
 * When this bit is set as ‘1’, W5300 can receive packet that is belong in itself or broadcasting. 
 * When this bit is set as ‘0’, W5300 can receive all packets on Ethernet. 
 * When using the hybrid TCP/IP stack, it is recommended to be set as ‘1’ for reducing the receiving overhead of host. \n
 * 0 : Disable, 1 : Enable
 */
#define  Sn_MR_MF  (1 << 6)             

/**
 * @brief IGMP version bit of \ref Sn_MR
 * details It is valid in case of @ref Sn_MR_MULTI='1' and UDP(@ref Sn_MR_UDP).
 * It configures IGMP version to send IGMP message such as <b>Join/Leave/Report</b> to multicast-group. \n
 * 0 : IGMPv2, 1 : IGMPv1
 */
#define  Sn_MR_IGMPv  (1 << 5)
#define  Sn_MR_MC     Sn_MR_IGMPv       ///< For compatible ioLibrary

/**
 * @brief No delayed ack bit of \ref Sn_MR
 * @details It is valid in TCP(@ref Sn_MR_TCP).
 * In case that it is set as '1', ACK packet is transmitted right after receiving DATA packet from the peer. 
 * It is recommended to be set as '1' for TCP performance improvement.
 * In case that it is set as '0', ACK packet is transmitted after the time set in @ref _RTR_ regardless of DATA packet receipt.\n
 * 0 : No use, 1 : Use 
 */
#define  Sn_MR_ND  (1 << 5)             

/**
 * @brief No mode
 * @details This configures the protocol mode of Socket n.
 * @sa Sn_MR
 */
#define  Sn_MR_CLOSE  0x00                 

/**
 * @brief TCP mode 
 * @details This configures the protocol mode of Socket n.
 * @sa Sn_MR
 */
#define  Sn_MR_TCP  0x01                

/**
 * @brief UDP mode
 * @details This configures the protocol mode of Socket n.
 * @sa Sn_MR
 */
#define  Sn_MR_UDP  0x02  /**< Protocol bits of \ref Sn_MR. */

/**
 * @brief IP LAYER RAW mode
 * @details This configures the protocol mode of Socket n.
 * @sa Sn_MR 
 */
#define  Sn_MR_IPRAW  0x03  /**< Protocol bits of \ref Sn_MR. */

/**
 * @brief MAC LAYER RAW mode
 * @details This configures the protocol mode of Socket 0.
 * @sa Sn_MR 
 * @note MACRAW mode should be only used in Socket 0.
 */
#define  Sn_MR_MACRAW  0x04 

/**
 * @brief PPPoE mode
 * @details This configures the protocol mode of Socket 0.
 * @sa Sn_MR 
 * @note PPPoE mode should be only used in Socket 0.
 */                
#define  Sn_MR_PPPoE  0x05  /**< Protocol bits of \ref Sn_MR. */

#define  SOCK_STREAM  Sn_MR_TCP  /**< For Berkeley Socket API, Refer to @ref Sn_MR_TCP */
#define  SOCK_DGRAM   Sn_MR_UDP  /**< For Berkeley Socket API, Refer to @ref Sn_MR_UDP */

/******************************/ 
/* The values of CR definition */ 
/******************************/
/**
 * @brief Initialize or open a socket
 * @details Socket n is initialized and opened according to the protocol selected in Sn_MR(P3:P0).
 * The table below shows the value of @ref Sn_SR corresponding to @ref Sn_MR.\n
 * <table>
 *   <tr>  <td>\b Sn_MR (P[3:0])</td> <td>\b Sn_SR</td>            		 </tr>
 *   <tr>  <td>Sn_MR_CLOSE  (000)</td> <td></td>         	   		 </tr>
 *   <tr>  <td>Sn_MR_TCP  (001)</td> <td>SOCK_INIT (0x13)</td>  		 </tr>
 *   <tr>  <td>Sn_MR_UDP  (010)</td>  <td>SOCK_UDP (0x22)</td>  		 </tr>
 *   <tr>  <td>Sn_MR_IPRAW  (010)</td>  <td>SOCK_IPRAW (0x32)</td>  		 </tr>
 *   <tr>  <td>Sn_MR_MACRAW  (100)</td>  <td>SOCK_MACRAW (0x42)</td>  </tr>
 *   <tr>  <td>Sn_MR_PPPoE  (101)</td>  <td>SOCK_PPPoE (0x5F)</td>  </tr>
 * </table>
 */
#define  Sn_CR_OPEN  0x01

/**
 * @brief Wait connection request in TCP mode(Server mode)
 * @details This is valid only in TCP mode (\ref Sn_MR(P3:P0) = \ref Sn_MR_TCP).
 * In this mode, Socket n operates as a TCP serverand waits for  connection-request (SYN packet) from any TCP client
 * The @ref Sn_SR changes the state from \ref SOCK_INIT to \ref SOCKET_LISTEN.
 * When a TCP clientconnection request is successfully established,
 * the @ref Sn_SR changes from SOCK_LISTEN to SOCK_ESTABLISHED and the @ref Sn_IR(0) becomes 
 * But when a TCP clientconnection request is failed, @ref Sn_IR(3) becomes and the status of @ref Sn_SR changes to SOCK_CLOSED.
 */
#define  Sn_CR_LISTEN  0x02

/**
 * @brief Send connection request in TCP mode(Client mode)
 * @details  To connect, a connect-request (SYN packet) is sent to <b>TCP server</b>configured by @ref Sn_DIPR & Sn_DPORT(destination address & port).
 * If the connect-request is successful, the @ref Sn_SR is changed to @ref SOCK_ESTABLISHED and the Sn_IR(0) becomes \n\n
 * The connect-request fails in the following three cases.\n
 * 1. When a @b ARPTO occurs (@ref Sn_IR[3] =  '1') because destination hardware address is not acquired through the ARP-process.\n
 * 2. When a @b SYN/ACK packet is not received and @b TCPTO (Sn_IR(3) =  )\n
 * 3. When a @b RST packet is received instead of a @b SYN/ACK packet. In these cases, @ref Sn_SR is changed to @ref SOCK_CLOSED.
 * @note This is valid only in TCP mode and operates when Socket n acts as <b>TCP client</b>
 */
#define  Sn_CR_CONNECT  0x04

/**
 * @brief Send closing request in TCP mode
 * @details Regardless of <b>TCP server</b>or <b>TCP client</b> the DISCON command processes the disconnect-process (b>Active close</b>or <b>Passive close</b>.\n
 * @par Active close
 * it transmits disconnect-request(FIN packet) to the connected peer\n
 * @par Passive close
 * When FIN packet is received from peer, a FIN packet is replied back to the peer.\n
 * @details When the disconnect-process is successful (that is, FIN/ACK packet is received successfully), @ref Sn_SR is changed to @ref SOCK_CLOSED.\n
 * Otherwise, @b TCPTO occurs (\ref Sn_IR[3]='1') and then @ref Sn_SR is changed to @ref SOCK_CLOSED.
 * @note Valid only in TCP mode.
 */
#define  Sn_CR_DISCON  0x08

/**
 * @brief Close socket
 * @details @ref Sn_SR is changed to @ref SOCK_CLOSED.
 */
#define  Sn_CR_CLOSE  0x10

/**
 * @brief Update TX buffer pointer and send data
 * @details SEND command transmits all the data in the Socket n TX buffer thru @ref Sn_TX_FIFOR.\n
 * For more details, please refer to Socket n TX Free Size Register (@ref Sn_TX_FSR) and Socket TX Write Size register (@ref Sn_TX_WRSR).
 */
#define  Sn_CR_SEND  0x20

/**
 * @brief Send data with MAC address, so without ARP process
 * @details The basic operation is same as SEND.\n
 * Normally SEND command transmits data after destination hardware address is acquired by the automatic ARP-process(Address Resolution Protocol).\n
 * But SEND_MAC command transmits data without the automatic ARP-process.\n
 * In this case, the destination hardware address is acquired from @ref Sn_DHAR configured by host, instead of APR-process.
 * @note Valid only in UDP mode.
 */
#define  Sn_CR_SEND_MAC  0x21

/**
 * @brief Send keep alive message
 * @details It checks the connection status by sending 1byte keep-alive packet.\n
 * If the peer can not respond to the keep-alive packet during timeout time, the connection is terminated and the timeout interrupt will occur.
 * @note Valid only in TCP mode.
 */
#define  Sn_CR_SEND_KEEP  0x22

/**
 * @brief Update RX buffer pointer and receive data
 * @details RECV completes the processing of the received data in Socket n RX Buffer thru @ref Sn_RX_FIFOR).\n
 * For more details, refer to Socket n RX Received Size Register (@ref Sn_RX_RSR) & @ref Sn_RX_FIFOR.
 */
#define  Sn_CR_RECV  0x40  /**< RECV command value of \ref Sn_CR */
#define  Sn_CR_PCON  0x23  /**< PPPoE connection begins by transmitting PPPoE discovery packet. Refer to \ref Sn_CR */
#define  Sn_CR_PDISCON  0x24  /**< Closes PPPoE connection. Refer to \ref Sn_CR */ 
#define  Sn_CR_PCR  0x25   /**< In each phase, it transmits REQ message. Refer to \ref Sn_CR */
#define  Sn_CR_PCN  0x26   /**< In each phase, it transmits NAK message. Refer to \ref Sn_CR */
#define  Sn_CR_PCJ  0x27   /**< In each phase, it transmits REJECT message. Refer to \ref Sn_CR */

/*********************************/ 
/* The values of Sn_IR definition */ 
/*********************************/
#define  Sn_IR_PRECV  0x80  /**< It is set in the case that option data which is not supported is received. Refer to \ref Sn_IR */
#define  Sn_IR_PFAIL  0x40  /**< It is set in the case that PAP authentication is failed. Refer to \ref Sn_IR */
#define  Sn_IR_PNEXT  0x20  /**< It is set in the case that the phase is changed during PPPoE connection process. \ref Sn_IR */
#define  Sn_IR_SENDOK 0x10  /**< It is set when SEND command is completed. Refer to \ref Sn_IR */
#define  Sn_IR_TIMEOUT  0x08  /**< It is set when ARPTO or TCPTO is occured.  Refer to \ref Sn_IR */
#define  Sn_IR_RECV   0x04  /**< It is set whenever data is received from a peer.  Refer to \ref Sn_IR */
#define  Sn_IR_DISCON 0x02  /**< It is set when FIN or FIN/ACK packet is received from a peer.  Refer to \ref Sn_IR */
#define  Sn_IR_CON    0x01  /**< It is set one time when the connection is successful and then @ref Sn_SR is changed to @ref SOCK_ESTABLISHED. */

/**********************************/ 
/* The values of Sn_SSR definition */ 
/**********************************/
/**
 * @brief The state of SOCKET intialized or closed
 * @details This indicates that Socket n is released.\n
 * When DICON, CLOSE command is ordered, or when a timeout occurs, it is changed to @ref SOCK_CLOSED regardless of previous status.
 */
#define  SOCK_CLOSED  0x00

/**
 * @brief The state of ARP process
 * @details It is temporary state for getting a peer MAC address when TCP connect or UDP Data Send\n
 * When DICON, CLOSE command is ordered, or when a timeout occurs, it is changed to @ref SOCK_CLOSED regardless of previous status.
 */
#define  SOCK_ARP  0x01  /**< ARP-request is transmitted in order to acquire destination hardware address. */

/**
 * @brief Initiate state in TCP.
 * @details This indicates Socket n is opened with TCP mode.\n
 * It is changed to @ref SOCK_INIT when \ref Sn_MR(P[3:0]) = '001' and OPEN command(\ref Sn_CR_OPEN) is ordered.\n
 * After SOCK_INIT, user can use LISTEN(@ref Sn_CR_LISTEN)/CONNECT(@ref Sn_CR_CONNET) command.
 */
#define  SOCK_INIT  0x13

/**
 * @brief Listen state
 * @details This indicates Socket n is operating as <b>TCP server</b>mode and waiting for connection-request (SYN packet) from a peer <b>TCP client</b>.\n
 * It will change to @ref SOCK_ESTALBLISHED when the connection-request is successfully accepted.\n
 * Otherwise it will change to @ref SOCK_CLOSED after TCPTO (@ref Sn_IR_TIMEOUT = '1') is occurred.
 */
#define  SOCK_LISTEN  0x14

/**
 * @brief Connection state
 * @details This indicates Socket n sent the connect-request packet (SYN packet) to a peer.\n
 * It is temporarily shown when @ref Sn_SR is changed from @ref SOCK_INIT to @ref SOCK_ESTABLISHED by @ref Sn_CR_CONNECT command.\n
 * If connect-accept(SYN/ACK packet) is received from the peer at SOCK_SYNSENT, it changes to @ref SOCK_ESTABLISHED.\n
 * Otherwise, it changes to @ref SOCK_CLOSED after TCPTO (@ref Sn_IR_TIMEOUT = '1') is occurred.
 */
#define  SOCK_SYNSENT  0x15

/**
 * @brief Connection state
 * @details It indicates Socket n successfully received the connect-request packet (SYN packet) from a peer.\n
 * If socket n sends the response (SYN/ACK  packet) to the peer successfully,  it changes to @ref SOCK_ESTABLISHED. \n
 * If not, it changes to @ref SOCK_CLOSED after timeout (@ref Sn_IR_TIMEOUT = '1') is occurred.
 */
#define  SOCK_SYNRECV  0x16

/**
 * @brief Success to connect
 * @details This indicates the status of the connection of Socket n.\n
 * It changes to @ref SOCK_ESTABLISHED when the <b>TCP SERVER</b>processed the SYN packet from the <b>TCP CLIENT</b>during @ref SOCK_LISTEN, or
 * when the @ref Sn_CR_CONNECT command is successful.\n
 * During @ref SOCK_ESTABLISHED, DATA packet can be transferred using @ref Sn_CR_SEND or @ref Sn_CR_RECV command.
 */
#define  SOCK_ESTABLISHED  0x17

/**
 * @brief Closing state
 * @details These indicate Socket n is closing.\n
 * These are shown in disconnect-process such as active-close and passive-close.\n
 * When Disconnect-process is successfully completed, or when timeout(@ref Sn_CR_TIMTEOUT = '1') is occurred, these change to @ref SOCK_CLOSED.
 */
#define  SOCK_FIN_WAIT  0x18

/**
 * @brief Closing state
 * @details These indicate Socket n is closing.\n
 * These are shown in disconnect-process such as active-close and passive-close.\n
 * When Disconnect-process is successfully completed, or when timeout occurs, these change to @ref SOCK_CLOSED.
 */
#define  SOCK_CLOSING  0x1A

/**
 * @brief Closing state
 * @details These indicate Socket n is closing.\n
 * These are shown in disconnect-process such as active-close and passive-close.\n
 * When Disconnect-process is successfully completed, or when timeout occurs, these change to @ref SOCK_CLOSED.
 */
#define  SOCK_TIME_WAIT  0x1B

/**
 * @brief Closing state
 * @details This indicates Socket n received the disconnect-request (FIN packet) from the connected peer.\n
 * This is half-closing status, and data can be transferred.\n
 * For full-closing, @ref Sn_CR_DISCON command is used. But For just-closing, @ref Sn_CR_CLOSE command is used.
 */
#define  SOCK_CLOSE_WAIT  0x1C

/**
 * @brief Closing state
 * @details This indicates Socket n is waiting for the response (FIN/ACK packet) to the disconnect-request (FIN packet) by passive-close.\n
 * It changes to @ref SOCK_CLOSED when Socket n received the response successfully, or when timeout (@ref Sn_IR_TIMEOUT = '1') is occurred.
 */
#define  SOCK_LAST_ACK  0x1D

/**
 * @brief UDP socket
 * @details This indicates Socket n is opened in UDP mode(@ref Sn_MR(P[3:0]) = '010').\n
 * It changes to SOCK_UDP when @ref Sn_MR(P[3:0]) = '010' and @ref Sn_CR_OPEN command is ordered.\n
 * Unlike TCP mode, data can be transfered without the connection-process.
 */
#define  SOCK_UDP  0x22

/**
 * @brief IP raw mode socket
 * @details TThe socket is opened in IPRAW mode. The SOCKET status is change to SOCK_IPRAW when @ref Sn_MR (P3:P0) is
 * Sn_MR_IPRAW and @ref Sn_CR_OPEN command is used.\n
 * IP Packet can be transferred without a connection similar to the UDP mode.
*/
#define  SOCK_IPRAW  0x32

/**
 * @brief MAC raw mode socket
 * @details This indicates Socket 0 is opened in MACRAW mode (@ref Sn_MR(P[3:0]) = '100' and n = 0) and is valid only in Socket 0.\n
 * It changes to SOCK_MACRAW when @ref Sn_MR(P[3:0] = 100)and @ ref Sn_CR_OPEN command is ordered.\n
 * Like UDP mode socket, MACRAW mode Socket 0 can transfer a MAC packet (Ethernet frame) without the connection-process.
 */
#define  SOCK_MACRAW  0x42  /**< SOCKET0 is open as MACRAW mode. */

/**
 * @brief PPPoE mode socket
 * @details It is the status that SOCKET0 is opened as PPPoE mode. 
 * It is changed to SOCK_PPPoE in case of @ref Sn_CR_OPEN command is ordered and @ref Sn_MR(P3:P0)= @ref Sn_MR_PPPoE\n
 * It is temporarily used at the PPPoE connection.
 */
#define  SOCK_PPPoE  0x5F  /**< SOCKET0 is open as PPPoE mode. */

/* IP PROTOCOL */
#define  IPPROTO_IP    0    //< Dummy for IP 
#define  IPPROTO_ICMP  1    //< Control message protocol
#define  IPPROTO_IGMP  2    //< Internet group management protocol
#define  IPPROTO_GGP   3    //< Gateway^2 (deprecated)
#define  IPPROTO_TCP   6    //< TCP
#define  IPPROTO_PUP   12   //< PUP
#define  IPPROTO_UDP   17   //< UDP
#define  IPPROTO_IDP   22   //< XNS idp
#define  IPPROTO_ND    77   //< UNOFFICIAL net disk protocol
#define  IPPROTO_RAW   255  //< Raw IP packet

typedef uint8_t SOCKET;
/*
class MR {
public:
  static const uint8_t RST   = 0x80;
  static const uint8_t PB    = 0x10;
  static const uint8_t PPPOE = 0x08;
  static const uint8_t LB    = 0x04;
  static const uint8_t AI    = 0x02;
  static const uint8_t IND   = 0x01;
};
*/
/*
class IR {
public:
  static const uint8_t CONFLICT = 0x80;
  static const uint8_t UNREACH  = 0x40;
  static const uint8_t PPPoE    = 0x20;
  static const uint8_t SOCK0    = 0x01;
  static const uint8_t SOCK1    = 0x02;
  static const uint8_t SOCK2    = 0x04;
  static const uint8_t SOCK3    = 0x08;
  static inline uint8_t SOCK(SOCKET ch) { return (0x01 << ch); };
};
*/

#if 1  /* redefinition of 'class SnMR' */ 
class SnMR {
public:
  static const uint8_t CLOSE  = 0x00;
  static const uint8_t TCP    = 0x01;
  static const uint8_t UDP    = 0x02;
  static const uint8_t IPRAW  = 0x03;
  static const uint8_t MACRAW = 0x04;
  static const uint8_t PPPOE  = 0x05;
  static const uint8_t ND     = 0x20;
  static const uint8_t MULTI  = 0x80;
};
#endif /* redefinition of 'class SnMR' */ 

enum SockCMD {
  Sock_OPEN      = 0x01,
  Sock_LISTEN    = 0x02,
  Sock_CONNECT   = 0x04,
  Sock_DISCON    = 0x08,
  Sock_CLOSE     = 0x10,
  Sock_SEND      = 0x20,
  Sock_SEND_MAC  = 0x21,
  Sock_SEND_KEEP = 0x22,
  Sock_RECV      = 0x40
};

/*class SnCmd {
public:
  static const uint8_t OPEN      = 0x01;
  static const uint8_t LISTEN    = 0x02;
  static const uint8_t CONNECT   = 0x04;
  static const uint8_t DISCON    = 0x08;
  static const uint8_t CLOSE     = 0x10;
  static const uint8_t SEND      = 0x20;
  static const uint8_t SEND_MAC  = 0x21;
  static const uint8_t SEND_KEEP = 0x22;
  static const uint8_t RECV      = 0x40;
};
*/

class SnIR {
public:
  static const uint8_t SEND_OK = 0x10;
  static const uint8_t TIMEOUT = 0x08;
  static const uint8_t RECV    = 0x04;
  static const uint8_t DISCON  = 0x02;
  static const uint8_t CON     = 0x01;
};

class SnSR {
public:
  static const uint8_t CLOSED      = 0x00;
  static const uint8_t INIT        = 0x13;
  static const uint8_t LISTEN      = 0x14;
  static const uint8_t SYNSENT     = 0x15;
  static const uint8_t SYNRECV     = 0x16;
  static const uint8_t ESTABLISHED = 0x17;
  static const uint8_t FIN_WAIT    = 0x18;
  static const uint8_t CLOSING     = 0x1A;
  static const uint8_t TIME_WAIT   = 0x1B;
  static const uint8_t CLOSE_WAIT  = 0x1C;
  static const uint8_t LAST_ACK    = 0x1D;
  static const uint8_t UDP         = 0x22;
  static const uint8_t IPRAW       = 0x32;
  static const uint8_t MACRAW      = 0x42;
  static const uint8_t PPPOE       = 0x5F;
};

class IPPROTO {
public:
  static const uint8_t IP   = 0;
  static const uint8_t ICMP = 1;
  static const uint8_t IGMP = 2;
  static const uint8_t GGP  = 3;
  static const uint8_t TCP  = 6;
  static const uint8_t PUP  = 12;
  static const uint8_t UDP  = 17;
  static const uint8_t IDP  = 22;
  static const uint8_t ND   = 77;
  static const uint8_t RAW  = 255;
};

class WizchipClass {
private:
  volatile uint16_t *mr;
  volatile uint16_t *idm_ar;
  volatile uint16_t *idm_dr;
  uint16_t txByteData[MAX_SOCK_NUM], rxByteData[MAX_SOCK_NUM];

  inline uint8_t write( uint16_t _addr, uint16_t _data )
  {
    *idm_ar = _addr;
    *idm_dr = _data;
    return 1;
  }

  inline uint8_t write( uint16_t _addr, [[maybe_unused]] uint8_t _cb, uint16_t _data )
  {
  //  (void)_cb;  // use [[maybe_unused]] from C++17
    *idm_ar = _addr;
    *idm_dr = _data;
    return 1;
  }

  inline uint16_t read( uint16_t _addr )
  {
    *idm_ar = _addr;
    return *idm_dr;
  }

  uint32_t write( uint16_t _addr, const uint8_t *_data, uint32_t _len );
  uint32_t write( uint16_t _addr, uint8_t _cb, const uint8_t *_data, uint32_t _len );
  uint32_t read( uint16_t _addr, volatile uint8_t *_data, uint32_t _len );

  void getSHAR( uint8_t *mac );
  void setSHAR( const uint8_t *mac );
  void getAnyIP( uint16_t adr, uint8_t *ip );
  void getGAR( uint8_t *gw );
  void getSUBR( uint8_t *subr );
  void getSIPR( uint8_t *sip );
  void setAnyIP( uint16_t adr, const uint8_t *ip );
  void setGAR( const uint8_t *gw );
  void setSUBR( const uint8_t *subr );
  void setSIPR( const uint8_t *sip );

  uint32_t readSnTX_FSR( SOCKET sn );
  uint32_t readSnRX_RSR( SOCKET sn );
  uint32_t readSnTX_WR( SOCKET sn );

  uint8_t rxFifoRead( SOCKET sn, uint32_t count = 0UL );
  uint32_t rxFifoRead( SOCKET sn, uint8_t *dst, uint32_t len );
  void txFifoWrite( SOCKET sn, uint8_t dat );

public:
  static const uint16_t SSIZE = 8192; // Max Tx buffer size
  static const uint16_t RSIZE = 8192; // Max Rx buffer size
  uint8_t  txByteCount[MAX_SOCK_NUM],rxByteCount[MAX_SOCK_NUM];
  uint32_t unReadDataSize[MAX_SOCK_NUM];
  uint8_t _remoteIP[MAX_SOCK_NUM][IPV4_SIZE];
  uint16_t _remotePort[MAX_SOCK_NUM];

  void swReset();
  void init( volatile uint16_t *m, volatile uint16_t *a, volatile uint16_t *d );
  uint16_t readVersion( void ) { *idm_ar = W5300_VERSIONR; return *idm_dr; }
  void execCmdSn( SOCKET sn, SockCMD _cmd );

  void setMACAddress( const uint8_t * addr );
  void getMACAddress( uint8_t * addr );
  void setGatewayIp( uint8_t *_addr );
  void getGatewayIp( uint8_t *_addr );
  void setSubnetMask( uint8_t *_addr );
  void getSubnetMask( uint8_t *_addr );
  void setIPAddress( uint8_t * addr );
  void getIPAddress( uint8_t * addr );

  void readTxMemorySize( uint16_t *sz );
  void readRxMemorySize( uint16_t *sz );
  int writeTxMemorySize( SOCKET sn, uint8_t sz );
  int writeRxMemorySize( SOCKET sn, uint8_t sz );
  uint32_t getTXFreeSize( SOCKET sn );
  uint32_t getRXReceivedSize( SOCKET sn );
  void enterMemoryTestMode() { *mr |= MR_MT; }  // Memory test bit of \ref MR. (0 : Normal, 1 : Internal Socket memory write & read Test)
  void exitMemoryTestMode() { *mr &= ~MR_MT; }  // Memory test bit of \ref MR. (0 : Normal, 1 : Internal Socket memory write & read Test)
  void writeSnTX_WR( SOCKET sn, uint32_t sz );

  inline void rxByteCountClear( SOCKET sn ) { rxByteCount[ sn ] = 0; }
//  inline void txByteCountClear( SOCKET sn ) { txByteCount[ sn ] = 0; }
//  inline uint8_t getTxByteCount( SOCKET sn ) { return txByteCount[ sn ]; }

  /**
   * @brief	 This function is being called by send() and sendto() function also. 
   * 
   * This function read the Tx write pointer register and after copy the data in buffer update the Tx write pointer
   * register. User should read upper byte first and lower byte later to get proper value.
   */
  void send_data_processing( SOCKET sn, const uint8_t *data, uint32_t len );
  /**
   * @brief A copy of send_data_processing that uses the provided ptr for the
   *        write offset.  Only needed for the "streaming" UDP API, where
   *        a single UDP packet is built up over a number of calls to
   *        send_data_processing_ptr, because TX_WR doesn't seem to get updated
   *        correctly in those scenarios
   * @param ptr value to use in place of TX_WR.  If 0, then the value is read
   *        in from TX_WR
   * @return New value for ptr, to be used in the next call
   */
  // FIXME Update documentation
  void send_data_processing_offset(
    SOCKET sn, uint32_t data_offset, const uint8_t *data, uint32_t len );

  /**
   * @brief	This function is being used for copy the data form Receive buffer of the chip to application buffer.
   * 
   * It calculate the actual physical address where one has to read
   * the data from Receive buffer. Here also take care of the condition while it exceed
   * the Rx memory uper-bound of socket.
   */
  void read_data( SOCKET sn, volatile uint16_t src, volatile uint8_t *dst, uint32_t len );
  void read_data( SOCKET sn, volatile uint8_t *dst, uint32_t len );

  /**
   * @brief	This function is being called by recv() also.
   * 
   * This function read the Rx read pointer register
   * and after copy the data from receive buffer update the Rx write pointer register.
   * User should read upper byte first and lower byte later to get proper value.
   */
  void recv_data_processing( SOCKET sn, uint8_t *data, uint32_t len, uint8_t peek = 0 );

  inline void setRetransmissionTime( uint16_t _timeout )
  {
    *idm_ar = W5300_RTR; *idm_dr = _timeout;
  }

  inline void setRetransmissionCount( uint8_t _retry )
  {
    *idm_ar = W5300_RCR; *idm_dr = _retry;
  }

  inline void writeSnMR( SOCKET sn, uint16_t dat ) { write( Sn_MR(sn), dat ); }
//  inline void writeSnCR( SOCKET sn, uint16_t dat ) { write( Sn_CR(sn), dat ); }
  inline void writeSnIR( SOCKET sn, uint16_t dat ) { write( Sn_IR(sn), dat ); }
//  inline void writeSnSR( SOCKET sn, uint16_t dat ) { write( Sn_SR(sn), dat ); }
  inline void writeSnPORT( SOCKET sn, uint16_t dat ) { write( Sn_PORT(sn), dat ); }
//  inline void writeSnMSSR( SOCKET sn, uint16_t dat ) { write( Sn_MSSR(sn), dat ); }
//  inline void writeSnPROTO( SOCKET sn, uint16_t dat ) { write( Sn_PROTOR(sn), dat ); }
//  inline void writeSnTOS( SOCKET sn, uint16_t dat ) { write( Sn_TOS(sn), dat ); }
//  inline void writeSnTTL( SOCKET sn, uint16_t dat ) { write( Sn_TTL(sn), dat ); }
//  inline void writeSnTX_FSR( SOCKET sn, uint16_t dat ) { write( Sn_TX_FSR(sn), dat ); }
//  inline void writeSnRX_RSR( SOCKET sn, uint16_t dat ) { write( Sn_RX_RSR(sn), dat ); }

  inline void writeSnDIPR( SOCKET sn, uint8_t *dip ) { setAnyIP( Sn_DIPR(sn), (const uint8_t *)dip ); }
  inline void writeSnDPORT( SOCKET sn, uint16_t port ) { write( Sn_DPORT(sn), port ); }

  inline uint16_t readSnMR( SOCKET sn ) { return read( Sn_MR(sn) ); }
  inline uint16_t readSnSR( SOCKET sn ) { return read( Sn_SR(sn) ); }
  inline uint16_t readSnIR( SOCKET sn ) { return read( Sn_IR(sn) ); }
  inline uint16_t readSnMSSR( SOCKET sn ) { return read( Sn_MSSR(sn) ); }
};

extern WizchipClass wizchip;

#endif  /* W5300_H_INCLUDED */
