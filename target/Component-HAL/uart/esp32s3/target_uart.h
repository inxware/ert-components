/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license.
 ***************************************************************/

/**
 * @file target_uart.h  (esp32s3)
 * @brief ESP32-S3 IDF UART HAL declarations.
 *
 * Uses the ESP-IDF uart driver (driver/uart.h).  A FreeRTOS task is
 * spawned per port via TgtUART_Intr_register(); it blocks on the IDF
 * event queue and calls the registered callback when UART_DATA events
 * arrive.
 *
 * GPIO pin assignments:
 *   Configured at compile time via TARGET_UART_PIN_{TX,RX,RTS,CTS}_PORTn macros.
 *   Defaults to -1 (use IDF default UART-matrix routing) unless overridden
 *   in the platform config.mk, e.g.:
 *     DEFS += TARGET_UART_PIN_TX_PORT0=17
 *     DEFS += TARGET_UART_PIN_RX_PORT0=18
 *
 * Port count:
 *   Default 1; override with -DEHS_TARGET_UART_COUNT=<n> in config.mk.
 */

#ifndef _TARGET_UART_H_
#define _TARGET_UART_H_

#include "driver/uart.h"
#include "globals.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_task_wdt.h"

/* -----------------------------------------------------------------------
 * Port count
 * ----------------------------------------------------------------------- */

#ifndef EHS_TARGET_UART_COUNT
#define EHS_TARGET_UART_COUNT 1
#endif
#define UART_COUNT EHS_TARGET_UART_COUNT

extern ehs_uint32 gUARTBaudRate[EHS_TARGET_UART_COUNT];
extern ehs_uint8  gUARTStopBits[EHS_TARGET_UART_COUNT];
extern ehs_uint8  gUARTParity[EHS_TARGET_UART_COUNT];
extern ehs_uint8  gUARTHWCTRL[EHS_TARGET_UART_COUNT];
extern ehs_uint8  gUARTLength[EHS_TARGET_UART_COUNT];

/* -----------------------------------------------------------------------
 * Buffer / queue sizes
 * ----------------------------------------------------------------------- */

#define UART_TX_BUFFER_SIZE 1024
#define UART_RX_BUFFER_SIZE 1024
#define UART_QUEUE_SIZE     20

/* -----------------------------------------------------------------------
 * Pin macros — expand TARGET_UART_PIN_TX_PORT0 etc. from config.mk
 * ----------------------------------------------------------------------- */

#define TARGET_UART_PIN_RX_PORT(x)   TARGET_UART_PIN_RX_PORT_(x)
#define TARGET_UART_PIN_RX_PORT_(x)  TARGET_UART_PIN_RX_PORT ## x
#define TARGET_UART_PIN_TX_PORT(x)   TARGET_UART_PIN_TX_PORT_(x)
#define TARGET_UART_PIN_TX_PORT_(x)  TARGET_UART_PIN_TX_PORT ## x
#define TARGET_UART_PIN_RTS_PORT(x)  TARGET_UART_PIN_RTS_PORT_(x)
#define TARGET_UART_PIN_RTS_PORT_(x) TARGET_UART_PIN_RTS_PORT ## x
#define TARGET_UART_PIN_CTS_PORT(x)  TARGET_UART_PIN_CTS_PORT_(x)
#define TARGET_UART_PIN_CTS_PORT_(x) TARGET_UART_PIN_CTS_PORT ## x

#ifndef TARGET_UART_PIN_TX_PORT0
#define TARGET_UART_PIN_TX_PORT0 -1
#endif
#ifndef TARGET_UART_PIN_RX_PORT0
#define TARGET_UART_PIN_RX_PORT0 -1
#endif
#ifndef TARGET_UART_PIN_RTS_PORT0
#define TARGET_UART_PIN_RTS_PORT0 -1
#endif
#ifndef TARGET_UART_PIN_CTS_PORT0
#define TARGET_UART_PIN_CTS_PORT0 -1
#endif

#ifndef TARGET_UART_PIN_TX_PORT1
#define TARGET_UART_PIN_TX_PORT1 -1
#endif
#ifndef TARGET_UART_PIN_RX_PORT1
#define TARGET_UART_PIN_RX_PORT1 -1
#endif
#ifndef TARGET_UART_PIN_RTS_PORT1
#define TARGET_UART_PIN_RTS_PORT1 -1
#endif
#ifndef TARGET_UART_PIN_CTS_PORT1
#define TARGET_UART_PIN_CTS_PORT1 -1
#endif

#ifndef TARGET_UART_PIN_TX_PORT2
#define TARGET_UART_PIN_TX_PORT2 -1
#endif
#ifndef TARGET_UART_PIN_RX_PORT2
#define TARGET_UART_PIN_RX_PORT2 -1
#endif
#ifndef TARGET_UART_PIN_RTS_PORT2
#define TARGET_UART_PIN_RTS_PORT2 -1
#endif
#ifndef TARGET_UART_PIN_CTS_PORT2
#define TARGET_UART_PIN_CTS_PORT2 -1
#endif

#ifndef TARGET_UART_PIN_TX_PORT3
#define TARGET_UART_PIN_TX_PORT3 -1
#endif
#ifndef TARGET_UART_PIN_RX_PORT3
#define TARGET_UART_PIN_RX_PORT3 -1
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
    ehs_bool     configured;
    ehs_bool     inUse;
    TaskHandle_t intrCtxObj;
    QueueHandle_t intrObj;
    ehs_bool     sending;
    ehs_char     data_to_send[250];
    ehs_uint8    len_data_to_send;
    ehs_char     data_received[250];
    ehs_uint8    len_data_received;
} TgtUART_t;

static const TgtUART_t TgtUART_t_default = {
    .configured = EHS_FALSE,
    .inUse      = EHS_FALSE,
    .intrCtxObj = NULL,
    .intrObj    = NULL
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
 * IDF parameter translation tables
 * ----------------------------------------------------------------------- */

/* Maximum UART Baudrate for this platform */
#define UART_BAUDRATE_MAX UART_BITRATE_MAX

typedef void (*uart_cb_func_t)(char *payload, int length);

#if UART_COUNT == 1
static uart_cb_func_t UART_CALLBACK_FUNCTIONS[UART_COUNT] = {NULL};
#elif UART_COUNT == 2
static uart_cb_func_t UART_CALLBACK_FUNCTIONS[UART_COUNT] = {NULL, NULL};
#elif UART_COUNT == 3
static uart_cb_func_t UART_CALLBACK_FUNCTIONS[UART_COUNT] = {NULL, NULL, NULL};
#elif UART_COUNT >= 4
static uart_cb_func_t UART_CALLBACK_FUNCTIONS[UART_COUNT] = {NULL, NULL, NULL, NULL};
#endif

#define UART_WORD_LENGTH_COUNT 9
/* Index by databits value (0–8); entries 0-4 unused */
static int UART_WORD_LENGTH[UART_WORD_LENGTH_COUNT] = {
    0, 0, 0, 0, 0,
    UART_DATA_5_BITS,
    UART_DATA_6_BITS,
    UART_DATA_7_BITS,
    UART_DATA_8_BITS
};

#define UART_STOP_BITS_COUNT 3
/* 0→1, 1→1.5, 2→2 */
static int UART_STOP_BITS[UART_STOP_BITS_COUNT] = {
    UART_STOP_BITS_1,
    UART_STOP_BITS_1_5,
    UART_STOP_BITS_2
};

#define UART_PARITY_COUNT 3
/* 0=none, 1=odd, 2=even */
static int gEhsUART_PARITY[UART_PARITY_COUNT] = {
    UART_PARITY_DISABLE,
    UART_PARITY_ODD,
    UART_PARITY_EVEN
};

#define UART_HW_FLOWCTRL_COUNT 4
/* 0=none, 1=RTS, 2=CTS, 3=RTS+CTS */
static int UART_HW_FLOWCTRL[UART_HW_FLOWCTRL_COUNT] = {
    UART_HW_FLOWCTRL_DISABLE,
    UART_HW_FLOWCTRL_RTS,
    UART_HW_FLOWCTRL_CTS,
    UART_HW_FLOWCTRL_CTS_RTS
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
    TgtUART_INVALID_CALLBACK     = -17,
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

/** COM port numbers are a Windows concept; no-op on ESP32. */
int TgtUart_SetComPort(int UART_num, int com_port_number);

/** Device paths are a Linux concept; no-op on ESP32. */
int TgtUart_SetDevicePath(int UART_num, const char *path);

/*
 * Feed the watchdog timer / yield to other RTOS tasks.
 * Used in the LoRaWAN send-wait-until-acknowledgement loop.
 */
#define TgtUART_WDT_feed() vTaskDelay(10)

void Common_UART_onReceive(char *recv_msg, int length);
void Common_UART_onSendComplete(int retCode);

#endif /* _TARGET_UART_H_ */
