
#if !defined(MQTT_GSM_H)
#define MQTT_GSM_H

#include "MQTTmbed.h"
#include "CellularInterface.h"
#include "MQTTSocket.h"

class MQTT_GSM : public MQTTSocket
{
public:    
    MQTT_GSM(BG96Interface &GSMIntf, const char *apn, const char *username, const char *passw) : GSM(GSMIntf)
    {
//        eth.init();                          // Use DHCP
        GSM.connect(apn, username, passw);
    }
    
    BG96Interface& getGSM()
    {
        return GSM;
    }
    
/*    void reconnect()
    {
        WiFi.connect();  // nothing I've tried actually works to reconnect 
    }
*/    
private:

BG96Interface& GSM;
    
};


#endif

