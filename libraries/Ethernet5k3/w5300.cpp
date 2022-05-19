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

#include  "w5300.h"

//#if defined(W5300_ETHERNET_SHIELD)

// W5300 controller instance
WizchipClass wizchip;

#if 0
void WizchipClass::cbINT()
{
}
#endif

/**
  * @brief w5300 software reset.
  * @retval none.
  * @todo Operation checks are ongoing.
  */
void WizchipClass::swReset()
{
  uint8_t gw[4],sn[4],sip[4],mac[6];
  *mr |= MR_IND;  // set indirect mode.

  getSHAR( mac );
  getGAR( gw ); getSUBR( sn ); getSIPR( sip );

  *mr |= MR_RST;  // software reset.
  while( *mr & MR_RST ) { delay( 10 ); }  // wait for release reset.
  *mr |= MR_IND;  // set indirect mode.

  setSHAR( (const uint8_t *)mac );
  setGAR( (const uint8_t *)gw );
  setSUBR( (const uint8_t *)sn );
  setSIPR( (const uint8_t *)sip );
}

/**
  * @brief w5300 chip initialize.
  * @param m Address of Mode Register.
  * @param a Address of IDM Address Register.
  * @param d Address of IDM Data Register.
  * @retval none.
  * @todo Operation checks are ongoing.
  */
void WizchipClass::init( volatile uint16_t *m, volatile uint16_t *a, volatile uint16_t *d )
{
  mr = m; idm_ar = a; idm_dr = d;
  for( int i = 0; i < MAX_SOCK_NUM; i++ )
  {
    txByteCount[i] = rxByteCount[i] = 0;
    unReadDataSize[i] = 0UL;
  }
  swReset();
  static const uint8_t tx_memory_size[MAX_SOCK_NUM] = {8,8,8,8,8,8,8,8,};
  static const uint8_t rx_memory_size[MAX_SOCK_NUM] = {8,8,8,8,8,8,8,8,};
  for( int i = 0; i < MAX_SOCK_NUM; i += 2 )
  {
    write( W5300_RMS01R + i, (rx_memory_size[ i ] << 8) | (rx_memory_size[ i + 1 ] << 0) );  // Sn_RXBUF_SIZE
    write( W5300_TMS01R + i, (tx_memory_size[ i ] << 8) | (tx_memory_size[ i + 1 ] << 0) );  // Sn_TXBUF_SIZE
  }
  write( W5300_MTYPER, 0x00FF );  // Memory Type Register
  for( int i = 0; i < MAX_SOCK_NUM; i++ )
  {
    uint16_t tempUS = read( Sn_IMR(i) ) | Sn_IR_RECV;
    write( Sn_IMR(i), tempUS );  // socket interrupt allowed.
  }
  write( W5300_IMR, 0x00FF );  // socket interrupt allowed.
}

/**
  * @brief Writing to the w5300 Register.
  * @param _addr Address of the Register.
  * @param _data Address of the array.
  * @param _len Size of the array in bytes.
  * @retval Return _len.
  * @todo Operation has been checked.
  */
uint32_t WizchipClass::write( uint16_t _addr, const uint8_t *_data, uint32_t _len )
{
  *idm_ar = _addr;
  for( int i = 0; i < (int)_len; i++ )
  {
    uint16_t tempUS = *_data++ << 8;
    if( ++i < (int)_len ) { tempUS |= *_data++; }
    *idm_dr = tempUS;
  }
  return _len;
}

/**
  * @brief Writing to the w5300 Register.
  * @param _addr Address of the Register.
  * @param _cb Data not actually used.
  * @param _data Address of the array.
  * @param _len Size of the array in bytes.
  * @retval Return _len.
  * @todo Operation has been checked.
  */
uint32_t WizchipClass::write(
  uint16_t _addr, [[maybe_unused]] uint8_t _cb, const uint8_t *_data, uint32_t _len )
{
//  (void)_cb;  // use [[maybe_unused]] from C++17
  return write( _addr, _data, _len );
}

/**
  * @brief Read from w5300 Register.
  * @param _addr Address of the Register.
  * @param _data Address of the array.
  * @param _len Size of the array in bytes.
  * @retval Return _len.
  * @todo Operation has been checked.
  */
uint32_t WizchipClass::read( uint16_t _addr, volatile uint8_t *_data, uint32_t _len )
{
  *idm_ar = _addr;
  for( int i = 0; i < (int)_len; i++ )
  {
    uint16_t tempUS = *idm_dr;
    *_data++ = (uint8_t)(tempUS >> 8);
    if( ++i < (int)_len ){ *_data++ = (uint8_t)(tempUS >> 0); }
  }
  return _len;
}

/**
  * @brief Execution of commands on the w5300 socket.
  * @param sn Socket number.
  * @param _cmd Value of command.
  * @retval none.
  * @todo Perhaps Operation has been checked.
  */
void WizchipClass::execCmdSn( SOCKET sn, SockCMD _cmd )
{
  if( sn >= MAX_SOCK_NUM ) return;

  if( _cmd == Sock_SEND && txByteCount[ sn ] & 1 ) { txFifoWrite( sn, 0 ); }
//  else if( _cmd == Sock_RECV && rxByteCount[ sn ] & 1 ) { (void)rxFifoRead( sn ); }

  write( Sn_CR( sn ), _cmd );  // Send command to socket
  while( read( Sn_CR( sn ) ) != 0 ) {}  // Wait for command to complete
}

/**
  * @brief Get w5300's own hardware address.
  * @param mac Array to store hardware addresses.
  * @retval none.
  * @todo Perhaps Operation has been checked.
  */
void WizchipClass::getSHAR( uint8_t *mac )
{
  uint16_t tempUS[3];
  tempUS[0] = read( W5300_SHAR );
  tempUS[1] = read( W5300_SHAR2 );
  tempUS[2] = read( W5300_SHAR4 );

  mac[0] = (uint8_t)(tempUS[0] >> 8);
  mac[1] = (uint8_t)(tempUS[0] >> 0);
  mac[2] = (uint8_t)(tempUS[1] >> 8);
  mac[3] = (uint8_t)(tempUS[1] >> 0);
  mac[4] = (uint8_t)(tempUS[2] >> 8);
  mac[5] = (uint8_t)(tempUS[2] >> 0);
}
void WizchipClass::getMACAddress( uint8_t *_addr ) { getSHAR( _addr ); }

/**
  * @brief Set w5300's own hardware address.
  * @param mac Array in which hardware addresses are stored.
  * @retval none.
  * @todo Perhaps Operation has been checked.
  */
void WizchipClass::setSHAR( const uint8_t *mac )
{
  uint16_t tempUS[3];
  tempUS[0] = (mac[0] << 8) | (mac[1] << 0);
  tempUS[1] = (mac[2] << 8) | (mac[3] << 0);
  tempUS[2] = (mac[4] << 8) | (mac[5] << 0);
  write( W5300_SHAR, tempUS[0] );
  write( W5300_SHAR2, tempUS[1] );
  write( W5300_SHAR4, tempUS[2] );
}
void WizchipClass::setMACAddress( const uint8_t *_addr ) { setSHAR( _addr ); }

/**
  * @brief Get w5300's any ip address.
  * @param ip Array to store ip addresses.
  * @retval none.
  * @todo Perhaps Operation has been checked.
  */
void WizchipClass::getAnyIP( uint16_t adr, uint8_t *ip )
{
  uint16_t tempUS[2];
  tempUS[0] = read( adr );
  tempUS[1] = read( adr + 2 );
  ip[0] = (uint8_t)(tempUS[0] >> 8);
  ip[1] = (uint8_t)(tempUS[0] >> 0);
  ip[2] = (uint8_t)(tempUS[1] >> 8);
  ip[3] = (uint8_t)(tempUS[1] >> 0);
}
void WizchipClass::getGAR( uint8_t *gw ) { getAnyIP( W5300_GAR, gw ); }
void WizchipClass::getSUBR( uint8_t *subr ) { getAnyIP( W5300_SUBR, subr ); }
void WizchipClass::getSIPR( uint8_t *sip ) { getAnyIP( W5300_SIPR, sip ); }
void WizchipClass::getGatewayIp( uint8_t *_addr ) { getGAR( _addr ); }
void WizchipClass::getSubnetMask( uint8_t *_addr ) { getSUBR( _addr ); }
void WizchipClass::getIPAddress( uint8_t *_addr ) { getSIPR( _addr ); }

/**
  * @brief Set w5300's any ip address.
  * @param mac Array in which ip addresses are stored.
  * @retval none.
  * @todo Perhaps Operation has been checked.
  */
void WizchipClass::setAnyIP( uint16_t adr, const uint8_t *ip )
{
  uint16_t tempUS[2];
  tempUS[0] = (ip[0] << 8) | (ip[1] << 0);
  tempUS[1] = (ip[2] << 8) | (ip[3] << 0);
  write( adr, tempUS[0] );
  write( adr + 2, tempUS[1] );
}
void WizchipClass::setGAR( const uint8_t *gw ) { setAnyIP( W5300_GAR, gw ); }
void WizchipClass::setSUBR( const uint8_t *subr ) { setAnyIP( W5300_SUBR, subr ); }
void WizchipClass::setSIPR( const uint8_t *sip ) { setAnyIP( W5300_SIPR, sip ); }
void WizchipClass::setGatewayIp( uint8_t *_addr ) { setGAR( (const uint8_t *)_addr ); }
void WizchipClass::setSubnetMask( uint8_t *_addr ) { setSUBR( (const uint8_t *)_addr ); }
void WizchipClass::setIPAddress( uint8_t *_addr ) { setSIPR( (const uint8_t *)_addr ); }

/**
  * @brief Get the free size of the send buffer of the socket.
  * @param sn Socket number.
  * @retval free size.
  * @todo Operation has been checked.
  */
uint32_t WizchipClass::readSnTX_FSR( SOCKET sn )
{
  uint16_t tempUS0,tempUS1;
  tempUS0 = read( Sn_TX_FSR( sn ) ) & 0x00FF;
  tempUS1 = read( Sn_TX_FSR( sn ) + 2 );
  return (tempUS0 << 16) | (tempUS1 << 0);
}

/**
  * @brief Get the received size of the receive buffer of the socket.
  * @param sn Socket number.
  * @retval received size.
  * @todo Operation has been checked.
  */
uint32_t WizchipClass::readSnRX_RSR( SOCKET sn )
{
  uint16_t tempUS0,tempUS1;
  tempUS0 = read( Sn_RX_RSR( sn ) ) & 0x00FF;
  tempUS1 = read( Sn_RX_RSR( sn ) + 2 );
  return (tempUS0 << 16) | (tempUS1 << 0);
}

/**
  * @brief Get the send size of the socket.
  * @param sn Socket number.
  * @retval send size.
  * @todo Operation checks are ongoing.
  */
uint32_t WizchipClass::readSnTX_WR( SOCKET sn )
{
  uint16_t tempUS0,tempUS1;
  tempUS0 = read( Sn_TX_WRSR( sn ) ) & 0x00FF;
  tempUS1 = read( Sn_TX_WRSR( sn ) + 2 );
  return (tempUS0 << 16) | (tempUS1 << 0);
}

/**
  * @brief Write the send size of the socket.
  * @param sn Socket number.
  * @retval send size.
  * @todo Operation checks are ongoing.
  */
void WizchipClass::writeSnTX_WR( SOCKET sn, uint32_t sz )
{
  sz &= 0x00FFFFFF;
  write( Sn_TX_WRSR( sn ), (uint16_t)(sz >> 16) );
  write( Sn_TX_WRSR( sn ) + 2, (uint16_t)(sz >> 0) );
}

/**
  * @brief Read the size of the transmit buffer of w5300.
  * @param sz An array to which the size is assigned.
  * @retval Assign the size to the array and return it..
  * @todo Operation has been checked.
  */
void WizchipClass::readTxMemorySize( uint16_t *sz )
{
  for( int i = 0; i < MAX_SOCK_NUM; i += 2 )
  {
    *sz++ = read( W5300_TMS01R + i );
  }
}

/**
  * @brief Read the size of the recieve buffer of w5300.
  * @param sz An array to which the size is assigned.
  * @retval Assign the size to the array and return it.
  * @todo Operation has been checked.
  */
void WizchipClass::readRxMemorySize( uint16_t *sz )
{
  for( int i = 0; i < MAX_SOCK_NUM; i += 2 )
  {
    *sz++ = read( W5300_RMS01R + i );
  }
}

/**
  * @brief Write the size of the transmit buffer.
  * @param sn Socket number.
  * @param sz Size to be set.
  * @retval Returns 0 if there is no problem, negative if there is a problem.
  * @todo Operation has been checked.
  */
int WizchipClass::writeTxMemorySize( SOCKET sn, uint8_t sz )
{
  if( sn >= MAX_SOCK_NUM || sz > 64 ) return (-1);
  uint16_t tempUS = read( W5300_TMS01R + (sn & 0x6) );
  if( sn & 1 ) { tempUS &= 0xFF00; tempUS |= sz << 0; }
  else { tempUS &= 0x00FF; tempUS |= sz << 8; }
  write( W5300_TMS01R + (sn & 0x6), tempUS );
  return 0;
}

/**
  * @brief Write the size of the recieve buffer.
  * @param sn Socket number.
  * @param sz Size to be set.
  * @retval Returns 0 if there is no problem, negative if there is a problem.
  * @todo Operation has been checked.
  */
int WizchipClass::writeRxMemorySize( SOCKET sn, uint8_t sz )
{
  if( sn >= MAX_SOCK_NUM || sz > 64 ) return (-1);
  uint16_t tempUS = read( W5300_RMS01R + (sn & 0x6) );
  if( sn & 1 ) { tempUS &= 0xFF00; tempUS |= sz << 0; }
  else { tempUS &= 0x00FF; tempUS |= sz << 8; }
  write( W5300_RMS01R + (sn & 0x6), tempUS );
  return 0;
}

/**
  * @brief Get the free size of the transmit buffer.
  * @param sn Socket number.
  * @retval Free size of the transmit buffer.
  * @todo Operation has been checked.
  */
uint32_t WizchipClass::getTXFreeSize( SOCKET sn )
{
  if( sn >= MAX_SOCK_NUM ) return 0;
  uint32_t val = 0UL,val1 = 0UL;
  do
  {
    val1 = readSnTX_FSR(sn);
    if( val1 != 0UL ) { val = readSnTX_FSR(sn); }
  } while (val != val1);
  return val;
}

/**
  * @brief Get the received size of the receive buffer.
  * @param sn Socket number.
  * @retval Received size of the receive buffer.
  * @todo Perhaps Operation has been checked.
  */
uint32_t WizchipClass::getRXReceivedSize( SOCKET sn )
{
  if( sn >= MAX_SOCK_NUM ) return 0;
  uint32_t val = 0UL,val1 = 0UL;
  do
  {
    val1 = readSnRX_RSR(sn);
    if(val1 != 0UL) { val = readSnRX_RSR(sn); }
  } while (val != val1);
  return val;
}

/**
  * @brief Write to w5300 TX FIFO Register.
  * @param sn socket number.
  * @param dat write data.
  * @retval none.
  * @todo Operation has been checked.
  */
void WizchipClass::txFifoWrite( SOCKET sn, uint8_t dat )
{
  if( txByteCount[ sn ] & 1 )  // odd
  {
    txByteData[ sn ] |= (uint16_t)dat << 0;
    *idm_ar = Sn_TX_FIFOR( sn );
    *idm_dr = txByteData[ sn ];
  }
  else  // even
  {
    txByteData[ sn ] = (uint16_t)dat << 8;
  }
  txByteCount[ sn ]++;
}

/**
  * @brief Transmission execution.
  * @param sn Socket number.
  * @param _data Address of the array.
  * @param _len Size of the array in bytes.
  * @retval none.
  * @todo Operation checks are ongoing.
  */
void WizchipClass::send_data_processing( SOCKET sn, const uint8_t *data, uint32_t len )
{
  if( sn >= MAX_SOCK_NUM ) return;
  // This is same as having no offset in a call to send_data_processing_offset
  send_data_processing_offset( sn, 0UL, data, len );
}
/**
  * @brief Transmission execution with offset position.
  * @param sn Socket number.
  * @param data_offset Data offset position, but not used.
  * @param _data Address of the array.
  * @param _len Size of the array in bytes.
  * @retval none.
  * @todo Operation checks are ongoing.
  */
void WizchipClass::send_data_processing_offset(
  SOCKET sn, uint32_t data_offset, const uint8_t *data, uint32_t len )
{
// [[maybe_unused]] uint32_t data_offset
  if( sn >= MAX_SOCK_NUM ) return;
  // Writes data to the FIFO.
  for( int i = 0; i < (int)len; i++ ) { txFifoWrite( sn, *data++ ); }
  // Register the position after writing.
  writeSnTX_WR( sn, len + data_offset );
  // Change the command for each protocol.
#if 0
  uint16_t prot = getModeSn( sn ) & 0x000F;
  if( prot == Sn_MR_UDP || prot == Sn_MR_IPRAW ) { execCmdSn( sn, Sock_SEND_MAC ); }  // socket 0 send mac
  else { execCmdSn( sn, Sock_SEND ); }  // socket 0 send
#endif
}

/**
  * @brief Read from w5300 RX FIFO Register.
  * @param sn socket number.
  * @retval Return byte recieved data.
  * @todo Operation has been checked.
  */
uint8_t WizchipClass::rxFifoRead( SOCKET sn, uint32_t count )
{
  uint8_t tempUC;
  if( rxByteCount[ sn ] & 1 )  // odd
  {
    tempUC = (uint8_t)(rxByteData[ sn ] >> 0);
  }
  else  // even
  {
    if( count <= 1UL ) { *idm_ar = Sn_RX_FIFOR( sn ); }
    uint16_t tempUS = *idm_dr;
    rxByteData[ sn ] = tempUS;
    tempUC = (uint8_t)(tempUS >> 8);
  }
  rxByteCount[ sn ]++;
  return tempUC;
}
uint32_t WizchipClass::rxFifoRead( SOCKET sn, uint8_t *dst, uint32_t len )
{
  for( uint32_t count = 0UL; count < len; count++ ) { *dst++ = rxFifoRead( sn, count ); }
  return len;
}


/**
  * @brief Reads received data from the receive FIFO with peek mode.
  * @param sn Socket number.
  * @param dst An array of storage locations.
  * @param len Size of the array in bytes.
  * @param peek If it is not 0, it allows reloading, however it is not used.
  * @retval none.
  * @todo Operation checks are ongoing.
  */
void WizchipClass::recv_data_processing(
  SOCKET sn, uint8_t *data, uint32_t len, [[maybe_unused]] uint8_t peek )
{
  if( sn >= MAX_SOCK_NUM ) return;
  rxFifoRead( sn, data, len );
  if( unReadDataSize[sn] > 0UL && unReadDataSize[sn] >= len ) { unReadDataSize[sn] -= len; }
  else { unReadDataSize[sn] = 0UL; }
}

/**
  * @brief Reads received data from the receive FIFO.
  * @param sn Socket number.
  * @param dst An array of storage locations.
  * @param len Size of the array in bytes.
  * @retval none.
  * @todo Operation checks are ongoing.
  */
void WizchipClass::read_data( SOCKET sn, volatile uint8_t *dst, uint32_t len )
{
#if 1
  recv_data_processing( sn, (uint8_t *)dst, len );
#else
  for( int i = 0; i < (int)len; i++ ) { *dst++ = rxFifoRead( sn ); }
#endif
}

/**
  * @brief Reads received data from the receive FIFO.
  * @param sn Socket number.
  * @param src Starting position of incoming data, but not used.
  * @param dst An array of storage locations.
  * @param len Size of the array in bytes.
  * @retval none.
  * @todo Operation checks are ongoing.
  */
void WizchipClass::read_data(
  SOCKET sn, [[maybe_unused]] volatile uint16_t src, volatile uint8_t *dst, uint32_t len )
{
  read_data( sn, dst, len );
}

#if 0  /* step by step */
/**
  * @brief w5300 interrupt handler.
  * @param sn Socket number.
  * @param dst An array of storage locations.
  * @param len Size of the array in bytes.
  * @param peek If it is not 0, it allows reloading, however it is not used.
  * @retval none.
  * @todo Operation checks are ongoing.
  */
void WizchipClass::intHandler()
{
  uint16_t commonIR = read( W5300_IR );
  if( commonIR & 0x00FF )
  {
    for( uint8_t sn = 0; sn < MAX_SOCK_NUM; sn++ )
    {
      if( commonIR & (0x0001 << sn) )
      {
        if( read( Sn_IR( sn ) ) & Sn_IR_RECV )
        {
          int32_t len = getRXReceivedSize( sn );
          *idm_ar = Sn_RX_FIFOR( sn );
          for( int i = 0; i < len; i += 2 )
          {
            uint16_t tempUS =*idm_dr;
            rcvFifo[sn].write( (uint8_t)(tempUS >> 8) );
            rcvFifo[sn].write( (uint8_t)(tempUS >> 0) );
          }
        }
      }
    }
  }
}

#endif  /* step by step */
