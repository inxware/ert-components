#include "target_config.h"
#if TARGET_USE_WIFI == 1

#include "target.h"
#include "wifi_station.h"
#include "target_wifi.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_wpa2.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "esp_err.h"
#include "esp_check.h"

#include <unistd.h>

#define TAG "wifi-station"

#define WIFI_CONNECTED_BIT  BIT0
#define WIFI_FAIL_BIT       BIT1

#define MACIP_LENGTH 20

static ehs_sint32 c_max_retry_num = 1;
static ehs_bool  c_reconnect = EHS_FALSE;
static int s_retry_num = 0;

static EventGroupHandle_t s_wifi_event_group;
static esp_netif_t * sta_netif = NULL;

static esp_event_handler_instance_t instance_any_id;
static esp_event_handler_instance_t instance_got_ip;

static ehs_char *serverCert_content = NULL;
static ehs_char *tlsCert_content = NULL;
static ehs_char *tlsKey_content = NULL;

static ehs_char IP_Address[MACIP_LENGTH] = {0};

static volatile ehs_bool gTargetWifiStationConnected = EHS_FALSE;

static volatile ehs_bool gsNetifInitialised = EHS_FALSE;

/*
 * This function read the file into an allocated buffer and return
 *  whether the file is accessible by the application. The buffer
 *  needs to be freed after usage.
 *
 * @arg: path - path to the file
 * @arg: output - allocated output buffer, which needs to be freed later
 * @return: whether the file is accessible.
 * */
static ehs_bool readValidFile_into_buffer(ehs_char *path, ehs_char *output);

static void event_handler(void* arg, esp_event_base_t event_base,
                              int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) esp_wifi_connect();
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (s_retry_num < c_max_retry_num && c_reconnect == EHS_TRUE)
        {
            s_retry_num++;
            ESP_LOGW(TAG, "retry to connect to the AP");
            esp_wifi_connect();
        }
        else 
        {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
            if (gTargetWifiStationConnected == EHS_TRUE)
            {
                wifi_event_sta_disconnected_t* event = (wifi_event_sta_disconnected_t*) event_data;
                Common_WifiStation_onDisconnected(EHS_TRUE, event->reason, event->rssi);
                gTargetWifiStationConnected = EHS_FALSE;
            }
        }
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED) {
        ESP_LOGI(TAG, "Station Connected");
        if (sfWifiStationNetifGet() == EHS_TRUE) xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Got IP:" IPSTR, IP2STR(&event->ip_info.ip));
        snprintf(IP_Address, MACIP_LENGTH - 1, IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        if (sfWifiStationNetifGet() != EHS_TRUE) xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        gTargetWifiStationConnected = EHS_TRUE;
    }
}

eWifiStationStatus doWifiStationConnect(
    ehs_char    *ssid,
    ehs_uint8   type,
    ehs_char    *PSKPass,
    ehs_uint8   EnterpriseType,
    ehs_uint8   EAP,
    ehs_uint8   TTLS2,
    ehs_bool    needServerCert,
    ehs_char    *serverCert,
    ehs_char    *tlsCert,
    ehs_char    *tlsKey,
    ehs_char    *eapID,
    ehs_char    *eapUser,
    ehs_char    *eapPass,
    ehs_char    *ip_address,
    ehs_char    *mac_address,
    ehs_sint8   *rssi
)
{
    esp_err_t ret = ESP_OK;
    s_wifi_event_group = xEventGroupCreate();
    if (sfWifiStationNetifGet() != EHS_TRUE)
        ESP_GOTO_ON_ERROR(esp_netif_init(), doWifiStationConnect_Return, TAG, "Network Interface init failed");

    ESP_GOTO_ON_ERROR(esp_event_loop_create_default(), doWifiStationConnect_Return, TAG, "Event loop creation failed");
    if (sfWifiStationNetifGet() != EHS_TRUE)
    {
        sta_netif = esp_netif_create_default_wifi_sta();
        assert(sta_netif);
        sfWifiStationNetifSet(EHS_TRUE);
    }

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_GOTO_ON_ERROR(esp_wifi_init(&cfg), doWifiStationConnect_Return, TAG, "Wi-Fi init failed");

    ESP_GOTO_ON_ERROR(esp_event_handler_instance_register(
                    WIFI_EVENT,
                    ESP_EVENT_ANY_ID,
                    &event_handler,
                    NULL,
                    &instance_any_id), doWifiStationConnect_Return, TAG, "Event handler (ANY_ID) registration failed");
    ESP_GOTO_ON_ERROR(esp_event_handler_instance_register(
                    IP_EVENT,
                    IP_EVENT_STA_GOT_IP,
                    &event_handler,
                    NULL,
                    &instance_got_ip), doWifiStationConnect_Return, TAG, "Event handler (GOT_IP) registration failed");

    ESP_GOTO_ON_ERROR(esp_wifi_set_storage(WIFI_STORAGE_RAM), doWifiStationConnect_Return, TAG, "Wi-Fi storage setting failed");
    wifi_config_t wifi_config;
    switch (type) {
        case Type_WifiStation_PSK:
            if (PSKPass[0] == 0) return WifiStation_AuthFailed;
            EhsStrcpy(wifi_config.sta.ssid, ssid); wifi_config.sta.ssid[strlen(ssid)] = '\0';
            EhsStrcpy(wifi_config.sta.password, PSKPass); wifi_config.sta.password[strlen(PSKPass)] = '\0';
            wifi_config.sta.pmf_cfg.required = false;
            ESP_LOGW(TAG, "SSID: [%s], password: [%s]", ssid, PSKPass);
            break;
        case Type_WifiStation_Open:
            EhsStrcpy(wifi_config.sta.ssid, ssid);
            wifi_config.sta.threshold.authmode = WIFI_AUTH_OPEN;
            wifi_config.sta.pmf_cfg.required = false;
            break;
        case Type_WifiStation_Enterprise:
            switch (EnterpriseType) {
                case Enterprise_WifiStation_WPA2:
                case Enterprise_WifiStation_WPA3:
                    EhsStrcpy(wifi_config.sta.ssid, ssid);
                    break;
                case Enterprise_WifiStation_WPA3_192Bit:
                    EhsStrcpy(wifi_config.sta.ssid, ssid);
                    wifi_config.sta.pmf_cfg.required = true;
                    break;
                default:
                    doWifiStationDestroy();
                    return WifiStation_InvalidArg;
            }
            break;
        default:
            doWifiStationDestroy();
            return WifiStation_InvalidArg;
    }
    ESP_GOTO_ON_ERROR(esp_wifi_set_mode(WIFI_MODE_STA), doWifiStationConnect_Return, TAG, "Wi-Fi Mode settnig failed");
    ESP_GOTO_ON_ERROR(esp_wifi_set_config(WIFI_IF_STA, &wifi_config),doWifiStationConnect_Return, TAG, "Wi-Fi configure failure");

    if (type == Type_WifiStation_Enterprise)
    {
        ESP_GOTO_ON_ERROR( esp_wifi_sta_wpa2_ent_set_identity( (uint8_t *) eapID, strlen(eapID) ),\
                          doWifiStationConnect_Return, TAG, "Enterprise ID configure failure" );
        if (needServerCert == EHS_TRUE || EnterpriseType == Enterprise_WifiStation_WPA3 || EnterpriseType == Enterprise_WifiStation_WPA3_192Bit)
        {
            // check file existance and read out content into buffer
            if (readValidFile_into_buffer(serverCert, serverCert_content) == EHS_FALSE)
            {
                doWifiStationDestroy();
                return WifiStation_FileNotFound;
            }
            ESP_GOTO_ON_ERROR( esp_wifi_sta_wpa2_ent_set_ca_cert( serverCert_content, strlen(serverCert_content) ),\
                              doWifiStationConnect_Return, TAG, "Enterprise Server cert configure failed");
        }
        switch (EAP)
        {
            case EAP_WifiStation_TLS:
                if (readValidFile_into_buffer(tlsCert, tlsCert_content) == EHS_FALSE ||
                    readValidFile_into_buffer(tlsKey, tlsKey_content) == EHS_FALSE)
                {
                    doWifiStationDestroy();
                    return WifiStation_FileNotFound;
                }
                ESP_GOTO_ON_ERROR( esp_wifi_sta_wpa2_ent_set_cert_key( tlsCert_content, strlen(tlsCert_content),\
                                                                   tlsKey_content, strlen(tlsKey_content), NULL, 0 ),\
                                  doWifiStationConnect_Return, TAG, "Enterprise TLS configure failed");
                break;
            
            case EAP_WifiStation_TTLS:
                if (TTLS2 <= TTLS2_WifiStation_MIN || TTLS2 >= TTLS2_WifiStation_MAX)
                {
                    doWifiStationDestroy();
                    return WifiStation_InvalidArg;
                }
                ESP_GOTO_ON_ERROR( esp_wifi_sta_wpa2_ent_set_ttls_phase2_method(TTLS2), doWifiStationConnect_Return, TAG, "TTLS Phase 2 configure failed" );
                // No break here because it shares the same following two commands in PEAP method.

            case EAP_WifiStation_PEAP:
                ESP_GOTO_ON_ERROR( esp_wifi_sta_wpa2_ent_set_username( (uint8_t *) eapUser, strlen(eapUser) ), doWifiStationConnect_Return, TAG, "Enterprise Username configure failed" );
                ESP_GOTO_ON_ERROR( esp_wifi_sta_wpa2_ent_set_password( (uint8_t *) eapPass, strlen(eapPass) ), doWifiStationConnect_Return, TAG, "Enterprise Password configure failed" );
                break;

            default:
                doWifiStationDestroy();
                return WifiStation_InvalidArg;
        }

        if (EnterpriseType == Enterprise_WifiStation_WPA3_192Bit)
            ESP_GOTO_ON_ERROR( esp_wifi_sta_wpa2_set_suiteb_192bit_certification(true), doWifiStationConnect_Return, TAG, "Enterprise 192-bit cert configure failed" );

        ESP_GOTO_ON_ERROR( esp_wifi_sta_wpa2_ent_enable(), doWifiStationConnect_Return, TAG, "Wi-Fi Enterprise enable failure" );
    }

    ESP_GOTO_ON_ERROR( esp_wifi_start(), doWifiStationConnect_Return, TAG, "Wi-Fi Start failed" );
    //ESP_GOTO_ON_ERROR( esp_wifi_connect(), doWifiStationConnect_Return, TAG, "Wi-Fi Connect failed" );

    ESP_LOGW(TAG, "Waiting for connection...");
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           portMAX_DELAY);

    eWifiStationStatus errorCode;
    wifi_ap_record_t ap_info;
    ehs_char mac_output[MACIP_LENGTH] = {0};
    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI(TAG, "Connected");
        errorCode = WifiStation_Connected;
        ESP_GOTO_ON_ERROR( esp_wifi_sta_get_ap_info(&ap_info), doWifiStationConnect_Return, TAG, "Cannot get Wi-FI AP information" );
        snprintf(mac_output, MACIP_LENGTH - 1, "%02x:%02x:%02x:%02x:%02x:%02x",
                 ap_info.bssid[0], ap_info.bssid[1], ap_info.bssid[2], ap_info.bssid[3], ap_info.bssid[4], ap_info.bssid[5]);
        *rssi = (ehs_sint8) ap_info.rssi; 
        ip_address = (ehs_char *) IP_Address;
        mac_address = (ehs_char *) mac_output;
    }
    else if (bits & WIFI_FAIL_BIT)
    {
        errorCode = WifiStation_StartFailed;
    }
    else 
    {
        errorCode = WifiStation_StateError;
    }

doWifiStationConnect_Return:
    //vEventGroupDelete(s_wifi_event_group);

    if (ret != ESP_OK)
    {
        switch (ret) {
            case ESP_ERR_WIFI_STATE:        errorCode = WifiStation_StateError;     break;
            case ESP_ERR_WIFI_SSID:         errorCode = WifiStation_NotFound;       break;
            case ESP_ERR_WIFI_PASSWORD:     errorCode = WifiStation_AuthFailed;     break;
            case ESP_ERR_WIFI_TIMEOUT:      errorCode = WifiStation_Timeout;        break;
            case ESP_ERR_WIFI_NOT_STARTED:  errorCode = WifiStation_NotStarted;     break;
            case ESP_ERR_WIFI_NOT_STOPPED:  errorCode = WifiStation_NotStopped;     break;
            case ESP_ERR_WIFI_NOT_INIT:     errorCode = WifiStation_InitFailed;     break;
            default:                        errorCode = WifiStation_InternalError;  break;
        }
        doWifiStationDestroy();
    }

    return errorCode;
}

void doWifiStationDestroy()
{
    esp_wifi_sta_wpa2_ent_clear_cert_key();
    if (serverCert_content != NULL) { free(serverCert_content); serverCert_content = NULL; }
    if (tlsCert_content != NULL) { free(tlsCert_content); tlsCert_content = NULL; }
    if (tlsKey_content != NULL) { free(tlsKey_content); tlsKey_content = NULL; }
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_disconnect());
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_stop());
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip));
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id));
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_loop_delete_default());
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_deinit());
    sfWifiStationNetifSet(EHS_FALSE);
}

void doWifiStationDisconnect()
{
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_disconnect());
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_stop());
}

static ehs_bool readValidFile_into_buffer(ehs_char *path, ehs_char *output)
{
    if (access(path, F_OK | R_OK) != 0)
        return EHS_FALSE;

    ehs_char *content = NULL;
    size_t file_size;
    FILE *f = fopen(path, "r");
    if (f == NULL)
        return EHS_FALSE;

    fseek(f, 0, SEEK_END);
    file_size = ftell(f);
    rewind(f);
    if ((content = (ehs_char *) calloc(file_size + 1, sizeof(ehs_char))) == NULL)
        return EHS_FALSE;
    fread(content, file_size, sizeof(ehs_char), f);
    fclose(f);
    output = content;

    return EHS_TRUE;
}

void configWifiStationSetReconnect(ehs_bool reconnect, ehs_sint32 retry)
{
    c_reconnect = reconnect;
    c_max_retry_num = retry;
}

void sfWifiStationNetifSet(ehs_bool status)
{
    gsNetifInitialised = status;
}

ehs_bool sfWifiStationNetifGet()
{
    return gsNetifInitialised;
}

#endif // #if TARGET_USE_WIFI == 1