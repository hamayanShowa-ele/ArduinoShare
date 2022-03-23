/*
  STM32Fxxx CPU Identity
  Copyright (c) 2022 Kazuaki Hamahara
    Released under the MIT license
    https://github.com/YukinobuKurata/YouTubeMagicBuyButton/blob/master/MIT-LICENSE.txt
*/
#include  "STM32F_CPU_Identity.h"

extern "C"{
}

String STM32F_CPU_IDENTITY::sysClockString()
{
  String msg = "systic:" + String( sysload(), DEC );
  msg += " hclk:"  + String( hclk(), DEC );
  msg += " pclk1:" + String( pclk1(), DEC );
  msg += " pclk2:" + String( pclk2(), DEC );
  msg += " sysclock:" + String( sysclk(), DEC );
  return msg;
}

String STM32F_CPU_IDENTITY::uIDtring()
{
  uint32_t uid[3];
  uID( uid );
  String msg = "UID0:0x" + String( uid[0], HEX );
  msg += " UID1:0x" + String( uid[1], HEX );
  msg += " UID2:0x" + String( uid[2], HEX );
  return msg;
}

/*
  constructor,destructor
*/

