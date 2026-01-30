/***************************************************************
* Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license.
****************************************************************/

/**
 * NimBLE BLE Service HAL Implementation
 *
 * This file provides the hardware abstraction layer for the BLE service
 * component using the NimBLE stack (ESP32-S3 and other platforms).
 */

#include "ble_service_nimble.h"
#include "globals.h"
#include "hal-api.h"
#include <string.h>
#include <stdio.h>

#include "esp_log.h"
#include "esp_nimble_hci.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "host/ble_uuid.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"

static const char *TAG = "BLE_SERVICE_HAL";

/* Maximum number of characteristics per service */
#define MAX_CHARACTERISTICS 16
#define MAX_CHAR_VALUE_LEN 512

/* BLE service context structure */
typedef struct {
    /* Service configuration */
    ble_uuid_any_t service_uuid;
    char service_name[64];
    uint8_t num_chars;
    uint16_t mtu_size;
    uint32_t adv_interval_ms;

    /* Characteristic configuration */
    struct {
        ble_uuid_any_t uuid;
        char name[32];
        uint8_t properties;  /* Read=1, Write=2, Notify=4, Indicate=8 */
        uint16_t max_len;
        uint16_t value_handle;
        uint8_t value[MAX_CHAR_VALUE_LEN];
        uint16_t value_len;
    } chars[MAX_CHARACTERISTICS];

    /* Connection state */
    uint16_t conn_handle;
    ehs_bool connected;
    ehs_bool advertising;

    /* GATT service handle */
    uint16_t service_handle;

    /* Component context for callbacks */
    void* component_context;

    /* Callback function pointers (to internal ports) */
    inx_ble_service_callbacks_t callbacks;

} ble_service_context_t;

static ble_service_context_t g_ble_ctx = {0};

/* Forward declarations */
static ehs_sint32 ble_gap_event_handler(struct ble_gap_event *event, void *arg);
static ehs_sint32 ble_gatt_char_access_cb(uint16_t conn_handle, uint16_t attr_handle,
                                    struct ble_gatt_access_ctxt *ctxt, void *arg);

/**
 * Parse UUID string to ble_uuid_any_t structure
 * Supports both 16-bit and 128-bit UUIDs
 */
static ehs_sint32 parse_uuid(const char* uuid_str, ble_uuid_any_t* uuid)
{
    //TODO2026 Check this out properly
    if (1/*strlen(uuid_str) == 4*/) {
        /* 16-bit UUID (e.g., "180A") */
        uint16_t uuid16 = 6154;
        //printf("[%s] string length 4\n", __func__);
        printf("1\n");
        //if (sscanf(uuid_str, "%04hx", &uuid16) != 1) {
        //    return -1;
        //}
        uuid->u.type = BLE_UUID_TYPE_16;
        uuid->u16.value = uuid16;
        return 0;
    } else if (strlen(uuid_str) == 36) {
        /* 128-bit UUID (e.g., "0000180A-0000-1000-8000-00805F9B34FB") */
        uint8_t uuid128[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                            0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
        if (ble_uuid_init_from_buf(uuid, uuid128, 16) != 0) {
            return -1;
        }
        //TODO: non-existant function for esp-idf version 5.1
        //  Need to come up with a custom function to convert the UUID string to the UUID object
        // if (ble_uuid_from_str(uuid_str, &uuid->u128.u) != 0) {
        //     return -1;
        // }
        return 0;
    }

    return -1;
}

/**
 * GATT characteristic access callback
 * Called when a client reads or writes a characteristic
 */
static ehs_sint32 ble_gatt_char_access_cb(uint16_t conn_handle, uint16_t attr_handle,
                                    struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    ehs_sint32 char_idx = (ehs_sint32)(intptr_t)arg;

    if (char_idx < 0 || char_idx >= g_ble_ctx.num_chars) {
        ESP_LOGE(TAG, "Invalid characteristic index: %d", char_idx);
        return BLE_ATT_ERR_UNLIKELY;
    }

    ESP_LOGI(TAG, "Char access: idx=%d, op=%d, handle=%d",
             char_idx, ctxt->op, attr_handle);

    switch (ctxt->op) {
        case BLE_GATT_ACCESS_OP_READ_CHR:
            /* Client is reading the characteristic */
            if (g_ble_ctx.chars[char_idx].properties & 0x01) { /* Read property */
                ehs_sint32 rc = os_mbuf_append(ctxt->om,
                                       g_ble_ctx.chars[char_idx].value,
                                       g_ble_ctx.chars[char_idx].value_len);
                return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
            }
            return BLE_ATT_ERR_READ_NOT_PERMITTED;

        case BLE_GATT_ACCESS_OP_WRITE_CHR:
            /* Client is writing to the characteristic */
            if (g_ble_ctx.chars[char_idx].properties & 0x02) { /* Write property */
                uint16_t len = OS_MBUF_PKTLEN(ctxt->om);
                if (len > g_ble_ctx.chars[char_idx].max_len) {
                    return BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN;
                }

                /* Copy the data */
                ehs_sint32 rc = ble_hs_mbuf_to_flat(ctxt->om,
                                            g_ble_ctx.chars[char_idx].value,
                                            g_ble_ctx.chars[char_idx].max_len,
                                            &g_ble_ctx.chars[char_idx].value_len);
                if (rc != 0) {
                    return BLE_ATT_ERR_UNLIKELY;
                }

                /* Trigger on_client_write callback */
                if (g_ble_ctx.callbacks.on_client_write) {
                    g_ble_ctx.callbacks.on_client_write(
                        g_ble_ctx.component_context,
                        char_idx,
                        (char*)g_ble_ctx.chars[char_idx].value,
                        g_ble_ctx.chars[char_idx].value_len);
                }

                return 0;
            }
            return BLE_ATT_ERR_WRITE_NOT_PERMITTED;

        default:
            return BLE_ATT_ERR_UNLIKELY;
    }
}

/**
 * GAP event handler
 * Handles connection, disconnection, and advertising events
 */
static ehs_sint32 ble_gap_event_handler(struct ble_gap_event *event, void *arg)
{
    switch (event->type) {
        case BLE_GAP_EVENT_CONNECT:
            ESP_LOGI(TAG, "Connection %s; status=%d",
                    event->connect.status == 0 ? "established" : "failed",
                    event->connect.status);

            if (event->connect.status == 0) {
                g_ble_ctx.conn_handle = event->connect.conn_handle;
                g_ble_ctx.connected = true;

                /* Trigger on_connect callback */
                if (g_ble_ctx.callbacks.on_connect) {
                    g_ble_ctx.callbacks.on_connect(
                        g_ble_ctx.component_context,
                        event->connect.conn_handle);
                }
            } else {
                /* Connection failed, resume advertising */
                g_ble_ctx.advertising = false;
            }
            break;

        case BLE_GAP_EVENT_DISCONNECT:
            ESP_LOGI(TAG, "Disconnect; reason=%d", event->disconnect.reason);
            g_ble_ctx.connected = false;
            g_ble_ctx.advertising = false;

            /* Trigger on_disconnect callback */
            if (g_ble_ctx.callbacks.on_disconnect) {
                g_ble_ctx.callbacks.on_disconnect(
                    g_ble_ctx.component_context,
                    event->disconnect.conn.conn_handle,
                    event->disconnect.reason);
            }
            break;

        case BLE_GAP_EVENT_ADV_COMPLETE:
            ESP_LOGI(TAG, "Advertising complete; reason=%d", event->adv_complete.reason);
            g_ble_ctx.advertising = false;
            break;

        case BLE_GAP_EVENT_MTU:
            ESP_LOGI(TAG, "MTU update: conn_handle=%d, mtu=%d",
                    event->mtu.conn_handle, event->mtu.value);
            break;

        case BLE_GAP_EVENT_SUBSCRIBE:
            ESP_LOGI(TAG, "Subscribe event; conn_handle=%d, attr_handle=%d, "
                    "reason=%d, prevn=%d, curn=%d, previ=%d, curi=%d",
                    event->subscribe.conn_handle,
                    event->subscribe.attr_handle,
                    event->subscribe.reason,
                    event->subscribe.prev_notify,
                    event->subscribe.cur_notify,
                    event->subscribe.prev_indicate,
                    event->subscribe.cur_indicate);
            break;

        default:
            break;
    }

    return 0;
}

/**
 * BLE host task
 */
static void ble_host_task(void *param)
{
    ESP_LOGI(TAG, "BLE Host Task Started");
    nimble_port_run();
    nimble_port_freertos_deinit();
}

/**
 * Initialize BLE service with configuration
 */
ehs_sint32 inx_ble_service_hal_init(
    const char* service_uuid,
    const char* service_name,
    ehs_uint8 num_chars,
    ehs_uint32 adv_interval_ms,
    ehs_uint16 mtu_size,
    inx_ble_char_config_t* char_configs,
    inx_ble_service_callbacks_t* callbacks,
    void* component_context)
{
    esp_log_level_set(TAG, ESP_LOG_DEBUG);
    esp_log_level_set("BLE_INIT", ESP_LOG_DEBUG);
    if (num_chars > MAX_CHARACTERISTICS) {
        ESP_LOGE(TAG, "Too many characteristics: %d (max %d)",
                 num_chars, MAX_CHARACTERISTICS);
        return -1;
    }

    //ESP_LOGI(TAG, "Initializing BLE service: %s", service_name);

    /* Store configuration */
    memset(&g_ble_ctx, 0, sizeof(g_ble_ctx));

    if (parse_uuid(service_uuid, &g_ble_ctx.service_uuid) != 0) {
        ESP_LOGE(TAG, "Failed to parse service UUID: %s", service_uuid);
        return -1;
    }

    //strncpy(g_ble_ctx.service_name, service_name, sizeof(g_ble_ctx.service_name) - 1);
    g_ble_ctx.num_chars = num_chars;
    g_ble_ctx.adv_interval_ms = adv_interval_ms;
    g_ble_ctx.mtu_size = mtu_size;
    g_ble_ctx.component_context = component_context;

    if (callbacks) {
        memcpy(&g_ble_ctx.callbacks, callbacks, sizeof(inx_ble_service_callbacks_t));
    }

    /* Store characteristic configurations */
    for (ehs_sint32 i = 0; i < num_chars; i++) {
        if (parse_uuid(char_configs[i].uuid, &g_ble_ctx.chars[i].uuid) != 0) {
            ESP_LOGE(TAG, "Failed to parse char UUID: %s", char_configs[i].uuid);
            return -1;
        }

        //strncpy(g_ble_ctx.chars[i].name, char_configs[i].name,
        //        sizeof(g_ble_ctx.chars[i].name) - 1);
        g_ble_ctx.chars[i].properties = char_configs[i].properties;
        g_ble_ctx.chars[i].max_len = char_configs[i].max_len;
        g_ble_ctx.chars[i].value_len = 0;
    }

    /* Initialize NimBLE */
    // esp_err_t ret = esp_nimble_hci_and_controller_init();
    // if (ret != ESP_OK) {
    //     ESP_LOGE(TAG, "Failed to initialize controller: %d", ret);
    //     return -1;
    // }

    esp_err_t err = nimble_port_init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Init failed @%d", err);
    }

    /* Initialize the NimBLE host configuration */
    ble_hs_cfg.sync_cb = NULL;
    ble_hs_cfg.reset_cb = NULL;

    /* Start the BLE host task */
    nimble_port_freertos_init(ble_host_task);

    ESP_LOGI(TAG, "BLE service initialized successfully");
    return 0;
}

/**
 * Register GATT service and characteristics
 */
ehs_sint32 inx_ble_service_hal_register_gatt(void)
{
    struct ble_gatt_svc_def *gatt_svcs;
    struct ble_gatt_chr_def *gatt_chrs;
    ehs_sint32 rc;

    /* Allocate memory for service and characteristic definitions */
    gatt_svcs = calloc(2, sizeof(struct ble_gatt_svc_def)); /* Service + NULL terminator */
    gatt_chrs = calloc(g_ble_ctx.num_chars + 1,
                      sizeof(struct ble_gatt_chr_def)); /* Chars + NULL terminator */

    if (!gatt_svcs || !gatt_chrs) {
        ESP_LOGE(TAG, "Failed to allocate memory for GATT definitions");
        free(gatt_svcs);
        free(gatt_chrs);
        return -1;
    }

    /* Build characteristic definitions */
    for (ehs_sint32 i = 0; i < g_ble_ctx.num_chars; i++) {
        gatt_chrs[i].uuid = &g_ble_ctx.chars[i].uuid.u;
        gatt_chrs[i].access_cb = ble_gatt_char_access_cb;
        gatt_chrs[i].arg = (void*)(intptr_t)i;
        gatt_chrs[i].val_handle = &g_ble_ctx.chars[i].value_handle;

        /* Set flags based on properties */
        uint8_t flags = 0;
        if (g_ble_ctx.chars[i].properties & 0x01) { /* Read */
            flags |= BLE_GATT_CHR_F_READ;
        }
        if (g_ble_ctx.chars[i].properties & 0x02) { /* Write */
            flags |= BLE_GATT_CHR_F_WRITE;
        }
        if (g_ble_ctx.chars[i].properties & 0x04) { /* Notify */
            flags |= BLE_GATT_CHR_F_NOTIFY;
        }
        if (g_ble_ctx.chars[i].properties & 0x08) { /* Indicate */
            flags |= BLE_GATT_CHR_F_INDICATE;
        }
        gatt_chrs[i].flags = flags;
    }

    /* Build service definition */
    gatt_svcs[0].type = BLE_GATT_SVC_TYPE_PRIMARY;
    gatt_svcs[0].uuid = &g_ble_ctx.service_uuid.u;
    gatt_svcs[0].characteristics = gatt_chrs;

    /* Register the GATT services */
    rc = ble_gatts_count_cfg(gatt_svcs);
    if (rc != 0) {
        ESP_LOGE(TAG, "Failed to count GATT config: %d", rc);
        free(gatt_svcs);
        free(gatt_chrs);
        return -1;
    }

    rc = ble_gatts_add_svcs(gatt_svcs);
    if (rc != 0) {
        ESP_LOGE(TAG, "Failed to add GATT services: %d", rc);
        free(gatt_svcs);
        free(gatt_chrs);
        return -1;
    }

    /* Start GATT services */
    ble_gatts_start();

    ESP_LOGI(TAG, "GATT service registered with %d characteristics",
             g_ble_ctx.num_chars);

    /* Note: Don't free gatt_svcs and gatt_chrs as they're still in use by NimBLE */
    return 0;
}

/**
 * Start BLE advertising
 */
ehs_sint32 inx_ble_service_hal_start_adv(void)
{
    if (g_ble_ctx.advertising) {
        ESP_LOGW(TAG, "Already advertising");
        return -1;
    }

    struct ble_gap_adv_params adv_params;
    struct ble_hs_adv_fields fields;
    ehs_sint32 rc;

    /* Set advertising data */
    memset(&fields, 0, sizeof(fields));
    fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;
    fields.name = (uint8_t *)g_ble_ctx.service_name;
    fields.name_len = strlen(g_ble_ctx.service_name);
    fields.name_is_complete = 1;

    rc = ble_gap_adv_set_fields(&fields);
    if (rc != 0) {
        ESP_LOGE(TAG, "Failed to set advertising data: %d", rc);
        return -1;
    }

    /* Start advertising */
    memset(&adv_params, 0, sizeof(adv_params));
    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;
    adv_params.itvl_min = (g_ble_ctx.adv_interval_ms * 1000) / 625; /* Convert ms to 0.625ms units */
    adv_params.itvl_max = (g_ble_ctx.adv_interval_ms * 1000) / 625;

    rc = ble_gap_adv_start(BLE_OWN_ADDR_PUBLIC, NULL, BLE_HS_FOREVER,
                          &adv_params, ble_gap_event_handler, NULL);
    if (rc != 0) {
        ESP_LOGE(TAG, "Failed to start advertising: %d", rc);
        return -1;
    }

    g_ble_ctx.advertising = true;
    ESP_LOGI(TAG, "Advertising started");
    return 0;
}

/**
 * Stop BLE advertising
 */
ehs_sint32 inx_ble_service_hal_stop_adv(void)
{
    if (!g_ble_ctx.advertising) {
        ESP_LOGW(TAG, "Not advertising");
        return -1;
    }

    ehs_sint32 rc = ble_gap_adv_stop();
    if (rc != 0) {
        ESP_LOGE(TAG, "Failed to stop advertising: %d", rc);
        return -1;
    }

    g_ble_ctx.advertising = false;
    ESP_LOGI(TAG, "Advertising stopped");
    return 0;
}

/**
 * Write value to a characteristic (local write, not notification)
 */
ehs_sint32 inx_ble_service_hal_write_char(uint8_t char_idx, const char* data, uint16_t length)
{
    if (char_idx >= g_ble_ctx.num_chars) {
        ESP_LOGE(TAG, "Invalid characteristic index: %d", char_idx);
        return -1;
    }

    if (length > g_ble_ctx.chars[char_idx].max_len) {
        ESP_LOGE(TAG, "Data too long: %d (max %d)", length,
                 g_ble_ctx.chars[char_idx].max_len);
        return -2;
    }

    memcpy(g_ble_ctx.chars[char_idx].value, data, length);
    g_ble_ctx.chars[char_idx].value_len = length;

    ESP_LOGI(TAG, "Characteristic %d value updated locally", char_idx);
    return 0;
}

/**
 * Read value from a characteristic
 */
ehs_sint32 inx_ble_service_hal_read_char(uint8_t char_idx, char* data,
                                   uint16_t* length, uint16_t max_len)
{
    if (char_idx >= g_ble_ctx.num_chars) {
        ESP_LOGE(TAG, "Invalid characteristic index: %d", char_idx);
        return -1;
    }

    uint16_t copy_len = g_ble_ctx.chars[char_idx].value_len;
    if (copy_len > max_len) {
        copy_len = max_len;
    }

    memcpy(data, g_ble_ctx.chars[char_idx].value, copy_len);
    *length = copy_len;

    ESP_LOGI(TAG, "Characteristic %d value read: %d bytes", char_idx, copy_len);
    return 0;
}

/**
 * Send notification to connected client
 */
ehs_sint32 inx_ble_service_hal_notify(uint8_t char_idx, const char* data, uint16_t length)
{
    if (!g_ble_ctx.connected) {
        ESP_LOGW(TAG, "Cannot notify: not connected");
        return -1;
    }

    if (char_idx >= g_ble_ctx.num_chars) {
        ESP_LOGE(TAG, "Invalid characteristic index: %d", char_idx);
        return -2;
    }

    if (!(g_ble_ctx.chars[char_idx].properties & 0x04)) {
        ESP_LOGE(TAG, "Characteristic %d does not support notifications", char_idx);
        return -3;
    }

    /* Update local value */
    if (length > g_ble_ctx.chars[char_idx].max_len) {
        length = g_ble_ctx.chars[char_idx].max_len;
    }
    memcpy(g_ble_ctx.chars[char_idx].value, data, length);
    g_ble_ctx.chars[char_idx].value_len = length;

    /* Create mbuf for notification */
    struct os_mbuf *om = ble_hs_mbuf_from_flat(data, length);
    if (om == NULL) {
        ESP_LOGE(TAG, "Failed to allocate mbuf for notification");
        return -4;
    }

    /* Send notification */
    ehs_sint32 rc = ble_gattc_notify_custom(g_ble_ctx.conn_handle,
                                     g_ble_ctx.chars[char_idx].value_handle, om);
    if (rc != 0) {
        ESP_LOGE(TAG, "Failed to send notification: %d", rc);
        return -5;
    }

    ESP_LOGI(TAG, "Notification sent for characteristic %d", char_idx);
    return 0;
}

/**
 * Check if a client is connected
 */
ehs_bool inx_ble_service_hal_is_connected(void)
{
    return g_ble_ctx.connected;
}

/**
 * Check if advertising
 */
ehs_bool inx_ble_service_hal_is_advertising(void)
{
    return g_ble_ctx.advertising;
}

/**
 * Deinitialize BLE service
 */
void inx_ble_service_hal_deinit(void)
{
    if (g_ble_ctx.advertising) {
        ble_gap_adv_stop();
    }

    if (g_ble_ctx.connected) {
        ble_gap_terminate(g_ble_ctx.conn_handle, BLE_ERR_REM_USER_CONN_TERM);
    }

    nimble_port_stop();
    // esp_nimble_hci_and_controller_deinit();

    memset(&g_ble_ctx, 0, sizeof(g_ble_ctx));
    ESP_LOGI(TAG, "BLE service deinitialized");
}

