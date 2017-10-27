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

#include "BG96.h"
#include "mbed_debug.h"

#define BG96_CONNECT_TIMEOUT    15000
#define BG96_SEND_TIMEOUT       250
#define BG96_RECV_TIMEOUT       250 //some commands like AT&F/W takes some time to get the result back!
#define BG96_MISC_TIMEOUT       1000
#define BG96_SOCKQ_TIMEOUT      3000

DigitalOut BG96_reset(D7);

DigitalOut VBAT_3V8_EN(D11);

//DigitalOut BG96_W_DISABLE(D11);

DigitalOut BG96_PWRKEY(D10);



BG96::BG96(PinName tx, PinName rx, PinName reset, PinName wakeup, bool debug)
    : _serial(tx, rx, 1024), _parser(_serial),
      _reset(reset, PIN_OUTPUT, PullNone, 1),
      _wakeup(wakeup, PIN_OUTPUT, PullNone, 0),
      dbg_on(debug)
{
    _serial.baud(115200);  // LICIO  FIXME increase the speed
    _parser.debugOn(debug);
}

bool BG96::startup(int mode)
{
    _parser.setTimeout(BG96_MISC_TIMEOUT);
    /*Test module before reset*/
    waitBG96Ready();
    wait_ms(10000);
    _parser.send("ATE0");
    while (1) {
        if (_parser.recv("OK"))
            break;
    }

    _parser.send("AT+QIACT=1\r");
    while (1) {
        if (_parser.recv("OK"))
            break;
    }

	#if 0
    _parser.send("AT+QIACT?\r");
    while (1) {
        if (_parser.recv("OK"))
            break;
    }
    #endif

    /*Reset module*/
    _parser.flush();
    //reset();

    return true;
}

bool BG96::hw_reset(void)
{
    wait_ms(500);

    BG96_reset = 1;

    VBAT_3V8_EN = 1;

//    BG96_W_DISABLE = 0;
    printf("HW reset BG96\r\n");

    BG96_PWRKEY = 1;

    //wait( 0.3 );
    wait_ms(400);

    BG96_reset = 0;

    //wait( 0.1 );
    wait_ms(150);

    return 1;
}

bool BG96::reset(void)
{
        
    printf("HW reset BG96\r\n");
    hw_reset();
    return true;
}

void BG96::waitBG96Ready(void)
{
    reset();
    printf("Wait for ready...\r\n");
    wait_ms(50);
    while(1) {
        if (_parser.recv("RDY\r")) {
            //wait_ms(15000);
        	printf("Reset done\r\n");
            return ;
        }
    }

}

bool BG96::connect(const char *apn, const char *username, const char *password)
{
    //TODO: add APN mobile network access configuration if needed
    return true;
}

bool BG96::disconnect(void)
{
    //TODO add needed further action after ACT close command ...
    if(!(_parser.send("AT+QIDEACT=%d\r", 1) && _parser.recv("OK"))) {
        debug_if(dbg_on, "BG96> error while disconnet...\r\n");
        return false;
    }
    //reset();
    return true;
}

const char *BG96::getIPAddress(void)
{
    char act_string[32];
    //IPV4 mode
    uint8_t n1, n2, n3, n4;

    if (!(_parser.send("AT+QIACT?")
            && _parser.recv("+QIACT: %s", &act_string[0])
            && _parser.recv("OK"))) {
        debug_if(dbg_on, "BG96> getIPAddress error\r\n");
        return 0;
    }
    //TODO parse string to find IP ...
    //now this functions returns always IPV4 0.0.0.0
    n1 = n2 = n3 = n4 = 0;
    sprintf((char*)_ip_buffer,"%u.%u.%u.%u", 	n1, n2, n3, n4);

    return _ip_buffer;
}


#define ID_ON_FLASH		0

#define IMEI_OFFS			2
#define	IMEI_LEN 			14+IMEI_OFFS

const char *BG96::getMACAddress(void)
{

    /* 00 0b 57 00 db 46 for Avnet Bluemix*/
    /* 00 80 e1 b7 d3 37 or 47 for Ibm Quickstart */
    uint32_t n1, n2, n3, n4, n5, n6;
#if ID_ON_FLASH
    n1=0x00;
    n2=0x0b;
    n3=0x57;
    n4=0x55;
    n5=0xdb;
    n6=0x45;
    sprintf((char*)_mac_buffer,"%02X%02X%02X%02X%02X%02X", n1, n2, n3, n4, n5, n6);
#else
    char sn[32];
    memset (sn, 0, 32);
    if (!(_parser.send("AT+CGSN")
            && _parser.read(&sn[0], IMEI_LEN)
            && _parser.recv("OK"))) {
        debug_if(dbg_on, "BG96> IMEI error\r\n");
        return 0;
    }
    n1=0x00;
    n2=0x0b;
    n3=(uint8_t)(((sn[4+IMEI_OFFS]&0x0f)<<4 | (sn[5+IMEI_OFFS]&0x0f)) + ((sn[12+IMEI_OFFS]&0x0f)<<4));
    n4=(uint8_t)(((sn[6+IMEI_OFFS]&0x0f)<<4 | (sn[7+IMEI_OFFS]&0x0f)) + (uint8_t)((sn[0+IMEI_OFFS]&0x0f)<<4 | (sn[1+IMEI_OFFS]&0x0f)));
    n5=(uint8_t)(((sn[8+IMEI_OFFS]&0x0f)<<4 | (sn[9+IMEI_OFFS]&0x0f)) + ((sn[13+IMEI_OFFS]&0x0f)));
    n6=(uint8_t)(((sn[10+IMEI_OFFS]&0x0f)<<4 | (sn[11+IMEI_OFFS]&0x0f)) + (uint8_t)((sn[2+IMEI_OFFS]&0x0f)<<4 | (sn[3+IMEI_OFFS]&0x0f)));

    sprintf((char*)_mac_buffer,"%02X%02X%02X%02X%02X%02X", n1, n2, n3, n4, n5, n6);
#endif

    return _mac_buffer;
}

bool BG96::isConnected(void)
{
    return getIPAddress() != 0;
}
uint8_t sock_id = 0;


bool BG96::open(const char *type, int* id, const char* addr, int port)
{
    Timer timer;
    //Timer timer_first;
    timer.start();
    socket_closed = 0;
    uint32_t time;
    bool res;
    char send_type[16];
    const char *send_type_pt = send_type;
    int result;

    switch(*type) {
        case 'u':
            send_type_pt = "UDP";
            sock_id = 1;
            break;
        case 't':
            send_type_pt = "TCP";
            sock_id = 0;
            break;
        default:
            send_type_pt = "TCP";
            break;
    };
    //_parser.setTimeout(BG96_SOCKQ_TIMEOUT*3);

    while(1) {

        if((_parser.send("AT+QIOPEN=1,%d,\"%s\",\"%s\",%d,0,0\r", sock_id, send_type_pt, addr, port)) && (_parser.recv("OK")))

        {

            while(1) {
                res = _parser.recv("+QIOPEN: %d,%d", id, &result);
                if (res == true) {
                    printf("Open socket result = %d\r\n", result);
                    if (result == 0)
                        return true;
                    if (result == 563) 
                        close((int)*id);
                    return false;
                }

                time = timer.read_ms();
                if (time > BG96_CONNECT_TIMEOUT) {
                    return false;
                    printf("Timeout sock open!!\r\n");

                }


                //TODO:implement time-out functionality in case of no response
                //if(timeout) return false;
                //TODO: deal with errors like "ERROR: Failed to resolve name"
                //TODO: deal with errors like "ERROR: Data mode not available"
            }

        }
        return false;
    }

}

bool BG96::send(int id, const void *data, uint32_t amount)
{
    char _buf[32];
    _parser.setTimeout(BG96_SEND_TIMEOUT);
    //amount = 0;

    sprintf((char*)_buf,"AT+QISEND=%d,%d\r", id, amount);

    //May take a second try if device is busy
    for (unsigned i = 0; i < 5; i++) {
        if (_parser.write((char*)_buf, strlen(_buf)) >=0) {
            if (_parser.recv("> ")) {
                if ((_parser.write((char*)data, (int)amount) >= 0)
                        && (_parser.recv("SEND OK"))) {
                    return true;
                }
            }
        }
        //if send fails, wait before further attempt
        wait_ms(2000);
    }
    return false;

}

uint8_t local_data_store[16];
uint8_t local_amount = 0;
uint8_t local_pt = 0;

int32_t BG96::recv(int id, void *data, uint32_t amount)
{
    uint32_t recv_amount=0;

    if (local_amount > 0) {
        memcpy(data, &local_data_store[local_pt], amount);
        local_amount = local_amount-amount;
        local_pt = local_pt+amount;
        return amount;
    }

    local_amount = 0;
    local_pt = 0;


    if (socket_closed) {
        socket_closed = 0;
        return -3;
    }

    /*
    int par_timeout = _parser.getTimeout();
    _parser.setTimeout(0);
    _parser.setTimeout(par_timeout);
    */
    switch (id) {
        case 0:
            if (!(_parser.recv("+QIURC: \"recv\",0")))
                return -1;
            break;
        case 1:
            if (!(_parser.recv("+QIURC: \"recv\",1")))
                return -1;
            break;
        default:
            return -1;
            break;
    }

    if ((_parser.send("AT+QIRD=%d,1000\r", id))  //send a query (will be required for secure sockets)
            && (_parser.recv("+QIRD: %u", &recv_amount))) {

        _parser.read((char*)data, recv_amount);
        while( _parser.recv("OK"));

        if(recv_amount > amount) {
            local_amount = recv_amount-amount;
            local_pt = local_pt+amount;
            memcpy(local_data_store, data, recv_amount);
            recv_amount = amount;
        }

        return recv_amount;

    }
    return -2;

}

bool BG96::close(int id)
{
    uint32_t recv_amount=0;
    void * data = NULL;

    _parser.setTimeout(BG96_MISC_TIMEOUT);
    _parser.flush();
    /* socket flush */
    if(!(_parser.send("AT+QIRD=%d,1000\r", id) //send a query (will be required for secure sockets)
            && _parser.recv("+QIRD: %u", &recv_amount))
            //&& _parser.recv("OK"))
      ) {
        return -2;
    }

    if (recv_amount>0) {
        data = malloc (recv_amount+4);
//       printf ("--->>>Close flushing recv_amount: %d  \n\r",recv_amount);
        if(!((_parser.read((char*)data, recv_amount) >0)
                && _parser.recv("OK"))) {
            free (data);
            return -2;
        }
        free (data);
    } else {
        if (!(_parser.recv("OK")))
            return -2;
    }



    //May take a second try if device is busy or error is returned
    for (unsigned i = 0; i < 2; i++) {
        if ((_parser.send("AT+QICLOSE=%d", id))
                && _parser.recv("OK")) {
            socket_closed = 1;
            return true;
        } else {
            if(_parser.recv("ERROR")) {
                debug_if(dbg_on, "BG96> ERROR!!!!\r\n");
                return false;
            }
        }
        //TODO: Deal with "ERROR: Pending data" (Closing a socket with pending data)
    }
    return false;

}


bool BG96::readable()
{
    return _serial.readable();
}

bool BG96::writeable()
{
    return _serial.writeable();
}

