/***************************************************************
* Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license.
****************************************************************/

/**
 * BLE Service HAL Glue Layer
 *
 * This file provides the glue between the BLE service component
 * and the platform-specific HAL implementation.
 * 
 * It handles async events from ble HAL implementation by calls the outout macros
 * to trigger internal port functions.
 * Usually this is built into the actual component C file, but doing it separately here.
 */

 //TODO move to Common/HAL/

#include "inx-ble_service.h"
#include "inx-parameters.h"
#include "inx-component.h"

//#if defined(nimble)
#include "ble_service_nimble.h"
//#else
//#include "ble_service_stubbed.h"
//#endif

#include <string.h>
#include <stdio.h>

/* Forward declarations of internal port functions */
extern EHS_FB_RUN_FUNCTION(ble_service_on_client_write);
extern EHS_FB_RUN_FUNCTION(ble_service_on_connect);
extern EHS_FB_RUN_FUNCTION(ble_service_on_disconnect);

/**
 * Callback from HAL when client writes to a characteristic
 */
static void hal_on_client_write_callback(void* ctx, ehs_uint8 char_idx,
                                         const char* data, ehs_uint16 length)
{
    EhsFunctionInstanceDataType* pFIdata = (EhsFunctionInstanceDataType*)ctx;
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)pFIdata->pObjData;

    /* Store callback data in state */
    state->cb_char_idx = char_idx;
    state->cb_data_len = length < sizeof(state->cb_data) ? length : sizeof(state->cb_data);
    if (data && state->cb_data_len > 0) {
        memcpy(state->cb_data, data, state->cb_data_len);
    }

    /* Call the internal port function which will populate outputs and trigger event */
    EhsRunble_service_on_client_write(pFIdata); //dodo - is their a MACRO for this that is more commonly called?
}

/**
 * Callback from HAL when client connects
 */
static void hal_on_connect_callback(void* ctx, ehs_uint16 conn_handle)
{
    EhsFunctionInstanceDataType* pFIdata = (EhsFunctionInstanceDataType*)ctx;
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)pFIdata->pObjData;

    /* Store callback data in state */
    state->cb_conn_handle = conn_handle;

    /* Call the internal port function which will populate outputs and trigger event */
    EhsRunble_service_on_connect(pFIdata);
}

/**
 * Callback from HAL when client disconnects
 */
static void hal_on_disconnect_callback(void* ctx, ehs_uint16 conn_handle, ehs_uint8 reason)
{
    EhsFunctionInstanceDataType* pFIdata = (EhsFunctionInstanceDataType*)ctx;
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)pFIdata->pObjData;

    /* Store callback data in state */
    state->cb_conn_handle = conn_handle;
    state->cb_reason = reason;

    /* Call the internal port function which will populate outputs and trigger event */
    EhsRunble_service_on_disconnect(pFIdata);
}

/**
 * Initialize BLE service HAL
 * Called from the component's init function
 */
ehs_sint32 inx_ble_service_hal_glue_init(void* context, void* state_ptr)
{
    printf("Test Called %s\n", __func__);
#if 1
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)state_ptr;
    inx_ble_char_config_t char_configs[16];
    inx_ble_service_callbacks_t callbacks = {0};

    /* Build characteristic configurations from parameters */
    ehs_sint32 num_chars = state->num_chars;
    if (num_chars > 16) {
        num_chars = 16;
    }

    /* Configure characteristic 0 */
    if (num_chars > 0) {
        char_configs[0].uuid = state->char_0_uuid;
        char_configs[0].name = state->char_0_name;
        char_configs[0].properties = (ehs_uint8)state->char_0_props;
        char_configs[0].max_len = (ehs_uint16)state->char_0_max_len;
    }

    /* Configure characteristic 1 */
    if (num_chars > 1) {
        char_configs[1].uuid = state->char_1_uuid;
        char_configs[1].name = state->char_1_name;
        char_configs[1].properties = (ehs_uint8)state->char_1_props;
        char_configs[1].max_len = (ehs_uint16)state->char_1_max_len;
    }

    /* Configure characteristic 2 */
    if (num_chars > 2) {
        char_configs[2].uuid = state->char_2_uuid;
        char_configs[2].name = state->char_2_name;
        char_configs[2].properties = (ehs_uint8)state->char_2_props;
        char_configs[2].max_len = (ehs_uint16)state->char_2_max_len;
    }

    /* Configure characteristic 3 */
    if (num_chars > 3) {
        char_configs[3].uuid = state->char_3_uuid;
        char_configs[3].name = state->char_3_name;
        char_configs[3].properties = (ehs_uint8)state->char_3_props;
        char_configs[3].max_len = (ehs_uint16)state->char_3_max_len;
    }

    /* Set up callbacks */
    callbacks.on_client_write = hal_on_client_write_callback;
    callbacks.on_connect = hal_on_connect_callback;
    callbacks.on_disconnect = hal_on_disconnect_callback;

    /* Initialize HAL */
    ehs_sint32 rc = inx_ble_service_hal_init(
        state->service_uuid,
        state->service_name,
        (ehs_uint8)num_chars,
        (ehs_uint32)state->adv_interval_ms,
        (ehs_uint16)state->mtu_size,
        char_configs,
        &callbacks,
        (void*)context);

    if (rc != 0) {
        return rc;
    }

    /* Register GATT service */
    rc = inx_ble_service_hal_register_gatt();
    return rc;

#else
    /* Platform not supported */
    return -1;
#endif
}

/**
 * Start advertising
 */
ehs_sint32 inx_ble_service_hal_glue_start_adv(void)
{
    printf("Test Called %s\n", __func__);
#ifdef EHS_ESP32
    return inx_ble_service_hal_start_adv();
#else
    return -1;
#endif
}

/**
 * Stop advertising
 */
ehs_sint32 inx_ble_service_hal_glue_stop_adv(void)
{
    printf("Test Called %s\n", __func__);
#ifdef EHS_ESP32
    return inx_ble_service_hal_stop_adv();
#else
    return -1;
#endif
}


/**
 * Write to characteristic
 */
ehs_sint32 inx_ble_service_hal_glue_write_char(ehs_uint8 char_idx, const char* data, ehs_uint16 length)
{
#ifdef EHS_ESP32
    return inx_ble_service_hal_write_char(char_idx, data, length);
#else
    return -1;
#endif
}

/**
 * Read from characteristic
 */
ehs_sint32 inx_ble_service_hal_glue_read_char(ehs_uint8 char_idx, char* data,
                                       ehs_uint16* length, ehs_uint16 max_len)
{
#ifdef EHS_ESP32
    return inx_ble_service_hal_read_char(char_idx, data, length, max_len);
#else
    return -1;
#endif
}

/**
 * Send notification
 */
ehs_sint32 inx_ble_service_hal_glue_notify(ehs_uint8 char_idx, const char* data, ehs_uint16 length)
{
#ifdef EHS_ESP32
    return inx_ble_service_hal_notify(char_idx, data, length);
#else
    return -1;
#endif
}

/**
 * Deinitialize HAL
 */
void inx_ble_service_hal_glue_deinit(void)
{
#ifdef EHS_ESP32
    inx_ble_service_hal_deinit();
#endif
}
