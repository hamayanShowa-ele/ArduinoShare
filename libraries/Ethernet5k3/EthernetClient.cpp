#include <Arduino.h>
#include "w5300.h"
#include "socket.h"
#include "Ethernet5k3.h"
#include "EthernetClient.h"
#include "EthernetServer.h"
#include "Dns.h"

extern "C"
{
  #include "string.h"
}

uint16_t EthernetClient::_srcport = 1024;

EthernetClient::EthernetClient() : _sock(MAX_SOCK_NUM) {}

EthernetClient::EthernetClient(uint8_t sock) : _sock(sock) {}

int EthernetClient::connect(const char* host, uint16_t port)
{
  // Look up the host first
  int ret = 0;
  DNSClient dns;
  IPAddress remote_addr;

  dns.begin(Ethernet.dnsServerIP());
  ret = dns.getHostByName(host, remote_addr);
  if (ret == 1)
  {
    ret = connect(remote_addr, port);
    return ret;
  }
  else
  {
    return ret;
  }
}

int EthernetClient::connect(IPAddress ip, uint16_t port)
{
  if (_sock != MAX_SOCK_NUM) { return 0; }
  _remoteIP = ip;
  for (int i = 0; i < MAX_SOCK_NUM; i++)
  {
    uint8_t s = wizchip.readSnSR(i);
    if (s == SnSR::CLOSED || s == SnSR::FIN_WAIT || s == SnSR::CLOSE_WAIT)
    {
      _sock = i;
      break;
    }
  }

  if (_sock == MAX_SOCK_NUM)
    return 0;

  _srcport++;
  if (_srcport == 0) _srcport = 1024;
  socket(_sock, SnMR::TCP, _srcport, 0);

  if (!::connect(_sock, rawIPAddress(ip), port)) {
    _sock = MAX_SOCK_NUM;
    return 0;
  }

  while (status() != SnSR::ESTABLISHED) {
//    delay(1);
    delay(100);
    if (status() == SnSR::CLOSED) {
      _sock = MAX_SOCK_NUM;
      return 0;
    }
  }
  return 1;
}

size_t EthernetClient::write(uint8_t b) {
  return write(&b, 1);
}

size_t EthernetClient::write(const uint8_t *buf, size_t size) {
  if (_sock == MAX_SOCK_NUM) {
    setWriteError();
    return 0;
  }
  if (!send(_sock, buf, size)) {
    setWriteError();
    return 0;
  }
  return size;
}

int EthernetClient::available()
{
  if (_sock != MAX_SOCK_NUM)
  {
  uint32_t len = wizchip.unReadDataSize[ _sock ];
  if( len ) { return len; }
  return wizchip.getRXReceivedSize( _sock );
  }
  return 0;
}

int EthernetClient::read()
{
  uint8_t b;
  read( &b, 1 );
  return b;
}
int EthernetClient::read( uint8_t *buf, size_t size )
{
  int32_t ret = recv(_sock, buf, size);
  return ret;
}

int EthernetClient::peek() {
  uint8_t b;
  // Unlike recv, peek doesn't check to see if there's any data available, so we must
  if (!available())
    return -1;
  ::peek(_sock, &b);
  return b;
}

void EthernetClient::flush() {
  ::flush(_sock);
}

void EthernetClient::stop() {
  if (_sock == MAX_SOCK_NUM)
    return;

  // attempt to close the connection gracefully (send a FIN to other side)
  disconnect(_sock);
  unsigned long start = millis();

  // wait a second for the connection to close
  while (status() != SnSR::CLOSED && millis() - start < 1000)
    delay(1);

  // if it hasn't closed, close it forcefully
  if (status() != SnSR::CLOSED)
    close(_sock);

  EthernetClass5k3::_server_port[_sock] = 0;
  _sock = MAX_SOCK_NUM;
}

uint8_t EthernetClient::connected() {
  if (_sock == MAX_SOCK_NUM) return 0;
  
  uint8_t s = status();
  return !(s == SnSR::LISTEN || s == SnSR::CLOSED || s == SnSR::FIN_WAIT ||
    (s == SnSR::CLOSE_WAIT && !available()));
}

uint8_t EthernetClient::status() {
  if (_sock == MAX_SOCK_NUM) return SnSR::CLOSED;
    uint8_t ret = wizchip.readSnSR(_sock);
    return ret;
}

// the next function allows us to use the client returned by
// EthernetServer::available() as the condition in an if-statement.

EthernetClient::operator bool() {
  return _sock != MAX_SOCK_NUM;
}

bool EthernetClient::operator==(const EthernetClient& rhs) {
  return _sock == rhs._sock && _sock != MAX_SOCK_NUM && rhs._sock != MAX_SOCK_NUM;
}

/**
  * @brief The socket reception process is performed here.
  * @param fifo Pointer to Stream.
  * @retval none.
  * @todo Operation checks are ongoing.
  */
void EthernetClient::run( MY_STREAM *fifo )
{
  int len = available();
  if( len >= 4 )
  {
    uint8_t *buf = new uint8_t[ len ];
    read( buf, len );
    uint16_t len = ((uint16_t)buf[0] << 8) | ((uint16_t)buf[1] << 0);
    fifo->write( (const uint8_t *)&buf[2], len, 0UL );
    delete [] buf;
  }
}
