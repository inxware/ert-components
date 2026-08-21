/***************************************************************
* Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license.
****************************************************************/

#ifndef INXWARE_ble_service
#define INXWARE_ble_service
#include "inx-component.h"
#include "hal_ble.h"

/*****************************************************
 * Creates and manages a Bluetooth Low Energy (BLE) GATT service with configurable characteristics. Supports up to 16 characteristics with read, write, notify, and indicate properties. Handles client connections, characteristic access, and notifications. Suitable for creating custom BLE peripherals on embedded devices.
 *****************************************************/
#define INXWARE_FB_ID_ble_service 0xA0B5
#define INXWARE_FB_NAME_ble_service "ble_service"

/* inx_ble_uuid_t, inx_ble_char_config_t, INX_BLE_NAME_MAX and
 * INX_BLE_UUID_TYPE_* come from hal_ble.h (included above). */
#define INX_BLE_SERVICE_UUID_STR_MAX        140  /* max chars for a UUID string (parse buffer) */
#define INX_BLE_CALLBACK_OBJECT_SIZE_MAX    512
#define INX_BLE_SERVICE_MAX_CHARACTERSTICS  4    /* max chars this FB supports (CDF/SODL limit) */

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
