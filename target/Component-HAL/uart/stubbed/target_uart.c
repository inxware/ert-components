#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "globals.h"


#include "target_uart.h"

static int esp_ret;

// Function prototypes

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
    return ret;
}

int TgtUart_RS485Setup(int UART_num)
{
    int ret = TgtUART_OK;
    return ret;
}

/*
 * Stop the UART communication. Reset all related UART instances
 * */
int TgtUart_Stop(int UART_num)
{
    int ret = TgtUART_OK;
    return ret;
}

/*
 * Send UART paylaod with a given length
 * */
int TgtUart_Send(int UART_num, char *payload, unsigned int length)
{
    int ret = TgtUART_OK;
    return ret;
}

/**
 * Send UART payload with a given length with threading.
 * Returns immediately without blocking. This should be used with TgtUART_SendInThread.
 * */
void TgtUART_SendThread(int UART_num, char *payload, unsigned int length)
{
    ;
}

/**
 * Send UART payload in thread.
 * Return code <= 0 for sending success and failure.
 * Return code > 0 (i.e. 1) for not sending the message.
 * */
int TgtUART_SendInThread(int UART_num)
{
    int ret = 0;
    return ret;
}

/*
 * Receive the UART message of a given length into the payload by polling
 * */
int TgtUART_RecvPoll(int UART_num, char *payload, unsigned int length)
{
    int ret = TgtUART_OK;
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
    return ret;
}

int TgtUart_Config(int UART_num, int baudrate, int databits, int parity,
                   int stop_bits, int flow_control)
{
    (void)UART_num; (void)baudrate; (void)databits;
    (void)parity;   (void)stop_bits; (void)flow_control;
    return TgtUART_OK;
}

int TgtUart_SetComPort(int UART_num, int com_port_number)
{
    (void)UART_num; (void)com_port_number;
    return TgtUART_OK;
}

int TgtUart_SetDevicePath(int UART_num, const char *path)
{
    (void)UART_num; (void)path;
    return TgtUART_OK;
}
