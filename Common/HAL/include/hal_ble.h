/***************************************************************
* Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license.
****************************************************************/

#ifndef EHS_HAL_BLE_H
#define EHS_HAL_BLE_H

#include "globals.h"

/* ---------------------------------------------------------------
 * Binary UUID
 *
 * Bytes in u128 are in network (big-endian) order, matching the
 * standard "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX" string.
 * HAL backends that need little-endian wire order (e.g. NimBLE)
 * must reverse the bytes on use.
 * --------------------------------------------------------------- */
#define INX_BLE_UUID_TYPE_UNSET  0
#define INX_BLE_UUID_TYPE_16     1   /* 16-bit Bluetooth SIG short UUID */
#define INX_BLE_UUID_TYPE_128    2   /* full 128-bit custom UUID */

typedef struct {
    ehs_uint8 type;          /* INX_BLE_UUID_TYPE_* */
    union {
        ehs_uint16 u16;
        ehs_uint8  u128[16];
    } value;
} inx_ble_uuid_t;

/* ---------------------------------------------------------------
 * Characteristic configuration
 *
 * Used by BLE service components to describe each GATT
 * characteristic. Lives here so any BLE component (peripheral,
 * central, scanner) can share the same type without depending on
 * a specific function-block header.
 * --------------------------------------------------------------- */
#define INX_BLE_NAME_MAX  32   /* max length of a BLE service or characteristic name */

typedef struct {
    inx_ble_uuid_t uuid;
    ehs_char       name[INX_BLE_NAME_MAX + 1];
    ehs_sint32     properties;  /* bitmask: Read=1, Write=2, Notify=4, Indicate=8 */
    ehs_sint32     max_len;
} inx_ble_char_config_t;

/* ---------------------------------------------------------------
 * Event callbacks
 *
 * Provided by the caller of inx_ble_service_hal_glue_init().
 * ctx is the opaque pointer passed to that call — the HAL and
 * glue never dereference it; it is forwarded as-is so the caller
 * can recover its own context (e.g. cast to EhsFunctionInstanceDataType*).
 * --------------------------------------------------------------- */
typedef struct {
    void (*on_client_write)(void* ctx, ehs_uint8 char_idx, const char* data, ehs_uint16 length);
    void (*on_connect)     (void* ctx, ehs_uint16 conn_handle);
    void (*on_disconnect)  (void* ctx, ehs_uint16 conn_handle, ehs_uint8 reason);
} inx_ble_service_callbacks_t;

#endif /* EHS_HAL_BLE_H */
