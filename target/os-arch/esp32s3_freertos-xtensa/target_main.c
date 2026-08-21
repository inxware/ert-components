/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
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

#include "globals.h"

#ifdef EHS_DEBUG_TCPIP_CONSOLE
  #include "console_server.h"
#endif

#include "ehs_main.h"
#include "targetos_init.h"
#include "hal-api.h" // required for the meta data storage

#ifdef EHS_MQTT_SUPPORT
#include "hal_mqtt.h"
#endif

#include "hal_network.h"
#include "hal_configs.h"

#include "sdkconfig.h"
#include "driver/uart.h"
#include "hal_console.h"
#include "freertos/FreeRTOS.h"
#include <freertos/task.h>
#ifdef CONFIG_SPIRAM
#include "freertos/idf_additions.h"
#endif

#ifdef EHS_RUNTIME_LOGGER_ENABLED
#include "esp_log.h"
#else
#define ESP_LOGI(...)
#define ESP_LOGE(...)
#define ESP_LOGW(...)
#define ESP_LOGD(...)
#endif
#include <stdio.h>


// #todo if need to include the following there are some #includes that fail ,
// which seems to depend on having the GNU macro defined #include
//"freertos/task.h"

#include "esp_event.h"

#ifdef EHS_NETWORK_WIFI_SUPPORT
#include "esp_wifi.h"
#include "target_wifi.h"
#include "wifi_station.h"
#endif

#ifdef EHS_NETWORK_BLE_SUPPORT
#include "ble_service_nimble.h"
#endif

#ifdef EHS_LORAWAN_SUPPORT
#include "lorawan.h"  /* LoRaWAN_module_peek* for the serial-console 'L' key */
#endif

#include "banner.h"  /* EhsBanner_PrintHelp — common banner+help for serial console */
#ifdef EHS_SERIAL_CONSOLE_SUPPORT
#include "serial_console.h"        /* command_prompt_task & helpers              */
#include "hal_serial_console.h"    /* EhsSerial_FlushTx / Write / Init           */
#endif

//todo need to change this to a normal config method
#ifdef EHS_NETWORK_ETHERNET_SUPPORT
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

/* todo This should be ehs_string.h */
#include <string.h>

#include "target_uart.h"
#include "target_ota.h"

#include "esp_rom_uart.h"  /* for esp_rom_uart_tx_wait_idle - console TX flush */
#include <unistd.h>        /* write(), fileno() — used by per-char console echo */
#if CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG
#include "hal/usb_serial_jtag_ll.h"  /* IDF VFS only flushes JTAG TX FIFO on '\n' */
#endif

#include "target_data_bin.h"

#ifdef EHS_RTC_SUPPORT
#include "target_hal_rtc.h"
#endif

#include "esp_heap_caps.h"
#include "hal_watchdog.h"
#include "esp_task_wdt.h"
#include "esp_system.h"

#include "hal_nvs.h"

#define TAG "target_main"

#define ESP32S3_TIMG0_BASE 0x6001F000
#define ESP32S3_TIMG1_BASE 0x60020000
#define ESP32S3_TIMG_WDT_CONFIG0_OFFSET 0x0048
#define ESP32S3_TIMG_WDT_WRITEPROTECT_OFFSET 0x0064
#define ESP32S3_TIMG_WDT_WRITEPROTECT_MAGIC_VALUE 0x50d83aa1

#define EHS_REG32(x) (*((volatile ehs_uint32 *) x))

volatile static ehs_uint32 gTimg1Config0 = 0;

#ifndef EHS_MAIN_ESP32_TASK_STACK_SIZE
#  if defined(CONFIG_SPIRAM) && defined(EHS_ESP32_PSRAM_EHSMAIN)
#    define EHS_MAIN_ESP32_TASK_STACK_SIZE 24000  /* PSRAM stack — size is essentially free */
#  else
#    define EHS_MAIN_ESP32_TASK_STACK_SIZE 10000
#  endif
#endif

#ifdef CONFIG_SPIRAM
#  define EHS_MCU_SLOW_HP_THR_STACKSIZE 6144
#else
#  define EHS_MCU_SLOW_HP_THR_STACKSIZE 3072
#endif

#ifndef EHS_MCU_SLOW_LP_THR_SLEEP_MS
#define EHS_MCU_SLOW_LP_THR_SLEEP_MS 10 /*ms*/
#endif


// set this to '1' for stdout memory usage every second
#define EHS_ESP32_ENABLE_MEMORY_MONITOR 0

#define CONFIG_ESP_MAXIMUM_RETRY 5



extern const char* base_path; // ert install path, defined in target_file

// #ifndef EHS_NETWORK_ETHERNET_SUPPORT
//     #ifndef EHS_NETWORK_WIFI_SUPPORT
//         #error EHS_NETWORK_ETHERNET_SUPPORT or EHS_NETWORK_WIFI_SUPPORT should be defined!
//     #endif
// #endif
// 
// #if defined(EHS_NETWORK_ETHERNET_SUPPORT) && defined(EHS_NETWORK_WIFI_SUPPORT)
//     #error You cannot use Ethernet and Wi-Fi at the same time!
// #endif
// 
// #if !defined(EHS_NETWORK_ETHERNET_SUPPORT) && !defined(EHS_NETWORK_WIFI_SUPPORT)
//     #error You must set one of Ethernet or Wi-Fi to be used for some network support
// #endif

#ifndef TARGET_HOSTNAME
    #define TARGET_HOSTNAME "INX-ESP32S3"
#endif

#ifdef EHS_NETWORK_WIFI_SUPPORT
    /* A character buffer, not an array of pointers - the '*' made sizeof() 128 and passed
     * ehs_char** to snprintf/doWifiStationNetifInit. "INX-HRDCv2-wXXXXXXXX" is 21 chars. */
    ehs_char gWiFiHostNameBuffer[32] = {'\0'};
#endif

/** Crash Recovery Global Variables & Definitions **/
#define NVS_CRASH_NAMESPACE "crash"
#define NVS_CRASH_FLAG_NAME "cf"
#define NVS_CRASH_COUNT_NAME "cc"
static uint8_t gCrash_count = 0;

/* Map the ESP-IDF reset reason onto the platform-agnostic EhsResetReasonType enum
 * so the kernel / =n report can classify reboots without caring about ESP specifics.
 * ESP_RST_USB/ESP_RST_JTAG were added in later ESP-IDF releases — guard them so older
 * IDF headers still build. */
static EhsResetReasonType esp_reset_reason_to_ehs(esp_reset_reason_t r)
{
    switch (r) {
        case ESP_RST_POWERON:   return EHS_RESET_REASON_POWER_ON;
        case ESP_RST_SW:        return EHS_RESET_REASON_SOFTWARE;
        case ESP_RST_EXT:       return EHS_RESET_REASON_EXTERNAL;
        case ESP_RST_PANIC:     return EHS_RESET_REASON_PANIC;
        case ESP_RST_INT_WDT:   return EHS_RESET_REASON_INT_WATCHDOG;
        case ESP_RST_TASK_WDT:  return EHS_RESET_REASON_TASK_WATCHDOG;
        case ESP_RST_WDT:       return EHS_RESET_REASON_OTHER_WATCHDOG;
        case ESP_RST_DEEPSLEEP: return EHS_RESET_REASON_DEEP_SLEEP_WAKE;
        case ESP_RST_BROWNOUT:  return EHS_RESET_REASON_BROWNOUT;
        case ESP_RST_SDIO:      return EHS_RESET_REASON_SDIO;
#ifdef ESP_RST_USB
        case ESP_RST_USB:       return EHS_RESET_REASON_USB;
#endif
#ifdef ESP_RST_JTAG
        case ESP_RST_JTAG:      return EHS_RESET_REASON_JTAG;
#endif
        default:                return EHS_RESET_REASON_UNKNOWN;
    }
}
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

/** Network Interface Config **/
#define NVS_INTERFACE_CONFIG_NAMESPACE "if_config"
#define NVS_INTERFACE_CONFIG_ETH_NAME "eth"
#define NVS_INTERFACE_CONFIG_WIFI_NAME "wifi"

#define EHS_NET_CONFIG_FILE "/ehs/userdata/config/net_config"
#define EHS_INTERFACE_CONFIG_FILE "/ehs/userdata/config/if_config"

#ifdef EHS_MAX31343_SUPPORT 
ehs_sint32 gEhsAmbientTemp = 0;
#endif

/* Callback used for notifing about state of the file system */
extern void (*ehs_filesystem_initalised_callback)(void);

/* Flag gets set to true when the file system has been initalised */
volatile ehs_bool gFileSystemInitalised = EHS_FALSE;

/* This flag is set to true once the app loading process completes regardless of failing or being sucessful. */
/* @TODO - create a proper state machine, to take appropriate action depending on the state of the app 
   e.g. free mqtt stack (if previously allocated) when the app fails to load, so there's enough resources for re-parsing */
volatile ehs_bool gAppLoadingDone = EHS_FALSE;

/* Returns if the build has WiFI Enabled */
static ehs_bool gEhsNetworkInterfaceWifiEnable
#if (defined(EHS_NETWORK_WIFI_SUPPORT))
= EHS_TRUE; // only wifi
#else
= EHS_FALSE; // ethernet only or both
#endif

static ehs_bool gEhsNetworkInterfaceEthEnable
#if defined(EHS_NETWORK_ETHERNET_SUPPORT)
= EHS_TRUE; // When Ethernet is configured , default to ethernet
#else
= EHS_FALSE; // only wifi
#endif

ehs_bool EhsNetworkInterfaceWifiIsEnabled()
{
    return gEhsNetworkInterfaceWifiEnable;
}

ehs_bool EhsNetworkInterfaceEthIsEnabled()
{
    return gEhsNetworkInterfaceEthEnable;
}

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
#ifdef EHS_TARGET_APPLOAD_RESTARTING_REBOOT
            printf(" EHS_APP_LOAD_RESTART.....\n");
            ESP_LOGI(TAG, "Rebooting now to load new application from flash");
            EhsHSysReboot();
#endif
            break;
        }
        case EHS_APP_LOAD_FAILED : {
            ESP_LOGE(TAG, "******* App loading failed *******");
            gAppLoadingDone = EHS_TRUE;
            break;
        }
        default:
            ESP_LOGW(TAG, "Unknown app loading status!");
            //EhsHWatchdogEnable();
            break;
    }
}

/* TODO2026 - this is all pretty much common code (?) except we haven'y bothered to use the abstracted delays ...*/



/* The next two functions read/write a key=value config file via the
 * EhsConfig* HAL declared in hal_configs.h. That HAL is only compiled
 * when EHS_CONFIGS_SUPPORT is set in the platform's config.mk, so guard
 * the definitions to match — otherwise targets that opt out of
 * EHS_CONFIGS_SUPPORT pick up unresolved EhsConfig* references that
 * only link today by accident (dead-stripping). */
#ifdef EHS_CONFIGS_SUPPORT
/*
 Loads the TCPIP configuration file (from user file systemand applies to the currently enabled interface
*/
void EhsLoadNetworkConfig()
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
        EhsNetworkConfigure(&net_config);
        EhsConfigFree(config); //TODO 2027  Can't we just create a config struct at the beginning of this and pass it into EhsCOnfigLoad to be populated?
    }
}
/* 
 Saves the TCPIP configuration from the currently enabled interface 
*/
void EhsSaveNetworkConfig(const EhsNetworkConfigDataType* net_config)
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
#endif /* EHS_CONFIGS_SUPPORT */


/* todo this should really go in a targetnetworking file?*/
// Override functions for esp32 network config
#ifdef EHS_HAL_NETWORK_CONFIG_SUPPORT
#if EHS_HAL_NETWORK_CONFIG_SUPPORT != EHS_HAL_NETWORK_CONFIG_STUBBED
//#ifdef EHS_HAL_NETWORK_CONFIG_SUPPORT

/* Set to true when device has network availble. Set to false when the network is disconnected. */
/* @TODO - At the momemnt we only use this in the ethernet mode, we should do the same in the wifi mode. */
volatile ehs_bool gNetworkConnected = EHS_FALSE;


/* Returns true when the eRT target network is connected */
ehs_bool EhsNetworkIsConnected()
{
    return (gEhsNetworkInterfaceEthEnable && gNetworkConnected) || (gEhsNetworkInterfaceWifiEnable 
        #ifdef EHS_NETWORK_WIFI_SUPPORT
        && isWifiStationConnected()
        #endif// EHS_NETWORK_WIFI_SUPPORT
    );
}

/* Configures the traget network interface */
ehs_sint32 EhsNetworkConfigure(const EhsNetworkConfigDataType* config)
{
    esp_netif_t* netif;
    if (!gEhsNetworkInterfaceEthEnable && gEhsNetworkInterfaceWifiEnable)
        netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    else if (gEhsNetworkInterfaceEthEnable && !gEhsNetworkInterfaceWifiEnable)
        netif = esp_netif_get_handle_from_ifkey("ETH_DEF");
    else {
        ESP_LOGE(TAG, "Network Config - Invalid network interface state!");
        return EHS_NETWORK_CONFIG_INVALID_PARAM_ID;
    }
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
#ifdef EHS_CONFIGS_SUPPORT
        EhsSaveNetworkConfig(config);
#endif
    }
    return nError;
}

static void EhsSaveNetworkInterfaceConfig(const EhsNetworkInterfaceConfigDataType* if_config)
{
    if(if_config == NULL){
        return;
    }
    esp_err_t err;
    nvs_handle_t nvs_handle;
    err = nvs_open(NVS_INTERFACE_CONFIG_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%d) opening NVS handle!\n", err);
        return;
    }
    err = nvs_set_u8(nvs_handle, NVS_INTERFACE_CONFIG_ETH_NAME, (if_config->b_eth_enable == EHS_TRUE) ? 1 : 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%d) saving Ethernet config to NVS!\n", err);
    }
    err = nvs_set_u8(nvs_handle, NVS_INTERFACE_CONFIG_WIFI_NAME, (if_config->b_wifi_enable == EHS_TRUE) ? 1 : 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%d) saving WiFi config to NVS!\n", err);
    }
    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%d) committing NVS config!\n", err);
    }
    nvs_close(nvs_handle);
}
#endif // EHS_HAL_NETWORK_CONFIG_SUPPORT
#endif

#ifdef EHS_HAL_INTERFACE_CONFIG_SUPPORT
#if EHS_HAL_INTERFACE_CONFIG_SUPPORT != EHS_HAL_INTERFACE_CONFIG_STUBBED

/* Configures the target network interface (Ethernet/Wi-Fi) 
   THis does not set up the IP settings. It only enables/disables the Ethernet or Wi-Fi interface.
*/

ehs_sint32 EhsNetworkInterfaceConfigure(const EhsNetworkInterfaceConfigDataType *config)
{   
    if(config == NULL){
        return EHS_NETWORK_CONFIG_INVALID_PARAM_ID;
    }
    // #if !defined(EHS_NETWORK_ETHERNET_SUPPORT) && !defined(EHS_NETWORK_WIFI_SUPPORT)
    //     #error "No network interface defined for this target! "
    // #endif//!EHS_NETWORK_ETHERNET_SUPPORT && !EHS_NETWORK_WIFI_SUPPORT
    // Currently do not support Ethernet and Wi-Fi enabled at the same time
    if (config->b_eth_enable == EHS_TRUE && config->b_wifi_enable == EHS_TRUE)
    {
        ESP_LOGE(TAG, "Network Interface Config - You cannot enable Ethernet and Wi-Fi at the same time!");
        return EHS_NETWORK_CONFIG_FAILED_NOT_SUPPORTED_ID;
    }
    
#if defined(EHS_NETWORK_ETHERNET_SUPPORT)
        if (gEhsNetworkInterfaceEthEnable != config->b_eth_enable)
        {
            if (config->b_eth_enable == EHS_TRUE)
            {
                if (ehs_eth_init() == ESP_OK)
                {
                    ESP_LOGI(TAG, "Ethernet Connection success");
                }
                else
                {
                    ESP_LOGE(TAG, "Ethernet Connection failed");
                }
            }
            else
            {
                if (eth_deinit() == ESP_OK)
                {
                    ESP_LOGI(TAG, "Ethernet Disconnection success");
                }
                else
                {
                    ESP_LOGE(TAG, "Ethernet Disconnection failed");
                }
            }
        }
        gEhsNetworkInterfaceEthEnable = config->b_eth_enable;
#endif//EHS_NETWORK_ETHERNET_SUPPORT
    #if defined(EHS_NETWORK_WIFI_SUPPORT)
        if (gEhsNetworkInterfaceWifiEnable != config->b_wifi_enable)
        {
            if (isEhsWiFiManagedByComponent() == EHS_TRUE)
            {
                if (config->b_wifi_enable == EHS_TRUE)
                {
                    setWifiStationConnectState(WifiStationConnectState_CONNECT);
                }
                else
                {
                    EhsTPMutex_lock(EhsTPMutex_fbIO);
                    EhsWifiStationSetCBSource(eWifiStationCallbackSource_Disconnect);
                    EhsTPMutex_unlock(EhsTPMutex_fbIO);
                }
            }
            else
            {
                if (config->b_wifi_enable == EHS_TRUE)
                {
                    setWifiStationConnectState(WifiStationConnectState_CONNECT);
                }
                else
                {
                    doWifiStationDestroy();
                    setWifiStationConnectState(WifiStationConnectState_IDLE);
                }
            }
        }
        gEhsNetworkInterfaceWifiEnable = config->b_wifi_enable;
    #endif//EHS_NETWORK_WIFI_SUPPORT
    if (config->save == EHS_TRUE)
    {
        EhsSaveNetworkInterfaceConfig(config);
    }

    return EHS_NETWORK_CONFIG_NO_ERROR_ID;
}

/* Loads the network interface config from NV RAM for Ethernet and WIFI
 Currently just saves enabling and disabling interfaces 
 */

static void EhsLoadNetworkInterfaceConfig()
{
    esp_err_t err;
    nvs_handle_t nvs_handle;
    err = nvs_open(NVS_INTERFACE_CONFIG_NAMESPACE, NVS_READONLY, &nvs_handle);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGI(TAG, "No network interface config in NVS");
        return;
    } else if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%d) opening NVS handle!\n", err);
        return;
    }
    uint8_t eth_enable = 0, wifi_enable = 0;
    err = nvs_get_u8(nvs_handle, NVS_INTERFACE_CONFIG_ETH_NAME, &eth_enable);
    if (err == ESP_OK) gEhsNetworkInterfaceEthEnable = (eth_enable == 1) ? EHS_TRUE : EHS_FALSE;
    err = nvs_get_u8(nvs_handle, NVS_INTERFACE_CONFIG_WIFI_NAME, &wifi_enable);
    if (err == ESP_OK) gEhsNetworkInterfaceWifiEnable = (wifi_enable == 1) ? EHS_TRUE : EHS_FALSE;
    nvs_close(nvs_handle);
}
#endif // EHS_HAL_INTERFACE_CONFIG_SUPPORT
#endif // EHS_INTERFACE_CONFIGSUPPORT != stubed

void EhsFilesystemInitalised()
{
    gFileSystemInitalised = EHS_TRUE;
}

/*
 * MCU_SLOW_HP_THR — High-Priority Auxiliary Loop
 *
 * Priority : EHS_PRI_MCU_SLOW_HP_THR (17) — just below EhsMain (18)
 * Stack    : EHS_MCU_SLOW_HP_THR_STACKSIZE (3072 bytes DRAM / 6144 bytes PSRAM)
 * Period   : 10 ms vTaskDelay
 *
 * PURPOSE
 *   Time-sensitive periodic background work that must not be starved by the
 *   low-priority network/IO thread.  Currently: Modbus master/slave polling.
 *
 * CONSTRAINTS — strictly enforced, do not violate:
 *   - NO printf, ESP_LOG*, or any console output.
 *   - NO blocking network calls (MQTT, HTTP, OTA, socket reads).
 *   - NO file I/O.
 *   - NO flash or NVS writes of any kind.  On CONFIG_SPIRAM builds this
 *     task's stack is in PSRAM; flash ops disable the cache, making the
 *     stack unreachable mid-operation → Double exception, corrupted
 *     backtrace.  Route flash/NVS work to a thread with an internal stack.
 *   - NO WiFi scan result printing or console command handling.
 *   - Work items must complete well within the 10 ms budget to avoid
 *     starving EhsMain.  If a new work item might block or take variable
 *     time, put it in MCU_SLOW_LP_THR instead.
 *   NOTE: EhsHMBTaskInLoop uses the UART driver's DMA queue interface and
 *   should not block indefinitely, but monitor if Modbus is added to a
 *   new target — verify the HAL implementation is non-blocking.
 */
void MCU_SLOW_HP_THR(void *pvp)
{
    ehs_threadname_t threadname = EHSTHREADNAME_MCU_SLOW_HP_THR;
    for (;;)
    {
        EhsHStatisticsLoopStart(threadname);
#ifdef EHS_MODBUS_SUPPORT
        EhsHMBTaskInLoop( EHS_TRUE );
#endif
        vTaskDelay( pdMS_TO_TICKS( 10 ) ); //todo2025 review period — should be driven by required Modbus poll rate, not a fixed tick count
        EhsHStatisticsLoopEnd(threadname);
    }
}

/*
 * MCU_SLOW_LP_THR — Low-Priority Background Services Loop
 *
 * Priority : EHS_PRI_MCU_SLOW_LP_THR (1) — well below EhsMain; preempted freely
 * Stack    : CONFIG_ESP_MAIN_TASK_STACK_SIZE (5120 bytes) — runs in app_main's task
 * Period   : EHS_MCU_SLOW_LP_THR_SLEEP_MS (10 ms) minimum sleep
 *
 * PURPOSE
 *   Runs background services that are non-time-critical and may block briefly.
 *   Currently: MQTT client loop, OTA polling, UART TX buffer draining
 *   (TgtUART_SendInThread), serial console TX flush, I2C/RTC periodic sync.
 *
 * CONSTRAINTS:
 *   - Low priority means it yields to almost everything; do NOT put work here
 *     that needs guaranteed latency.
 *   - Stack is shared with app_main init overhead.  Keep individual call-chain
 *     depth shallow — avoid deeply nested calls or large stack-local buffers.
 *   - printf is acceptable but keep it minimal (diagnostic only); heavy output
 *     belongs in command_prompt_task.
 *   - WiFi scan result printing must NOT be done here — use command_prompt_task
 *     (see WifiStationIsScanResultReady / WifiStationPrintAndClearScanResults).
 *   - Do not add blocking socket/file reads that could stall the loop for more
 *     than a few milliseconds; use non-blocking / timeout-bounded APIs only.
 */
void MCU_SLOW_LP_THR(void *pvParameters)
{
    ehs_threadname_t threadname = EHSTHREADNAME_MCU_SLOW_LP_THR;
    // Flag used for indicating whether the inital network settings have been loaded
    ehs_bool bNetworkSettingsLoaded = EHS_FALSE;
    ehs_bool bOtaInitalised = EHS_FALSE;
    int i;

    //todo2025 - change this MACRO to EHS_RTC___MAX31343  (I think we agreed to use "___" for equals in macro land)
#ifdef EHS_MAX31343_SUPPORT 
    int j = 0;
    //char str_datetime[21] = { 0 };
#endif//EHS_MAX31343_SUPPORT 
// todo2025 Do we use _SUPPORT to say enabled usually?? for EHS_I2C_SUPPORT or EHS_I2C___ENABLED???
#if defined(EHS_MAX31343_SUPPORT) && defined(EHS_I2C_SUPPORT)
    EhsTI2CMasterInit();
#endif//EHS_MAX31343_SUPPORT && EHS_I2C_SUPPORT
    int k = 0;
    /* No WiFi state forcing here: the boot path in app_main (netif init +
     * doWifiStationInit + EhsStartWifiStationThread) owns the connect
     * request; forcing CONNECT from this thread raced the SM. */
    for (;;)
    {
        EhsHStatisticsLoopStart(threadname);
        // this statement runs or initalises services, which should wait until
        // parsing process is compleated
        if(gAppLoadingDone==EHS_TRUE){
            
#ifdef EHS_NETWORK_WIFI_SUPPORT
#else
            // load network settings from the file, once both file system and network
            // stack have been initalised
            if(bNetworkSettingsLoaded==EHS_FALSE){
#ifdef EHS_CONFIGS_SUPPORT
                EhsLoadNetworkConfig();
#endif
                bNetworkSettingsLoaded = EHS_TRUE;
            }
#endif

            /* Gate MQTT and OTA loops on actual link state via EhsNetworkIsConnected().
             * Both interfaces may be compiled in and the active one can be switched
             * via NVS; the helper ORs (Eth enabled && link up) with (WiFi enabled &&
             * station connected), so a fallback branch can't fire MQTT init before
             * a route exists. */
#ifdef EHS_MQTT_SUPPORT
            {
                static int s_net_diag_count = 0;
                ehs_bool net_ok = EhsNetworkIsConnected();
                if (net_ok) {
                    if (s_net_diag_count++ == 0)
                        ESP_LOGI(TAG, "MCU_SLOW_LP_THR: network up, starting MQTT loop");
                    EhsMqttClientLoop( (void*)EhsMqttDevmanMonSupport() );
                } else {
                    s_net_diag_count = 0;
                    static int s_no_net_count = 0;
                    if (++s_no_net_count % 500 == 0)
#ifdef EHS_NETWORK_WIFI_SUPPORT
                        ESP_LOGW(TAG, "MCU_SLOW_LP_THR: waiting for network (wifi_en=%d wifi_connected=%d)",
                               (int)gEhsNetworkInterfaceWifiEnable, (int)isWifiStationConnected());
#else
                        ;
#endif // EHS_NETWORK_WIFI_SUPPORT
                }
            }
#endif // EHS_MQTT_SUPPORT

#if EHS_OTA_SUPPORT == EHS_OTA_SUPPORT_SUPPORT
            if (EhsNetworkIsConnected()) {
                target_OTA_task(NULL);
            }
#endif //EHS_OTA_SUPPORT

            // @TODO - This is used by Uart function block - needs to review and potentially moved or Wifi connect needs to be done non-blockig (prefered)
#ifdef EHS_SERIAL_CONSOLE_SUPPORT
            EhsSerial_FlushTx();
#endif
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
#ifdef EHS_NETWORK_WIFI_SUPPORT
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
        EhsHStatisticsLoopEnd(threadname);
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
 * @return void
 *
 * eRT's Entry point for esp32 platform.
 * Note: esp32's freertos calls appmain  you don't use normal main.
 * 
 * There’s already an api for setting the next app to be launched. Simply call this at any point before EhsMain
 * EhsHMetaSetNextAppToRun("default");  or EhsHMetaSetNextAppToRun("fallbacks"); etc.
 */
/* FreeRTOS task entry: TaskFunction_t is void(*)(void*), but EhsMain takes two args
 * (loop-iteration callback + env blob). Passing EhsMain directly left the second
 * parameter reading whatever was in the argument register. Both are NULL here. */
static void EhsMainTaskEntry(void *pvParameters)
{
    (void)pvParameters;
    EhsMain(NULL, NULL);
}

void app_main(void)
{
#if defined(EHS_TEST_FUNC_OVERRIDE) && defined(EHS_TEST_FUNC_NO_ERT_INIT)
    // Bare metal mode: Run test immediately and hang
    extern void EHS_TEST_FUNC_NAME(void);
    ESP_LOGI(TAG, "EHS Bare Metal Test: Running %s", EHS_MACRO_STRINGIFY(EHS_TEST_FUNC_NAME));
    EHS_TEST_FUNC_NAME();
    ESP_LOGI(TAG, "Test completed");
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
#endif//defined(EHS_TEST_FUNC_OVERRIDE) && defined(EHS_TEST_FUNC_NO_ERT_INIT)

    ESP_LOGI(TAG, "eRT app_main starting");
#ifdef EHS_TARGET_TASK_WATCHDOG
    /* Keep the task watchdog running so a hang panics with a backtrace instead of going
     * silent. Only subscribed tasks are checked, so this catches a task that spins or
     * stops feeding the watchdog; a task blocked on a mutex is not detected unless it is
     * subscribed and resets the watchdog itself. Set via EHS_TARGET_TASK_WATCHDOG in
     * config.mk (unset or 'no' = deinit, the previous behaviour). Timeout defaults to 8 s;
     * a platform can override EHS_TARGET_TASK_WATCHDOG_MS via DEFS if it needs a different
     * resolution. */
#ifndef EHS_TARGET_TASK_WATCHDOG_MS
#define EHS_TARGET_TASK_WATCHDOG_MS 8000
#endif
    {
        esp_task_wdt_config_t wdt_cfg = {
            .timeout_ms = EHS_TARGET_TASK_WATCHDOG_MS,
            .idle_core_mask = (1 << portNUM_PROCESSORS) - 1,
            .trigger_panic = true
        };
        if (esp_task_wdt_reconfigure(&wdt_cfg) != ESP_OK)
        {
            ESP_ERROR_CHECK_WITHOUT_ABORT(esp_task_wdt_init(&wdt_cfg));
        }
        ESP_LOGW(TAG, "Task watchdog enabled: %d ms", (int)EHS_TARGET_TASK_WATCHDOG_MS);
    }
#else
    esp_task_wdt_deinit();
#endif
#if EHS_ESP32_ENABLE_LOGS == 1
    /* Verbose-logging mode: full-default INFO floods the sys_evt task's
     * ~2.3 KB stack via vsnprintf in the wifi/lwip event-handler context
     * (StackOverflow in sys_evt). Cap everything to WARN, then re-raise
     * just the tags we actually want to diagnose. */
    esp_log_level_set("*", ESP_LOG_WARN);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("esp-tls", ESP_LOG_VERBOSE);
    esp_log_level_set("esp-tls-mbedtls", ESP_LOG_VERBOSE);
    esp_log_level_set("transport_base", ESP_LOG_VERBOSE);
#else
    esp_log_level_set("*", ESP_LOG_NONE);
#endif//EHS_ESP32_ENABLE_LOGS == 1
    /* EHS's own logger (EhsHLoggerModuleLogLevel[]) already gates every EHSH_LOG_* message
     * by module and level before it ever reaches EhsStdioPrintf/ESP_LOGx - don't let this
     * ESP-IDF tag-level cap silently double-gate it on top. Always let our own tag through;
     * the "*" cap above still applies to every other component's own ESP_LOGx calls. */
    esp_log_level_set(TS_TAG, ESP_LOG_VERBOSE);
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

    /* Capture and publish the reset reason so the kernel console (=n) and, when
     * EHS_DEBUG_TCPIP_CONSOLE is enabled, the Lucid IDE can report it. */
    {
        EhsResetReasonType ehs_reset_reason = esp_reset_reason_to_ehs(esp_reset_reason());
        EhsHMetaSetLastResetReason(ehs_reset_reason);
        ESP_LOGI(TAG, "Reset reason: %s (crash_count=%u)",
                 EhsHResetReasonToString(ehs_reset_reason), (unsigned)crash_count);
    }

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

#ifdef EHS_HAL_INTERFACE_CONFIG_SUPPORT
#if EHS_HAL_INTERFACE_CONFIG_SUPPORT != EHS_HAL_INTERFACE_CONFIG_STUBBED
    EhsLoadNetworkInterfaceConfig();
#endif//EHS_HAL_INTERFACE_CONFIG_SUPPORT != EHS_HAL_INTERFACE_CONFIG_STUBBED
#endif//EHS_HAL_INTERFACE_CONFIG_SUPPORT
    if (gEhsNetworkInterfaceWifiEnable)
    {
#ifdef EHS_NETWORK_WIFI_SUPPORT        
        ehs_bool startup = EHS_TRUE;
        ehs_nvs_obj_t nvs;
        EhsNvsOpen(&nvs, EHS_NVS_WIFI_NAMESPACE);
        EhsNvsGetBool(&nvs, EHS_NVS_WIFI_KEY_ONSTARTUP, &startup);
        EhsNvsClose(&nvs);
        // Append unique suffix from MAC
        uint8_t mac[6];
        esp_mac_type_t mac_type = ESP_MAC_EFUSE_FACTORY; // Use the efuse which was burnt by Espressif in production
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_read_mac(mac, mac_type));
        snprintf(gWiFiHostNameBuffer, sizeof(gWiFiHostNameBuffer), TARGET_HOSTNAME"-w%02X%02X%02X%02X", mac[2], mac[3], mac[4], mac[5]);
        ESP_LOGI(TAG, "Wi-Fi host name : %s", gWiFiHostNameBuffer);
        /* Netif first (sets hostname + default event loop), then driver init —
         * both idempotent.  Radio comes up in scan-only mode so console scans
         * work before any credentials are configured. */
        if (doWifiStationNetifInit(gWiFiHostNameBuffer) == EHS_TRUE) {
            doWifiStationInit();
            if (startup == EHS_TRUE) EhsStartWifiStationThread();
        }
        else ESP_LOGE(TAG, "Failed to initalise wifi netif");
#endif//EHS_NETWORK_WIFI_SUPPORT
    }
    if (gEhsNetworkInterfaceEthEnable)
    {
        
#ifdef EHS_NETWORK_ETHERNET_SUPPORT 
        if (ehs_eth_init() == ESP_OK)
        {
            ESP_LOGI(TAG, "Connection success");
        }
        else
        {
            ESP_LOGE(TAG, "Connection failed");
        }
#endif//EHS_NETWORK_ETHERNET_SUPPORT
    }
 

 #define EHS_ERT_KERNEL_AVAILABLE
 #ifdef EHS_ERT_KERNEL_AVAILABLE
 
    //TaskHandle_t xHandle = NULL; //todo not sure why this isn't found in FreeRTOS headers....
    //uint32_t stack_depth = EHS_MAIN_ESP32_TASK_STACK_SIZE; Same again..
 
    // we need to specify the installation path before starting EhsMain, 
    // so that things like user dir gets set-up properly
    EhsHMetaSetInstPath(base_path);

    // create eRT main loop task
    ESP_LOGI(TAG,"EhsMain stack depth = %u", EHS_MAIN_ESP32_TASK_STACK_SIZE);
    // Disable Watchdog timer
    gTimg1Config0 = EHS_REG32(ESP32S3_TIMG1_BASE + ESP32S3_TIMG_WDT_CONFIG0_OFFSET);
    EHS_REG32(ESP32S3_TIMG1_BASE + ESP32S3_TIMG_WDT_WRITEPROTECT_OFFSET) = ESP32S3_TIMG_WDT_WRITEPROTECT_MAGIC_VALUE;
    EHS_REG32(ESP32S3_TIMG1_BASE + ESP32S3_TIMG_WDT_CONFIG0_OFFSET) = 0;
    EHS_REG32(ESP32S3_TIMG1_BASE + ESP32S3_TIMG_WDT_WRITEPROTECT_OFFSET) = 0;

#ifdef EHS_TEST_FUNC_OVERRIDE
#if !defined(EHS_TEST_FUNC_NO_ERT_INIT)
    // Test mode with full init: Run test instead of EhsMain
    extern void EHS_TEST_FUNC_NAME(void);
    ESP_LOGI(TAG, "EHS Test Mode: Running %s", EHS_MACRO_STRINGIFY(EHS_TEST_FUNC_NAME));
#ifdef CONFIG_SPIRAM
    xTaskCreateWithCaps(EHS_TEST_FUNC_NAME, EHS_MACRO_STRINGIFY(EHS_TEST_FUNC_NAME), EHS_MAIN_ESP32_TASK_STACK_SIZE, NULL, EHS_PRI_EHS_MAIN, NULL, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
#else
    xTaskCreate(EHS_TEST_FUNC_NAME, EHS_MACRO_STRINGIFY(EHS_TEST_FUNC_NAME), EHS_MAIN_ESP32_TASK_STACK_SIZE, NULL, EHS_PRI_EHS_MAIN, NULL);
#endif//CONFIG_SPIRAM
#endif//!defined(EHS_TEST_FUNC_NO_ERT_INIT)
#else
    // Normal production mode
#if defined(CONFIG_SPIRAM) && defined(EHS_ESP32_PSRAM_EHSMAIN)
    xTaskCreateWithCaps(EhsMainTaskEntry, "EhsMain", EHS_MAIN_ESP32_TASK_STACK_SIZE, NULL, EHS_PRI_EHS_MAIN, NULL, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
#else
    xTaskCreate(EhsMainTaskEntry, "EhsMain", EHS_MAIN_ESP32_TASK_STACK_SIZE, NULL, EHS_PRI_EHS_MAIN, NULL);
#endif//defined(CONFIG_SPIRAM) && defined(EHS_ESP32_PSRAM_EHSMAIN)
#endif//EHS_TEST_FUNC_OVERRIDE

#endif//EHS_ERT_KERNEL_AVAILABLE
 
#ifdef EHS_SERIAL_CONSOLE_SUPPORT
    // TODO - shell we use this in MCU_SLOW_LP_THR ?
    // create a command prompt task for interacting with the device over a console
    /* Stack MUST be internal RAM (never MALLOC_CAP_SPIRAM): console commands
     * write flash — 'w'/'f' via NVS, 'D' via the filesystem — and flash ops
     * disable the cache, making a PSRAM stack unreachable mid-operation
     * (crashes with a Double exception / corrupted backtrace). */
    xTaskCreate(command_prompt_task, "CommandPrompt", 4096, NULL, EHS_PRI_SERIAL_CMD, NULL);
#endif//EHS_SERIAL_CONSOLE_SUPPORT
#ifdef CONFIG_SPIRAM
    /* PSRAM stack: MCU_SLOW_HP_THR must therefore NEVER do IO, flash or NVS
     * work — a flash op disables the cache and the task's own stack vanishes
     * (Double exception).  Keep it to sampling/compute; route IO to
     * CommandPrompt and flash writes to an internal-stack thread. */
    xTaskCreateWithCaps(MCU_SLOW_HP_THR, "MCU_SLOW_HP_THR", EHS_MCU_SLOW_HP_THR_STACKSIZE, NULL, EHS_PRI_MCU_SLOW_HP_THR, NULL, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
#else
    xTaskCreate(MCU_SLOW_HP_THR, "MCU_SLOW_HP_THR", EHS_MCU_SLOW_HP_THR_STACKSIZE, NULL, EHS_PRI_MCU_SLOW_HP_THR, NULL);
#endif//CONFIG_SPIRAM
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
