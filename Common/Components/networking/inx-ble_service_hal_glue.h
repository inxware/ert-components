/***************************************************************
* Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license.
****************************************************************/

#ifndef INX_BLE_SERVICE_HAL_GLUE_H
#define INX_BLE_SERVICE_HAL_GLUE_H

#include "hal_ble.h"

/* ---------------------------------------------------------------
 * BLE service HAL glue — platform-selection layer.
 *
 * Callers pass all parameters explicitly; no EHS or FB types
 * appear here so this API can be used from any context (function
 * block, bare-metal driver, test harness, etc.).
 *
 * ctx is an opaque pointer forwarded as-is to every callback.
 * --------------------------------------------------------------- */

ehs_sint32 inx_ble_service_hal_glue_init(
    const inx_ble_uuid_t*              service_uuid,
    const char*                        service_name,
    ehs_uint8                          num_chars,
    ehs_uint32                         adv_interval_ms,
    const inx_ble_char_config_t*       chars,
    const inx_ble_service_callbacks_t* callbacks,
    void*                              ctx);

ehs_sint32 inx_ble_service_hal_glue_start_adv(void);
ehs_sint32 inx_ble_service_hal_glue_stop_adv(void);
ehs_sint32 inx_ble_service_hal_glue_write_char(ehs_uint8 char_idx, const char* data, ehs_uint16 length);
ehs_sint32 inx_ble_service_hal_glue_read_char(ehs_uint8 char_idx, char* data, ehs_uint16* length, ehs_uint16 max_len);
ehs_sint32 inx_ble_service_hal_glue_notify(ehs_uint8 char_idx, const char* data, ehs_uint16 length);
void       inx_ble_service_hal_glue_deinit(void);

#endif /* INX_BLE_SERVICE_HAL_GLUE_H */
