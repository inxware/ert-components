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
#include "hal_string.h"

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


//#define ble_debug_printf printf
#define ble_debug_printf 

static const char *TAG = "BLE_SERVICE_HAL";

#define MAX_CHAR_VALUE_LEN  512
/* Upper bound for the nimble-specific per-char runtime state array.
 * Independent of any FB limit — sized to what this backend can support. */
#define BLE_HAL_MAX_CHARS    16

/*
 * Module-level lifecycle state for the BLE HAL.
 *
 * Used by inx_ble_service_hal_init / _deinit to be idempotent and to recover
 * from partial-teardown by tearing down before re-init. NimBLE's
 * esp_bt_controller_init returns ESP_ERR_INVALID_STATE (0x103) if called
 * again while the controller is not in IDLE — see
 * ert-contrib-middleware/.../bt/controller/esp32c3/bt.c:1110.
 *
 * NOTE: this is a target-local copy. There is an open TODO (see the BLE
 * section in docs/ert-porting-guide.md) to lift the state machine into a
 * shared Common/HAL helper so other BLE backends (Zephyr, BlueZ, Win32) do
 * not need to reinvent it — the pattern is the same as the WiFi station SM.
 */
typedef enum {
    BLE_HAL_STATE_UNINIT = 0,    /* before first init / after full deinit */
    BLE_HAL_STATE_INITIALIZED,   /* nimble_port_init + freertos_init done  */
    BLE_HAL_STATE_RUNNING,       /* registered & advertising                */
    BLE_HAL_STATE_ERROR,         /* last transition failed; safe to retry  */
} ble_hal_state_t;

static volatile ble_hal_state_t g_ble_hal_state = BLE_HAL_STATE_UNINIT;

/* BLE service context structure */
typedef struct {
    /* Service configuration */
    ble_uuid_any_t service_uuid;
    char service_name[INX_BLE_NAME_MAX + 1];
    uint8_t num_chars;
    uint32_t adv_interval_ms;

    /* Pointer to the caller's inx_ble_char_config_t array — holds config
     * fields (uuid in inx format, name, properties, max_len).  The caller's
     * state struct outlives the HAL so this pointer remains valid. */
    const inx_ble_char_config_t* char_configs;

    /* Per-characteristic NimBLE-specific runtime state only.
     * Config fields (properties, max_len, name) are read from char_configs. */
    struct {
        ble_uuid_any_t uuid;          /* converted to NimBLE byte order at init */
        uint16_t       value_handle;  /* assigned by NimBLE at GATT registration */
        uint8_t        value[MAX_CHAR_VALUE_LEN];
        uint16_t       value_len;
    } chars[BLE_HAL_MAX_CHARS];

    /* Connection state */
    uint16_t conn_handle;
    ehs_bool connected;
    ehs_bool advertising;

    /* GATT service handle */
    uint16_t service_handle;

    /* Host-sync handshake state. host_synced is set by the NimBLE
     * sync callback once the host has finished syncing with the
     * controller; advertising must wait for this. want_adv is set
     * by inx_ble_service_hal_start_adv() if it is called before
     * sync — the sync callback will then start advertising. */
    ehs_bool host_synced;
    ehs_bool want_adv;

    /* Component context for callbacks */
    void* component_context;

    /* Callback function pointers */
    inx_ble_service_callbacks_t callbacks;

} ble_service_context_t;

static ble_service_context_t g_ble_ctx = {0};

/* Forward declarations */
static ehs_sint32 ble_gap_event_handler(struct ble_gap_event *event, void *arg);
static ehs_sint32 ble_gatt_char_access_cb(uint16_t conn_handle, uint16_t attr_handle,
                                    struct ble_gatt_access_ctxt *ctxt, void *arg);
static void ble_on_sync(void);
static void ble_on_reset(int reason);
static ehs_sint32 ble_do_start_adv(void);
static ehs_sint32 register_gatt(void);

/* Convert a pre-parsed inx_ble_uuid_t to the NimBLE ble_uuid_any_t.
 *
 * NimBLE stores 128-bit UUIDs in little-endian byte order (wire order).
 * inx_ble_uuid_t stores them in big-endian (network) order matching the
 * standard "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX" string. We reverse the
 * bytes on the way in. 16-bit UUIDs are just a direct copy of the value. */
static ehs_sint32 INX_PARSE_UUID(const inx_ble_uuid_t* inx_uuid, ble_uuid_any_t* out)
{
    if (!inx_uuid || !out) return -1;

    if (inx_uuid->type == INX_BLE_UUID_TYPE_16) {
        out->u.type    = BLE_UUID_TYPE_16;
        out->u16.value = inx_uuid->value.u16;
        return 0;
    }

    if (inx_uuid->type == INX_BLE_UUID_TYPE_128) {
        out->u.type = BLE_UUID_TYPE_128;
        /* Reverse byte order: inx big-endian → NimBLE little-endian */
        for (int i = 0; i < 16; i++) {
            out->u128.value[i] = inx_uuid->value.u128[15 - i];
        }
        return 0;
    }

    ble_debug_printf("[BLE_HAL] INX_PARSE_UUID: unset or unknown UUID type %d\n", inx_uuid->type);
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
        ble_debug_printf("[BLE_HAL] gatt_access_cb: bad char_idx=%d (num_chars=%d)\n",
               char_idx, g_ble_ctx.num_chars);
        return BLE_ATT_ERR_UNLIKELY;
    }

    ble_debug_printf("[BLE_HAL] gatt_access_cb: char=%d op=%d attr_handle=%d name=%s\n",
           char_idx, ctxt->op, attr_handle,
           g_ble_ctx.char_configs[char_idx].name);

    switch (ctxt->op) {
        case BLE_GATT_ACCESS_OP_READ_CHR:
            if (g_ble_ctx.char_configs[char_idx].properties & 0x01) { /* Read */
                ble_debug_printf("[BLE_HAL] gatt_access_cb: READ char=%d len=%d\n",
                       char_idx, g_ble_ctx.chars[char_idx].value_len);
                ehs_sint32 rc = os_mbuf_append(ctxt->om,
                                       g_ble_ctx.chars[char_idx].value,
                                       g_ble_ctx.chars[char_idx].value_len);
                return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
            }
            ble_debug_printf("[BLE_HAL] gatt_access_cb: READ not permitted on char=%d props=0x%02x\n",
                   char_idx, g_ble_ctx.char_configs[char_idx].properties);
            return BLE_ATT_ERR_READ_NOT_PERMITTED;

        case BLE_GATT_ACCESS_OP_WRITE_CHR:
            if (g_ble_ctx.char_configs[char_idx].properties & 0x02) { /* Write */
                uint16_t len = OS_MBUF_PKTLEN(ctxt->om);
                ble_debug_printf("[BLE_HAL] gatt_access_cb: WRITE char=%d len=%d max=%d\n",
                       char_idx, len, g_ble_ctx.char_configs[char_idx].max_len);
                if (len > (uint16_t)g_ble_ctx.char_configs[char_idx].max_len) {
                    ble_debug_printf("[BLE_HAL] gatt_access_cb: WRITE too long, rejecting\n");
                    return BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN;
                }

                ehs_sint32 rc = ble_hs_mbuf_to_flat(ctxt->om,
                                            g_ble_ctx.chars[char_idx].value,
                                            (uint16_t)g_ble_ctx.char_configs[char_idx].max_len,
                                            &g_ble_ctx.chars[char_idx].value_len);
                if (rc != 0) {
                    ble_debug_printf("[BLE_HAL] gatt_access_cb: ble_hs_mbuf_to_flat failed rc=%d\n", rc);
                    return BLE_ATT_ERR_UNLIKELY;
                }

                ble_debug_printf("[BLE_HAL] gatt_access_cb: WRITE done, firing on_client_write cb=%s\n",
                       g_ble_ctx.callbacks.on_client_write ? "set" : "NULL");
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
            ble_debug_printf("[BLE_HAL] gatt_access_cb: WRITE not permitted on char=%d props=0x%02x\n",
                   char_idx, g_ble_ctx.char_configs[char_idx].properties);
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
            ble_debug_printf("[BLE_HAL] GAP CONNECT: status=%d handle=%d\n",
                   event->connect.status, event->connect.conn_handle);

            if (event->connect.status == 0) {
                g_ble_ctx.conn_handle = event->connect.conn_handle;
                g_ble_ctx.connected = true;
                g_ble_ctx.advertising = false;  /* controller stops adv on connect */

                ble_debug_printf("[BLE_HAL] GAP CONNECT: connected OK, firing on_connect cb=%s\n",
                       g_ble_ctx.callbacks.on_connect ? "set" : "NULL");
                if (g_ble_ctx.callbacks.on_connect) {
                    g_ble_ctx.callbacks.on_connect(
                        g_ble_ctx.component_context,
                        event->connect.conn_handle);
                }
            } else {
                ble_debug_printf("[BLE_HAL] GAP CONNECT: connection failed, clearing advertising flag\n");
                g_ble_ctx.advertising = false;
            }
            break;

        case BLE_GAP_EVENT_DISCONNECT:
            ble_debug_printf("[BLE_HAL] GAP DISCONNECT: handle=%d reason=%d\n",
                   event->disconnect.conn.conn_handle, event->disconnect.reason);
            g_ble_ctx.connected = false;
            g_ble_ctx.advertising = false;

            ble_debug_printf("[BLE_HAL] GAP DISCONNECT: firing on_disconnect cb=%s\n",
                   g_ble_ctx.callbacks.on_disconnect ? "set" : "NULL");
            if (g_ble_ctx.callbacks.on_disconnect) {
                g_ble_ctx.callbacks.on_disconnect(
                    g_ble_ctx.component_context,
                    event->disconnect.conn.conn_handle,
                    event->disconnect.reason);
            }
            break;

        case BLE_GAP_EVENT_ADV_COMPLETE:
            ble_debug_printf("[BLE_HAL] GAP ADV_COMPLETE: reason=%d\n", event->adv_complete.reason);
            g_ble_ctx.advertising = false;
            break;

        case BLE_GAP_EVENT_MTU:
            ble_debug_printf("[BLE_HAL] GAP MTU: conn_handle=%d mtu=%d\n",
                   event->mtu.conn_handle, event->mtu.value);
            break;

        case BLE_GAP_EVENT_CONN_UPDATE:
            ble_debug_printf("[BLE_HAL] GAP CONN_UPDATE: conn_handle=%d status=%d\n",
                   event->conn_update.conn_handle, event->conn_update.status);
            break;

        case BLE_GAP_EVENT_SUBSCRIBE:
            ble_debug_printf("[BLE_HAL] GAP SUBSCRIBE: conn=%d attr=%d reason=%d notify=%d->%d indicate=%d->%d\n",
                   event->subscribe.conn_handle,
                   event->subscribe.attr_handle,
                   event->subscribe.reason,
                   event->subscribe.prev_notify, event->subscribe.cur_notify,
                   event->subscribe.prev_indicate, event->subscribe.cur_indicate);
            break;

        default:
            ble_debug_printf("[BLE_HAL] GAP event type=%d (unhandled)\n", event->type);
            break;
    }

    return 0;
}

/**
 * BLE host task
 */
static void ble_host_task(void *param)
{
    ble_debug_printf("[BLE_HAL] ble_host_task: started\n");
    nimble_port_run();
    ble_debug_printf("[BLE_HAL] ble_host_task: nimble_port_run returned, calling deinit\n");
    nimble_port_freertos_deinit();
    ble_debug_printf("[BLE_HAL] ble_host_task: exited\n");
}

/*
 * Bring the NimBLE host + ESP BT controller back to a clean UNINIT state.
 * Used by inx_ble_service_hal_init() to recover from a previous failed init
 * and by inx_ble_service_hal_deinit().
 *
 * Order matters: nimble_port_stop() makes nimble_port_run() return inside
 * the host task; after that returns, nimble_port_freertos_deinit() removes
 * the host task; then nimble_port_deinit() disables and deinits the
 * controller. Skipping any step leaves the controller in a non-IDLE state
 * which makes the next esp_bt_controller_init() return ESP_ERR_INVALID_STATE.
 */
static void ble_full_teardown(void)
{
    ble_debug_printf("[BLE_HAL] ble_full_teardown: state=%d\n", (int)g_ble_hal_state);
    if (g_ble_hal_state == BLE_HAL_STATE_UNINIT) {
        ble_debug_printf("[BLE_HAL] ble_full_teardown: already uninit, skipping\n");
        return;
    }

    if (g_ble_ctx.advertising) {
        ble_debug_printf("[BLE_HAL] ble_full_teardown: stopping advertising\n");
        ble_gap_adv_stop();
        g_ble_ctx.advertising = false;
    }
    if (g_ble_ctx.connected) {
        ble_debug_printf("[BLE_HAL] ble_full_teardown: terminating connection handle=%d\n",
               g_ble_ctx.conn_handle);
        ble_gap_terminate(g_ble_ctx.conn_handle, BLE_ERR_REM_USER_CONN_TERM);
        g_ble_ctx.connected = false;
    }

    ble_debug_printf("[BLE_HAL] ble_full_teardown: calling nimble_port_stop\n");
    int rc = nimble_port_stop();
    if (rc != 0) {
        ble_debug_printf("[BLE_HAL] ble_full_teardown: nimble_port_stop returned %d\n", rc);
    }

    ble_debug_printf("[BLE_HAL] ble_full_teardown: calling nimble_port_deinit\n");
    esp_err_t err = nimble_port_deinit();
    if (err != ESP_OK) {
        ble_debug_printf("[BLE_HAL] ble_full_teardown: nimble_port_deinit returned 0x%x\n", err);
    }

    memset(&g_ble_ctx, 0, sizeof(g_ble_ctx));
    g_ble_hal_state = BLE_HAL_STATE_UNINIT;
    ble_debug_printf("[BLE_HAL] ble_full_teardown: done\n");
}

/*
 * NimBLE host sync callback. Fires on the nimble_host task once the host has
 * finished its initial sync with the controller. Until this fires, no GAP
 * action (advertising, scanning, connecting) will succeed — this is the
 * canonical barrier in every IDF NimBLE example.
 *
 * If start_adv was requested before sync, honour it now.
 */
static void ble_on_sync(void)
{
    ble_debug_printf("[BLE_HAL] ble_on_sync: NimBLE host synced, want_adv=%d advertising=%d\n",
           g_ble_ctx.want_adv, g_ble_ctx.advertising);
    g_ble_ctx.host_synced = EHS_TRUE;
    g_ble_hal_state = BLE_HAL_STATE_RUNNING;

    if (g_ble_ctx.want_adv && !g_ble_ctx.advertising) {
        ble_debug_printf("[BLE_HAL] ble_on_sync: deferred adv request pending, starting now\n");
        g_ble_ctx.want_adv = EHS_FALSE;
        ble_do_start_adv();
    } else {
        ble_debug_printf("[BLE_HAL] ble_on_sync: no pending adv request\n");
    }
}

/*
 * NimBLE host reset callback. Fires on the nimble_host task if the host
 * loses sync with the controller (e.g. controller HW reset). After this
 * fires the host re-syncs and ble_on_sync will fire again — clear our
 * cached sync state here.
 */
static void ble_on_reset(int reason)
{
    ble_debug_printf("[BLE_HAL] ble_on_reset: host reset reason=%d, clearing synced/advertising/connected\n",
           reason);
    g_ble_ctx.host_synced = EHS_FALSE;
    g_ble_ctx.advertising = EHS_FALSE;
    g_ble_ctx.connected   = EHS_FALSE;
}

/**
 * Initialize BLE service with configuration
 *
 * Idempotent: if the HAL is already initialized this performs a full
 * teardown first so we always start from a known-IDLE controller state.
 * This avoids esp_bt_controller_init() returning ESP_ERR_INVALID_STATE
 * (0x103) on a second call (FB block destroy/re-init cycle).
 */
ehs_sint32 inx_ble_service_hal_init(
    const inx_ble_uuid_t* service_uuid,
    const char* service_name,
    ehs_uint8 num_chars,
    ehs_uint32 adv_interval_ms,
    inx_ble_char_config_t* char_configs,
    inx_ble_service_callbacks_t* callbacks,
    void* component_context)
{
    ble_debug_printf("[BLE_HAL] hal_init: name=%s num_chars=%d adv_ms=%d state=%d\n",
           service_name, (int)num_chars, (int)adv_interval_ms, (int)g_ble_hal_state);

    if (num_chars > BLE_HAL_MAX_CHARS) {
        ble_debug_printf("[BLE_HAL] hal_init: ERROR too many characteristics %d (max %d)\n",
               num_chars, BLE_HAL_MAX_CHARS);
        return -1;
    }

    /* HACK: init-once policy — NimBLE controller cannot be re-initialised
     * without a full teardown (esp_bt_controller_init returns INVALID_STATE).
     * On app reload we just refresh the context pointers and return. */
    if (g_ble_hal_state != BLE_HAL_STATE_UNINIT) {
        ble_debug_printf("[BLE_HAL] hal_init: already initialised (state=%d), refreshing context pointers only\n",
               (int)g_ble_hal_state);
        g_ble_ctx.component_context = component_context;
        g_ble_ctx.char_configs = char_configs;
        if (callbacks) {
            EhsMemcpy(&g_ble_ctx.callbacks, callbacks,
                      sizeof(inx_ble_service_callbacks_t));
        }
        ble_debug_printf("[BLE_HAL] hal_init: context refreshed, callbacks.on_write=%s\n",
               g_ble_ctx.callbacks.on_client_write ? "set" : "NULL");
        return 0;
    }

    ble_debug_printf("[BLE_HAL] hal_init: first init, zeroing context\n");
    memset(&g_ble_ctx, 0, sizeof(g_ble_ctx));

    ble_debug_printf("[BLE_HAL] hal_init: parsing service UUID (type=%d)\n",
           service_uuid ? service_uuid->type : -1);
    if (INX_PARSE_UUID(service_uuid, &g_ble_ctx.service_uuid) != 0) {
        ble_debug_printf("[BLE_HAL] hal_init: ERROR failed to parse service UUID\n");
        g_ble_hal_state = BLE_HAL_STATE_ERROR;
        return -1;
    }
    EhsSnprintf(g_ble_ctx.service_name, sizeof(g_ble_ctx.service_name),"%s",service_name); //safe version of strncpy with null termination/
    //EhsStrcpyUpTo(g_ble_ctx.service_name, service_name, sizeof(g_ble_ctx.service_name) - 1);
    g_ble_ctx.num_chars = num_chars;
    g_ble_ctx.adv_interval_ms = adv_interval_ms;
    g_ble_ctx.component_context = component_context;
    g_ble_ctx.char_configs = char_configs;
    if (callbacks) {
        EhsMemcpy(&g_ble_ctx.callbacks, callbacks, sizeof(inx_ble_service_callbacks_t));
    }
    ble_debug_printf("[BLE_HAL] hal_init: callbacks.on_write=%s on_connect=%s on_disconnect=%s\n",
           g_ble_ctx.callbacks.on_client_write ? "set" : "NULL",
           g_ble_ctx.callbacks.on_connect      ? "set" : "NULL",
           g_ble_ctx.callbacks.on_disconnect   ? "set" : "NULL");

    for (ehs_sint32 i = 0; i < num_chars; i++) {
        ble_debug_printf("[BLE_HAL] hal_init: parsing char[%d] uuid type=%d name=%s props=0x%02x max_len=%d\n",
               i, char_configs[i].uuid.type, char_configs[i].name,
               char_configs[i].properties, char_configs[i].max_len);
        if (INX_PARSE_UUID(&char_configs[i].uuid, &g_ble_ctx.chars[i].uuid) != 0) {
            ble_debug_printf("[BLE_HAL] hal_init: ERROR failed to parse char[%d] UUID\n", i);
            g_ble_hal_state = BLE_HAL_STATE_ERROR;
            return -1;
        }
        g_ble_ctx.chars[i].value_len = 0;
    }

    ble_debug_printf("[BLE_HAL] hal_init: step 1 — nimble_port_init\n");
    esp_err_t err = nimble_port_init();
    if (err != ESP_OK) {
        ble_debug_printf("[BLE_HAL] hal_init: ERROR nimble_port_init failed 0x%x\n", err);
        g_ble_hal_state = BLE_HAL_STATE_ERROR;
        return -1;
    }
    ble_debug_printf("[BLE_HAL] hal_init: step 1 OK\n");

    ble_debug_printf("[BLE_HAL] hal_init: step 2 — setting sync/reset callbacks\n");
    ble_hs_cfg.sync_cb  = ble_on_sync;
    ble_hs_cfg.reset_cb = ble_on_reset;

    ble_debug_printf("[BLE_HAL] hal_init: step 3 — ble_svc_gap_init + ble_svc_gatt_init\n");
    ble_svc_gap_init();
    ble_svc_gatt_init();

    ble_debug_printf("[BLE_HAL] hal_init: step 4 — ble_svc_gap_device_name_set(\"%s\")\n",
           g_ble_ctx.service_name);
    int rc = ble_svc_gap_device_name_set(g_ble_ctx.service_name);
    if (rc != 0) {
        ble_debug_printf("[BLE_HAL] hal_init: WARNING ble_svc_gap_device_name_set failed rc=%d (non-fatal)\n", rc);
    } else {
        ble_debug_printf("[BLE_HAL] hal_init: step 4 OK\n");
    }

    ble_debug_printf("[BLE_HAL] hal_init: step 5 — register_gatt (must be before host task starts)\n");
    rc = register_gatt();
    if (rc != 0) {
        ble_debug_printf("[BLE_HAL] hal_init: ERROR register_gatt failed rc=%d\n", rc);
        g_ble_hal_state = BLE_HAL_STATE_ERROR;
        return -1;
    }
    ble_debug_printf("[BLE_HAL] hal_init: step 5 OK\n");

    ble_debug_printf("[BLE_HAL] hal_init: step 6 — nimble_port_freertos_init (spawning host task)\n");
    nimble_port_freertos_init(ble_host_task);

    g_ble_hal_state = BLE_HAL_STATE_INITIALIZED;
    ble_debug_printf("[BLE_HAL] hal_init: done, state=INITIALIZED, awaiting ble_on_sync\n");
    return 0;
}

/*
 * Register custom GATT services and characteristics.
 *
 * Called from inx_ble_service_hal_init() before nimble_port_freertos_init()
 * so the table is populated before ble_hs_start() → ble_gatts_start() freezes it.
 */
static ehs_sint32 register_gatt(void)
{
    struct ble_gatt_svc_def *gatt_svcs;
    struct ble_gatt_chr_def *gatt_chrs;
    ehs_sint32 rc;

    /* Allocate memory for service and characteristic definitions */
    gatt_svcs = calloc(2, sizeof(struct ble_gatt_svc_def)); /* Service + NULL terminator */
    gatt_chrs = calloc(g_ble_ctx.num_chars + 1,
                      sizeof(struct ble_gatt_chr_def)); /* Chars + NULL terminator */

    ble_debug_printf("[BLE_HAL] register_gatt: num_chars=%d\n", g_ble_ctx.num_chars);

    if (!gatt_svcs || !gatt_chrs) {
        ble_debug_printf("[BLE_HAL] register_gatt: ERROR alloc failed gatt_svcs=%p gatt_chrs=%p\n",
               gatt_svcs, gatt_chrs);
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

        ehs_sint32 props = g_ble_ctx.char_configs[i].properties;
        uint8_t flags = 0;
        if (props & 0x01) flags |= BLE_GATT_CHR_F_READ;
        if (props & 0x02) flags |= BLE_GATT_CHR_F_WRITE;
        if (props & 0x04) flags |= BLE_GATT_CHR_F_NOTIFY;
        if (props & 0x08) flags |= BLE_GATT_CHR_F_INDICATE;
        gatt_chrs[i].flags = flags;
        ble_debug_printf("[BLE_HAL] register_gatt: char[%d] name=%s props=0x%02x nimble_flags=0x%02x\n",
               i, g_ble_ctx.char_configs[i].name, props, flags);
    }

    /* Build service definition */
    gatt_svcs[0].type = BLE_GATT_SVC_TYPE_PRIMARY;
    gatt_svcs[0].uuid = &g_ble_ctx.service_uuid.u;
    gatt_svcs[0].characteristics = gatt_chrs;

    ble_debug_printf("[BLE_HAL] register_gatt: calling ble_gatts_count_cfg\n");
    rc = ble_gatts_count_cfg(gatt_svcs);
    if (rc != 0) {
        ble_debug_printf("[BLE_HAL] register_gatt: ERROR ble_gatts_count_cfg rc=%d\n", rc);
        free(gatt_svcs);
        free(gatt_chrs);
        return -1;
    }

    ble_debug_printf("[BLE_HAL] register_gatt: calling ble_gatts_add_svcs\n");
    rc = ble_gatts_add_svcs(gatt_svcs);
    if (rc != 0) {
        ble_debug_printf("[BLE_HAL] register_gatt: ERROR ble_gatts_add_svcs rc=%d\n", rc);
        free(gatt_svcs);
        free(gatt_chrs);
        return -1;
    }

    /* NOTE: do NOT call ble_gatts_start() here — NimBLE calls it automatically
     * from ble_hs_start() inside nimble_port_run() on the host task. */

    ble_debug_printf("[BLE_HAL] register_gatt: OK, %d characteristics registered\n",
           g_ble_ctx.num_chars);

    /* Note: Don't free gatt_svcs and gatt_chrs as they're still in use by NimBLE */
    return 0;
}

/*
 * Internal: actually program the controller to advertise. Callable only
 * after host sync. Runs either on the EHS thread (if start_adv arrived
 * after sync) or on the nimble_host task (if start_adv arrived before
 * sync and ble_on_sync replays it). NimBLE's GAP API is thread-safe via
 * its event queue.
 */
static ehs_sint32 ble_do_start_adv(void)
{
    struct ble_gap_adv_params adv_params;
    struct ble_hs_adv_fields fields;
    ehs_sint32 rc;
    uint8_t own_addr_type;

    /* Pick a usable own address. ble_hs_id_infer_auto chooses public if
     * the controller has one, otherwise random — matches every IDF
     * NimBLE example. */
    ble_debug_printf("[BLE_HAL] do_start_adv: ble_hs_id_infer_auto\n");
    rc = ble_hs_id_infer_auto(0, &own_addr_type);
    if (rc != 0) {
        ble_debug_printf("[BLE_HAL] do_start_adv: ERROR ble_hs_id_infer_auto rc=%d\n", rc);
        return -1;
    }
    ble_debug_printf("[BLE_HAL] do_start_adv: own_addr_type=%d\n", own_addr_type);

    memset(&fields, 0, sizeof(fields));
    fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;
    fields.name = (uint8_t *)g_ble_ctx.service_name;
    fields.name_len = strlen(g_ble_ctx.service_name);
    fields.name_is_complete = 1;
    fields.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;
    fields.tx_pwr_lvl_is_present = 1;

    ble_debug_printf("[BLE_HAL] do_start_adv: ble_gap_adv_set_fields name=\"%s\"\n",
           g_ble_ctx.service_name);
    rc = ble_gap_adv_set_fields(&fields);
    if (rc != 0) {
        ble_debug_printf("[BLE_HAL] do_start_adv: ERROR ble_gap_adv_set_fields rc=%d\n", rc);
        return -1;
    }

    memset(&adv_params, 0, sizeof(adv_params));
    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;
    adv_params.itvl_min = (g_ble_ctx.adv_interval_ms * 1000) / 625;
    adv_params.itvl_max = (g_ble_ctx.adv_interval_ms * 1000) / 625;

    ble_debug_printf("[BLE_HAL] do_start_adv: ble_gap_adv_start itvl=%lu units (%lu ms)\n",
           (unsigned long)adv_params.itvl_min,
           (unsigned long)g_ble_ctx.adv_interval_ms);
    rc = ble_gap_adv_start(own_addr_type, NULL, BLE_HS_FOREVER,
                           &adv_params, ble_gap_event_handler, NULL);
    if (rc != 0) {
        ble_debug_printf("[BLE_HAL] do_start_adv: ERROR ble_gap_adv_start rc=%d\n", rc);
        return -1;
    }

    g_ble_ctx.advertising = true;
    ble_debug_printf("[BLE_HAL] do_start_adv: advertising started OK\n");
    return 0;
}

/**
 * Start BLE advertising
 *
 * If the host has not yet synced with the controller this call defers —
 * it sets g_ble_ctx.want_adv and ble_on_sync() will start advertising
 * once sync completes. This avoids the race where start_adv runs before
 * sync and ble_gap_adv_start returns BLE_HS_EAGAIN / BLE_HS_EBUSY.
 */
ehs_sint32 inx_ble_service_hal_start_adv(void)
{
    ble_debug_printf("[BLE_HAL] hal_start_adv: state=%d synced=%d advertising=%d\n",
           (int)g_ble_hal_state, g_ble_ctx.host_synced, g_ble_ctx.advertising);

    if (g_ble_hal_state == BLE_HAL_STATE_UNINIT ||
        g_ble_hal_state == BLE_HAL_STATE_ERROR) {
        ble_debug_printf("[BLE_HAL] hal_start_adv: ERROR called before init (state=%d)\n",
               (int)g_ble_hal_state);
        return -1;
    }

    if (g_ble_ctx.advertising) {
        ble_debug_printf("[BLE_HAL] hal_start_adv: already advertising, ignoring\n");
        return -1;
    }

    if (!g_ble_ctx.host_synced) {
        ble_debug_printf("[BLE_HAL] hal_start_adv: host not yet synced, deferring (want_adv=true)\n");
        g_ble_ctx.want_adv = EHS_TRUE;
        return 0;
    }

    return ble_do_start_adv();
}

/**
 * Stop BLE advertising
 */
ehs_sint32 inx_ble_service_hal_stop_adv(void)
{
    ble_debug_printf("[BLE_HAL] hal_stop_adv: advertising=%d\n", g_ble_ctx.advertising);
    if (!g_ble_ctx.advertising) {
        ble_debug_printf("[BLE_HAL] hal_stop_adv: not advertising, ignoring\n");
        return -1;
    }

    ehs_sint32 rc = ble_gap_adv_stop();
    if (rc != 0) {
        ble_debug_printf("[BLE_HAL] hal_stop_adv: ERROR ble_gap_adv_stop rc=%d\n", rc);
        return -1;
    }

    g_ble_ctx.advertising = false;
    ble_debug_printf("[BLE_HAL] hal_stop_adv: advertising stopped\n");
    return 0;
}

/**
 * Write value to a characteristic (local write, not notification)
 */
ehs_sint32 inx_ble_service_hal_write_char(uint8_t char_idx, const char* data, uint16_t length)
{
    ble_debug_printf("[BLE_HAL] hal_write_char: char=%d len=%d\n", char_idx, length);
    if (char_idx >= g_ble_ctx.num_chars) {
        ble_debug_printf("[BLE_HAL] hal_write_char: ERROR bad char_idx=%d\n", char_idx);
        return -1;
    }

    if (length > (uint16_t)g_ble_ctx.char_configs[char_idx].max_len) {
        ble_debug_printf("[BLE_HAL] hal_write_char: ERROR data too long %d (max %d)\n",
               length, g_ble_ctx.char_configs[char_idx].max_len);
        return -2;
    }

    EhsMemcpy(g_ble_ctx.chars[char_idx].value, data, length);
    g_ble_ctx.chars[char_idx].value_len = length;
    ble_debug_printf("[BLE_HAL] hal_write_char: char=%d updated, len=%d\n", char_idx, length);
    return 0;
}

/**
 * Read value from a characteristic
 */
ehs_sint32 inx_ble_service_hal_read_char(uint8_t char_idx, char* data,
                                   uint16_t* length, uint16_t max_len)
{
    if (char_idx >= g_ble_ctx.num_chars) {
        ble_debug_printf("[BLE_HAL] hal_read_char: ERROR bad char_idx=%d\n", char_idx);
        return -1;
    }

    uint16_t copy_len = g_ble_ctx.chars[char_idx].value_len;
    if (copy_len > max_len) copy_len = max_len;

    EhsMemcpy(data, g_ble_ctx.chars[char_idx].value, copy_len);
    *length = copy_len;
    ble_debug_printf("[BLE_HAL] hal_read_char: char=%d read %d bytes\n", char_idx, copy_len);
    return 0;
}

/**
 * Send notification to connected client
 */
ehs_sint32 inx_ble_service_hal_notify(uint8_t char_idx, const char* data, uint16_t length)
{
    ble_debug_printf("[BLE_HAL] hal_notify: char=%d len=%d connected=%d\n",
           char_idx, length, g_ble_ctx.connected);
    if (!g_ble_ctx.connected) {
        ble_debug_printf("[BLE_HAL] hal_notify: not connected\n");
        return -1;
    }

    if (char_idx >= g_ble_ctx.num_chars) {
        ble_debug_printf("[BLE_HAL] hal_notify: ERROR bad char_idx=%d\n", char_idx);
        return -2;
    }

    if (!(g_ble_ctx.char_configs[char_idx].properties & 0x04)) {
        ble_debug_printf("[BLE_HAL] hal_notify: ERROR char=%d props=0x%02x does not support notify\n",
               char_idx, g_ble_ctx.char_configs[char_idx].properties);
        return -3;
    }

    if (length > (uint16_t)g_ble_ctx.char_configs[char_idx].max_len) {
        ble_debug_printf("[BLE_HAL] hal_notify: clamping len %d -> %d\n",
               length, g_ble_ctx.char_configs[char_idx].max_len);
        length = (uint16_t)g_ble_ctx.char_configs[char_idx].max_len;
    }
    EhsMemcpy(g_ble_ctx.chars[char_idx].value, data, length);
    g_ble_ctx.chars[char_idx].value_len = length;

    ble_debug_printf("[BLE_HAL] hal_notify: ble_hs_mbuf_from_flat len=%d\n", length);
    struct os_mbuf *om = ble_hs_mbuf_from_flat(data, length);
    if (om == NULL) {
        ble_debug_printf("[BLE_HAL] hal_notify: ERROR mbuf alloc failed\n");
        return -4;
    }

    ble_debug_printf("[BLE_HAL] hal_notify: ble_gattc_notify_custom conn=%d val_handle=%d\n",
           g_ble_ctx.conn_handle, g_ble_ctx.chars[char_idx].value_handle);
    ehs_sint32 rc = ble_gattc_notify_custom(g_ble_ctx.conn_handle,
                                     g_ble_ctx.chars[char_idx].value_handle, om);
    if (rc != 0) {
        ble_debug_printf("[BLE_HAL] hal_notify: ERROR ble_gattc_notify_custom rc=%d\n", rc);
        return -5;
    }

    ble_debug_printf("[BLE_HAL] hal_notify: notification sent OK char=%d\n", char_idx);
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
 *
 * Lifecycle policy: init-once, stay-initialised.
 *
 * This is intentionally a no-op for now. We do not tear down the
 * NimBLE stack on FB destroy / app reload because:
 *   - It is undefined whether teardown + re-init across application
 *     transitions is the desired behaviour, vs leaving BLE up so
 *     advertising / GATT remain available between apps.
 *   - We have not yet seen a path that legitimately wants a teardown.
 *
 * The full-teardown helper (ble_full_teardown) is preserved in source
 * for the day this policy is revisited. See
 * docs/ert-porting-guide.md § BLE Subsystem › "Lifecycle policy —
 * open questions" for the unresolved design points.
 */
void inx_ble_service_hal_deinit(void)
{
    (void)ble_full_teardown;  /* keep symbol referenced */
    ble_debug_printf("[BLE_HAL] hal_deinit: no-op (stay-initialised policy), state=%d\n",
           (int)g_ble_hal_state);
}

