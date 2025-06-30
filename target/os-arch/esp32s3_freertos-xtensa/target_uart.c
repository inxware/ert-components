#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "target.h"
#include "target_config.h"
#include "target_types.h"
#include "target_uart.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_err.h"

// Log
#include "esp_log.h"
#define TAG "target_uart"

#define NOTIFY_TgtUART_ISR_DELETE ( ( uint32_t ) 3 )

#define ESP_GOTO_ON_ERROR_TGT(x, goto_pos) {                            \
    esp_ret = x;                                                        \
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_ret);                             \
    if (esp_ret != ESP_OK) { ret = TgtUART_ESPERROR; goto goto_pos; }   \
}

#define TARGET_ESP32S3_UART_DEBUG

static int esp_ret;

// Function prototypes
void UART_ISR_espressif(void *pvParameters);

// Global status variables
//
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

// UART data and status type declaration
static TgtUART_t UART_statusData[UART_COUNT];

// // Whether the UART is configured
// static ehs_bool UART_Configured[UART_COUNT] = {EHS_FALSE, EHS_FALSE, EHS_FALSE};
// 
// // Whether the specific UART channel is currently used
// static ehs_bool UART_InUse[UART_COUNT] = {EHS_FALSE, EHS_FALSE, EHS_FALSE};
// 
// // List the UART interrupt context to be used to manage the interrupt (i.e. Destroy)
// static TaskHandle_t UART_IntrContextObject[UART_COUNT] = {NULL, NULL, NULL};
// 
// // List the global variable of the UART channel to be used for interrupt
// static QueueHandle_t UART_IntrObject[UART_COUNT] = {NULL, NULL, NULL};

/*
 * The very first stage before everything proceeds. Normally used to initialise default values
 * */
int TgtUart_Stage0( int UART_num )
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
 * Configure and start the UART with a bunch of parameters and UART GPIO pins
 * If the GPIO pins are not specified, use -1.
 *
 * Parameter Settings
 *  data_length:
 *   0 -> 5 bits
 *   1 -> 6 bits
 *   2 -> 7 bits
 *   3 -> 8 bits
 *
 *  stop bits:
 *   0 -> 1 stop bits
 *   1 -> 1.5 stop bits
 *   2 -> 2 stop bits
 *
 *  parity config:
 *   0 -> disable parity check
 *   1 -> odd parity bit check
 *   2 -> even parity bit check
 *
 *  hardware flow control:
 *   0 -> disable hardware flow control
 *   1 -> enable RX flow control (Ready To Send)
 *   2 -> enable TX flow control (Clear To Send)
 *   3 -> enable full flow control (Ready To Send & Clear To Send)
 * */
int TgtUart_Start(int UART_num, int tx_io, int rx_io, int rts_io, int cts_io, int baudrate, int databits, int parity, int stop_bits, int flow_control)
{
    int ret = TgtUART_OK;
    if (UART_num + 1 > UART_COUNT) { ret = TgtUART_INEXIST; }
    if (UART_statusData[UART_num].inUse == EHS_TRUE || uart_is_driver_installed(UART_num) || UART_statusData[UART_num].intrCtxObj != NULL) 
    {
        ESP_LOGE(TAG, "UART port %d is in use", UART_num);
        ret = TgtUART_INUSE;
    }
    if (baudrate > UART_BAUDRATE_MAX) { ret = TgtUART_BAUDRATEERROR; }
    if (databits >= UART_WORD_LENGTH_COUNT || parity >= UART_PARITY_COUNT || stop_bits >= UART_STOP_BITS_COUNT || flow_control >= UART_HW_FLOWCTRL_COUNT)
    {
        ret = TgtUART_CONFIG_ERROR;
    }
    if (ret == TgtUART_OK)
    {
        uart_config_t uart_config = {
            .baud_rate = baudrate,
            .data_bits = UART_WORD_LENGTH[databits],
            .parity = gEhsUART_PARITY[parity],
            .stop_bits = UART_STOP_BITS[stop_bits],
            .flow_ctrl = UART_HW_FLOWCTRL[flow_control],
            .source_clk = UART_SCLK_APB,
        };
        esp_log_level_set(TAG, ESP_LOG_NONE);
        ESP_GOTO_ON_ERROR_TGT(uart_driver_install(UART_num, UART_RX_BUFFER_SIZE * 2, UART_TX_BUFFER_SIZE * 2, UART_QUEUE_SIZE, &(UART_statusData[UART_num].intrObj), 0), TgtUart_Config_Return);
        ESP_GOTO_ON_ERROR_TGT(uart_param_config(UART_num, &uart_config), TgtUart_Config_Return);
        ESP_GOTO_ON_ERROR_TGT(uart_set_pin(UART_num, tx_io, rx_io, rts_io, cts_io), TgtUart_Config_Return);
        UART_statusData[UART_num].configured = EHS_TRUE;
        UART_statusData[UART_num].inUse = EHS_TRUE;
    }
#ifdef TARGET_ESP32S3_UART_DEBUG
    ESP_LOGI(TAG, "TgtUart_Config OK");
#endif
TgtUart_Config_Return:
    if (ret != TgtUART_OK) UART_statusData[UART_num].intrObj = NULL;
    return ret;
}

int TgtUart_RS485Setup(int UART_num)
{
    int ret = TgtUART_OK;
    ESP_GOTO_ON_ERROR_TGT(uart_set_mode(UART_num, UART_MODE_RS485_HALF_DUPLEX), TgtUart_RS485Setup_Return);
    ESP_GOTO_ON_ERROR_TGT(uart_set_rx_timeout(UART_num, 3), TgtUart_RS485Setup_Return);
TgtUart_RS485Setup_Return:
    return ret;
}

// /*
//  * Start the UART communication after configuration
//  * */
// int TgtUart_Start(int UART_num)
// {
//     int ret = TgtUART_OK;
//     if (UART_num + 1 > UART_COUNT) { ret = TgtUART_INEXIST; }
//    // if (UART_statusData[UART_num].inUse == EHS_TRUE || uart_is_driver_installed(UART_num) || UART_statusData[UART_num].intrCtxObj != NULL)
//    // {
//    //     ESP_LOGE(TAG, "UART port %d is in use", UART_num);
//    //     ret = TgtUART_INUSE;
//    // }
//     // Check whether the UART is configured.
//     if (UART_statusData[UART_num].configured == EHS_FALSE) { ret = TgtUART_NOTCONFIGURED; }
//         ESP_LOGW(TAG, "before driver install");
//     if (ret == TgtUART_OK)
//     {
//         //ESP_GOTO_ON_ERROR_TGT(uart_driver_install(UART_num, UART_RX_BUFFER_SIZE * 2, UART_TX_BUFFER_SIZE * 2, UART_QUEUE_SIZE, &(UART_statusData[UART_num].intrObj), 0), TgtUart_Start_Return);
//         UART_statusData[UART_num].inUse = EHS_TRUE;
//     }
// TgtUart_Start_Return:
//     if (ret != TgtUART_OK) UART_statusData[UART_num].intrObj = NULL;
// #ifdef TARGET_ESP32S3_UART_DEBUG
//     ESP_LOGI(TAG, "TgtUart_Start, ret: %d", ret);
// #endif
//     return ret;
// }

/*
 * Stop the UART communication. Reset all related UART instances
 * */
int TgtUart_Stop(int UART_num)
{
    int ret = TgtUART_OK;
    if (UART_num + 1 > UART_COUNT) { ret = TgtUART_INEXIST; }
    if (!uart_is_driver_installed(UART_num)) { ret = TgtUART_NOTINUSE; }
    if (ret == TgtUART_OK)
    {
        ESP_GOTO_ON_ERROR_TGT(uart_driver_delete(UART_num), TgtUart_Stop_Return);
        // Notify to delete the UART ISR Task if it's created and not deleted
        // TODO: Is this called from HWTimer interrupt? If so, use xTaskNotifyFromISR.
        if (UART_statusData[UART_num].intrCtxObj != NULL && eTaskGetState(UART_statusData[UART_num].intrCtxObj) != eDeleted)
            ret = xTaskNotify(UART_statusData[UART_num].intrCtxObj, NOTIFY_TgtUART_ISR_DELETE, eSetValueWithOverwrite) == pdPASS ? TgtUART_OK : TgtUART_INTERRUPT_DEL_ERROR;
        UART_statusData[UART_num].configured = EHS_FALSE;
        UART_statusData[UART_num].inUse = EHS_FALSE;
        UART_statusData[UART_num].intrObj = 0;
        UART_CALLBACK_FUNCTIONS[UART_num] = NULL;
    }
#ifdef TARGET_ESP32S3_UART_DEBUG
    ESP_LOGI(TAG, "TgtUart_Stop OK");
#endif
TgtUart_Stop_Return:
    return ret;
}

/*
 * Send UART paylaod with a given length
 * */
int TgtUart_Send(int UART_num, char *payload, unsigned int length)
{
    int ret = TgtUART_OK;
    if (UART_num + 1 > UART_COUNT) { ret = TgtUART_INEXIST; }
    if (!uart_is_driver_installed(UART_num)) { ret = TgtUART_NOTINUSE; }
    int iLength;
    if (ret == TgtUART_OK)
    {
        iLength = uart_write_bytes(UART_num, payload, length);
        if (iLength == -1) ret = TgtUART_ERROR;
        else if (iLength != length) ret = TgtUART_LENGTH_NOT_MATCH;
    }
#ifdef TARGET_ESP32S3_UART_DEBUG
    if (ret == TgtUART_OK)
        ESP_LOGI(TAG, "TgtUart_Send OK");
#endif
    return ret;
}

/**
 * Send UART payload with a given length with threading.
 * Returns immediately without blocking. This should be used with TgtUART_SendInThread.
 * */
void TgtUART_SendThread(int UART_num, char *payload, unsigned int length)
{
    memcpy(UART_statusData[UART_num].data_to_send, payload, length);
    UART_statusData[UART_num].len_data_to_send = length;
    UART_statusData[UART_num].sending = EHS_TRUE;
}

/**
 * Send UART payload in thread.
 * Return code <= 0 for sending success and failure.
 * Return code > 0 (i.e. 1) for not sending the message.
 * */
int TgtUART_SendInThread(int UART_num)
{
    int ret = 0;
    if (UART_statusData[UART_num].sending == EHS_TRUE)
    {
        UART_statusData[UART_num].sending = EHS_FALSE;
        ret =  TgtUart_Send(UART_num, UART_statusData[UART_num].data_to_send, UART_statusData[UART_num].len_data_to_send);
        Common_UART_onSendComplete(ret);
        return ret;
    }
    else return 1;
}

/*
 * Receive the UART message of a given length into the payload by polling
 * */
int TgtUART_RecvPoll(int UART_num, char *payload, unsigned int length)
{
    int ret = TgtUART_OK;
    if (UART_num + 1 > UART_COUNT) { ret = TgtUART_INEXIST; }
    if (!uart_is_driver_installed(UART_num)) { ret = TgtUART_NOTINUSE; }
    int iLength;
    if (ret == TgtUART_OK)
    {
        iLength = uart_read_bytes(UART_num, payload, length, 100);
        if (iLength == -1) ret = TgtUART_ERROR;
        else if (iLength != length) ret = TgtUART_LENGTH_NOT_MATCH;
    }
#ifdef TARGET_ESP32S3_UART_DEBUG
    ESP_LOGI(TAG, "TgtUart_RecvPoll OK");
#endif
    return ret;
}

/*
 * Register the already defined interrupt handler, which is common to all channels.
 * Register the UART event interrupt data receive callback.
 * The callback function is:
 *      void cb_func (char * payload, int payload_length);
 *  Noting that both arguments are inputs
 * */
int TgtUART_Intr_register(int UART_num, uart_cb_func_t cb_func)
{
    int ret = TgtUART_OK;
    if (UART_num + 1 > UART_COUNT) { ret = TgtUART_INEXIST; }
    if (!uart_is_driver_installed(UART_num)) { ret = TgtUART_NOTINUSE; }
    if (UART_statusData[UART_num].intrCtxObj != NULL && eTaskGetState(UART_statusData[UART_num].intrCtxObj) != eDeleted)
    {
        ret = TgtUART_INTERRUPT_EXISTED;
    }
    char task_str[20];
    BaseType_t xReturned;
    if (ret == TgtUART_OK)
    {
        UART_CALLBACK_FUNCTIONS[UART_num] = cb_func;
        sprintf(task_str, "uart_event_task-%d", UART_num);
        xReturned = xTaskCreate(UART_ISR_espressif, task_str, 3072, ( void * ) UART_num, EHS_PRI_UART, &(UART_statusData[UART_num].intrCtxObj));
        if (xReturned != pdPASS)
        {
            UART_statusData[UART_num].intrCtxObj = NULL;
            ret = TgtUART_INTERRUPT_REG_ERROR;
        }
#ifdef TARGET_ESP32S3_UART_DEBUG
        ESP_LOGI(TAG, "TgtUart_Intr_register OK");
#endif
    }
    return ret;
}

/*
 * UART Interrupt Routine
 * It will response on receive or the other event. Actions defined in Common code
 *  will extract the received message, or dealing with other scenarios.
 * */
void UART_ISR_espressif(void *pvParameters)
{
    int UART_num = (int) pvParameters;
    uart_event_t event;
    size_t buffered_size = UART_RX_BUFFER_SIZE + 1;
    uint8_t* dtmp = (uint8_t*) malloc(buffered_size);
    int length;
    // Temporary point for reallocation error handling
    uint8_t* tmp;
    // Error code for error handling at the end of each loop
    int err = TgtUART_OK;
    // Task notification value
    uint32_t notify_value = 0;
    while (1)
    {
        // Get notify value to know whether to delete current task (break current loop)
        notify_value = ulTaskNotifyTake( pdTRUE, 0 );
        if (notify_value == NOTIFY_TgtUART_ISR_DELETE) break;

        err = TgtUART_OK;
        if (xQueueReceive(UART_statusData[UART_num].intrObj, (void * )&event, 1000 / portTICK_PERIOD_MS))
        {
            bzero(dtmp, buffered_size);
            ESP_LOGI(TAG, "uart[%d] event:", UART_num);
            switch (event.type)
            {
                // Event of UART receiving data
                case UART_DATA:
                    if (event.size > buffered_size)
                    {
                        ESP_LOGW(TAG, "UART data size is greater than the configured size.");
                        buffered_size = event.size + 1;
                        if ((tmp = (uint8_t *) realloc(dtmp, buffered_size)) != NULL) dtmp = tmp;
                        else err = TgtUART_HEAP_INSUFFICIENT;
                    }
                    ESP_LOGI(TAG, "[UART DATA SIZE]: %d", event.size);
                    if ((length = uart_read_bytes(UART_num, dtmp, event.size, 1000 / portTICK_PERIOD_MS)) == -1)
                    {
                        err = TgtUART_ERROR;
                        break;
                    }
                    /**************************************************************************/
                    /* Insert DATA received callback function defined in the Common code here */
                    /**************************************************************************/
#ifdef TARGET_ESP32S3_UART_DEBUG
                    ESP_LOGI(TAG, "[UART DATA]: [%s]", dtmp);
#endif
                    UART_CALLBACK_FUNCTIONS[UART_num]((char *)dtmp, length);
                    break;

                // When HW FIFO overflow detected
                case UART_FIFO_OVF:
                    err = TgtUART_BUFFER_FULL;
                    ESP_LOGW(TAG, "UART FIFO overflow");
                    uart_flush_input(UART_num);
                    xQueueReset(UART_statusData[UART_num].intrObj);
                    break;

                // Event of UART ring buffer full
                case UART_BUFFER_FULL:
                    err = TgtUART_BUFFER_FULL;
                    ESP_LOGW(TAG, "UART ring buffer full");
                    // TODO: increase buffer size
                    // TODO: proper handling the buffer full event
                    uart_flush_input(UART_num);
                    xQueueReset(UART_statusData[UART_num].intrObj);
                    break;

                // Event of UART RX break detected
                case UART_BREAK:
                    break;

                // Event of UART parity check error
                case UART_PARITY_ERR:
                    err = TgtUART_DATA_ERROR;
                    ESP_LOGW(TAG, "UART parity check error");
                    break;

                // Event of UART Frame error
                case UART_FRAME_ERR:
                    err = TgtUART_DATA_ERROR;
                    ESP_LOGW(TAG, "UART Frame error");
                    break;

                // Other UART events
                default:
                    err = TgtUART_EVENT_UNDEFINED;
                    ESP_LOGW(TAG, "Undefined UART event");
                    break;
            }
        }
        // Error handling at the end of each loop
        if (err != TgtUART_OK)
        {
            // TODO: add proper error handling
        }
    }
    // Task deletion cleanup
    free(dtmp);
    dtmp = NULL;
    tmp = NULL;
    vTaskDelete(NULL);
}
