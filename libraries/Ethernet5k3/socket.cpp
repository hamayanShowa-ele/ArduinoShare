 /*
 * - 10 Apr. 2015
 * Added support for Arduino Ethernet Shield 2
 * by Arduino.org team
 */
 
#include "socket.h"

#if __wizChip__ == 5500
  #define  wizchip  w5500
#elif __wizChip__ == 5300
#endif

static uint16_t local_port = 5000U;

/**
 * @brief	This Socket function initialize the channel in perticular mode, and set the port and wait for w5500 done it.
 * @return 	1 for success else 0.
 */
uint8_t socket( SOCKET s, uint8_t protocol, uint16_t port, uint8_t flag )
{
  if( (protocol == SnMR::TCP) || (protocol == SnMR::UDP) || (protocol == SnMR::IPRAW) || (protocol == SnMR::MACRAW) || (protocol == SnMR::PPPOE) )
  {
    close( s );
    wizchip.writeSnMR( s, protocol | flag );
    if( port != 0 ) { wizchip.writeSnPORT( s, port ); } 
    else
    {
      wizchip.writeSnPORT( s, local_port++ );
      if( local_port >= 0U && local_port < 5000U ) { local_port = 5000U; } // if don't set the source port, set local_port number.
    }
    wizchip.execCmdSn( s, Sock_OPEN );
    delay( 10UL );  // Wait a short time for the OPEN command to complete execution.
    return 1;
  }
  return 0;
}


/**
 * @brief	This function close the socket and parameter is "s" which represent the socket number
 */
void close( SOCKET s )
{
  wizchip.execCmdSn( s, Sock_CLOSE );
  wizchip.writeSnIR( s, 0xFF );
}


/**
 * @brief	This function established  the connection for the channel in passive (server) mode. This function waits for the request from the peer.
 * @return	1 for success else 0.
 */
uint8_t listen(SOCKET s)
{
  uint8_t status = wizchip.readSnSR( s );
  if( status != SnSR::INIT ) { return 0; }
  wizchip.execCmdSn( s, Sock_LISTEN );
  return 1;
}

uint8_t accept( SOCKET s )
{
  uint8_t status;
  while( 1 )
  {
    status = wizchip.readSnSR( s );
    if( status == SnSR::LISTEN ) {}
    else if( status == SnSR::ESTABLISHED ) { break; }
    else { break; }
    delay( 10UL );
  }
  return status;
}

/**
 * @brief	This function established  the connection for the channel in Active (client) mode. 
 * 		This function waits for the untill the connection is established.
 * 		
 * @return	1 for success else 0.
 */
uint8_t connect( SOCKET s, uint8_t * addr, uint16_t port )
{
  if( ((addr[0] == 0xFF) && (addr[1] == 0xFF) && (addr[2] == 0xFF) && (addr[3] == 0xFF)) ||
      ((addr[0] == 0x00) && (addr[1] == 0x00) && (addr[2] == 0x00) && (addr[3] == 0x00)) ||
      (port == 0) ) { return 0; }

  // set destination IP
  wizchip.writeSnDIPR( s, addr );
  wizchip.writeSnDPORT( s, port );
  wizchip.execCmdSn( s, Sock_CONNECT );
  return 1;
}



/**
 * @brief	This function used for disconnect the socket and parameter is "s" which represent the socket number
 * @return	1 for success else 0.
 */
void disconnect( SOCKET s )
{
  wizchip.execCmdSn( s, Sock_DISCON );
}

/**
 * @brief	This function used to send the data in TCP mode
 * @return	1 for success else 0.
 */
uint16_t send( SOCKET s, const uint8_t * buf, uint32_t len )
{
  uint8_t status = 0;
  uint16_t ret = 0;
  uint32_t freesize = 0;

  if( len > wizchip.SSIZE ) { ret = wizchip.SSIZE; }  // check size not to exceed MAX size.
  else { ret = len; }
  // if freebuf is available, start.
  do
  {
    freesize = wizchip.getTXFreeSize( s );
    status = wizchip.readSnSR( s );
    if( (status != SnSR::ESTABLISHED) && (status != SnSR::CLOSE_WAIT) )
    {
      ret = 0; 
      break;
    }
  } while( freesize < ret );
  // copy data
  wizchip.send_data_processing( s, (uint8_t *)buf, ret );
  wizchip.execCmdSn( s, Sock_SEND );
  /* +2008.01 bj */
  while( (wizchip.readSnIR( s ) & SnIR::SEND_OK) != SnIR::SEND_OK ) 
  {
    /* m2008.01 [bj] : reduce code */
    if ( wizchip.readSnSR( s ) == SnSR::CLOSED )
    {
      close( s );
      return 0;
    }
  }
  /* +2008.01 bj */
  wizchip.writeSnIR( s, SnIR::SEND_OK );
  return ret;
}


/**
 * @brief	This function is an application I/F function which is used to receive the data in TCP mode.
 * 		It continues to wait for data as much as the application wants to receive.
 * 		
 * @return	received data size for success else -1.
 */
int32_t recv( SOCKET s, uint8_t *buf, int32_t len )
{
  int32_t ret;
  // Check how much data is available
  if( wizchip.unReadDataSize[s] )  // When there is still unread data in the w5300's FIFO.
  {
    if( wizchip.unReadDataSize[s] < (uint32_t)len ) { len = (int32_t)wizchip.unReadDataSize[s]; }
    wizchip.recv_data_processing( s, buf, len );
    if( wizchip.unReadDataSize[s] == 0UL ) { wizchip.execCmdSn( s, Sock_RECV ); }
    ret = len;
  }
  else  // When no unread data remains in the w5300's FIFO.
  {
    ret = (int32_t)wizchip.getRXReceivedSize( s );
    if( ret == 0 )
    {
      // No data available.
      uint8_t status = wizchip.readSnSR(s);
      if( status == SnSR::LISTEN || status == SnSR::CLOSED || status == SnSR::CLOSE_WAIT )
      {
        return 0;  // The remote end has closed its side of the connection, so this is the eof state
      }
      else { return -1; }  // The connection is still up, but there's no data waiting to be read
    }

    uint16_t remain,prot;
    prot = wizchip.readSnMR(s) & 0x000F;  // Check the socket protocol.
    if( prot == SnMR::UDP )
    {
      remain = (int)len;
      wizchip.unReadDataSize[s] == remain;
    }
    else if( prot == SnMR::TCP )
    {
      wizchip.rxByteCount[s] = 0;  // Initialize the number of bytes received.
      uint8_t tempUC[2];
      wizchip.recv_data_processing( s, tempUC, sizeof(tempUC) );  // Obtain the size of the following data from the first 2 bytes of the FIFO data.
      remain = (tempUC[0] << 8) | (tempUC[1] << 0);
      wizchip.unReadDataSize[s] = remain;  // Save the data size in the receive FIFO.
    }
    else { return -2; }

    if( remain < len ) { ret = remain; }
    else { ret = len; }
    wizchip.recv_data_processing( s, buf, ret );
    if( wizchip.unReadDataSize[s] == 0UL )
    {
      wizchip.execCmdSn( s, Sock_RECV );
    }
  }
  return ret;
}


/**
 * @brief	Returns the first byte in the receive queue (no checking)
 * 		
 * @return
 */
uint16_t peek( SOCKET s, uint8_t *buf )
{
  wizchip.recv_data_processing( s, buf, 1, 1 );
  return 1;
}


/**
 * @brief	This function is an application I/F function which is used to send the data for other then TCP mode. 
 * 		Unlike TCP transmission, The peer's destination address and the port is needed.
 * 		
 * @return	This function return send data size for success else -1.
 */
uint32_t sendto( SOCKET s, const uint8_t *buf, uint32_t len, uint8_t *addr, uint16_t port )
{
  uint32_t ret = 0UL;

  if( len > wizchip.SSIZE ) { ret = wizchip.SSIZE; } // check size not to exceed MAX size.
  else { ret = len; }

  if( ((addr[0] == 0) && (addr[1] == 0) && (addr[2] == 0) && (addr[3] == 0)) ||
      ((port == 0x00)) ||(ret == 0) ) 
  {
    /* +2008.01 [bj] : added return value */
    ret = 0UL;
  }
  else
  {
    wizchip.writeSnDIPR( s, addr );
    wizchip.writeSnDPORT( s, port );
    // copy data
    wizchip.send_data_processing( s, (uint8_t *)buf, ret );
    wizchip.execCmdSn( s, Sock_SEND );
    /* +2008.01 bj */
    while( (wizchip.readSnIR(s) & SnIR::SEND_OK) != SnIR::SEND_OK ) 
    {
      if( wizchip.readSnIR(s) & SnIR::TIMEOUT )
      {
        /* +2008.01 [bj]: clear interrupt */
        wizchip.writeSnIR( s, (SnIR::SEND_OK | SnIR::TIMEOUT) ); /* clear SEND_OK & TIMEOUT */
        return 0UL;
      }
    }
    /* +2008.01 bj */
    wizchip.writeSnIR( s, SnIR::SEND_OK );
  }
  return ret;
}


/**
 * @brief	This function is an application I/F function which is used to receive the data in other then
 * 	TCP mode. This function is used to receive UDP, IP_RAW and MAC_RAW mode, and handle the header as well. 
 * 	
 * @return	This function return received data size for success else -1.
 */
uint32_t recvfrom( SOCKET s, uint8_t *buf, uint32_t len, uint8_t *addr, uint16_t *port )
{
  if( len == 0UL ) { return 0UL; }
  int32_t ret = 0;
  // Check how much data is available
  if( wizchip.unReadDataSize[s] )  // When there is still unread data in the w5300's FIFO.
  {
    if( wizchip.unReadDataSize[s] < (uint32_t)len ) { len = (int32_t)wizchip.unReadDataSize[s]; }
    wizchip.recv_data_processing( s, buf, len );
    if( wizchip.unReadDataSize[s] == 0UL ) { wizchip.execCmdSn( s, Sock_RECV ); }
    ret = len;
  }
  else  // When no unread data remains in the w5300's FIFO.
  {
    if( !wizchip.getRXReceivedSize( s ) ) { return 0UL; }  // No data available.
  
    wizchip.rxByteCount[s] = 0;  // Initialize the number of bytes received.
    uint16_t remain = 0;
    uint16_t prot = wizchip.readSnMR(s) & 0x000F;  // Check the socket protocol.
    if( prot == SnMR::UDP )
    {
      uint8_t tempUC[8];
      wizchip.recv_data_processing( s, tempUC, sizeof(tempUC) );  // Obtain the size of the following data from the first 2 bytes of the FIFO data.
      memcpy( wizchip._remoteIP[s], tempUC, IPV4_SIZE );
      memcpy( addr, tempUC, IPV4_SIZE );
      wizchip._remotePort[s] = (tempUC[4] << 8) | (tempUC[5] << 0);
      *port = wizchip._remotePort[s];
      remain = (tempUC[6] << 8) | (tempUC[7] << 0);
    }
    else if( prot == SnMR::IPRAW )
    {
      uint8_t tempUC[6];
      wizchip.recv_data_processing( s, tempUC, sizeof(tempUC) );  // Obtain the size of the following data from the first 2 bytes of the FIFO data.
      memcpy( wizchip._remoteIP[s], tempUC, IPV4_SIZE );
      memcpy( addr, tempUC, IPV4_SIZE );
      remain = (tempUC[4] << 8) | (tempUC[5] << 0);
    }
    else if( prot == SnMR::MACRAW )
    {
      uint8_t tempUC[2];
      wizchip.recv_data_processing( s, tempUC, sizeof(tempUC) );  // Obtain the size of the following data from the first 2 bytes of the FIFO data.
      remain = (tempUC[0] << 8) | (tempUC[1] << 0);
    }
    else { return 0UL; }
    wizchip.unReadDataSize[s] = remain;  // Save the data size in the receive FIFO.

    if( remain < len ) { ret = remain; }
    else { ret = len; }
    wizchip.recv_data_processing( s, buf, ret );
    if( wizchip.unReadDataSize[s] == 0UL ) { wizchip.execCmdSn( s, Sock_RECV ); }
  }
  return ret;
}

/**
 * @brief	Wait for buffered transmission to complete.
 */
void flush( SOCKET s ) { /* To Do */ }

/**
 * @brief	Set the number of received counters to 0.
 */
void rxFlush( SOCKET s ) { wizchip.rxByteCountClear( s ); }

uint16_t igmpsend( SOCKET s, const uint8_t * buf, uint32_t len )
{
  uint8_t status = 0;
  uint32_t ret = 0UL;

  if( len > wizchip.SSIZE ) { ret = wizchip.SSIZE; }  // check size not to exceed MAX size.
  else { ret = len; }
  if( ret == 0UL ) { return 0; }

  wizchip.send_data_processing( s, (uint8_t *)buf, ret );
  wizchip.execCmdSn( s, Sock_SEND );
  while( (wizchip.readSnIR(s) & SnIR::SEND_OK) != SnIR::SEND_OK ) 
  {
    status = wizchip.readSnSR(s);
    if( wizchip.readSnIR(s) & SnIR::TIMEOUT )
    {
      /* in case of igmp, if send fails, then socket closed */
      /* if you want change, remove this code. */
      close( s );
      return 0;
    }
  }
  wizchip.writeSnIR( s, SnIR::SEND_OK );
  return ret;
}

uint16_t bufferData( SOCKET s, uint16_t offset, const uint8_t* buf, uint32_t len )
{
  uint32_t ret = 0;
  if( len > wizchip.getTXFreeSize(s) ) { ret = wizchip.getTXFreeSize(s); }  // check size not to exceed MAX size.
  else { ret = len; }
  wizchip.send_data_processing_offset( s, offset, buf, ret );
  return ret;
}

int startUDP( SOCKET s, uint8_t* addr, uint16_t port )
{
  if( ((addr[0] == 0) && (addr[1] == 0) && (addr[2] == 0) && (addr[3] == 0)) ||
      ((port == 0)) ) 
  {
    return 0;
  }
  else
  {
    wizchip.writeSnDIPR( s, addr );
    wizchip.writeSnDPORT( s, port );
    return 1;
  }
}

int sendUDP( SOCKET s )
{
  wizchip.execCmdSn( s, Sock_SEND );
  /* +2008.01 bj */
  while( (wizchip.readSnIR(s) & SnIR::SEND_OK) != SnIR::SEND_OK ) 
  {
    if( wizchip.readSnIR(s) & SnIR::TIMEOUT )
    {
      /* +2008.01 [bj]: clear interrupt */
      wizchip.writeSnIR( s, (SnIR::SEND_OK|SnIR::TIMEOUT) );
      return 0;
    }
  }
  /* +2008.01 bj */	
  wizchip.writeSnIR( s, SnIR::SEND_OK );
  /* Sent ok */
  return 1;
}
