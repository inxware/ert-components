/*---------------------------------------------------------------
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
 *---------------------------------------------------------------*/

/**
 * @file target_uart.c
 * Zephyr UART HAL implementation for eRT.
 *
 * UART devices are obtained via DTS aliases inx-uart0, inx-uart1, etc.
 * defined in the board overlay.  Only port 0 (inx-uart0) is wired up
 * in this initial implementation; additional ports can be added by
 * extending the alias array below.
 *
 * Transmit is synchronous (polling).  Receive uses the async IRQ callback
 * path when TgtUART_Intr_register() is called; otherwise RecvPoll is
 * provided as a fallback.
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include "globals.h"
#include "target_uart.h"

/* --------------------------------------------------------------------------
 * DTS-sourced device handles
 * -------------------------------------------------------------------------- */

static const struct device *uart_dev[UART_COUNT];

static void uart_init_devices(void)
{
    static int done = 0;
    if (done) return;

#if DT_NODE_HAS_STATUS(DT_ALIAS(inx_uart0), okay)
    uart_dev[0] = DEVICE_DT_GET(DT_ALIAS(inx_uart0));
#endif
#if UART_COUNT > 1 && DT_NODE_HAS_STATUS(DT_ALIAS(inx_uart1), okay)
    uart_dev[1] = DEVICE_DT_GET(DT_ALIAS(inx_uart1));
#endif

    done = 1;
}

/* --------------------------------------------------------------------------
 * Static receive callback storage
 * -------------------------------------------------------------------------- */

static uart_cb_func_t uart_rx_cb[UART_COUNT];

/* Zephyr IRQ callback — byte-at-a-time receive via interrupt-driven API */
static void uart_irq_handler(const struct device *dev, void *user_data)
{
    int port = (int)(intptr_t)user_data;
    if (!uart_irq_update(dev)) return;

    while (uart_irq_rx_ready(dev)) {
        uint8_t ch;
        int n = uart_fifo_read(dev, &ch, 1);
        if (n > 0 && port < UART_COUNT && uart_rx_cb[port]) {
            uart_rx_cb[port]((char *)&ch, 1);
        }
    }
}

/* --------------------------------------------------------------------------
 * Public API
 * -------------------------------------------------------------------------- */

int TgtUart_Stage0(int uart_num)
{
    return TgtUART_OK;
}

int TgtUart_Start(int uart_num, int tx_io, int rx_io, int rts_io, int cts_io,
                  int baudrate, int databits, int parity, int stop_bits,
                  int flow_control)
{
    uart_init_devices();

    if (uart_num < 0 || uart_num >= UART_COUNT) return TgtUART_ERR;
    if (uart_dev[uart_num] == NULL ||
        !device_is_ready(uart_dev[uart_num])) return TgtUART_ERR;

    struct uart_config cfg = {
        .baudrate  = (uint32_t)(baudrate > 0 ? baudrate : 115200),
        .parity    = (parity == 1) ? UART_CFG_PARITY_ODD  :
                     (parity == 2) ? UART_CFG_PARITY_EVEN : UART_CFG_PARITY_NONE,
        .stop_bits = (stop_bits == 2) ? UART_CFG_STOP_BITS_2 : UART_CFG_STOP_BITS_1,
        .data_bits = UART_CFG_DATA_BITS_8,
        .flow_ctrl = UART_CFG_FLOW_CTRL_NONE,
    };

    int rc = uart_configure(uart_dev[uart_num], &cfg);
    return (rc == 0) ? TgtUART_OK : TgtUART_ERR;
}

int TgtUart_Stop(int uart_num)
{
    return TgtUART_OK;
}

int TgtUart_RS485Setup(int uart_num)
{
    return TgtUART_OK;
}

int TgtUart_Send(int uart_num, char *payload, unsigned int length)
{
    if (uart_num < 0 || uart_num >= UART_COUNT) return TgtUART_ERR;
    if (uart_dev[uart_num] == NULL) return TgtUART_ERR;

    for (unsigned int i = 0; i < length; i++) {
        uart_poll_out(uart_dev[uart_num], (unsigned char)payload[i]);
    }
    return TgtUART_OK;
}

void TgtUART_SendThread(int uart_num, char *payload, unsigned int length)
{
    TgtUart_Send(uart_num, payload, length);
}

int TgtUART_SendInThread(int uart_num)
{
    return 0; /* sent synchronously */
}

int TgtUART_RecvPoll(int uart_num, char *payload, unsigned int length)
{
    if (uart_num < 0 || uart_num >= UART_COUNT) return TgtUART_ERR;
    if (uart_dev[uart_num] == NULL) return TgtUART_ERR;

    for (unsigned int i = 0; i < length; i++) {
        int rc = uart_poll_in(uart_dev[uart_num], (unsigned char *)&payload[i]);
        if (rc != 0) return (int)i; /* return bytes received so far */
    }
    return (int)length;
}

int TgtUART_Intr_register(int uart_num, uart_cb_func_t cb_func)
{
    if (uart_num < 0 || uart_num >= UART_COUNT) return TgtUART_ERR;
    if (uart_dev[uart_num] == NULL) return TgtUART_ERR;

    uart_rx_cb[uart_num] = cb_func;

    if (cb_func) {
        uart_irq_callback_user_data_set(uart_dev[uart_num],
                                        uart_irq_handler,
                                        (void *)(intptr_t)uart_num);
        uart_irq_rx_enable(uart_dev[uart_num]);
    } else {
        uart_irq_rx_disable(uart_dev[uart_num]);
    }
    return TgtUART_OK;
}

int TgtUart_Config(int uart_num, int baudrate, int databits, int parity,
                   int stop_bits, int flow_control)
{
    return TgtUart_Start(uart_num, -1, -1, -1, -1,
                         baudrate, databits, parity, stop_bits, flow_control);
}

int TgtUart_SetComPort(int uart_num, int com_port_number)
{
    (void)uart_num; (void)com_port_number;
    return TgtUART_OK;
}

int TgtUart_SetDevicePath(int uart_num, const char *path)
{
    (void)uart_num; (void)path;
    return TgtUART_OK;
}
