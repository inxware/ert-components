#ifndef _LORAWAN_H_
#define _LORAWAN_H_
#include "globals.h"

#ifndef LORA_UART_PORT
#define LORA_UART_PORT 0
#endif

/* UART pin assignments for the LoRaWAN modem.
 * On Linux/sferalabs these are ignored by TgtUart_Start.
 * On ESP32 they must be set to the physical GPIO numbers in target_config.h.
 * Default -1 means "not assigned / ignored". */
#ifndef LORA_UART_TX_PIN
#define LORA_UART_TX_PIN  -1
#endif
#ifndef LORA_UART_RX_PIN
#define LORA_UART_RX_PIN  -1
#endif
#ifndef LORA_UART_RTS_PIN
#define LORA_UART_RTS_PIN -1
#endif
#ifndef LORA_UART_CTS_PIN
#define LORA_UART_CTS_PIN -1
#endif

#define EHS_LORAWAN_ID_STRLEN 16
#define EHS_LORAWAN_KEY_STRLEN 32

#ifndef LW_RX_BUFFER_SIZE
#define LW_RX_BUFFER_SIZE 255
#endif//LW_RX_BUFFER_SIZE

typedef enum {
    E_EHS_LWTARGET_WIO_E5 = 0,
} e_ehs_lw_target_t;

typedef enum {
    E_LWCLASS_A = 0,
    E_LWCLASS_B,
    E_LWCLASS_C,
    E_LWCLASS_MAXVALUE
} e_ehs_lw_class_t;

/* START - LoRaWAN enum - START */
typedef enum {
    E_LORAWAN_API_CONNECT = 0,
    E_LORAWAN_API_SEND_MSG,
    E_LORAWAN_API_RESET,
    E_LORAWAN_API_GET_SYSDATA,
    E_LORAWAN_API_SET_DATARATE,
    E_LORAWAN_API_GET_PAYLOADLENGTH,
    E_LORAWAN_API_DISABLE,
    E_LORAWAN_API_SET_CLASS,
    E_LORAWAN_API_SET_TXPOWER,
    E_LORAWAN_API_LINK_CHECK,
    // Normal command maximum number
    E_LORAWAN_API__MAX_VALUE,
    // Status
    E_LORAWAN_API__NOT_INTIALISED,
    E_LORAWAN_API__COMPLETE,
    E_LORAWAN_API__IDLE,
} e_ehs_lorawan_api_cmd_t;
/* END - LoRaWAN enum - END */

typedef enum {
    E_LWREGION_EU868 = 0,
    E_LWREGION_US915,
    E_LWREGION_CN779,
    E_LWREGION_EU433,
    E_LWREGION_AU915,
    E_LWREGION_CN470,
    E_LWREGION_AS923,
    E_LWREGION_KR920,
    E_LWREGION_IN865,
    E_LWREGION_RU864,
    E_LWREGION_STE920,
    E_LWREGION_MAXVALUE
} e_ehs_lw_region_t;

typedef enum {
    // OK
    E_LWAPIERRNO_OK = 0,
    // Separator Should NOT use
    _E_LWAPIERRNO_SEPARATOR_ = 100,
    // LoRaWAN Error
    E_LWAPIERRNO_INVALID_PARAMETERS,
    E_LWAPIERRNO_NOT_INITIALISED,
    E_LWAPIERRNO_ALREADY_INITIALISED,
    E_LWAPIERRNO_INTERNAL_ERROR,
    E_LWAPIERRNO_GENERIC_ERROR,
    E_LWAPIERRNO_BUSY,
    E_LWAPIERRNO_JOIN_FAILED,
    E_LWAPIERRNO_ALREADY_JOINED,
    E_LWAPIERRNO_MODEM_BUSY,
    E_LWAPIERRNO_INVALID_REGION,
    E_LWAPIERRNO_NOT_JOINED,
    E_LWAPIERRNO_ALL_CHANNEL_OCCUPIED,
    E_LWAPIERRNO_NO_BAND_AVAILABLE,
    E_LWAPIERRNO_DR_ERROR,
    E_LWAPIERRNO_PAYLOAD_TOO_LONG,
    E_LWAPIERRNO_INVALID_CLASS,
    E_LWAPIERRNO_INVALID_ID,
    E_LWAPIERRNO_INTERNAL_INVALID_CTRLSEQ,
} ehs_lorawan_api_errno_t;

typedef struct {
    ehs_lorawan_api_errno_t error_ret[E_LORAWAN_API__MAX_VALUE];
    ehs_sint32 rssi;
    ehs_float snr;
    ehs_sint32 rxwin;
    ehs_uint8 link_margin;
    ehs_uint8 gateway_count;
    ehs_uint8 fport;
    ehs_uint8 dr;
    ehs_bool adr;
    ehs_sint32 tx_power;   /* current TX power index */
    ehs_sint32 current_dr; /* current DR (updated by ADR or set_datarate) */
    /* Output pointer to write max payload length */
    ehs_sint32 *p_length_out;
} ehs_lorawan_api_data_t;

extern ehs_lorawan_api_data_t gEhsLoraApiData;

// rx_buffer
extern char gEhsLorawanRxBuffer[LW_RX_BUFFER_SIZE];

ehs_lorawan_api_errno_t LoRaWAN_init( e_ehs_lw_target_t target, ehs_sint32 com_port );

ehs_lorawan_api_errno_t LoRaWAN_deinit( void );

ehs_lorawan_api_errno_t LoRaWAN_connect(char *AppKey, char *AppEui, ehs_bool mode, char *DevAddr_ABP, char *AppSKey, char *NwkSKey, ehs_sint32 REPT, ehs_sint32 RETRY, e_ehs_lw_region_t region, ehs_bool ADR, ehs_sint32 DR, ehs_sint32 autoJoin, char *DevAddr_OUT, e_ehs_lw_class_t class_type, ehs_sint32 subband, ehs_float rxwin2_freq, ehs_sint32 rxwin2_dr, ehs_sint32 tx_power);

ehs_lorawan_api_errno_t LoRaWAN_send_msg(char *payload, int fport, ehs_bool confirmed);

ehs_lorawan_api_errno_t LoRaWAN_reset( void );

ehs_lorawan_api_errno_t LoRaWAN_get_sysData(char *sysData_out, char *DevAddr_out);

ehs_lorawan_api_errno_t LoRaWAN_set_datarate(ehs_sint32 dr);

ehs_lorawan_api_errno_t LoRaWAN_get_payloadLength(ehs_sint32 *length_out);

ehs_lorawan_api_errno_t LoRaWAN_disable( void );

ehs_lorawan_api_errno_t LoRaWAN_set_class(e_ehs_lw_class_t class_type);

ehs_lorawan_api_errno_t LoRaWAN_set_txpower(ehs_sint32 tx_power);

ehs_lorawan_api_errno_t LoRaWAN_link_check( void );

void Common_LoRaWAN_onReceive(char *recv_msg, ehs_bool has_message);
void Common_LoRaWAN_FBCBs(e_ehs_lorawan_api_cmd_t cmd);

#endif
