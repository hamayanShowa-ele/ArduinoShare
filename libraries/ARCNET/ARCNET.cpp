/**
  ******************************************************************************
  * @file           : ARCNET.c
  * @brief          : ARCNET code
  */
/* Includes ------------------------------------------------------------------*/
#include "ARCNET.h"

/**
  * @brief begin.
  * @retval none.
  */
int ARCNET::begin( uint8_t nid, uint8_t baud, uint8_t node_number )
{
  rcvPageW = rcvPageR = COM20022_ENTRY_RECIEVE_PAGE;
  transmit_count = transmit_complete_count = 0;
  recieve_count = recieve_complete_count = 0;
  reconfig_count = my_reconfig_count = 0;
  next_id = 0;

  volatile uint16_t tempUS;
  /* clear node map. */
  for( int i = 0; i < (int)(sizeof(nodeMap) / sizeof(nodeMap[0])); i++ ) { nodeMap[i] = 0UL; }
  /* dummy read status. */
  tempUS = STATUS();
  /* select data bus type. */
  BUSCTRL( (is16bit) ? ARC_REG_BUSCTRL_W16 : ARC_REG_BUSCTRL_W08 );
  /* Software resetting. */
  softReset();
  /* transmit disable. */
  txDisable();
  /* Write self-node ID. */
  NODEID( nid ); delay( 1 );
  if( NODEID() != nid ) return ARC_ERR_BEGIN_0;
  /* Set the communication speed 2.5Mbps. */
  p1mode( true ); ckp( (baud >= ARC_BAUD_5M) ? 0 : baud );  // default baud = ARC_BAUD_2M5
  /* setup2 register change. */
  EF( (isCOM20022) ? true : false );  // enhanced functions.
  rbusTMG( false );  // not high speed CPU BUS.
  // noSync( false );  // default
  if( node_number <= 16 ) rcnTM( ARC_RCNTM_13m125 );
  else if( node_number <= 32 ) rcnTM( ARC_RCNTM_26m25 );
  else if( node_number <= 64 ) rcnTM( ARC_RCNTM_52m5 );
  else rcnTM( ARC_RCNTM_210m );  // RECON period at 10 Mbps
  /* backplane mode. */
//  backPlaneDisable();  // backPlaneEnable();
  ckup( (baud == ARC_BAUD_2M5) ? ARC_CLOCK_20M :
        (baud == ARC_BAUD_5M) ? ARC_CLOCK_40M : ARC_CLOCK_80M );  // not multiply PLL. up to 2.5Mbps
  delay( 1 );
  /* Interrupt mask. */
  imask = ARC_REG_IMASK_EXCNAK | ARC_REG_IMASK_RECON;
//  IMR( imask );  // EXCNAK and RECON except RI/TR1 and NEW NEXTID and TA/TTA
  /* Software reset release and start ARCNET. */
  softResetRelease();	delay( 2 );
  /* start internal operations. */
  startIntOperation();
  /* Confirmation of ARCNET operation. */
  delay( 350 );
  ADDRESS( 0xC000 );
  if( (DATA() & 0x00FF) != 0x00D1 ) return ARC_ERR_BEGIN_1;
  if( (uint8_t)DATA() != nid ) return ARC_ERR_BEGIN_2;
  /* flag test. */
  COMMAND( 0x07 | 0x08 );
  if( !chk_TEST() ) return ARC_ERR_BEGIN_3;
  COMMAND( 0x07 | 0x00 );
  if( chk_TEST() ) return ARC_ERR_BEGIN_4;
  /* Allow LONG packets in packet format. */
  defConfig( true );
  /* Internal RAM check. */
  const int size = COM20022_BUFFER_SIZE;
  uint8_t *buf = new uint8_t[ COM20022_BUFFER_SIZE ];
  if( memCheck( buf, size ) != 0 ) { delete [] buf; return ARC_ERR_BEGIN_5; }
  delete [] buf;
  /* Internal RAM clear. */
  memClear();
  /* Clear FLAGS. */
  COMMAND( 0x1E );  // clear POR and EXCNAK flags.
  /* Receiver initialization. */
  enaRcvPage();  // 
  for( int i = 0; i < 100; i++ )
  {
    if( !chk_RI() ) break;
    if( i == 99 ) return ARC_ERR_BEGIN_6;
    delay( 10 );
  }
  /* The RAM initialization sequence is now complete. */
  noSync( true );
  /* ready for recieve. */
  putIntoReceiveState();
  /* transmit enable. */
  txEnable();

  return 0;
}

/**
  * @brief read.
  * @retval The number of data actually read.
  */
uint16_t ARCNET::read() { return DATA(); }
uint16_t ARCNET::read( uint16_t adr )
{
  ADDRESS( (ARC_REG_ADR_HIGH_RD_DATA << 8) | adr );  /* not address increment. */
  return DATA();
}
int ARCNET::read( uint16_t adr, uint8_t *dat, int size )
{
  ADDRESS( ((ARC_REG_ADR_HIGH_RD_DATA | ARC_REG_ADR_HIGH_AUTO_INC) << 8) | adr );  /* address increment. */
  int loop = size;
  for( int i = 0; i < loop; i++ ) { *dat++ = (uint8_t)DATA_READ(); }  // DATA_READ()
  return size;
}
int ARCNET::read( uint16_t adr, uint16_t *dat, int size )
{
  ADDRESS( ((ARC_REG_ADR_HIGH_RD_DATA | ARC_REG_ADR_HIGH_AUTO_INC) << 8) | adr );  /* address increment. */
  int loop = size / 2;
  for( int i = 0; i < loop; i++ ) { *dat++ = DATA_READ(); }  // DATA_READ()
  return size;
}

/**
  * @brief write.
  * @retval The number of data actually written.
  */
void ARCNET::write( uint16_t dat ) { DATA( dat ); }
void ARCNET::write( uint16_t adr, uint16_t dat )
{
  ADDRESS( adr );  /* not address increment. */
  DATA( dat );
}
int ARCNET::write( uint16_t adr, const uint8_t *dat, int size )
{
  ADDRESS( (ARC_REG_ADR_HIGH_AUTO_INC << 8) | adr );  /* address increment. */
  int loop = size;
  for( int i = 0; i < loop; i++ ) { DATA_WRITE( *dat++ ); }
  return size;
}
int ARCNET::write( uint16_t adr, const uint16_t *dat, int size )
{
  ADDRESS( (ARC_REG_ADR_HIGH_AUTO_INC << 8) | adr );  /* address increment. */
  int loop = size / 2;
  for( int i = 0; i < loop; i++ ) { DATA_WRITE( *dat++ ); }
  return size;
}

/**
  * @brief page write.
  * Since the packet size is 1-253 bytes when short, and 257-508 bytes when long,
  *  0-padding must be performed when the packet size is 254-256 bytes.
  * @retval count value or A negative value indicating an illegal termination..
  */
int ARCNET::pageWrite( int page, uint8_t did, const uint8_t *dat, int size )
{
  if( size == 0 ) { return 0; }
  else if( size > (512 - 4) ) { size = 512 - 4; }

  uint16_t adr;
  if( page == ARC_PAGE_0 ) adr = (512 * 0);
  else if( page == ARC_PAGE_1 ) adr = (512 * 1);
  else if( page == ARC_PAGE_2 ) adr = (512 * 2);
  else if( page == ARC_PAGE_3 ) adr = (512 * 3);
  else return ARC_ERR_PAGE_WRITE_0;

  /* set source id. */
  uint8_t sid = NODEID();
  write( adr + 0, sid );
  /* set destination id. */
  write( adr + 1, did );
  /* set count. */
  int count,limit;
  if( size > (256 - 3) )  /* long packet. */
  {
    limit = 512;
    count = limit - size;
    write( adr + 2, 0 );
    write( adr + 3, (uint8_t)count );
  }
  else  /* short packet. */
  {
    limit = 256;
    count = limit - size;
    write( adr + 2, (uint8_t)count );
  }
  ADDRESS( (ARC_REG_ADR_HIGH_AUTO_INC << 8) + adr + count );
  for( int i = count; i < limit; i++ ) { write( *dat++ ); }

  return size;
}

/**
  * @brief page read.
  * @retval count value or A negative value indicating an illegal termination..
  */
int ARCNET::pageRead( int page, uint8_t *sid, uint8_t *dat, int size )
{
  if( size == 0 ) return 0;
  if( size > (512 - 4) ) size = 512 - 4;

  uint16_t adr;
  if( page == ARC_PAGE_0 ) adr = (512 * 0);
  else if( page == ARC_PAGE_1 ) adr = (512 * 1);
  else if( page == ARC_PAGE_2 ) adr = (512 * 2);
  else if( page == ARC_PAGE_3 ) adr = (512 * 3);
  else return ARC_ERR_PAGE_READ_0;

  uint8_t source,dest,cnt0,cnt1;
  /* get source id. */
  source = (uint8_t)read( adr + 0 );
  if( source == 0 ) { adr += 256; source = (uint8_t)read( adr + 0 ); }
  /* get destination id. */
  dest = (uint8_t)read( adr + 1 );
  /* get count 0. */
  cnt0 = (uint8_t)read( adr + 2 );
  /* get count 1. */
  cnt1 = (uint8_t)read( adr + 3 );

  int count,limit;
  if( cnt0 == 0 )  /* long packet. */
  {
    limit = 512;
    count = cnt1;
    if( count >= 0 && count <= 2 ) { count += 256; }
  }
  else  /* short packet. */
  {
    limit = 256;
    count = cnt0;
  }
  /* get data. */
  uint8_t *ptr = dat;
  ADDRESS( ((ARC_REG_ADR_HIGH_RD_DATA + ARC_REG_ADR_HIGH_AUTO_INC) << 8) + adr + count );
  for( int i = count; i < limit; i++ ) { *ptr++ = (uint8_t)read(); }

  *sid = source;
#if 0
  Serial.print( "pageRead adr = " ); Serial.println( adr, DEC );
  Serial.print( "pageRead sid = " ); Serial.print( *sid, DEC );
  Serial.print( " did = " ); Serial.print( dest, DEC );
  Serial.print( " cnt0 = " ); Serial.print( cnt0, DEC );
  Serial.print( " cnt1 = " ); Serial.print( cnt1, DEC );
  Serial.print( " count = " ); Serial.println( count, DEC );
#endif
  return limit - count;
}

/**
  * @brief send data.
  * @retval size is success, not 0 is failed.
  */
int ARCNET::send( uint8_t did, const uint8_t *dat, int size )
{
  /* Set up the outgoing packets in advance. */
  int ret = pageWrite( COM20022_ENTRY_TRANSMIT_PAGE, did, dat, size );
  if( ret <= 0 ) return ret;
  /* Since we don't know what kind of interrupt is occurring, we mask all interrupt sources. */
  clrIMASK_TA_TTA();  // imask = 0; IMR( imask );
  /* waitting for TA(Transmitter available). */
  uint32_t baseTim = millis();
  while( !chk_TA() ) { delay( 1 ); if( (millis() - baseTim) >= 100UL ) return ARC_ERR_SEND_0; }
  /* Set up the sending page and start sending. */
  enaTraPage( size );
  /* After the command takes effect, confirm that TA has changed to 0. */
  baseTim = millis();
  while( chk_TA() ) { delay( 1 ); if( (millis() - baseTim) >= 100UL ) return ARC_ERR_SEND_1; }
  /* In order to expect an interrupt due to a change in the TA bit, only the TA bit is unmasked. */
  uint32_t transmit_count_Base = transmit_complete_count;
  setIMASK_TA_TTA();
  enaInterrupt();
  /* Wait for transmission completion. */
  baseTim = millis();
  while( transmit_complete_count == transmit_count_Base )
  {
    delay( 1 );
    if( (millis() - baseTim) >= 100UL ) return ARC_ERR_SEND_2;
  }
  transmit_count++;

  return size;
}

/**
  * @brief Put it into receive state.
  * @retval If successful, 0 is returned. If it fails, a negative value is returned..
  */
int ARCNET::putIntoReceiveState()
{
  /* Since we don't know what kind of interrupt is occurring, we mask all interrupt sources. */
  clrIMASK_RI_TRI();  // imask = 0; IMR( imask );
  /* waitting for RI(The receive command is complete.). */
  uint32_t baseTim = millis();
//  while( !chk_RI() ) { delay( 1 ); if( (millis() - baseTim) >= 100UL ) return ARC_ERR_RECV_0; }
  /* Issue a receive command. */
  enaRcvPage();
  /* After issuing the receive command, confirm that the RI is cleared. */
  baseTim = millis();
  while( chk_RI() ) { delay( 1 ); if( (millis() - baseTim) >= 100UL ) return ARC_ERR_RECV_1; }
  /* In order to expect an interrupt due to a change in the RI bit, only the RI bit is unmasked. */
  setIMASK_RI_TRI();

  return 0;
}

/**
  * @brief recieve data.
  * @retval Returns the number of data retrieved.
  */
int ARCNET::recv( uint8_t *sid, uint8_t *dat, int size )
{
  /* Verify that it has been received. */
//  if( rcvPageW == rcvPageR ) return 0;
  if( recieve_count == recieve_complete_count ) return 0;
  /* page read. */
  int ret = pageRead( rcvPageR, sid, dat, size );
  if( ret <= 0 ) return ret;
  /* Proceed to the next page. */
  if( ++rcvPageR > COM20022_END_RECIEVE_PAGE ) rcvPageR = COM20022_ENTRY_RECIEVE_PAGE;
  recieve_complete_count++;
  return ret;
}

/**
  * @brief chip revision.
  * @retval 0xC0 = Rev.C, 0x80 = Rev.B, other = failed.
  */
uint8_t ARCNET::chipRevision()
{
  uint8_t rev;
  SUB_WRITE( 0 );
  rev = SUB_READ();
  SUB_WRITE( 0xC0 );
  rev = SUB_READ();
  return rev;
}

/**
  * @brief regist call back function.
  * @retval none.
  */
void ARCNET::regCBfunction( void (*cb)(void) )
{
  /* exti interrupt enable. */
  attachEXTI( extiPin, cb, FALLING );
}

/**
  * @brief A process that is called when an interrupt is detected.
  * @retval none.
  */
void ARCNET::intHandler()
{
  uint8_t status = statusByte();
  uint8_t diag = diagByte();

  /* check status register. */
  /* reconfig */
  Serial.print( 'i' );
  if( status & ARC_REG_STATUS_RECON )
  {
    Serial.print( 'c' );
    clrReconFlag();  /* Clearing the RECON bit */
    reconfig_count++;
    if( diag & ARC_REG_DIAG_MY_RECON ) { my_reconfig_count++; }
  }
  /* recieve interrupt. */
  if( status & ARC_REG_STATUS_RI_TRI )
  {
    Serial.print( 'r' );
    clrIMASK_RI_TRI();
    recieve_count++;
    if( ++rcvPageW > COM20022_END_RECIEVE_PAGE ) rcvPageW = COM20022_ENTRY_RECIEVE_PAGE;
    putIntoReceiveState();
    enaInterrupt();
  }
  /* transmit end interrupt. */
  if( status & ARC_REG_STATUS_TA_TTA )
  {
    Serial.print( 't' );
    clrIMASK_TA_TTA();
    if( status & ARC_REG_STATUS_TMA ) { transmit_complete_count++; }
  }

  /* check diagnostic register. */
  if( diag & ARC_REG_DIAG_EXC_NAK )
  {
    Serial.print( 'e' );
    clrPORFlag();  /* clear POR and diagnostioc EXCNAK status bit. */
    disTransmitter();  /* disable transmitter. */
    clrIMASK_TA_TTA();
  }
  if( diag & ARC_REG_DIAG_NEW_NEXTID )
  {
    Serial.print( 'n' );
    next_id = NEXTID();
  }
}

/**
  * @brief memory check.
  * @retval 0 is success, not 0 is failed.
  */
int ARCNET::memCheck( uint8_t *buf, int size )
{
  if( size > COM20022_BUFFER_SIZE ) return ARC_ERR_MEMCHECK_0;
  int ret = 0;

  /* address set and write data. */
  ADDRESS( (ARC_REG_ADR_HIGH_AUTO_INC << 8) | 0 );
  for( int i = 0; i < size; i++ ) { write( 0x0055 ); }
  ADDRESS( (ARC_REG_ADR_HIGH_AUTO_INC << 8) | 0 );
  for( int i = 0; i < size; i++ ) { write( (uint16_t)i ); }

  /* clear buffer. */
  for( int i = 0; i < size; i++ ) { buf[i] = 0; }

  /* address set and read data. */
  ADDRESS( ((ARC_REG_ADR_HIGH_RD_DATA | ARC_REG_ADR_HIGH_AUTO_INC) << 8) | 0 );
  for( int i = 0; i < size; i++ ) { buf[i] = (uint8_t)read(); }

  for( int i = 0; i < size; i++ )
  {
    if( buf[i] != (uint8_t)i ) { ret = (-1); break; }
  }

  return ret;
}

/**
  * @brief memory check.
  * @retval 0 is success, not 0 is failed.
  */
int ARCNET::memCheck( uint16_t *buf, int size )
{
  if( size > COM20022_BUFFER_SIZE ) return ARC_ERR_MEMCHECK_1;
  size /= 2;
  int ret = 0;

  /* address set and write data. */
  ADDRESS( (ARC_REG_ADR_HIGH_AUTO_INC << 8) | 0 );
  for( int i = 0; i < size; i++ ) { write( 0x55AA ); }
  ADDRESS( (ARC_REG_ADR_HIGH_AUTO_INC << 8) | 0 );
  for( int i = 0; i < size; i++ ) { write( (uint16_t)i ); }

  /* clear buffer. */
  for( int i = 0; i < size; i++ ) { buf[i] = 0; }

  /* address set and read data. */
  ADDRESS( ((ARC_REG_ADR_HIGH_RD_DATA | ARC_REG_ADR_HIGH_AUTO_INC) << 8) | 0 );
  for( int i = 0; i < size; i++ ) { buf[i] = read(); }

  for( int i = 0; i < size; i++ )
  {
    if( buf[i] != (uint16_t)i ) { ret = (-1); break; }
  }

  return ret;
}

/**
  * @brief memory clear.
  * @retval none.
  */
void ARCNET::memClear()
{
  /* address set and write data. */
  ADDRESS( (ARC_REG_ADR_HIGH_AUTO_INC << 8) | 0 );
  for( int i = 0; i < COM20022_BUFFER_SIZE; i++ ) { write(0); }
}

extern "C"
{
}  /* extern "C" */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
