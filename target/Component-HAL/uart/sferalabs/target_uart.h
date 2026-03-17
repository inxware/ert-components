/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file target_uart.h
 * @brief Sfera Labs UART / RS-485 HAL header.
 *
 * Uses standard Linux termios serial I/O on the board's serial port(s).
 * RS-485 TX-enable is managed automatically by the Sfera Labs kernel driver
 * and optionally configured via sysfs when RS-485 mode is enabled.
 *
 * Device paths default to SFERALABS_UART0_DEV (defined in sferalabs_hal.h)
 * but can be overridden per-port in the platform config.mk:
 *   DEFS += SFERALABS_UART0_DEV=\"/dev/ttyAMA0\"
 *   DEFS += SFERALABS_UART1_DEV=\"/dev/ttyS0\"
 */

#ifndef TARGET_UART_SFERALABS_H
#define TARGET_UART_SFERALABS_H

#include "sferalabs_hal.h"

/* Re-use the common UART count, status struct, and API from the stubbed header */
#ifndef EHS_TARGET_UART_COUNT
#define EHS_TARGET_UART_COUNT 2
#endif
#define UART_COUNT EHS_TARGET_UART_COUNT

/* Fallback device paths if not set by platform config */
#ifndef SFERALABS_UART1_DEV
#define SFERALABS_UART1_DEV "/dev/ttyS0"
#endif

#define UART_TX_BUFFER_SIZE 1024
#define UART_RX_BUFFER_SIZE 1024

typedef void (*uart_cb_func_t)(char *payload, int length);

enum TgtUART_Return_t {
    TgtUART_OK               =   0,
    TgtUART_INEXIST          =  -1,
    TgtUART_INUSE            =  -2,
    TgtUART_NOTINUSE         =  -3,
    TgtUART_NOTCONFIGURED    =  -4,
    TgtUART_BAUDRATEERROR    =  -5,
    TgtUART_CONFIG_ERROR     =  -6,
    TgtUART_ERROR            =  -7,
    TgtUART_LENGTH_NOT_MATCH =  -8,
    TgtUART_INVALID_CALLBACK = -18
};

int TgtUart_Stage0(int UART_num);
int TgtUart_Start(int UART_num, int tx_io, int rx_io, int rts_io, int cts_io,
                  int baudrate, int databits, int parity, int stop_bits, int flow_control);
int TgtUart_RS485Setup(int UART_num);
int TgtUart_Stop(int UART_num);
int TgtUart_Send(int UART_num, char *payload, unsigned int length);
int TgtUART_RecvPoll(int UART_num, char *payload, unsigned int length);
int TgtUART_Intr_register(int UART_num, uart_cb_func_t cb_func);

void Common_UART_onReceive(char *recv_msg, int length);
void Common_UART_onSendComplete(int retCode);

#define TgtUART_WDT_feed()

#endif /* TARGET_UART_SFERALABS_H */
