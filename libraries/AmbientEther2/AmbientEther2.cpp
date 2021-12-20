/*
 * ambient.cpp - Library for sending data to Ambient
 * Created by Takehiko Shimojima, April 21, 2016
 * 
 * chobichan has modified it for the Ethernet shield.
*/
#include "AmbientEther2.h"

#define AMBIENT_DEBUG 0

#if AMBIENT_DEBUG
#define DBG(...) { Serial.print(__VA_ARGS__); }
#define ERR(...) { Serial.print(__VA_ARGS__); }
#else
#define DBG(...)
#define ERR(...)
#endif /* AMBIENT_DBG */

// const char* AMBIENT_HOST = "54.65.206.59";

const char* AMBIENT_HOST = "ambidata.io";
unsigned short AMBIENT_PORT = 80;
const char* AMBIENT_HOST_DEV = "192.168.11.2";
unsigned short AMBIENT_PORT_DEV = 4567;

const char * ambient_keys[] =
  {"\"d1\":\"", "\"d2\":\"", "\"d3\":\"", "\"d4\":\"", 
  "\"d5\":\"", "\"d6\":\"", "\"d7\":\"", "\"d8\":\"", 
  "\"lat\":\"", "\"lng\":\"", "\"created\":\""};

bool
AmbientEther2::begin(
    unsigned long channelId, const char * writeKey,
    EthernetClient * c, const char * readKey, int dev ) {
    this->channelId = channelId;

    if (sizeof(writeKey) > AMBIENT_WRITEKEY_SIZE) {
        ERR("writeKey length > AMBIENT_WRITEKEY_SIZE");
        return false;
    }
    strcpy(this->writeKey, writeKey);

    if(NULL == c) {
        ERR("Socket Pointer is NULL, open a socket.");
        return false;
    }
    this->client = c;
    if (readKey != NULL) {
        strcpy(this->readKey, readKey);
    } else {
        strcpy(this->readKey, "");
    }
    this->dev = dev;
    if (dev) {
        strcpy(this->host, AMBIENT_HOST_DEV);
        this->port = AMBIENT_PORT_DEV;
    } else {
        strcpy(this->host, AMBIENT_HOST);
        this->port = AMBIENT_PORT;
    }
    for (int i = 0; i < AMBIENT_NUM_PARAMS; i++) {
        this->data[i].set = false;
    }
    this->cmnt.set = false;
    return true;
}

bool
AmbientEther2::set(int field,const char * data) {
    --field;
    if (field < 0 || field >= AMBIENT_NUM_PARAMS) {
        return false;
    }
    if (strlen(data) > AMBIENT_DATA_SIZE) {
        return false;
    }
    this->data[field].set = true;
    strcpy(this->data[field].item, data);

    return true;
}

bool AmbientEther2::set(int field, double data)
{
	return set(field,String(data).c_str());
}

bool AmbientEther2::set(int field, int data)
{
	return set(field, String(data).c_str());
}

bool
AmbientEther2::clear(int field) {
    --field;
    if (field < 0 || field >= AMBIENT_NUM_PARAMS) {
        return false;
    }
    this->data[field].set = false;
    this->cmnt.set = false;

    return true;
}

bool
AmbientEther2::setcmnt(const char * data) {
    if (strlen(data) > AMBIENT_CMNT_SIZE) {
        return false;
    }
    this->cmnt.set = true;
    strcpy(this->cmnt.item, data);

    return true;
}

bool AmbientEther2::connect2host(uint32_t tmout) {
    int retry;
    for (retry = 0; retry < AMBIENT_MAX_RETRY; retry++) {
        int ret;
        this->client->setTimeout(tmout);
        ret = this->client->connect(this->host, this->port);
        if (ret) {
            break ;
        }
    }
    if(retry == AMBIENT_MAX_RETRY) {
        ERR("Could not connect socket to host\r\n");
        return false;
    }
    return true;
}

int
AmbientEther2::getStatusCode() {
    String _buf = this->client->readStringUntil('\n');
    int from = _buf.indexOf("HTTP/1.1 ") + sizeof("HTTP/1.1 ") - 1;
    int to = _buf.indexOf(' ', from);
    this->status = _buf.substring(from, to).toInt();
    return this->status;
}

bool
AmbientEther2::send(uint32_t tmout) {
#if 0
    char str[180];
    char body[192];
    char inChar;

    this->status = 0;
    if (connect2host(tmout) == false) {
        return false;
    }
    memset(body, 0, sizeof(body));
    strcat(body, "{\"writeKey\":\"");
    strcat(body, this->writeKey);
    strcat(body, "\",");

    for (int i = 0; i < AMBIENT_NUM_PARAMS; i++) {
        if (this->data[i].set) {
            strcat(body, ambient_keys[i]);
            strcat(body, this->data[i].item);
            strcat(body, "\",");
        }
    }
    if (this->cmnt.set) {
        strcat(body, "\"cmnt\":\"");
        strcat(body, this->cmnt.item);
        strcat(body, "\",");
    }
    body[strlen(body) - 1] = '\0';
    strcat(body, "}\r\n");

    memset(str, 0, sizeof(str));
    sprintf(str, "POST /api/v2/channels/%lu/data HTTP/1.1\r\n", this->channelId);
    if (this->port == 80) {
        sprintf(&str[strlen(str)], "Host: %s\r\n", this->host);
    } else {
        sprintf(&str[strlen(str)], "Host: %s:%d\r\n", this->host, this->port);
    }
    sprintf(&str[strlen(str)], "Content-Length: %d\r\n", strlen(body));
    sprintf(&str[strlen(str)], "Content-Type: application/json\r\n\r\n");

    DBG("sending: ");DBG(strlen(str));DBG("bytes\r\n");DBG(str);
//Serial.print( str ); Serial.print( body );

    int ret;
    ret = this->client->print(str);
    delay(30);
    DBG(ret);DBG(" bytes sent\n\n");
    if (ret == 0) {
        ERR("send failed\n");
        return false;
    }
    ret = this->client->print(body);
    delay(30);
    DBG(ret);DBG(" bytes sent\n\n");
    if (ret == 0) {
        ERR("send failed\n");
        return false;
    }

    getStatusCode();
    while (this->client->available()) {
        inChar = this->client->read();
#if AMBIENT_DEBUG
        Serial.write(inChar);
#endif
    }

    this->client->stop();

    for (int i = 0; i < AMBIENT_NUM_PARAMS; i++) {
        this->data[i].set = false;
    }
    this->cmnt.set = false;

    return true;
#else
  this->status = 0;

  if( connect2host(tmout) == false ) { return false; }

  String body = "{\"writeKey\":\""; body += this->writeKey; body += "\"";
  for( int i = 0; i < AMBIENT_NUM_PARAMS; i++ )
  {
    if( this->data[i].set ) { body += dataString( i, this->data[i].item ); }
  }
  if( this->cmnt.set )
  {
    body += ",\"cmnt\":\""; body += this->cmnt.item; body += "\"";
  }
  body += "}\r\n";

  String head = "POST /api/v2/channels/"; head += this->channelId; head += "/data HTTP/1.1\r\n";
  if( this->port == 80 )
  {
    head += "Host: "; head += this->host; head += "\r\n";
  }
  else
  {
    head += "Host: "; head += this->host; head += ":"; head += this->port; head += "\r\n";
  }
  head += "Content-Length: "; head += body.length(); head += "\r\n";
  head += "Content-Type: application/json\r\n\r\n";

  DBG("sending: ");DBG( body.length() );DBG("bytes\r\n");DBG( body );
//Serial.print( head ); Serial.print( body );

  int ret;
  ret = this->client->print( head );
  delay(30);
  DBG(ret);DBG(" bytes sent\n\n");
  if (ret == 0)
  {
    ERR("send failed\n");
    return false;
  }
  ret = this->client->print( body );
  delay(30);
  DBG(ret);DBG(" bytes sent\n\n");
  if (ret == 0)
  {
    ERR("send failed\n");
    return false;
  }

  getStatusCode();
  while( this->client->available() )
  {
    char inChar = this->client->read();
#if AMBIENT_DEBUG
    Serial.write(inChar);
#endif
  }
  this->client->stop();

  for( int i = 0; i < AMBIENT_NUM_PARAMS; i++ )
  {
    this->data[i].set = false;
  }
  this->cmnt.set = false;

  return true;
#endif
}

int
AmbientEther2::bulk_send(char *buf, uint32_t tmout) {
    char str[180];
    char inChar;

    this->status = 0;
    if (connect2host(tmout) == false) {
        return false;
    }
    memset(str, 0, sizeof(str));
    sprintf(str, "POST /api/v2/channels/%lu/dataarray HTTP/1.1\r\n", this->channelId);
    if (this->port == 80) {
        sprintf(&str[strlen(str)], "Host: %s\r\n", this->host);
    } else {
        sprintf(&str[strlen(str)], "Host: %s:%d\r\n", this->host, this->port);
    }
    sprintf(&str[strlen(str)], "Content-Length: %d\r\n", strlen(buf));
    sprintf(&str[strlen(str)], "Content-Type: application/json\r\n\r\n");

    DBG("sending: ");DBG(strlen(str));DBG("bytes\r\n");DBG(str);

    int ret;
    ret = this->client->print(str); // send header
    delay(30);
    DBG(ret);DBG(" bytes sent\n\n");
    if (ret == 0) {
        ERR("send failed\n");
        return -1;
    }

    int sent = 0;
    unsigned long starttime = millis();
    while ((millis() - starttime) < AMBIENT_TIMEOUT) {
        ret = this->client->print(&buf[sent]);
        delay(30);
        DBG(ret);DBG(" bytes sent\n\n");
        if (ret == 0) {
            ERR("send failed\n");
            return -1;
        }
        sent += ret;
        if (sent >= strlen(buf)) {
            break;
        }
    }
    delay(500);

    getStatusCode();
    while (this->client->available()) {
        inChar = this->client->read();
#if AMBIENT_DEBUG
        Serial.write(inChar);
#endif
    }

    this->client->stop();

    for (int i = 0; i < AMBIENT_NUM_PARAMS; i++) {
        this->data[i].set = false;
    }
    this->cmnt.set = false;

    return (sent == 0) ? -1 : sent;
}

int AmbientEther2::bulk_send( String body, uint32_t tmout )
{
  this->status = 0;
  if (connect2host(tmout) == false) { return false; }
  String head = "POST /api/v2/channels/"; head += this->channelId; head += "/dataarray HTTP/1.1\r\n";
  if( this->port == 80 )
  {
    head += "Host: "; head += this->host; head += "\r\n";
  }
  else
  {
    head += "Host: "; head += this->host; head += ":"; head += this->port; head += "\r\n";
  }
  head += "Content-Length: "; head += body.length(); head += "\r\n";
  head += "Content-Type: application/json\r\n\r\n";

  DBG("sending: ");DBG( head.length() );DBG("bytes\r\n");DBG(head);
  int ret = this->client->print(head); // send header
  delay(30);
  DBG(ret);DBG(" bytes sent\n\n");
  if (ret == 0) { ERR("send failed\n"); return -1; }

  int sent = 0;
  char *str = body.c_str();
  unsigned long starttime = millis();
  while( (millis() - starttime) < AMBIENT_TIMEOUT )
  {
    ret = this->client->print( str );
    delay(30);
    DBG(ret);DBG(" bytes sent\n\n");
    if(ret == 0) { ERR("send failed\n"); return -1; }
    sent += ret;
    str += ret;
    if( sent >= body.length() ) { break; }
  }
//  delay(500);
  delay( 300UL );
//Serial.print( head ); Serial.print( body );

  getStatusCode();
  while( this->client->available() )
  {
    char inChar = this->client->read();
#if AMBIENT_DEBUG
    Serial.write(inChar);
#endif
  }
  this->client->stop();
  for (int i = 0; i < AMBIENT_NUM_PARAMS; i++) { this->data[i].set = false; }
  this->cmnt.set = false;

  return (sent == 0) ? -1 : sent;
}


bool
AmbientEther2::read(char * buf, int len, int n, uint32_t tmout) {
    char str[180];
    String _buf;

    this->status = 0;
    if (connect2host(tmout) == false) {
        return false;
    }
    memset(str, 0, sizeof(str));
    sprintf(str, "GET /api/v2/channels/%lu/data?readKey=%s&n=%d HTTP/1.1\r\n", this->channelId, this->readKey, n);
    if (this->port == 80) {
        sprintf(&str[strlen(str)], "Host: %s\r\n\r\n", this->host);
    } else {
        sprintf(&str[strlen(str)], "Host: %s:%d\r\n\r\n", this->host, this->port);
    }

    DBG("sending: ");DBG(strlen(str));DBG("bytes\r\n");DBG(str);

    int ret;
    ret = this->client->print(str);
    delay(30);
    DBG(ret);DBG(" bytes sent\n\n");
    if (ret == 0) {
        ERR("send failed\n");
        return false;
    }

    if (getStatusCode() != 200) {  // If the status code is not 200, read off the rest.
        while (this->client->available()) {
            this->client->readStringUntil('\n');
        }
        return false;
    }
    while (this->client->available()) {
        _buf = this->client->readStringUntil('\n');
        if (_buf.length() == 1)  // Find an empty line.
            break;
    }
    _buf = this->client->readStringUntil('\n');
    _buf.toCharArray(buf, len);

    this->client->stop();

    return true;
}

String AmbientEther2::read( int n, uint32_t tmout )
{
  this->status = 0;
  if( connect2host(tmout) == false ) { return ""; }

  String head = "GET /api/v2/channels/"; head += this->channelId;
  head += "/data?readKey="; head += this->readKey;
  head += "&n="; head += n; head += " HTTP/1.1\r\n";
  if( this->port == 80 )
  {
    head += "Host: "; head += this->host; head += "\r\n\r\n";
  }
  else
  {
    head += "Host: "; head += this->host; head += ":"; head += this->port; head += "\r\n\r\n";
  }
  DBG("sending: ");DBG( head.length() );DBG("bytes\r\n");DBG( head );
//Serial.print( head );

  int ret = this->client->print( head );
  delay( 30UL );
  DBG(ret);DBG(" bytes sent\n\n");
  if( ret == 0 ) { ERR("send failed\n"); return ""; }

  delay( 300UL );
  if( getStatusCode() != 200 )  // If the status code is not 200, read off the rest.
  {
    while (this->client->available()) { this->client->readStringUntil('\n'); }
    return "";
  }
  // Find an empty line.
  String body;
  while( this->client->available() )
  {
    body = this->client->readStringUntil('\n');
    if( body.length() == 1 ) { break; }
  }
  body = this->client->readStringUntil('\n');

  return body;
}

bool
AmbientEther2::delete_data(const char * userKey, uint32_t tmout) {
#if 0
    char str[180];
    char inChar;

    this->status = 0;
    if (connect2host(tmout) == false) {
        return false;
    }
    memset(str, 0, sizeof(str));
    sprintf(str, "DELETE /api/v2/channels/%lu/data?userKey=%s HTTP/1.1\r\n", this->channelId, userKey);
    if (this->port == 80) {
        sprintf(&str[strlen(str)], "Host: %s\r\n", this->host);
    } else {
        sprintf(&str[strlen(str)], "Host: %s:%d\r\n", this->host, this->port);
    }
    sprintf(&str[strlen(str)], "Content-Length: 0\r\n");
    sprintf(&str[strlen(str)], "Content-Type: application/json\r\n\r\n");
    DBG(str);

    int ret;
    ret = this->client->print(str);
    delay(30);
    DBG(ret);DBG(" bytes sent\r\n");
    if (ret == 0) {
        ERR("send failed\r\n");
        return false;
    }

    getStatusCode();
    while (this->client->available()) {
        inChar = this->client->read();
#if AMBIENT_DEBUG
        Serial.write(inChar);
#endif
    }

    this->client->stop();

    for (int i = 0; i < AMBIENT_NUM_PARAMS; i++) {
        this->data[i].set = false;
    }
    this->cmnt.set = false;

    return true;
#else
  this->status = 0;
  if( connect2host(tmout) == false ) { return false; }

  String head = "DELETE /api/v2/channels/"; head += this->channelId;
  head += "/data?userKey="; head += userKey; head += " HTTP/1.1\r\n";
  if( this->port == 80 )
  {
    head += "Host: "; head += this->host; head += "\r\n";
  }
  else
  {
    head += "Host: "; head += this->host; head += ":"; head += this->port; head += "\r\n";
  }
  head += "Content-Length: 0\r\n";
  head += "Content-Type: application/json\r\n\r\n";
  DBG( head );

  int ret = this->client->print( head );
  delay(30);
  DBG(ret);DBG(" bytes sent\r\n");
  if (ret == 0) { ERR("send failed\r\n"); return false; }
//Serial.print( head );

  getStatusCode();
  while (this->client->available())
  {
    char inChar = this->client->read();
#if AMBIENT_DEBUG
    Serial.write(inChar);
#endif
  }
  this->client->stop();

  for(int i = 0; i < AMBIENT_NUM_PARAMS; i++) { this->data[i].set = false; }
  this->cmnt.set = false;

  return true;
#endif
}

// This method is not suitable for Arduino.
bool
AmbientEther2::getchannel(
    const char * userKey, const char * devKey,
    unsigned long & channelId,
    char * writeKey, int len,
    EthernetClient * c, uint32_t tmout, int dev) {

    this->status = 0;
    if(NULL == c) {
        ERR("Socket Pointer is NULL, open a socket.");
        return false;
    }
    this->client = c;
    this->dev = dev;
    if (dev) {
        strcpy(this->host, AMBIENT_HOST_DEV);
        this->port = AMBIENT_PORT_DEV;
    } else {
        strcpy(this->host, AMBIENT_HOST);
        this->port = AMBIENT_PORT;
    }

    if (connect2host(tmout) == false) {
        return false;
    }

//    char str[1024];
    char str[ 180 ];
    char inChar;

    memset(str, 0, sizeof(str));
    sprintf(str, "GET /api/v2/channels/?userKey=%s&devKey=%s HTTP/1.1\r\n", userKey, devKey);
    if (this->port == 80) {
        sprintf(&str[strlen(str)], "Host: %s\r\n", this->host);
    } else {
        sprintf(&str[strlen(str)], "Host: %s:%d\r\n", this->host, this->port);
    }
    sprintf(&str[strlen(str)], "Content-Type: application/json\r\n\r\n");
    DBG(str);

    int ret;
    ret = this->client->print(str);
    delay(30);
    DBG(ret);DBG(" bytes sent\r\n");
    if (ret == 0) {
        ERR("send failed\r\n");
        return false;
    }

    if (getStatusCode() != 200) {  // If the status code is not 200, read off the rest.
        while (this->client->available()) {
            this->client->readStringUntil('\n');
        }
        return false;
    }
    while (this->client->available()) {
        String buf = this->client->readStringUntil('\n');
        if (buf.length() == 1)
            break;
    }
    String buf = this->client->readStringUntil('\n');

    int from, to;
    from = buf.indexOf("\"ch\":\"") + strlen("\"ch\":\"");
    to = buf.indexOf("\",", from);
    channelId = buf.substring(from, to).toInt();
    from = buf.indexOf("\"writeKey\":\"") + strlen("\"writeKey\":\"");
    to = buf.indexOf("\",", from);
    buf.substring(from, to).toCharArray(writeKey, len);

    this->client->stop();

    return true;
}

String AmbientEther2::dataString( int index, const char *data )
{
  if( index < 0 || index > 10 ) return "";
  String str = ",\"";
  if( index < 8 )
  {
    str += "d"; str += index + 1; str += "\"";
  }
  else if( index == 8 )  // latitude
  {
    str += "\"lat\"";
  }
  else if( index == 9 )  // longitude
  {
    str += "\"lng\"";
  }
  else if( index == 10 )  // created
  {
    str += "\"created\"";
  }
  else return "";

  str += ":\""; str += data; str += "\"";
  return str;
}
