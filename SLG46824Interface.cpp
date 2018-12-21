/**
  ******************************************************************************
  * @file    SLG46824 Interface.cpp 
  * @author  Avnet Silica
  * @brief   cpp interface for SLG46824 class
  *
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
