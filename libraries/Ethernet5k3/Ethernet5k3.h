/*
 modified 12 Aug 2013
 by Soohwan Kim (suhwan@wiznet.co.kr)

 - 10 Apr. 2015
 Added support for Arduino Ethernet Shield 2
 by Arduino.org team
 
 */
#ifndef ethernet_5k3_h
#define ethernet_5k3_h

#define  __wizChip__  5300

#include <inttypes.h>
#include <IPAddress.h>
#include "w5300.h"
#include "EthernetClient.h"
#include "EthernetServer.h"
#include "Dhcp.h"

enum EthernetHardwareStatus {
  EthernetNoHardware,
  EthernetW5100,
  EthernetW5200,
  EthernetW5500,
  EthernetW5300,
  EthernetENC28J60 = 10
};

enum EthernetLinkStatus {
  Unknown,
  LinkON,
  LinkOFF
};

class EthernetClass5k3 {
private:
  IPAddress _dnsServerAddress;
  char* _dnsDomainName;
  char* _hostName;
  DhcpClass* _dhcp;

public:
  volatile uint16_t *mr;
  volatile uint16_t *idm_ar;
  volatile uint16_t *idm_dr;
  static uint8_t _state[MAX_SOCK_NUM];
  static uint16_t _server_port[MAX_SOCK_NUM];

  EthernetClass5k3() { _dhcp = NULL; }

  void init( volatile uint16_t *m, volatile uint16_t *a, volatile uint16_t *d )
  {
    mr = m; idm_ar = a; idm_dr = d;
  }

  // Initialize the Ethernet shield to use the provided MAC address and gain the rest of the
  // configuration through DHCP.
  // Returns 0 if the DHCP configuration failed, and 1 if it succeeded
  int  begin( const uint8_t *mac_address );
  void begin( const uint8_t *mac_address, IPAddress local_ip );
  void begin( const uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server );
  void begin( const uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway );
  void begin( const uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet );
  
  int maintain();

  IPAddress localIP();
  IPAddress subnetMask();
  IPAddress gatewayIP();
  IPAddress dnsServerIP();
  char* dnsDomainName();
  char* hostName();
  int hardwareStatus() { return EthernetW5300; }
  int linkStatus() { return LinkON; }

  friend class EthernetClient;
  friend class EthernetServer;
};

extern EthernetClass5k3 Ethernet;

#endif /* ethernet_5k3_h */
