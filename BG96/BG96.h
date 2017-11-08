/* SPWFInterface Example
 * Copyright (c) 2015 ARM Limited
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
 
#ifndef BG96_H
#define BG96_H

 
#include "ATParser.h"
 
/** BG96Interface class.
    This is an interface to a BG96 module.
 */
class BG96
{
public:
    
    BG96(PinName tx, PinName rx, PinName reset=NC, PinName wakeup=NC, bool debug=false);
    
    /**
    * Init the BG96
    *
    * @param mode mode in which to startup
    * @return true only if BG96 has started up correctly
    */
    bool startup(int mode);
 
    void waitBG96Ready(void);
    /**
    * Reset BG96
    *
    * @return true only if BG96 resets successfully
    */
    bool reset(void);
    
    bool hw_reset(void);
 
    /**
    * Enable/Disable DHCP
    *
    * @param mode mode of DHCP 2-softAP, 1-on, 0-off
    * @return true only if BG96 enables/disables DHCP successfully
    */
    bool dhcp(int mode);
 
    /**
    * Connect BG96 to AP
    *
    * @param ap the name of the AP
    * @param passPhrase the password of AP
    * @param securityMode the security mode of AP (WPA/WPA2, WEP, Open)
    * @return true only if BG96 is connected successfully
    */
    bool connect(const char *apn, const char *username, const char *password);
 
    /**
    * Disconnect BG96 from AP
    *
    * @return true only if BG96 is disconnected successfully
    */
    bool disconnect(void);
 
    /**
    * Get the IP address of BG96
    *
    * @return null-teriminated IP address or null if no IP address is assigned
    */
    const char *getIPAddress(void);
 
    /**
    * Get the MAC address of BG96
    *
    * @return null-terminated MAC address or null if no MAC address is assigned
    */
    const char *getMACAddress(void);
 
    /**
    * Check if BG96 is conenected
    *
    * @return true only if the chip has an IP address
    */
    bool isConnected(void);
 
    /**
    * Open a socketed connection
    *
    * @param type the type of socket to open "u" (UDP) or "t" (TCP)
    * @param id id to get the new socket number, valid 0-7
    * @param port port to open connection with
    * @param addr the IP address of the destination
    * @return true only if socket opened successfully
    */
    bool open(const char *type, int* id, const char* addr, int port);
 
    /**
    * Sends data to an open socket
    *
    * @param id id of socket to send to
    * @param data data to be sent
    * @param amount amount of data to be sent - max 1024
    * @return true only if data sent successfully
    */
    bool send(int id, const void *data, uint32_t amount);
 
    /**
    * Receives data from an open socket
    *
    * @param id id to receive from
    * @param data placeholder for returned information
    * @param amount number of bytes to be received
    * @return the number of bytes received
    */
    int32_t recv(int id, void *data, uint32_t amount);
 
    /**
    * Closes a socket
    *
    * @param id id of socket to close, valid only 0-4
    * @return true only if socket is closed successfully
    */
    bool close(int id);
 
    /**
    * Checks if data is available
    */
    bool readable();
 
    /**
    * Checks if data can be written
    */
    bool writeable();
 
private:
    BufferedSerial _serial;
    ATParser _parser;
    DigitalInOut _wakeup;
    DigitalInOut _reset; 
    char _ip_buffer[16];
    char _mac_buffer[18];
    bool dbg_on;
//    int _timeout; // FIXME LICIO we have "virtual" socket tmo, module socket tmo, 
// AT parser tmo, recv/send tmo, actually used the NetworksocketAPI socket tmo
    unsigned int _recv_timeout; // see SO_RCVTIMEO setsockopt
    unsigned int _send_timeout; // see SO_SNDTIMEO setsockopt   
    unsigned int socket_closed;
};
 
#endif  //BG96_H

