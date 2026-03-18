/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license.
 ***************************************************************/

/**
 * @file target_uart.h  (win32)
 * @brief Win32 serial COM-port UART HAL declarations.
 *
 * Implements the standard target_uart.h API using Win32 CreateFile /
 * ReadFile / WriteFile serial port APIs.  A background receive thread
 * is started per port in TgtUart_Start(); it calls the callback
 * registered via TgtUART_Intr_register() whenever bytes arrive.
 *
 * COM port selection:
 *   Port 0 → WIN32_UART0_COM_PORT  (default "\\\\.\\COM3")
 *   Port 1 → WIN32_UART1_COM_PORT  (default "\\\\.\\COM4")
 *
 * Override before including this header (or via CFLAGS) to use a
 * different port, e.g. -DWIN32_UART0_COM_PORT="\\\\.\\COM5".
 *
 * Note: the LoRaWAN WIO-E5 USB–serial adapter commonly enumerates as
 * COM3 on Windows.  Check Device Manager for the actual port number.
 */

#ifndef _TARGET_UART_H_
#define _TARGET_UART_H_

#include <windows.h>

/* -----------------------------------------------------------------------
 * COM port name defaults (one per supported UART port)
 * ----------------------------------------------------------------------- */

#ifndef WIN32_UART0_COM_PORT
#define WIN32_UART0_COM_PORT "\\\\.\\COM3"
#endif

#ifndef WIN32_UART1_COM_PORT
#define WIN32_UART1_COM_PORT "\\\\.\\COM4"
#endif

#ifndef WIN32_UART2_COM_PORT
#define WIN32_UART2_COM_PORT "\\\\.\\COM5"
#endif

#ifndef WIN32_UART3_COM_PORT
#define WIN32_UART3_COM_PORT "\\\\.\\COM6"
#endif

/* -----------------------------------------------------------------------
 * Port count
 * ----------------------------------------------------------------------- */

#ifndef EHS_TARGET_UART_COUNT
#define EHS_TARGET_UART_COUNT 2
#endif
#define UART_COUNT EHS_TARGET_UART_COUNT

extern ehs_uint32 gUARTBaudRate[EHS_TARGET_UART_COUNT];
extern ehs_uint8  gUARTStopBits[EHS_TARGET_UART_COUNT];
extern ehs_uint8  gUARTParity[EHS_TARGET_UART_COUNT];
extern ehs_uint8  gUARTHWCTRL[EHS_TARGET_UART_COUNT];
extern ehs_uint8  gUARTLength[EHS_TARGET_UART_COUNT];

/* -----------------------------------------------------------------------
 * Buffer sizes
 * ----------------------------------------------------------------------- */

#define UART_TX_BUFFER_SIZE 1024
#define UART_RX_BUFFER_SIZE 1024
#define UART_QUEUE_SIZE     20

/* -----------------------------------------------------------------------
 * Pin macros (not applicable on Win32 — COM ports have no pin numbers)
 * Provided for API compatibility only; values are ignored by TgtUart_Start.
 * ----------------------------------------------------------------------- */

#define TARGET_UART_PIN_RX_PORT(x)  TARGET_UART_PIN_RX_PORT_(x)
#define TARGET_UART_PIN_RX_PORT_(x) TARGET_UART_PIN_RX_PORT ## x
#define TARGET_UART_PIN_TX_PORT(x)  TARGET_UART_PIN_TX_PORT_(x)
#define TARGET_UART_PIN_TX_PORT_(x) TARGET_UART_PIN_TX_PORT ## x
#define TARGET_UART_PIN_RTS_PORT(x) TARGET_UART_PIN_RTS_PORT_(x)
#define TARGET_UART_PIN_RTS_PORT_(x)TARGET_UART_PIN_RTS_PORT ## x
#define TARGET_UART_PIN_CTS_PORT(x) TARGET_UART_PIN_CTS_PORT_(x)
#define TARGET_UART_PIN_CTS_PORT_(x)TARGET_UART_PIN_CTS_PORT ## x

#ifndef TARGET_UART_PIN_TX_PORT0
#define TARGET_UART_PIN_TX_PORT0  -1
#endif
#ifndef TARGET_UART_PIN_RX_PORT0
#define TARGET_UART_PIN_RX_PORT0  -1
#endif
#ifndef TARGET_UART_PIN_RTS_PORT0
#define TARGET_UART_PIN_RTS_PORT0 -1
#endif
#ifndef TARGET_UART_PIN_CTS_PORT0
#define TARGET_UART_PIN_CTS_PORT0 -1
#endif

#ifndef TARGET_UART_PIN_TX_PORT1
#define TARGET_UART_PIN_TX_PORT1  -1
#endif
#ifndef TARGET_UART_PIN_RX_PORT1
#define TARGET_UART_PIN_RX_PORT1  -1
#endif
#ifndef TARGET_UART_PIN_RTS_PORT1
#define TARGET_UART_PIN_RTS_PORT1 -1
#endif
#ifndef TARGET_UART_PIN_CTS_PORT1
#define TARGET_UART_PIN_CTS_PORT1 -1
#endif

#ifndef TARGET_UART_PIN_TX_PORT2
#define TARGET_UART_PIN_TX_PORT2  -1
#endif
#ifndef TARGET_UART_PIN_RX_PORT2
#define TARGET_UART_PIN_RX_PORT2  -1
#endif
#ifndef TARGET_UART_PIN_RTS_PORT2
#define TARGET_UART_PIN_RTS_PORT2 -1
#endif
#ifndef TARGET_UART_PIN_CTS_PORT2
#define TARGET_UART_PIN_CTS_PORT2 -1
#endif

#ifndef TARGET_UART_PIN_TX_PORT3
#define TARGET_UART_PIN_TX_PORT3  -1
#endif
#ifndef TARGET_UART_PIN_RX_PORT3
#define TARGET_UART_PIN_RX_PORT3  -1
#endif
#ifndef TARGET_UART_PIN_RTS_PORT3
#define TARGET_UART_PIN_RTS_PORT3 -1
#endif
#ifndef TARGET_UART_PIN_CTS_PORT3
#define TARGET_UART_PIN_CTS_PORT3 -1
#endif

extern const int gTargetUartPinTx[];
extern const int gTargetUartPinRx[];
extern const int gTargetUartPinCts[];
extern const int gTargetUartPinRts[];

/* -----------------------------------------------------------------------
 * Status structure
 * ----------------------------------------------------------------------- */

typedef struct {
    ehs_bool configured;
    ehs_bool inUse;
    void    *intrCtxObj;
    void    *intrObj;
    ehs_bool sending;
    ehs_char data_to_send[250];
    ehs_uint8 len_data_to_send;
    ehs_char data_received[250];
    ehs_uint8 len_data_received;
} TgtUART_t;

static const TgtUART_t TgtUART_t_default = {
    .configured  = EHS_FALSE,
    .inUse       = EHS_FALSE,
    .intrCtxObj  = NULL,
    .intrObj     = NULL
};
static inline TgtUART_t TgtUART_getTgtUART_t_default(void) {
    return TgtUART_t_default;
}

enum UART_State {
    Stopped  = 0,
    Running,
    Sending,
    Received
};

/* -----------------------------------------------------------------------
 * Configuration tables (shared with inx-uart_config.c)
 * ----------------------------------------------------------------------- */

#define UART_BAUDRATE_MAX 921600

typedef void (*uart_cb_func_t)(char *payload, int length);

#if UART_COUNT == 1
static uart_cb_func_t UART_CALLBACK_FUNCTIONS[UART_COUNT] = {NULL};
#elif UART_COUNT == 2
static uart_cb_func_t UART_CALLBACK_FUNCTIONS[UART_COUNT] = {NULL, NULL};
#elif UART_COUNT >= 3
static uart_cb_func_t UART_CALLBACK_FUNCTIONS[UART_COUNT] = {NULL, NULL, NULL, NULL};
#endif

#define UART_WORD_LENGTH_COUNT 9
static int UART_WORD_LENGTH[UART_WORD_LENGTH_COUNT] = {
    0, 0, 0, 0, 0, 5, 6, 7, 8
};

#define UART_STOP_BITS_COUNT 3
static int UART_STOP_BITS[UART_STOP_BITS_COUNT] = {
    0, 1, 2
};

#define UART_PARITY_COUNT 3
static int gEhsUART_PARITY[UART_PARITY_COUNT] = {
    0, 1, 2
};

#define UART_HW_FLOWCTRL_COUNT 4
static int UART_HW_FLOWCTRL[UART_PARITY_COUNT] = {
    0, 1, 2
};

/* -----------------------------------------------------------------------
 * Return codes
 * ----------------------------------------------------------------------- */

enum TgtUART_Return_t {
    TgtUART_OK                   =   0,
    TgtUART_INEXIST              =  -1,
    TgtUART_INUSE                =  -2,
    TgtUART_NOTINUSE             =  -3,
    TgtUART_NOTCONFIGURED        =  -4,
    TgtUART_BAUDRATEERROR        =  -5,
    TgtUART_CONFIG_ERROR         =  -6,
    TgtUART_ERROR                =  -7,
    TgtUART_LENGTH_NOT_MATCH     =  -8,
    TgtUART_INTERRUPT_EXISTED    =  -9,
    TgtUART_INTERRUPT_NO_CB      = -10,
    TgtUART_INTERRUPT_REG_ERROR  = -11,
    TgtUART_INTERRUPT_DEL_ERROR  = -12,
    TgtUART_DATA_ERROR           = -13,
    TgtUART_BUFFER_FULL          = -14,
    TgtUART_EVENT_UNDEFINED      = -15,
    TgtUART_HEAP_INSUFFICIENT    = -16,
    TgtUART_INVALID_CALLBACK     = -18,
    TgtUART_ESPERROR             = -100
};

/* -----------------------------------------------------------------------
 * API
 * ----------------------------------------------------------------------- */

int TgtUart_Stage0(int UART_num);

int TgtUart_Start(int UART_num, int tx_io, int rx_io, int rts_io, int cts_io,
                  int baudrate, int databits, int parity, int stop_bits,
                  int flow_control);

int TgtUart_Config(int UART_num, int baudrate, int databits, int parity,
                   int stop_bits, int flow_control);

int TgtUart_RS485Setup(int UART_num);

int TgtUart_Stop(int UART_num);

int TgtUart_Send(int UART_num, char *payload, unsigned int length);

void TgtUART_SendThread(int UART_num, char *payload, unsigned int length);

int TgtUART_SendInThread(int UART_num);

int TgtUART_RecvPoll(int UART_num, char *payload, unsigned int length);

int TgtUART_Intr_register(int UART_num, uart_cb_func_t cb_func);

int TgtUart_SetComPort(int UART_num, int com_port_number);

int TgtUart_SetDevicePath(int UART_num, const char *path);

/* -----------------------------------------------------------------------
 * WDT feed — on Windows, yield the CPU briefly while spinning in the
 * LoRaWAN send-wait loop to avoid pegging a core at 100%.
 * ----------------------------------------------------------------------- */
#define TgtUART_WDT_feed() Sleep(1)

void Common_UART_onReceive(char *recv_msg, int length);
void Common_UART_onSendComplete(int retCode);

#endif /* _TARGET_UART_H_ */
