/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license.
 ***************************************************************/

/**
 * @file target_uart.c  (esp32s3)
 * @brief ESP32-S3 IDF UART HAL implementation.
 *
 * Uses the ESP-IDF uart driver for configuration and a FreeRTOS task
 * for interrupt-driven receive handling via the IDF event queue.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "globals.h"
#include "target_uart.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_err.h"
#include "esp_log.h"

#define TAG "target_uart"

#define NOTIFY_TgtUART_ISR_DELETE ((uint32_t)3)

#define ESP_GOTO_ON_ERROR_TGT(x, goto_pos) {                               \
    esp_ret = x;                                                            \
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_ret);                                 \
    if (esp_ret != ESP_OK) { ret = TgtUART_ESPERROR; goto goto_pos; }      \
}

static int esp_ret;

/* Forward declaration */
void UART_ISR_espressif(void *pvParameters);

/* -----------------------------------------------------------------------
 * Pin arrays — populated from TARGET_UART_PIN_*_PORTn macros
 * ----------------------------------------------------------------------- */

const int gTargetUartPinTx[] = {
    TARGET_UART_PIN_TX_PORT(0),
    TARGET_UART_PIN_TX_PORT(1),
    TARGET_UART_PIN_TX_PORT(2),
    TARGET_UART_PIN_TX_PORT(3),
};

const int gTargetUartPinRx[] = {
    TARGET_UART_PIN_RX_PORT(0),
    TARGET_UART_PIN_RX_PORT(1),
    TARGET_UART_PIN_RX_PORT(2),
    TARGET_UART_PIN_RX_PORT(3),
};

const int gTargetUartPinCts[] = {
    TARGET_UART_PIN_CTS_PORT(0),
    TARGET_UART_PIN_CTS_PORT(1),
    TARGET_UART_PIN_CTS_PORT(2),
    TARGET_UART_PIN_CTS_PORT(3),
};

const int gTargetUartPinRts[] = {
    TARGET_UART_PIN_RTS_PORT(0),
    TARGET_UART_PIN_RTS_PORT(1),
    TARGET_UART_PIN_RTS_PORT(2),
    TARGET_UART_PIN_RTS_PORT(3),
};

/* -----------------------------------------------------------------------
 * Per-port state
 * ----------------------------------------------------------------------- */

static TgtUART_t UART_statusData[UART_COUNT];

/* -----------------------------------------------------------------------
 * Public API
 * ----------------------------------------------------------------------- */

int TgtUart_Stage0(int UART_num)
{
    TgtUART_t defaults = TgtUART_getTgtUART_t_default();
    size_t defsize = sizeof(TgtUART_t);
    int i;
    for (i = 0; i < UART_COUNT; i++)
    {
        if (i == UART_num) memcpy(UART_statusData + defsize * i, &defaults, defsize);
    }
    return TgtUART_OK;
}

/*
 * Configure and start the UART.
 *
 * databits:     5-8 (maps via UART_WORD_LENGTH table)
 * parity:       0=none, 1=odd, 2=even
 * stop_bits:    0=1, 1=1.5, 2=2
 * flow_control: 0=none, 1=RTS, 2=CTS, 3=RTS+CTS
 */
int TgtUart_Start(int UART_num, int tx_io, int rx_io, int rts_io, int cts_io,
                  int baudrate, int databits, int parity, int stop_bits,
                  int flow_control)
{
    int ret = TgtUART_OK;
    if (UART_num + 1 > UART_COUNT) { ret = TgtUART_INEXIST; }
    if (UART_statusData[UART_num].inUse == EHS_TRUE ||
        uart_is_driver_installed(UART_num) ||
        UART_statusData[UART_num].intrCtxObj != NULL)
    {
        ESP_LOGE(TAG, "UART port %d is in use", UART_num);
        ret = TgtUART_INUSE;
    }
    if (baudrate > UART_BAUDRATE_MAX) { ret = TgtUART_BAUDRATEERROR; }
    if (databits >= UART_WORD_LENGTH_COUNT || parity >= UART_PARITY_COUNT ||
        stop_bits >= UART_STOP_BITS_COUNT || flow_control >= UART_HW_FLOWCTRL_COUNT)
    {
        ret = TgtUART_CONFIG_ERROR;
    }
    if (ret == TgtUART_OK)
    {
        uart_config_t uart_config = {
            .baud_rate  = baudrate,
            .data_bits  = UART_WORD_LENGTH[databits],
            .parity     = gEhsUART_PARITY[parity],
            .stop_bits  = UART_STOP_BITS[stop_bits],
            .flow_ctrl  = UART_HW_FLOWCTRL[flow_control],
            .source_clk = UART_SCLK_APB,
        };
        esp_log_level_set(TAG, ESP_LOG_NONE);
        ESP_GOTO_ON_ERROR_TGT(uart_driver_install(UART_num,
                                                   UART_RX_BUFFER_SIZE * 2,
                                                   UART_TX_BUFFER_SIZE * 2,
                                                   UART_QUEUE_SIZE,
                                                   &(UART_statusData[UART_num].intrObj), 0),
                              TgtUart_Start_Return);
        ESP_GOTO_ON_ERROR_TGT(uart_param_config(UART_num, &uart_config), TgtUart_Start_Return);
        ESP_GOTO_ON_ERROR_TGT(uart_set_pin(UART_num, tx_io, rx_io, rts_io, cts_io), TgtUart_Start_Return);
        UART_statusData[UART_num].configured = EHS_TRUE;
        UART_statusData[UART_num].inUse      = EHS_TRUE;
    }
    ESP_LOGI(TAG, "TgtUart_Start %s", ret == TgtUART_OK ? "OK" : "FAILED");
TgtUart_Start_Return:
    if (ret != TgtUART_OK) UART_statusData[UART_num].intrObj = NULL;
    return ret;
}

/*
 * Reconfigure a running UART port.
 * No-op (returns OK) if the driver is not yet installed — the new settings
 * will be picked up from the gUART* globals at the next TgtUart_Start call.
 */
int TgtUart_Config(int UART_num, int baudrate, int databits, int parity,
                   int stop_bits, int flow_control)
{
    if (UART_num < 0 || UART_num >= UART_COUNT) return TgtUART_INEXIST;
    if (!uart_is_driver_installed(UART_num))    return TgtUART_OK;
    if (databits >= UART_WORD_LENGTH_COUNT || parity >= UART_PARITY_COUNT ||
        stop_bits >= UART_STOP_BITS_COUNT || flow_control >= UART_HW_FLOWCTRL_COUNT)
        return TgtUART_CONFIG_ERROR;

    uart_config_t uart_config = {
        .baud_rate  = baudrate,
        .data_bits  = UART_WORD_LENGTH[databits],
        .parity     = gEhsUART_PARITY[parity],
        .stop_bits  = UART_STOP_BITS[stop_bits],
        .flow_ctrl  = UART_HW_FLOWCTRL[flow_control],
        .source_clk = UART_SCLK_APB,
    };
    esp_err_t err = uart_param_config(UART_num, &uart_config);
    return (err == ESP_OK) ? TgtUART_OK : TgtUART_ESPERROR;
}

int TgtUart_RS485Setup(int UART_num)
{
    int ret = TgtUART_OK;
    ESP_GOTO_ON_ERROR_TGT(uart_set_mode(UART_num, UART_MODE_RS485_HALF_DUPLEX),
                          TgtUart_RS485Setup_Return);
    ESP_GOTO_ON_ERROR_TGT(uart_set_rx_timeout(UART_num, 3), TgtUart_RS485Setup_Return);
TgtUart_RS485Setup_Return:
    return ret;
}

int TgtUart_Stop(int UART_num)
{
    int ret = TgtUART_OK;
    if (UART_num + 1 > UART_COUNT) { ret = TgtUART_INEXIST; }
    if (!uart_is_driver_installed(UART_num)) { ret = TgtUART_NOTINUSE; }
    if (ret == TgtUART_OK)
    {
        ESP_GOTO_ON_ERROR_TGT(uart_driver_delete(UART_num), TgtUart_Stop_Return);
        if (UART_statusData[UART_num].intrCtxObj != NULL &&
            eTaskGetState(UART_statusData[UART_num].intrCtxObj) != eDeleted)
        {
            ret = xTaskNotify(UART_statusData[UART_num].intrCtxObj,
                              NOTIFY_TgtUART_ISR_DELETE, eSetValueWithOverwrite) == pdPASS
                  ? TgtUART_OK : TgtUART_INTERRUPT_DEL_ERROR;
        }
        UART_statusData[UART_num].configured = EHS_FALSE;
        UART_statusData[UART_num].inUse      = EHS_FALSE;
        UART_statusData[UART_num].intrObj    = 0;
        UART_CALLBACK_FUNCTIONS[UART_num]    = NULL;
    }
    ESP_LOGI(TAG, "TgtUart_Stop %s", ret == TgtUART_OK ? "OK" : "FAILED");
TgtUart_Stop_Return:
    return ret;
}

int TgtUart_Send(int UART_num, char *payload, unsigned int length)
{
    int ret = TgtUART_OK;
    if (UART_num + 1 > UART_COUNT) { ret = TgtUART_INEXIST; }
    if (!uart_is_driver_installed(UART_num)) { ret = TgtUART_NOTINUSE; }
    if (ret == TgtUART_OK)
    {
        int iLength = uart_write_bytes(UART_num, payload, length);
        if (iLength == -1)             ret = TgtUART_ERROR;
        else if ((unsigned)iLength != length) ret = TgtUART_LENGTH_NOT_MATCH;
    }
    return ret;
}

void TgtUART_SendThread(int UART_num, char *payload, unsigned int length)
{
    memcpy(UART_statusData[UART_num].data_to_send, payload, length);
    UART_statusData[UART_num].len_data_to_send = length;
    UART_statusData[UART_num].sending = EHS_TRUE;
}

int TgtUART_SendInThread(int UART_num)
{
    if (UART_statusData[UART_num].sending == EHS_TRUE)
    {
        UART_statusData[UART_num].sending = EHS_FALSE;
        int ret = TgtUart_Send(UART_num,
                               UART_statusData[UART_num].data_to_send,
                               UART_statusData[UART_num].len_data_to_send);
        Common_UART_onSendComplete(ret);
        return ret;
    }
    return 1;
}

int TgtUART_RecvPoll(int UART_num, char *payload, unsigned int length)
{
    int ret = TgtUART_OK;
    if (UART_num + 1 > UART_COUNT) { ret = TgtUART_INEXIST; }
    if (!uart_is_driver_installed(UART_num)) { ret = TgtUART_NOTINUSE; }
    if (ret == TgtUART_OK)
    {
        int iLength = uart_read_bytes(UART_num, payload, length,
                                      100 / portTICK_PERIOD_MS);
        if (iLength == -1)             ret = TgtUART_ERROR;
        else if ((unsigned)iLength != length) ret = TgtUART_LENGTH_NOT_MATCH;
    }
    return ret;
}

int TgtUART_Intr_register(int UART_num, uart_cb_func_t cb_func)
{
    int ret = TgtUART_OK;
    if (UART_num + 1 > UART_COUNT) { ret = TgtUART_INEXIST; }
    if (!uart_is_driver_installed(UART_num)) { ret = TgtUART_NOTINUSE; }
    if (UART_statusData[UART_num].intrCtxObj != NULL &&
        eTaskGetState(UART_statusData[UART_num].intrCtxObj) != eDeleted)
    {
        ret = TgtUART_INTERRUPT_EXISTED;
    }
    if (ret == TgtUART_OK)
    {
        char task_str[20];
        UART_CALLBACK_FUNCTIONS[UART_num] = cb_func;
        sprintf(task_str, "uart_event_task-%d", UART_num);
        BaseType_t xReturned = xTaskCreate(UART_ISR_espressif, task_str, 3072,
                                            (void *)UART_num, EHS_PRI_UART,
                                            &(UART_statusData[UART_num].intrCtxObj));
        if (xReturned != pdPASS)
        {
            UART_statusData[UART_num].intrCtxObj = NULL;
            ret = TgtUART_INTERRUPT_REG_ERROR;
        }
    }
    return ret;
}

int TgtUart_SetComPort(int UART_num, int com_port_number)
{
    /* COM port numbers are a Windows concept; no-op on ESP32. */
    (void)UART_num; (void)com_port_number;
    return TgtUART_OK;
}

int TgtUart_SetDevicePath(int UART_num, const char *path)
{
    /* Device paths are a Linux concept; no-op on ESP32. */
    (void)UART_num; (void)path;
    return TgtUART_OK;
}

/*
 * UART Interrupt Task — blocks on IDF event queue and dispatches to callback.
 */
void UART_ISR_espressif(void *pvParameters)
{
    int UART_num = (int)pvParameters;
    uart_event_t event;
    size_t buffered_size = UART_RX_BUFFER_SIZE + 1;
    uint8_t *dtmp = (uint8_t *)malloc(buffered_size);
    uint8_t *tmp;
    int err;
    uint32_t notify_value = 0;

    while (1)
    {
        notify_value = ulTaskNotifyTake(pdTRUE, 0);
        if (notify_value == NOTIFY_TgtUART_ISR_DELETE) break;

        err = TgtUART_OK;
        if (xQueueReceive(UART_statusData[UART_num].intrObj,
                          (void *)&event, 1000 / portTICK_PERIOD_MS))
        {
            bzero(dtmp, buffered_size);
            switch (event.type)
            {
                case UART_DATA:
                    if (event.size > buffered_size)
                    {
                        buffered_size = event.size + 1;
                        if ((tmp = (uint8_t *)realloc(dtmp, buffered_size)) != NULL)
                            dtmp = tmp;
                        else
                            err = TgtUART_HEAP_INSUFFICIENT;
                    }
                    if (err == TgtUART_OK)
                    {
                        int length = uart_read_bytes(UART_num, dtmp, event.size,
                                                     1000 / portTICK_PERIOD_MS);
                        if (length == -1)
                            err = TgtUART_ERROR;
                        else if (UART_CALLBACK_FUNCTIONS[UART_num])
                            UART_CALLBACK_FUNCTIONS[UART_num]((char *)dtmp, length);
                    }
                    break;

                case UART_FIFO_OVF:
                case UART_BUFFER_FULL:
                    err = TgtUART_BUFFER_FULL;
                    uart_flush_input(UART_num);
                    xQueueReset(UART_statusData[UART_num].intrObj);
                    break;

                case UART_BREAK:
                    break;

                case UART_PARITY_ERR:
                case UART_FRAME_ERR:
                    err = TgtUART_DATA_ERROR;
                    break;

                default:
                    err = TgtUART_EVENT_UNDEFINED;
                    break;
            }
        }
        (void)err; /* TODO: add proper error handling */
    }

    free(dtmp);
    vTaskDelete(NULL);
}
