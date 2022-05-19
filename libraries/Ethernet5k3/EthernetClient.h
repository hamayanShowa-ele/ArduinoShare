#ifndef ethernetclient_h
#define ethernetclient_h
#include <Arduino.h>
#include <Print.h>
#include <Client.h>
#include <IPAddress.h>
#include  <MY_STREAM.h>

class EthernetClient : public Client {

public:
  EthernetClient();
  EthernetClient(uint8_t sock);

  uint8_t status();
  virtual int connect(IPAddress ip, uint16_t port);
  virtual int connect(const char *host, uint16_t port);
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *buf, size_t size);
  virtual int available();
  virtual int read();
  virtual int read(uint8_t *buf, size_t size);
  virtual int peek();
  virtual void flush();
  virtual void stop();
  virtual uint8_t connected();
  virtual operator bool();
  virtual bool operator==(const EthernetClient&);
  virtual bool operator!=(const EthernetClient& rhs) { return !this->operator==(rhs); };

  IPAddress remoteIP() { return _remoteIP; }

  friend class EthernetServer;
  
  using Print::write;

  // Added method.
  uint8_t whatIsSocket() { return _sock; }
  void run( MY_STREAM *fifo );

private:
  static uint16_t _srcport;
  uint8_t _sock;
  IPAddress _remoteIP;
//  uint32_t rcvLen = 0UL;
};

#endif
