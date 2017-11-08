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
  * @file    BG96Interface.h 
  * @author  STMicroelectronics
  * @brief   Header file of the NetworkStack for the BG96 Device
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
  
#ifndef BG96SA_INTERFACE_H
#define BG96SA_INTERFACE_H

#include "CellularInterface.h"
#include "BG96.h"
 
#define BG96SA_SOCKET_COUNT 8
#define SERVER_SOCKET_NO    9
  
/** BG96SAInterface class
 *  Implementation of the NetworkStack for the BG96 Device
 */
class BG96Interface : public NetworkStack, public CellularInterface
{
public:
    BG96Interface(PinName tx, PinName rx, bool debug);
    BG96Interface(PinName tx, PinName rx, PinName reset = PC_12, PinName wakeup = PC_8, bool debug = false);    
    virtual     ~BG96Interface();
 
    // Implementation of WiFiInterface
    virtual     int connect(														
														const char *apn,
													  const char *username,
														const char *password);
 
    virtual     int disconnect();    
    virtual     const char *get_mac_address();    
    void        debug(const char * string);
    
    //Implementation of NetworkStack
    virtual     const char *get_ip_address();
    int         init(void);														
    
protected:
    //Implementation of NetworkStack
    virtual     int socket_open(void **handle, nsapi_protocol_t proto);    
    virtual     int socket_close(void *handle);    
    virtual     int socket_bind(void *handle, const SocketAddress &address);  //not supported  
    virtual     int socket_listen(void *handle, int backlog);
    virtual     int socket_connect(void *handle, const SocketAddress &address);
    virtual     int socket_accept(void **handle, void *server);
    virtual     int socket_send(void *handle, const void *data, unsigned size);  
    virtual     int socket_recv(void *handle, void *data, unsigned size);    
    virtual     int socket_sendto(void *handle, const SocketAddress &address, const void *data, unsigned size);    
    virtual     int socket_recvfrom(void *handle, SocketAddress *address, void *buffer, unsigned size);    
    virtual     void socket_attach(void *handle, void (*callback)(void *), void *data);
    virtual     int setsockopt(void *handle, int level, int optname, const void *optval, unsigned optlen);    
    virtual     int getsockopt(void *handle, int level, int optname, void *optval, unsigned *optlen);    
				
private:
       
    BG96 _BG96;
    bool _ids[BG96SA_SOCKET_COUNT];
    bool isListening;
    bool isInitialized;
};


#endif


