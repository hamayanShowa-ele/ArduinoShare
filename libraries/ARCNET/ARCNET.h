/**
  ******************************************************************************
  * @file           : ARCNET.h
  * @brief          : ARCNET header
  ******************************************************************************
  */
#ifndef  __ARCNET_H__
#define  __ARCNET_H__

#include  <Arduino.h>
#include  <STM32_EXTI.h>
#include  <DEBUG_UTIL.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Exported functions prototypes ---------------------------------------------*/
#ifdef __cplusplus
}
#endif

/* Exported functions prototypes ---------------------------------------------*/

#define  COM20022_BUFFER_SIZE     2048
#define  COM20022_ENTRY_TRANSMIT_PAGE 0
#define  COM20022_END_TRANSMIT_PAGE   0
#define  COM20022_ENTRY_RECIEVE_PAGE  1
#define  COM20022_END_RECIEVE_PAGE    3  // 3

enum ARCNET_BAUD { ARC_BAUD_2M5, ARC_BAUD_1M25, ARC_BAUD_625K, ARC_BAUD_312K5, ARC_BAUD_156K25, ARC_BAUD_5M, ARC_BAUD_10M };
enum ARCNET_CLOCK { ARC_CLOCK_20M, ARC_CLOCK_40M, ARC_CLOCK_RESERVE, ARC_CLOCK_80M, };
enum ARCNET_RCNTM { ARC_RCNTM_210m, ARC_RCNTM_52m5, ARC_RCNTM_26m25, ARC_RCNTM_13m125, };  // RECON period at 10 Mbps
enum ARCNET_PAGES { ARC_PAGE_0, ARC_PAGE_1, ARC_PAGE_2, ARC_PAGE_3, };
enum ARCNET_ERROR_CODE
{
  ARC_ERR_BEGIN_0 = -1,
  ARC_ERR_BEGIN_1 = -2,
  ARC_ERR_BEGIN_2 = -3,
  ARC_ERR_BEGIN_3 = -4,
  ARC_ERR_BEGIN_4 = -5,
  ARC_ERR_BEGIN_5 = -6,
  ARC_ERR_BEGIN_6 = -7,
  ARC_ERR_PAGE_WRITE_0 = -10,
  ARC_ERR_PAGE_READ_0 = -11,
  ARC_ERR_SEND_0 = -20,
  ARC_ERR_SEND_1 = -21,
  ARC_ERR_SEND_2 = -22,
  ARC_ERR_RECV_0 = -30,
  ARC_ERR_RECV_1 = -31,
  ARC_ERR_MEMCHECK_0 = -40,
  ARC_ERR_MEMCHECK_1 = -41,
};

/*
  command and status and read register bit.
*/
#define  ARC_ADR_STATUS *((volatile uint16_t *)(baseAddress + (0 * 2)))  /* status register 1xx1 0001 */
#define  ARC_ADR_IMASK  *((volatile uint16_t *)(baseAddress + (0 * 2)))  /* interrupt mask register 0000 0000 */
#define  ARC_ADR_DIAG   *((volatile uint16_t *)(baseAddress + (1 * 2)))  /* diagnostic resistor 0000 000x */
#define  ARC_ADR_CMD    *((volatile uint16_t *)(baseAddress + (1 * 2)))  /* command register */
#define  ARC_ADR_ADR_HIGH  *((volatile uint16_t *)(baseAddress + (2 * 2)))  /* address pointer (upper) */
#define  ARC_ADR_ADR_LOW  *((volatile uint16_t *)(baseAddress + (3 * 2)))  /* address pointer (lower) */
#define  ARC_ADR_ADR_U  *((volatile uint16_t *)(baseAddress + (2 * 2)))  /* address pointer (upper) */
#define  ARC_ADR_ADR_D  *((volatile uint16_t *)(baseAddress + (3 * 2)))  /* address pointer (lower) */
#define  ARC_ADR_DATA   *((volatile uint16_t *)(baseAddress + (4 * 2)))  /* data register */
#define  ARC_ADR_SUB_ADR *((volatile uint16_t *)(baseAddress + (5 * 2)))  /* sub address register */
#define  ARC_ADR_CONFIG *((volatile uint16_t *)(baseAddress + (6 * 2)))  /* configuration register 0001 1000 */
#define  ARC_ADR_TENTID *((volatile uint16_t *)(baseAddress + (7 * 2)))  /* TENT-ID register 0000 0000 */
#define  ARC_ADR_NODEID *((volatile uint16_t *)(baseAddress + (7 * 2)))  /* NODE-ID register 0000 0000 */
#define  ARC_ADR_SETUP1 *((volatile uint16_t *)(baseAddress + (7 * 2)))  /* SETUP1 register 0000 0000 */
#define  ARC_ADR_SETUP2 *((volatile uint16_t *)(baseAddress + (7 * 2)))  /* SETUP2 register 0000 0000 */
#define  ARC_ADR_NEXTID *((volatile uint16_t *)(baseAddress + (7 * 2)))  /* NEXT-ID register 0000 0000 */
#define  ARC_ADR_TEST   *((volatile uint16_t *)(baseAddress + (7 * 2)))  /* TEST register */
#define  ARC_ADR_BUSCTRL *((volatile uint16_t *)(baseAddress + (7 * 2)))  /* BUS CONTROL register */

#define  ARC_REG_STATUS_RI_TRI  0x80  /**/
#define  ARC_REG_STATUS_X_RI    0x40  /**/
#define  ARC_REG_STATUS_X_TA    0x20  /**/
#define  ARC_REG_STATUS_POR     0x10  /**/
#define  ARC_REG_STATUS_TEST    0x08  /**/
#define  ARC_REG_STATUS_RECON   0x04  /**/
#define  ARC_REG_STATUS_TMA     0x02  /**/
#define  ARC_REG_STATUS_TA_TTA  0x01  /**/

#define  ARC_REG_IMASK_RI_TRI      0x80  /**/
#define  ARC_REG_IMASK_EXCNAK      0x08  /**/
#define  ARC_REG_IMASK_RECON       0x04  /**/
#define  ARC_REG_IMASK_NEW_NEXTID  0x02  /**/
#define  ARC_REG_IMASK_TA_TTA      0x01  /**/
#define  ARC_REG_IMASK_MASK  (ARC_REG_IMASK_RI_TRI | ARC_REG_IMASK_EXCNAK | ARC_REG_IMASK_RECON | ARC_REG_IMASK_NEW_NEXTID | ARC_REG_IMASK_TA_TTA)

#define  ARC_REG_DIAG_MY_RECON   0x80  /**/
#define  ARC_REG_DIAG_DUPID      0x40  /**/
#define  ARC_REG_DIAG_RCV_ACT    0x20  /**/
#define  ARC_REG_DIAG_TOKEN      0x10  /**/
#define  ARC_REG_DIAG_EXC_NAK    0x08  /**/
#define  ARC_REG_DIAG_TENTID     0x04  /**/
#define  ARC_REG_DIAG_NEW_NEXTID 0x02  /**/

#define  ARC_REG_ADR_HIGH_RD_DATA  0x80  /**/
#define  ARC_REG_ADR_HIGH_AUTO_INC 0x40  /**/
#define  ARC_REG_ADR_HIGH_MASK  (ARC_REG_ADR_HIGH_RD_DATA | ARC_REG_ADR_HIGH_AUTO_INC | (0x07 << 0))  /**/

#define  ARC_REG_ADR_LOW_MASK   0xFF  /**/

#define  ARC_REG_SUB_ADR_MASK  (0x07 << 0)  /**/

#define  ARC_REG_CONFIG_RESET        0x80  /**/
#define  ARC_REG_CONFIG_CCHEN        0x40  /**/
#define  ARC_REG_CONFIG_TXEN         0x20  /**/
#define  ARC_REG_CONFIG_ET1          0x10  /**/
#define  ARC_REG_CONFIG_ET2          0x08  /**/
#define  ARC_REG_CONFIG_BACKPLANE    0x04  /**/
#define  ARC_REG_CONFIG_SUB_ADR_MASK (0x03 << 0)  /**/

#define  ARC_REG_SETUP1_P1MODE     0x80  /**/
#define  ARC_REG_SETUP1_FOUR_NAKS  0x40  /**/
#define  ARC_REG_SETUP1_RCV_ALL    0x10  /**/
#define  ARC_REG_SETUP1_CKP_MASK   (0x07 << 1)  /**/
#define  ARC_REG_SETUP1_SLOW_ARB   0x01  /**/
#define  ARC_REG_SETUP1_MASK  (ARC_REG_SETUP1_P1MODE | ARC_REG_SETUP1_FOUR_NAKS | ARC_REG_SETUP1_RCV_ALL | ARC_REG_SETUP1_CKP_MASK | ARC_REG_SETUP1_SLOW_ARB)

#define  ARC_REG_SETUP2_RBUS_TMG     0x80  /**/
#define  ARC_REG_SETUP2_CKUP_MASK    (0x03 << 4)  /**/
#define  ARC_REG_SETUP2_EF           0x08  /**/
#define  ARC_REG_SETUP2_NO_SYNC      0x04  /**/
#define  ARC_REG_SETUP2_RCN_TM_MASK  (0x03 << 0)  /**/
#define  ARC_REG_SETUP2_MASK  (ARC_REG_SETUP2_RBUS_TMG | ARC_REG_SETUP2_CKUP_MASK | ARC_REG_SETUP2_EF | ARC_REG_SETUP2_NO_SYNC | ARC_REG_SETUP2_RCN_TM_MASK)

#define  ARC_REG_BUSCTRL_W16  0x80  /**/
#define  ARC_REG_BUSCTRL_MASK  (ARC_REG_BUSCTRL_W16)

#define  STATUS_READ()        ARC_ADR_STATUS
#define  IMR_WRITE(mask)      do{ ARC_ADR_IMASK = mask; }while(0)
#define  DIAG_READ()          ARC_ADR_DIAG
#define  COMMAND_WRITE(cmd)   do{ ARC_ADR_CMD = cmd; }while(0)
#define  ADR_HIGH_READ()      ARC_ADR_ADR_HIGH
#define  ADR_HIGH_WRITE(adr)  do{ ARC_ADR_ADR_HIGH = adr; }while(0)
#define  ADR_LOW_READ()       ARC_ADR_ADR_LOW
#define  ADR_LOW_WRITE(adr)   do{ ARC_ADR_ADR_LOW = adr; }while(0)
#define  DATA_READ()          ARC_ADR_DATA
#define  DATA_WRITE(dat)      do{ ARC_ADR_DATA = dat; }while(0)
#define  SUB_READ()           ARC_ADR_SUB_ADR
#define  SUB_WRITE(adr)       do{ ARC_ADR_SUB_ADR = adr; }while(0)
#define  CONFIG_READ()        ARC_ADR_CONFIG
#define  CONFIG_WRITE(cfg)    do{ ARC_ADR_CONFIG = cfg; }while(0)
#define  TENTID_READ()        ARC_ADR_TENTID
#define  TENTID_WRITE(tid)    do{ ARC_ADR_TENTID = tid; }while(0)
#define  NODEID_READ()        ARC_ADR_NODEID
#define  NODEID_WRITE(nid)    do{ ARC_ADR_NODEID = nid; }while(0)
#define  SETUP1_READ()        ARC_ADR_SETUP1
#define  SETUP1_WRITE(stp1)   do{ ARC_ADR_SETUP1 = stp1; }while(0)
#define  SETUP2_READ()        ARC_ADR_SETUP2
#define  SETUP2_WRITE(stp2)   do{ ARC_ADR_SETUP2 = stp2; }while(0)
#define  NEXTID_READ()        ARC_ADR_NEXTID
#define  TEST_WRITE(tst)      do{ ARC_ADR_TEST = tst; }while(0)
#define  BUSCTRL_WRITE(ctl)   do{ ARC_ADR_BUSCTRL = ctl; }while(0)

/*
  command type.
*/
#define  ARC_CMD_CLEAR_TRANSMIT_INTERRUPT       (0x00)
#define  ARC_CMD_DISABLE_TRANSMITTER            (0x01)
#define  ARC_CMD_DISABLE_RECIEVER               (0x02)
#define  ARC_CMD_ENABLE_RECIEVE_PAGE_fnn(b,f,nn) (0x04 | (b << 7) | (f << 5) | (nn << 3))
#define  ARC_CMD_ENABLE_TRANSMIT_PAGE_fnn(f,nn)  (0x03 | (f << 5) | (nn << 3))
#define  ARC_CMD_DEFINE_CONFIGURATION(c)        (0x05 | (c << 3))
#define  ARC_CMD_CLEAR_FLAGS(r,p)               (0x06 | (r << 4) | (p << 3))
#define  ARC_CMD_CLEAR_RECIEVE_INTERRUPT        (0x08)
#define  ARC_CMD_START_INTERNAL_OPERATION       (0x18)

class ARCNET
{
private:
  bool is16bit;  /* 16 bit data bus width? */
  bool isCOM20022;  /* 16 bit data bus width? */
  volatile void *baseAddress;
//  volatile uint8_t  imask;
  uint8_t  rcvPageW,rcvPageR;
  uint32_t extiPin;  /* exti pin number. */
  uint32_t nodeMap[ 256 / 32 ];  /* node map */

  void setNodeMap( int tid )
  {
    int col = tid / 32;
    int row = tid % 32;
    uint32_t tempUL = nodeMap[ col ];
    tempUL |= 1 << row;
    nodeMap[ col ] = tempUL;
  }

  void clrNodeMap( int tid )
  {
    int col = tid / 32;
    int row = tid % 32;
    uint32_t tempUL = nodeMap[ col ];
    tempUL &= (1 << row) ^ 0xFFFFFFFF;
    nodeMap[ col ] = tempUL;
  }

  bool isNode( int tid )
  {
    int col = tid / 32;
    int row = tid % 32;
    uint32_t tempUL = 1 << row;
    return ( nodeMap[ col ] & tempUL ) ? true : false;
  }


  uint8_t STATUS(){ return (uint8_t)STATUS_READ(); }  /* ADDR:00 default value is 1xx1 0001. */
  void IMR( uint8_t imr ){ IMR_WRITE(imr & ARC_REG_IMASK_MASK); }  /* ADDR:00 default value is 0. */

  uint8_t DIAG(){ return (uint8_t)DIAG_READ(); }  /* ADDR:01 default value is 0000 000x. */
  void COMMAND( uint8_t cmd ){ COMMAND_WRITE( cmd ); }  /* ADDR:01 */

  uint16_t ADDRESS()  /* ADDR:02,03 */
  {
    uint16_t tempH = ADR_HIGH_READ() & ARC_REG_ADR_HIGH_MASK;
    uint16_t tempL = ADR_LOW_READ() & ARC_REG_ADR_LOW_MASK;
    return tempH << 8 | tempL;
  }
  void ADDRESS( uint16_t adr )  /* ADDR:02,03 */
  {
    ADR_HIGH_WRITE( (adr >> 8) & ARC_REG_ADR_HIGH_MASK );
    ADR_LOW_WRITE( adr & ARC_REG_ADR_LOW_MASK );
  }

  uint16_t DATA(){ return DATA_READ(); }  /* ADDR:04 */
  void DATA( uint16_t dat ){ DATA_WRITE( dat ); }  /* ADDR:04 */

  uint8_t SUB_ADR(){ return (uint8_t)(SUB_READ() & ARC_REG_SUB_ADR_MASK); }  /* ADDR:05 */
  void SUB_ADR( uint8_t adr ){ SUB_WRITE( adr & ARC_REG_SUB_ADR_MASK ); }  /* ADDR:05 */

  uint8_t CONFIG(){ return (uint8_t)CONFIG_READ(); }  /* ADDR:06 default value is 0001 1000. */
  void CONFIG( uint8_t cfg ){ CONFIG_WRITE( cfg ); }  /* ADDR:06 */

  uint8_t TENTID(){ SUB_ADR(0); return (uint8_t)TENTID_READ(); }  /* ADDR:07-0 default value is 0. */
  void TENTID( uint8_t tid ){ SUB_ADR(0); TENTID_WRITE( tid ); }  /* ADDR:07-0 */

  uint8_t NODEID(){ SUB_ADR(1); return (uint8_t)NODEID_READ(); }  /* ADDR:07-1 default value is 0. */
  void NODEID( uint8_t nid ){ SUB_ADR(1); NODEID_WRITE( nid ); }  /* ADDR:07-1 */

  uint8_t SETUP1(){ SUB_ADR(2); return (uint8_t)SETUP1_READ(); }  /* ADDR:07-2 default value is 0. */
  void SETUP1( uint8_t stp1 ){ SUB_ADR(2); SETUP1_WRITE( stp1 & ARC_REG_SETUP1_MASK ); }  /* ADDR:07-2 */

  uint8_t NEXTID(){ SUB_ADR(3); return (uint8_t)NEXTID_READ(); }  /* ADDR:07-3 default value is 0. */
  void TEST(){ SUB_ADR(3); TEST_WRITE( 0 ); }  /* ADDR:07-3 */

  uint8_t SETUP2(){ SUB_ADR(4); return (uint8_t)SETUP2_READ(); }  /* ADDR:07-4 */
  void SETUP2( uint8_t stp2 ){ SUB_ADR(4); SETUP2_WRITE( stp2 & ARC_REG_SETUP2_MASK ); }  /* ADDR:07-4 */

  void BUSCTRL( uint8_t ctl ){ SUB_ADR(5); BUSCTRL_WRITE( ctl & ARC_REG_BUSCTRL_MASK ); }  /* ADDR:07-5 */

  /**
    * @brief status check methode.
    * @retval Depends on the type of method.
    */
  uint8_t statusByte() { return STATUS(); }
  bool chk_RI() { return ( STATUS() & ARC_REG_STATUS_RI_TRI ) ? true : false; }
  bool chk_POR() { return ( STATUS() & ARC_REG_STATUS_POR ) ? true : false; }
  bool chk_TEST() { return ( STATUS() & ARC_REG_STATUS_TEST ) ? true : false; }
  bool chk_RECON() { return ( STATUS() & ARC_REG_STATUS_RECON ) ? true : false; }
  bool chk_TMA() { return ( STATUS() & ARC_REG_STATUS_TMA ) ? true : false; }
  bool chk_TA() { return ( STATUS() & ARC_REG_STATUS_TA_TTA ) ? true : false; }

  /**
    * @brief diagnostic status check methode.
    * @retval Depends on the type of method.
    */
  uint8_t diagByte() { return DIAG(); }
  bool chk_MY_RECON(uint8_t diag) { return ( diag & ARC_REG_DIAG_MY_RECON ) ? true : false; }
  bool chk_DUPID(uint8_t diag) { return ( diag & ARC_REG_DIAG_DUPID ) ? true : false; }
  bool chk_RCV_ACT(uint8_t diag) { return ( diag & ARC_REG_DIAG_RCV_ACT ) ? true : false; }
  bool chk_TOKEN(uint8_t diag) { return ( diag & ARC_REG_DIAG_TOKEN ) ? true : false; }
  bool chk_EXC_NAK(uint8_t diag) { return ( diag & ARC_REG_DIAG_EXC_NAK ) ? true : false; }
  bool chk_TENTID(uint8_t diag) { return ( diag & ARC_REG_DIAG_TENTID ) ? true : false; }
  bool chk_NEW_NEXTID(uint8_t diag) { return ( diag & ARC_REG_DIAG_NEW_NEXTID ) ? true : false; }
  bool chk_JOINED()
  {
    uint8_t tempUC = DIAG();
    if( (tempUC & ARC_REG_DIAG_RCV_ACT) && (tempUC & ARC_REG_DIAG_TOKEN) ) return true;
    else return false;
  }


  /**
    * @brief set interrupt mask.
    * @retval none.
    */
  void setIMASK_RI_TRI() { imask |= ARC_REG_IMASK_RI_TRI; /* IMR( imask ); */ }
  void clrIMASK_RI_TRI() { imask &= ~ARC_REG_IMASK_RI_TRI; IMR( imask ); }
  void setIMASK_EXCNAK() { imask |= ARC_REG_IMASK_EXCNAK; /* IMR( imask ); */ }
  void clrIMASK_EXCNAK() { imask &= ~ARC_REG_IMASK_EXCNAK; IMR( imask ); }
  void setIMASK_RECON() { imask |= ARC_REG_IMASK_RECON; /* IMR( imask ); */ }
  void clrIMASK_RECON() { imask &= ~ARC_REG_IMASK_RECON; IMR( imask ); }
  void setIMASK_NEW_NEXTID() { imask |= ARC_REG_IMASK_NEW_NEXTID; /* IMR( imask ); */ }
  void clrIMASK_NEW_NEXTID() { imask &= ~ARC_REG_IMASK_NEW_NEXTID; IMR( imask ); }
  void setIMASK_TA_TTA() { imask |= ARC_REG_IMASK_TA_TTA; /* IMR( imask ); */ }
  void clrIMASK_TA_TTA() { imask &= ~ARC_REG_IMASK_TA_TTA; IMR( imask ); }

  /**
    * @brief command methode.
    * @retval Depends on the type of method.
    */
  void clrTxInt() { COMMAND( ARC_CMD_CLEAR_TRANSMIT_INTERRUPT ); }
  void disTransmitter() { COMMAND( ARC_CMD_DISABLE_TRANSMITTER ); }
  void disReciever() { COMMAND( ARC_CMD_DISABLE_RECIEVER ); }
  void enaRcvPage( bool b = false, bool f = true )
  {
    uint8_t tempUC = 0x04;
    tempUC |= (b) ? 0x80 : 0x00;
    tempUC |= (f) ? 0x20 : 0x00;
    tempUC |= (rcvPageW & 0x03) << 3;
    COMMAND( tempUC );
  }
  void enaTraPage( int size )
  {
    /* short packet */
    if( size <= (256 - 3) ) { COMMAND( ARC_CMD_ENABLE_TRANSMIT_PAGE_fnn( 0, (COM20022_ENTRY_TRANSMIT_PAGE & 0x03) ) ); }
    /* long packet */
    else { COMMAND( ARC_CMD_ENABLE_TRANSMIT_PAGE_fnn( 1, (COM20022_ENTRY_TRANSMIT_PAGE & 0x03) ) ); }
  }
  void defConfig( bool c )
  {
    /* allow long and short packet. */
    if( c ) { COMMAND( ARC_CMD_DEFINE_CONFIGURATION( 1 ) ); }
    /* only short packet */
    else { COMMAND( ARC_CMD_DEFINE_CONFIGURATION( 0 ) ); }
  }
  void clrReconFlag() { COMMAND( ARC_CMD_CLEAR_FLAGS( 1, 0 ) ); }
  void clrPORFlag() { COMMAND( ARC_CMD_CLEAR_FLAGS( 0, 1 ) ); }
  void clrRxInt() { COMMAND( ARC_CMD_CLEAR_RECIEVE_INTERRUPT ); }
  void startIntOperation() { COMMAND( ARC_CMD_START_INTERNAL_OPERATION ); }

  /**
    * @brief configuration methode.
    * @retval Depends on the type of method.
    */
  uint8_t configByte() { return CONFIG(); }
  void softReset() { uint8_t tempUC = CONFIG(); tempUC |= ARC_REG_CONFIG_RESET; CONFIG( tempUC ); }
  void softResetRelease() { uint8_t tempUC = CONFIG(); tempUC &= ~ARC_REG_CONFIG_RESET; CONFIG( tempUC ); }
  void cmdChainEnable() { uint8_t tempUC = CONFIG(); tempUC |= ARC_REG_CONFIG_CCHEN; CONFIG( tempUC ); }
  void cmdChainDisable() { uint8_t tempUC = CONFIG(); tempUC &= ~ARC_REG_CONFIG_CCHEN; CONFIG( tempUC ); }
  void txEnable() { uint8_t tempUC = CONFIG(); tempUC |= ARC_REG_CONFIG_TXEN; CONFIG( tempUC ); }
  void txDisable() { uint8_t tempUC = CONFIG(); tempUC &= ~ARC_REG_CONFIG_TXEN; CONFIG( tempUC ); }
  void extTimeout( uint8_t et )
  {
    uint8_t tempUC = CONFIG();
    tempUC &= ~(ARC_REG_CONFIG_ET1 | ARC_REG_CONFIG_ET2);
    tempUC |= (et & 0x03) << 3;
    CONFIG( tempUC );
  }
  void backPlaneEnable() { uint8_t tempUC = CONFIG(); tempUC |= ARC_REG_CONFIG_BACKPLANE; CONFIG( tempUC ); }
  void backPlaneDisable() { uint8_t tempUC = CONFIG(); tempUC &= ~ARC_REG_CONFIG_BACKPLANE; CONFIG( tempUC ); }
  void subAddress( uint8_t adr )
  {
    if( isCOM20022 ) { SUB_ADR( adr ); }
    else
    {
      uint8_t tempUC = CONFIG();
      tempUC &= ~ARC_REG_CONFIG_SUB_ADR_MASK;
      tempUC |= adr & 0x03;
      CONFIG( tempUC );
    }
  }

  /**
    * @brief setup1 register methode.
    * @retval none.
    */
  void p1mode( bool md )
  {
    uint8_t tempUC = SETUP1() & ~ARC_REG_SETUP1_P1MODE;
    if( md ) { tempUC |= ARC_REG_SETUP1_P1MODE; }
    SETUP1( tempUC );
  }
  void fourNacks( bool nak )
  {
    uint8_t tempUC = SETUP1() & ~ARC_REG_SETUP1_FOUR_NAKS;
    if( nak ) { tempUC |= ARC_REG_SETUP1_FOUR_NAKS; }
    SETUP1( tempUC );
  }
  void rcvAll( bool rcv )
  {
    uint8_t tempUC = SETUP1() & ~ARC_REG_SETUP1_RCV_ALL;
    if( rcv ) { tempUC |= ARC_REG_SETUP1_RCV_ALL; }
    SETUP1( tempUC );
  }
  void ckp( uint8_t baud )
  {
    uint8_t tempUC = SETUP1() & ~ARC_REG_SETUP1_CKP_MASK;
    tempUC |= (baud << 1) & ARC_REG_SETUP1_CKP_MASK;
    SETUP1( tempUC );
  }
  void slowArb( bool arb )
  {
    uint8_t tempUC = SETUP1() & ~ARC_REG_SETUP1_SLOW_ARB;
    if( arb ) { tempUC |= ARC_REG_SETUP1_SLOW_ARB; }
    SETUP1( tempUC );
  }

  /**
    * @brief setup2 register methode.
    * @retval none.
    */
  void rbusTMG( bool tmg )
  {
    uint8_t tempUC = SETUP2() & ~ARC_REG_SETUP2_RBUS_TMG;
    if( tmg ) { tempUC |= ARC_REG_SETUP2_RBUS_TMG; }
    SETUP2( tempUC );
  }
  void ckup( uint8_t ck )
  {
    uint8_t tempUC = SETUP2() & ~ARC_REG_SETUP2_CKUP_MASK;
    tempUC |= (ck << 4) & ARC_REG_SETUP2_CKUP_MASK;
    SETUP2( tempUC );
  }
  void EF( bool ef )
  {
    uint8_t tempUC = SETUP2() & ~ARC_REG_SETUP2_EF;
    if( ef ) { tempUC |= ARC_REG_SETUP2_EF; }
    SETUP2( tempUC );
  }
  void noSync( bool sync )
  {
    uint8_t tempUC = SETUP2() & ~ARC_REG_SETUP2_NO_SYNC;
    if( sync ) { tempUC |= ARC_REG_SETUP2_NO_SYNC; }
    SETUP2( tempUC );
  }
  void rcnTM( uint8_t tm )
  {
    uint8_t tempUC = SETUP2() & ~ARC_REG_SETUP2_RCN_TM_MASK;
    tempUC |= (tm << 0) & ARC_REG_SETUP2_RCN_TM_MASK;
    SETUP2( tempUC );
  }

  uint16_t read();
  uint16_t read( uint16_t adr );
  int  read( uint16_t adr, uint8_t *dat, int size );
  int  read( uint16_t adr, uint16_t *dat, int size );
  void write( uint16_t dat );
  void write( uint16_t adr, uint16_t dat );
  int  write( uint16_t adr, const uint8_t *dat, int size );
  int  write( uint16_t adr, const uint16_t *dat, int size );
  int  pageRead( int page, uint8_t *sid, uint8_t *dat, int size );
  int  pageWrite( int page, uint8_t did, const uint8_t *dat, int size );
  int  putIntoReceiveState();

  /* for debug use. */
  int  memCheck( uint8_t *buf, int size );
  int  memCheck( uint16_t *buf, int size );
  void memClear();

public:
  volatile uint8_t imask;
  volatile uint8_t next_id;
  uint32_t transmit_count,transmit_complete_count;
  uint32_t recieve_count,recieve_complete_count;
  uint32_t reconfig_count,my_reconfig_count;

  ARCNET() {}
  ARCNET( volatile void *a, uint32_t _p, bool c = true, bool b = true )
  {
    config( a, _p, c, b );
  }
  ~ARCNET() {}
  void config( volatile void *a, uint32_t _p, bool c = true, bool b = true )
  {
    baseAddress = a; extiPin = _p; isCOM20022 = c; is16bit = b;
  }

  int  begin( uint8_t nid, uint8_t baud = ARC_BAUD_2M5, uint8_t num = 255 );
  void end() { detachInterrupt( extiPin ); }
  uint8_t nodeid() { return NODEID(); }
  uint8_t nextid() { return NEXTID(); }
  bool joined() { return chk_JOINED(); }

  int  send( uint8_t did, const uint8_t *dat, int size );
  int  recv( uint8_t *sid, uint8_t *dat, int size );
//  uint8_t available() { return rcvPageW - rcvPageR; }
  uint32_t available() { return recieve_count - recieve_complete_count; }

  uint8_t chipRevision();
  void regCBfunction( void (*cb)(void) );

  void enaInterrupt() { IMR( imask ); }
  void disInterrupt() { IMR( 0 ); }
  void intHandler();
};

#endif  /* __ARCNET_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
