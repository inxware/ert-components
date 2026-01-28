/***************************************************************
* Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license.
****************************************************************/

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-ble_service.h"
//ICB HEADER MACRO END -- DO NOT ALTER

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
#ifndef INX_BLE_SERVICE_STATE_TYPE_DEFINED
#define INX_BLE_SERVICE_STATE_TYPE_DEFINED
typedef struct
{
    /* Parameters */
    ehs_char* service_uuid; // 128-bit UUID for the BLE service (format: XXXXXXXX
    ehs_char* service_name; // Human-readable name for the service
    ehs_sint32 num_chars; // Number of characteristics in this service (1-16)
    ehs_sint32 adv_interval_ms; // Advertising interval in milliseconds (20-10240ms).
    ehs_sint32 mtu_size; // Maximum Transmission Unit size in bytes (23-512).
    ehs_char* char_0_uuid; // UUID for characteristic 0
    ehs_char* char_0_name; // Name for characteristic 0
    ehs_sint32 char_0_props; // Properties for char 0: 1=Read, 2=Write, 4=Notify,
    ehs_sint32 char_0_max_len; // Maximum data length for characteristic 0
    ehs_char* char_1_uuid; // UUID for characteristic 1
    ehs_char* char_1_name; // Name for characteristic 1
    ehs_sint32 char_1_props; // Properties for char 1: 1=Read, 2=Write, 4=Notify,
    ehs_sint32 char_1_max_len; // Maximum data length for characteristic 1
    ehs_char* char_2_uuid; // UUID for characteristic 2
    ehs_char* char_2_name; // Name for characteristic 2
    ehs_sint32 char_2_props; // Properties for char 2: 1=Read, 2=Write, 4=Notify,
    ehs_sint32 char_2_max_len; // Maximum data length for characteristic 2
    ehs_char* char_3_uuid; // UUID for characteristic 3
    ehs_char* char_3_name; // Name for characteristic 3
    ehs_sint32 char_3_props; // Properties for char 3: 1=Read, 2=Write, 4=Notify,
    ehs_sint32 char_3_max_len; // Maximum data length for characteristic 3

    /* Callback data storage */
    ehs_uint8 cb_char_idx;
    ehs_char cb_data[512];
    ehs_uint16 cb_data_len;
    ehs_uint16 cb_conn_handle;
    ehs_uint8 cb_reason;
} inx_ble_service_state_type;
#endif
//ICB STATE VAR MACRO END -- DO NOT ALTER

/* Include HAL glue layer functions */
#include "inx-ble_service_hal_glue.h"

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(ble_service)

EHS_FB_FUNCTION_ENTRY("init", 0x1, ble_service_init)
EHS_FB_FUNCTION_ENTRY("start_adv", 0x2, ble_service_start_adv)
EHS_FB_FUNCTION_ENTRY("stop_adv", 0x3, ble_service_stop_adv)
EHS_FB_FUNCTION_ENTRY("write_char", 0x4, ble_service_write_char)
EHS_FB_FUNCTION_ENTRY("read_char", 0x5, ble_service_read_char)
EHS_FB_FUNCTION_ENTRY("notify_char", 0x6, ble_service_notify_char)
EHS_FB_FUNCTION_ENTRY("on_client_write", 0x7, ble_service_on_client_write) // Internal event
EHS_FB_FUNCTION_ENTRY("on_connect", 0x8, ble_service_on_connect) // Internal event
EHS_FB_FUNCTION_ENTRY("on_disconnect", 0x9, ble_service_on_disconnect) // Internal event

EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER

//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */

/* init */
#define INX_ble_service_ARG_init__DO 1
#define INX_ble_service_ARG_init__EO 1
#define INX_ble_service_ARG_init__EO 2

/* start_adv */
#define INX_ble_service_ARG_start_adv__EO 1
#define INX_ble_service_ARG_start_adv__EO 2

/* stop_adv */
#define INX_ble_service_ARG_stop_adv__EO 1

/* write_char */
#define INX_ble_service_ARG_write_char__DI 1
#define INX_ble_service_ARG_write_char__DI 2
#define INX_ble_service_ARG_write_char__DI 3
#define INX_ble_service_ARG_write_char__DO 1
#define INX_ble_service_ARG_write_char__EO 1
#define INX_ble_service_ARG_write_char__EO 2

/* read_char */
#define INX_ble_service_ARG_read_char__DI 1
#define INX_ble_service_ARG_read_char__DO 1
#define INX_ble_service_ARG_read_char__DO 2
#define INX_ble_service_ARG_read_char__DO 3
#define INX_ble_service_ARG_read_char__EO 1
#define INX_ble_service_ARG_read_char__EO 2

/* notify_char */
#define INX_ble_service_ARG_notify_char__DI 1
#define INX_ble_service_ARG_notify_char__DI 2
#define INX_ble_service_ARG_notify_char__DI 3
#define INX_ble_service_ARG_notify_char__DO 1
#define INX_ble_service_ARG_notify_char__EO 1
#define INX_ble_service_ARG_notify_char__EO 2

/* on_client_write */
#define INX_ble_service_ARG_on_client_write__DO 1
#define INX_ble_service_ARG_on_client_write__DO 2
#define INX_ble_service_ARG_on_client_write__DO 3
#define INX_ble_service_ARG_on_client_write__EO 1

/* on_connect */
#define INX_ble_service_ARG_on_connect__DO 1
#define INX_ble_service_ARG_on_connect__EO 1

/* on_disconnect */
#define INX_ble_service_ARG_on_disconnect__DO 1
#define INX_ble_service_ARG_on_disconnect__DO 2
#define INX_ble_service_ARG_on_disconnect__EO 1

//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER

//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(ble_service)
{
    printf("Test Called %s\n", __func__);
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_ble_service_state_type);
}
//ICB IDENTIFY FUNCTION MACRO END -- DO NOT ALTER

//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ble_service)
{
    printf("Test Called %s\n", __func__);
    ehs_bool bRet = EHS_TRUE; /* assume success */
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)EHS_FB_INIT_CONTEXT;

    /* Parse initialization parameters using incremental parser functions */
    const char* pParams = EHS_FB_INIT_PARAMETERS;
    pParams = EhsGetWordFromString(&(state->service_uuid), pParams); // 128-bit UUID for the BLE service (format
    pParams = EhsGetWordFromString(&(state->service_name), pParams); // Human-readable name for the service
    pParams = EhsGetSint32FromString(&(state->num_chars), pParams); // Number of characteristics in this servic
    pParams = EhsGetSint32FromString(&(state->adv_interval_ms), pParams); // Advertising interval in milliseconds (20
    pParams = EhsGetSint32FromString(&(state->mtu_size), pParams); // Maximum Transmission Unit size in bytes 
    pParams = EhsGetWordFromString(&(state->char_0_uuid), pParams); // UUID for characteristic 0
    pParams = EhsGetWordFromString(&(state->char_0_name), pParams); // Name for characteristic 0
    pParams = EhsGetSint32FromString(&(state->char_0_props), pParams); // Properties for char 0: 1=Read, 2=Write, 
    pParams = EhsGetSint32FromString(&(state->char_0_max_len), pParams); // Maximum data length for characteristic 0
    pParams = EhsGetWordFromString(&(state->char_1_uuid), pParams); // UUID for characteristic 1
    pParams = EhsGetWordFromString(&(state->char_1_name), pParams); // Name for characteristic 1
    pParams = EhsGetSint32FromString(&(state->char_1_props), pParams); // Properties for char 1: 1=Read, 2=Write, 
    pParams = EhsGetSint32FromString(&(state->char_1_max_len), pParams); // Maximum data length for characteristic 1
    pParams = EhsGetWordFromString(&(state->char_2_uuid), pParams); // UUID for characteristic 2
    pParams = EhsGetWordFromString(&(state->char_2_name), pParams); // Name for characteristic 2
    pParams = EhsGetSint32FromString(&(state->char_2_props), pParams); // Properties for char 2: 1=Read, 2=Write, 
    pParams = EhsGetSint32FromString(&(state->char_2_max_len), pParams); // Maximum data length for characteristic 2
    pParams = EhsGetWordFromString(&(state->char_3_uuid), pParams); // UUID for characteristic 3
    pParams = EhsGetWordFromString(&(state->char_3_name), pParams); // Name for characteristic 3
    pParams = EhsGetSint32FromString(&(state->char_3_props), pParams); // Properties for char 3: 1=Read, 2=Write, 
    pParams = EhsGetSint32FromString(&(state->char_3_max_len), pParams); // Maximum data length for characteristic 3

    /* Add initialization code here */

    return bRet;
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER

//ICB FUNCTION init MACRO START -- DO NOT ALTER
/**
 * Definition of ble_service_init.
 * This function can access the object data using:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ble_service_init)
{
    printf("Test Called %s\n", __func__);
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)EHS_FB_RUN_CONTEXT;

    /* Initialize BLE service HAL */
    int rc = inx_ble_service_hal_glue_init(EHS_FB_RUN_CONTEXT, state);

    /* Write status output port */
    if (EHS_FB_OUT_CONNECTED_API2(INX_ble_service_ARG_init__DO))
    {
        EHS_FB_OUT_I_API2(INX_ble_service_ARG_init__DO) = rc;
    }

    /* Trigger appropriate finish event */
    if (rc == 0) {
        EHS_FB_FINISH(INX_ble_service_ARG_init__EO);  /* init_done */
    } else {
        EHS_FB_FINISH(INX_ble_service_ARG_init__EO + 1);  /* init_error */
    }
}
//ICB FUNCTION init MACRO END -- DO NOT ALTER

//ICB FUNCTION start_adv MACRO START -- DO NOT ALTER
/**
 * Definition of ble_service_start_adv.
 * This function can access the object data using:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ble_service_start_adv)
{
    printf("Test Called %s\n", __func__);
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)EHS_FB_RUN_CONTEXT;

    /* Start BLE advertising */
    int rc = inx_ble_service_hal_glue_start_adv();

    /* Trigger appropriate finish event */
    if (rc == 0) {
        EHS_FB_FINISH(INX_ble_service_ARG_start_adv__EO);  /* adv_started */
    } else {
        EHS_FB_FINISH(INX_ble_service_ARG_start_adv__EO + 1);  /* adv_error */
    }
}
//ICB FUNCTION start_adv MACRO END -- DO NOT ALTER

//ICB FUNCTION stop_adv MACRO START -- DO NOT ALTER
/**
 * Definition of ble_service_stop_adv.
 * This function can access the object data using:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ble_service_stop_adv)
{
    printf("Test Called %s\n", __func__);
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)EHS_FB_RUN_CONTEXT;

    /* Stop BLE advertising */
    int rc = inx_ble_service_hal_glue_stop_adv();

    /* Trigger finish event (only adv_stopped, no error event in CDF) */
    EHS_FB_FINISH(INX_ble_service_ARG_stop_adv__EO);  /* adv_stopped */
}
//ICB FUNCTION stop_adv MACRO END -- DO NOT ALTER

//ICB FUNCTION write_char MACRO START -- DO NOT ALTER
/**
 * Definition of ble_service_write_char.
 * This function can access the object data using:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ble_service_write_char)
{
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)EHS_FB_RUN_CONTEXT;

    /* Read input ports */
    uint8_t char_idx = 0;
    const char* data = NULL;
    uint16_t length = 0;

    if (EHS_FB_IN_CONNECTED_API2(INX_ble_service_ARG_write_char__DI))
    {
        char_idx = (uint8_t)EHS_FB_IN_I_API2(INX_ble_service_ARG_write_char__DI);
    }

    if (EHS_FB_IN_CONNECTED_API2(INX_ble_service_ARG_write_char__DI + 1))
    {
        data = EHS_FB_IN_S_API2(INX_ble_service_ARG_write_char__DI + 1);
    }

    if (EHS_FB_IN_CONNECTED_API2(INX_ble_service_ARG_write_char__DI + 2))
    {
        length = (uint16_t)EHS_FB_IN_I_API2(INX_ble_service_ARG_write_char__DI + 2);
    }

    /* Write to characteristic */
    int rc = inx_ble_service_hal_glue_write_char(char_idx, data, length);

    /* Write status output port */
    if (EHS_FB_OUT_CONNECTED_API2(INX_ble_service_ARG_write_char__DO))
    {
        EHS_FB_OUT_I_API2(INX_ble_service_ARG_write_char__DO) = rc;
    }

    /* Trigger appropriate finish event */
    if (rc == 0) {
        EHS_FB_FINISH(INX_ble_service_ARG_write_char__EO);  /* write_done */
    } else {
        EHS_FB_FINISH(INX_ble_service_ARG_write_char__EO + 1);  /* write_error */
    }
}
//ICB FUNCTION write_char MACRO END -- DO NOT ALTER

//ICB FUNCTION read_char MACRO START -- DO NOT ALTER
/**
 * Definition of ble_service_read_char.
 * This function can access the object data using:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ble_service_read_char)
{
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)EHS_FB_RUN_CONTEXT;

    /* Read input port */
    uint8_t char_idx = 0;
    if (EHS_FB_IN_CONNECTED_API2(INX_ble_service_ARG_read_char__DI))
    {
        char_idx = (uint8_t)EHS_FB_IN_I_API2(INX_ble_service_ARG_read_char__DI);
    }

    /* Read from characteristic */
    char read_buffer[512];
    uint16_t read_length = 0;
    int rc = inx_ble_service_hal_glue_read_char(char_idx, read_buffer, &read_length, sizeof(read_buffer));

    /* Write output ports */
    if (EHS_FB_OUT_CONNECTED_API2(INX_ble_service_ARG_read_char__DO))
    {
        /* read_data */
        char* out_str = EHS_FB_OUT_S_API2(INX_ble_service_ARG_read_char__DO);
        if (out_str && rc == 0) {
            memcpy(out_str, read_buffer, read_length);
            out_str[read_length] = '\0';
        }
    }

    if (EHS_FB_OUT_CONNECTED_API2(INX_ble_service_ARG_read_char__DO + 1))
    {
        /* read_length */
        EHS_FB_OUT_I_API2(INX_ble_service_ARG_read_char__DO + 1) = read_length;
    }

    if (EHS_FB_OUT_CONNECTED_API2(INX_ble_service_ARG_read_char__DO + 2))
    {
        /* read_status */
        EHS_FB_OUT_I_API2(INX_ble_service_ARG_read_char__DO + 2) = rc;
    }

    /* Trigger appropriate finish event */
    if (rc == 0) {
        EHS_FB_FINISH(INX_ble_service_ARG_read_char__EO);  /* read_done */
    } else {
        EHS_FB_FINISH(INX_ble_service_ARG_read_char__EO + 1);  /* read_error */
    }
}
//ICB FUNCTION read_char MACRO END -- DO NOT ALTER

//ICB FUNCTION notify_char MACRO START -- DO NOT ALTER
/**
 * Definition of ble_service_notify_char.
 * This function can access the object data using:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ble_service_notify_char)
{
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)EHS_FB_RUN_CONTEXT;

    /* Read input ports */
    uint8_t char_idx = 0;
    const char* data = NULL;
    uint16_t length = 0;

    if (EHS_FB_IN_CONNECTED_API2(INX_ble_service_ARG_notify_char__DI))
    {
        char_idx = (uint8_t)EHS_FB_IN_I_API2(INX_ble_service_ARG_notify_char__DI);
    }

    if (EHS_FB_IN_CONNECTED_API2(INX_ble_service_ARG_notify_char__DI + 1))
    {
        data = EHS_FB_IN_S_API2(INX_ble_service_ARG_notify_char__DI + 1);
    }

    if (EHS_FB_IN_CONNECTED_API2(INX_ble_service_ARG_notify_char__DI + 2))
    {
        length = (uint16_t)EHS_FB_IN_I_API2(INX_ble_service_ARG_notify_char__DI + 2);
    }

    /* Send notification */
    int rc = inx_ble_service_hal_glue_notify(char_idx, data, length);

    /* Write status output port */
    if (EHS_FB_OUT_CONNECTED_API2(INX_ble_service_ARG_notify_char__DO))
    {
        EHS_FB_OUT_I_API2(INX_ble_service_ARG_notify_char__DO) = rc;
    }

    /* Trigger appropriate finish event */
    if (rc == 0) {
        EHS_FB_FINISH(INX_ble_service_ARG_notify_char__EO);  /* notify_sent */
    } else {
        EHS_FB_FINISH(INX_ble_service_ARG_notify_char__EO + 1);  /* notify_error */
    }
}
//ICB FUNCTION notify_char MACRO END -- DO NOT ALTER

//ICB FUNCTION on_client_write MACRO START -- DO NOT ALTER
/**
 * Definition of ble_service_on_client_write.
 * INTERNAL EVENT: This function can be called from external sources (ISR, threads, etc.)
 * This function can access the object data using:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ble_service_on_client_write)
{
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)EHS_FB_RUN_CONTEXT;

    /* Populate output ports from callback data stored in state */
    if (EHS_FB_OUT_CONNECTED_API2(INX_ble_service_ARG_on_client_write__DO)) {
        EHS_FB_OUT_I_API2(INX_ble_service_ARG_on_client_write__DO) = (ehs_sint32)state->cb_char_idx;
    }

    if (EHS_FB_OUT_CONNECTED_API2(INX_ble_service_ARG_on_client_write__DO + 1)) {
        ehs_char* out_str = EHS_FB_OUT_S_API2(INX_ble_service_ARG_on_client_write__DO + 1);
        if (out_str) {
            ehs_uint16 len = state->cb_data_len < EHS_STRING_LENGTH_MAX ? state->cb_data_len : EHS_STRING_LENGTH_MAX - 1;
            memcpy(out_str, state->cb_data, len);
            out_str[len] = '\0';
        }
    }

    if (EHS_FB_OUT_CONNECTED_API2(INX_ble_service_ARG_on_client_write__DO + 2)) {
        EHS_FB_OUT_I_API2(INX_ble_service_ARG_on_client_write__DO + 2) = (ehs_sint32)state->cb_data_len;
    }

    /* Trigger finish event */
    EHS_FB_FINISH(INX_ble_service_ARG_on_client_write__EO);  /* client_wrote_evt */
}
//ICB FUNCTION on_client_write MACRO END -- DO NOT ALTER

//ICB FUNCTION on_connect MACRO START -- DO NOT ALTER
/**
 * Definition of ble_service_on_connect.
 * INTERNAL EVENT: This function can be called from external sources (ISR, threads, etc.)
 * This function can access the object data using:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ble_service_on_connect)
{
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)EHS_FB_RUN_CONTEXT;

    /* Populate output ports from callback data stored in state */
    if (EHS_FB_OUT_CONNECTED_API2(INX_ble_service_ARG_on_connect__DO)) {
        EHS_FB_OUT_I_API2(INX_ble_service_ARG_on_connect__DO) = (ehs_sint32)state->cb_conn_handle;
    }

    /* Trigger finish event */
    EHS_FB_FINISH(INX_ble_service_ARG_on_connect__EO);  /* connect_evt */
}
//ICB FUNCTION on_connect MACRO END -- DO NOT ALTER

//ICB FUNCTION on_disconnect MACRO START -- DO NOT ALTER
/**
 * Definition of ble_service_on_disconnect.
 * INTERNAL EVENT: This function can be called from external sources (ISR, threads, etc.)
 * This function can access the object data using:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ble_service_on_disconnect)
{
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)EHS_FB_RUN_CONTEXT;

    /* Populate output ports from callback data stored in state */
    if (EHS_FB_OUT_CONNECTED_API2(INX_ble_service_ARG_on_disconnect__DO)) {
        EHS_FB_OUT_I_API2(INX_ble_service_ARG_on_disconnect__DO) = (ehs_sint32)state->cb_conn_handle;
    }

    if (EHS_FB_OUT_CONNECTED_API2(INX_ble_service_ARG_on_disconnect__DO + 1)) {
        EHS_FB_OUT_I_API2(INX_ble_service_ARG_on_disconnect__DO + 1) = (ehs_sint32)state->cb_reason;
    }

    /* Trigger finish event */
    EHS_FB_FINISH(INX_ble_service_ARG_on_disconnect__EO);  /* disconnect_evt */
}
//ICB FUNCTION on_disconnect MACRO END -- DO NOT ALTER

//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Destroy the function block. Clean up any resources.
 */
EHS_FB_DESTROY_FUNCTION(ble_service)
{
    // inx_ble_service_state_type* state = (inx_ble_service_state_type*)EHS_FB_DESTROY_CONTEXT;

    /* Deinitialize HAL */
    inx_ble_service_hal_glue_deinit();

    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER
