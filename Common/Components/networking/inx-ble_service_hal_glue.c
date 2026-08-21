/***************************************************************
* Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license.
****************************************************************/

/*
 * BLE service HAL glue — platform-selection layer.
 *
 * Owns nothing but the #ifdef dispatch to the target HAL.
 * No EHS types, no FB state, no component headers.
 */

#include "inx-ble_service_hal_glue.h"

#include "ble_service_nimble.h"

ehs_sint32 inx_ble_service_hal_glue_init(
    const inx_ble_uuid_t*              service_uuid,
    const char*                        service_name,
    ehs_uint8                          num_chars,
    ehs_uint32                         adv_interval_ms,
    const inx_ble_char_config_t*       chars,
    const inx_ble_service_callbacks_t* callbacks,
    void*                              ctx)
{
    return inx_ble_service_hal_init(
        service_uuid, service_name, num_chars,
        adv_interval_ms,
        (inx_ble_char_config_t*)chars,
        (inx_ble_service_callbacks_t*)callbacks,
        ctx);
}

ehs_sint32 inx_ble_service_hal_glue_start_adv(void)
{
    return inx_ble_service_hal_start_adv();
}

ehs_sint32 inx_ble_service_hal_glue_stop_adv(void)
{
    return inx_ble_service_hal_stop_adv();
}

ehs_sint32 inx_ble_service_hal_glue_write_char(ehs_uint8 char_idx, const char* data, ehs_uint16 length)
{
    return inx_ble_service_hal_write_char(char_idx, data, length);
}

ehs_sint32 inx_ble_service_hal_glue_read_char(ehs_uint8 char_idx, char* data,
                                               ehs_uint16* length, ehs_uint16 max_len)
{
    return inx_ble_service_hal_read_char(char_idx, data, length, max_len);
}

ehs_sint32 inx_ble_service_hal_glue_notify(ehs_uint8 char_idx, const char* data, ehs_uint16 length)
{
    return inx_ble_service_hal_notify(char_idx, data, length);
}

void inx_ble_service_hal_glue_deinit(void)
{
    inx_ble_service_hal_deinit();
}
