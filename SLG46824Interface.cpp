/* mbed Microcontroller Library
* Copyright (c) 20015 ARM Limited
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/**
  ******************************************************************************
  * @file    BG96Interface.cpp 
  * @author  STMicroelectronics
  * @brief   Implementation of the NetworkStack for the BG96 Device
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  ******************************************************************************
  */
  
#include "SLG46824Interface.h"
 
// Various timeouts for different BG96 operations
// NO NEEDED HERE!!
/*
#define BG96_CONNECT_TIMEOUT 20000
#define BG96_SEND_TIMEOUT    500
#define BG96_RECV_TIMEOUT    500
#define BG96_MISC_TIMEOUT    15000
*/


/**
* @brief  BG96SAInterface constructor         
* @param  tx: Pin USART TX  
*         rx: Pin USART RX
*         rst: reset pin for BG96 module
*         wkup: reset pin for BG96 module
*         rts: Pin USART RTS
*         debug : not used
* @retval none
*/
SLG46824Interface::SLG46824Interface(PinName tx, PinName rx)     
    : _SLG46824(tx, rx)
{
 
}

   

/**
* @brief  BG96SAInterface destructor         
* @param  none
* @retval none
*/
SLG46824Interface::~SLG46824Interface()
{
}

/**
* @brief  init function
          initializes BG96 FW and module         
* @param  none
* @retval error value
*/
int SLG46824Interface::hw_set(void) 
{

   return _SLG46824.hw_set();
}

/**
* @brief  init function
          initializes BG96 FW and module         
* @param  none
* @retval error value
*/
void SLG46824Interface::startup(void) 
{

   _SLG46824.startup();
}


/** 
* @brief  Get the local I2C address
* @param  none
* @retval Null-terminated representation of the local IP address
*         or null if not yet connected
*/
bool SLG46824Interface::get_i2c_address(uint8_t *addr)
{
    return _SLG46824.getI2CAddress(addr);
}



void SLG46824Interface::test_task(void) 
{

   _SLG46824.test_task();
}
