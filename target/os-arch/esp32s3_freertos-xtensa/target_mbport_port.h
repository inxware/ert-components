#ifndef _TARGET_MBPORT_PORT_H_ 
#define _TARGET_MBPORT_PORT_H_ 

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"         // for queue

#include "esp_log.h"                // for ESP_LOGE macro
#include "esp_timer.h"
#include "driver/uart.h"            // for uart_event_t

#include "target_types.h"

#if __has_include("driver/gptimer.h")
#include "driver/gptimer.h"
#else
#include "driver/timer.h"
#endif

#define MB_PORT_TAG                     "MB_PORT_COMMON"

#define MODBUS_LOGI                     ESP_LOGI
#define MODBUS_LOGW                     ESP_LOGW
#define MODBUS_LOGE                     ESP_LOGE
#define MODBUS_LOGD                     ESP_LOGD
#define MODBUS_LOG_BUFFER_HEX_LEVEL     ESP_LOG_BUFFER_HEX_LEVEL
#define MODBUS_LOG_DEBUG                ESP_LOG_DEBUG
#define MODBUS_EARLY_LOGD               ESP_EARLY_LOGD

#define EHS_MB_MS_TO_TICKS pdMS_TO_TICKS

#define EHS_MB_LOGE(str, ...) ESP_LOGE(MB_PORT_TAG, "%s(%u): " str, __FUNCTION__, __LINE__, ##__VA_ARGS__)

// Macro to check if stack shutdown event is active
#define TCP_PORT_CHECK_SHDN(sema_ptr, callback_func) do { \
    if (sema_ptr) { \
        ESP_LOGD(MB_PORT_TAG, "Shutdown stack from %s(%d)", __func__, __LINE__); \
        callback_func(); \
    } \
} while(0)

#define EHS_MB_ENTER_CRITICAL_SECTION { ESP_EARLY_LOGD(MB_PORT_TAG,"%s: Port enter critical.", __func__); \
                                    vMBPortEnterCritical(); }

#define EHS_MB_EXIT_CRITICAL_SECTION { vMBPortExitCritical(); \
                                    ESP_EARLY_LOGD(MB_PORT_TAG,"%s: Port exit critical", __func__); }

#ifndef MB_SERIAL_API_RESP_TICS 
#define MB_SERIAL_API_RESP_TICS (pdMS_TO_TICKS(MB_MAX_RESPONSE_TIME_MS))
#endif//MB_SERIAL_API_RESP_TICS 

#endif//_TARGET_MBPORT_PORT_H_ 