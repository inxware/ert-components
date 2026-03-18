#ifndef _TARGET_UART_H_
#define _TARGET_UART_H_

#ifndef EHS_TARGET_UART_COUNT 
#define EHS_TARGET_UART_COUNT 1
#endif//EHS_TARGET_UART_COUNT 
#define UART_COUNT EHS_TARGET_UART_COUNT//UART_NUM_MAX
extern ehs_uint32 gUARTBaudRate[EHS_TARGET_UART_COUNT];
extern ehs_uint8 gUARTStopBits[EHS_TARGET_UART_COUNT];
extern ehs_uint8 gUARTParity[EHS_TARGET_UART_COUNT];
extern ehs_uint8 gUARTHWCTRL[EHS_TARGET_UART_COUNT];
extern ehs_uint8 gUARTLength[EHS_TARGET_UART_COUNT];

#define UART_TX_BUFFER_SIZE 1024
#define UART_RX_BUFFER_SIZE 1024
#define UART_QUEUE_SIZE 20

#define TARGET_UART_PIN_RX_PORT(x) TARGET_UART_PIN_RX_PORT_(x)
#define TARGET_UART_PIN_RX_PORT_(x) TARGET_UART_PIN_RX_PORT ## x
#define TARGET_UART_PIN_TX_PORT(x) TARGET_UART_PIN_TX_PORT_(x)
#define TARGET_UART_PIN_TX_PORT_(x) TARGET_UART_PIN_TX_PORT ## x
#define TARGET_UART_PIN_RTS_PORT(x) TARGET_UART_PIN_RTS_PORT_(x)
#define TARGET_UART_PIN_RTS_PORT_(x) TARGET_UART_PIN_RTS_PORT ## x
#define TARGET_UART_PIN_CTS_PORT(x) TARGET_UART_PIN_CTS_PORT_(x)
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

typedef struct {
    // Whether the UART is configured
    ehs_bool configured;
    // Whether the specific UART channel is currently used
    ehs_bool inUse;
    // UART interrupt context to be used to manage the interrupt (i.e. Destroy)
    void *intrCtxObj;
    // Object to be used for interrupt purpose
    void *intrObj;
    // Status of whether the UART interface is sending the data
    ehs_bool sending;
    // The data to be sent;
    ehs_char data_to_send[250];
    // The length of the data to be sent
    ehs_uint8 len_data_to_send;
    // Received data
    ehs_char data_received[250];
    // The length of the received data
    ehs_uint8 len_data_received;
} TgtUART_t;
static const TgtUART_t TgtUART_t_default = {
    .configured = EHS_FALSE,
    .inUse = EHS_FALSE,
    .intrCtxObj = NULL,
    .intrObj = NULL
};
static inline TgtUART_t TgtUART_getTgtUART_t_default() {
    return TgtUART_t_default;
}

enum UART_State {
    Stopped = 0,
    Running,
    Sending,
    Received
};

/* Maximum UART Baudrate for this platform */
#define UART_BAUDRATE_MAX UART_BITRATE_MAX

typedef void (*uart_cb_func_t)(char *payload, int length) ;
#if UART_COUNT == 1
static uart_cb_func_t UART_CALLBACK_FUNCTIONS[UART_COUNT] = {NULL};
#elif UART_COUNT == 2
static uart_cb_func_t UART_CALLBACK_FUNCTIONS[UART_COUNT] = {NULL, NULL};
#elif UART_COUNT == 3
static uart_cb_func_t UART_CALLBACK_FUNCTIONS[UART_COUNT] = {NULL, NULL, NULL};
#endif

#define UART_WORD_LENGTH_COUNT 9
/*
 * data_length:
 * 5 -> 5 bits
 * 6 -> 6 bits
 * 7 -> 7 bits
 * 8 -> 8 bits
 *
 * */
static int UART_WORD_LENGTH[UART_WORD_LENGTH_COUNT] = {
    0,0,0,0,0,5, 6, 7, 8
};

#define UART_STOP_BITS_COUNT 3
/*
 * stop bits:
 * 0 -> 1 stop bits
 * 1 -> 1.5 stop bits
 * 2 -> 2 stop bits
 *
 * */
static int UART_STOP_BITS[UART_STOP_BITS_COUNT] = {
    0, 1, 2
};

#define UART_PARITY_COUNT 3
/*
 * parity config:
 * 0 -> disable parity check
 * 1 -> odd parity bit check
 * 2 -> even parity bit check
 *
 * */
static int gEhsUART_PARITY[UART_PARITY_COUNT] = {
    0,1,2
};

#define UART_HW_FLOWCTRL_COUNT 4
/*
 * hardware flow control:
 * 0 -> disable hardware flow control
 * 1 -> enable RX flow control (Ready To Send)
 * 2 -> enable TX flow control (Clear To Send)
 * 3 -> enable full flow control (Ready To Send & Clear To Send)
 *
 * */
static int UART_HW_FLOWCTRL[UART_PARITY_COUNT] = {
    0,1,2
};

/*
 * All target UART functions should return error code for error handling
 *
 * */
enum TgtUART_Return_t {
    TgtUART_OK = 0,
    TgtUART_INEXIST = -1,
    TgtUART_INUSE = -2,
    TgtUART_NOTINUSE = -3,
    TgtUART_NOTCONFIGURED = -4,
    TgtUART_BAUDRATEERROR = -5,
    TgtUART_CONFIG_ERROR = -6,
    TgtUART_ERROR = -7,
    TgtUART_LENGTH_NOT_MATCH = -8,
    TgtUART_INTERRUPT_EXISTED = -9,
    TgtUART_INTERRUPT_NO_CB = -10,
    TgtUART_INTERRUPT_REG_ERROR = -11,
    TgtUART_INTERRUPT_DEL_ERROR = -12,
    TgtUART_DATA_ERROR = -13,
    TgtUART_BUFFER_FULL = -14,
    TgtUART_EVENT_UNDEFINED = -15,
    TgtUART_HEAP_INSUFFICIENT = -16,
    TgtUART_INVALID_CALLBACK = -18,
    TgtUART_ESPERROR = -100
};

int TgtUart_Stage0( int UART_num );

int TgtUart_Start(int UART_num, int tx_io, int rx_io, int rts_io, int cts_io, int baudrate, int databits, int parity, int stop_bits, int flow_control);

int TgtUart_Config(int UART_num, int baudrate, int databits, int parity,
                   int stop_bits, int flow_control);

int TgtUart_RS485Setup(int UART_num);

//int TgtUart_Start(int UART_num);

int TgtUart_Stop(int UART_num);

int TgtUart_Send(int UART_num, char *payload, unsigned int length);

void TgtUART_SendThread(int UART_num, char *payload, unsigned int length);

int TgtUART_SendInThread(int UART_num);

int TgtUART_RecvPoll(int UART_num, char *payload, unsigned int length);

int TgtUART_Intr_register(int UART_num, uart_cb_func_t cb_func);

int TgtUart_SetComPort(int UART_num, int com_port_number);

int TgtUart_SetDevicePath(int UART_num, const char *path);


// /*
//  * Register the UART event interrupt data receive callback.
//  * The callback function is:
//  *  void cb_func (char * payload, int payload_length);
//  * 
//  * Noting that both arguments are inputs
//  *
//  * */
// inline int TgtUART_register_callback(int UART_num, uart_cb_func_t func) {
//     if (UART_num >= UART_COUNT) return TgtUART_INEXIST;
//     UART_CALLBACK_FUNCTIONS[UART_num] = func;
//     return TgtUART_OK;
// }

/*
 * Feed the watchdog timer or give time slot to the other tasks in the RTOS schedule.
 * Mainly used within the AT commands' send-wait-until-acknowledgement loop.
 *
 * */
#define TgtUART_WDT_feed()

void Common_UART_onReceive(char *recv_msg, int length);
void Common_UART_onSendComplete(int retCode);

#endif
