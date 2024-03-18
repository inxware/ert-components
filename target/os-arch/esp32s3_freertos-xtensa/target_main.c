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
#endif
#if TARGET_USE_ETHERNET == 1
#include "esp_wifi.h"
#include "esp_eth.h"
#include "target_ethernet.h"
#endif
#include "freertos/event_groups.h"
#include "freertos/task.h"
//#include "mdns.h" // - not used atm
#include "nvs_flash.h"
#include "nvs.h"
#include <string.h>

#include "target_uart.h"
#include "target_ota.h"

#include "target_data_bin.h"

#ifndef EHS_MAIN_ESP32_TASK_STACK_SIZE
#define EHS_MAIN_ESP32_TASK_STACK_SIZE 20000
#endif

#define CONFIG_ESP_MAXIMUM_RETRY 5
#define CONFIG_ESP_WIFI_SSID "dlink_DWR-920_003F"
#define CONFIG_ESP_WIFI_PASSWORD "VvRCB27795"
// #define CONFIG_ESP_WIFI_SSID "dlink_DWR-920_003F"
// #define CONFIG_ESP_WIFI_PASSWORD "VvRCB27795"
/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

static esp_event_handler_instance_t instance_any_id;
static esp_event_handler_instance_t instance_got_ip;

/* The event group allows multiple bits for each event, but we only care about
 * two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

/* target_host is www.espressif.com */
// char *TARGET_HOST = "www.espressif.com";
/* target_host is own gateway */
char *TARGET_HOST = "www.example.com";

extern const char* base_path; // ert install path, defined in target_file

static int s_retry_num = 0;

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
#define TARGET_HOSTNAME "INX esp32s3"
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

static void app_load_status_handler(ehs_uint32 status)
{
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
            break;
        }
        case EHS_APP_LOAD_FAILED : {
            ESP_LOGE(TAG, "******* App loading failed *******");
            break;
        }
        default:
            ESP_LOGW(TAG, "Unknow app loading status!");
            break;
    }
}

#if TARGET_USE_WIFI
static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT &&
             event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (s_retry_num < CONFIG_ESP_MAXIMUM_RETRY)
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        }
        else
        {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG, "connect to the AP fail");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

UBaseType_t uxHighWaterMark;

esp_err_t wifi_init_sta()
{
    esp_err_t ret_value = ESP_OK;
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *netif = esp_netif_create_default_wifi_sta();

    esp_netif_set_hostname(netif, TARGET_HOSTNAME);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                               &event_handler, NULL, &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP,
                                               &event_handler, NULL, &instance_got_ip));

    wifi_config_t wifi_config =
        {
            .sta = {
                .ssid = CONFIG_ESP_WIFI_SSID,
                .password = CONFIG_ESP_WIFI_PASSWORD},
        };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or
     * connection failed for the maximum number of re-tries (WIFI_FAIL_BIT). The
     * bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE, // xClearOnExit
                                           pdFALSE, // xWaitForAllBits
                                           portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we
     * can test which event actually happened. */
    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s", CONFIG_ESP_WIFI_SSID,
                 CONFIG_ESP_WIFI_PASSWORD);
        
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip));
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id));
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_loop_delete_default());
    }
    else if (bits & WIFI_FAIL_BIT)
    {
        ESP_LOGE(TAG, "Failed to connect to SSID:%s, password:%s",
                 CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD);
        ret_value = ESP_FAIL;
    }
    else
    {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
        ret_value = ESP_ERR_INVALID_STATE;
    }

    ESP_LOGI(TAG, "wifi_init_sta finished.");
    ESP_LOGI(TAG, "connect to ap SSID:%s", CONFIG_ESP_WIFI_SSID);
    vEventGroupDelete(s_wifi_event_group);
    sfWifiStationNetifSet(EHS_TRUE);
    return ret_value;
}
#endif // TARGET_USE_WIFI
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
        break;
    case ETHERNET_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "Ethernet Link Down");
        break;
    case ETHERNET_EVENT_START:
        ESP_LOGI(TAG, "Ethernet Started");
        break;
    case ETHERNET_EVENT_STOP:
        ESP_LOGI(TAG, "Ethernet Stopped");
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

    // Set the hostname
    esp_netif_set_hostname(eth_netif, TARGET_HOSTNAME);

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

// #define TEST_TCP_SOCKET
#ifdef TEST_TCP_SOCKET
// #include "protocol_examples_common.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include "esp_netif.h"

#define PORT 11425
#define KEEPALIVE_IDLE 5
#define KEEPALIVE_INTERVAL 5
#define KEEPALIVE_COUNT 3

static void do_retransmit(const int sock)
{
    int len;
    char rx_buffer[128];

    do
    {
        len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
        if (len < 0)
        {
            ESP_LOGE(TAG, "Error occurred during receiving: errno %d", errno);
        }
        else if (len == 0)
        {
            ESP_LOGW(TAG, "Connection closed");
        }
        else
        {
            rx_buffer[len] = 0; // Null-terminate whatever is received and treat it like a string
            ESP_LOGI(TAG, "Received %d bytes: %s", len, rx_buffer);

            // send() can return less bytes than supplied length.
            // Walk-around for robust implementation.
            int to_write = len;
            while (to_write > 0)
            {
                int written = send(sock, rx_buffer + (len - to_write), to_write, 0);
                if (written < 0)
                {
                    ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                }
                to_write -= written;
            }
        }
    } while (len > 0);
}

static void tcp_server_task(void *pvParameters)
{
    char addr_str[128];
    int addr_family = (int)pvParameters;
    int ip_protocol = 0;
    int keepAlive = 1;
    int keepIdle = KEEPALIVE_IDLE;
    int keepInterval = KEEPALIVE_INTERVAL;
    int keepCount = KEEPALIVE_COUNT;
    struct sockaddr_storage dest_addr;

    if (addr_family == AF_INET)
    {
        struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
        dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
        dest_addr_ip4->sin_family = AF_INET;
        dest_addr_ip4->sin_port = htons(PORT);
        ip_protocol = IPPROTO_IP;
    }
#ifdef CONFIG_LWIP_IPV6
    else if (addr_family == AF_INET6)
    {
        struct sockaddr_in6 *dest_addr_ip6 = (struct sockaddr_in6 *)&dest_addr;
        bzero(&dest_addr_ip6->sin6_addr.un, sizeof(dest_addr_ip6->sin6_addr.un));
        dest_addr_ip6->sin6_family = AF_INET6;
        dest_addr_ip6->sin6_port = htons(PORT);
        ip_protocol = IPPROTO_IPV6;
    }
#endif

    int listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (listen_sock < 0)
    {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        vTaskDelete(NULL);
        return;
    }
    int opt = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#if defined(CONFIG_LWIP_IPV4) && defined(CONFIG_LWIP_IPV6)
    // Note that by default IPV6 binds to both protocols, it is must be disabled
    // if both protocols used at the same time (used in CI)
    setsockopt(listen_sock, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt));
#endif

    ESP_LOGI(TAG, "Socket created");

    int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0)
    {
        ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        ESP_LOGE(TAG, "IPPROTO: %d", addr_family);
        goto CLEAN_UP;
    }
    ESP_LOGI(TAG, "Socket bound, port %d", PORT);

    err = listen(listen_sock, 1);
    if (err != 0)
    {
        ESP_LOGE(TAG, "Error occurred during listen: errno %d", errno);
        goto CLEAN_UP;
    }

    while (1)
    {

        ESP_LOGI(TAG, "Socket listening");

        struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
        socklen_t addr_len = sizeof(source_addr);

        int sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        ESP_LOGI(TAG, "Looking for ip address: %s", inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1));
        if (sock < 0)
        {
            ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
            break;
        }

        // Set tcp keepalive option
        setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof(int));
        setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &keepIdle, sizeof(int));
        setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &keepInterval, sizeof(int));
        setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &keepCount, sizeof(int));
        // Convert ip address to string

        if (source_addr.ss_family == PF_INET)
        {
            inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
        }
#ifdef CONFIG_EXAMPLE_IPV6
        else if (source_addr.ss_family == PF_INET6)
        {
            inet6_ntoa_r(((struct sockaddr_in6 *)&source_addr)->sin6_addr, addr_str, sizeof(addr_str) - 1);
        }
#endif
        ESP_LOGI(TAG, "Socket accepted ip address: %s", addr_str);

        do_retransmit(sock);

        shutdown(sock, 0);
        close(sock);
    }

CLEAN_UP:
    close(listen_sock);
    vTaskDelete(NULL);
}
#endif

// #define EHS_ESP32_LED_TEST
#ifdef EHS_ESP32_LED_TEST
#include "driver/gpio.h"
#include "esp_log.h"
// #include "led_strip.h"
#define BLINK_GPIO GPIO_NUM_2

static uint8_t s_led_state = 0;

static void configure_led(void)
{
    gpio_reset_pin(BLINK_GPIO);
    // Set the GPIO as a push/pull output
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}

static void blink_led(void)
{
    /// Set the GPIO level according to the state (LOW or HIGH)
    gpio_set_level(BLINK_GPIO, s_led_state);
}

void app_test_main(void)
{

    // Configure the peripheral according to the LED type
    configure_led();
    for (int i = 0; i < 1000;
         i++) // make this finite loop so code afterwards is compiled
    {
        printf("Hello world! LED\n");
        fflush(stdout);
        blink_led();
        // Toggle the LED state
        s_led_state = !s_led_state;
        // vTaskDelay(1000 / portTICK_PERIOD_MS);
        sleep(1);
    }
}

#endif

#include "mqtt.h"

void mqtt_test_main(void)
{
    int i;
    for (;;)
    {
        //  printf("Hello MQTT!\n");
        mqttMainLoop();

        fflush(stdout);
        for (i = 0; i < UART_COUNT; i++)
            TgtUART_SendInThread(i);
        // todo: change the sleep interval for EHS MQTT responsive rate
        //sleep(1);
        usleep(10000);

       
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
     if (wifi_init_sta() == ESP_OK)
    #elif TARGET_USE_ETHERNET == 1
     if (eth_init() == ESP_OK)
    #endif
     {
         ESP_LOGI(TAG, "Connection success");
     }
     else
     {
         ESP_LOGE(TAG, "Connection failed");
         while (1)
         {
             vTaskDelay(1);
         }
     }

  // #ifdef EHS_ESP32_LED_TEST
  //     app_test_main();
  // #endif
 
 #define EHS_ERT_KERNEL_AVAILABLE
 #ifdef EHS_ERT_KERNEL_AVAILABLE
 
     TaskHandle_t xHandle = NULL;
     uint32_t stack_depth = EHS_MAIN_ESP32_TASK_STACK_SIZE;
 
 #ifdef TEST_TCP_SOCKET
 #ifdef CONFIG_LWIP_IPV4
     xTaskCreate(tcp_server_task, "tcp_server", 4096, (void *)AF_INET, 5, NULL);
 #endif
 #ifdef CONFIG_LWIP_IPV6
     xTaskCreate(tcp_server_task, "tcp_server", 4096, (void *)AF_INET6, 5, NULL);
 #endif
 #endif
     // we need to specify the installation path before starting EhsMain, 
     // so that things like user dir gets set-up properly
     EhsHMetaSetInstPath(base_path);

     xTaskCreate(target_OTA_task, "OTA_Task", 8192, NULL, ESP_TASK_TCPIP_PRIO - 1, NULL);
     xTaskCreate(EhsMain, "EhsMain", stack_depth, NULL, ESP_TASK_TCPIP_PRIO + 1, xHandle); // tskIDLE_PRIORITY + 5
     // xTaskCreate(app_test_main, "app_test_main", stack_depth, ( void * ) 1, 1, NULL);
     // EhsMain(NULL, NULL); /* doesn't return in this version */;
 
 #endif
    // We do mqtt main 
     mqtt_test_main();
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
