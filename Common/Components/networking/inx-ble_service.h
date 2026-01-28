/***************************************************************
* Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license.
****************************************************************/

#ifndef INXWARE_ble_service
#define INXWARE_ble_service
#include "inx-component.h"

/*****************************************************
 * Creates and manages a Bluetooth Low Energy (BLE) GATT service with configurable characteristics. Supports up to 16 characteristics with read, write, notify, and indicate properties. Handles client connections, characteristic access, and notifications. Suitable for creating custom BLE peripherals on embedded devices.
 *****************************************************/
#define INXWARE_FB_ID_ble_service 0xA0B5
#define INXWARE_FB_NAME_ble_service "ble_service"

/* Component state data structure - forward declaration for HAL glue layer */
/* Full definition is in the .c file within ICB macros */
#ifndef INX_BLE_SERVICE_STATE_TYPE_DEFINED
#define INX_BLE_SERVICE_STATE_TYPE_DEFINED
typedef struct
{
    /* Parameters */
    ehs_char* service_uuid;
    ehs_char* service_name;
    ehs_sint32 num_chars;
    ehs_sint32 adv_interval_ms;
    ehs_sint32 mtu_size;
    ehs_char* char_0_uuid;
    ehs_char* char_0_name;
    ehs_sint32 char_0_props;
    ehs_sint32 char_0_max_len;
    ehs_char* char_1_uuid;
    ehs_char* char_1_name;
    ehs_sint32 char_1_props;
    ehs_sint32 char_1_max_len;
    ehs_char* char_2_uuid;
    ehs_char* char_2_name;
    ehs_sint32 char_2_props;
    ehs_sint32 char_2_max_len;
    ehs_char* char_3_uuid;
    ehs_char* char_3_name;
    ehs_sint32 char_3_props;
    ehs_sint32 char_3_max_len;

    /* Callback data storage */
    ehs_uint8 cb_char_idx;
    ehs_char cb_data[512];
    ehs_uint16 cb_data_len;
    ehs_uint16 cb_conn_handle;
    ehs_uint8 cb_reason;
} inx_ble_service_state_type;
#endif

/* Port argument index macros for HAL glue layer */
#define INX_ble_service_ARG_on_client_write__DO 1
#define INX_ble_service_ARG_on_connect__DO 1
#define INX_ble_service_ARG_on_disconnect__DO 1

/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ble_service); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ble_service); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(ble_service); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ble_service_init); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ble_service_start_adv); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ble_service_stop_adv); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ble_service_write_char); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ble_service_read_char); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ble_service_notify_char); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ble_service_on_client_write); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ble_service_on_connect); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ble_service_on_disconnect); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ble_service)

#endif /* INXWARE_ble_service */
