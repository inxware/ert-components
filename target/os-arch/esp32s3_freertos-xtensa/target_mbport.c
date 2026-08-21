/* (c) inx limited 2020

TODO2025 - this needs to be moved to a component HAL.

*/
#include "target_mbconfig.h"
#include "target_mbport.h"
#include "mbport.h"
#include "mb.h"
#include "mb_m.h"
#include "mbframe.h"
#include "portcallbacks.h"

/* ----------------------- System includes ----------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdatomic.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include "freertos/semphr.h"
#include "driver/uart.h"
#include "soc/uart_periph.h"
#include "driver/gpio.h"
#include "esp_log.h"        // for esp_log
#include "esp_err.h"        // for ESP_ERROR_CHECK macro
#include "soc/dport_access.h"
#include "esp_timer.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "net/if.h"
#include "esp_netif.h"
#include "target_mbport_port.h"
#include "target_types.h"
#include "target_uart.h"



/* ----------------------- Variables ----------------------------------------*/
static QueueHandle_t xQueueEventHdl;
static QueueHandle_t xQueueEventMasterHdl;
static _lock_t s_port_lock;
static mbUCHAR ucPortMode = 0;

/* ----------------------- Supporting functions -----------------------------*/

inline void
vMBPortEnterCritical(void)
{
    _lock_acquire(&s_port_lock);
}

inline void
vMBPortExitCritical(void)
{
    _lock_release(&s_port_lock);
}

mbUCHAR
ucMBPortGetMode( void )
{
    return ucPortMode;
}

void
vMBPortSetMode( mbUCHAR ucMode )
{
    ENTER_CRITICAL_SECTION();
    ucPortMode = ucMode;
    EXIT_CRITICAL_SECTION();
}

#if MB_MASTER_RTU_ENABLED || MB_MASTER_ASCII_ENABLED || MB_SLAVE_RTU_ENABLED || MB_SLAVE_ASCII_ENABLED

mbBOOL xMBPortSerialWaitEvent(QueueHandle_t xMbUartQueue_, uart_event_t* pxEvent, mbULONG xTimeout)
{
    mbBOOL xResult = (BaseType_t)xQueueReceive(xMbUartQueue_, (void*)pxEvent, (TickType_t) xTimeout);
    ESP_LOGD(MB_PORT_TAG, "%s, UART event: %u ", __func__, (unsigned)pxEvent->type);
    return xResult;
}

#endif

#if MB_TCP_DEBUG

// This function is kept to realize legacy freemodbus frame logging functionality
void
prvvMBTCPLogFrame( const mbCHAR * pucMsg, mbUCHAR * pucFrame, mbUSHORT usFrameLen )
{
    int             i;
    int             res = 0;
    int             iBufPos = 0;
    size_t          iBufLeft = MB_TCP_FRAME_LOG_BUFSIZE;
    static mbCHAR     arcBuffer[MB_TCP_FRAME_LOG_BUFSIZE];

    assert( pucFrame != NULL );

    for ( i = 0; i < usFrameLen; i++ ) {
        // Print some additional frame information.
        switch ( i )
        {
        case 0:
            // TID = Transaction Identifier.
            res = snprintf( &arcBuffer[iBufPos], iBufLeft, "| TID = " );
            break;
        case 2:
            // PID = Protocol Identifier.
            res = snprintf( &arcBuffer[iBufPos], iBufLeft, " | PID = " );
            break;
        case 4:
            // Length
            res = snprintf( &arcBuffer[iBufPos], iBufLeft, " | LEN = " );
            break;
        case 6:
            // UID = Unit Identifier.
            res = snprintf( &arcBuffer[iBufPos], iBufLeft, " | UID = " );
            break;
        case 7:
            // MB Function Code.
            res = snprintf( &arcBuffer[iBufPos], iBufLeft, " | FUNC = " );
            break;
        case 8:
            // MB PDU rest.
            res = snprintf( &arcBuffer[iBufPos], iBufLeft, " | DATA = " );
            break;
        default:
            res = 0;
            break;
        }
        if( res == -1 ) {
            break;
        }
        else {
            iBufPos += res;
            iBufLeft -= res;
        }

        // Print the data.
        res = snprintf( &arcBuffer[iBufPos], iBufLeft, "%02X", pucFrame[i] );
        if( res == -1 ) {
            break;
        } else {
            iBufPos += res;
            iBufLeft -= res;
        }
    }

    if( res != -1 ) {
        // Append an end of frame string.
        res = snprintf( &arcBuffer[iBufPos], iBufLeft, " |" );
        if( res != -1 ) {
            ESP_LOGD(pucMsg, "%s", arcBuffer);
        }
    }
}
#endif

mbBOOL
xMBPortEventInit( void )
{
    mbBOOL bStatus = FALSE;
    if ((xQueueEventHdl = xQueueCreate(MB_EVENT_QUEUE_SIZE, sizeof(eMBEventType))) != NULL)
    {
        vQueueAddToRegistry(xQueueEventHdl, "MbPortEventQueue");
        bStatus = TRUE;
    }
    return bStatus;
}

void
vMBPortEventClose( void )
{
    if(xQueueEventHdl != NULL)
    {
        vQueueDelete(xQueueEventHdl);
        xQueueEventHdl = NULL;
    }
}

mbBOOL MB_PORT_ISR_ATTR
xMBPortEventPost( eMBEventType eEvent )
{
    BaseType_t xStatus, xHigherPriorityTaskWoken = pdFALSE;
    assert(xQueueEventHdl != NULL);

    if( (mbBOOL)xPortInIsrContext() == TRUE )
    {
        xStatus = xQueueSendFromISR(xQueueEventHdl, (const void*)&eEvent, &xHigherPriorityTaskWoken);
        if ( xHigherPriorityTaskWoken )
        {
            portYIELD_FROM_ISR();
        }
        if (xStatus != pdTRUE) {
            ESP_EARLY_LOGV(MB_PORT_TAG, "%s: Post message failure = %u.", __func__, (unsigned)xStatus);
            return FALSE;
        }
    }
    else
    {
        xStatus = xQueueSend(xQueueEventHdl, (const void*)&eEvent, MB_EVENT_QUEUE_TIMEOUT);
        MB_PORT_CHECK((xStatus == pdTRUE), FALSE, "%s: Post message failure.", __func__);
    }
    return TRUE;
}

mbBOOL
xMBPortEventGet(  /*@out@ */ eMBEventType * peEvent )
{
    assert(xQueueEventHdl != NULL);
    mbBOOL xEventHappened = FALSE;

    if (xQueueReceive(xQueueEventHdl, peEvent, portMAX_DELAY) == pdTRUE) {
        xEventHappened = TRUE;
    }
    return xEventHappened;
}

#if MB_MASTER_RTU_ENABLED || MB_MASTER_ASCII_ENABLED || MB_MASTER_TCP_ENABLED

// Event bit mask for eMBMasterWaitRequestFinish()
#define MB_EVENT_REQ_MASK   (EventBits_t)( EV_MASTER_PROCESS_SUCCESS | \
                                            EV_MASTER_ERROR_RESPOND_TIMEOUT | \
                                            EV_MASTER_ERROR_RECEIVE_DATA | \
                                            EV_MASTER_ERROR_EXECUTE_FUNCTION )

/* ----------------------- Variables ----------------------------------------*/
static SemaphoreHandle_t xResourceMasterHdl;
static EventGroupHandle_t xEventGroupMasterHdl;
static EventGroupHandle_t xEventGroupMasterConfirmHdl;
static QueueHandle_t xQueueMasterHdl;

static uint64_t xTransactionID = 0;

mbBOOL
xMBMasterPortEventInit( void )
{
    xEventGroupMasterHdl = xEventGroupCreate();
    xEventGroupMasterConfirmHdl = xEventGroupCreate();
    MB_PORT_CHECK((xEventGroupMasterHdl != NULL) && (xEventGroupMasterConfirmHdl != NULL),
                    FALSE, "mb stack event group creation error.");
    xQueueMasterHdl = xQueueCreate(MB_EVENT_QUEUE_SIZE, sizeof(xMBMasterEventType));
    MB_PORT_CHECK(xQueueMasterHdl, FALSE, "mb stack event group creation error.");
    vQueueAddToRegistry(xQueueMasterHdl, "MbMasterPortEventQueue");
    xTransactionID = 0;
    return TRUE;
}

mbBOOL MB_PORT_ISR_ATTR
xMBMasterPortEventPost( eMBMasterEventEnum eEvent )
{
    BaseType_t xStatus, xHigherPriorityTaskWoken = pdFALSE;
    assert(xQueueMasterHdl != NULL);
    xMBMasterEventType xEvent;
    xEvent.xPostTimestamp = esp_timer_get_time();
    
    if (eEvent & EV_MASTER_TRANS_START) {
        atomic_store(&(xTransactionID), xEvent.xPostTimestamp);
    }
    xEvent.eEvent = (eEvent & ~EV_MASTER_TRANS_START);

    if( (mbBOOL)xPortInIsrContext() == TRUE ) {
        xStatus = xQueueSendFromISR(xQueueMasterHdl, (const void*)&xEvent, &xHigherPriorityTaskWoken);
        if ( xHigherPriorityTaskWoken ) {
            portYIELD_FROM_ISR();
        }
        if (xStatus != pdTRUE) {
            ESP_EARLY_LOGV(MB_PORT_TAG, "%s: Post message failure = %d.", __func__, xStatus);
            return FALSE;
        }
    } else {
        xStatus = xQueueSend(xQueueMasterHdl, (const void*)&xEvent, MB_EVENT_QUEUE_TIMEOUT);
        MB_PORT_CHECK((xStatus == pdTRUE), FALSE, "%s: Post message failure.", __func__);
    }
    return TRUE;
}

mbBOOL
xMBMasterPortEventGet(  /*@out@ */ xMBMasterEventType * peEvent )
{
    assert(xQueueMasterHdl != NULL);
    mbBOOL xEventHappened = FALSE;

    if (xQueueReceive(xQueueMasterHdl, peEvent, portMAX_DELAY) == pdTRUE) {
        peEvent->xTransactionId = atomic_load(&xTransactionID);
        // Set event bits in confirmation group (for synchronization with port task)
        xEventGroupSetBits(xEventGroupMasterConfirmHdl, peEvent->eEvent);
        peEvent->xGetTimestamp = esp_timer_get_time();
        xEventHappened = TRUE;
    }
    return xEventHappened;
}

eMBMasterEventEnum
xMBMasterPortFsmWaitConfirmation( eMBMasterEventEnum eEventMask, mbULONG ulTimeout)
{
    EventBits_t uxBits;
    uxBits = xEventGroupWaitBits( xEventGroupMasterConfirmHdl,  // The event group being tested.
                                    eEventMask,                 // The bits within the event group to wait for.
                                    pdFALSE,                    // Keep masked bits.
                                    pdFALSE,                    // Don't wait for both bits, either bit will do.
                                    ulTimeout);                 // Wait timeout for either bit to be set.
    if (ulTimeout && uxBits) {
        // Clear confirmation events that where set in the mask
        xEventGroupClearBits( xEventGroupMasterConfirmHdl, (uxBits & eEventMask) );
    }
    return (eMBMasterEventEnum)(uxBits & eEventMask);
}

// This function is initialize the OS resource for modbus master.
void
vMBMasterOsResInit( void )
{
    xResourceMasterHdl = xSemaphoreCreateBinary();
    MB_PORT_CHECK((xResourceMasterHdl != NULL), ; , "%s: Resource create error.", __func__);
}

/**
 * This function is take Mobus Master running resource.
 * Note:The resource is define by Operating System.
 *
 * @param lTimeOut the waiting time.
 *
 * @return resource take result
 */
mbBOOL
xMBMasterRunResTake( mbLONG lTimeOut )
{
    BaseType_t xStatus = pdTRUE;
    xStatus = xSemaphoreTake( xResourceMasterHdl, lTimeOut );
    MB_PORT_CHECK((xStatus == pdTRUE), FALSE , "%s: Resource take failure.", __func__);
    ESP_LOGD(MB_PORT_TAG,"%s:Take MB resource (%lu ticks).", __func__, lTimeOut);
    return TRUE;
}

/**
 * This function is release Modbus Master running resource.
 * Note:The resource is define by Operating System. If you not use OS this function can be empty.
 */
void
vMBMasterRunResRelease( void )
{
    BaseType_t xStatus = pdFALSE;
    xStatus = xSemaphoreGive( xResourceMasterHdl );
    if (xStatus != pdTRUE) {
        ESP_LOGD(MB_PORT_TAG,"%s: Release resource fail.", __func__);
    }
}

uint64_t
xMBMasterPortGetTransactionId( void )
{
    return atomic_load(&xTransactionID);
}

/************ Target Specific Implementation ************/
/**
 * This is modbus master respond timeout error process callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 *
 * @param ucDestAddress destination salve address
 * @param pucPDUData PDU buffer data
 * @param ucPDULength PDU buffer length
 *
 */
void vMBMasterErrorCBRespondTimeout(mbUCHAR ucDestAddress, const mbUCHAR* pucPDUData, mbUSHORT ucPDULength)
{
    (void)xEventGroupSetBits( xEventGroupMasterHdl, EV_MASTER_ERROR_RESPOND_TIMEOUT );
    ESP_LOGD(MB_PORT_TAG,"%s:Callback respond timeout.", __func__);
}

/**
 * This is modbus master receive data error process callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 *
 * @param ucDestAddress destination salve address
 * @param pucPDUData PDU buffer data
 * @param ucPDULength PDU buffer length
 */
void vMBMasterErrorCBReceiveData(mbUCHAR ucDestAddress, const mbUCHAR* pucPDUData, mbUSHORT ucPDULength)
{
    (void)xEventGroupSetBits( xEventGroupMasterHdl, EV_MASTER_ERROR_RECEIVE_DATA );
    ESP_LOGD(MB_PORT_TAG,"%s:Callback receive data timeout failure.", __func__);
    ESP_LOG_BUFFER_HEX_LEVEL("Err rcv buf", (void *)pucPDUData, (mbUSHORT)ucPDULength, ESP_LOG_DEBUG);
}

/**
 * This is modbus master execute function error process callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 * @param ucDestAddress destination salve address
 * @param pucPDUData PDU buffer data
 * @param ucPDULength PDU buffer length
 *
 */
void vMBMasterErrorCBExecuteFunction(mbUCHAR ucDestAddress, const mbUCHAR* pucPDUData, mbUSHORT ucPDULength)
{
    xEventGroupSetBits( xEventGroupMasterHdl, EV_MASTER_ERROR_EXECUTE_FUNCTION );
    ESP_LOGD(MB_PORT_TAG,"%s:Callback execute data handler failure.", __func__);
    ESP_LOG_BUFFER_HEX_LEVEL("Exec func buf", (void*)pucPDUData, (mbUSHORT)ucPDULength, ESP_LOG_DEBUG);
}

/**
 * This is modbus master request process success callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system. Do not execute too much waiting process.
 */
void vMBMasterCBRequestSuccess( void ) 
{
    (void)xEventGroupSetBits( xEventGroupMasterHdl, EV_MASTER_PROCESS_SUCCESS );
    ESP_LOGD(MB_PORT_TAG,"%s: Callback request success.", __func__);
}

/**
 * This function is wait for modbus master request finish and return result.
 * Waiting result include request process success, request respond timeout,
 * receive data error and execute function error.You can use the above callback function.
 * @note If you are use OS, you can use OS's event mechanism. Otherwise you have to run
 * much user custom delay for waiting.
 *
 * @return request error code
 */
eMBMasterReqErrCode eMBMasterWaitRequestFinish( void ) {
    eMBMasterReqErrCode eErrStatus = MB_MRE_NO_ERR;
    eMBMasterEventEnum xRecvedEvent;

    EventBits_t uxBits = xEventGroupWaitBits( xEventGroupMasterHdl, // The event group being tested.
                                                MB_EVENT_REQ_MASK,  // The bits within the event group to wait for.
                                                pdTRUE,             // Masked bits should be cleared before returning.
                                                pdFALSE,            // Don't wait for both bits, either bit will do.
                                                portMAX_DELAY );    // Wait forever for either bit to be set.
    xRecvedEvent = (eMBMasterEventEnum)(uxBits);
    if (xRecvedEvent) {
        ESP_LOGD(MB_PORT_TAG,"%s: returned event = 0x%x", __func__, (int)xRecvedEvent);
        if (!(xRecvedEvent & MB_EVENT_REQ_MASK)) {
            // if we wait for certain event bits but get from poll subset
            ESP_LOGE(MB_PORT_TAG,"%s: incorrect event set = 0x%x", __func__, (int)xRecvedEvent);
        }
        xEventGroupSetBits( xEventGroupMasterConfirmHdl, (xRecvedEvent & MB_EVENT_REQ_MASK) );
        if (MB_PORT_CHECK_EVENT(xRecvedEvent, EV_MASTER_PROCESS_SUCCESS)) {
            eErrStatus = MB_MRE_NO_ERR;
        } else if (MB_PORT_CHECK_EVENT(xRecvedEvent, EV_MASTER_ERROR_RESPOND_TIMEOUT)) {
            eErrStatus = MB_MRE_TIMEDOUT;
        } else if (MB_PORT_CHECK_EVENT(xRecvedEvent, EV_MASTER_ERROR_RECEIVE_DATA)) {
            eErrStatus = MB_MRE_REV_DATA;
        } else if (MB_PORT_CHECK_EVENT(xRecvedEvent, EV_MASTER_ERROR_EXECUTE_FUNCTION)) {
            eErrStatus = MB_MRE_EXE_FUN;
        }
    } else {
        ESP_LOGE(MB_PORT_TAG,"%s: Incorrect event or timeout xRecvedEvent = 0x%x", __func__, (int)uxBits);
        // https://github.com/espressif/esp-idf/issues/5275
        // if a no event is received, that means vMBMasterPortEventClose()
        // has been closed, so event group has been deleted by FreeRTOS, which
        // triggers the send of 0 value to the event group to unlock this task
        // waiting on it. For this patch, handles it as a time out without assert.
        eErrStatus = MB_MRE_TIMEDOUT;
    }
    return eErrStatus;
}

void vMBMasterPortEventClose(void)
{
    if (xEventGroupMasterHdl) {
        vEventGroupDelete(xEventGroupMasterHdl);
        xEventGroupMasterHdl = NULL;
    }
    if (xQueueMasterHdl) {
        vQueueDelete(xQueueMasterHdl);
        xQueueMasterHdl = NULL;
    }
    if (xEventGroupMasterConfirmHdl) {
        vEventGroupDelete(xEventGroupMasterConfirmHdl);
        xEventGroupMasterConfirmHdl = NULL;
    }
    if (xResourceMasterHdl) {
        vSemaphoreDelete(xResourceMasterHdl);
        xResourceMasterHdl = NULL;
    }
}
/************ Target Specific Implementation ************/

#endif // MB_MASTER_RTU_ENABLED || MB_MASTER_ASCII_ENABLED || MB_MASTER_TCP_ENABLED
/* ----------------------- Serial port functions ----------------------------*/

// A queue to handle UART event.
static QueueHandle_t xMbUartQueue;
static TaskHandle_t  xMbSerialTaskHandle;
static const mbCHAR *MBSTAG = "MB_SERIAL";

// The UART hardware port number
static mbUCHAR ucUartNumber = UART_NUM_MAX - 1;

static mbBOOL bRxStateEnabled = FALSE; // Receiver enabled flag
static mbBOOL bTxStateEnabled = FALSE; // Transmitter enabled flag

/********************** Target Specific START ************************/
static mbUSHORT usMBPortSerialRxPoll(size_t xEventSize)
{
    mbBOOL xReadStatus = TRUE;
    mbUSHORT usCnt = 0;

    if (bRxStateEnabled) {
        // Get received packet into Rx buffer
        while(xReadStatus && (usCnt++ <= xEventSize)) {
            // Call the Modbus stack callback function and let it fill the buffers.
            xReadStatus = pxMBFrameCBByteReceived(); // callback to execute receive FSM
        }
        uart_flush_input(ucUartNumber);
        // Send event EV_FRAME_RECEIVED to allow stack process packet
#if !CONFIG_FMB_TIMER_PORT_ENABLED
        pxMBPortCBTimerExpired();
#endif
        ESP_LOGD(MBSTAG, "RX: %u bytes\n", (unsigned)usCnt);
    }
    return usCnt;
}

mbBOOL xMBPortSerialTxPoll(void)
{
    mbUSHORT usCount = 0;
    mbBOOL bNeedPoll = TRUE;

    if( bTxStateEnabled ) {
        // Continue while all response bytes put in buffer or out of buffer
        while((bNeedPoll) && (usCount++ < MB_SERIAL_BUF_SIZE)) {
            // Calls the modbus stack callback function to let it fill the UART transmit buffer.
            bNeedPoll = pxMBFrameCBTransmitterEmpty( ); // callback to transmit FSM
        }
        ESP_LOGD(MBSTAG, "MB_TX_buffer send: (%u) bytes\n", (unsigned)usCount);
        // Waits while UART sending the packet
        esp_err_t xTxStatus = uart_wait_tx_done(ucUartNumber, MB_SERIAL_TX_TOUT_TICKS);
        vMBPortSerialEnable(TRUE, FALSE);
        MB_PORT_CHECK((xTxStatus == ESP_OK), FALSE, "mb serial sent buffer failure.");
        return TRUE;
    }
    return FALSE;
}

static void vUartTask(void *pvParameters)
{
    uart_event_t xEvent;
    mbUSHORT usResult = 0;
    for(;;) {
        if (xMBPortSerialWaitEvent(xMbUartQueue, (void*)&xEvent, portMAX_DELAY)) {
            ESP_LOGD(MBSTAG, "MB_uart[%u] event:", (unsigned)ucUartNumber);
            switch(xEvent.type) {
                //Event of UART receving data
                case UART_DATA:
                    ESP_LOGD(MBSTAG,"Data event, length: %u", (unsigned)xEvent.size);
                    // This flag set in the event means that no more
                    // data received during configured timeout and UART TOUT feature is triggered
                    if (xEvent.timeout_flag) {
                        // Get buffered data length
                        ESP_ERROR_CHECK(uart_get_buffered_data_len(ucUartNumber, &xEvent.size));
                        // Read received data and send it to modbus stack
                        usResult = usMBPortSerialRxPoll(xEvent.size);
                        ESP_LOGD(MBSTAG,"Timeout occured, processed: %u bytes", (unsigned)usResult);
                    }
                    break;
                //Event of HW FIFO overflow detected
                case UART_FIFO_OVF:
                    ESP_LOGD(MBSTAG, "hw fifo overflow");
                    xQueueReset(xMbUartQueue);
                    break;
                //Event of UART ring buffer full
                case UART_BUFFER_FULL:
                    ESP_LOGD(MBSTAG, "ring buffer full");
                    xQueueReset(xMbUartQueue);
                    uart_flush_input(ucUartNumber);
                    break;
                //Event of UART RX break detected
                case UART_BREAK:
                    ESP_LOGD(MBSTAG, "uart rx break");
                    break;
                //Event of UART parity check error
                case UART_PARITY_ERR:
                    ESP_LOGD(MBSTAG, "uart parity error");
                    break;
                //Event of UART frame error
                case UART_FRAME_ERR:
                    ESP_LOGD(MBSTAG, "uart frame error");
                    break;
                default:
                    ESP_LOGD(MBSTAG, "uart event type: %u", (unsigned)xEvent.type);
                    break;
            }
        }
        vTaskDelay(1);
    }
    vTaskDelete(NULL);
}
/********************** Target Specific ENDED ************************/

mbBOOL
xMBPortSerialInit( mbUCHAR ucPort, mbULONG ulBaudRate,
                    mbUCHAR ucDataBits, eMBParity eParity )
{
    esp_err_t xErr = ESP_OK;
    // Set communication port number
    ucUartNumber = ucPort;
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
            ESP_LOGE(MBSTAG, "Incorrect parity option: %u", (unsigned)eParity);
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
    xErr = uart_param_config(ucUartNumber, &xUartConfig);
    MB_PORT_CHECK((xErr == ESP_OK),
            FALSE, "mb config failure, uart_param_config() returned (0x%x).", (int)xErr);
    // Install UART driver, and get the queue.
    xErr = uart_driver_install(ucUartNumber, MB_SERIAL_BUF_SIZE, MB_SERIAL_BUF_SIZE,
                                    MB_QUEUE_LENGTH, &xMbUartQueue, MB_PORT_SERIAL_ISR_FLAG);
    MB_PORT_CHECK((xErr == ESP_OK), FALSE,
            "mb serial driver failure, uart_driver_install() returned (0x%x).", (int)xErr);
#if !CONFIG_FMB_TIMER_PORT_ENABLED
    // Set timeout for TOUT interrupt (T3.5 modbus time)
    xErr = uart_set_rx_timeout(ucUartNumber, MB_SERIAL_TOUT);
    MB_PORT_CHECK((xErr == ESP_OK), FALSE,
            "mb serial set rx timeout failure, uart_set_rx_timeout() returned (0x%x).", (int)xErr);
#endif
    xErr = uart_set_pin(
        ucUartNumber,
        gTargetUartPinTx[ucUartNumber],
        gTargetUartPinRx[ucUartNumber],
        gTargetUartPinRts[ucUartNumber],
        gTargetUartPinCts[ucUartNumber]);
    MB_PORT_CHECK((xErr == ESP_OK), FALSE,
            "mb serial pin setting failed, uart_set_pin() returned (0x%x).", (int)xErr);

    // Set always timeout flag to trigger timeout interrupt even after rx fifo full
    uart_set_always_rx_timeout(ucUartNumber, true);
    
    uart_set_mode(ucUartNumber, UART_MODE_RS485_HALF_DUPLEX);

    // Create a task to handle UART events
    BaseType_t xStatus = xTaskCreatePinnedToCore(vUartTask, "uart_queue_task",
                                                    MB_SERIAL_TASK_STACK_SIZE,
                                                    NULL, MB_SERIAL_TASK_PRIO,
                                                    &xMbSerialTaskHandle, MB_PORT_TASK_AFFINITY);
    if (xStatus != pdPASS) {
        vTaskDelete(xMbSerialTaskHandle);
        // Force exit from function with failure
        MB_PORT_CHECK(FALSE, FALSE,
                "mb stack serial task creation error. xTaskCreate() returned (0x%x).",
                (int)xStatus);
    } else {
        vTaskSuspend(xMbSerialTaskHandle); // Suspend serial task while stack is not started
    }
    return TRUE;
}

void vMBPortSerialClose(void)
{
    (void)vTaskSuspend(xMbSerialTaskHandle);
    (void)vTaskDelete(xMbSerialTaskHandle);
    ESP_ERROR_CHECK(uart_driver_delete(ucUartNumber));
}

void
vMBPortClose( void )
{
    vMBPortSerialClose(  );
    vMBPortTimerClose(  );
    vMBPortEventClose(  );
}



void
xMBPortSerialClose( void )
{
    (void)vTaskSuspend(xMbSerialTaskHandle);
    (void)vTaskDelete(xMbSerialTaskHandle);
    ESP_ERROR_CHECK(uart_driver_delete(ucUartNumber));
}

void
vMBPortSerialEnable( mbBOOL bRxEnable, mbBOOL bTxEnable )
{
    // This function can be called from xMBRTUTransmitFSM() of different task
    if (bTxEnable) {
        bTxStateEnabled = TRUE;
    } else {
        bTxStateEnabled = FALSE;
    }
    if (bRxEnable) {
        //uart_enable_rx_intr(ucUartNumber);
        bRxStateEnabled = TRUE;
        vTaskResume(xMbSerialTaskHandle); // Resume receiver task
    } else {
        vTaskSuspend(xMbSerialTaskHandle); // Block receiver task
        bRxStateEnabled = FALSE;
    }
}

mbBOOL
xMBPortSerialGetByte( mbCHAR * pucByte )
{
    assert(pucByte != NULL);
    mbUSHORT usLength = uart_read_bytes(ucUartNumber, (uint8_t*)pucByte, 1, MB_SERIAL_RX_TOUT_TICKS);
    return (usLength == 1);
}

mbBOOL
xMBPortSerialPutByte( mbCHAR ucByte )
{
    // Send one byte to UART transmission buffer
    // This function is called by Modbus stack
    mbUCHAR ucLength = uart_write_bytes(ucUartNumber, &ucByte, 1);
    return (ucLength == 1);
}

__attribute__ ((weak))
mbBOOL
xMBPortSerialGetRequest( mbUCHAR **ppucMBSerialFrame, mbUSHORT * pusSerialLength )
{
    ESP_LOGD(MB_PORT_TAG, "%s default", __func__);
    return TRUE;
}

__attribute__ ((weak))
mbBOOL
xMBPortSerialSendResponse( mbUCHAR *pucMBSerialFrame, mbUSHORT usSerialLength )
{
    ESP_LOGD(MB_PORT_TAG, "%s default", __func__);
    return TRUE;
}

#if MB_MASTER_RTU_ENABLED || MB_MASTER_ASCII_ENABLED

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

/********************** Target Specific START ************************/

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
        vTaskDelay(1);
    }
    vTaskDelete(NULL);
}

/********************** Target Specific ENDED ************************/

mbBOOL
xMBMasterPortSerialInit( mbUCHAR ucPort, mbULONG ulBaudRate,
                            mbUCHAR ucDataBits, eMBParity eParity )
{
    esp_err_t xErr = ESP_OK;
    // Set communication port number
    ucMUartNumber = ucPort;
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
    xErr = uart_set_pin(
        ucMUartNumber,
        gTargetUartPinTx[ucMUartNumber],
        gTargetUartPinRx[ucMUartNumber],
        gTargetUartPinRts[ucMUartNumber],
        gTargetUartPinCts[ucMUartNumber]);
    MB_PORT_CHECK((xErr == ESP_OK), FALSE,
            "mb serial pin setting failed, uart_set_pin() returned (0x%x).", (int)xErr);
    // Set timeout for TOUT interrupt (T3.5 modbus time)
    xErr = uart_set_rx_timeout(ucMUartNumber, MB_SERIAL_TOUT);
    MB_PORT_CHECK((xErr == ESP_OK), FALSE,
            "mb serial set rx timeout failure, uart_set_rx_timeout() returned (0x%x).", (int)xErr);

    // Set always timeout flag to trigger timeout interrupt even after rx fifo full
    uart_set_always_rx_timeout(ucMUartNumber, true);
    uart_set_mode(ucMUartNumber, UART_MODE_RS485_HALF_DUPLEX);
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

static xTimerContext_t* pxTimerMContext = NULL;

void vMBMasterPortTimerClose(void)
{
    // Delete active timer
    if (pxTimerMContext) {
        if (pxTimerMContext->xTimerIntHandle) {
            esp_timer_stop(pxTimerMContext->xTimerIntHandle);
            esp_timer_delete(pxTimerMContext->xTimerIntHandle);
        }
        free(pxTimerMContext);
        pxTimerMContext = NULL;
    }
}

void
vMBMasterPortClose( void )
{
    vMBMasterPortSerialClose(  );
    vMBMasterPortTimerClose(  );
    vMBMasterPortEventClose(  );
}

void
xMBMasterPortSerialClose( void )
{
    vMBMasterPortRxSemaClose();
    (void)vTaskDelete(xMbMSerialTaskHandle);
    ESP_ERROR_CHECK(uart_driver_delete(ucMUartNumber));
}

void
vMBMasterPortSerialEnable( mbBOOL bRxEnable, mbBOOL bTxEnable )
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

mbBOOL
xMBMasterPortSerialGetByte( mbCHAR * pucByte )
{
    assert(pucByte != NULL);
    mbUSHORT usLength = uart_read_bytes(ucMUartNumber, (uint8_t*)pucByte, 1, MB_SERIAL_RX_TOUT_TICKS);
    return (usLength == 1);
}

mbBOOL
xMBMasterPortSerialPutByte( mbCHAR ucByte )
{
    // Send one byte to UART transmission buffer
    // This function is called by Modbus stack
    mbUCHAR ucLength = uart_write_bytes(ucMUartNumber, &ucByte, 1);
    return (ucLength == 1);
}

/*
 * The function is called from ASCII/RTU module to get processed data buffer. Sets the
 * received buffer and its length using parameters.
 */
__attribute__ ((weak))
mbBOOL xMBMasterPortSerialGetResponse( mbUCHAR **ppucMBSerialFrame, mbUSHORT * usSerialLength )
{
    ESP_LOGD(MB_PORT_TAG, " %s default", __func__);
    return TRUE;
}

/*
 * The function is called from ASCII/RTU module to set processed data buffer
 * to be sent in transmitter state machine.
 */
__attribute__ ((weak))
mbBOOL xMBMasterPortSerialSendRequest( mbUCHAR *pucMBSerialFrame, mbUSHORT usSerialLength )
{
    ESP_LOGD(MB_PORT_TAG, "%s default", __func__);
    return TRUE;
}

void
vMBMasterRxFlush( void )
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

#endif

/* ----------------------- Timers functions ---------------------------------*/

static const char *MBTTAG = "MBS_TIMER";

static xTimerContext_t* pxTimerContext = NULL;

/********************** Target Specific START ************************/
static void IRAM_ATTR vTimerAlarmCBHandler(void *param)
{
    pxMBPortCBTimerExpired(); // Timer expired callback function
    pxTimerContext->xTimerState = TRUE;
    ESP_EARLY_LOGD(MBTTAG, "Slave timeout triggered.");
}
/********************** Target Specific ENDED ************************/

mbBOOL
xMBPortTimersInit( mbUSHORT usTimeOut50us )
{
#if CONFIG_FMB_TIMER_PORT_ENABLED
    MB_PORT_CHECK((usTimeOut50us > 0), FALSE,
            "Modbus timeout discreet is incorrect.");
    MB_PORT_CHECK(!pxTimerContext, FALSE,
                "Modbus timer is already created.");
    pxTimerContext = calloc(1, sizeof(xTimerContext_t));
    if (!pxTimerContext) {
        return FALSE;
    }
    pxTimerContext->xTimerIntHandle = NULL;
    // Save timer reload value for Modbus T35 period
    pxTimerContext->usT35Ticks = usTimeOut50us;
    esp_timer_create_args_t xTimerConf = {
        .callback = vTimerAlarmCBHandler,
        .arg = NULL,
#if (MB_TIMER_SUPPORTS_ISR_DISPATCH_METHOD && CONFIG_FMB_TIMER_USE_ISR_DISPATCH_METHOD)
        .dispatch_method = ESP_TIMER_ISR,
#else
        .dispatch_method = ESP_TIMER_TASK,
#endif
        .name = "MBS_T35timer"
    };
    // Create Modbus timer
    esp_err_t xErr = esp_timer_create(&xTimerConf, &(pxTimerContext->xTimerIntHandle));
    if (xErr) {
        return FALSE;
    }
#endif
    return TRUE;
}

void
xMBPortTimersClose( void )
{
#if CONFIG_FMB_TIMER_PORT_ENABLED
    // Delete active timer
    if (pxTimerContext) {
        if (pxTimerContext->xTimerIntHandle) {
            esp_timer_stop(pxTimerContext->xTimerIntHandle);
            esp_timer_delete(pxTimerContext->xTimerIntHandle);
        }
        free(pxTimerContext);
        pxTimerContext = NULL;
    }
#endif
}

void vMBPortTimerClose(void)
{
#if CONFIG_FMB_TIMER_PORT_ENABLED
    // Delete active timer
    if (pxTimerContext) {
        if (pxTimerContext->xTimerIntHandle) {
            esp_timer_stop(pxTimerContext->xTimerIntHandle);
            esp_timer_delete(pxTimerContext->xTimerIntHandle);
        }
        free(pxTimerContext);
        pxTimerContext = NULL;
    }
#endif
}

void
vMBPortTimersEnable( void )
{
#if CONFIG_FMB_TIMER_PORT_ENABLED
    MB_PORT_CHECK((pxTimerContext && pxTimerContext->xTimerIntHandle), ; ,
                                "timer is not initialized.");
    uint64_t xToutUs = (pxTimerContext->usT35Ticks * MB_TIMER_TICK_TIME_US);
    esp_timer_stop(pxTimerContext->xTimerIntHandle);
    esp_timer_start_once(pxTimerContext->xTimerIntHandle, xToutUs);
    pxTimerContext->xTimerState = FALSE;
#endif
}

void MB_PORT_ISR_ATTR
vMBPortTimersDisable( void )
{
#if CONFIG_FMB_TIMER_PORT_ENABLED
    // Disable timer alarm
    esp_timer_stop(pxTimerContext->xTimerIntHandle);
#endif
}

void
vMBPortTimersDelay( mbUSHORT usTimeOutMS )
{
    vTaskDelay(usTimeOutMS / portTICK_PERIOD_MS);
}

#if MB_MASTER_RTU_ENABLED || MB_MASTER_ASCII_ENABLED || MB_MASTER_TCP_ENABLED
static const char *MBTMTAG = "MBM_TIMER";

/********************** Target Specific START ************************/
static void IRAM_ATTR vTimerMAlarmCBHandler(void *param)
{
    pxMBMasterPortCBTimerExpired(); // Timer expired callback function
    pxTimerMContext->xTimerState = TRUE;
    ESP_EARLY_LOGD(MBTMTAG, "Timer mode: (%u) triggered", (unsigned)xMBMasterGetCurTimerMode());
}

// Set timer alarm value
static mbBOOL xMBMasterPortTimersEnable(uint64_t xToutUs)
{
    MB_PORT_CHECK(pxTimerMContext && (pxTimerMContext->xTimerIntHandle), FALSE,
                                "timer is not initialized.");
    MB_PORT_CHECK((xToutUs > 0), FALSE,
                            "incorrect tick value for timer = (0x%llu).", xToutUs);
    esp_timer_stop(pxTimerMContext->xTimerIntHandle);
    esp_timer_start_once(pxTimerMContext->xTimerIntHandle, xToutUs);
    pxTimerMContext->xTimerState = FALSE;
    return TRUE;
}
/********************** Target Specific ENDED ************************/

mbBOOL
xMBMasterPortTimersInit( mbUSHORT usTimeOut50us )
{
    MB_PORT_CHECK((usTimeOut50us > 0), FALSE,
            "Modbus timeout discreet is incorrect.");
    MB_PORT_CHECK(!pxTimerMContext, FALSE,
                "Modbus timer is already created.");
    pxTimerMContext = calloc(1, sizeof(xTimerContext_t));
    if (!pxTimerMContext) {
        return FALSE;
    }
    pxTimerMContext->xTimerIntHandle = NULL;
    // Save timer reload value for Modbus T35 period
    pxTimerMContext->usT35Ticks = usTimeOut50us;
    esp_timer_create_args_t xTimerConf = {
        .callback = vTimerMAlarmCBHandler,
        .arg = NULL,
#if (MB_TIMER_SUPPORTS_ISR_DISPATCH_METHOD && CONFIG_FMB_TIMER_USE_ISR_DISPATCH_METHOD)
        .dispatch_method = ESP_TIMER_ISR,
#else
        .dispatch_method = ESP_TIMER_TASK,
#endif
        .name = "MBM_T35timer"
    };
    // Create Modbus timer
    esp_err_t xErr = esp_timer_create(&xTimerConf, &(pxTimerMContext->xTimerIntHandle));
    if (xErr) {
        return FALSE;
    }
    return TRUE;
}

void
xMBMasterPortTimersClose( void )
{
    // Delete active timer
    if (pxTimerMContext) {
        if (pxTimerMContext->xTimerIntHandle) {
            esp_timer_stop(pxTimerMContext->xTimerIntHandle);
            esp_timer_delete(pxTimerMContext->xTimerIntHandle);
        }
        free(pxTimerMContext);
        pxTimerMContext = NULL;
    }
}

void
vMBMasterPortTimersT35Enable( void )
{
    uint64_t xToutUs = (pxTimerMContext->usT35Ticks * MB_TIMER_TICK_TIME_US);

    // Set current timer mode, don't change it.
    vMBMasterSetCurTimerMode(MB_TMODE_T35);
    // Set timer alarm
    (void)xMBMasterPortTimersEnable(xToutUs);
}

void
vMBMasterPortTimersConvertDelayEnable( void )
{
    // Covert time in milliseconds into ticks
    uint64_t xToutUs = (MB_MASTER_DELAY_MS_CONVERT * 1000);

    // Set current timer mode
    vMBMasterSetCurTimerMode(MB_TMODE_CONVERT_DELAY);
    ESP_LOGD(MB_PORT_TAG,"%s Convert delay enable.", __func__);
    (void)xMBMasterPortTimersEnable(xToutUs);
}

void
vMBMasterPortTimersRespondTimeoutEnable( void )
{
    uint64_t xToutUs = (MB_MASTER_TIMEOUT_MS_RESPOND * 1000);

    vMBMasterSetCurTimerMode(MB_TMODE_RESPOND_TIMEOUT);
    ESP_LOGD(MB_PORT_TAG,"%s Respond enable timeout.", __func__);
    (void)xMBMasterPortTimersEnable(xToutUs);
}

void MB_PORT_ISR_ATTR
vMBMasterPortTimersDisable( void )
{
    // Disable timer alarm
    esp_timer_stop(pxTimerMContext->xTimerIntHandle);
}

#endif
/* ----------------------- Callback for the protocol stack ------------------*/
/*!
 * \brief Callback function for the porting layer when a new byte is
 *   available.
 *
 * Depending upon the mode this callback function is used by the RTU or
 * ASCII transmission layers. In any case a call to xMBPortSerialGetByte()
 * must immediately return a new character.
 *
 * \return <code>TRUE</code> if a event was posted to the queue because
 *   a new byte was received. The port implementation should wake up the
 *   tasks which are currently blocked on the eventqueue.
 */
extern          mbBOOL( *pxMBFrameCBByteReceived ) ( void );

extern          mbBOOL( *pxMBFrameCBTransmitterEmpty ) ( void );

extern          mbBOOL( *pxMBPortCBTimerExpired ) ( void );

#if MB_MASTER_RTU_ENABLED || MB_MASTER_ASCII_ENABLED || MB_MASTER_TCP_ENABLED
extern          mbBOOL( *pxMBMasterFrameCBByteReceived ) ( void );

extern          mbBOOL( *pxMBMasterFrameCBTransmitterEmpty ) ( void );

extern          mbBOOL( *pxMBMasterPortCBTimerExpired ) ( void );
#endif
/* ----------------------- TCP port functions -------------------------------*/
#if MB_TCP_ENABLED

/********************** Target Specific START ************************/
#define MB_TCP_DISCONNECT_TIMEOUT       ( CONFIG_FMB_TCP_CONNECTION_TOUT_SEC * 1000000 ) // disconnect timeout in uS
#define MB_TCP_RESP_TIMEOUT_MS          ( MB_MASTER_TIMEOUT_MS_RESPOND - 1 ) // slave response time limit
#define MB_TCP_NET_LISTEN_BACKLOG       ( SOMAXCONN )

static const char *MBTCPTAG = "MB_TCP_SLAVE_PORT";
static int xListenSock = -1;
static SemaphoreHandle_t xShutdownSema = NULL;
static MbSlavePortConfig_t xConfig = { 0 };

// The helper function to get time stamp in microseconds
static int64_t xMBTCPGetTimeStamp(void)
{
    int64_t xTimeStamp = esp_timer_get_time();
    return xTimeStamp;
}

static void vxMBTCPPortMStoTimeVal(mbUSHORT usTimeoutMs, struct timeval *pxTimeout)
{
    pxTimeout->tv_sec = usTimeoutMs / 1000;
    pxTimeout->tv_usec = (usTimeoutMs - (pxTimeout->tv_sec * 1000)) * 1000;
}

static QueueHandle_t xMBTCPPortRespQueueCreate(void)
{
    QueueHandle_t xRespQueueHandle = xQueueCreate(2, sizeof(void*));
    MB_PORT_CHECK((xRespQueueHandle != NULL), NULL, "TCP respond queue creation failure.");
    return xRespQueueHandle;
}

static void vMBTCPPortRespQueueDelete(QueueHandle_t xRespQueueHandle)
{
    vQueueDelete(xRespQueueHandle);
}

static void* vxMBTCPPortRespQueueRecv(QueueHandle_t xRespQueueHandle)
{
    void* pvResp = NULL;
    MB_PORT_CHECK(xRespQueueHandle != NULL, NULL, "Response queue is not initialized.");
    BaseType_t xStatus = xQueueReceive(xRespQueueHandle,
                                        (void*)&pvResp,
                                        pdMS_TO_TICKS(MB_TCP_RESP_TIMEOUT_MS));
    if (xStatus != pdTRUE) {
        ESP_LOGD(MBTCPTAG, "Could not get respond confirmation.");
    }
    return pvResp;
}

static mbBOOL vxMBTCPPortRespQueueSend(QueueHandle_t xRespQueueHandle, void* pvResp)
{
    MB_PORT_CHECK(xRespQueueHandle != NULL, FALSE, "Response queue is not initialized.");
    BaseType_t xStatus = xQueueSend(xConfig.xRespQueueHandle,
                                    (const void*)&pvResp,
                                    pdMS_TO_TICKS(MB_TCP_RESP_TIMEOUT_MS));
    MB_PORT_CHECK((xStatus == pdTRUE), FALSE, "FAIL to send to response queue.");
    return TRUE;
}

static void vMBTCPPortServerTask(void *pvParameters);

void vMBTCPPortSlaveSetNetOpt(void* pvNetIf, eMBPortIpVer xIpVersion, eMBPortProto xProto, mbCHAR* pcBindAddrStr)
{
    // Set network options
    xConfig.pvNetIface = pvNetIf;
    xConfig.eMbProto = xProto;
    xConfig.xIpVer = xIpVersion;
    xConfig.pcBindAddr = pcBindAddrStr;
}

static int xMBTCPPortAcceptConnection(int xListenSockId, char** pcIPAddr)
{
    MB_PORT_CHECK(pcIPAddr, -1, "Wrong IP address pointer.");
    MB_PORT_CHECK((xListenSockId > 0), -1, "Incorrect listen socket ID.");

    // Address structure large enough for both IPv4 or IPv6 address
    struct sockaddr_storage xSrcAddr;
    mbCHAR cAddrStr[128];
    int xSockId = -1;
    mbCHAR* pcStr = NULL;
    socklen_t xSize = sizeof(struct sockaddr_storage);

    // Accept new socket connection if not active
    xSockId = accept(xListenSockId, (struct sockaddr *)&xSrcAddr, &xSize);
    if (xSockId < 0) {
        ESP_LOGE(MBTCPTAG, "Unable to accept connection: errno=%u", (unsigned)errno);
        close(xSockId);
    } else {
        // Get the sender's ip address as string
        if (xSrcAddr.ss_family == PF_INET) {
            inet_ntoa_r(((struct sockaddr_in *)&xSrcAddr)->sin_addr.s_addr, cAddrStr, sizeof(cAddrStr) - 1);
        }
#if CONFIG_LWIP_IPV6
        else if (xSrcAddr.ss_family == PF_INET6) {
            inet6_ntoa_r(((struct sockaddr_in6 *)&xSrcAddr)->sin6_addr, cAddrStr, sizeof(cAddrStr) - 1);
        }
#endif
        else {
            // Make sure ss_family is valid
            abort();
        }
        ESP_LOGI(MBTCPTAG, "Socket (#%d), accept client connection from address: %s", (int)xSockId, cAddrStr);
        pcStr = calloc(1, strlen(cAddrStr) + 1);
        if (pcStr && pcIPAddr) {
            memcpy(pcStr, cAddrStr, strlen(cAddrStr));
            pcStr[strlen(cAddrStr)] = '\0';
            *pcIPAddr = pcStr; // Set IP address of connected client
        }
    }
    return xSockId;
}

static mbBOOL xMBTCPPortCloseConnection(MbClientInfo_t* pxInfo)
{
    MB_PORT_CHECK(pxInfo, FALSE, "Client info is NULL.");

    if (pxInfo->xSockId == -1) {
        ESP_LOGE(MBTCPTAG, "Wrong socket info or disconnected socket: %d.", (int)pxInfo->xSockId);
        return FALSE;
    }
    
    // Empty tcp buffer before shutdown
    (void)recv(pxInfo->xSockId, &pxInfo->pucTCPBuf[0], MB_PDU_SIZE_MAX, MSG_DONTWAIT);

    if (shutdown(pxInfo->xSockId, SHUT_RDWR) == -1)
    {
        ESP_LOGE(MBTCPTAG, "Socket (#%d), shutdown failed: errno %u", (int)pxInfo->xSockId, (unsigned)errno);
    }
    close(pxInfo->xSockId);
    pxInfo->xSockId = -1;
    if (xConfig.usClientCount) {
        xConfig.usClientCount--; // decrement counter of client connections
    } else {
        xConfig.pxCurClientInfo = NULL;
    }
    return TRUE;
}

static void vMBTCPPortFreeClientInfo(MbClientInfo_t *pxClientInfo)
{
    if (pxClientInfo) {
        if (pxClientInfo->pucTCPBuf) {
            free((void *)pxClientInfo->pucTCPBuf);
        }
        if (pxClientInfo->pcIpAddr) {
            free((void *)pxClientInfo->pcIpAddr);
        }
        free((void *)pxClientInfo);
    }
}

static void vMBTCPPortShutdown(void)
{
    xSemaphoreGive(xShutdownSema);
    vTaskDelete(NULL);
    xConfig.xMbTcpTaskHandle = NULL;

    for (int i = 0; i < MB_TCP_PORT_MAX_CONN; i++) {
        MbClientInfo_t *pxClientInfo = xConfig.pxMbClientInfo[i];
        if ((pxClientInfo != NULL) && (pxClientInfo->xSockId > 0)) {
            xMBTCPPortCloseConnection(pxClientInfo);
            vMBTCPPortFreeClientInfo(pxClientInfo);
            xConfig.pxMbClientInfo[i] = NULL;
        }
    }
    free(xConfig.pxMbClientInfo);
}

static int xMBTCPPortRxPoll(MbClientInfo_t *pxClientInfo, mbULONG xTimeoutMs)
{
    int xRet = ERR_CLSD;
    struct timeval xTimeVal;
    fd_set xReadSet;
    int64_t xStartTimeStamp = 0;

    // Receive data from connected client
    if (pxClientInfo && pxClientInfo->xSockId > -1) {
        // Set receive timeout
        vxMBTCPPortMStoTimeVal(xTimeoutMs, &xTimeVal);
        xStartTimeStamp = xMBTCPGetTimeStamp();
        while (1)
        {
            FD_ZERO(&xReadSet);
            FD_SET(pxClientInfo->xSockId, &xReadSet);
            xRet = select(pxClientInfo->xSockId + 1, &xReadSet, NULL, NULL, &xTimeVal);
            if (xRet == -1)
            {
                // If select an error occurred
                xRet = ERR_CLSD;
                TCP_PORT_CHECK_SHDN(xShutdownSema, vMBTCPPortShutdown);
                break;
            } else if (xRet == 0) {
                // timeout occurred
                if ((xStartTimeStamp + xTimeoutMs * 1000) > xMBTCPGetTimeStamp()) {
                    ESP_LOGD(MBTCPTAG, "Socket (#%d) Read timeout.", (int)pxClientInfo->xSockId);
                    xRet = ERR_TIMEOUT;
                    TCP_PORT_CHECK_SHDN(xShutdownSema, vMBTCPPortShutdown);
                    break;
                }
            }
            if (FD_ISSET(pxClientInfo->xSockId, &xReadSet)) {
                // If new buffer received then read Modbus packet into buffer
                MB_PORT_CHECK((pxClientInfo->usTCPBufPos + pxClientInfo->usTCPFrameBytesLeft < MB_TCP_BUF_SIZE),
                                    ERR_BUF, "Socket (#%d), incorrect request buffer size = %u, ignore.",
                                    (int)pxClientInfo->xSockId,
                                    (unsigned)(pxClientInfo->usTCPBufPos + pxClientInfo->usTCPFrameBytesLeft));
                int xLength = recv(pxClientInfo->xSockId, &pxClientInfo->pucTCPBuf[pxClientInfo->usTCPBufPos],
                                      pxClientInfo->usTCPFrameBytesLeft, MSG_DONTWAIT);
                if (xLength < 0) {
                    // If an error occurred during receiving
                    ESP_LOGE(MBTCPTAG, "Receive failed: length=%u, errno=%u", (unsigned)xLength, (unsigned)errno);
                    xRet = (err_t)xLength;
                    break;
                } else if (xLength == 0) {
                    // Socket connection closed
                    ESP_LOGD(MBTCPTAG, "Socket (#%d)(%s), connection closed.",
                                                        (int)pxClientInfo->xSockId, pxClientInfo->pcIpAddr);
                    xRet = ERR_CLSD;
                    break;
                } else {
                    // New data received
                    pxClientInfo->usTCPBufPos += xLength;
                    pxClientInfo->usTCPFrameBytesLeft -= xLength;
                    if (pxClientInfo->usTCPBufPos >= MB_TCP_FUNC) {
                        // Length is a byte count of Modbus PDU (function code + data) and the
                        // unit identifier.
                        xLength = (int)MB_TCP_GET_FIELD(pxClientInfo->pucTCPBuf, MB_TCP_LEN);
                        // Is the frame already complete.
                        if (pxClientInfo->usTCPBufPos < (MB_TCP_UID + xLength)) {
                            // The incomplete frame is received
                            pxClientInfo->usTCPFrameBytesLeft = xLength + MB_TCP_UID - pxClientInfo->usTCPBufPos;
                        } else if (pxClientInfo->usTCPBufPos == (MB_TCP_UID + xLength)) {
#if MB_TCP_DEBUG
                            prvvMBTCPLogFrame(MBTCPTAG, (mbUCHAR*)&pxClientInfo->pucTCPBuf[0], pxClientInfo->usTCPBufPos);
#endif
                            // Copy TID field from incoming packet
                            pxClientInfo->usTidCnt = MB_TCP_GET_FIELD(pxClientInfo->pucTCPBuf, MB_TCP_TID);
                            xRet = pxClientInfo->usTCPBufPos;
                            break;
                        } else if ((pxClientInfo->usTCPBufPos + xLength) >= MB_TCP_BUF_SIZE) {
                            ESP_LOGE(MBTCPTAG, "Incorrect buffer received (%u) bytes.", (unsigned)xLength);
                            // This should not happen. We can't deal with such a client and
                            // drop the connection for security reasons.
                            xRet = ERR_BUF;
                            break;
                        }
                    } // if ( pxClientInfo->usTCPBufPos >= MB_TCP_FUNC )
                } // if data received
            } // if (FD_ISSET(pxClientInfo->xSockId, &xReadSet))
        } // while (1)
    }
    return (xRet);
}

// Create a listening socket on pcBindIp: Port
static int
vMBTCPPortBindAddr(const mbCHAR* pcBindIp)
{
    int xPar, xRet;
    int xListenSockFd = -1;
    struct addrinfo xHint;
    struct addrinfo* pxAddrList;
    struct addrinfo* pxCurAddr;
    mbCHAR* pcStr = NULL;

    memset( &xHint, 0, sizeof( xHint ) );

    // Bind to IPv6 and/or IPv4, but only in the desired protocol
    // Todo: Find a reason why AF_UNSPEC does not work for IPv6
    xHint.ai_family = (xConfig.xIpVer == MB_PORT_IPV4) ? AF_INET : AF_INET6;
    xHint.ai_socktype = (xConfig.eMbProto == MB_PROTO_UDP) ? SOCK_DGRAM : SOCK_STREAM;
    // The LWIP has an issue when connection to IPv6 socket
    xHint.ai_protocol = (xConfig.eMbProto == MB_PROTO_UDP) ? IPPROTO_UDP : IPPROTO_TCP;
    xHint.ai_flags = AI_NUMERICSERV;

    if (pcBindIp == NULL) {
        xHint.ai_flags |= AI_PASSIVE;
    } else {
        xHint.ai_flags |= AI_CANONNAME;
    }

    if (asprintf(&pcStr, "%u", xConfig.usPort) == -1) {
        abort();
    }

    xRet = getaddrinfo(pcBindIp, pcStr, &xHint, &pxAddrList);
    free(pcStr);

    if (xRet != 0) {
        return -1;
    }

    // Try the sockaddr until a binding succeeds
    for (pxCurAddr = pxAddrList; pxCurAddr != NULL; pxCurAddr = pxCurAddr->ai_next)
    {
        xListenSockFd = (int)socket(pxCurAddr->ai_family, pxCurAddr->ai_socktype,
                                        pxCurAddr->ai_protocol);
        if (xListenSockFd < 0)
        {
            continue;
        }

        xPar = 1;
        // Allow multi client connections
        if (setsockopt(xListenSockFd, SOL_SOCKET, SO_REUSEADDR,
                        (const char*)&xPar, sizeof(xPar)) != 0)
        {
            close(xListenSockFd);
            xListenSockFd = -1;
            continue;
        }

        if (bind(xListenSockFd, (struct sockaddr *)pxCurAddr->ai_addr,
                                        (socklen_t)pxCurAddr->ai_addrlen) != 0 )
        {
            close(xListenSockFd);
            xListenSockFd = -1;
            continue;
        }

        // Listen only makes sense for TCP
        if (xConfig.eMbProto == MB_PROTO_TCP)
        {
            if (listen(xListenSockFd, MB_TCP_NET_LISTEN_BACKLOG) != 0)
            {
                ESP_LOGE(MBTCPTAG, "Error occurred during listen: errno=%u", (unsigned)errno);
                close(xListenSockFd);
                xListenSockFd = -1;
                continue;
            }
        }
        // Bind was successful
        pcStr = (pxCurAddr->ai_canonname == NULL) ? (mbCHAR*)"\0" : pxCurAddr->ai_canonname;
        ESP_LOGI(MBTCPTAG, "Socket (#%d), listener %s on port: %u, errno=%u",
                                            (int)xListenSockFd, pcStr, (unsigned)xConfig.usPort, (unsigned)errno);
        break;
    }

    freeaddrinfo(pxAddrList);
    return(xListenSockFd);
}

static void vMBTCPPortServerTask(void *pvParameters)
{
    int xErr = 0;
    fd_set xReadSet;
    int i;
    mbCHAR* pcClientIp = NULL;
    struct timeval xTimeVal;

    // Main connection cycle
    while (1) {
        // Create listen socket
        xListenSock = vMBTCPPortBindAddr(xConfig.pcBindAddr);
        if (xListenSock < 0) {
            TCP_PORT_CHECK_SHDN(xShutdownSema, vMBTCPPortShutdown);
            continue;
        }

        // Connections handling cycle
        while (1) {
            // clear the socket set
            FD_ZERO(&xReadSet);
            // add master socket to set
            FD_SET(xListenSock, &xReadSet);
            int xMaxSd = xListenSock;
            xConfig.usClientCount = 0;

            vxMBTCPPortMStoTimeVal(1, &xTimeVal);
            // Initialize read set and file descriptor according to
            // all registered connected clients
            for (i = 0; i < MB_TCP_PORT_MAX_CONN; i++) {
                TCP_PORT_CHECK_SHDN(xShutdownSema, vMBTCPPortShutdown);
                if ((xConfig.pxMbClientInfo[i] != NULL) && (xConfig.pxMbClientInfo[i]->xSockId > 0)) {
                    // calculate max file descriptor for select
                    xMaxSd = (xConfig.pxMbClientInfo[i]->xSockId > xMaxSd) ?
                            xConfig.pxMbClientInfo[i]->xSockId : xMaxSd;
                    FD_SET(xConfig.pxMbClientInfo[i]->xSockId, &xReadSet);
                    xConfig.usClientCount++;
                }
            }

            vxMBTCPPortMStoTimeVal(MB_TCP_RESP_TIMEOUT_MS, &xTimeVal);

            // Wait for an activity on one of the sockets during timeout
            xErr = select(xMaxSd + 1, &xReadSet, NULL, NULL, &xTimeVal);
            if ((xErr < 0) && (errno != EINTR)) {
                // error occurred during wait for read
                ESP_LOGE(MBTCPTAG, "select() errno = %u.", (unsigned)errno);
                TCP_PORT_CHECK_SHDN(xShutdownSema, vMBTCPPortShutdown);
                continue;
            } else if (xErr == 0) {
                // If timeout happened, something is wrong
                ESP_LOGD(MBTCPTAG, "select() timeout, errno = %u.", (unsigned)errno);
                TCP_PORT_CHECK_SHDN(xShutdownSema, vMBTCPPortShutdown);
            }

            // If something happened on the master socket, then its an incoming connection.
            if (FD_ISSET(xListenSock, &xReadSet) && xConfig.usClientCount < MB_TCP_PORT_MAX_CONN) {
                MbClientInfo_t* pxClientInfo = NULL;
                // find first empty place to insert connection info
                for (i = 0; i < MB_TCP_PORT_MAX_CONN; i++) {
                    pxClientInfo = xConfig.pxMbClientInfo[i];
                    if (pxClientInfo == NULL) {
                        break;
                    }
                }
                // if request for new connection but no space left
                if (pxClientInfo != NULL) {
                    if (xConfig.pxMbClientInfo[MB_TCP_PORT_MAX_CONN] == NULL) {
                        ESP_LOGE(MBTCPTAG, "Fail to accept connection %u, only %u connections supported.", (unsigned)(i + 1), (unsigned)MB_TCP_PORT_MAX_CONN);
                    }
                    xConfig.pxMbClientInfo[MB_TCP_PORT_MAX_CONN] = pxClientInfo; // set last connection info
                } else {
                    // allocate memory for new client info
                    pxClientInfo = calloc(1, sizeof(MbClientInfo_t));
                    if (!pxClientInfo) {
                        ESP_LOGE(MBTCPTAG, "Client info allocation fail.");
                        vMBTCPPortFreeClientInfo(pxClientInfo);
                        pxClientInfo = NULL;
                    } else {
                        // Accept new client connection
                        pxClientInfo->xSockId = xMBTCPPortAcceptConnection(xListenSock, &pcClientIp);
                        if (pxClientInfo->xSockId < 0) {
                            ESP_LOGE(MBTCPTAG, "Fail to accept connection for client %u.", (unsigned)(xConfig.usClientCount - 1));
                            // Accept connection fail, then free client info and continue polling.
                            vMBTCPPortFreeClientInfo(pxClientInfo);
                            pxClientInfo = NULL;
                            continue;
                        }
                        pxClientInfo->pucTCPBuf = calloc(MB_TCP_BUF_SIZE, sizeof(mbUCHAR));
                        if (!pxClientInfo->pucTCPBuf) {
                            ESP_LOGE(MBTCPTAG, "Fail to allocate buffer for client %u.", (unsigned)(xConfig.usClientCount - 1));
                            vMBTCPPortFreeClientInfo(pxClientInfo);
                            pxClientInfo = NULL;
                            continue;
                        }
                        // Fill the connection info structure
                        xConfig.pxMbClientInfo[i] = pxClientInfo;
                        pxClientInfo->xIndex = i;
                        xConfig.usClientCount++;
                        pxClientInfo->pcIpAddr = pcClientIp;
                        pxClientInfo->xRecvTimeStamp = xMBTCPGetTimeStamp();
                        xConfig.pxMbClientInfo[MB_TCP_PORT_MAX_CONN] = NULL;
                        pxClientInfo->usTCPFrameBytesLeft = MB_TCP_FUNC;
                        pxClientInfo->usTCPBufPos = 0;
                    }
                }
            }
            // Handle data request from client
            if (xErr > 0) {
                // Handling client connection requests
                for (i = 0; i < MB_TCP_PORT_MAX_CONN; i++) {
                    MbClientInfo_t* pxClientInfo = xConfig.pxMbClientInfo[i];
                    if ((pxClientInfo != NULL) && (pxClientInfo->xSockId > 0)) {
                        if (FD_ISSET(pxClientInfo->xSockId, &xReadSet)) {
                            // Other sockets are ready to be read
                            xErr = xMBTCPPortRxPoll(pxClientInfo, MB_TCP_READ_TIMEOUT_MS);
                            // If an invalid data received from socket or connection fail
                            // or if timeout then drop connection and restart
                            if (xErr < 0) {
                                uint64_t xTimeStamp = xMBTCPGetTimeStamp();
                                // If data update is timed out
                                switch(xErr)
                                {
                                    case ERR_TIMEOUT:
                                        ESP_LOGE(MBTCPTAG, "Socket (#%d)(%s), data receive timeout, time[us]: %" PRIu64 ", close active connection.",
                                                                            (int)pxClientInfo->xSockId, pxClientInfo->pcIpAddr,
                                                                            (uint64_t)(xTimeStamp - pxClientInfo->xRecvTimeStamp));
                                        break;
                                    case ERR_CLSD:
                                        ESP_LOGE(MBTCPTAG, "Socket (#%d)(%s), connection closed by peer.",
                                                                            (int)pxClientInfo->xSockId, pxClientInfo->pcIpAddr);
                                        break;
                                    case ERR_BUF:
                                    default:
                                        ESP_LOGE(MBTCPTAG, "Socket (#%d)(%s), read data error: 0x%x",
                                                                            (int)pxClientInfo->xSockId, pxClientInfo->pcIpAddr, (int)xErr);
                                        break;
                                }

                                TCP_PORT_CHECK_SHDN(xShutdownSema, vMBTCPPortShutdown);

                                // Close client connection
                                xMBTCPPortCloseConnection(pxClientInfo);

                                // This client does not respond, then unregister it
                                vMBTCPPortFreeClientInfo(pxClientInfo);
                                xConfig.pxMbClientInfo[i] = NULL;
                                xConfig.pxMbClientInfo[MB_TCP_PORT_MAX_CONN] = NULL;
                                // If no any active connections, break
                                if (!xConfig.usClientCount) {
                                    xConfig.pxCurClientInfo = NULL;
                                    break;
                                }
                            } else {
                                pxClientInfo->xRecvTimeStamp = xMBTCPGetTimeStamp();

                                // set current client info to active client from which we received request
                                xConfig.pxCurClientInfo = pxClientInfo;

                                // Complete frame received, inform state machine to process frame
                                xMBPortEventPost(EV_FRAME_RECEIVED);

                                ESP_LOGD(MBTCPTAG, "Socket (#%d)(%s), get packet TID=0x%X, %d bytes.",
                                                                    (int)pxClientInfo->xSockId, pxClientInfo->pcIpAddr,
                                                                    (int)pxClientInfo->usTidCnt, (int)xErr);

                                // Wait while response is not processed by stack by timeout
                                mbUCHAR* pucSentBuffer = vxMBTCPPortRespQueueRecv(xConfig.xRespQueueHandle);
                                if (pucSentBuffer == NULL) {
                                    ESP_LOGD(MBTCPTAG, "Response is ignored, time exceeds configured %d [ms].",
                                                                        (unsigned)MB_TCP_RESP_TIMEOUT_MS);
                                } else  {
                                    mbUSHORT usSentTid = MB_TCP_GET_FIELD(pucSentBuffer, MB_TCP_TID);
                                    if (usSentTid != pxClientInfo->usTidCnt) {
                                        ESP_LOGE(MBTCPTAG, "Sent TID(%x) != Recv TID(%x), ignore packet.",
                                                                            (int)usSentTid, (int)pxClientInfo->usTidCnt);
                                    }
                                }

                                // Get time stamp of last data update
                                pxClientInfo->xSendTimeStamp = xMBTCPGetTimeStamp();
                                ESP_LOGD(MBTCPTAG, "Client %d, Socket(#%d), processing time = %" PRIu64 "(us).",
                                                            (int)pxClientInfo->xIndex, (int)pxClientInfo->xSockId,
                                                            (uint64_t)(pxClientInfo->xSendTimeStamp - pxClientInfo->xRecvTimeStamp));
                            }
                        } else {
                            if (pxClientInfo) {
                                // client is not ready to be read
                                int64_t xTime = xMBTCPGetTimeStamp() - pxClientInfo->xRecvTimeStamp;
                                if (xTime > MB_TCP_DISCONNECT_TIMEOUT) {
                                    ESP_LOGE(MBTCPTAG, "Client %d, Socket(#%d) do not answer for %" PRIu64 " (us). Drop connection...",
                                                                    (int)pxClientInfo->xIndex, (int)pxClientInfo->xSockId, (uint64_t)xTime);
                                    xMBTCPPortCloseConnection(pxClientInfo);

                                    // This client does not respond, then delete registered data
                                    vMBTCPPortFreeClientInfo(pxClientInfo);
                                    xConfig.pxMbClientInfo[i] = NULL;
                                }
                            } else {
                                ESP_LOGE(MBTCPTAG, "Client %d is disconnected.", (int)i);
                            }
                        }
                    } // if ((pxClientInfo != NULL)
                } // Handling client connection requests
            }
        } // while(1) // Handle connection cycle
    } // Main connection cycle
    vTaskDelete(NULL);
}


/********************** Target Specific ENDED ************************/

mbBOOL
xMBTCPPortInit( mbUSHORT usTCPPort )
{
    mbBOOL bOkay = FALSE;

    xConfig.pxMbClientInfo = calloc(MB_TCP_PORT_MAX_CONN + 1, sizeof(MbClientInfo_t*));
    if (!xConfig.pxMbClientInfo) {
        ESP_LOGE(MBTCPTAG, "TCP client info allocation failure.");
        return FALSE;
    }
    for(int idx = 0; idx < MB_TCP_PORT_MAX_CONN; xConfig.pxMbClientInfo[idx] = NULL, idx++);

    xConfig.xRespQueueHandle = xMBTCPPortRespQueueCreate();
    if (!xConfig.xRespQueueHandle) {
        ESP_LOGE(MBTCPTAG, "Response queue allocation failure.");
        return FALSE;
    }

    xConfig.usPort = usTCPPort;
    xConfig.eMbProto = MB_PROTO_TCP;
    xConfig.usClientCount = 0;
    xConfig.pvNetIface = NULL;
    xConfig.xIpVer = MB_PORT_IPV4;
    xConfig.pcBindAddr = NULL;

    // Create task for packet processing
    BaseType_t xErr = xTaskCreatePinnedToCore(vMBTCPPortServerTask,
                                    "tcp_slave_task",
                                    MB_TCP_STACK_SIZE,
                                    NULL,
                                    MB_TCP_TASK_PRIO,
                                    &xConfig.xMbTcpTaskHandle,
                                    MB_PORT_TASK_AFFINITY);
    if (xErr != pdTRUE)
    {
        ESP_LOGE(MBTCPTAG, "Server task creation failure.");
        vTaskDelete(xConfig.xMbTcpTaskHandle);
    } else {
        ESP_LOGI(MBTCPTAG, "Protocol stack initialized.");
        bOkay = TRUE;
    }
    return bOkay;
}

void
vMBTCPPortClose( void )
{
    // Try to exit the task gracefully, so select could release its internal callbacks
    // that were allocated on the stack of the task we're going to delete
    xShutdownSema = xSemaphoreCreateBinary();
    if (xShutdownSema == NULL || // if no semaphore (alloc issues) or couldn't acquire it, just delete the task
        xSemaphoreTake(xShutdownSema, 2 * pdMS_TO_TICKS(CONFIG_FMB_MASTER_TIMEOUT_MS_RESPOND)) != pdTRUE) {
        ESP_LOGE(MBTCPTAG, "Task couldn't exit gracefully within timeout -> abruptly deleting the task");
        vTaskDelete(xConfig.xMbTcpTaskHandle);
    }

    close(xListenSock);
    xListenSock = -1;

    vMBTCPPortRespQueueDelete(xConfig.xRespQueueHandle);

    if (xShutdownSema) {
        vSemaphoreDelete(xShutdownSema);
        xShutdownSema = NULL;
    }
    vMBPortEventClose();
}

void
vMBTCPPortEnable( void )
{
    ;
}

void
vMBTCPPortDisable( void )
{
    ;
}

mbBOOL
xMBTCPPortGetRequest( mbUCHAR **ppucMBTCPFrame, mbUSHORT * usTCPLength )
{
    mbBOOL xRet = FALSE;
    if (xConfig.pxCurClientInfo) {
        *ppucMBTCPFrame = &xConfig.pxCurClientInfo->pucTCPBuf[0];
        *usTCPLength = xConfig.pxCurClientInfo->usTCPBufPos;

        // Reset the buffer.
        xConfig.pxCurClientInfo->usTCPBufPos = 0;
        xConfig.pxCurClientInfo->usTCPFrameBytesLeft = MB_TCP_FUNC;
        xRet = TRUE;
    }
    return xRet;
}

mbBOOL
xMBTCPPortSendResponse( mbUCHAR *pucMBTCPFrame, mbUSHORT usTCPLength )
{
    mbBOOL bFrameSent = FALSE;
    fd_set xWriteSet;
    fd_set xErrorSet;
    int xErr = -1;
    struct timeval xTimeVal;

    if (xConfig.pxCurClientInfo) {
        FD_ZERO(&xWriteSet);
        FD_ZERO(&xErrorSet);
        FD_SET(xConfig.pxCurClientInfo->xSockId, &xWriteSet);
        FD_SET(xConfig.pxCurClientInfo->xSockId, &xErrorSet);
        vxMBTCPPortMStoTimeVal(MB_TCP_SEND_TIMEOUT_MS, &xTimeVal);
        // Check if socket writable
        xErr = select(xConfig.pxCurClientInfo->xSockId + 1, NULL, &xWriteSet, &xErrorSet, &xTimeVal);
        if ((xErr == -1) || FD_ISSET(xConfig.pxCurClientInfo->xSockId, &xErrorSet)) {
            ESP_LOGE(MBTCPTAG, "Socket(#%d) , send select() error = %u.",
                            (int)xConfig.pxCurClientInfo->xSockId, (unsigned)errno);
            return FALSE;
        }

        // Apply TID field from request to the frame before send response
        pucMBTCPFrame[MB_TCP_TID] = (mbUCHAR)(xConfig.pxCurClientInfo->usTidCnt >> 8U);
        pucMBTCPFrame[MB_TCP_TID + 1] = (mbUCHAR)(xConfig.pxCurClientInfo->usTidCnt & 0xFF);

        // Write message into socket and disable Nagle's algorithm
        xErr = send(xConfig.pxCurClientInfo->xSockId, pucMBTCPFrame, usTCPLength, TCP_NODELAY);
        if (xErr < 0) {
            ESP_LOGE(MBTCPTAG, "Socket(#%d), fail to send data, errno = %u",
                        (int)xConfig.pxCurClientInfo->xSockId, (unsigned)errno);
            xConfig.pxCurClientInfo->xError = xErr;
        } else {
            bFrameSent = TRUE;
            vxMBTCPPortRespQueueSend(xConfig.xRespQueueHandle, (void*)pucMBTCPFrame);
        }
    } else {
        ESP_LOGD(MBTCPTAG, "Port is not active. Release lock.");
        vxMBTCPPortRespQueueSend(xConfig.xRespQueueHandle, (void*)pucMBTCPFrame);
    }
    return bFrameSent;
}

#endif

#if MB_MASTER_TCP_ENABLED

/********************** Target Specific START ************************/
#define MB_TCP_CONNECTION_TIMEOUT_MS    ( 20 )      // Connection timeout in mS
#define MB_TCP_RECONNECT_TIMEOUT        ( 5000000 ) // Connection timeout in uS

#define MB_EVENT_REQ_DONE_MASK          (   EV_MASTER_PROCESS_SUCCESS | \
                                            EV_MASTER_ERROR_RESPOND_TIMEOUT | \
                                            EV_MASTER_ERROR_RECEIVE_DATA | \
                                            EV_MASTER_ERROR_EXECUTE_FUNCTION )

#define MB_EVENT_REQ_ERR_MASK           ( EV_MASTER_PROCESS_SUCCESS )

#define MB_EVENT_WAIT_TOUT_MS           ( 3000 )
#define MB_SHDN_WAIT_TOUT_MS            ( 5000 )

#define MB_TCP_READ_TICK_MS             ( 1 )
#define MB_TCP_READ_BUF_RETRY_CNT       ( 4 )
#define MB_SLAVE_FMT(fmt)               "Slave #%d, Socket(#%d)(%s)"fmt

void vMBPortEventClose(void);

static const char *MBTCPMTAG = "MB_TCP_MASTER_PORT";
static MbPortConfig_t xMbPortConfig;
static EventGroupHandle_t xMasterEventHandle = NULL;
static SemaphoreHandle_t xShutdownMSema = NULL;
static EventBits_t xMasterEvent = 0;

static void vMBTCPPortMasterTask(void *pvParameters);

// Waits for stack start event to start Modbus event processing
mbBOOL xMBTCPPortMasterWaitEvent(EventGroupHandle_t xEventHandle, EventBits_t xEvent, mbUSHORT usTimeout)
{
    xMasterEventHandle = xEventHandle;
    xMasterEvent = xEvent;
    BaseType_t status = xEventGroupWaitBits(xMasterEventHandle,
                                            (BaseType_t)(xEvent),
                                            pdFALSE, // do not clear start bit
                                            pdFALSE,
                                            usTimeout);
    return (mbBOOL)(status & xEvent);
}

static MbSlaveInfo_t *vMBTCPPortMasterFindSlaveInfo(mbUCHAR ucSlaveAddr)
{
    int xIndex;
    mbBOOL xFound = false;
    for (xIndex = 0; xIndex < xMbPortConfig.usMbSlaveInfoCount; xIndex++) {
        if (xMbPortConfig.pxMbSlaveInfo[xIndex]->ucSlaveAddr == ucSlaveAddr) {
            xMbPortConfig.pxMbSlaveCurrInfo = xMbPortConfig.pxMbSlaveInfo[xIndex];
            xFound = TRUE;
            xMbPortConfig.ucCurSlaveIndex = xIndex;
        }
    }
    if (!xFound) {
        xMbPortConfig.pxMbSlaveCurrInfo = NULL;
        ESP_LOGE(MBTCPMTAG, "Slave info for short address %u not found.", ucSlaveAddr);
    }
    return xMbPortConfig.pxMbSlaveCurrInfo;
}

static MbSlaveInfo_t *vMBTCPPortMasterGetCurrInfo(void)
{
    if (!xMbPortConfig.pxMbSlaveCurrInfo) {
        ESP_LOGE(MBTCPMTAG, "Incorrect current slave info.");
    }
    return xMbPortConfig.pxMbSlaveCurrInfo;
}

// Start Modbus event state machine
static void vMBTCPPortMasterStartPoll(void)
{
    if (xMasterEventHandle) {
        // Set the mbcontroller start flag
        EventBits_t xFlags = xEventGroupSetBits(xMasterEventHandle,
                                                (EventBits_t)xMasterEvent);
        if (!(xFlags & xMasterEvent)) {
            ESP_LOGE(MBTCPMTAG, "Fail to start TCP stack.");
        }
    } else {
        ESP_LOGE(MBTCPMTAG, "Fail to start polling. Incorrect event handle...");
    }
}

// Stop Modbus event state machine
static void vMBTCPPortMasterStopPoll(void)
{
    if (xMasterEventHandle) {
        // Set the mbcontroller start flag
        EventBits_t xFlags = xEventGroupClearBits(xMasterEventHandle,
                                                (EventBits_t)xMasterEvent);
        if (!(xFlags & xMasterEvent)) {
            ESP_LOGE(MBTCPMTAG, "Fail to stop polling.");
        }
    } else {
        ESP_LOGE(MBTCPMTAG, "Fail to stop polling. Incorrect event handle...");
    }
}

// The helper function to get time stamp in microseconds
static int64_t xMBTCPMasterGetTimeStamp(void)
{
    int64_t xTimeStamp = esp_timer_get_time();
    return xTimeStamp;
}

static void vMBTCPPortMasterMStoTimeVal(mbUSHORT usTimeoutMs, struct timeval *tv)
{
    tv->tv_sec = usTimeoutMs / 1000;
    tv->tv_usec = (usTimeoutMs - (tv->tv_sec * 1000)) * 1000;
}

static mbBOOL xMBTCPPortMasterCloseConnection(MbSlaveInfo_t *pxInfo)
{
    if (!pxInfo) {
        return FALSE;
    }
    if (pxInfo->xSockId == -1) {
        ESP_LOGE(MBTCPMTAG, "Wrong socket info or disconnected socket: %d, skip.", (int)pxInfo->xSockId);
        return FALSE;
    }
    if (shutdown(pxInfo->xSockId, SHUT_RDWR) == -1) {
        ESP_LOGV(MBTCPMTAG, "Shutdown failed sock %d, errno=%u", (int)pxInfo->xSockId, (unsigned)errno);
    }
    close(pxInfo->xSockId);
    pxInfo->xSockId = -1;
    return TRUE;
}

static void xMBTCPPortMasterShutdown(void)
{
    xSemaphoreGive(xShutdownMSema);

    for (mbUSHORT ucCnt = 0; ucCnt < MB_TCP_PORT_MAX_CONN; ucCnt++) {
        MbSlaveInfo_t* pxInfo = xMbPortConfig.pxMbSlaveInfo[ucCnt];
        if (pxInfo) {
            xMBTCPPortMasterCloseConnection(pxInfo);
            if (pxInfo->pucRcvBuf) {
                free(pxInfo->pucRcvBuf);
            }
            free(pxInfo);
            xMbPortConfig.pxMbSlaveInfo[ucCnt] = NULL;
        }
    }
    free(xMbPortConfig.pxMbSlaveInfo);
    vTaskDelete(NULL);
    xMbPortConfig.xMbTcpTaskHandle = NULL;
}

void vMBTCPPortMasterSetNetOpt(void *pvNetIf, eMBPortIpVer xIpVersion, eMBPortProto xProto)
{
    xMbPortConfig.pvNetIface = pvNetIf;
    xMbPortConfig.eMbProto = xProto;
    xMbPortConfig.eMbIpVer = xIpVersion;
}

// Function returns time left for response processing according to response timeout
static int64_t xMBTCPPortMasterGetRespTimeLeft(MbSlaveInfo_t *pxInfo)
{
    if (!pxInfo) {
        return 0;
    }
    int64_t xTimeStamp = xMBTCPMasterGetTimeStamp() - pxInfo->xSendTimeStamp;
    return (xTimeStamp > (1000 * MB_MASTER_TIMEOUT_MS_RESPOND)) ? 0 :
                    (MB_MASTER_TIMEOUT_MS_RESPOND - (xTimeStamp / 1000) - 1);
}

// Wait socket ready to read state
static int vMBTCPPortMasterRxCheck(int xSd, fd_set *pxFdSet, int xTimeMs)
{
    fd_set xReadSet = *pxFdSet;
    fd_set xErrorSet = *pxFdSet;
    int xRes = 0;
    struct timeval xTimeout;

    vMBTCPPortMasterMStoTimeVal(xTimeMs, &xTimeout);
    xRes = select(xSd + 1, &xReadSet, NULL, &xErrorSet, &xTimeout);
    if (xRes == 0) {
        // No respond from slave during timeout
        xRes = ERR_TIMEOUT;
    } else if ((xRes < 0) || FD_ISSET(xSd, &xErrorSet)) {
        xRes = -1;
    }

    *pxFdSet = xReadSet;
    return xRes;
}

static int xMBTCPPortMasterGetBuf(MbSlaveInfo_t *pxInfo, mbUCHAR *pucDstBuf, mbUSHORT usLength, uint16_t xTimeMs)
{
    int xLength = 0;
    mbUCHAR *pucBuf = pucDstBuf;
    mbUSHORT usBytesLeft = usLength;
    struct timeval xTime;

    MB_PORT_CHECK((pxInfo && pxInfo->xSockId > -1), -1, "Try to read incorrect socket = #%d.", (int)pxInfo->xSockId);

    // Set receive timeout for socket <= slave respond time
    xTime.tv_sec = xTimeMs / 1000;
    xTime.tv_usec = (xTimeMs % 1000) * 1000;
    setsockopt(pxInfo->xSockId, SOL_SOCKET, SO_RCVTIMEO, &xTime, sizeof(xTime));

    // Receive data from connected client
    while (usBytesLeft > 0) {
        TCP_PORT_CHECK_SHDN(xShutdownMSema, xMBTCPPortMasterShutdown);
        xLength = recv(pxInfo->xSockId, pucBuf, usBytesLeft, 0);
        if (xLength < 0) {
            if (errno == EAGAIN) {
                // Read timeout occurred, check the timeout and return
            } else if (errno == ENOTCONN) {
                // Socket connection closed
                ESP_LOGE(MBTCPMTAG, "Socket(#%d)(%s) connection closed.",
                         (int)pxInfo->xSockId, pxInfo->pcIpAddr);
                return ERR_CONN;
            } else {
                // Other error occurred during receiving
                ESP_LOGE(MBTCPMTAG, "Socket(#%d)(%s) receive error, length=%u, errno=%u",
                         (int)pxInfo->xSockId, pxInfo->pcIpAddr, (unsigned)xLength, (unsigned)errno);
                return -1;
            }
        } else if (xLength) {
            pucBuf += xLength;
            usBytesLeft -= xLength;
        }
        if (xMBTCPPortMasterGetRespTimeLeft(pxInfo) == 0) {
            return ERR_TIMEOUT;
        }
    }
    return usLength;
}

static int vMBTCPPortMasterReadPacket(MbSlaveInfo_t *pxInfo)
{
    int xLength = 0;
    int xRet = 0;
    mbUSHORT usTidRcv = 0;

    // Receive data from connected client
    if (pxInfo) {
        MB_PORT_CHECK((pxInfo->xSockId > 0), -1, "Try to read incorrect socket = #%d.", pxInfo->xSockId);
        // Read packet header
        xRet = xMBTCPPortMasterGetBuf(pxInfo, &pxInfo->pucRcvBuf[0],
                                        MB_TCP_UID, xMBTCPPortMasterGetRespTimeLeft(pxInfo));
        if (xRet < 0) {
            pxInfo->xRcvErr = xRet;
            return xRet;
        } else if (xRet != MB_TCP_UID) {
            ESP_LOGD(MBTCPMTAG, "Socket (#%d)(%s), Fail to read modbus header. ret=%d",
                     (int)pxInfo->xSockId, pxInfo->pcIpAddr, (int)xRet);
            pxInfo->xRcvErr = ERR_VAL;
            return ERR_VAL;
        }
        // If we have received the MBAP header we can analyze it and calculate
        // the number of bytes left to complete the current request.
        xLength = (int)MB_TCP_GET_FIELD(pxInfo->pucRcvBuf, MB_TCP_LEN);
        xRet = xMBTCPPortMasterGetBuf(pxInfo, &pxInfo->pucRcvBuf[MB_TCP_UID],
                                        xLength, xMBTCPPortMasterGetRespTimeLeft(pxInfo));
        if (xRet < 0) {
            pxInfo->xRcvErr = xRet;
            return xRet;
        } else if (xRet != xLength) {
            // Received incorrect or fragmented packet.
            ESP_LOGD(MBTCPMTAG, "Socket(#%d)(%s) incorrect packet, length=%u, TID=0x%02x, errno=%u(%s)",
                     (int)pxInfo->xSockId, pxInfo->pcIpAddr, (int)pxInfo->usRcvPos,
                     (int)usTidRcv, (unsigned)errno, strerror(errno));
            pxInfo->xRcvErr = ERR_VAL;
            return ERR_VAL;
        }
        usTidRcv = MB_TCP_GET_FIELD(pxInfo->pucRcvBuf, MB_TCP_TID);

        // Check transaction identifier field in the incoming packet.
        if ((pxInfo->usTidCnt - 1) != usTidRcv) {
            ESP_LOGD(MBTCPMTAG, "Socket (#%d)(%s), incorrect TID(0x%02x)!=(0x%02x) received, discard data.",
                     (int)pxInfo->xSockId, pxInfo->pcIpAddr, (int)usTidRcv, (int)(pxInfo->usTidCnt - 1));
            pxInfo->xRcvErr = ERR_BUF;
            return ERR_BUF;
        }
        pxInfo->usRcvPos += xRet + MB_TCP_UID;
        ESP_LOGD(MBTCPMTAG, "Socket(#%d)(%s) get data, length=%u, TID=0x%02x, errno=%u(%s)",
                 (int)pxInfo->xSockId, pxInfo->pcIpAddr, (unsigned)pxInfo->usRcvPos,
                 (unsigned)usTidRcv, (unsigned)errno, strerror(errno));
        pxInfo->xRcvErr = ERR_OK;
        return pxInfo->usRcvPos;
    }
    return -1;
}

static err_t xMBTCPPortMasterSetNonBlocking(MbSlaveInfo_t *pxInfo)
{
    if (!pxInfo) {
        return ERR_CONN;
    }
    // Set non blocking attribute for socket
    mbULONG ulFlags = fcntl(pxInfo->xSockId, F_GETFL);
    if (fcntl(pxInfo->xSockId, F_SETFL, ulFlags | O_NONBLOCK) == -1) {
        ESP_LOGE(MBTCPMTAG, "Socket(#%d)(%s), fcntl() call error=%u",
                 (int)pxInfo->xSockId, pxInfo->pcIpAddr, (unsigned)errno);
        return ERR_WOULDBLOCK;
    }
    return ERR_OK;
}

static void vMBTCPPortSetKeepAlive(MbSlaveInfo_t *pxInfo)
{
    int optval = 1;
    setsockopt(pxInfo->xSockId, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval));
}

// Check connection for timeout helper
static err_t xMBTCPPortMasterCheckAlive(MbSlaveInfo_t *pxInfo, mbULONG xTimeoutMs)
{
    fd_set xWriteSet;
    fd_set xErrorSet;
    err_t xErr = -1;
    struct timeval xTimeVal;

    if (pxInfo && pxInfo->xSockId != -1) {
        FD_ZERO(&xWriteSet);
        FD_ZERO(&xErrorSet);
        FD_SET(pxInfo->xSockId, &xWriteSet);
        FD_SET(pxInfo->xSockId, &xErrorSet);
        vMBTCPPortMasterMStoTimeVal(xTimeoutMs, &xTimeVal);
        // Check if the socket is writable
        xErr = select(pxInfo->xSockId + 1, NULL, &xWriteSet, &xErrorSet, &xTimeVal);
        if ((xErr < 0) || FD_ISSET(pxInfo->xSockId, &xErrorSet)) {
            if (errno == EINPROGRESS) {
                xErr = ERR_INPROGRESS;
            } else {
                ESP_LOGV(MBTCPMTAG, MB_SLAVE_FMT(" connection, select write err(errno) = 0x%x(%u)."),
                         (int)pxInfo->xIndex, (int)pxInfo->xSockId, pxInfo->pcIpAddr, (int)xErr, (unsigned)errno);
                xErr = ERR_CONN;
            }
        } else if (xErr == 0) {
            ESP_LOGV(MBTCPMTAG, "Socket(#%d)(%s), connection timeout occurred, err(errno) = 0x%x(%u).",
                     (int)pxInfo->xSockId, pxInfo->pcIpAddr, (int)xErr, (unsigned)errno);
            return ERR_INPROGRESS;
        } else {
            int xOptErr = 0;
            mbULONG ulOptLen = sizeof(xOptErr);
            // Check socket error
            xErr = getsockopt(pxInfo->xSockId, SOL_SOCKET, SO_ERROR, (void*)&xOptErr, (socklen_t*)&ulOptLen);
            if (xOptErr != 0) {
                ESP_LOGD(MBTCPMTAG, "Socket(#%d)(%s), sock error occurred (%u).",
                         (int)pxInfo->xSockId, pxInfo->pcIpAddr, (unsigned)xOptErr);
                return ERR_CONN;
            }
            ESP_LOGV(MBTCPMTAG, "Socket(#%d)(%s), is alive.",
                     (int)pxInfo->xSockId, pxInfo->pcIpAddr);
            return ERR_OK;
        }
    } else {
        xErr = ERR_CONN;
    }
    return xErr;
}

// Resolve host name and/or fill the IP address structure
static mbBOOL xMBTCPPortMasterCheckHost(const mbCHAR *pcHostStr, ip_addr_t *pxHostAddr)
{
    MB_PORT_CHECK((pcHostStr), FALSE, "Wrong host name or IP.");
    mbCHAR cStr[45];
    mbCHAR *pcStr = &cStr[0];
    ip_addr_t xTargetAddr;
    struct addrinfo xHint;
    struct addrinfo *pxAddrList;
    memset(&xHint, 0, sizeof(xHint));
    // Do name resolution for both protocols
    xHint.ai_family = AF_UNSPEC;
    xHint.ai_flags = AI_ADDRCONFIG; // get IPV6 address if supported, otherwise IPV4
    memset(&xTargetAddr, 0, sizeof(xTargetAddr));

    // convert domain name to IP address
    // Todo: check EAI_FAIL error when resolve host name
    int xRet = getaddrinfo(pcHostStr, NULL, &xHint, &pxAddrList);

    if (xRet != 0) {
        ESP_LOGE(MBTCPMTAG, "Incorrect host name or IP: %s", pcHostStr);
        return FALSE;
    }
    if (pxAddrList->ai_family == AF_INET) {
        struct in_addr addr4 = ((struct sockaddr_in *) (pxAddrList->ai_addr))->sin_addr;
        inet_addr_to_ip4addr(ip_2_ip4(&xTargetAddr), &addr4);
        pcStr = ip4addr_ntoa_r(ip_2_ip4(&xTargetAddr), cStr, sizeof(cStr));
    }
#if CONFIG_LWIP_IPV6
    else {
        struct in6_addr addr6 = ((struct sockaddr_in6 *) (pxAddrList->ai_addr))->sin6_addr;
        inet6_addr_to_ip6addr(ip_2_ip6(&xTargetAddr), &addr6);
        pcStr = ip6addr_ntoa_r(ip_2_ip6(&xTargetAddr), cStr, sizeof(cStr));
    }
#endif
    if (pxHostAddr) {
        *pxHostAddr = xTargetAddr;
    }
    ESP_LOGI(MBTCPMTAG, "Host[IP]: \"%s\"[%s]", pxAddrList->ai_canonname, pcStr);
    freeaddrinfo(pxAddrList);
    return TRUE;
}

mbBOOL xMBTCPPortMasterAddSlaveIp(const mbUSHORT usIndex, const mbCHAR *pcIpStr, mbUCHAR ucSlaveAddress)
{
    mbBOOL xRes = FALSE;
    MbSlaveAddrInfo_t xSlaveAddrInfo = {0};
    MB_PORT_CHECK(xMbPortConfig.xConnectQueue != NULL, FALSE, "Wrong slave IP address to add.");
    if (pcIpStr && (usIndex != 0xFF)) {
        xRes = xMBTCPPortMasterCheckHost(pcIpStr, NULL);
    }
    if (xRes || !pcIpStr) {
        xSlaveAddrInfo.pcIPAddr = pcIpStr;
        xSlaveAddrInfo.usIndex = usIndex;
        xSlaveAddrInfo.ucSlaveAddr = ucSlaveAddress;
        BaseType_t xStatus = xQueueSend(xMbPortConfig.xConnectQueue, (void *)&xSlaveAddrInfo, 100);
        MB_PORT_CHECK((xStatus == pdTRUE), FALSE, "FAIL to add slave IP address: [%s].", pcIpStr);
    }
    return xRes;
}

// Unblocking connect function
static err_t xMBTCPPortMasterConnect(MbSlaveInfo_t *pxInfo)
{
    if (!pxInfo) {
        return ERR_CONN;
    }

    err_t xErr = ERR_OK;
    mbCHAR cStr[128];
    mbCHAR *pcStr = NULL;
    ip_addr_t xTargetAddr;
    struct addrinfo xHint;
    struct addrinfo *pxAddrList;
    struct addrinfo *pxCurAddr;

    memset(&xHint, 0, sizeof(xHint));
    // Do name resolution for both protocols
    // xHint.ai_family = AF_UNSPEC; Todo: Find a reason why AF_UNSPEC does not work
    xHint.ai_flags = AI_ADDRCONFIG; // get IPV6 address if supported, otherwise IPV4
    xHint.ai_family = (xMbPortConfig.eMbIpVer == MB_PORT_IPV4) ? AF_INET : AF_INET6;
    xHint.ai_socktype = (pxInfo->xMbProto == MB_PROTO_UDP) ? SOCK_DGRAM : SOCK_STREAM;
    xHint.ai_protocol = (pxInfo->xMbProto == MB_PROTO_UDP) ? IPPROTO_UDP : IPPROTO_TCP;
    memset(&xTargetAddr, 0, sizeof(xTargetAddr));

    if (asprintf(&pcStr, "%u", xMbPortConfig.usPort) == -1) {
        abort();
    }

    // convert domain name to IP address
    int xRet = getaddrinfo(pxInfo->pcIpAddr, pcStr, &xHint, &pxAddrList);
    free(pcStr);
    if (xRet != 0) {
        ESP_LOGE(MBTCPMTAG, "Cannot resolve host: %s", pxInfo->pcIpAddr);
        return ERR_CONN;
    }

    for (pxCurAddr = pxAddrList; pxCurAddr != NULL; pxCurAddr = pxCurAddr->ai_next) {
        if (pxCurAddr->ai_family == AF_INET) {
            struct in_addr addr4 = ((struct sockaddr_in *) (pxCurAddr->ai_addr))->sin_addr;
            inet_addr_to_ip4addr(ip_2_ip4(&xTargetAddr), &addr4);
            pcStr = ip4addr_ntoa_r(ip_2_ip4(&xTargetAddr), cStr, sizeof(cStr));
        }
#if CONFIG_LWIP_IPV6
        else if (pxCurAddr->ai_family == AF_INET6) {
            struct in6_addr addr6 = ((struct sockaddr_in6 *) (pxCurAddr->ai_addr))->sin6_addr;
            inet6_addr_to_ip6addr(ip_2_ip6(&xTargetAddr), &addr6);
            pcStr = ip6addr_ntoa_r(ip_2_ip6(&xTargetAddr), cStr, sizeof(cStr));
            // Set scope id to fix routing issues with local address
            ((struct sockaddr_in6 *)(pxCurAddr->ai_addr))->sin6_scope_id =
                esp_netif_get_netif_impl_index(xMbPortConfig.pvNetIface);
        }
#endif
        if (pxInfo->xSockId <= 0) {
            pxInfo->xSockId = socket(pxCurAddr->ai_family, pxCurAddr->ai_socktype, pxCurAddr->ai_protocol);
            if (pxInfo->xSockId < 0) {
                ESP_LOGE(MBTCPMTAG, "Unable to create socket: #%d, errno %u", (int)pxInfo->xSockId, (unsigned)errno);
                xErr = ERR_IF;
                continue;
            }
        } else {
            ESP_LOGV(MBTCPMTAG, "Socket (#%d)(%s) created.", (int)pxInfo->xSockId, cStr);
        }

        // Set non blocking attribute for socket
        xMBTCPPortMasterSetNonBlocking(pxInfo);

        // Can return EINPROGRESS as an error which means
        // that connection is in progress and should be checked later
        xErr = connect(pxInfo->xSockId, (struct sockaddr*)pxCurAddr->ai_addr, pxCurAddr->ai_addrlen);
        if ((xErr < 0) && (errno == EINPROGRESS || errno == EALREADY)) {
            // The unblocking connect is pending (check status later) or already connected
            ESP_LOGV(MBTCPMTAG, "Socket(#%d)(%s) connection is pending, errno %u (%s).",
                     (int)pxInfo->xSockId, cStr, (unsigned)errno, strerror(errno));

            // Set keep alive flag in socket options
            vMBTCPPortSetKeepAlive(pxInfo);
            xErr = xMBTCPPortMasterCheckAlive(pxInfo, MB_TCP_CONNECTION_TIMEOUT_MS);
            continue;
        } else if ((xErr < 0) && (errno == EISCONN)) {
            // Socket already connected
            xErr = ERR_OK;
            continue;
        } else if (xErr != ERR_OK) {
            // Other error occurred during connection
            ESP_LOGV(MBTCPMTAG, MB_SLAVE_FMT(" unable to connect, error=0x%x, errno %u (%s)"),
                     (int)pxInfo->xIndex, (int)pxInfo->xSockId, cStr, (int)xErr, (unsigned)errno, strerror(errno));
            xMBTCPPortMasterCloseConnection(pxInfo);
            xErr = ERR_CONN;
        } else {
            ESP_LOGI(MBTCPMTAG, MB_SLAVE_FMT(", successfully connected."),
                     (int)pxInfo->xIndex, (int)pxInfo->xSockId, cStr);
            continue;
        }
    }
    freeaddrinfo(pxAddrList);
    return xErr;
}

// Find the first slave info whose descriptor is set in xFdSet
static MbSlaveInfo_t *xMBTCPPortMasterGetSlaveReady(fd_set *pxFdSet)
{
    MbSlaveInfo_t *pxInfo = NULL;

    // Slave connection loop
    for (int xIndex = 0; (xIndex < MB_TCP_PORT_MAX_CONN); xIndex++) {
        pxInfo = xMbPortConfig.pxMbSlaveInfo[xIndex];
        if (pxInfo) {
            // Is this response for current processing slave
            if (FD_ISSET(pxInfo->xSockId, pxFdSet)) {
                FD_CLR(pxInfo->xSockId, pxFdSet);
                return pxInfo;
            }
        }
    }
    return (MbSlaveInfo_t *)NULL;
}

static int xMBTCPPortMasterCheckConnState(fd_set *pxFdSet)
{
    fd_set xConnSetCheck = *pxFdSet;
    MbSlaveInfo_t *pxInfo = NULL;
    int64_t xTime = 0;
    int xErr = 0;
    int xCount = 0;
    do {
        xTime = xMBTCPMasterGetTimeStamp();
        pxInfo = xMBTCPPortMasterGetSlaveReady(&xConnSetCheck);
        if (pxInfo) {
            xErr = xMBTCPPortMasterCheckAlive(pxInfo, 0);
            if ((xErr < 0) && (((xTime - pxInfo->xRecvTimeStamp) > MB_TCP_RECONNECT_TIMEOUT) ||
                                ((xTime - pxInfo->xSendTimeStamp) > MB_TCP_RECONNECT_TIMEOUT))) {
                ESP_LOGI(MBTCPMTAG, MB_SLAVE_FMT(", slave is down, off_time[r][w](us) = [%" PRIu64 "][%" PRIu64 "]."),
                         (int)pxInfo->xIndex,
                         (int)pxInfo->xSockId,
                         pxInfo->pcIpAddr,
                         (int64_t)(xTime - pxInfo->xRecvTimeStamp),
                         (int64_t)(xTime - pxInfo->xSendTimeStamp));
                xCount++;
            }
        }
    } while (pxInfo && (xCount < MB_TCP_PORT_MAX_CONN));
    return xCount;
}

static void xMBTCPPortMasterFsmSetError(eMBMasterErrorEventType xErrType, eMBMasterEventEnum xPostEvent)
{
    vMBMasterPortTimersDisable();
    vMBMasterSetErrorType(xErrType);
    xMBMasterPortEventPost(xPostEvent);
}

static void vMBTCPPortMasterTask(void *pvParameters)
{
    MbSlaveInfo_t *pxInfo;
    MbSlaveInfo_t *pxCurrInfo;

    fd_set xConnSet;
    fd_set xReadSet;
    int xMaxSd = 0;
    err_t xErr = ERR_ABRT;
    mbUSHORT usSlaveConnCnt = 0;
    int64_t xTime = 0;

    // Register each slave in the connection info structure
    while (1) {
        MbSlaveAddrInfo_t xSlaveAddrInfo = { 0 };
        BaseType_t xStatus = xQueueReceive(xMbPortConfig.xConnectQueue, (void*)&xSlaveAddrInfo, pdMS_TO_TICKS(MB_EVENT_WAIT_TOUT_MS));
        TCP_PORT_CHECK_SHDN(xShutdownMSema, xMBTCPPortMasterShutdown);
        if (xStatus != pdTRUE) {
            ESP_LOGE(MBTCPMTAG, "Fail to register slave IP.");
        } else {
            if (xSlaveAddrInfo.pcIPAddr == NULL && xMbPortConfig.usMbSlaveInfoCount && xSlaveAddrInfo.usIndex == 0xFF) {
                // Init start timeout that allows to initialize the main FSM
                xMBMasterPortEventPost(EV_MASTER_READY);
                break;
            }
            if (xMbPortConfig.usMbSlaveInfoCount > MB_TCP_PORT_MAX_CONN) {
                ESP_LOGE(MBTCPMTAG, "Exceeds maximum connections limit=%u.", (unsigned)MB_TCP_PORT_MAX_CONN);
                break;
            }
            pxInfo = calloc(1, sizeof(MbSlaveInfo_t));
            if (!pxInfo) {
                ESP_LOGE(MBTCPMTAG, "Slave(#%u), info structure allocation fail.",
                         (unsigned)xMbPortConfig.usMbSlaveInfoCount);
                free(pxInfo);
                break;
            }
            pxInfo->pucRcvBuf = calloc(MB_TCP_BUF_SIZE, sizeof(mbUCHAR));
            if (!pxInfo->pucRcvBuf) {
                ESP_LOGE(MBTCPMTAG, "Slave(#%u), receive buffer allocation fail.",
                         (unsigned)xMbPortConfig.usMbSlaveInfoCount);
                free(pxInfo->pucRcvBuf);
                break;
            }
            pxInfo->usRcvPos = 0;
            pxInfo->pcIpAddr = xSlaveAddrInfo.pcIPAddr;
            pxInfo->xSockId = -1;
            pxInfo->xError = -1;
            pxInfo->xRecvTimeStamp = xMBTCPMasterGetTimeStamp();
            pxInfo->xSendTimeStamp = xMBTCPMasterGetTimeStamp();
            pxInfo->xMbProto = MB_PROTO_TCP;
            pxInfo->ucSlaveAddr = xSlaveAddrInfo.ucSlaveAddr;
            pxInfo->xIndex = xSlaveAddrInfo.usIndex;
            pxInfo->usTidCnt = (mbUSHORT)(xMbPortConfig.usMbSlaveInfoCount << 8U);
            // Register slave
            xMbPortConfig.pxMbSlaveInfo[xMbPortConfig.usMbSlaveInfoCount++] = pxInfo;
            ESP_LOGI(MBTCPMTAG, "Add slave IP: %s", xSlaveAddrInfo.pcIPAddr);
        }
    }

    // Main connection cycle
    while (1)
    {
        ESP_LOGI(MBTCPMTAG, "Connecting to slaves...");
        xTime = xMBTCPMasterGetTimeStamp();
        usSlaveConnCnt = 0;
        mbCHAR ucDot = '.';
        while(usSlaveConnCnt < xMbPortConfig.usMbSlaveInfoCount) {
            usSlaveConnCnt = 0;
            FD_ZERO(&xConnSet);
            ucDot ^= 0x03;
            // Slave connection loop
            for (mbUCHAR ucCnt = 0; (ucCnt < MB_TCP_PORT_MAX_CONN); ucCnt++) {
                pxInfo = xMbPortConfig.pxMbSlaveInfo[ucCnt];
                // if slave descriptor is NULL then it is end of list or connection closed.
                if (!pxInfo) {
                    ESP_LOGV(MBTCPMTAG, "Index: % is not initialized, skip.", ucCnt);
                    if (xMbPortConfig.usMbSlaveInfoCount) {
                        continue;
                    }
                    break;
                }
                putchar(ucDot);
                // if we don't yield we run the risk of hogging CPU
                vTaskDelay(pdMS_TO_TICKS(MB_TCP_CONNECTION_TIMEOUT_MS));
                xErr = xMBTCPPortMasterConnect(pxInfo);
                switch(xErr)
                {
                    case ERR_CONN:
                    case ERR_INPROGRESS:
                        // In case of connection errors remove the socket from set
                        if (FD_ISSET(pxInfo->xSockId, &xConnSet)) {
                            FD_CLR(pxInfo->xSockId, &xConnSet);
                            ESP_LOGE(MBTCPMTAG, MB_SLAVE_FMT(" connect failed, error = 0x%x."),
                                                                            (int)pxInfo->xIndex, (int)pxInfo->xSockId,
                                                                            (char*)pxInfo->pcIpAddr, (int)xErr);
                            if (usSlaveConnCnt) {
                                usSlaveConnCnt--;
                            }
                        }
                        break;
                    case ERR_OK:
                        // if connection is successful, add the descriptor into set
                        if (!FD_ISSET(pxInfo->xSockId, &xConnSet)) {
                            FD_SET(pxInfo->xSockId, &xConnSet);
                            usSlaveConnCnt++;
                            xMaxSd = (pxInfo->xSockId > xMaxSd) ? pxInfo->xSockId : xMaxSd;
                            ESP_LOGD(MBTCPMTAG, MB_SLAVE_FMT(", connected %u slave(s), error = 0x%x."),
                                                                (int)pxInfo->xIndex, (int)pxInfo->xSockId,
                                                                pxInfo->pcIpAddr,
                                                                (unsigned)usSlaveConnCnt, (int)xErr);
                            // Update time stamp for connected slaves
                            pxInfo->xRecvTimeStamp = xMBTCPMasterGetTimeStamp();
                            pxInfo->xSendTimeStamp = xMBTCPMasterGetTimeStamp();
                        }
                        break;
                    default:
                        ESP_LOGE(MBTCPMTAG, MB_SLAVE_FMT(", unexpected error = 0x%x."),
                                                            (int)pxInfo->xIndex,
                                                            (int)pxInfo->xSockId,
                                                            pxInfo->pcIpAddr, (int)xErr);
                        break;
                }
                if (pxInfo) {
                    pxInfo->xError = xErr;
                }
                TCP_PORT_CHECK_SHDN(xShutdownMSema, xMBTCPPortMasterShutdown);
            }
        }
        ESP_LOGI(MBTCPMTAG, "Connected %u slaves, start polling...", (unsigned)usSlaveConnCnt);

        vMBTCPPortMasterStartPoll(); // Send event to start stack

        // Slave receive data loop
        while(usSlaveConnCnt) {
            xReadSet = xConnSet;
            // Check transmission event to clear appropriate bit.
            xMBMasterPortFsmWaitConfirmation(EV_MASTER_FRAME_TRANSMIT, pdMS_TO_TICKS(MB_EVENT_WAIT_TOUT_MS));
            TCP_PORT_CHECK_SHDN(xShutdownMSema, xMBTCPPortMasterShutdown);
            // Synchronize state machine with send packet event
            if (xMBMasterPortFsmWaitConfirmation(EV_MASTER_FRAME_SENT, pdMS_TO_TICKS(MB_EVENT_WAIT_TOUT_MS))) {
                ESP_LOGD(MBTCPMTAG, "FSM Synchronized with sent event.");
            }
            // Get slave info for the current slave.
            pxCurrInfo = vMBTCPPortMasterGetCurrInfo();
            if (!pxCurrInfo) {
                ESP_LOGE(MBTCPMTAG, "Incorrect connection options for slave index: %d.",
                         (int)xMbPortConfig.ucCurSlaveIndex);
                vMBTCPPortMasterStopPoll();
                TCP_PORT_CHECK_SHDN(xShutdownMSema, xMBTCPPortMasterShutdown);
                break; // incorrect slave descriptor, reconnect.
            }
            xTime = xMBTCPPortMasterGetRespTimeLeft(pxCurrInfo);
            ESP_LOGD(MBTCPMTAG, "Set select timeout, left time: %" PRIu64 " ms.",
                     xMBTCPPortMasterGetRespTimeLeft(pxCurrInfo));
            // Wait respond from current slave during respond timeout
            int xRes = vMBTCPPortMasterRxCheck(pxCurrInfo->xSockId, &xReadSet, xTime);
            if (xRes == ERR_TIMEOUT) {
                // No respond from current slave, process timeout.
                // Need to drop response later if it is received after timeout.
                ESP_LOGD(MBTCPMTAG, "Select timeout, left time: %" PRIu64 " ms.",
                         xMBTCPPortMasterGetRespTimeLeft(pxCurrInfo));
                xTime = xMBTCPPortMasterGetRespTimeLeft(pxCurrInfo);
                // Wait completion of last transaction
                xMBMasterPortFsmWaitConfirmation(MB_EVENT_REQ_DONE_MASK, pdMS_TO_TICKS(xTime + 1));
                TCP_PORT_CHECK_SHDN(xShutdownMSema, xMBTCPPortMasterShutdown);
                continue;
            } else if (xRes < 0) {
                // Select error (slave connection or r/w failure).
                ESP_LOGD(MBTCPMTAG, MB_SLAVE_FMT(", socket select error. Slave disconnected?"),
                         (int)pxCurrInfo->xIndex, (int)pxCurrInfo->xSockId, pxCurrInfo->pcIpAddr);
                xTime = xMBTCPPortMasterGetRespTimeLeft(pxCurrInfo);
                // Wait completion of last transaction
                xMBMasterPortFsmWaitConfirmation(MB_EVENT_REQ_DONE_MASK, pdMS_TO_TICKS(xTime));
                // Stop polling process
                vMBTCPPortMasterStopPoll();
                TCP_PORT_CHECK_SHDN(xShutdownMSema, xMBTCPPortMasterShutdown);
                // Check disconnected slaves, do not need a result just to print information.
                xMBTCPPortMasterCheckConnState(&xConnSet);
                break;
            } else {
                // Check to make sure that active slave data is ready
                if (FD_ISSET(pxCurrInfo->xSockId, &xReadSet)) {
                    int xRet = ERR_BUF;
                    for (int retry = 0; (xRet == ERR_BUF) && (retry < MB_TCP_READ_BUF_RETRY_CNT); retry++) {
                        xRet = vMBTCPPortMasterReadPacket(pxCurrInfo);
                        // The error ERR_BUF means received response to previous request
                        // (due to timeout) with the same socket ID and incorrect TID,
                        // then ignore it and try to get next response buffer.
                    }
                    if (xRet > 0) {
                        // Response received correctly, send an event to stack
                        xMBTCPPortMasterFsmSetError(EV_ERROR_INIT, EV_MASTER_FRAME_RECEIVED);
                        ESP_LOGD(MBTCPMTAG, MB_SLAVE_FMT(", frame received."),
                                    (int)pxCurrInfo->xIndex, (int)pxCurrInfo->xSockId, pxCurrInfo->pcIpAddr);
                    } else if ((xRet == ERR_TIMEOUT) || (xMBTCPPortMasterGetRespTimeLeft(pxCurrInfo) == 0)) {
                        // Timeout occurred when receiving frame, process respond timeout
                        xMBTCPPortMasterFsmSetError(EV_ERROR_RESPOND_TIMEOUT, EV_MASTER_ERROR_PROCESS);
                        ESP_LOGD(MBTCPMTAG, MB_SLAVE_FMT(", frame read timeout."),
                                    (int)pxCurrInfo->xIndex, (int)pxCurrInfo->xSockId, pxCurrInfo->pcIpAddr);
                    } else if (xRet == ERR_BUF) {
                        // After retries a response with incorrect TID received, process failure.
                        xMBTCPPortMasterFsmSetError(EV_ERROR_RECEIVE_DATA, EV_MASTER_ERROR_PROCESS);
                        ESP_LOGD(MBTCPMTAG, MB_SLAVE_FMT(", frame error."),
                                    (int)pxCurrInfo->xIndex, (int)pxCurrInfo->xSockId, pxCurrInfo->pcIpAddr);
                    } else {
                        ESP_LOGE(MBTCPMTAG, MB_SLAVE_FMT(", critical error=%d."),
                                 (int)pxCurrInfo->xIndex, (int)pxCurrInfo->xSockId, pxCurrInfo->pcIpAddr, (int)xRet);
                        // Stop polling process
                        vMBTCPPortMasterStopPoll();
                        TCP_PORT_CHECK_SHDN(xShutdownMSema, xMBTCPPortMasterShutdown);
                        // Check disconnected slaves, do not need a result just to print information.
                        xMBTCPPortMasterCheckConnState(&xConnSet);
                        break;
                    }
                    xTime = xMBTCPPortMasterGetRespTimeLeft(pxCurrInfo);
                    ESP_LOGD(MBTCPMTAG, "Slave #%d, data processing left time %" PRIu64 " [ms].", (int)pxCurrInfo->xIndex, xTime);
                    // Wait completion of Modbus frame processing before start of new transaction.
                    if (xMBMasterPortFsmWaitConfirmation(MB_EVENT_REQ_DONE_MASK, pdMS_TO_TICKS(xTime))) {
                        ESP_LOGD(MBTCPMTAG, MB_SLAVE_FMT(", data processing completed."),
                                 (int)pxCurrInfo->xIndex, (int)pxCurrInfo->xSockId, pxCurrInfo->pcIpAddr);
                    }
                    xTime = xMBTCPMasterGetTimeStamp() - pxCurrInfo->xSendTimeStamp;
                    ESP_LOGD(MBTCPMTAG, MB_SLAVE_FMT(", processing time[us] = %" PRIu64 "."),
                             (int)pxCurrInfo->xIndex, (int)pxCurrInfo->xSockId, pxCurrInfo->pcIpAddr, xTime);
                }
            }
            TCP_PORT_CHECK_SHDN(xShutdownMSema, xMBTCPPortMasterShutdown);
        } // while(usMbSlaveInfoCount)
    } // while (1)
    vTaskDelete(NULL);
}

int xMBMasterTCPPortWritePoll(MbSlaveInfo_t *pxInfo, const mbUCHAR *pucMBTCPFrame, mbUSHORT usTCPLength, mbULONG xTimeout)
{
    // Check if the socket is alive (writable and SO_ERROR == 0)
    int xRes = (int)xMBTCPPortMasterCheckAlive(pxInfo, xTimeout);
    if ((xRes < 0) && (xRes != ERR_INPROGRESS)) {
        ESP_LOGE(MBTCPMTAG, MB_SLAVE_FMT(", is not writable, error: %d, errno %u"),
                 (int)pxInfo->xIndex, (int)pxInfo->xSockId, pxInfo->pcIpAddr, (int)xRes, (unsigned)errno);
        return xRes;
    }
    xRes = send(pxInfo->xSockId, pucMBTCPFrame, usTCPLength, TCP_NODELAY);
    if (xRes < 0) {
        ESP_LOGE(MBTCPMTAG, MB_SLAVE_FMT(", send data error: %d, errno %u"),
                 (int)pxInfo->xIndex, (int)pxInfo->xSockId, pxInfo->pcIpAddr, (int)xRes, (unsigned)errno);
    }
    return xRes;
}

// Timer handler to check timeout of socket response
mbBOOL MB_PORT_ISR_ATTR
xMBMasterTCPTimerExpired(void)
{
    mbBOOL xNeedPoll = FALSE;
    eMBMasterTimerMode eTimerMode = xMBMasterGetCurTimerMode();

    vMBMasterPortTimersDisable();

    // If timer mode is respond timeout, the master event then turns EV_MASTER_EXECUTE status.
    if (eTimerMode == MB_TMODE_RESPOND_TIMEOUT) {
        vMBMasterSetErrorType(EV_ERROR_RESPOND_TIMEOUT);
        xNeedPoll = xMBMasterPortEventPost(EV_MASTER_ERROR_PROCESS);
    }

    return xNeedPoll;
}
/********************** Target Specific ENDED ************************/

mbBOOL
xMBMasterTCPPortInit( mbUSHORT usTCPPort )
{
    mbBOOL bOkay = FALSE;

    xMbPortConfig.pxMbSlaveInfo = calloc(MB_TCP_PORT_MAX_CONN, sizeof(MbSlaveInfo_t*));
    if (!xMbPortConfig.pxMbSlaveInfo) {
        ESP_LOGE(MBTCPMTAG, "TCP slave info alloc failure.");
        return FALSE;
    }
    for(int idx = 0; idx < MB_TCP_PORT_MAX_CONN; xMbPortConfig.pxMbSlaveInfo[idx] = NULL, idx++);

    xMbPortConfig.xConnectQueue = NULL;
    xMbPortConfig.usPort = usTCPPort;
    xMbPortConfig.usMbSlaveInfoCount = 0;
    xMbPortConfig.ucCurSlaveIndex = 1;
    xMbPortConfig.pxMbSlaveCurrInfo = NULL;

    xMbPortConfig.xConnectQueue = xQueueCreate(2, sizeof(MbSlaveAddrInfo_t));
    if (xMbPortConfig.xConnectQueue == 0)
    {
        // Queue was not created and must not be used.
        ESP_LOGE(MBTCPMTAG, "TCP master queue creation failure.");
        return FALSE;
    }

    // Create task for packet processing
    BaseType_t xErr = xTaskCreatePinnedToCore(vMBTCPPortMasterTask,
                                              "mbm_port_tcp_task",
                                              MB_TCP_STACK_SIZE,
                                              NULL,
                                              MB_TCP_TASK_PRIO,
                                              &xMbPortConfig.xMbTcpTaskHandle,
                                              MB_PORT_TASK_AFFINITY);
    if (xErr != pdTRUE)
    {
        ESP_LOGE(MBTCPMTAG, "TCP master task creation failure.");
        (void)vTaskDelete(xMbPortConfig.xMbTcpTaskHandle);
    } else {
        ESP_LOGI(MBTCPMTAG, "TCP master stack initialized.");
        bOkay = TRUE;
    }
    return bOkay;
}

void
vMBMasterTCPPortClose( void )
{
    vQueueDelete(xMbPortConfig.xConnectQueue);
    vMBMasterPortTimerClose();
    // Release resources for the event queue.
    vMBMasterPortEventClose();
}

void
vMBMasterTCPPortEnable( void )
{
    ;
}

void
vMBMasterTCPPortDisable( void )
{
    // Try to exit the task gracefully, so select could release its internal callbacks
    // that were allocated on the stack of the task we're going to delete
    xShutdownMSema = xSemaphoreCreateBinary();
    // if no semaphore (alloc issues) or couldn't acquire it, just delete the task
    if (xShutdownMSema == NULL || xSemaphoreTake(xShutdownMSema, pdMS_TO_TICKS(MB_SHDN_WAIT_TOUT_MS)) != pdTRUE) {
        ESP_LOGW(MBTCPMTAG, "Modbus port task couldn't exit gracefully within timeout -> abruptly deleting the task.");
        vTaskDelete(xMbPortConfig.xMbTcpTaskHandle);
    }
    if (xShutdownMSema) {
        vSemaphoreDelete(xShutdownMSema);
        xShutdownMSema = NULL;
    }
}

mbBOOL
xMBMasterTCPPortGetRequest( mbUCHAR **ppucMBTCPFrame, mbUSHORT * usTCPLength )
{
    MbSlaveInfo_t *pxInfo = vMBTCPPortMasterGetCurrInfo();
    *ppucMBTCPFrame = pxInfo->pucRcvBuf;
    *usTCPLength = pxInfo->usRcvPos;

    // Reset the buffer.
    pxInfo->usRcvPos = 0;
    // Save slave receive timestamp
    if (pxInfo->xRcvErr == ERR_OK && *usTCPLength > 0) {
        pxInfo->xRecvTimeStamp = xMBTCPMasterGetTimeStamp();
        return TRUE;
    }
    return FALSE;
}

mbBOOL
xMBMasterTCPPortSendResponse( mbUCHAR *pucMBTCPFrame, mbUSHORT usTCPLength )
{
    mbBOOL bFrameSent = FALSE;
    mbUSHORT ucCurSlaveIndex = ucMBMasterGetDestAddress();
    MbSlaveInfo_t *pxInfo = vMBTCPPortMasterFindSlaveInfo(ucCurSlaveIndex);

    // If the slave is correct and active then send data
    // otherwise treat slave as died and skip
    if (pxInfo != NULL) {
        if (pxInfo->xSockId < 0) {
            ESP_LOGD(MBTCPMTAG, MB_SLAVE_FMT(", send to died slave, error = %u"),
                                                (int)pxInfo->xIndex, (int)pxInfo->xSockId, pxInfo->pcIpAddr, (unsigned)pxInfo->xError);
        } else {
            // Apply TID field to the frame before send
            pucMBTCPFrame[MB_TCP_TID] = (mbUCHAR)(pxInfo->usTidCnt >> 8U);
            pucMBTCPFrame[MB_TCP_TID + 1] = (mbUCHAR)(pxInfo->usTidCnt & 0xFF);

            int xRes = xMBMasterTCPPortWritePoll(pxInfo, pucMBTCPFrame, usTCPLength, MB_TCP_SEND_TIMEOUT_MS);
            if (xRes < 0) {
                ESP_LOGE(MBTCPMTAG, MB_SLAVE_FMT(", send data failure, err(errno) = %d(%u)."),
                         (int)pxInfo->xIndex, (int)pxInfo->xSockId, pxInfo->pcIpAddr, (int)xRes, (unsigned)errno);
                bFrameSent = FALSE;
                pxInfo->xError = xRes;
            } else {
                bFrameSent = TRUE;
                ESP_LOGD(MBTCPMTAG, MB_SLAVE_FMT(", send data successful: TID=0x%02x, %d (bytes), errno %u"),
                         (int)pxInfo->xIndex, (int)pxInfo->xSockId, pxInfo->pcIpAddr, pxInfo->usTidCnt, (int)xRes, (unsigned)errno);
                pxInfo->xError = 0;
                pxInfo->usRcvPos = 0;
                if (pxInfo->usTidCnt < (USHRT_MAX - 1)) {
                    pxInfo->usTidCnt++;
                } else {
                    pxInfo->usTidCnt = (mbUSHORT)(pxInfo->xIndex << 8U);
                }
            }
            pxInfo->xSendTimeStamp = xMBTCPMasterGetTimeStamp();
        }
    } else {
        ESP_LOGD(MBTCPMTAG, "Send data to died slave, address = %u", (unsigned)ucCurSlaveIndex);
    }
    vMBMasterPortTimersRespondTimeoutEnable();
    xMBMasterPortEventPost(EV_MASTER_FRAME_SENT);
    return bFrameSent;
}
#endif

/********************** EHS Specific START ************************/

void EhsHMBTaskInLoop( ehs_bool use_tcpip )
{
    if (gbEhsMBInitialised == EHS_FALSE) return ;

    if (use_tcpip == EHS_TRUE && gbEhsMBTCPIPFlag == EHS_TRUE) return ;

    if (gbEhsMBMasterFlag == EHS_FALSE)
    {
        (void)eMBPoll();
        if (gbEhsMBTCPIPFlag == EHS_TRUE) return ;
        if (xMBPortSerialTxPoll())
        {
            (void)xMBPortEventPost( EV_FRAME_SENT );
        }
    }
    else
    {
        (void)eMBMasterPoll();
        if (gbEhsMBTCPIPFlag == EHS_TRUE) return ;
        if (xMBMasterPortSerialTxPoll())
        {
            (void)xMBMasterPortEventPost( EV_MASTER_FRAME_SENT );
        }
    }
}

/********************** EHS Specific ENDED ************************/