/*
 modified 12 Aug 2013
 by Soohwan Kim (suhwan@wiznet.co.kr)

- 10 Apr. 2015
 Added support for Arduino Ethernet Shield 2
 by Arduino.org team

 */
 
#include "Ethernet5k3.h"
#include "Dhcp.h"

// XXX: don't make assumptions about the value of MAX_SOCK_NUM.
uint8_t EthernetClass5k3::_state[MAX_SOCK_NUM] = { 0, };
uint16_t EthernetClass5k3::_server_port[MAX_SOCK_NUM] = { 0, };

int EthernetClass5k3::begin( const uint8_t *mac_address )
{
  if(_dhcp != NULL ) { delete _dhcp; }
  _dhcp = new DhcpClass();
  // Initialise the basic info
  wizchip.init( mr, idm_ar, idm_dr );
  wizchip.setMACAddress( mac_address );
  wizchip.setIPAddress( IPAddress(0,0,0,0).raw_address() );

  // Now try to get our config info from a DHCP server
  int ret = _dhcp->beginWithDHCP( (uint8_t *)mac_address );
  if( ret == 1 )
  {
    // We've successfully found a DHCP server and got our configuration info, so set things
    // accordingly
    wizchip.setIPAddress( _dhcp->getLocalIp().raw_address() );
    wizchip.setGatewayIp( _dhcp->getGatewayIp().raw_address() );
    wizchip.setSubnetMask( _dhcp->getSubnetMask().raw_address() );
    _dnsServerAddress = _dhcp->getDnsServerIp();
    _dnsDomainName = _dhcp->getDnsDomainName();
    _hostName = _dhcp->getHostName();
  }

  return ret;
}

void EthernetClass5k3::begin( const uint8_t *mac_address, IPAddress local_ip )
{
  // Assume the DNS server will be the machine on the same network as the local IP
  // but with last octet being '1'
  IPAddress dns_server = local_ip;
  dns_server[3] = 1;
  begin( mac_address, local_ip, dns_server );
}

void EthernetClass5k3::begin( const uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server )
{
  // Assume the gateway will be the machine on the same network as the local IP
  // but with last octet being '1'
  IPAddress gateway = local_ip;
  gateway[3] = 1;
  begin( mac_address, local_ip, dns_server, gateway );
}

void EthernetClass5k3::begin(
  const uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway )
{
  IPAddress subnet(255, 255, 255, 0);
  begin( mac_address, local_ip, dns_server, gateway, subnet );
}

void EthernetClass5k3::begin(
  const uint8_t *mac, IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet )
{
  wizchip.init( mr, idm_ar, idm_dr );
  wizchip.setMACAddress( mac );
  wizchip.setIPAddress( local_ip.raw_address() );
  wizchip.setGatewayIp( gateway.raw_address() );
  wizchip.setSubnetMask( subnet.raw_address() );
  _dnsServerAddress = dns_server;
}


int EthernetClass5k3::maintain()
{
  int rc = DHCP_CHECK_NONE;
  if( _dhcp != NULL )
  {
    //we have a pointer to dhcp, use it
    rc = _dhcp->checkLease();
    switch ( rc )
    {
      case DHCP_CHECK_NONE:
        //nothing done
        break;
      case DHCP_CHECK_RENEW_OK:
      case DHCP_CHECK_REBIND_OK:
        //we might have got a new IP.
        wizchip.setIPAddress( _dhcp->getLocalIp().raw_address() );
        wizchip.setGatewayIp( _dhcp->getGatewayIp().raw_address() );
        wizchip.setSubnetMask( _dhcp->getSubnetMask().raw_address() );
        _dnsServerAddress = _dhcp->getDnsServerIp();
        _dnsDomainName = _dhcp->getDnsDomainName();
        _hostName = _dhcp->getHostName();
        break;
      default:
        //this is actually a error, it will retry though
        break;
    }
  }
  return rc;
}

IPAddress EthernetClass5k3::localIP()
{
  IPAddress ret;
  wizchip.getIPAddress( ret.raw_address() );
  return ret;
}

IPAddress EthernetClass5k3::subnetMask()
{
  IPAddress ret;
  wizchip.getSubnetMask( ret.raw_address() );
  return ret;
}

IPAddress EthernetClass5k3::gatewayIP()
{
  IPAddress ret;
  wizchip.getGatewayIp( ret.raw_address() );
  return ret;
}

IPAddress EthernetClass5k3::dnsServerIP() { return _dnsServerAddress; }
char* EthernetClass5k3::dnsDomainName() { return _dnsDomainName; }
char* EthernetClass5k3::hostName() { return _hostName; }

EthernetClass5k3 Ethernet;
