/*
  Arduino IDE setup.
  Board type : Generic STM32F4 Generic F407ZETx
  U(S)ART support : Enable (No generic Serial).
  USB support : CDC (generic Serial supersede U(S)ART).
  USB speed : Low/Full Speed.
  Optimize : ANY.
  C Runtime Library : ANY.
  Update method : STM32CubeProgrammer (SWD).
  Serial port : ANY.
*/
/**
  * @brief Add the following definition to the beginning of HardwareSerial.h in advance.
          #define SERIAL_TX_BUFFER_SIZE 256
          #define SERIAL_RX_BUFFER_SIZE 256
  */

//#include  <HardwareSerial.h>
#include  <DEBUG_UTIL.h>  // https://github.com/hamayanShowa-ele/ArduinoShare/tree/main/libraries/DEBUG_UTIL
#include  <STM32F_CPU_Identity.h>  // https://github.com/hamayanShowa-ele/ArduinoShare/tree/main/libraries/STM32F_CPU_Identity
#include  <STM32_FSMC.h>
#include  <Ethernet5k3.h>
#include  <EthernetUdp2.h>
//#include  <socket.h>
#include  <bd1401a.h>

#define  TIME_ZONE  (9 * 3600UL)
#define  TZ  TIME_ZONE

static const byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01, 0x01 };
#if 0
static const IPAddress local_ip( 192,168,100,253 );
static const IPAddress dns_server( 192,168,100,1 );
static const IPAddress gateway( 192,168,100,1 );
static const IPAddress subnet( 255,255,255,0 );
#endif
IPAddress myIP;
unsigned int localPort = 8888;       // local port to listen for UDP packets

static const char timeServer[] = "ntp.nict.jp"; // "time.nist.gov"; // time.nist.gov NTP server
static const IPAddress timeServerIp = {133,243,238,244}; // "time.nist.gov"; // time.nist.gov NTP server
static const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
static byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

/**
  * @brief GPIO initialize.
  * @retval none.
  */
static void initGPIO()
{
  pinMode( LED_RED, OUTPUT );
  pinMode( LED_BLUE, OUTPUT );
  pinMode( LED_GREEN, OUTPUT );
  pinMode( LED_BUILTIN, OUTPUT );
  // initialize usb power on pin USBON as an output.
  pinMode( USBON, OUTPUT );
  digitalWrite( USBON, USB_POWER_ON_LEVEL );
  // reset i/o.
  pinMode( IO_RST, OUTPUT );
  digitalWrite( IO_RST, LOW );
  delay( 10UL );
  digitalWrite( IO_RST, HIGH );
}

/**
  * @brief  setup
  * @retval none
  */
void setup()
{
  // GPIO initialize.
  initGPIO();

  // USB/Serial initialize.
  Serial.begin( 115200UL );
  while( !Serial ) { delay( 20 ); }
  Serial.println( "1401A W5300 make the Arduino library from USB Serial." );
  delay( 100UL );

  // External CPU BUS initialize.
  fsmc_address( 19 );  // set address bus size
  fsmc_data( 16 );  // set data bus size.
  fsmc_sram( FSMC_NORSRAM_BANK2, FSMC_NORSRAM_MEM_BUS_WIDTH_16, 0,12,0,9, 0 );

  // Ethernet initialize.
  Ethernet.init( W5300_MR_ADDRESS, W5300_IDM_AR_ADDRESS, W5300_IDM_DR_ADDRESS );
#if 0
  Ethernet.begin( (uint8_t *)mac, local_ip, dns_server, gateway, subnet );
  myIP = local_ip;
#else
  if( Ethernet.begin( mac ) == 0 )
  {
    Serial.println( "Failed to configure Ethernet using DHCP" );
    // Check for Ethernet hardware present
    // no point in carrying on, so do nothing forevermore:
    while (true) { delay( 1000 ); }
  }
  myIP = Ethernet.localIP();
  Serial.print( "\r\nIP number assigned by DHCP is " ); Serial.println( myIP );
#endif

  Udp.begin( localPort );
}

/**
  * @brief  loop
  * @retval none
  */
void loop()
{
  sendNTPpacket( timeServer ); // send an NTP packet to a time server by url strings.
//  sendNTPpacket( timeServerIp ); // send an NTP packet to a time server by ip address.

  delay( 1000 );

  int recvLen;
  if( (recvLen = Udp.parsePacket()) )
  {
    // We've received a packet, read the data from it
    int ret = Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
    if( ret > 0 )
    {
      Serial.printf( "ntp packet recieved length = %d\r\n", recvLen );
      dump( packetBuffer, NTP_PACKET_SIZE );

      // the timestamp starts at byte 40 of the received packet and is four bytes,
      // or two words, long. First, extract the two words:
      unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
      unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
      // combine the four bytes (two words) into a long integer
      // this is NTP time (seconds since Jan 1 1900):
      unsigned long secsSince1900 = highWord << 16 | lowWord;
      Serial.print("Seconds since Jan 1 1900 = ");
      Serial.println(secsSince1900);
      // now convert NTP time into everyday time:
      Serial.print("Unix time = ");
      // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
      const unsigned long seventyYears = 2208988800UL;
      // subtract seventy years:
      unsigned long epoch = secsSince1900 - seventyYears;
      // print Unix time:
      Serial.println(epoch);
      // print the hour, minute and second:
      epoch += TZ;
      Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
      Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
      Serial.print(':');
      if( ((epoch % 3600) / 60) < 10 )
      {
        // In the first 10 minutes of each hour, we'll want a leading '0'
        Serial.print('0');
      }
      Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
      Serial.print(':');
      if( (epoch % 60) < 10 )
      {
        // In the first 10 seconds of each minute, we'll want a leading '0'
        Serial.print('0');
      }
      Serial.println(epoch % 60); // print the second
    }
    else
    {
      Serial.printf( "ntp packet recieved, but error %d\r\n", ret );
    }
  }

  // wait ten seconds before asking for the time again
  delay( 10000 );
  Ethernet.maintain();
  digitalWrite( LED_BUILTIN, (digitalRead(LED_BUILTIN) == LOW) ? HIGH : LOW );
}

// send an NTP request to the time server at the given address
void sendNTPpacket( const char *address )
{
  // set all bytes in the buffer to 0
  memset( packetBuffer, 0, NTP_PACKET_SIZE );
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket( address, 123 ); // NTP requests are to port 123
  Udp.write( packetBuffer, NTP_PACKET_SIZE );
  Udp.endPacket();
}

//void sendNTPpacket( const uint8_t *address )
void sendNTPpacket( IPAddress address )
{
  // set all bytes in the buffer to 0
  memset( packetBuffer, 0, NTP_PACKET_SIZE );
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket( address, 123 ); // NTP requests are to port 123
  Udp.write( packetBuffer, NTP_PACKET_SIZE );
  Udp.endPacket();
}

/**
  * @brief System Clock Configuration
  * @retval None
  * @attention
  * Be sure to refer to this site to make changes to stm32yyxx_hal_conf.h.
  * https://www.stm32duino.com/viewtopic.php?p=8819
  * 
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    user_Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    user_Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void user_Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
