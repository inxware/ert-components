/***************************************************************
* Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license.
****************************************************************/

#ifndef INX_BLE_SERVICE_HAL_GLUE_H
#define INX_BLE_SERVICE_HAL_GLUE_H

#include <stdint.h>

/* HAL glue layer function declarations */
/* Note: Uses void* for context and state to avoid header dependencies */
int inx_ble_service_hal_glue_init(void* context, void* state);
int inx_ble_service_hal_glue_start_adv(void);
int inx_ble_service_hal_glue_stop_adv(void);
int inx_ble_service_hal_glue_write_char(uint8_t char_idx, const char* data, uint16_t length);
int inx_ble_service_hal_glue_read_char(uint8_t char_idx, char* data, uint16_t* length, uint16_t max_len);
int inx_ble_service_hal_glue_notify(uint8_t char_idx, const char* data, uint16_t length);
void inx_ble_service_hal_glue_deinit(void);

#endif /* INX_BLE_SERVICE_HAL_GLUE_H */
