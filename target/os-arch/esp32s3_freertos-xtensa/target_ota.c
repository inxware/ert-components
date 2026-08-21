#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>

#include "globals.h"
#include "hal_ota.h"
#include "target_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_app_format.h"
#include "esp_flash_partitions.h"
#include "esp_partition.h"
#include "nvs.h"
#include "nvs_flash.h"

#define TAG "target_ota"

enum {
    OTA_FLAG_INIT   = 0,
    OTA_FLAG_BEGIN  = 1 << 0,
    OTA_FLAG_WRITE  = 1 << 1,
    OTA_FLAG_END    = 1 << 2,
    OTA_FLAG_ABORT  = 1 << 3,
    OTA_FLAG_NUMBER = 1 << 4
};

static target_ota_state_t ota_state = TARGET_OTA_IDLE;

static TaskHandle_t gOTA_task_handle = NULL;
static ehs_bool gsWriting = EHS_FALSE;

//static ehs_uint8 *gData = NULL;
static ehs_uint8 gData[EHS_STRING_LENGTH_MAX] = { 0 };
static ehs_sint32 gSize = 0;
static ehs_sint32 gOffset = 0;

static esp_ota_handle_t gESP_OTA_handle = 0;

static const esp_partition_t *update_partition = NULL;

static ehs_bool gOtaTaskNotify = EHS_FALSE;
static ehs_uint8 gOtaTaskNotifyValue = OTA_FLAG_INIT;

static target_ota_on_abort_t gOnAbortCallback = NULL;

target_ota_state_t thOTA_current_state(void)
{
    return ota_state;
}

ehs_sint32 thOTA_begin(ehs_bool alt_partition, ehs_sint32 partition_num)
{
    if (ota_state > TARGET_OTA_IDLE) return 1;
    const esp_partition_t *configured = esp_ota_get_boot_partition();
    const esp_partition_t *running = esp_ota_get_running_partition();
    if (configured != running)
    {
        ESP_LOGW(TAG, "Configured OTA boot partition at offset 0x%08"PRIx32", but running from offset 0x%08"PRIx32,
                 configured->address, running->address);
    }
    update_partition = esp_ota_get_next_update_partition(NULL);
    if (update_partition == NULL) return 2;
    esp_err_t err = esp_ota_begin(
        update_partition, 
        OTA_SIZE_UNKNOWN, // Used with esp_ota_write_with_offset // OTA_WITH_SEQUENTIAL_WRITES, // This is used for esp_ota_write only
        &gESP_OTA_handle
    );
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "OTA begin aborted.");
        esp_ota_abort(gESP_OTA_handle);
        gESP_OTA_handle = 0; /* esp_ota_handle_t is an integer, not a pointer */
        if (err == ESP_ERR_NO_MEM) return 3;
        return 4;
    }
    // xTaskNotify(gOTA_task_handle, OTA_FLAG_BEGIN, eSetValueWithOverwrite);
    gOtaTaskNotify = EHS_TRUE;
    gOtaTaskNotifyValue = OTA_FLAG_BEGIN;
    return 0;
}

ehs_uint8 thOTA_write_passthrough(ehs_char * databuf, ehs_sint32 size, ehs_sint32 offset)
{
    if (gsWriting == EHS_TRUE) return 1;
    if (size <= 0) return 2;
    if (databuf == NULL) return 3;
    if (offset < 0) return 4;
    gSize = size;
    gOffset = offset;
    // if (gData == NULL) gData = (ehs_uint8 *) calloc(gSize, sizeof(ehs_uint8));
    // else gData = realloc(gData, gSize * sizeof(ehs_uint8));
    /* gData is a static array (see declaration) - it can never be NULL */
    if (gSize > EHS_STRING_LENGTH_MAX) return 6;
    memcpy(gData, databuf, gSize * sizeof(ehs_uint8));
    //ESP_LOGD(TAG, "OTA Write %d -> %d", offset, offset + size);
    // xTaskNotify(gOTA_task_handle, OTA_FLAG_WRITE, eSetValueWithOverwrite);
    gOtaTaskNotify = EHS_TRUE;
    gOtaTaskNotifyValue = OTA_FLAG_WRITE;
    return 0;
}

void thOTA_end(void)
{
    if (ota_state == TARGET_OTA_IDLE || gESP_OTA_handle == 0) return;
    // xTaskNotify(gOTA_task_handle, OTA_FLAG_END, eSetValueWithOverwrite);
    gOtaTaskNotify = EHS_TRUE;
    gOtaTaskNotifyValue = OTA_FLAG_END;
}

static inline char IRAM_ATTR to_hex_digit(unsigned val)
{
    return (val < 10) ? ('0' + val) : ('a' + val - 10);
}

ehs_bool thOTA_checkChecksum(ehs_bool alt_partition, ehs_sint32 partition_num, ehs_char * CS_token, ehs_sint32 CS_token_len)
{
    if (CS_token_len != 64 || CS_token == NULL) return EHS_FALSE;
    ehs_uint8 sha_256[32] = { 0 };
    ehs_char str_sha_256[65] = { 0 };
    int i;
    esp_err_t err = esp_partition_get_sha256(update_partition, sha_256);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "OTA cannot get partition checksum!");
        return EHS_FALSE;
    }
    for (i = 0 ; i < 32 ; i++)
    {
        // sprintf(str_sha_256 + i * 2, "%02X", sha_256[i]);
        str_sha_256[i * 2]      = to_hex_digit(sha_256[i] >> 4);
        str_sha_256[i * 2 + 1]  = to_hex_digit(sha_256[i] & 0xf);
    }
    str_sha_256[64] = 0;
    ESP_LOGI(TAG, "Passed in SHA256: [%s]", CS_token);
    ESP_LOGI(TAG, "Int calcd SHA256: [%s]", str_sha_256);
    if (strcmp(str_sha_256, CS_token) != 0)
        {
            ESP_LOGE(TAG, "Checksum NOT match!");
            return EHS_FALSE;
        }
    return EHS_TRUE;
}

ehs_bool thOTA_checkValid(ehs_bool alt_partition, ehs_sint32 partition_num)
{
    esp_app_desc_t app_desc;
    esp_err_t err = esp_ota_get_partition_description(update_partition, &app_desc);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "OTA image is not valid!");
        return EHS_FALSE;
    }
    return EHS_TRUE;
}

ehs_bool thOTA_switch(ehs_bool alt_partition, ehs_sint32 partition_num)
{
    if (update_partition == NULL || ota_state != TARGET_OTA_ENDED) return EHS_FALSE;
    esp_err_t err = esp_ota_set_boot_partition(update_partition);
    update_partition = NULL;
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "OTA Switch failed!");
        return EHS_FALSE;
    }
    return EHS_TRUE;
}

void thOTA_abort(void)
{
    if (ota_state == TARGET_OTA_IDLE || gESP_OTA_handle == 0 ) return;
    // xTaskNotify(gOTA_task_handle, OTA_FLAG_ABORT, eSetValueWithOverwrite);
    gOtaTaskNotify = EHS_TRUE;
    gOtaTaskNotifyValue = OTA_FLAG_ABORT;
}

void thOTA_on_abort_callback(target_ota_on_abort_t callback)
{
    gOnAbortCallback = callback;
}

void thOTA_idle(void)
{
    ota_state = TARGET_OTA_IDLE;
}

void target_OTA_task(void *pvParameters)
{
    ehs_uint32 notificationValue = OTA_FLAG_INIT;
    esp_err_t err;
    if (gOtaTaskNotify == EHS_TRUE)
    {
        gOtaTaskNotify = EHS_FALSE;
        notificationValue = gOtaTaskNotifyValue;
        switch (ota_state) {
        case TARGET_OTA_IDLE:
            if ((notificationValue & OTA_FLAG_BEGIN) != 0) ota_state = TARGET_OTA_BEGAN;
        break;
        case TARGET_OTA_BEGAN:
            if ((notificationValue & OTA_FLAG_ABORT) != 0)
            {
                esp_ota_abort(gESP_OTA_handle);
                gESP_OTA_handle = 0; /* esp_ota_handle_t is an integer, not a pointer */
                ota_state = TARGET_OTA_IDLE;
                if(gOnAbortCallback != NULL) gOnAbortCallback();
            }
            else if ((notificationValue & OTA_FLAG_END) != 0)
            {
                esp_ota_end(gESP_OTA_handle);
                gESP_OTA_handle = 0; /* esp_ota_handle_t is an integer, not a pointer */
                ota_state = TARGET_OTA_ENDED;
            }
            else if ((notificationValue & OTA_FLAG_WRITE) != 0)
            {
                gsWriting = EHS_TRUE;
                err = esp_ota_write_with_offset(gESP_OTA_handle, gData, gSize, gOffset);
                if (err != ESP_OK)
                {
                    ESP_LOGE(TAG, "OTA write aborted, writing state failed.");
                    esp_ota_abort(gESP_OTA_handle);
                    gESP_OTA_handle = 0;
                    ota_state = TARGET_OTA_IDLE;
                    if(gOnAbortCallback != NULL) gOnAbortCallback();
                }
                gSize = 0;
                gOffset = 0;
                Common_OTA_Write_ACK((ehs_uint8)err);
                gsWriting = EHS_FALSE;
            }
//            if (gData != NULL) - not needed according to compiler always true
//            {
                memset(gData, 0, EHS_STRING_LENGTH_MAX);
//            }
        break;
        case TARGET_OTA_ENDED:
            // wait to complete
        break;
        default:
            vTaskDelay(1);
        break;
        }
        notificationValue = OTA_FLAG_INIT;
    }
}
