/*
 * SPDX-FileCopyrightText: 2013 Armink
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * SPDX-FileContributor: 2016-2021 Espressif Systems (Shanghai) CO LTD
 */
/*
 * FreeModbus Libary: ESP32 Port
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * IF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id: portserial.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions $
 */

#include <string.h>
#include "driver/uart.h"
#include "soc/dport_access.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "sdkconfig.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "port.h"
#include "mbport.h"
#include "mb_m.h"
#include "mbrtu.h"
#include "mbconfig.h"
#include "port_serial_master.h"

/* ----------------------- Defines ------------------------------------------*/
#define MB_SERIAL_MASTER_RX_SEMA_TOUT_MS   (1000)
#define MB_SERIAL_MASTER_RX_SEMA_TOUT      (pdMS_TO_TICKS(MB_SERIAL_MASTER_RX_SEMA_TOUT_MS))
#define MB_SERIAL_MASTER_RX_FLUSH_RETRY    (2)

/* ----------------------- Static variables ---------------------------------*/
static const mbCHAR *MBSMTAG = "MB_MASTER_SERIAL";

// A queue to handle UART event.
static QueueHandle_t xMbMUartQueue;
static TaskHandle_t  xMbMSerialTaskHandle;

// The UART hardware port number
static mbUCHAR ucMUartNumber = UART_NUM_MAX - 1;

static mbBOOL bMRxStateEnabled = FALSE; // Receiver enabled flag
static mbBOOL bMTxStateEnabled = FALSE; // Transmitter enabled flag

static SemaphoreHandle_t xMasterSemaRxHandle; // Rx blocking semaphore handle

static mbBOOL xMBMasterPortRxSemaInit( void )
{
    xMasterSemaRxHandle = xSemaphoreCreateBinary();
    MB_PORT_CHECK((xMasterSemaRxHandle != NULL), FALSE , "%s: RX semaphore create failure.", __func__);
    return TRUE;
}

static void vMBMasterPortRxSemaClose( void )
{
    if (xMasterSemaRxHandle) {
        vSemaphoreDelete(xMasterSemaRxHandle);
        xMasterSemaRxHandle = NULL;
    }
}

static mbBOOL xMBMasterPortRxSemaTake( mbLONG lTimeOut )
{
    BaseType_t xStatus = pdTRUE;
    xStatus = xSemaphoreTake(xMasterSemaRxHandle, lTimeOut );
    MB_PORT_CHECK((xStatus == pdTRUE), FALSE , "%s: RX semaphore take failure.", __func__);
    ESP_LOGV(MB_PORT_TAG,"%s:Take RX semaphore (%" PRIu64 " ticks).", __func__, (uint64_t)lTimeOut);
    return TRUE;
}

static void vMBMasterRxSemaRelease( void )
{
    BaseType_t xStatus = pdFALSE;
    xStatus = xSemaphoreGive(xMasterSemaRxHandle);
    if (xStatus != pdTRUE) {
        ESP_LOGD(MB_PORT_TAG,"%s:RX semaphore is free.", __func__);
    }
}

static mbBOOL vMBMasterRxSemaIsBusy( void )
{
    BaseType_t xStatus = pdFALSE;
    xStatus = (uxSemaphoreGetCount(xMasterSemaRxHandle) == 0) ? TRUE : FALSE;
    return xStatus;
}

void vMBMasterRxFlush( void )
{
    size_t xSize = 1;
    esp_err_t xErr = ESP_OK;
    for (int xCount = 0; (xCount < MB_SERIAL_MASTER_RX_FLUSH_RETRY) && xSize; xCount++) {
        xErr = uart_get_buffered_data_len(ucMUartNumber, &xSize);
        MB_PORT_CHECK((xErr == ESP_OK), ; , "mb flush serial fail, error = 0x%x.", (int)xErr);
        BaseType_t xStatus = xQueueReset(xMbMUartQueue);
        if (xStatus) {
            xErr = uart_flush_input(ucMUartNumber);
            MB_PORT_CHECK((xErr == ESP_OK), ; , "mb flush serial fail, error = 0x%x.", (int)xErr);
        }
    }
}

void vMBMasterPortSerialEnable(mbBOOL bRxEnable, mbBOOL bTxEnable)
{
    // This function can be called from xMBRTUTransmitFSM() of different task
    if (bTxEnable) {
        vMBMasterRxFlush();
        bMTxStateEnabled = TRUE;
    } else {
        bMTxStateEnabled = FALSE;
    }
    if (bRxEnable) {
        bMRxStateEnabled = TRUE;
        vMBMasterRxSemaRelease();
        vTaskResume(xMbMSerialTaskHandle); // Resume receiver task
    } else {
        vTaskSuspend(xMbMSerialTaskHandle); // Block receiver task
        bMRxStateEnabled = FALSE;
    }
}

static mbUSHORT usMBMasterPortSerialRxPoll(size_t xEventSize)
{
    mbBOOL xStatus = TRUE;
    mbUSHORT usCnt = 0;

    xStatus = xMBMasterPortRxSemaTake(MB_SERIAL_MASTER_RX_SEMA_TOUT);
    if (xStatus) {
        while(xStatus && (usCnt++ <= xEventSize)) {
            // Call the Modbus stack callback function and let it fill the stack buffers.
            xStatus = pxMBMasterFrameCBByteReceived(); // callback to receive FSM
        }
        // The buffer is transferred into Modbus stack and is not needed here any more
        uart_flush_input(ucMUartNumber);
        ESP_LOGD(MBSMTAG, "Received data: %u(bytes in buffer)", (unsigned)usCnt);
#if !CONFIG_FMB_TIMER_PORT_ENABLED
        vMBMasterSetCurTimerMode(MB_TMODE_T35);
        xStatus = pxMBMasterPortCBTimerExpired();
        if (!xStatus) {
            xMBMasterPortEventPost(EV_MASTER_FRAME_RECEIVED);
            ESP_LOGD(MBSMTAG, "Send additional RX ready event.");
        }
#endif
    } else {
        ESP_LOGE(MBSMTAG, "%s: bRxState disabled but junk data (%u bytes) received. ", 
                            __func__, (unsigned)xEventSize);
    }
    return usCnt;
}

mbBOOL xMBMasterPortSerialTxPoll(void)
{
    mbUSHORT usCount = 0;
    mbBOOL bNeedPoll = TRUE;

    if( bMTxStateEnabled ) {
        // Continue while all response bytes put in buffer or out of buffer
        while(bNeedPoll && (usCount++ < MB_SERIAL_BUF_SIZE)) {
            // Calls the modbus stack callback function to let it fill the UART transmit buffer.
            bNeedPoll = pxMBMasterFrameCBTransmitterEmpty( ); // callback to transmit FSM
        }
        ESP_LOGD(MBSMTAG, "MB_TX_buffer sent: (%u) bytes.", (unsigned)(usCount - 1));
        // Waits while UART sending the packet
        esp_err_t xTxStatus = uart_wait_tx_done(ucMUartNumber, MB_SERIAL_TX_TOUT_TICKS);
        vMBMasterPortSerialEnable(TRUE, FALSE);
        MB_PORT_CHECK((xTxStatus == ESP_OK), FALSE, "mb serial sent buffer failure.");
        return TRUE;
    }
    return FALSE;
}

// UART receive event task
static void vUartMTask(void* pvParameters)
{
    uart_event_t xEvent;
    mbUSHORT usResult = 0;
    for(;;) {
        if (xMBPortSerialWaitEvent(xMbMUartQueue, (void*)&xEvent, portMAX_DELAY)) {
            ESP_LOGD(MBSMTAG, "MB_uart[%u] event:", (unsigned)ucMUartNumber);
            switch(xEvent.type) {
                //Event of UART receiving data
                case UART_DATA:
                    ESP_LOGD(MBSMTAG,"Data event, len: %u.", (unsigned)xEvent.size);
                    // This flag set in the event means that no more
                    // data received during configured timeout and UART TOUT feature is triggered
                    if (xEvent.timeout_flag) {
                        // Response is received but previous packet processing is pending
                        // Do not wait completion of processing and just discard received data as incorrect
                        if (vMBMasterRxSemaIsBusy()) {
                            vMBMasterRxFlush();
                            break;
                        }
                        // Get buffered data length
                        ESP_ERROR_CHECK(uart_get_buffered_data_len(ucMUartNumber, &xEvent.size));
                        // Read received data and send it to modbus stack
                        usResult = usMBMasterPortSerialRxPoll(xEvent.size);
                        ESP_LOGD(MBSMTAG,"Timeout occured, processed: %u bytes", (unsigned)usResult);
                    }
                    break;
                //Event of HW FIFO overflow detected
                case UART_FIFO_OVF:
                    ESP_LOGD(MBSMTAG, "hw fifo overflow.");
                    xQueueReset(xMbMUartQueue);
                    break;
                //Event of UART ring buffer full
                case UART_BUFFER_FULL:
                    ESP_LOGD(MBSMTAG, "ring buffer full.");
                    xQueueReset(xMbMUartQueue);
                    uart_flush_input(ucMUartNumber);
                    break;
                //Event of UART RX break detected
                case UART_BREAK:
                    ESP_LOGD(MBSMTAG, "uart rx break.");
                    break;
                //Event of UART parity check error
                case UART_PARITY_ERR:
                    ESP_LOGD(MBSMTAG, "uart parity error.");
                    xQueueReset(xMbMUartQueue);
                    uart_flush_input(ucMUartNumber);
                    break;
                //Event of UART frame error
                case UART_FRAME_ERR:
                    ESP_LOGD(MBSMTAG, "uart frame error.");
                    xQueueReset(xMbMUartQueue);
                    uart_flush_input(ucMUartNumber);
                    break;
                default:
                    ESP_LOGD(MBSMTAG, "uart event type: %u.", (unsigned)xEvent.type);
                    break;
            }
        }
    }
    vTaskDelete(NULL);
}

/* ----------------------- Start implementation -----------------------------*/
mbBOOL xMBMasterPortSerialInit( mbUCHAR ucPORT, mbULONG ulBaudRate, mbUCHAR ucDataBits, eMBParity eParity )
{
    esp_err_t xErr = ESP_OK;
    // Set communication port number
    ucMUartNumber = ucPORT;
    // Configure serial communication parameters
    mbUCHAR ucParity = UART_PARITY_DISABLE;
    mbUCHAR ucData = UART_DATA_8_BITS;
    switch(eParity){
        case MB_PAR_NONE:
            ucParity = UART_PARITY_DISABLE;
            break;
        case MB_PAR_ODD:
            ucParity = UART_PARITY_ODD;
            break;
        case MB_PAR_EVEN:
            ucParity = UART_PARITY_EVEN;
            break;
        default:
            ESP_LOGE(MBSMTAG, "Incorrect parity option: %u", (unsigned)eParity);
            return FALSE;
    }
    switch(ucDataBits){
        case 5:
            ucData = UART_DATA_5_BITS;
            break;
        case 6:
            ucData = UART_DATA_6_BITS;
            break;
        case 7:
            ucData = UART_DATA_7_BITS;
            break;
        case 8:
            ucData = UART_DATA_8_BITS;
            break;
        default:
            ucData = UART_DATA_8_BITS;
            break;
    }
    uart_config_t xUartConfig = {
        .baud_rate = ulBaudRate,
        .data_bits = ucData,
        .parity = ucParity,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 2,
#if (ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0))
        .source_clk = UART_SCLK_DEFAULT,
#else
        .source_clk = UART_SCLK_APB,
#endif
    };
    // Set UART config
    xErr = uart_param_config(ucMUartNumber, &xUartConfig);
    MB_PORT_CHECK((xErr == ESP_OK),
            FALSE, "mb config failure, uart_param_config() returned (0x%x).", (int)xErr);
    // Install UART driver, and get the queue.
    xErr = uart_driver_install(ucMUartNumber, MB_SERIAL_BUF_SIZE, MB_SERIAL_BUF_SIZE,
                                    MB_QUEUE_LENGTH, &xMbMUartQueue, MB_PORT_SERIAL_ISR_FLAG);
    MB_PORT_CHECK((xErr == ESP_OK), FALSE,
            "mb serial driver failure, uart_driver_install() returned (0x%x).", (int)xErr);
    // Set timeout for TOUT interrupt (T3.5 modbus time)
    xErr = uart_set_rx_timeout(ucMUartNumber, MB_SERIAL_TOUT);
    MB_PORT_CHECK((xErr == ESP_OK), FALSE,
            "mb serial set rx timeout failure, uart_set_rx_timeout() returned (0x%x).", (int)xErr);

    // Set always timeout flag to trigger timeout interrupt even after rx fifo full
    uart_set_always_rx_timeout(ucMUartNumber, true);
    MB_PORT_CHECK((xMBMasterPortRxSemaInit()), FALSE,
                        "mb serial RX semaphore create fail.");
    // Create a task to handle UART events
    BaseType_t xStatus = xTaskCreatePinnedToCore(vUartMTask, "uart_queue_task",
                                                    MB_SERIAL_TASK_STACK_SIZE,
                                                    NULL, MB_SERIAL_TASK_PRIO,
                                                    &xMbMSerialTaskHandle, MB_PORT_TASK_AFFINITY);
    if (xStatus != pdPASS) {
        vTaskDelete(xMbMSerialTaskHandle);
        // Force exit from function with failure
        MB_PORT_CHECK(FALSE, FALSE,
                "mb stack serial task creation error. xTaskCreate() returned (0x%x).", (int)xStatus);
    } else {
        vTaskSuspend(xMbMSerialTaskHandle); // Suspend serial task while stack is not started
    }
    ESP_LOGD(MB_PORT_TAG,"%s Init serial.", __func__);
    return TRUE;
}

void vMBMasterPortSerialClose(void)
{
    vMBMasterPortRxSemaClose();
    (void)vTaskDelete(xMbMSerialTaskHandle);
    ESP_ERROR_CHECK(uart_driver_delete(ucMUartNumber));
}

mbBOOL xMBMasterPortSerialPutByte(mbCHAR ucByte)
{
    // Send one byte to UART transmission buffer
    // This function is called by Modbus stack
    mbUCHAR ucLength = uart_write_bytes(ucMUartNumber, &ucByte, 1);
    return (ucLength == 1);
}

// Get one byte from intermediate RX buffer
mbBOOL xMBMasterPortSerialGetByte(mbCHAR* pucByte)
{
    assert(pucByte != NULL);
    mbUSHORT usLength = uart_read_bytes(ucMUartNumber, (uint8_t*)pucByte, 1, MB_SERIAL_RX_TOUT_TICKS);
    return (usLength == 1);
}
