/*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 02 Sept 2015
 by Arturo Guadalupi
 
 */
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

#include  <STM32_FSMC.h>
#include  <Ethernet5k3.h>
#include  <bd1401a.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01, 0x01 };

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 100, 128);
IPAddress myDns(192, 168, 100, 1);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

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

  // Open serial communications and wait for port to open:
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
  // start the Ethernet connection and the server:
  Ethernet.begin( mac, ip );
  // Check for Ethernet hardware present
  if( Ethernet.hardwareStatus() == EthernetNoHardware )
  {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while( true ) { delay( 1000UL ); }   // do nothing, no point running without Ethernet hardware
  }
  if( Ethernet.linkStatus() == LinkOFF )
  {
    Serial.println("Ethernet cable is not connected.");
    while( true ) { delay( 1000UL ); }  // do nothing, no point running without Ethernet hardware
  }

  // start the server
  server.begin();
  Serial.print("server is at "); Serial.println(Ethernet.localIP());
}

/**
  * @brief  loop
  * @retval none
  */
int sensorReading = 1;
void loop()
{
  delay( 500UL );
  // listen for incoming clients
  EthernetClient client = server.available();
  if( client )
  {
    Serial.println("new client");
    // an http request ends with a blank line
    bool currentLineIsBlank = true;
    while( client.connected() )
    {
      if( client.available() )
      {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if( c == '\n' && currentLineIsBlank )
        {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          for( int analogChannel = 0; analogChannel < 6; analogChannel++ )
          {
//            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading); sensorReading++;
            client.println("<br />");
          }
          client.println("</html>");
          break;
        }
        if (c == '\n')
        {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r')
        {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
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
  while (1) { }
  /* USER CODE END Error_Handler_Debug */
}
