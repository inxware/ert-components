#include "target_network.h"
#include "target_logger.h"
#include "hal_mqtt.h"
#include "hal-api.h"

#include <WiFiNINA.h>

// This is also used for the mqtt stack, so it may need memory in the future
#ifndef EHS_TARGET_NETWORK_THREAD_STACK_SIZE
#define EHS_TARGET_NETWORK_THREAD_STACK_SIZE (1 << 13) /* 2^13 = 8192 */
#endif


typedef struct _EhsTgtNetworkCtrl {
    ehs_sint32  app_status;
    ehs_uint8   initalised;
    ehs_bool    stop;
} EhsTgtNetworkCtrl; 

volatile EhsTgtNetworkCtrl gNetworkCtrl = { 0 };

EhsThreadFuncReturnType EhsTgtNetworkLoop(void* args);

void EhsTgtNetworkSetAppStatus(ehs_uint32 status)
{
    // EHS_APP_LOAD_STARTED
    // EHS_APP_LOAD_SUCCESFULL
    // EHS_APP_LOAD_RESTARTING
    // EHS_APP_LOAD_FAILED
    gNetworkCtrl.app_status = status;
#ifdef EHS_MQTT_SUPPORT
    EhsTgtNetworkAppStatus_MQTT(status);
#endif
}

ehs_bool EhsTgtNetworkInit()
{
    ehs_bool ret = EHS_TRUE;
    if(gNetworkCtrl.initalised == 0){
        gNetworkCtrl.app_status = -1;
        gNetworkCtrl.stop = EHS_FALSE;
        EhsHThread_execute(EhsTgtNetworkLoop, (void*)NULL, EHS_PRI_MCU_SLOW_LP_THR, EHS_TARGET_NETWORK_THREAD_STACK_SIZE,"ardnetwork");
        gNetworkCtrl.initalised = 1;
    }else{
        ret = EHS_FALSE; // already initalised
    }
    return ret;
}

void EhsTgtMakeHostname(String& hostname)
{
    byte mac[6] = {0};
    EhsTPMutex_lock(EhsTPMutex_socketClient);
    WiFi.macAddress(mac);
    EhsTPMutex_unlock(EhsTPMutex_socketClient);

    hostname = "inx-arduino-";
    for(int i = 0; i < 6; i++){
        hostname += mac[i];
    }
}

ehs_bool EhsTgtNetworkStartWifi(const ehs_char* ssid, const ehs_char* pass)
{
    EhsTPMutex_lock(EhsTPMutex_socketClient);
    int status = WiFi.status();
    EhsTPMutex_unlock(EhsTPMutex_socketClient);
    if (status == WL_NO_MODULE) {
        EhsStdioPrintf("No WiFi module!\n");
        return false;
    }
    EhsStdioPrintf("WiFi Config Start\n");
    EhsTPMutex_lock(EhsTPMutex_socketClient);
    String firmware = WiFi.firmwareVersion();
    EhsTPMutex_unlock(EhsTPMutex_socketClient);
    if (firmware < WIFI_FIRMWARE_LATEST_VERSION) { // do we have correct header files ?
        // @TODO - do we want to report this ?
        //EhsStdioPrintf("Please upgrade the WiFi firmware\n");
    }
    if(ssid == NULL) {
#ifndef EHS_CONFIG_WIFI_SSID
        EhsStdioPrintf("No WiFi SSID!\n");
        return false;
#else
        ssid = EHS_CONFIG_WIFI_SSID;
#endif
    }
    if(pass == NULL) {
#ifndef EHS_CONFIG_WIFI_PASSWORD
        EhsStdioPrintf("No WiFi Password!\n");
        return false;
#else
        pass = EHS_CONFIG_WIFI_PASSWORD;
#endif
    }

    // Set the host 
    String hostname;
    EhsTgtMakeHostname(hostname);
    EhsTPMutex_lock(EhsTPMutex_socketClient);
    WiFi.setHostname(hostname.c_str());
    EhsTPMutex_unlock(EhsTPMutex_socketClient);
    //EhsStdioPrintf("Hostname: %s\n",hostname.c_str());

    status = WL_IDLE_STATUS;
    EhsStdioPrintf("Connecting to Network named: %s\n", ssid);
    while (status != WL_CONNECTED) {
        //EhsStdioPrintf(".");
        // Connect to WPA/WPA2 network:
        EhsTPMutex_lock(EhsTPMutex_socketClient);
        status = WiFi.begin(ssid, pass);
        EhsTPMutex_unlock(EhsTPMutex_socketClient);
        // wait 5 seconds for connection:
        if(status != WL_CONNECTED){
            EhsSleep(5000);
        }
    }
    EhsStdioPrintf("\n");
    char ipString[16];
    EhsTgtNetworkGetIPv4(ipString,16);
    EhsStdioPrintf("WiFi Connected IP=%s\n", ipString);
    return true;
}

ehs_bool EhsTgtNetworkConnected()
{
    ehs_bool connected = EHS_FALSE;
    EhsTPMutex_lock(EhsTPMutex_socketClient);
    if(WiFi.status() == WL_CONNECTED){
        connected = EHS_TRUE;
    }
    EhsTPMutex_unlock(EhsTPMutex_socketClient);
    return connected;
}

/* outputs wifi ssid to a buffer */
void EhsTgtNetworkGetSSID(ehs_char* ssid_buff, ehs_uint32 buff_size)
{

}

/* outputs wifi pass to a buffer */
void EhsTgtNetworkGetPass(ehs_char* pass_buff, ehs_uint32 buff_size)
{

}

/* outputs ip address */
void EhsTgtNetworkGetIPv4(ehs_char* ip_buff, ehs_uint32 buff_size)
{
    EhsTPMutex_lock(EhsTPMutex_socketClient);
    IPAddress ip = WiFi.localIP();
    EhsTPMutex_unlock(EhsTPMutex_socketClient);
    ip.toString().toCharArray(ip_buff, buff_size);
}

/* outputs mac address */
void EhsTgtNetworkGetMAC(ehs_char* mac_buff, ehs_uint32 buff_size)
{
    byte mac[6] = {0};
    EhsTPMutex_lock(EhsTPMutex_socketClient);
    WiFi.macAddress(mac);
    EhsTPMutex_unlock(EhsTPMutex_socketClient);
    snprintf(mac_buff, buff_size, "%02X:%02X:%02X:%02X:%02X:%02X", 
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

EhsThreadFuncReturnType EhsTgtNetworkLoop(void* args)
{
    while (gNetworkCtrl.stop == EHS_FALSE)
    {
        ehs_bool connected = EhsTgtNetworkConnected();
#ifdef EHS_MQTT_SUPPORT
        if(connected == EHS_TRUE){
            EhsMqttClientLoop( (void*)NULL );
        // EhsMqttClientLoop( (void*)EhsMqttDevmanMonSupport() );
        }
#endif
        EhsSleep(EHS_TIME_ms(10)); // sleep
    }
    
    EhsTPThread_exit();
    return 0;
}
