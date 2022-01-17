/**
  ******************************************************************************
  * @file           : WIZCHIP_RAW_MODE_SPI.cpp
  * @brief          : W5500 RAW MODE librarly.
    Copyright (c) 2022 Kazuaki Hamahara
    Released under the MIT license
    https://github.com/YukinobuKurata/YouTubeMagicBuyButton/blob/master/MIT-LICENSE.txt
  ******************************************************************************
  */

#include  <WIZCHIP_RAW_MODE_SPI.h>

//static SPISettings w5500_SPI_settings( 42000000UL / 2, MSBFIRST, SPI_MODE0 );
//static SPISettings w5500_SPI_settings( 36000000UL / 2, MSBFIRST, SPI_MODE0 );
//static SPISettings w5500_SPI_settings( 8000000UL, MSBFIRST, SPI_MODE0 );

static const char *phycfgr_opmdc_message[]
{
  "10BT Half-duplex, Auto-negotiation disabled",  // 0
  "10BT Full-duplex, Auto-negotiation disabled",  // 1
  "100BT Half-duplex, Auto-negotiation disabled",  // 2
  "100BT Full-duplex, Auto-negotiation disabled",  // 3
  "100BT Half-duplex, Auto-negotiation enabled",  // 4
  "Not used",  // 5
  "Power Down mode",  // 6
  "All capable, Auto-negotiation enabled",  // 7
};

static const char *phycfgr_dpx_message[]
{
  "Half duplex",  // 0
  "Full duplex"  // 1
};

static const char *phycfgr_spd_message[]
{
  "10Mpbs based",  // 0
  "100Mpbs based"  // 1
};

static const char *phycfgr_lnk_message[]
{
  "Link down",  // 0
  "Link up"  // 1
};

static const char over_range[] = "The argument is out of range.";

/*
 * network byte order converter.
 */
uint16_t hton( uint16_t ab )
{
  uint8_t a,b;
  a = ab >> 8;
  b = ab >> 0;
  return b << 8 | a << 0;
}
uint32_t hton( uint32_t abcd )
{
  uint8_t a,b,c,d;
  a = abcd >> 24;
  b = abcd >> 16;
  c = abcd >> 8;
  d = abcd >> 0;
  return d << 24 | c << 16 | b << 8 | a << 0;
}

/* ----------------------------------------
  int2Hex02
  Converts the argument num to hexadecimal characters
    and writes it to dest.
  The dest needs to be large enough.
---------------------------------------- */
char* int2Hex02( char *dest, uint8_t num )
{
  static const char hex[] = "0123456789ABCDEF";

  dest[ 0 ] = hex[ num / 16 ];
  dest[ 1 ] = hex[ num % 16 ];
  dest[ 2 ] = '\0';

  return dest;
}

/*
 * byte dump string.
 */
void dump( const uint8_t *data, uint32_t sz )
{
  String lines;
  int col = (sz / 16); if( col % 16 ) col++;
  for( int c = 0; c < col; c++ )  // column
  {
    lines = "  ";
    /* numerical dump */
    const uint8_t *dat = data;
    int row = (sz / 16) ? 16 : sz % 16;
    for( int r = 0; r < row; r++ )  // row
    {
      char buf[4];
      lines += int2Hex02( buf, *dat++ );
      lines += (r == 7) ? ", " : ",";
    }
    sz -= 16;
    lines += "\r\n";
    Serial.print( lines );
    data += 16;
  }
}

/*
 * softReset
 */
int WIZCHIP_RAW_MODE::softReset( uint32_t tmout )
{
  setMR( (uint8_t)(getMR() | COMMON_MR_RST_bit) );
  uint32_t baseTim = millis();
  while( true )
  {
    if( !(getMR() & COMMON_MR_RST_bit) ) break;
    if( (millis() - baseTim) >= tmout ) return (-1);
  }
  return 0;
}

/*
 * initW5500
 */
int WIZCHIP_RAW_MODE::initW5500( const uint8_t sz[2][MAX_SOCK_NUM], uint32_t tmout )
{
  initCS();
  if( softReset() ) return (-1);
  if( sz != nullptr )
  {
    for( int i = 0; i < MAX_SOCK_NUM; i++ )
    {
      setSnTxBufSize( (uint8_t)i, sz[0][ i ] );  // writeSnReg( (uint8_t)i, Sn_TXBUF_SIZE, sz[0][ i ] );
      setSnRxBufSize( (uint8_t)i, sz[1][ i ] );  // writeSnReg( (uint8_t)i, Sn_RXBUF_SIZE, sz[1][ i ] );
    }
  }
  uint32_t baseTim = millis();
  while( true )
  {
    if( linked() ) break;
    if( (millis() - baseTim) >= tmout ) return (-1);
  }
  /* set INTLEVEL IAWT=about 10us */
  setINTLEVEL( (uint16_t)312 );  // IAWT = (INTLEVEL + 1) * 8ns * 4
  /* set tcp retry time value is 100ms. unit is micro seconds. */
  setRTR( (uint16_t)(100 * 10) );  // value = time(ms) * 10
  /* set tcp retry count value is 3. 3 = 2 + 1*/
  setRCR( (uint8_t)2 );

  return 0;
}

#define  waitCSnegate()  delay(2)

/*
 * write register.
 */
void WIZCHIP_RAW_MODE::writeReg( uint16_t addr, uint8_t ctrl, uint8_t data )
{
  ctrl &= ~(RWB_MASK | OM_MASK);
  ctrl |= RWB_WRITE | OM_FDM_1;
  SPISettings w5500_SPI_settings( SPI2_PCLK, MSBFIRST, SPI_MODE0 );
  spi->beginTransaction( w5500_SPI_settings );
  setCS();
  spi->transfer( (uint8_t)(addr >> 8) );
  spi->transfer( (uint8_t)addr );
  spi->transfer( ctrl );
  spi->transfer( data );
  resetCS();
  spi->endTransaction();
  waitCSnegate();
}

void WIZCHIP_RAW_MODE::writeReg( uint16_t addr, uint8_t ctrl, uint16_t data )
{
  ctrl &= ~(RWB_MASK | OM_MASK);
  ctrl |= RWB_WRITE | OM_FDM_2;
  SPISettings w5500_SPI_settings( SPI2_PCLK, MSBFIRST, SPI_MODE0 );
  spi->beginTransaction( w5500_SPI_settings );
  setCS();
  spi->transfer( (uint8_t)(addr >> 8) );
  spi->transfer( (uint8_t)addr );
  spi->transfer( ctrl );
  spi->transfer( (uint8_t)(data >> 8) );
  spi->transfer( (uint8_t)data );
  resetCS();
  spi->endTransaction();
  waitCSnegate();
}

void WIZCHIP_RAW_MODE::writeReg( uint16_t addr, uint8_t ctrl, uint32_t data )
{
  ctrl &= ~(RWB_MASK | OM_MASK);
  ctrl |= RWB_WRITE | OM_FDM_4;
  SPISettings w5500_SPI_settings( SPI2_PCLK, MSBFIRST, SPI_MODE0 );
  spi->beginTransaction( w5500_SPI_settings );
  setCS();
  spi->transfer( (uint8_t)(addr >> 8) );
  spi->transfer( (uint8_t)addr );
  spi->transfer( ctrl );
  spi->transfer( (uint8_t)(data >> 24) );
  spi->transfer( (uint8_t)(data >> 16) );
  spi->transfer( (uint8_t)(data >> 8) );
  spi->transfer( (uint8_t)data );
  resetCS();
  spi->endTransaction();
  waitCSnegate();
}

void WIZCHIP_RAW_MODE::writeReg( uint16_t addr, uint8_t ctrl, const uint8_t *data, int sz )
{
  ctrl &= ~(RWB_MASK | OM_MASK);
  ctrl |= RWB_WRITE | OM_VDM;
  SPISettings w5500_SPI_settings( SPI2_PCLK, MSBFIRST, SPI_MODE0 );
  spi->beginTransaction( w5500_SPI_settings );
  setCS();
  spi->transfer( (uint8_t)(addr >> 8) );
  spi->transfer( (uint8_t)addr );
  spi->transfer( ctrl );
  for( ; sz > 0; sz-- ) { spi->transfer( *data++ ); }
  resetCS();
  spi->endTransaction();
  waitCSnegate();
}

/*
 * read register.
 */
uint8_t WIZCHIP_RAW_MODE::readReg( uint16_t addr, uint8_t ctrl )
{
  ctrl &= ~(RWB_MASK | OM_MASK);
  ctrl |= RWB_READ | OM_FDM_1;
  SPISettings w5500_SPI_settings( pClk / 2, MSBFIRST, SPI_MODE0 );
  spi->beginTransaction( w5500_SPI_settings );
  setCS();
  spi->transfer( (uint8_t)(addr >> 8) );
  spi->transfer( (uint8_t)addr );
  spi->transfer( ctrl );
  uint8_t tempUC = spi->transfer(0);
  resetCS();
  spi->endTransaction();
  waitCSnegate();
  return tempUC;
}

uint16_t WIZCHIP_RAW_MODE::readReg( uint16_t addr, uint8_t ctrl, uint16_t *data )
{
  ctrl &= ~(RWB_MASK | OM_MASK);
  ctrl |= RWB_READ | OM_FDM_2;
  SPISettings w5500_SPI_settings( SPI2_PCLK, MSBFIRST, SPI_MODE0 );
  spi->beginTransaction( w5500_SPI_settings );
  setCS();
  spi->transfer( (uint8_t)(addr >> 8) );
  spi->transfer( (uint8_t)addr );
  spi->transfer( ctrl );
  uint16_t tempUS = spi->transfer(0) << 8;
  tempUS |= spi->transfer(0);
  resetCS();
  spi->endTransaction();
  waitCSnegate();
  if( data != nullptr ) *data = tempUS;
  return tempUS;
}

uint32_t WIZCHIP_RAW_MODE::readReg( uint16_t addr, uint8_t ctrl, uint32_t *data )
{
  ctrl &= ~(RWB_MASK | OM_MASK);
  ctrl |= RWB_READ | OM_FDM_4;
  SPISettings w5500_SPI_settings( SPI2_PCLK, MSBFIRST, SPI_MODE0 );
  spi->beginTransaction( w5500_SPI_settings );
  setCS();
  spi->transfer( (uint8_t)(addr >> 8) );
  spi->transfer( (uint8_t)addr );
  spi->transfer( ctrl );
  uint32_t tempUL;
  tempUL =  spi->transfer(0) << 24;
  tempUL |= spi->transfer(0) << 16;
  tempUL |= spi->transfer(0) << 8;
  tempUL |= spi->transfer(0);
  resetCS();
  spi->endTransaction();
  waitCSnegate();
  if( data != nullptr ) *data = tempUL;
  return tempUL;
}

void WIZCHIP_RAW_MODE::readReg( uint16_t addr, uint8_t ctrl, uint8_t *data, int sz )
{
  ctrl &= ~(RWB_MASK | OM_MASK);
  ctrl |= RWB_READ | OM_VDM;
  SPISettings w5500_SPI_settings( SPI2_PCLK, MSBFIRST, SPI_MODE0 );
  spi->beginTransaction( w5500_SPI_settings );
  setCS();
  spi->transfer( (uint8_t)(addr >> 8) );
  spi->transfer( (uint8_t)addr );
  spi->transfer( ctrl );
  for( ; sz > 0; sz-- ) { *data++ = spi->transfer(0); }
  resetCS();
  spi->endTransaction();
  waitCSnegate();
}


const char* WIZCHIP_RAW_MODE::get_opmdc_message( int num )
{
  if( num >= (int)(sizeof(phycfgr_opmdc_message) / sizeof(phycfgr_opmdc_message[0])) ) return over_range;
  return phycfgr_opmdc_message[num];
}
const char* WIZCHIP_RAW_MODE::get_dpx_message( int num )
{
  if( num >= (int)(sizeof(phycfgr_dpx_message) / sizeof(phycfgr_dpx_message[0])) ) return over_range;
  return phycfgr_dpx_message[num];
}
const char* WIZCHIP_RAW_MODE::get_spd_message( int num )
{
  if( num >= (int)(sizeof(phycfgr_spd_message) / sizeof(phycfgr_spd_message[0])) ) return over_range;
  return phycfgr_spd_message[num];
}
const char* WIZCHIP_RAW_MODE::get_lnk_message( int num )
{
  if( num >= (int)(sizeof(phycfgr_lnk_message) / sizeof(phycfgr_lnk_message[0])) ) return over_range;
  return phycfgr_lnk_message[num];
}

String WIZCHIP_RAW_MODE::linkMessage()
{
  uint8_t phycfg = getPHYCFGR();

  String msg = "LINK STATUS : \r\n  ";
  msg += get_opmdc_message( (phycfg & COMMON_PHYCFGR_OPMDC_bit) >> 3 ); msg += "\r\n  ";
  msg += get_dpx_message( (phycfg & COMMON_PHYCFGR_DPX_bit) >> 2 ); msg += "\r\n  ";
  msg += get_spd_message( (phycfg & COMMON_PHYCFGR_SPD_bit) >> 1 ); msg += "\r\n  ";
  msg += get_lnk_message( (phycfg & COMMON_PHYCFGR_LNK_bit) >> 0 ); msg += "\r\n";
  return msg;
}

String WIZCHIP_RAW_MODE::socketStatusMessage( uint8_t val )
{
  struct TYPES
  {
    uint8_t value;
    char *msg;
  } types[] =
  {
    {0x00,"SOCK_CLOSED"},
    {0x13,"SOCK_INIT"},
    {0x14,"SOCK_LISTEN"},
    {0x17,"SOCK_ESTABLISHED"},
    {0x1C,"SOCK_CLOSE_WAIT"},
    {0x22,"SOCK_UDP"},
    {0x42,"SOCK_MACRAW"},
    {0x15,"SOCK_SYNSENT"},
    {0x16,"SOCK_SYNRECV"},
    {0x18,"SOCK_FIN_WAIT"},
    {0x1A,"SOCK_CLOSING"},
    {0x1B,"SOCK_TIME_WAIT"},
    {0x1D,"SOCK_LAST_ACK"},
  };

  int i;
  for( i = 0; i < (int)(sizeof(types) / sizeof(types[0])); i++ )
  {
    if( val == types[i].value ) break;
  }
  if( i == (int)(sizeof(types) / sizeof(types[0])) ) return "";
  return types[ i ].msg;
}


/*
 * write ether frame to socket tx buffer.
 */
uint16_t WIZCHIP_RAW_MODE::writeEther(
  uint8_t sn, const uint8_t *dst, uint16_t typ, const uint8_t *payload, int sz )
{
  uint16_t free = getSnTxFSR( sn );
  if( free < (sz + 14) ) return 0;
  int total = ((sz + 14) < 46) ? 46 : (sz + 14);

  uint8_t src[ MAC_BYTE_COUNT ];
  getSHAR( src );
#if 0
  writeSnTxData( sn, dst, MAC_BYTE_COUNT );  // destination hardware address
  writeSnTxData( sn, src, MAC_BYTE_COUNT );  // source hardware address
  writeSnTxData( sn, (const uint8_t *)&type, sizeof(uint16_t) );  // frame type
  writeSnTxData( sn, payload, sz );  // payload and payload size
#else

  int len = 0;
  uint8_t *buf = new uint8_t[ total ];
  /* set destination hardware address. */
  memcpy( buf + len, dst, MAC_BYTE_COUNT );
  len += MAC_BYTE_COUNT;
  /* set source hardware address. */
  memcpy( buf + len, src, MAC_BYTE_COUNT );
  len += MAC_BYTE_COUNT;
  /* set ethernet type. */
  typ = hton( typ );
  memcpy( buf + len, &typ, sizeof(uint16_t) );
  len += sizeof(uint16_t);
  /* add payload data. */
  memcpy( buf + len, payload, sz );
  len += sz;
#if 0  // Do not need the FCS.
  uint32_t crc32 = crc32Right( len, buf );  // right shifted crc32. CRC-32
//  uint32_t crc32 = crc32Left( len, buf );  // left shifted crc32. CRC-32/BZIP2
  crc32 = hton( crc32 );
  memcpy( buf + len, &crc32, sizeof(uint32_t) );
  len += sizeof(uint32_t);
#endif
  /* zero padding */
  if( (sz + 14) < 46 )
  {
    for( int i = len; i < total; i++ ) { buf[i] = 0; }
  }
  dump( buf, total );
  writeSnTxData( sn, (const uint8_t *)buf, total );  //
  delete [] buf;
#endif
  SnSend( sn );

  return sz;
}

/*
 * read ether frame from socket rx buffer.
 */
uint16_t WIZCHIP_RAW_MODE::readEther(
  uint8_t sn, uint8_t *dst, uint16_t *typ, uint8_t *payload, int sz )
{
//  uint16_t remain = snRxAvailable( sn );
//  if( remain < (sz + 14) ) return 0;
  /* get ethernet header. */
  t_ether2_header header;
  readSnRxData( sn, (uint8_t *)&header, (int)sizeof(t_ether2_header) );
  memcpy( dst, header.src, sizeof(header.src) );
  *typ = (header.typ[0] << 8) | (header.typ[1] << 0);
  /* get ethernet payload. */
  readSnRxData( sn, payload, sz );

  return sz;
}

/*
 * is socket rx buffer available ?
 */
uint16_t WIZCHIP_RAW_MODE::snRxAvailable( uint8_t sn )
{
  if( !getSnRxRSR( sn ) ) return 0;
  uint16_t remain;
  readSnRxData( sn, (uint8_t *)&remain, sizeof(uint16_t) );

  return hton( remain );
}

extern "C"
{
/*
  crc32.c -- CRC
*/
#define CRCPOLY1 0x04C11DB7UL
    /* x^{32}+x^{26}+x^{23}+x^{22}+x^{16}+x^{12}+x^{11]+
       x^{10}+x^8+x^7+x^5+x^4+x^2+x^1+1 */
#define CRCPOLY2 0xEDB88320UL  /* ×óÓÒÄæÜ */

uint32_t crc32Left( int n, uint8_t c[] )  // left shifted.
{
    unsigned int i, j;
    uint32_t r;

    r = 0xFFFFFFFFUL;
    for (i = 0; i < n; i++) {
        r ^= (uint32_t)c[i] << (32 - CHAR_BIT);
        for (j = 0; j < CHAR_BIT; j++)
            if (r & 0x80000000UL) r = (r << 1) ^ CRCPOLY1;
            else                  r <<= 1;
    }
    return ~r & 0xFFFFFFFFUL;
}

uint32_t crc32Right( int n, uint8_t c[] )  // right shifted.
{
    unsigned int i, j;
    uint32_t r;

    r = 0xFFFFFFFFUL;
    for (i = 0; i < n; i++) {
        r ^= c[i];
        for (j = 0; j < CHAR_BIT; j++)
            if (r & 1) r = (r >> 1) ^ CRCPOLY2;
            else       r >>= 1;
    }
    return r ^ 0xFFFFFFFFUL;
}

}  /* extern "C" */
