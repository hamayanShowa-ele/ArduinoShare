/**
  ******************************************************************************
  * @file           : DEBUG_UTIL.cpp
  * @brief          : DEBUG_UTIL code
  */
/* Includes ------------------------------------------------------------------*/
#include "DEBUG_UTIL.h"

extern "C"
{
}  /* extern "C" */

/**
  * @brief memory dump program
  * @retval none.
  */
void dump( const uint8_t *dat, int size )
{
  if( size % 16 ) { size = size / 16; size++; size *= 16; }
  for( int i = 0; i < size; i += 16 )
  {
    String line = "";
    char buf[16];
    /* display address. */
    sprintf( buf, "%08X : ", (uint32_t)dat );
    line += buf;
    /* display hex data. */
    const uint8_t *ptr = dat;
    for( int j = 0; j < 16; j++ )
    {
      if( j == 8 ) sprintf( buf, " %02X,", *ptr );
      else sprintf( buf, "%02X,", *ptr );
      line += buf;
      ptr++;
    }
    /* display characters. */
    line += "    ";
    for( int j = 0; j < 16; j++ )
    {
      if( j == 8 ) sprintf( buf, " %c", isprint(*dat) ? *dat : '.' );
      else sprintf( buf, "%c", isprint(*dat) ? *dat : '.' );
      line += buf;
      dat++;
    }
    Serial.println( line );
  }
  Serial.println();
}

void dump( const uint16_t *dat, int size )
{
  if( size % 16 ) { size = size / 16; size++; size *= 16; }
  size /= 2;
  for( int i = 0; i < size; i += 8 )
  {
    String line = "";
    char buf[16];
    sprintf( buf, "%08X : ", (uint32_t)dat );
    line += buf;
    for( int j = 0; j < 8; j++ )
    {
      sprintf( buf, "%04X,", *dat++ );
      line += buf;
    }
    Serial.println( line );
  }
  Serial.println();
}

void dump( const uint32_t *dat, int size )
{
  if( size % 16 ) { size = size / 16; size++; size *= 16; }
  size /= 4;
  for( int i = 0; i < size; i += 4 )
  {
    String line = "";
    char buf[16];
    sprintf( buf, "%08X : ", (uint32_t)dat );
    line += buf;
    for( int j = 0; j < 4; j++ )
    {
      sprintf( buf, "%08X,", *dat++ );
      line += buf;
    }
    Serial.println( line );
  }
  Serial.println();
}

/**
  * @brief memory check program
  * @retval Return 0 on success. If it fails, the address where the error occurred.
  */
uint32_t memCheck( uint8_t *dat, uint32_t size )
{
  if( size == 0 ) return 0;
  uint8_t *ptr = dat;
  uint32_t result = 0;
  /* memory clear */
  for( uint32_t i = 0; i < size; i++ ) { *ptr++ = 0; }
  /* memory ramdom write */
  ptr = dat;
  srand( 4649 );
  for( uint32_t i = 0; i < size; i++ ) { *ptr++ = (uint8_t)rand(); }
  /* memory check */
  ptr = dat;
  srand( 4649 );
  for( uint32_t i = 0; i < size; i++ )
  {
    uint8_t r = rand();
    if( r != *ptr )
    {
      result = (uint32_t)ptr;
      break;
    }
    ptr++;
  }
  return result;
}

uint32_t memCheck( uint16_t *dat, uint32_t size )
{
  size /= 2;
  if( size == 0 ) return 0;
  uint16_t *ptr = dat;
  uint32_t result = 0;
  /* memory clear */
  for( uint32_t i = 0; i < size; i++ ) { *ptr++ = 0; }
  /* memory ramdom write */
  ptr = dat;
  srand( 4649 );
  for( uint32_t i = 0; i < size; i++ ) { *ptr++ = (uint16_t)rand(); }
  /* memory check */
  ptr = dat;
  srand( 4649 );
  for( uint32_t i = 0; i < size; i++ )
  {
    uint16_t r = rand();
    if( r != *ptr )
    {
      result = (uint32_t)ptr;
      break;
    }
    ptr++;
  }
  return result;
}

uint32_t memCheck( uint32_t *dat, uint32_t size )
{
  size /= 4;
  if( size == 0 ) return 0;
  uint32_t *ptr = dat;
  uint32_t result = 0;
  /* memory clear */
  for( uint32_t i = 0; i < size; i++ ) { *ptr++ = 0; }
  /* memory ramdom write */
  ptr = dat;
  srand( 4649 );
  for( uint32_t i = 0; i < size; i++ ) { *ptr++ = (uint32_t)rand(); }
  /* memory check */
  ptr = dat;
  srand( 4649 );
  for( uint32_t i = 0; i < size; i++ )
  {
    uint32_t r = rand();
    if( r != *ptr )
    {
      result = (uint32_t)ptr;
      break;
    }
    ptr++;
  }
  return result;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
