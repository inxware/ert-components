/***************************************************************
* Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license.
****************************************************************/

#ifndef BLE_SERVICE_NIMBLE_H
#define BLE_SERVICE_NIMBLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "globals.h"
/**
 * Characteristic configuration structure
 */
typedef struct {
    const char* uuid;        /* UUID string (16-bit or 128-bit) */
    const char* name;        /* Characteristic name */
    ehs_uint8 properties;      /* Properties: Read=1, Write=2, Notify=4, Indicate=8 */
    ehs_uint16 max_len;        /* Maximum data length */
} inx_ble_char_config_t;

/**
 * Callback function types for internal events
 */
typedef void (*inx_ble_on_client_write_cb)(void* ctx, ehs_uint8 char_idx,
                                           const char* data, ehs_uint16 length);
typedef void (*inx_ble_on_connect_cb)(void* ctx, ehs_uint16 conn_handle);
typedef void (*inx_ble_on_disconnect_cb)(void* ctx, ehs_uint16 conn_handle, ehs_uint8 reason);

/**
 * Callback structure for internal port events
 */
typedef struct {
    inx_ble_on_client_write_cb on_client_write;
    inx_ble_on_connect_cb on_connect;
    inx_ble_on_disconnect_cb on_disconnect;
} inx_ble_service_callbacks_t;

/**
 * Initialize BLE service with configuration
 *
 * @param service_uuid Service UUID string
 * @param service_name Service name (used in advertising)
 * @param num_chars Number of characteristics (1-16)
 * @param adv_interval_ms Advertising interval in milliseconds
 * @param mtu_size MTU size (23-512 bytes)
 * @param char_configs Array of characteristic configurations
 * @param callbacks Callback functions for internal events
 * @param component_context Component context pointer (passed to callbacks)
 * @return 0 on success, negative on error
 */
int inx_ble_service_hal_init(
    const char* service_uuid,
    const char* service_name,
    ehs_uint8 num_chars,
    ehs_uint32 adv_interval_ms,
    ehs_uint16 mtu_size,
    inx_ble_char_config_t* char_configs,
    inx_ble_service_callbacks_t* callbacks,
    void* component_context);

/**
 * Register GATT service and characteristics
 * Must be called after init and before start_adv
 *
 * @return 0 on success, negative on error
 */
int inx_ble_service_hal_register_gatt(void);

/**
 * Start BLE advertising
 *
 * @return 0 on success, negative on error
 */
int inx_ble_service_hal_start_adv(void);

/**
 * Stop BLE advertising
 *
 * @return 0 on success, negative on error
 */
int inx_ble_service_hal_stop_adv(void);

/**
 * Write value to a characteristic (local write)
 *
 * @param char_idx Characteristic index (0-based)
 * @param data Data buffer
 * @param length Data length
 * @return 0 on success, negative on error
 */
int inx_ble_service_hal_write_char(ehs_uint8 char_idx, const char* data, ehs_uint16 length);

/**
 * Read value from a characteristic
 *
 * @param char_idx Characteristic index (0-based)
 * @param data Output buffer
 * @param length Output: bytes read
 * @param max_len Maximum buffer size
 * @return 0 on success, negative on error
 */
int inx_ble_service_hal_read_char(ehs_uint8 char_idx, char* data,
                                   ehs_uint16* length, ehs_uint16 max_len);

/**
 * Send notification to connected client
 *
 * @param char_idx Characteristic index (0-based)
 * @param data Data buffer
 * @param length Data length
 * @return 0 on success, negative on error
 */
int inx_ble_service_hal_notify(ehs_uint8 char_idx, const char* data, ehs_uint16 length);

/**
 * Check if a client is connected
 *
 * @return true if connected, false otherwise
 */
ehs_bool inx_ble_service_hal_is_connected(void);

/**
 * Check if advertising is active
 *
 * @return true if advertising, false otherwise
 */
ehs_bool inx_ble_service_hal_is_advertising(void);

/**
 * Deinitialize BLE service
 */
void inx_ble_service_hal_deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* BLE_SERVICE_NIMBLE_H */
