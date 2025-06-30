/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file target_main.c
 * This file represents the main entry point for the target. Typically
 * it also incldes an event handler, should the target require one.
 *
 * @author: inx limited
 *
 */

/**
 * @page Verification Verification report
 * @section target_main
 * @anchor target_main
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on 2007-10-12
 * Last modified on $Date$
 *
 * This file contained no derogations to the MISRA standard.
 *
 * Note it is necessary to replace <sys/types.h> with <types.h> to lint this
 * file successfully.
 */

/**
 * Provides access to the target-specific declarations of header files
 */

#define EHS_TARGET_CODE

/*****************************************************************************/
/* Included files */
// #include <sys/types.h>
// #include <signal.h>
#include <unistd.h>

#include "target.h"
#ifdef EHS_DEBUG_TCPIP_CONSOLE
#include "console_server.h"
#endif
#include "ehs_main.h"
#include "targetos_init.h"
#include "globals.h"
#include "hal-api.h" // required for the meta data storage
#ifdef EHS_MQTT_SUPPORT
#include "hal_mqtt.h"
#endif
#include "hal_network.h"

#include "hal_configs.h"

#include "freertos/FreeRTOS.h"
#include "sdkconfig.h"
#ifdef EHS_RUNTIME_LOGGER_ENABLED
#include "esp_log.h"
#else
#define ESP_LOGI(...)
#define ESP_LOGE(...)
#define ESP_LOGW(...)
#define ESP_LOGD(...)
#endif
#include <stdio.h>

#include "target_config.h"

// #todo if need to include the following there are some #includes that fail ,
// which seems to depend on having the GNU macro defined #include
//"freertos/task.h"

#include "esp_event.h"
#if TARGET_USE_WIFI == 1
#include "esp_wifi.h"
#include "target_wifi.h"
#include "wifi_station.h"
#endif
#if TARGET_USE_ETHERNET == 1
#include "esp_wifi.h"
#include "esp_eth.h"
#include "target_ethernet.h"
#endif
#include "freertos/event_groups.h"
#include "freertos/task.h"
//#include "mdns.h" // - not used atm
#include "esp_mac.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <string.h>

#include "target_uart.h"
#include "target_ota.h"

#include "target_data_bin.h"

#include "target_specific.h"

#ifdef EHS_RTC_SUPPORT
#include "target_hal_rtc.h"
#endif

#include "esp_heap_caps.h"

#define ESP32S3_TIMG0_BASE 0x6001F000
#define ESP32S3_TIMG1_BASE 0x60020000
#define ESP32S3_TIMG_WDT_CONFIG0_OFFSET 0x0048
#define ESP32S3_TIMG_WDT_WRITEPROTECT_OFFSET 0x0064
#define ESP32S3_TIMG_WDT_WRITEPROTECT_MAGIC_VALUE 0x50d83aa1

#define EHS_REG32(x) (*((volatile ehs_uint32 *) x))

volatile static ehs_uint32 gTimg1Config0 = 0;

#ifndef EHS_MAIN_ESP32_TASK_STACK_SIZE
#define EHS_MAIN_ESP32_TASK_STACK_SIZE 10000
#endif


#ifndef EHS_MCU_SLOW_LP_THR_SLEEP_MS
#define EHS_MCU_SLOW_LP_THR_SLEEP_MS 10 /*ms*/
#endif


// set this to '1' for stdout memory usage every second
#define EHS_ESP32_ENABLE_MEMORY_MONITOR 0

#define CONFIG_ESP_MAXIMUM_RETRY 5

#if TARGET_USE_WIFI == 1

// define wifi ssid
#ifdef EHS_CONFIG_WIFI_SSID
#define CONFIG_ESP_WIFI_SSID EHS_CONFIG_WIFI_SSID
#else
#define CONFIG_ESP_WIFI_SSID ""
#endif
// define wifi password
#ifdef EHS_CONFIG_WIFI_PASSWORD
#define CONFIG_ESP_WIFI_PASSWORD EHS_CONFIG_WIFI_PASSWORD
#else
#define CONFIG_ESP_WIFI_PASSWORD ""
#endif

#define EHS_WIFI_CONFIG_FILE "/ehs/userdata/wifi_config"
#define EHS_WIFI_SSID_BUFF_MAX 64

ehs_bool EhsWifiStationLoadSettings(ehs_char* ssid, ehs_char* pass);
ehs_bool EhsWifiStationSaveSettings(const ehs_char* ssid, const ehs_char* pass);

#endif // TARGET_USE_WIFI

extern const char* base_path; // ert install path, defined in target_file

#if TARGET_USE_ETHERNET == 0
#if TARGET_USE_WIFI == 0
#error TARGET_USE_ETHERNET and TARGET_USE_WIFI should be defined!
#endif
#endif
#if TARGET_USE_ETHERNET == 1 && TARGET_USE_WIFI == 1
#error You cannot use Ethernet and Wi-Fi at the same time!
#endif
#if TARGET_USE_ETHERNET == 0 && TARGET_USE_WIFI == 0
#error You must set one of Ethernet or Wi-Fi to be used!
#endif

#ifndef TARGET_HOSTNAME
#define TARGET_HOSTNAME "INX-ESP32S3"
#endif
#if TARGET_USE_WIFI == 1
ehs_char* gWiFiHostNameBuffer[32] = {'\0'};
#endif
#if TARGET_USE_ETHERNET == 1
ehs_char* gEthHostNameBuffer[32] = {'\0'};
#endif

/** Crash Recovery Global Variables & Definitions **/
#define NVS_CRASH_NAMESPACE "crash"
#define NVS_CRASH_FLAG_NAME "cf"
#define NVS_CRASH_COUNT_NAME "cc"
static uint8_t gCrash_count = 0;
// The lowest level, set application to "prev"
#define CRASH_COUNT_LEVEL0 2
// The level to swtich the OTA partition
#define CRASH_COUNT_LEVEL1 4
// Switch back to the factory firmware partition
#define CRASH_COUNT_LEVEL2 6
// Switch back to the factory firmware with OTA update application
#define CRASH_COUNT_LEVEL3 8

/** OTA-Data Pairing **/
#define NVS_OTA_PART_NAMESPACE "otapart"
#define NVS_OTA_PART_SEC_NAME "sn"

#define EHS_NET_CONFIG_FILE "/ehs/userdata/config/net_config"

#ifdef EHS_MAX31343_SUPPORT 
ehs_sint32 gEhsAmbientTemp = 0;
#endif

/* Callback used for notifing about state of the file system */
extern void (*ehs_filesystem_initalised_callback)(void);

/* Flag gets set to true when the file system has been initalised */
volatile ehs_bool gFileSystemInitalised = EHS_FALSE;

/* Flag gets set to true once the network interface stack has been initalised and started */
volatile ehs_bool gNetworkStarted = EHS_FALSE;

/* Set to true when device has network availble. Set to false when the network is disconnected. */
/* @TODO - At the momemnt we only use this in the ethernet mode, we should do the same in the wifi mode. */
volatile ehs_bool gNetworkConnected = EHS_FALSE;

/* This flag is set to true once the app loading process completes regardless of failing or being sucessful. */
/* @TODO - create a proper state machine, to take appropriate action depending on the state of the app 
   e.g. free mqtt stack (if previously allocated) when the app fails to load, so there's enough resources for re-parsing */
volatile ehs_bool gAppLoadingDone = EHS_FALSE;

/* eRT appliaction loading status callback invoked inside the eRT kernel */
static void app_load_status_handler(ehs_uint32 status)
{
    // Re-Enable Watchdog timer
    EHS_REG32(ESP32S3_TIMG1_BASE + ESP32S3_TIMG_WDT_WRITEPROTECT_OFFSET) = ESP32S3_TIMG_WDT_WRITEPROTECT_MAGIC_VALUE;
    EHS_REG32(ESP32S3_TIMG1_BASE + ESP32S3_TIMG_WDT_CONFIG0_OFFSET) = gTimg1Config0;
    EHS_REG32(ESP32S3_TIMG1_BASE + ESP32S3_TIMG_WDT_WRITEPROTECT_OFFSET) = 0;
    nvs_handle_t nvs_handle;
    switch(status)
    {
        case EHS_APP_LOAD_STARTED : {
            ESP_LOGI(TAG, "******* App loading started *******");
            break;
        }
        case EHS_APP_LOAD_SUCCESFULL : {
            ESP_LOGI(TAG, "******* App loaded sucessfully *******");
            ESP_ERROR_CHECK_WITHOUT_ABORT(nvs_open(NVS_CRASH_NAMESPACE, NVS_READWRITE, &nvs_handle));
            ESP_ERROR_CHECK_WITHOUT_ABORT(nvs_set_u8(nvs_handle, NVS_CRASH_FLAG_NAME, 0));            
            ESP_ERROR_CHECK_WITHOUT_ABORT(nvs_commit(nvs_handle));
            nvs_close(nvs_handle);
            gAppLoadingDone = EHS_TRUE;
            break;
        }
        case EHS_APP_LOAD_RESTARTING : {
            ESP_LOGI(TAG, "******* App restarting *******");
            EhsTargetReboot();
            break;
        }
        case EHS_APP_LOAD_FAILED : {
            ESP_LOGE(TAG, "******* App loading failed *******");
            gAppLoadingDone = EHS_TRUE;
            break;
        }
        default:
            ESP_LOGW(TAG, "Unknow app loading status!");
            break;
    }
}

#ifdef EHS_ESP32_CMD_PROMPT_SUPPORT

#define EHS_PROMPT_READ_MAX 64 // if changed make sure to update "%Ns" in command_prompt_read "%(max_value-1)s"
#define EHS_PROMPT_READ_SLEEP 1000

#ifdef EHS_WIFI_SSID_BUFF_MAX
#if EHS_PROMPT_READ_MAX != EHS_WIFI_SSID_BUFF_MAX
#error "EHS_PROMPT_READ_MAX and EHS_WIFI_SSID_BUFF_MAX must be the same"
#endif
#endif

void command_prompt_println(const char* text)
{
    vTaskDelay(pdMS_TO_TICKS(EHS_PROMPT_READ_SLEEP));
    printf("%s\n",text);
}

ehs_uint32 command_prompt_read(char* buffer)
{
    while (1) {
        if (fscanf(stdin, "%63s", buffer) > 0) { // EHS_PROMPT_READ_MAX 64
            // Check for and remove the trailing newline, if it exists
            ehs_uint32 len = EhsStrlen(buffer);  // Find the length of the string
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
            }
            return len;
        }
        vTaskDelay(pdMS_TO_TICKS(EHS_PROMPT_READ_SLEEP)); // Please use Ehs functions for tthis stufff
    }
}

ehs_bool command_prompt_ask_yes_no(const char* question)
{
    command_prompt_println(question);
    char anws[EHS_PROMPT_READ_MAX] = {0};
    if(command_prompt_read(anws) > 0){
        return (EhsStrcmp(anws,"y")==0) || (EhsStrcmp(anws,"Y")==0) || (EhsStrcmp(anws,"yes")==0) || (EhsStrcmp(anws,"YES")==0) || (EhsStrcmp(anws,"Yes")==0);
    }
    return EHS_FALSE;
}

void command_prompt_wifi_conf()
{
    char ssid[EHS_PROMPT_READ_MAX] = {0};
    char pass[EHS_PROMPT_READ_MAX] = {0};
    ehs_bool yes = EHS_FALSE;
    command_prompt_println("**** WiFi config ****");
    command_prompt_println("Enter SSID:");
    command_prompt_read(ssid);
    command_prompt_println(ssid);
    command_prompt_println("Enter Password:");
    command_prompt_read(pass);
    command_prompt_println(pass);
    yes = command_prompt_ask_yes_no("Are these correct? (y/n)");
    if(yes==EHS_TRUE){
        command_prompt_println("Saving above WiFi credentials.");
        EhsWifiStationSaveSettings(ssid, pass);
        command_prompt_println("Connecting to WiFi, please wait...");
        setWifiStationConnectState(WifiStationConnectState_INIT);
    }else{
        command_prompt_println("WiFi NOT configured. Type 'w' to try again.");
    }
}

void command_prompt_wifi_reconnect()
{
    command_prompt_println("wifi re-connect");
    setWifiStationConnectState(WifiStationConnectState_INIT);
}

void command_prompt_wifi_ssid()
{
    char ssid[EHS_PROMPT_READ_MAX] = {0};
    char pass[EHS_PROMPT_READ_MAX] = {0};
    if(EhsWifiStationLoadSettings(ssid, pass) == EHS_TRUE){
        ehs_bool connected = isWifiStationConnected();
        printf("SSID: %s  (%s)\n", ssid, (connected) ? "connected" : "not connected");
    }
}

void command_prompt_ip_addr()
{
    const ehs_char* ip = isWifiStationConnected() ? WifiStationIpAddress() : NULL;
    printf("IP: %s\n", (ip && EhsStrlen(ip) > 0) ? ip : "N/A");
}

void command_prompt_list_ssid_bssid()
{
    ehs_char ssid[33] = {0};
    ehs_char bssid[6] = {0};
    ehs_sint32 channel = 0, rssi = 0;
    ehs_uint32 index = 0;
    vTaskDelay(pdMS_TO_TICKS(EHS_PROMPT_READ_SLEEP));
    while(WifiStationScanResult(index, ssid, 33, bssid, 6, &channel, &rssi) == EHS_TRUE){
        printf("SSID=%s, BSSID(MAC)=%02x:%02x:%02x:%02x:%02x:%02x, Channel=%d, RSSI=%d dBm\n",
                ssid, bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5], channel, rssi);
        index++;
        vTaskDelay(pdMS_TO_TICKS(EHS_PROMPT_READ_SLEEP));
    }
    if(index == 0){
        printf("No SSID found!\n");
    }
}

void command_prompt_reboot()
{
    command_prompt_println("reboot");
    EhsTargetReboot();
}

void command_prompt_help()
{
    command_prompt_println("help:\n'w' - WiFi config \n'c' - reconnect WiFi \n's' - get WiFi SSID \n'i' - get IP Address \n'h' - help");
}

void command_prompt_task(void* params) {
    
    command_prompt_println("Type 'w' to configure WiFi or 'h' for help.");
    char command;
    while (1) {
        if (fscanf(stdin, "%c", &command) == 1) {  // Read a single character for command
            if (command == 'w') {
                command_prompt_wifi_conf();
            }
            else if(command == 'c'){
                command_prompt_wifi_reconnect();
            }
            else if(command == 's'){
                command_prompt_wifi_ssid();
            }
            else if(command == 'i'){
                command_prompt_ip_addr();
            }
            else if(command == 'l'){
                command_prompt_list_ssid_bssid();
            }
            //else if(command == 'r'){
            //    command_prompt_reboot();
            //}
            else if(command == 'h'){
                command_prompt_help();
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // Small delay to avoid CPU overload
    }
    command_prompt_println("quit command prompt");
    vTaskDelete(NULL);
}

#endif // EHS_ESP32_CMD_PROMPT_SUPPORT

#if TARGET_USE_ETHERNET == 1
static void eth_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    uint8_t mac_addr[6] = {0};
    esp_eth_handle_t eth_handle = *(esp_eth_handle_t *) event_data;

    switch (event_id) {
    case ETHERNET_EVENT_CONNECTED:
        esp_eth_ioctl(eth_handle, ETH_CMD_G_MAC_ADDR, mac_addr);
        ESP_LOGI(TAG, "Ethernet Link Up");
        ESP_LOGI(TAG, "Ethernet HW Addr %02x:%02x:%02x:%02x:%02x:%02x",
                    mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
        gNetworkConnected = EHS_TRUE;
        break;
    case ETHERNET_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "Ethernet Link Down");
        gNetworkConnected = EHS_FALSE;
        break;
    case ETHERNET_EVENT_START:
        ESP_LOGI(TAG, "Ethernet Started");
        gNetworkStarted = EHS_TRUE;
        break;
    case ETHERNET_EVENT_STOP:
        ESP_LOGI(TAG, "Ethernet Stopped");
        gNetworkStarted = EHS_FALSE;
        break;
    default:
        break;
    }
}

static void got_ip_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
    const esp_netif_ip_info_t *ip_info = &event->ip_info;

    ESP_LOGI(TAG, "Ethernet Got IP Address");
    ESP_LOGI(TAG, "#######################");
    ESP_LOGI(TAG, "ETHIP:" IPSTR, IP2STR(&ip_info->ip));
    ESP_LOGI(TAG, "ETHMASK:" IPSTR, IP2STR(&ip_info->netmask));
    ESP_LOGI(TAG, "ETHGW:" IPSTR, IP2STR(&ip_info->gw));
    ESP_LOGI(TAG, "#######################");
    // make sure ip address/mac is updated after establishing connection 
    EhsHMetaUpdateDynamic();
}

static esp_err_t eth_init()
{
    esp_err_t ret = ESP_OK;
    esp_eth_handle_t *eth_handle;
    ESP_ERROR_CHECK_WITHOUT_ABORT(target_eth_init(&eth_handle));

    // Initialise TCP/IP network interface
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_netif_init());

    // Create defaultevent loop running in the background
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_loop_create_default());

    // Create instance fo eps-netif for Ethernet
    esp_netif_config_t cfg = ESP_NETIF_DEFAULT_ETH();
    esp_netif_t *eth_netif = esp_netif_new(&cfg);

    // Append unique suffix from MAC
    uint8_t mac[6];
    esp_mac_type_t mac_type = ESP_MAC_EFUSE_FACTORY; // Use the efuse which was burnt by Espressif in production
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_read_mac(mac, mac_type));
    snprintf(gEthHostNameBuffer, sizeof(gEthHostNameBuffer), TARGET_HOSTNAME"-e%02X%02X%02X%02X", mac[2], mac[3], mac[4], mac[5]);
    printf("Eth host name : %s\n", gEthHostNameBuffer);
    // Set the hostname
    esp_netif_set_hostname(eth_netif, gEthHostNameBuffer);

    // Attach Ethernet driver to TCP/IP stack
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_netif_attach(eth_netif, esp_eth_new_netif_glue(eth_handle)));

    // Register user defined event handler
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_handler_register(ETH_EVENT, ESP_EVENT_ANY_ID, &eth_event_handler, NULL));
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_handler_register(IP_EVENT, IP_EVENT_ETH_GOT_IP, &got_ip_event_handler, NULL));

    // Start Ethernet driver state machine
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_eth_start(eth_handle));

    return ret;
}
#endif // TARGET_USE_ETHERNET == 1

#if TARGET_USE_WIFI == 1

ehs_bool EhsWifiStationLoadSettings(ehs_char* ssid, ehs_char* pass)
{
    if(!ssid || !pass){
        return EHS_FALSE;
    }
    EhsConfig* config = EhsConfigLoad(EHS_WIFI_CONFIG_FILE);
    if(!config){
        EhsStrcpy(ssid, CONFIG_ESP_WIFI_SSID);
        EhsStrcpy(pass, CONFIG_ESP_WIFI_PASSWORD);
        return EHS_TRUE;
    }
    ehs_bool success = EHS_TRUE;
    const ehs_char* config_ssid = EhsConfigGetValue(config, "ssid");
    const ehs_char* config_pass = EhsConfigGetValue(config, "pass");

    if(success==EHS_TRUE){
        if(config_ssid && EhsStrlen(config_ssid) > 0){
            EhsStrcpy(ssid, config_ssid);
        }else{
            success = EHS_FALSE;
        }
    }
    if(success==EHS_TRUE){
        if(config_pass && EhsStrlen(config_pass) > 0){
            EhsStrcpy(pass, config_pass);
        }else{
            success = EHS_FALSE;
        }
    }

    EhsConfigFree(config);
    
    return success;
}

ehs_bool EhsWifiStationSaveSettings(const ehs_char* ssid, const ehs_char* pass)
{
    ehs_bool success = EHS_TRUE;

    EhsConfig* config = EhsConfigLoad(EHS_WIFI_CONFIG_FILE);
    config = (config == NULL) ? EhsConfigCreate() : config;

    EhsConfigSetValue(config, "ssid", (ssid) ? ssid : "" );
    EhsConfigSetValue(config, "pass", (pass) ? pass : "" );

    EhsConfigSave(config, EHS_WIFI_CONFIG_FILE);
    EhsConfigFree(config);
    
    return success;
}

eWifiStationStatus EhsWifiStationConnect(const ehs_char* ssid, const ehs_char* pass, ehs_char *ip_address, ehs_char *mac_address, ehs_sint32* rssi)
{
    if(!ssid || !pass || EhsStrlen(ssid) == 0 || EhsStrlen(pass) == 0) {
        return WifiStation_StateError;
    }
    eWifiStationStatus status = doWifiStationConnect(
                        ssid, 
                        Type_WifiStation_PSK, 
                        pass, 
                        Enterprise_WifiStation_WPA2, 
                        EAP_WifiStation_TLS, 
                        TTLS2_WifiStation_MIN,
                        EHS_FALSE, 
                        NULL, 
                        NULL,
                        NULL, 
                        NULL, 
                        NULL, 
                        NULL, 
                        ip_address,
                        mac_address, 
                        rssi
                        );

    return status;
}

#endif // TARGET_USE_WIFI == 1

void EhsLoadNetworkInterfaceConfig()
{
    EhsConfig* config = EhsConfigLoad(EHS_NET_CONFIG_FILE);
    if(config){
        ESP_LOGI(TAG, "Loading network settings from config (" EHS_NET_CONFIG_FILE ")");
        EhsNetworkConfigDataType net_config = { 0 };
        net_config.save = EHS_FALSE; // we're loading settings, so no need to save them
        net_config.mode = EHS_NET_DHCP_MODE_ID;
        const ehs_char* net_static = EhsConfigGetValue(config, "net_static");
        if(net_static && EhsStrcmp("1", net_static)==0){
            net_config.mode = EHS_NET_STATIC_MODE_ID;
            net_config.address = EhsConfigGetValue(config, "net_address");
            net_config.gateway = EhsConfigGetValue(config, "net_gateway");
            net_config.mask = EhsConfigGetValue(config, "net_mask");
            net_config.dns = EhsConfigGetValue(config, "net_dns");
        }
        EhsNetworkConfigureInterface(&net_config);
        EhsConfigFree(config);
    }
}

void EhsSaveNetworkInterfaceConfig(const EhsNetworkConfigDataType* net_config)
{
    if(net_config == NULL){
        return;
    }
    EhsConfig* config = EhsConfigLoad(EHS_NET_CONFIG_FILE);
    if(config == NULL){
        config = EhsConfigCreate();
    }
    if(net_config->mode == EHS_NET_STATIC_MODE_ID){
        EhsConfigSetValue(config, "net_static", "1");
        if(net_config->address){
            EhsConfigSetValue(config, "net_address", net_config->address);
        }
        if(net_config->gateway){
            EhsConfigSetValue(config, "net_gateway", net_config->gateway);
        }
        if(net_config->mask){
            EhsConfigSetValue(config, "net_mask", net_config->mask);
        }
        if(net_config->dns){
            EhsConfigSetValue(config, "net_dns", net_config->dns);
        }
    }else{ // DHCP
        EhsConfigSetValue(config, "net_static", "0");
    }
    EhsConfigSave(config, EHS_NET_CONFIG_FILE);
    EhsConfigFree(config);
}

// Override functions for esp32 network config
#if EHS_NETWORK_CONFIG_SUPPORT==EHS_NETWORK_CONFIG_TYPE_ESP32
/* Returns true when the eRT target network is connected */
ehs_bool EhsNetworkIsConnected()
{
    return gNetworkConnected;
}

/* Configures the traget network interface */
ehs_sint32 EhsNetworkConfigureInterface(const EhsNetworkConfigDataType* config)
{
    esp_netif_t* netif;
    #if TARGET_USE_WIFI
        netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    #elif TARGET_USE_ETHERNET
        netif = esp_netif_get_handle_from_ifkey("ETH_DEF");
    #else
        #error "No network interface defined for this target! "
    #endif
    ehs_sint32 nError = EHS_NETWORK_CONFIG_NO_ERROR_ID;
    if(config->mode == EHS_NET_STATIC_MODE_ID){
        esp_err_t err = esp_netif_dhcpc_stop(netif);
        if(err == ESP_OK || err == ESP_ERR_ESP_NETIF_DHCP_ALREADY_STOPPED){
            ESP_LOGI(TAG, "Network Config - DHCP stopped.");
            esp_netif_ip_info_t ip;
            memset(&ip, 0 , sizeof(esp_netif_ip_info_t));
            ip.ip.addr = (config->address) ? ipaddr_addr(config->address) : 0;
            ip.netmask.addr = (config->gateway) ? ipaddr_addr(config->gateway) : 0;
            ip.gw.addr = (config->mask) ? ipaddr_addr(config->mask) : 0;
            err = esp_netif_set_ip_info(netif, &ip);
            if(err == ESP_OK){
                // set dns1 address
                if(config->dns != NULL){
                    uint32_t dns_addr = ipaddr_addr(config->dns);
                    if (dns_addr && (dns_addr != IPADDR_NONE)) {
                        esp_netif_dns_info_t dns;
                        dns.ip.u_addr.ip4.addr = dns_addr;
                        dns.ip.type = IPADDR_TYPE_V4;
                        err = esp_netif_set_dns_info(netif, ESP_NETIF_DNS_MAIN, &dns);
                        if(err != ESP_OK){
                            ESP_LOGE(TAG, "Network Config - Failed to configure static dns err=(%d).", err);
                            nError = EHS_NETWORK_CONFIG_FAILED_DNS1_ID;
                        }
                    }else{
                        ESP_LOGE(TAG, "Network Config - Failed to configure static dns. Invalid address (%s).", config->dns);
                        nError = EHS_NETWORK_CONFIG_FAILED_DNS1_ID;
                    }
                }
                ESP_LOGI(TAG, "Network Config - Static interface configured!");
            }else{
                ESP_LOGE(TAG, "Network Config - Failed to configure static interface err=(%d).", err);
                nError = EHS_NETWORK_CONFIG_FAILED_STATIC_ID;
            }
        }else{
            ESP_LOGE(TAG, "Network Config - Failed to stop DHCP err=(%d).", err);
            nError = EHS_NETWORK_CONFIG_FAILED_STATIC_ID;
        }
    }else{ // DHCP mode
        esp_err_t err = esp_netif_dhcpc_start(netif);
        if(err == ESP_OK || err == ESP_ERR_ESP_NETIF_DHCP_ALREADY_STARTED){
            ESP_LOGI(TAG, "Network Config - DHCP started!");
        }else{
            ESP_LOGE(TAG, "Network Config - Failed to start DHCP err=(%d).", err);
            nError = EHS_NETWORK_CONFIG_FAILED_DHCP_ID;
        }
    }
    EhsHMetaUpdateNetwork();
    if(config->save==EHS_TRUE){
        EhsSaveNetworkInterfaceConfig(config);
    }
    return nError;
}

#endif

void EhsFilesystemInitalised()
{
    gFileSystemInitalised = EHS_TRUE;
}

void MCU_SLOW_HP_THR(void *pvp)
{
    //
    for (;;)
    {
#ifdef EHS_MODBUS_SUPPORT
        EhsHMBTaskInLoop( EHS_TRUE );
#endif
        vTaskDelay( pdMS_TO_TICKS( 10 ) ); //todo2025 1 tick delay seems absurdly fast? What is actually needed? This needs to be changed to a specific amount of time not ticks. 
                       // Consider using the EhsSleep function which should map to an appropriate delay method (i.e. vDelay with a real)
    }
}

void MCU_SLOW_LP_THR(void *pvParameters)
{
    // Flag used for indicating whether the inital network settings have been loaded
    ehs_bool bNetworkSettingsLoaded = EHS_FALSE;
    ehs_bool bOtaInitalised = EHS_FALSE;
    int i;

    //todo2025 - change this MACRO to EHS_RTC___MAX31343  (I think we agreed to use "___" for equals in macro land)
#ifdef EHS_MAX31343_SUPPORT 
    int j = 0;
    char str_datetime[21] = { 0 };
#endif//EHS_MAX31343_SUPPORT 
// todo2025 Do we use _SUPPORT to say enabled usually?? for EHS_I2C_SUPPORT or EHS_I2C___ENABLED???
#if defined(EHS_MAX31343_SUPPORT) && defined(EHS_I2C_SUPPORT)
    EhsTI2CMasterInit();
#endif//EHS_MAX31343_SUPPORT && EHS_I2C_SUPPORT
    int k = 0;
#if TARGET_USE_WIFI == 1
    // wifi setup init
    ehs_char wifi_ssid[EHS_WIFI_SSID_BUFF_MAX] = { 0 };
    ehs_char wifi_pass[EHS_WIFI_SSID_BUFF_MAX] = { 0 };
    setWifiStationConnectState(WifiStationConnectState_INIT);
#endif
    for (;;)
    {
        // this statement runs or initalises services, which should wait until
        // parsing process is compleated
        if(gAppLoadingDone==EHS_TRUE){
            
#if TARGET_USE_WIFI == 1
            eWifiStationConnectState state = getWifiStationConnectState();

            switch (state)
            {
                case WifiStationConnectState_IDLE:
                    // do nothing
                    break;
                case WifiStationConnectState_INIT:
                    if(isWifiStationInitalised() == EHS_TRUE){
                        ESP_LOGI(TAG, "Destroy wifi as it's initalised before re-connecting.");
                        doWifiStationDestroy();
                        setWifiStationConnectState(WifiStationConnectState_CONFIGURE); // @TODO - this may need seperate state waiting for destroy to compleate
                    }else{
                        setWifiStationConnectState(WifiStationConnectState_CONFIGURE);
                    }
                    break;
                case WifiStationConnectState_CONFIGURE:
                    EhsMemset(wifi_ssid, '\0', EHS_WIFI_SSID_BUFF_MAX);
                    EhsMemset(wifi_pass, '\0', EHS_WIFI_SSID_BUFF_MAX);
                    if(EhsWifiStationLoadSettings(wifi_ssid, wifi_pass) == EHS_TRUE){
                        ESP_LOGI(TAG, "Wifi settings loaded for ssid=%s", wifi_ssid);
                        // configure remining parameters
                        configWifiStationSetReconnect(EHS_TRUE, 5); // TODO - read this from config file
                        // ...
                        setWifiStationConnectState(WifiStationConnectState_CONNECT);
                    }else{
                        ESP_LOGW(TAG, "Wifi settings not available.");
                        ESP_LOGI(TAG, "Press 'w' to configure WiFi via console.");
                        setWifiStationConnectState(WifiStationConnectState_IDLE);
                    }
                    
                    break;
                case WifiStationConnectState_CONNECT:
                case WifiStationConnectState_CONNECTING:
                case WifiStationConnectState_CONNECTING_GOT_IP:
                    eWifiStationStatus conn_status = EhsWifiStationConnect(wifi_ssid, wifi_pass, NULL, NULL, NULL);
                    if      (conn_status == WifiStation_Connecting){
                        // wait for wifi to connect ...
                    }else if(conn_status == WifiStation_Connected){
                        setWifiStationConnectState(WifiStationConnectState_CONNECTED);
                    }else{
                        setWifiStationConnectState(WifiStationConnectState_FAILED);
                    }
                    break;
                case WifiStationConnectState_CONNECTED:
                    const ehs_char* ip = isWifiStationConnected() ? WifiStationIpAddress() : "N/A";
                    printf("Wifi connected ip=%s\n", (ip) ? ip : "N/A");
                    EhsHMetaUpdateDynamic(); // update network metadata with a new IP.
                    setWifiStationConnectState(WifiStationConnectState_IDLE);
                    break;
                case WifiStationConnectState_FAILED:
                    printf("Failed to connected to the wifi.\n");
                    setWifiStationConnectState(WifiStationConnectState_IDLE);
                    break;
                default:
                    ESP_LOGE(TAG, "Unknown wifi connect state (%d).", (int)state);
                    setWifiStationConnectState(WifiStationConnectState_IDLE);
                    break;
            }
#else
            // load network settings from the file, once both file system and network 
            // stack have been initalised
            if(bNetworkSettingsLoaded==EHS_FALSE){
                EhsLoadNetworkInterfaceConfig();
                bNetworkSettingsLoaded = EHS_TRUE;
            }
#endif

#ifdef EHS_MQTT_SUPPORT
            // execute esp mqtt clinet loop
#if TARGET_USE_WIFI == 1           
            if(isWifiStationConnected()){
                EhsMqttClientLoop( (void*)EhsMqttDevmanMonSupport() );
            }
#else
            EhsMqttClientLoop( (void*)EhsMqttDevmanMonSupport() );
#endif // TARGET_USE_WIFI  

#endif // EHS_MQTT_SUPPORT

#if EHS_OTA_SUPPORT == EHS_OTA_SUPPORT_SUPPORT

#if TARGET_USE_WIFI == 1
            if(isWifiStationConnected()){
                // execute OTA loop
                target_OTA_task(NULL);
            }
#else
            // execute OTA loop
            target_OTA_task(NULL);
#endif //TARGET_USE_WIFI
        
#endif //EHS_OTA_SUPPORT

            // @TODO - This is used by Uart function block - needs to review and potentially moved or Wifi connect needs to be done non-blockig (prefered)
            fflush(stdout);
            for (i = 0; i < UART_COUNT; i++){
                TgtUART_SendInThread(i);
            }

// #ifdef EHS_MODBUS_SUPPORT
//             EhsHMBTaskInLoop( EHS_TRUE );
// #endif
#ifdef EHS_MAX31343_SUPPORT 
            gEhsAmbientTemp = EhsTMax31343GetTemperatureFP();
            //ESP_LOGD(TAG, "Ambient Temp: %d", gEhsAmbientTemp / 1024);
#ifdef EHS_RTC_SUPPORT
            // Set the time to RTC every 360000 loops (3600 seconds)
            if (++j % 360000 == 0)
            {
                time_t _t1 = EhsTGetRtcSecond();
                struct timeval tv = {_t1, 0};
                settimeofday(&tv, NULL);
                // struct tm *tm = localtime(&_t1);
                // ESP_LOGD(TAG, "Time set from RTC %04d-%02d-%02dT%02d:%02d:%02d %d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, tm->tm_wday ? tm->tm_wday : 7);
            }
#endif//EHS_RTC_SUPPORT
#endif//EHS_MAX31343_SUPPORT         
        } // if(gAppLoadingDone==EHS_TRUE)

        // todo: change the sleep interval for EHS MQTT responsive rate
        // todo2024 this needs to be set to a targete-specific (overridable) loop time for a network thread as per the above.
        vTaskDelay( pdMS_TO_TICKS( EHS_MCU_SLOW_LP_THR_SLEEP_MS ) );

/* @TODO - may need to do wifi connect function non-blocking first
#if TARGET_USE_WIFI == 1
        // Wifi reconnect request counter
        static int wifi_reconnect_counter = 0;
        const static int WIFI_RECONECT_TIME_OUT = 30000/EHS_MCU_SLOW_LP_THR_SLEEP_MS; // ~30 sec
        if(wifi_reconnect_counter > WIFI_RECONECT_TIME_OUT){  
            wifi_reconnect_counter++;
        }else{
            ESP_LOGW(TAG, "WiFi re-connect reqired");
            wifi_reconnect_counter = 0;
        }
#endif
*/
#if EHS_ESP32_ENABLE_MEMORY_MONITOR == 1
        static int mem_check_counter = 0;
        if(mem_check_counter > 1000/EHS_MCU_SLOW_LP_THR_SLEEP_MS){  // 1 sec
            multi_heap_info_t info;
            heap_caps_get_info(&info, MALLOC_CAP_8BIT);
            printf("memory used=%d, free=%d \n",info.total_allocated_bytes,info.total_free_bytes);
            mem_check_counter=0;
    
        }
        mem_check_counter++;
#endif
    }
}

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */
/*****************************************************************************/
/* Declare prototypes of local functions */
/*****************************************************************************/
/* Variables defined with file-scope */

/**
 * Handle the SIGTERM signal
 */
EHS_LOCAL void EhsTargetHandleTerm(int);

/*****************************************************************************/
/* Variables defined with global-scope */
/*****************************************************************************/
/* Function definitions */

/* linux (and gnu) is always ready as soon as main is run */
ehs_bool EhsTPlatformReady(void (*target_loop_iteration)(void *),
                           void *target_env_blob)
{
    return EHS_TRUE;
}


/**
 * Main entry point to the application.
 * @return Integer representing exit code of application.
 *
 */
// EhsTargetIntType main(int argc, ehs_char ** argv )


/**
 * There’s already an api for setting the next app to be launched. Simply call this at any point before EhsMain
 * EhsHMetaSetNextAppToRun("default");  or EhsHMetaSetNextAppToRun("fallbacks"); etc.
 */
void app_main(void)
{
#if EHS_ESP32_DISABLE_LOGS == 1
    esp_log_level_set("*", ESP_LOG_NONE);
#endif
    // Set callback function for notifying about file systems status
    ehs_filesystem_initalised_callback = EhsFilesystemInitalised;

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);

    /* Check the crash flag and count from NVS */
    uint8_t crash_flag = 0;
    uint8_t crash_count = 0;
    nvs_handle_t nvs_handle;
    ret = nvs_open(NVS_CRASH_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (ret == ESP_OK)
    {
        ret = nvs_get_u8(nvs_handle, NVS_CRASH_FLAG_NAME, &crash_flag);
        if (ret != ESP_OK) crash_flag = 0;
        ret = nvs_get_u8(nvs_handle, NVS_CRASH_COUNT_NAME, &crash_count);
        if (ret != ESP_OK)
        {
            crash_count = 0;
            ESP_ERROR_CHECK_WITHOUT_ABORT(nvs_set_u8(nvs_handle, NVS_CRASH_COUNT_NAME, crash_count));
        }
        if (crash_flag == 1)
        {
            crash_count += 1;
            ESP_ERROR_CHECK_WITHOUT_ABORT(nvs_set_u8(nvs_handle, NVS_CRASH_COUNT_NAME, crash_count));
        }
        else // if (crash_flag == 0)
        {
            ESP_ERROR_CHECK_WITHOUT_ABORT(nvs_set_u8(nvs_handle, NVS_CRASH_FLAG_NAME, 1));
            if (crash_count > 0)
            {
                crash_count = 0;
                ESP_ERROR_CHECK_WITHOUT_ABORT(nvs_set_u8(nvs_handle, NVS_CRASH_COUNT_NAME, crash_count));
            }
        }
    }
    ESP_ERROR_CHECK_WITHOUT_ABORT(nvs_commit(nvs_handle));
    nvs_close(nvs_handle);
    gCrash_count = crash_count;

    // Set app loading callback
    EhsHSetAppLoadStatusCallback(app_load_status_handler);
    //todo2024 - review if this should be in or out.
    // /*** Determine the booting behaviour according to the crash count ***/
    // // Level 0 - Switch the application to the previous one (fallbacks)
    // if (crash_count >= CRASH_COUNT_LEVEL0 && crash_count < CRASH_COUNT_LEVEL1)
    // {
    //     EhsHMetaSetNextAppToRun(EHS_RUNTIME_APPDATAFALLBACKS_DIR_STRING);
    // }
    // // Level 1 - 
    // else if (crash_count >= CRASH_COUNT_LEVEL1 && crash_count < CRASH_COUNT_LEVEL2)
    // {}

    const esp_partition_t* current_partition = esp_ota_get_running_partition();
    const esp_partition_t* appdata_partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_SPIFFS, "appdata");
    size_t nvs_read_required_size;
    size_t ota_data_writing_index = 0, ota_data_writing_size = 2048;
    if (APP_DATA_BIN_LEN > 0)
    {
        ret = nvs_open(NVS_OTA_PART_NAMESPACE, NVS_READWRITE, &nvs_handle);
        if (ret == ESP_OK)
        {
            ret = nvs_get_str(nvs_handle, NVS_OTA_PART_SEC_NAME, NULL, &nvs_read_required_size);
            if (ret == ESP_ERR_NVS_NOT_FOUND) goto ota_data_write_jump;
            char *nvs_secname = (char *) malloc(nvs_read_required_size);
            ret = nvs_get_str(nvs_handle, NVS_OTA_PART_SEC_NAME, nvs_secname, &nvs_read_required_size);
            if (strcmp(current_partition->label, nvs_secname) != 0)
            {
ota_data_write_jump:
                if (appdata_partition != NULL) 
                {
                    ESP_LOGW(TAG, "Erase the entire appdata binary...");
                    esp_partition_erase_range(appdata_partition, 0, appdata_partition->size);
                }
                // Write the appdata binary into the partition
                ESP_LOGW(TAG, "Writing appdata binary into the data partition...");
                for (ota_data_writing_index = 0 ; ota_data_writing_index < APP_DATA_BIN_LEN ; ota_data_writing_index += ota_data_writing_size)
                {
                    if (appdata_partition == NULL) break;
                    esp_partition_write(appdata_partition, ota_data_writing_index, APP_DATA_BIN + ota_data_writing_index ,ota_data_writing_size);
                }
                // Set the nvs to the current partition label
                if (appdata_partition != NULL) nvs_set_str(nvs_handle, NVS_OTA_PART_SEC_NAME, current_partition->label);
                ESP_LOGW(TAG, "Writing appdata binary complete!");
            }
        }
    }

    #if TARGET_USE_WIFI == 1
    // Append unique suffix from MAC
    uint8_t mac[6];
    esp_mac_type_t mac_type = ESP_MAC_EFUSE_FACTORY; // Use the efuse which was burnt by Espressif in production
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_read_mac(mac, mac_type));
    snprintf(gWiFiHostNameBuffer, sizeof(gWiFiHostNameBuffer), TARGET_HOSTNAME"-w%02X%02X%02X%02X", mac[2], mac[3], mac[4], mac[5]);
    printf("Wi-Fi host name : %s\n", gWiFiHostNameBuffer);
     if (doWifiStationNetifInit(gWiFiHostNameBuffer) == EHS_TRUE)
    #elif TARGET_USE_ETHERNET == 1
     if (eth_init() == ESP_OK)
    #endif
     {
        ESP_LOGI(TAG, "Connection success");
     }
     else
     {
        ESP_LOGE(TAG, "Connection failed");
     }
 
 #define EHS_ERT_KERNEL_AVAILABLE
 #ifdef EHS_ERT_KERNEL_AVAILABLE
 
    TaskHandle_t xHandle = NULL;
    uint32_t stack_depth = EHS_MAIN_ESP32_TASK_STACK_SIZE;
 
    // we need to specify the installation path before starting EhsMain, 
    // so that things like user dir gets set-up properly
    EhsHMetaSetInstPath(base_path);

    // create eRT main loop task
    ESP_LOGI(TAG,"EhsMain stack depth = %u", stack_depth);
    // Disable Watchdog timer
    gTimg1Config0 = EHS_REG32(ESP32S3_TIMG1_BASE + ESP32S3_TIMG_WDT_CONFIG0_OFFSET);
    EHS_REG32(ESP32S3_TIMG1_BASE + ESP32S3_TIMG_WDT_WRITEPROTECT_OFFSET) = ESP32S3_TIMG_WDT_WRITEPROTECT_MAGIC_VALUE;
    EHS_REG32(ESP32S3_TIMG1_BASE + ESP32S3_TIMG_WDT_CONFIG0_OFFSET) = 0;
    EHS_REG32(ESP32S3_TIMG1_BASE + ESP32S3_TIMG_WDT_WRITEPROTECT_OFFSET) = 0;
    xTaskCreate(EhsMain, "EhsMain", stack_depth, NULL, EHS_PRI_EHS_MAIN, xHandle); // tskIDLE_PRIORITY + 5
 
 #endif
 #ifdef EHS_ESP32_CMD_PROMPT_SUPPORT
    // TODO - shell we use this in MCU_SLOW_LP_THR ?
    // create a command prompt task for interacting with the device over a console
    xTaskCreate(command_prompt_task, "CommandPrompt", 4096, NULL, EHS_PRI_SERIAL_CMD, NULL);
#endif
    xTaskCreate(MCU_SLOW_HP_THR, "MCU_SLOW_HP_THR", 3072, NULL, EHS_PRI_MCU_SLOW_HP_THR, NULL);
    MCU_SLOW_LP_THR(NULL);
    // vTaskDelete(NULL);
}

/**
 * Handle the SIGTERM signal
 */
void EhsTargetHandleTerm(int sig)
{
    /* Tod we should call some HAL tear down functions here libxml , libcurl
     * etc.*/
    EhsExit(0);
}

/* House keeping life cycle functions
  todo2022 these would normally go in the target_is_init.cfile
*/
