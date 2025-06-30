#include "target_config.h"
#if TARGET_USE_WIFI == 1

#include "hal_string.h"
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
#define ESP32_WIFI_RECONNECT_DELAY_MS 2500
#define ESP32_SCAN_RECONNECT_DELAY_MS 1000

static ehs_sint32 c_max_retry_num = 1;
static ehs_bool  c_reconnect = EHS_FALSE;
static int s_retry_num = 0;

static esp_netif_t * sta_netif = NULL;

static esp_event_handler_instance_t instance_any_id;
static esp_event_handler_instance_t instance_got_ip;

/* used for WiFi scanning */
#ifndef ESP32_MAX_AP_RECORDS
#define ESP32_MAX_AP_RECORDS 20
#endif
// @TODO - output this in the wifi-station function block
static wifi_ap_record_t s_ap_records[ESP32_MAX_AP_RECORDS] = {0};
static const wifi_ap_record_t* s_p_ap_record = NULL;
// @TODO - make these configurable
static bool c_show_hidden = false;
static int c_scan_channel = 0; // set '0' to scan all channels

static ehs_char *serverCert_content = NULL;
static ehs_char *tlsCert_content = NULL;
static ehs_char *tlsKey_content = NULL;

static ehs_char IP_Address[MACIP_LENGTH] = {0};
static ehs_char mac_output[MACIP_LENGTH] = {0};

static volatile ehs_bool gTargetWifiStationConnected = EHS_FALSE;
static volatile ehs_bool gWifiStationInitalised = EHS_FALSE;
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
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START){
        ESP_LOGI(TAG, "Wi-Fi started, scanning for available networks...");
        const char* ssid = NULL;
        wifi_config_t wifi_config = {0};
        esp_err_t err = esp_wifi_get_config(WIFI_IF_STA, &wifi_config);
        if (err == ESP_OK && (wifi_config.sta.ssid != NULL && strlen(wifi_config.sta.ssid) > 0)) {
            ssid = wifi_config.sta.ssid;
            ESP_LOGI(TAG, "Set SSID of intrest (%s)", ssid);
        }else{
            ESP_LOGW(TAG, "SSID of intrest is NOT configured!");
        }
        
        s_p_ap_record = NULL; // reset the currect record
        // make these configurable
        wifi_active_scan_time_t active_scan_time = {
            .min = 100, // Set active scan min timeout to 100ms per channel.   (default=0ms)
            .max = 500  // Set active scan max timeout to 500ms per channel.   (default=300ms)
        };
        wifi_scan_config_t scan_config = {
            .ssid = ssid,
            .bssid = NULL,                          // TODO - make this configurable by FB?
            .channel = c_scan_channel,              // set '0' to scan all channels 
            .show_hidden = c_show_hidden,           // 'false' by default
            // Active Scan  : It sends probe requests to access points (APs) and waits for their responses. This is typically quicker than passive scanning.
            .scan_type = WIFI_SCAN_TYPE_ACTIVE,     // active (default)
            // Passive Scan : It waits for APs to send responses without actively probing them.
            //.scan_type = WIFI_SCAN_TYPE_PASSIVE,  // passive
            .scan_time = {
                .active =  active_scan_time,
                .passive = 1000                     // Set passive scan timeout to 1000ms per channel. (default=250ms)
            }
        };
        // Start scanning for networks
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_scan_start(&scan_config, true));
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_SCAN_DONE){
        wifi_config_t wifi_config = {0};
        wifi_event_sta_scan_done_t* scan_event = (wifi_event_sta_scan_done_t *)event_data;
        bool ssid_found = false;
        uint16_t ap_count = 0;
        esp_err_t err = esp_wifi_get_config(WIFI_IF_STA, &wifi_config);
        if (err == ESP_OK && (scan_event && scan_event->status == 0)) {
            ESP_LOGI(TAG, "Scan done, looking for SSID %s", wifi_config.sta.ssid);
        }else{
            ESP_LOGE(TAG, "Failed to do WiFi scan ...");
        }
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_scan_get_ap_num(&ap_count)); 
        if (ap_count > 0) {
            s_p_ap_record = NULL; // reset the currect record
            ap_count = (ap_count > ESP32_MAX_AP_RECORDS) ? ESP32_MAX_AP_RECORDS : ap_count;
            memset(s_ap_records, 0, ap_count * sizeof(wifi_ap_record_t));
            esp_wifi_scan_get_ap_records(&ap_count, s_ap_records);
            //ESP_LOGI(TAG, "Found %d records. Check if SSID is available ...", ap_count);
            for (int i = 0; i < ap_count; i++) {
                char* temp_ssid=(char *)s_ap_records[i].ssid;
                //char* temp_bssid=(char *)s_ap_records[i].bssid;
                //ESP_LOGI(TAG, "(%d)   %s | %02x:%02x:%02x:%02x:%02x:%02x", i, temp_ssid, 
                //            temp_bssid[0], temp_bssid[1], temp_bssid[2],
                //            temp_bssid[3], temp_bssid[4], temp_bssid[5]);
                if (strcmp(temp_ssid, wifi_config.sta.ssid) == 0) {
                    s_p_ap_record = &s_ap_records[i];
                    ssid_found = true;
                    break;
                }
            }
        } else {
            ESP_LOGE(TAG, "No Wi-Fi networks found.");
        }

        if (ssid_found && s_p_ap_record) {
            char* temp_bssid = (char *)s_p_ap_record->bssid;
            ESP_LOGI(TAG, "Found SSID=%s, BSSID(MAC)=%02x:%02x:%02x:%02x:%02x:%02x, Channel=%d, RSSI=%d dBm! Attempting to connect...",
                        s_p_ap_record->ssid, temp_bssid[0], temp_bssid[1], temp_bssid[2], temp_bssid[3], temp_bssid[4], temp_bssid[5],
                        s_p_ap_record->primary, s_p_ap_record->rssi);
            memcpy(wifi_config.sta.bssid, s_p_ap_record->bssid, sizeof(wifi_config.sta.bssid));
            // Use the BSSID (MAC address) obtained from the scan to connect directly to the target AP. This avoids ambiguity
            // in environments with multiple APs using the same SSID.
            wifi_config.sta.bssid_set = 1;
            wifi_config.sta.channel = s_p_ap_record->primary; // overwrite channel now since we know what it is now
            ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
            ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_disconnect());
            vTaskDelay(pdMS_TO_TICKS(500));
            ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_connect());
            s_retry_num=0;
        } else {
            //ESP_LOGE(TAG, "SSID %s not found", wifi_config.sta.ssid);
            // Retry logic or fallback mechanism
            if (s_retry_num < c_max_retry_num) {
                // Wait for 'ESP32_SCAN_RECONNECT_DELAY_MS' (ms) before doing the Wi-Fi scanning retry
                vTaskDelay(pdMS_TO_TICKS(ESP32_SCAN_RECONNECT_DELAY_MS));
                if(s_retry_num == 0){
                    ESP_LOGI(TAG, "Retrying SSID scanning... Attempt %d/%d", s_retry_num, c_max_retry_num);
                }
                esp_wifi_scan_start(NULL, true);  // Retry scanning for all IDs
                s_retry_num++;
            } else {
                // Switch to AP mode or perform another fallback action
                int status = (scan_event) ? scan_event->status : 1;
                Common_WifiStation_onDisconnected(EHS_TRUE, status, 0);
                gTargetWifiStationConnected = EHS_FALSE;
                ESP_LOGE(TAG, "SSID scanning max retries reached (%d/%d). Falling back.. (status=%d)", s_retry_num, c_max_retry_num, status);
                s_p_ap_record = NULL; // reset the currect record
                s_retry_num=0;
                setWifiStationConnectState(WifiStationConnectState_FAILED);
            }
        }
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        ESP_LOGE(TAG, "SSID: %s, reason: %d, rssi: %d", 
            ((wifi_event_sta_disconnected_t *)event_data)->ssid, 
            ((wifi_event_sta_disconnected_t *)event_data)->reason, 
            ((wifi_event_sta_disconnected_t *)event_data)->rssi);
        if (s_retry_num < c_max_retry_num && c_reconnect == EHS_TRUE)
        {
            s_retry_num++;
            ESP_LOGW(TAG, "retry to connect to the AP");
            // According to https://www.esp32.com/viewtopic.php?t=37331
            //  Stop and start to do reconnect reliably alongwith
            //  `failure_retry_cnt` in the wifi_config structure
            //esp_wifi_stop();
            //esp_wifi_start();
            vTaskDelay(pdMS_TO_TICKS(ESP32_WIFI_RECONNECT_DELAY_MS));
            esp_wifi_disconnect();
            vTaskDelay(pdMS_TO_TICKS(500));
            esp_wifi_connect();
        }
        else 
        {
            if (gTargetWifiStationConnected == EHS_TRUE)
            {
                wifi_event_sta_disconnected_t* event = (wifi_event_sta_disconnected_t*) event_data;
                Common_WifiStation_onDisconnected(EHS_TRUE, event->reason, event->rssi);
                gTargetWifiStationConnected = EHS_FALSE;
            }
            setWifiStationConnectState(WifiStationConnectState_FAILED);
        }
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED) {

    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Got IP:" IPSTR, IP2STR(&event->ip_info.ip));
        snprintf(IP_Address, MACIP_LENGTH - 1, IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        gTargetWifiStationConnected = EHS_TRUE;
        setWifiStationConnectState(WifiStationConnectState_CONNECTING_GOT_IP);
    }
    vTaskDelay(pdMS_TO_TICKS(1)); // 1 ms sleep
}

ehs_bool WifiStationScanResult(ehs_uint32 index, ehs_char* ssid, ehs_uint16 ssid_size, ehs_char* bssid, ehs_uint16 bssid_size,
                               ehs_sint32* channel, ehs_sint32* rssi)
{
    if(ssid_size < 33 || bssid < 6){
        return EHS_FALSE;
    }
    if(index < ESP32_MAX_AP_RECORDS){
        const wifi_ap_record_t* p_ap_record = &(s_ap_records[index]);
        if(p_ap_record && p_ap_record->ssid && strlen(p_ap_record->ssid) > 0){
            if(ssid) {
                int len = strlen(p_ap_record->ssid);
                if(len < 33){
                    EhsMemcpy(ssid, (const ehs_char*)p_ap_record->ssid, sizeof(ehs_char) * len+1);
                    ssid[len+1] = '\0';
                }
            }
            if(bssid) {
                EhsMemcpy(bssid, (const ehs_char*)p_ap_record->bssid, sizeof(ehs_char) * 6);
            }
            if(channel) *channel = (ehs_sint32)(p_ap_record->primary);
            if(rssi) *rssi = (ehs_sint32)(p_ap_record->rssi);
            return EHS_TRUE;
        }
    }
    return EHS_FALSE;
}

ehs_bool doWifiStationNetifInit(const ehs_char* host_name)
{
    esp_err_t ret = ESP_OK;
        
    if (sfWifiStationNetifGet() != EHS_TRUE)
    {
        ESP_GOTO_ON_ERROR(esp_netif_init(), doWifiStationInit_Return, TAG, "Network Interface init failed");
        ESP_GOTO_ON_ERROR(esp_event_loop_create_default(), doWifiStationInit_Return, TAG, "Event loop creation failed");
        sta_netif = esp_netif_create_default_wifi_sta();
        if(sta_netif){
            if(host_name) {
                ESP_LOGI(TAG, "Station hostname:%s",host_name);
                esp_netif_set_hostname(sta_netif, host_name);
            }
            ESP_LOGI(TAG, "Station Initalised!");
            sfWifiStationNetifSet(EHS_TRUE);
        }else{
            ret = ESP_FAIL;
            goto doWifiStationInit_Return;
        }
    }
doWifiStationInit_Return:
    return (ret == ESP_OK) ? EHS_TRUE : EHS_FALSE;
}

void doWifiStationNetifDestroy()
{
    if (sfWifiStationNetifGet() == EHS_TRUE)
    {
        sfWifiStationNetifSet(EHS_FALSE);
        if(sta_netif){
            esp_netif_destroy_default_wifi(sta_netif);
            sta_netif = NULL;
        }
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_loop_delete_default());
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_netif_deinit());
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

    eWifiStationConnectState conn_state = getWifiStationConnectState();

    if(conn_state == WifiStationConnectState_CONNECT){ // initalise a connect

        ehs_char* host_name = NULL; // @TODO - set it as optional fb param
        if(doWifiStationNetifInit(host_name) == EHS_FALSE){ // @TODO - this may only have to be initalised in the main thread
            ret = ESP_ERR_WIFI_NOT_INIT;
            goto doWifiStationConnect_Return;
        }
        gWifiStationInitalised = EHS_TRUE;
        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        ESP_GOTO_ON_ERROR(esp_wifi_init(&cfg), doWifiStationConnect_Return, TAG, "Wi-Fi init failed");
        ESP_GOTO_ON_ERROR(esp_wifi_set_ps(WIFI_PS_NONE), doWifiStationConnect_Return, TAG, "Wi-Fi setting power saver failed");
        
        s_retry_num = 0;

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
                ESP_LOGW(TAG, "SSID: [%s], password: [%s]", wifi_config.sta.ssid, wifi_config.sta.password);
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
        // Set to scan all channels in case the AP's channel changes
        wifi_config.sta.scan_method = WIFI_ALL_CHANNEL_SCAN;
        // Set Wi-Fi failure re-connect attempt to 10
        wifi_config.sta.failure_retry_cnt = 10;
        // Reset the channel field to 0, it will be identified by Wifi scan
        wifi_config.sta.channel = 0;
        // Reset the BSSID by setting it to zero (no specific BSSID)
        memset(wifi_config.sta.bssid, 0, sizeof(wifi_config.sta.bssid));
        // @TODO - Once BSSID is configurable make sure to copy it to wifi_config.bssid from the config passed into this function 
        
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
        setWifiStationConnectState(WifiStationConnectState_CONNECTING);
        return WifiStation_Connecting;

    }else if(conn_state == WifiStationConnectState_CONNECTING){

        return WifiStation_Connecting;
    }

    eWifiStationStatus errorCode;
    wifi_ap_record_t ap_info;
    
    if (conn_state == WifiStationConnectState_CONNECTING_GOT_IP)
    {
        ESP_LOGI(TAG, "Connected");
        errorCode = WifiStation_Connected;
        ESP_GOTO_ON_ERROR( esp_wifi_sta_get_ap_info(&ap_info), doWifiStationConnect_Return, TAG, "Cannot get Wi-FI AP information" );
        // TODO - fix this
        /*snprintf(mac_output, MACIP_LENGTH - 1, "%02x:%02x:%02x:%02x:%02x:%02x",
                 ap_info.bssid[0], ap_info.bssid[1], ap_info.bssid[2], ap_info.bssid[3], ap_info.bssid[4], ap_info.bssid[5]);
        if(rssi){
            *rssi = (ehs_sint8) ap_info.rssi;
        }
        if(ip_address){
            ip_address = (ehs_char *) IP_Address;
        }
        if(mac_address){
            mac_address = (ehs_char *) mac_output;
        }*/
    }
    else if (conn_state == WifiStationConnectState_FAILED)
    {
        errorCode = WifiStation_StartFailed;
        ret = (ret == ESP_OK) ? ESP_FAIL : ret;
    }
    else 
    {
        errorCode = WifiStation_StateError;
        ret = (ret == ESP_OK) ? ESP_FAIL : ret;
    }

doWifiStationConnect_Return:

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
    ESP_LOGW(TAG, "Destroy");
    esp_wifi_sta_wpa2_ent_clear_cert_key();
    if (serverCert_content != NULL) { free(serverCert_content); serverCert_content = NULL; }
    if (tlsCert_content != NULL) { free(tlsCert_content); tlsCert_content = NULL; }
    if (tlsKey_content != NULL) { free(tlsKey_content); tlsKey_content = NULL; }
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_disconnect());
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_stop());
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip));
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id));
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_deinit());
    gTargetWifiStationConnected = EHS_FALSE; // @TODO - use callback
    gWifiStationInitalised = EHS_FALSE;
}

void doWifiStationDisconnect()
{
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_disconnect());
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_stop());
    gTargetWifiStationConnected = EHS_FALSE; // @TODO - use callback
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

const ehs_char* WifiStationIpAddress()
{
    return IP_Address;
}

ehs_bool isWifiStationInitalised()
{
    return gWifiStationInitalised;
}

ehs_bool isWifiStationConnected()
{
    return gTargetWifiStationConnected;
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
