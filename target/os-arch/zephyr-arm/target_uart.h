/*---------------------------------------------------------------
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
 *---------------------------------------------------------------*/

/**
 * @file target_uart.h
 * Zephyr UART HAL header for eRT.
 *
 * UART 0 is driven via the DTS alias inx-uart0 defined in the board overlay.
 * Higher-numbered ports use inx-uart1, etc. when defined.
 */

#ifndef TARGET_UART_ZEPHYR_H_
#define TARGET_UART_ZEPHYR_H_

#include "globals.h"

#ifndef EHS_TARGET_UART_COUNT
#define EHS_TARGET_UART_COUNT 1
#endif

#define UART_COUNT EHS_TARGET_UART_COUNT

#define TgtUART_OK   0
#define TgtUART_ERR (-1)

typedef void (*uart_cb_func_t)(char *payload, int length);

typedef struct {
    const void         *dev;        /**< Zephyr device pointer */
    uart_cb_func_t      cb;         /**< Receive callback, or NULL */
    ehs_bool            configured;
} TgtUART_t;

/* Public API */
int TgtUart_Stage0(int uart_num);
int TgtUart_Start(int uart_num, int tx_io, int rx_io, int rts_io, int cts_io,
                  int baudrate, int databits, int parity, int stop_bits,
                  int flow_control);
int TgtUart_Stop(int uart_num);
int TgtUart_Send(int uart_num, char *payload, unsigned int length);
int TgtUART_RecvPoll(int uart_num, char *payload, unsigned int length);
int TgtUART_Intr_register(int uart_num, uart_cb_func_t cb_func);
int TgtUart_Config(int uart_num, int baudrate, int databits, int parity,
                   int stop_bits, int flow_control);
int TgtUart_SetComPort(int uart_num, int com_port_number);
int TgtUart_SetDevicePath(int uart_num, const char *path);
int TgtUart_RS485Setup(int uart_num);
void TgtUART_SendThread(int uart_num, char *payload, unsigned int length);
int  TgtUART_SendInThread(int uart_num);

#endif /* TARGET_UART_ZEPHYR_H_ */
