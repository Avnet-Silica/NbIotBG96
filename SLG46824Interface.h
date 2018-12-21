/**
  ******************************************************************************
  * @file    SLG46824Interface.h
  * @author  Avnet Silica
  * @brief   header file cpp interface for SLG46824 class
  *
  ******************************************************************************
  */
  
#ifndef SLG46824_INTERFACE_H
#define SLG46824_INTERFACE_H


#include "SLG46824_driver.h"

  
/** BG96SAInterface class
 *  Implementation of the NetworkStack for the BG96 Device
 */
class SLG46824Interface 			//: public NetworkStack, public CellularInterface
{
public:
    SLG46824Interface(PinName tx, PinName rx);
 
    virtual     ~SLG46824Interface();
 

    //Implementation of NetworkStack
    virtual     bool get_i2c_address(uint8_t *addr);
		virtual     int  hw_set(void);
		virtual			void startup(void);
		virtual			void test_task(void);

   
protected:

				
private:
       
    SLG46824 			_SLG46824;

};


#endif	//SLG46824


