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
#include "freertos/FreeRTOS.h"
#include <freertos/task.h>

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

#include "esp_netif_types.h"

#ifdef EHS_NETWORK_WIFI_SUPPORT
#include "esp_wifi.h"
#include "target_wifi.h"
#include "wifi_station.h"
#endif

#ifdef EHS_NETWORK_BLE_SUPPORT
#include "ble_service_nimble.h"
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

#include "target_data_bin.h"

#ifdef EHS_RTC_SUPPORT
#include "target_hal_rtc.h"
#endif

#include "esp_heap_caps.h"
#include "hal_watchdog.h"
#include "esp_task_wdt.h"

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
#define EHS_MAIN_ESP32_TASK_STACK_SIZE 10000
#endif

#define EHS_MCU_SLOW_HP_THR_STACKSIZE 3072

#ifndef EHS_MCU_SLOW_LP_THR_SLEEP_MS
#define EHS_MCU_SLOW_LP_THR_SLEEP_MS 10 /*ms*/
#endif


// set this to '1' for stdout memory usage every second
#define EHS_ESP32_ENABLE_MEMORY_MONITOR 0

#define CONFIG_ESP_MAXIMUM_RETRY 5

#ifdef EHS_NETWORK_WIFI_SUPPORT

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

#endif //#ifdef EHS_NETWORK_WIFI_SUPPORT

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
    ehs_char* gWiFiHostNameBuffer[32] = {'\0'};
#endif
#ifdef EHS_NETWORK_ETHERNET_SUPPORT
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

/* Flag gets set to true once the network interface stack has been initalised and started */
volatile ehs_bool gNetworkStarted = EHS_FALSE;

/* Set to true when device has network availble. Set to false when the network is disconnected. */
/* @TODO - At the momemnt we only use this in the ethernet mode, we should do the same in the wifi mode. */
volatile ehs_bool gNetworkConnected = EHS_FALSE;

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
            EhsTargetReboot();
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
#ifdef EHS_SERIAL_CONSOLE_SUPPORT

#define EHS_PROMPT_READ_MAX 64 // if changed make sure to update "%Ns" in command_prompt_read "%(max_value-1)s"
#define EHS_PROMPT_READ_SLEEP 100 // wait 100ms before printing prompt?
#define EHS_PROMPT_CHAR_POLL_MS 10 // check for chars every 10ms

#ifdef EHS_WIFI_SSID_BUFF_MAX
#if EHS_PROMPT_READ_MAX != EHS_WIFI_SSID_BUFF_MAX
#error "EHS_PROMPT_READ_MAX and EHS_WIFI_SSID_BUFF_MAX must be the same"
#endif
#endif

/**
 * Flush stdout and wait for the console UART TX hardware FIFO to fully drain.
 * This prevents the issue where a subsequent blocking read (fscanf/stdin)
 * occurs before the UART has finished physically transmitting all bytes,
 * causing output to be lost or corrupted.
 *
 * Must be called instead of bare fflush(stdout) before any blocking read.
 */
static void console_flush_tx(void)
{
    fflush(stdout);
    /* Wait for UART TX FIFO to fully drain at hardware level.
     * CONFIG_ESP_CONSOLE_UART_NUM is defined by sdkconfig and defaults to 0. */
    esp_rom_uart_tx_wait_idle(CONFIG_ESP_CONSOLE_UART_NUM);
}

/* Prints a whole line with newline and ensures it's flushed immediately */
void command_prompt_println(const char* text)
{
    printf("%s\n", text);
    console_flush_tx();
    vTaskDelay(pdMS_TO_TICKS(EHS_PROMPT_READ_SLEEP)); // Brief delay after printing for readability
}

/**
 * Print text without newline - flushes and waits for UART TX to complete
 * before any subsequent blocking read.
 */
void command_prompt_print(const char* text)
{
    printf("%s", text);
    console_flush_tx();
}

/**
 * Echo mode for input reading
 */
typedef enum {
    ECHO_NORMAL,    // Echo characters as typed
    ECHO_PASSWORD,  // Echo asterisks for each character
    ECHO_HIDDEN     // No echo at all
} command_prompt_echo_mode_t;

//TODO These functions should go into a common HAL file.

/**  REad a single char (with or without echo) 
 * Read a line of input from stdin, handling character-at-a-time terminals.
 * Buffers input until Enter (CR or LF) is pressed.
 * Supports backspace for basic line editing.
 *
 * @param buffer Buffer to store the input (must be at least EHS_PROMPT_READ_MAX bytes)
 * @param echo_mode How to echo characters (ECHO_NORMAL, ECHO_PASSWORD, ECHO_HIDDEN)
 * @return Length of the input string (excluding null terminator)
 */
ehs_bool command_prompt_read_char(char * ch, command_prompt_echo_mode_t echo_mode)
{
    // Skip any leftover newlines/carriage returns from previous input
    do {
        fscanf(stdin, "%c", ch);
    } while (*ch == '\n' || *ch == '\r');

    switch (echo_mode) {
        case ECHO_NORMAL:
            printf("%c\n", *ch);
            console_flush_tx();
            break;
        case ECHO_PASSWORD:
            printf("*\n");
            console_flush_tx();
            break;
        case ECHO_HIDDEN:
            // No echo
            break;
    }
    return EHS_TRUE;
}

/**
 * Read a line of input from stdin, handling character-at-a-time terminals.
 * Buffers input until Enter (CR or LF) is pressed.
 * Supports backspace for basic line editing.
 *
 * @param buffer Buffer to store the input (must be at least EHS_PROMPT_READ_MAX bytes)
 * @param echo_mode How to echo characters (ECHO_NORMAL, ECHO_PASSWORD, ECHO_HIDDEN)
 * @return Length of the input string (excluding null terminator)
 */
ehs_uint32 command_prompt_read_with_echo(char* buffer, command_prompt_echo_mode_t echo_mode)
{
    ehs_uint32 pos = 0;
    char ch;

    // Clear the buffer
    buffer[0] = '\0';

    while (1) {
        // Try to read a single character
        if (fscanf(stdin, "%c", &ch) == 1) {
            // Check for Enter key (CR or LF)
            if (ch == '\n' || ch == '\r') {
                printf("\n");  // Echo newline
                console_flush_tx();
                buffer[pos] = '\0';
                return pos;
            }
            // Handle backspace (ASCII 8 or DEL 127)
            else if (ch == '\b' || ch == 127) {
                if (pos > 0) {
                    pos--;
                    buffer[pos] = '\0';
                    // Erase character on terminal: backspace, space, backspace
                    if (echo_mode != ECHO_HIDDEN) {
                        printf("\b \b");
                        console_flush_tx();
                    }
                }
            }
            // Handle Ctrl+C (cancel input)
            else if (ch == 3) {
                printf(" ^C\n");
                console_flush_tx();
                buffer[0] = '\0';
                return 0;
            }
            // Handle Ctrl+U (clear line)
            else if (ch == 21) {
                // Erase all characters on the line
                while (pos > 0) {
                    pos--;
                    if (echo_mode != ECHO_HIDDEN) {
                        printf("\b \b");
                    }
                }
                buffer[0] = '\0';
                console_flush_tx();
            }
            // Handle Escape key (cancel input)
            else if (ch == 27) {
                printf(" [ESC]\n");
                console_flush_tx();
                buffer[0] = '\0';
                return 0;
            }
            // Handle printable characters
            else if (ch >= 32 && ch < 127) {
                // Only add if we have room (leave space for null terminator)
                if (pos < EHS_PROMPT_READ_MAX - 1) {
                    buffer[pos++] = ch;
                    buffer[pos] = '\0';
                    // Echo based on mode
                    switch (echo_mode) {
                        case ECHO_NORMAL:
                            printf("%c", ch);
                            break;
                        case ECHO_PASSWORD:
                            printf("*");
                            break;
                        case ECHO_HIDDEN:
                            // No echo
                            break;
                    }
                    console_flush_tx();
                } else {
                    // Buffer full - beep (bell character)
                    printf("\a");
                    console_flush_tx();
                }
            }
            // Ignore other control characters
        }
        // Small delay to avoid CPU overload when polling
        vTaskDelay(pdMS_TO_TICKS(EHS_PROMPT_CHAR_POLL_MS));
    }
}

/**
 * Read a line of input with normal echo (convenience wrapper)
 *
 * @param buffer Buffer to store the input (must be at least EHS_PROMPT_READ_MAX bytes)
 * @return Length of the input string (excluding null terminator)
 */
ehs_uint32 command_prompt_read(char* buffer)
{
    return command_prompt_read_with_echo(buffer, ECHO_NORMAL);
}

/**
 * Read a password with masked echo (shows asterisks)
 *
 * @param buffer Buffer to store the password (must be at least EHS_PROMPT_READ_MAX bytes)
 * @return Length of the password string (excluding null terminator)
 */
ehs_uint32 command_prompt_read_password(char* buffer)
{
    return command_prompt_read_with_echo(buffer, ECHO_PASSWORD);
}

ehs_bool command_prompt_ask_yes_no(const char* question)
{
    char ch = 0;

    command_prompt_println(question);

    while (1) {
        // Read a character
        if (fscanf(stdin, "%c", &ch) == 1) {
            // Skip whitespace (newlines, spaces, etc)
            if (ch == '\n' || ch == '\r' || ch == ' ' || ch == '\t') {
                continue;
            }

            // Check for valid y/n response
            if (ch == 'y' || ch == 'Y') {
                printf("%c\n", ch);
                console_flush_tx();
                return EHS_TRUE;
            }
            else if (ch == 'n' || ch == 'N') {
                printf("%c\n", ch);
                console_flush_tx();
                return EHS_FALSE;
            }
            else {
                // Invalid input - echo it and ask again
                printf("%c - please enter 'y' or 'n': ", ch);
                console_flush_tx();
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    return EHS_FALSE;
}

#ifdef EHS_NETWORK_WIFI_SUPPORT
void command_prompt_wifi_conf()
{
    if (isEhsWiFiManagedByComponent() == EHS_TRUE)
    {
        command_prompt_println("WiFi is managed by function block. Please configure WiFi there.");
        return;
    }
    char ssid[EHS_PROMPT_READ_MAX] = {0};
    char pass[EHS_PROMPT_READ_MAX] = {0};
    ehs_bool yes = EHS_FALSE;
    ehs_uint32 ssid_len, pass_len;

    command_prompt_println("**** WiFi config ****");
    command_prompt_println("Enter SSID:");
    ssid_len = command_prompt_read(ssid);
    if (ssid_len == 0) {
        command_prompt_println("Cancelled.");
        return;
    }

    command_prompt_println("Enter Password:");
    pass_len = command_prompt_read_password(pass);
    if (pass_len == 0) {
        command_prompt_println("Cancelled.");
        return;
    }

    // Show confirmation (password masked)
    printf("SSID: %s\n", ssid);
    printf("Password: ");
    for (ehs_uint32 i = 0; i < pass_len; i++) {
        printf("*");
    }
    printf(" (%d chars)\n", (int)pass_len);
    console_flush_tx();

    yes = command_prompt_ask_yes_no("Are these correct? (y/n)");
    if(yes==EHS_TRUE){
        command_prompt_println("Saving WiFi credentials...");
        EhsWifiStationSaveSettings(ssid, pass);
        command_prompt_println("Connecting to WiFi, please wait...");
        //setWifiStationConnectState(WifiStationConnectState_INIT);
        WifiStationSetSSIDPSK(ssid, EhsStrlen(ssid), pass, EhsStrlen(pass));
    }else{
        command_prompt_println("WiFi NOT configured. Type 'w' to try again.");
    }
}

void command_prompt_wifi_reconnect()
{
    if (isEhsWiFiManagedByComponent() == EHS_TRUE)
    {
        command_prompt_println("WiFi is managed by function block. Please reconnect WiFi there.");
        return;
    }
    command_prompt_println("wifi re-connect");
	setWifiStationConnectState(WifiStationConnectState_CONNECT);
    EhsWifiStationSetCBSource(eWifiStationCallbackSource_Connect);
    EhsStartWifiStationThread();
}

void command_prompt_wifi_ssid()
{
    if (isEhsWiFiManagedByComponent() == EHS_TRUE)
    {
        printf("WiFi is managed by function block. Please get SSID there. This device is %s.\n", (isWifiStationConnected()) ? "connected" : "not connected");
        return;
    }
    char ssid[EHS_PROMPT_READ_MAX] = {0};
    char pass[EHS_PROMPT_READ_MAX] = {0};
    WifiStationGetCurrentSsid(ssid);
    ehs_bool connected = isWifiStationConnected();
    printf("SSID: %s  (%s)\n", ssid, (connected) ? "connected" : "not connected");
}

void command_prompt_wifi_disconnect()
{
    if (isEhsWiFiManagedByComponent() == EHS_TRUE)
    {
        command_prompt_println("WiFi is managed by function block. Please disconnect WiFi there.");
        return;
    }
    if (isWifiStationConnected() == EHS_FALSE)
    {
        command_prompt_println("WiFi is not connected.");
        return;
    }
    command_prompt_println("Disconnecting from WiFi...");
    doWifiStationDisconnect();
    command_prompt_println("WiFi disconnected.");
}

void command_prompt_wifi_forget()
{
    if (isEhsWiFiManagedByComponent() == EHS_TRUE)
    {
        command_prompt_println("WiFi is managed by function block. Please manage credentials there.");
        return;
    }
    ehs_bool yes = command_prompt_ask_yes_no("Are you sure you want to forget saved WiFi credentials? (y/n)");
    if (yes == EHS_TRUE)
    {
        // Disconnect if currently connected
        if (isWifiStationConnected() == EHS_TRUE)
        {
            command_prompt_println("Disconnecting from WiFi...");
            doWifiStationDisconnect();
        }
        // Clear saved credentials by saving empty strings
        EhsWifiStationSaveSettings("", "");
        command_prompt_println("WiFi credentials forgotten.");
    }
    else
    {
        command_prompt_println("Credentials NOT forgotten.");
    }
}
#endif // #ifdef EHS_NETWORK_WIFI_SUPPORT

void command_prompt_ip_addr()
{
    const ehs_char* ip = NULL;
    #ifdef EHS_NETWORK_WIFI_SUPPORT
    if (EhsNetworkInterfaceWifiIsEnabled) {ip = isWifiStationConnected() ? WifiStationIpAddress() : NULL; printf("WiFi "); }
    #endif//#ifdef EHS_NETWORK_WIFI_SUPPORT
    #ifdef EHS_NETWORK_ETHERNET_SUPPORT
    if (EhsNetworkInterfaceEthIsEnabled && ip != NULL) {ip = gNetworkConnected ? EhsHMetaGetIPAddr() : NULL; printf("Ethernet "); }
    #endif//#ifdef EHS_NETWORK_ETHERNET_SUPPORT
    printf("IP: %s\n", (ip && EhsStrlen(ip) > 0) ? ip : "N/A");
}

#ifdef EHS_NETWORK_WIFI_SUPPORT
static bool g_cmd_list_ssid_bssid = false;
void command_prompt_list_ssid_bssid()
{
    if (isWifiStationInitalised() == EHS_FALSE)
    {
        command_prompt_println("WiFi is not initalised yet. You can connect to a dummy SSID first.");
        return;
    }
    if (isWifiStationConnecting() == EHS_TRUE)
    {
        command_prompt_println("Wi-Fi Connection in process. Please try again later.");
        return;
    }
    if (g_cmd_list_ssid_bssid)
    {
        printf("List SSID in process...\n");
        return;
    }
    g_cmd_list_ssid_bssid = true;
    ehs_char ssid[33] = {0};
    ehs_char bssid[6] = {0};
    ehs_sint32 channel = 0, rssi = 0;
    ehs_uint32 index = 0;
    vTaskDelay(pdMS_TO_TICKS(EHS_PROMPT_READ_SLEEP));
    int _rssi = 0;
    if (esp_wifi_sta_get_rssi(&_rssi) == ESP_OK) printf("Current connected AP RSSI: %d dBm\n", _rssi);
    //while(WifiStationScanResult(index, ssid, 33, bssid, 6, &channel, &rssi) == EHS_TRUE){
    //    printf("SSID=%s, BSSID(MAC)=%02x:%02x:%02x:%02x:%02x:%02x, Channel=%d, RSSI=%d dBm\n",
    //            ssid, bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5], channel, rssi);
    //    index++;
    //    vTaskDelay(pdMS_TO_TICKS(EHS_PROMPT_READ_SLEEP));
    //}
    //if(index == 0){
    //    printf("No SSID found!\n");
    //}
    if (doWifiStationFullScan(EHS_TRUE) == EHS_TRUE)
    {
        printf("Scanning...\n");
    }
    else {
        printf("Already scanning, please wait...\n");
    }
    g_cmd_list_ssid_bssid = false;
}

void command_prompt_wifi_stop_scan()
{
    if (isWifiStationScanning() == EHS_TRUE)
    {
        if (doWifiStationScanStop() == EHS_TRUE)
        {
            command_prompt_println("WiFi scan stopped.");
        }
        else
        {
            command_prompt_println("Failed to stop WiFi scan.");
        }
    }
    else
    {
        command_prompt_println("No WiFi scan in progress.");
    }
}
#endif // #ifdef EHS_NETWORK_WIFI_SUPPORT

void command_prompt_reboot()
{
    command_prompt_println("rebooting...");
    EhsTargetReboot();
}

void command_prompt_help()
{
    if (gEhsNetworkInterfaceWifiEnable == EHS_TRUE)
    {
#ifdef EHS_NETWORK_WIFI_SUPPORT
        if (isEhsWiFiManagedByComponent() == EHS_TRUE)
        {
             command_prompt_println("Commands:\n's' - get WiFi SSID\n'i' - get IP Address\n'l' - list SSIDs\n'x' - stop WiFi scan\n'r' - reboot device\n'h' - help\n(WiFi is managed by function block)");
        }
        else command_prompt_println("Commands:\n'w' - WiFi config\n'c' - reconnect WiFi\n'd' - disconnect WiFi\n'f' - forget credentials\n's' - get WiFi SSID\n'i' - get IP Address\n'l' - list SSIDs\n'x' - stop WiFi scan\n'r' - reboot device\n'h' - help");
#else
        command_prompt_println("Commands:\n'i' - get IP Address\n'r' - reboot device\n'h' - help\n(WiFi Disabled)");
#endif
    }
    else command_prompt_println("Commands:\n'i' - get IP Address\n'r' - reboot device\n'h' - help");
}

/**
 * Display the command prompt
 */
void command_prompt_show_prompt(void)
{
    printf("> ");
    console_flush_tx();
}

/**
 * Echo a command character and print newline
 * Shows the user what command was received
 */
void command_prompt_echo_command(char cmd)
{
    // Echo printable characters with newline
    if (cmd >= 32 && cmd < 127) {
        printf("%c\n", cmd);
    } else if (cmd == '\n' || cmd == '\r') {
        // Just echo newline for enter key
        printf("\n");
    }
    // Don't echo other control characters
    console_flush_tx();
}

void command_prompt_task(void* params) {

    ehs_threadname_t threadname = EHSTHREADNAME_EHS_CONSOLE_THR;

    // Disable stdio buffering so single-character writes (echo) reach the UART immediately
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);

    if (gEhsNetworkInterfaceWifiEnable == EHS_TRUE) command_prompt_println("Type 'w' to configure WiFi or 'h' for help.");
    else command_prompt_println("Type 'h' for help.");
    char command;
    ehs_bool show_prompt = EHS_TRUE;

    while (1) {

        EhsHStatisticsLoopStart(threadname);

        // Show prompt when ready for input
        if (show_prompt) {
            //command_prompt_show_prompt();
            show_prompt = EHS_FALSE;
        }

        if (fscanf(stdin, "%c", &command) == 1) {
            // Skip whitespace characters (newline, carriage return, space, tab)
            if (command == '\n' || command == '\r' || command == ' ' || command == '\t') {
                // Just show a new prompt on enter
                if (command == '\n' || command == '\r') {
                    printf("\n");
                    show_prompt = EHS_TRUE;
                }
                EhsHStatisticsLoopEnd(threadname);
                vTaskDelay(pdMS_TO_TICKS(10));
                continue;
            }

            // Echo the command character
            command_prompt_echo_command(command);
            show_prompt = EHS_TRUE;  // Show prompt after command completes

            if(command == 'h'){
                command_prompt_help();
            }
            #if EHS_NETWORK_WIFI_SUPPORT
            else if (command == 'w') {
                if (gEhsNetworkInterfaceWifiEnable && isEhsWiFiManagedByComponent() == EHS_FALSE) command_prompt_wifi_conf();
            }
            else if(command == 'c'){
                if (gEhsNetworkInterfaceWifiEnable && isEhsWiFiManagedByComponent() == EHS_FALSE) command_prompt_wifi_reconnect();
            }
            else if(command == 'd'){
                if (gEhsNetworkInterfaceWifiEnable && isEhsWiFiManagedByComponent() == EHS_FALSE) command_prompt_wifi_disconnect();
            }
            else if(command == 'f'){
                if (gEhsNetworkInterfaceWifiEnable && isEhsWiFiManagedByComponent() == EHS_FALSE) command_prompt_wifi_forget();
            }
            else if(command == 's'){
                if (gEhsNetworkInterfaceWifiEnable) command_prompt_wifi_ssid();
            }
            else if(command == 'l'){
                if (gEhsNetworkInterfaceWifiEnable) command_prompt_list_ssid_bssid();
            }
            else if(command == 'x'){
                if (gEhsNetworkInterfaceWifiEnable) command_prompt_wifi_stop_scan();
            }
            #endif // #if EHS_NETWORK_WIFI_SUPPORT
            else if(command == 'r'){
                command_prompt_reboot();
            }
            else if(command == 'i'){
                command_prompt_ip_addr();
            }
            else if(command == '?'){
                // Alternative help command
                command_prompt_help();
            }
            else {
                // Echo unknown command feedback
                printf("Unknown command '%c'. Type 'h' for help.\n", command);
            }
        }
        EhsHStatisticsLoopEnd(threadname);
        vTaskDelay(pdMS_TO_TICKS(50)); // Reduced delay for more responsive input
    }
    command_prompt_println("quit command prompt");
    vTaskDelete(NULL);
}

#endif // EHS_SERIAL_CONSOLE_SUPPORT


// TODO this should be in the target_ethernet.c file

#ifdef EHS_NETWORK_ETHERNET_SUPPORT
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

static esp_eth_handle_t eth_handle = NULL;
static esp_netif_t *eth_netif = NULL;

/* Initi8alise an Ethernet MAC (if one is fitted)
   Currently this is hardwired to call initialisation of only W5500 ethernet MAC via a specific SPI line)
*/

static esp_err_t eth_init()
{
    esp_err_t ret = ESP_OK;
    ESP_ERROR_CHECK_WITHOUT_ABORT(target_eth_init(&eth_handle));

    // Initialise TCP/IP network interface
    if (sfNetifStatusGet() != EHS_TRUE)
    {
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_netif_init());
        sfNetifStatusSet(EHS_TRUE);
    }

    // Create defaultevent loop running in the background
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_loop_create_default());

    // Create instance fo eps-netif for Ethernet
    esp_netif_config_t cfg = ESP_NETIF_DEFAULT_ETH();
    eth_netif = esp_netif_new(&cfg);

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

static esp_err_t eth_deinit()
{
    esp_err_t ret = ESP_OK;
    if(eth_handle){
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_eth_stop(eth_handle));
        printf("Unregistering Ethernet event handlers...\n");
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_handler_unregister(IP_EVENT, IP_EVENT_ETH_GOT_IP, &got_ip_event_handler));
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_handler_unregister(ETH_EVENT, ESP_EVENT_ANY_ID, &eth_event_handler));
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_loop_delete_default());
        if (eth_netif != NULL)
        {
            esp_netif_destroy(eth_netif);
            eth_netif = NULL;
        }
        ESP_ERROR_CHECK_WITHOUT_ABORT(target_eth_deinit(eth_handle));
        eth_handle = NULL;
    }
    return ret;
}
#endif // EHS_NETWORK_ETHERNET_SUPPORT 

#ifdef EHS_NETWORK_WIFI_SUPPORT

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
    eWifiStationStatus status = doWifiStationStart(
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

#endif // #ifdef EHS_NETWORK_WIFI_SUPPORT

/* 
 Loads the TCPIP configuration file (from user file systemand applies to the currently enabled interface 
*/
void EhsLoadNetworkConfig()
{
    EhsConfig* config = EhsConfigLoad(EHS_NET_CONFIG_FILE);
    if(config){
        //ESP_LOGI(TAG, "Loading network settings from config (" EHS_NET_CONFIG_FILE ")");
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


/* todo this should really go in a targetnetworking file?*/
// Override functions for esp32 network config
#ifdef EHS_HAL_NETWORK_CONFIG_SUPPORT
#if EHS_HAL_NETWORK_CONFIG_SUPPORT != EHS_HAL_NETWORK_CONFIG_STUBBED
//#ifdef EHS_HAL_NETWORK_CONFIG_SUPPORT

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
        EhsSaveNetworkConfig(config);
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
                if (eth_init() == ESP_OK)
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

void MCU_SLOW_HP_THR(void *pvp)
{
    //
    ehs_threadname_t threadname = EHSTHREADNAME_MCU_SLOW_HP_THR;
    for (;;)
    {
        EhsHStatisticsLoopStart(threadname);
#ifdef EHS_MODBUS_SUPPORT
        EhsHMBTaskInLoop( EHS_TRUE );
#endif
        vTaskDelay( pdMS_TO_TICKS( 10 ) ); //todo2025 1 tick delay seems absurdly fast? What is actually needed? This needs to be changed to a specific amount of time not ticks. 
                       // Consider using the EhsSleep function which should map to an appropriate delay method (i.e. vDelay with a real)
        EhsHStatisticsLoopEnd(threadname);
    }
}

void MCU_SLOW_LP_THR(void *pvParameters)
{
    // Thread Name
    ehs_threadname_t threadname = EHSTHREADNAME_MCU_SLOW_LP_THR;
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
#ifdef EHS_NETWORK_WIFI_SUPPORT
    // wifi setup init
    ehs_char wifi_ssid[EHS_WIFI_SSID_BUFF_MAX] = { 0 };
    ehs_char wifi_pass[EHS_WIFI_SSID_BUFF_MAX] = { 0 };
    if (gEhsNetworkInterfaceWifiEnable == EHS_TRUE && isEhsWiFiManagedByComponent() == EHS_FALSE)
        setWifiStationConnectState(WifiStationConnectState_CONNECT);
#endif
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
                EhsLoadNetworkConfig();
                bNetworkSettingsLoaded = EHS_TRUE;
            }
#endif

#ifdef EHS_MQTT_SUPPORT
            // execute esp mqtt clinet loop
            ehs_bool _mqttloop_already_run = EHS_FALSE;
#ifdef EHS_NETWORK_WIFI_SUPPORT
            if (gEhsNetworkInterfaceWifiEnable == EHS_TRUE)
            {
                if(isWifiStationConnected()){
                    EhsMqttClientLoop( (void*)EhsMqttDevmanMonSupport() );
                    _mqttloop_already_run = EHS_TRUE;
                }
            }
#endif
#ifdef EHS_NETWORK_ETHERNET_SUPPORT
            if (gEhsNetworkInterfaceEthEnable && _mqttloop_already_run == EHS_FALSE)
                EhsMqttClientLoop( (void*)EhsMqttDevmanMonSupport() );
#endif//EHS_NETWORK_ETHERNET_SUPPORT

#endif // EHS_MQTT_SUPPORT

#if EHS_OTA_SUPPORT == EHS_OTA_SUPPORT_SUPPORT
            ehs_bool _ota_loop_already_run = EHS_FALSE;
#ifdef EHS_NETWORK_WIFI_SUPPORT
            if(isWifiStationConnected()){
                // execute OTA loop
                target_OTA_task(NULL);
                _ota_loop_already_run = EHS_TRUE;
            }
#endif //#ifdef EHS_NETWORK_WIFI_SUPPORT
#ifdef EHS_NETWORK_ETHERNET_SUPPORT
            // execute OTA loop
            if (_ota_loop_already_run == EHS_FALSE) target_OTA_task(NULL);
#endif//#ifdef EHS_NETWORK_ETHERNET_SUPPORT
#endif //EHS_OTA_SUPPORT

            // @TODO - This is used by Uart function block - needs to review and potentially moved or Wifi connect needs to be done non-blockig (prefered)
#ifdef EHS_SERIAL_CONSOLE_SUPPORT
            console_flush_tx();
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
void app_main(void)
{
#if defined(EHS_TEST_FUNC_OVERRIDE) && defined(EHS_TEST_FUNC_NO_ERT_INIT)
    // Bare metal mode: Run test immediately and hang
    extern void EHS_TEST_FUNC_NAME(void);
    ESP_LOGI(TAG, "EHS Bare Metal Test: Running %s", #EHS_TEST_FUNC_NAME);
    EHS_TEST_FUNC_NAME();
    ESP_LOGI(TAG, "Test completed");
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
#endif

    esp_task_wdt_deinit();
#if EHS_ESP32_ENABLE_LOGS != 1
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

#ifdef EHS_HAL_INTERFACE_CONFIG_SUPPORT
#if EHS_HAL_INTERFACE_CONFIG_SUPPORT != EHS_HAL_INTERFACE_CONFIG_STUBBED
    EhsLoadNetworkInterfaceConfig();
#endif
#endif
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
        printf("Wi-Fi host name : %s\n", gWiFiHostNameBuffer);
        if (startup == EHS_TRUE){
            if (doWifiStationNetifInit(gWiFiHostNameBuffer) == EHS_TRUE) EhsStartWifiStationThread();
            else ESP_LOGE(TAG, "Failed to initalise wifi netif");
        }
#endif
    }
    if (gEhsNetworkInterfaceEthEnable)
    {
        
#ifdef EHS_NETWORK_ETHERNET_SUPPORT 
        if (eth_init() == ESP_OK)
        {
            ESP_LOGI(TAG, "Connection success");
        }
        else
        {
            ESP_LOGE(TAG, "Connection failed");
        }
#endif
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
    // Test mode with full init: Run test instead of EhsMain
    extern void EHS_TEST_FUNC_NAME(void);
    ESP_LOGI(TAG, "EHS Test Mode: Running %s", #EHS_TEST_FUNC_NAME);
    xTaskCreate(EHS_TEST_FUNC_NAME, #EHS_TEST_FUNC_NAME, EHS_MAIN_ESP32_TASK_STACK_SIZE, NULL, EHS_PRI_EHS_MAIN, NULL);
#else
    // Normal production mode
    xTaskCreate(EhsMain, "EhsMain", EHS_MAIN_ESP32_TASK_STACK_SIZE, NULL, EHS_PRI_EHS_MAIN, NULL/* see above should be xHandle*/); // tskIDLE_PRIORITY + 5
#endif

 #endif
 
  #ifdef EHS_NETWORK_BLE_SUPPORT
  inx_ble_char_config_t qwepoi[2] = {
        {
            "0x4321",
            "char1",
            1,
            50
        },
        {
            "0x3241",
            "char2",
            2,
            30
        }
    };
    inx_ble_service_hal_init("0x1234",
    "BT",
    2,
    100,
    23,
    qwepoi,//(inx_ble_char_config_t*) NULL,
    (inx_ble_service_callbacks_t*) NULL,
    (void*) NULL);
    
    inx_ble_service_hal_register_gatt();

    inx_ble_service_hal_start_adv();    	
 #endif

 #ifdef EHS_SERIAL_CONSOLE_SUPPORT
    // TODO - shell we use this in MCU_SLOW_LP_THR ?
    // create a command prompt task for interacting with the device over a console
    xTaskCreate(command_prompt_task, "CommandPrompt", 4096, NULL, EHS_PRI_SERIAL_CMD, NULL);
#endif
    xTaskCreate(MCU_SLOW_HP_THR, "MCU_SLOW_HP_THR", EHS_MCU_SLOW_HP_THR_STACKSIZE, NULL, EHS_PRI_MCU_SLOW_HP_THR, NULL);
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
