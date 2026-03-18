#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "lorawan-wio_e5.h"
#include "lorawan.h"
#include "target_uart.h"
#include "lorawan_helper.h"

/**
 * @brief Notes
 * 1. The maximum received payload length is 115 (in bytes as two digit into one).
 *    Example received message:
 *       +MSG: PORT: 100; RX: "00112233445566778899001122334455667788990011223344556677889900112233445566778899001122334455667788990011223344556677889900112233445566778899001122334455667788990011223344556677889900112233445566778899001122334455667788990011223344"
 *    1a. Therefore the maximum possible length of a single AT message return is 253+1(NULL END)
 *
 */

//#define EHS_LORAWAN_DEBUG
#ifdef EHS_LORAWAN_DEBUG
#define ehs_lorawan_debug(...) printf(__VA_ARGS__)
#else
#define ehs_lorawan_debug(...)
#endif

#define RECV_TIMEOUT_S 10

enum AT_COMMAND_t {
    AT_ID = 0,
    AT_KEY,
    AT_MODE,
    AT_CLASS,
    AT_DR,
    AT_REPT,
    AT_RETRY,
    AT_JOIN,
    /* Start DON'T MODIFY THIS BLOCK */
    AT_MSGHEX,
    AT_CMSGHEX,
    AT_MSG,
    AT_CMSG,
    /* Ended DON'T MODIFY THIS BLOCK */
    AT_LW,
    AT_RESET,
    AT_VDD,
    AT_TEMP,
    AT_RTC,
    AT_VER,
    AT_PORT,
    AT_ADR,
    AT_POWER,
    AT_RXWIN2,
    AT_MAX_VALUE
};
static const char AT_COMMAND_STRING[AT_MAX_VALUE][9] = {
    "+ID",
    "+KEY",
    "+MODE",
    "+CLASS",
    "+DR",
    "+REPT",
    "+RETRY",
    "+JOIN",
    /* Start DON'T MODIFY THIS BLOCK */
    "+MSGHEX",
    "+CMSGHEX",
    "+MSG",
    "+CMSG",
    /* Ended DON'T MODIFY THIS BLOCK */
    "+LW",
    "+RESET",
    "+VDD",
    "+TEMP",
    "+RTC",
    "+VER",
    "+PORT",
    "+ADR",
    "+POWER",
    "+RXWIN2"
};

enum LORAWAN_WIO_E5_LW_CMD_t {
    LW_WIO_E5_LW_CMD_CDR = 0,
    LW_WIO_E5_LW_CMD_ULDL,
    LW_WIO_E5_LW_CMD_DC,
    LW_WIO_E5_LW_CMD_NET,
    LW_WIO_E5_LW_CMD_MC,
    LW_WIO_E5_LW_CMD_THLD,
    LW_WIO_E5_LW_CMD_BAT,
    LW_WIO_E5_LW_CMD_TPS,
    LW_WIO_E5_LW_CMD_SCR,
    LW_WIO_E5_LW_CMD_JDC,
    LW_WIO_E5_LW_CMD_CT,
    LW_WIO_E5_LW_CMD_LEN,
    LW_WIO_E5_LW_CMD_VER,
    LW_WIO_E5_LW_CMD_DTR,
    LW_WIO_E5_LW_CMD_LCR,
    LW_WIO_E5_LW_CMD_LDRO,
    LW_WIO_E5_LW_CMD_DCMRX,
    LW_WIO_E5_LW_CMD_DUMRX,
    LW_WIO_E5_LW_CMD_AFPACK,
    LW_WIO_E5_LW_CMD_CHRB,
    LW_WIO_E5_LW_CMD_WDT,
    LW_WIO_E5_LW_CMD_MAX_VALUE,
};

static const char LW_CMD_STRING[LW_WIO_E5_LW_CMD_MAX_VALUE][7] = {
    "CDR",
    "ULDL",
    "DC",
    "NET",
    "MC",
    "THLD",
    "BAT",
    "TPS",
    "SCR",
    "JDC",
    "CT",
    "LEN",
    "VER",
    "DTR",
    "LCR",
    "LDRO",
    "DCMRX",
    "DUMRX",
    "AFPACK",
    "CHRB",
    "WDT"
};

static const char LW_REGION_STRING[E_LWREGION_MAXVALUE][7] = {
    "EU868",
    "US915",
    "CN779",
    "EU433",
    "AU915",
    "CN470",
    "AS923",
    "KR920",
    "IN865",
    "RU864",
    "STE920"
};

typedef struct {
    char *rtc;
    char *temp;
    char *vdd;
    char *DevEui;
} lw_sysdata_t;

typedef struct {
    char DevAddr[25];
    char DevEui[25];
    char AppEui[25];
} lw_iddata_t ;

typedef struct {
    int fport;
    ehs_bool adr;
    int dr;
    e_ehs_lw_region_t region;
} lw_localconfig_t;

static lw_iddata_t gLW_idData = {
    .DevAddr = "",
    .DevEui = "",
    .AppEui = ""
};

static lw_localconfig_t gLW_localConfig = {
    .fport = -1, // set it to -1 so that the first send command will always set it
    .adr = EHS_FALSE,
    .dr = -1,
    .region = E_LWREGION_MAXVALUE,
};

// LoRaWAN message start indicates the MSG is sent
static ehs_bool loraMsgStart[4] = {EHS_FALSE};
// Command done
static volatile ehs_bool loraDone[AT_MAX_VALUE] = {EHS_FALSE};
// Command Error
static volatile ehs_lorawan_api_errno_t loraError[AT_MAX_VALUE] = {E_LWAPIERRNO_OK};
// AT command In process
static ehs_bool loraInProcess[AT_MAX_VALUE] = {EHS_FALSE};

// Connecting
static ehs_bool loraConnecting = EHS_FALSE;

// send buffer size
#define LW_SEND_BUFFER_SIZE 128

// buffer
#define LW_BUFFER_SIZE 1024
static char buffer[LW_BUFFER_SIZE] = {0};
static int buffer_index = 0;

// misc buffer
#define LW_MISC_BUFFER_SIZE 70
static char lw_misc_buffer[LW_MISC_BUFFER_SIZE] = {0};

#define _COPY_INTO_SEND_BUFFER(x, ...) do { \
    snprintf(x, LW_SEND_BUFFER_SIZE - 1, __VA_ARGS__); \
} while (0)

static void LoRaWAN_wioe5_onUart(char *payload, int length);

/*
 * Send the UART message until "DONE" or "ERROR" received.
 * Return:
 * 	0 if "DONE"
 * 	any positive number if "ERROR". Check ehs_lorawan_api_errno_t enum definition
 * 	< 0 if UART error
 *
 * */
static ehs_lorawan_api_errno_t sendWaitUntilComplete(char *UART_payload, int at_command)
{
    ehs_lorawan_debug("payload: %s, at_command: %d\n", UART_payload, at_command);
    assert(at_command < AT_MAX_VALUE);
    loraInProcess[at_command] = EHS_TRUE;
    int ret = 0;
    ret = TgtUart_Send(LORA_UART_PORT, UART_payload, strlen(UART_payload));
    while (ret == 0) {
        if (loraDone[at_command] == EHS_TRUE) {
            loraDone[at_command] = EHS_FALSE;
            ret = 0;
            break;
        }
        if (loraError[at_command] != E_LWAPIERRNO_OK) {
            ret = loraError[at_command];
            loraError[at_command] = E_LWAPIERRNO_OK;
            break;
        }
        TgtUART_WDT_feed();
    }
    loraInProcess[at_command] = EHS_FALSE;
    return ret;
}

/*
 * Init the communication interface with LoRaWAN module
 *
 * */
ehs_lorawan_api_errno_t LoRaWAN_wioe5_init(ehs_sint32 com_port)
{
    ehs_lorawan_api_errno_t ret = TgtUART_OK;
    if ((ret = TgtUart_Stage0(LORA_UART_PORT)) != TgtUART_OK) return ret;
    if (com_port > 0)
        TgtUart_SetComPort(LORA_UART_PORT, com_port);
    // UART default pins, baudrate 9600, 8 data bits, 1 stop bit, no parity check, no flow control
    if ((ret = TgtUart_Start(LORA_UART_PORT,
                             TARGET_UART_PIN_TX_PORT(LORA_UART_PORT),
                             TARGET_UART_PIN_RX_PORT(LORA_UART_PORT),
                             TARGET_UART_PIN_RTS_PORT(LORA_UART_PORT),
                             TARGET_UART_PIN_CTS_PORT(LORA_UART_PORT),
                             9600, 8, 0, 0, 0)) != TgtUART_OK) {ehs_lorawan_debug("Uart Start ret: %d\n", ret); return ret;}
//	if ((ret = TgtUart_Start(LORA_UART_PORT)) != TgtUART_OK) { ehs_lorawan_debug("UART Start ret: %d\n", ret); return ret; }
    if ((ret = TgtUART_Intr_register(LORA_UART_PORT, &LoRaWAN_wioe5_onUart)) != TgtUART_OK) {ehs_lorawan_debug("Uart Intr Reg ret: %d\n", ret); return ret;}

    return ret;
}

/*
 * De-init (destroy) the UART interface
 *
 * */
ehs_lorawan_api_errno_t LoRaWAN_wioe5_deinit()
{
    ehs_lorawan_api_errno_t ret = TgtUART_OK;
    ret = TgtUart_Stop(LORA_UART_PORT);
    return ret;
}

/*
 * Initiate the connection to the LoRaWAN server. Pass the resulting DevAddr to DevAddr_OUT.
 *  Note that the DevAddr_OUT should be allocated with sufficient memory.
 *
 * */
ehs_lorawan_api_errno_t LoRaWAN_wioe5_connect(char *AppKey, char *AppEui, ehs_bool mode, char *DevAddr_ABP, char *AppSKey, char *NwkSKey, ehs_sint32 REPT, ehs_sint32 RETRY, e_ehs_lw_region_t region, ehs_bool ADR, ehs_sint32 DR, ehs_sint32 autoJoin, char *DevAddr_OUT, e_ehs_lw_class_t class_type, ehs_sint32 subband, ehs_float rxwin2_freq, ehs_sint32 rxwin2_dr, ehs_sint32 tx_power)
{
    loraConnecting = EHS_TRUE;
    ehs_lorawan_api_errno_t ret = E_LWAPIERRNO_OK;
    char send_buffer[LW_SEND_BUFFER_SIZE];

    // Set essential parameters according to the mode
    if (mode == EHS_FALSE)
    {
        // mode=0 --> OTAA method
        _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=AppKey,\"%s\"\r\n", AT_COMMAND_STRING[AT_KEY], AppKey);
        if ((ret = sendWaitUntilComplete(send_buffer, AT_KEY)) != 0) return ret;
        _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=AppEui,\"%s\"\r\n", AT_COMMAND_STRING[AT_ID], AppEui);
        if ((ret = sendWaitUntilComplete(send_buffer, AT_ID)) != 0) return ret;
        _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=LWOTAA\r\n", AT_COMMAND_STRING[AT_MODE]);
        if ((ret = sendWaitUntilComplete(send_buffer, AT_MODE)) != 0) return ret;
    }
    else if (mode == EHS_TRUE) {
        // mode=1 --> ABP method
        _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=DevAddr,\"%s\"\r\n", AT_COMMAND_STRING[AT_ID], DevAddr_ABP);
        if ((ret = sendWaitUntilComplete(send_buffer, AT_ID)) != 0) return ret;
        _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=AppSKey,\"%s\"\r\n", AT_COMMAND_STRING[AT_KEY], AppSKey);
        if ((ret = sendWaitUntilComplete(send_buffer, AT_KEY)) != 0) return ret;
        _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=NwkSKey,\"%s\"\r\n", AT_COMMAND_STRING[AT_KEY], NwkSKey);
        if ((ret = sendWaitUntilComplete(send_buffer, AT_KEY)) != 0) return ret;
        _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=LWABP\r\n", AT_COMMAND_STRING[AT_MODE]);
        if ((ret = sendWaitUntilComplete(send_buffer, AT_MODE)) != 0) return ret;
    }
    gLW_localConfig.region = region;
    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=%s\r\n", AT_COMMAND_STRING[AT_DR], LW_REGION_STRING[region]);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_DR)) != 0) return ret;
    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=%d\r\n", AT_COMMAND_STRING[AT_REPT], REPT);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_REPT)) != 0) return ret;
    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=%d\r\n", AT_COMMAND_STRING[AT_RETRY], RETRY);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_RETRY)) != 0) return ret;
    /* Set device class (A, B or C) */
    {
        const char class_char = (class_type == E_LWCLASS_B) ? 'B' : (class_type == E_LWCLASS_C) ? 'C' : 'A';
        _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=%c\r\n", AT_COMMAND_STRING[AT_CLASS], class_char);
        if ((ret = sendWaitUntilComplete(send_buffer, AT_CLASS)) != 0) return ret;
    }
    autoJoin = autoJoin < 0 ? 0 : autoJoin > 86400 ? 86400 : autoJoin;
    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=AUTO,%d,0,0\r\n", AT_COMMAND_STRING[AT_JOIN], autoJoin);
    while ((ret = sendWaitUntilComplete(send_buffer, AT_JOIN)) != 0);

    // The actual joining process is completed
    loraConnecting = EHS_FALSE;

    // LoRaWAN MAC command to retreive RTC data
    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=%s\r\n", AT_COMMAND_STRING[AT_LW], LW_CMD_STRING[LW_WIO_E5_LW_CMD_DTR]);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_LW)) != 0) return ret;
    // LoRaWAN MAC command to get the link status
    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=%s\r\n", AT_COMMAND_STRING[AT_LW], LW_CMD_STRING[LW_WIO_E5_LW_CMD_LCR]);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_LW)) != 0) return ret;
    // Send the empty message with the MAC commands
    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s\r\n", AT_COMMAND_STRING[AT_MSGHEX]);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_MSGHEX)) != 0) return ret;

    // Get DevAddr
    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=DevAddr\r\n", AT_COMMAND_STRING[AT_ID]);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_ID)) != 0) return ret;
    ret = strncmp(strcpy(DevAddr_OUT, gLW_idData.DevAddr), gLW_idData.DevAddr, strlen(gLW_idData.DevAddr) + 1);

    // Data rate setting
    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=%s\r\n", AT_COMMAND_STRING[AT_ADR], ADR ? "ON" : "OFF");
    if ((ret = sendWaitUntilComplete(send_buffer, AT_ADR)) != 0) return ret;
    // Only set data rate if ADR is off
    if (ADR == EHS_FALSE)
    {
        _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=DR%d\r\n", AT_COMMAND_STRING[AT_DR], DR);
        if ((ret = sendWaitUntilComplete(send_buffer, AT_DR)) != 0) return ret;
    }

    /* Set TX power (0 = max regional EIRP, higher = lower power in ~2 dB steps) */
    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=%d\r\n", AT_COMMAND_STRING[AT_POWER], tx_power);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_POWER)) != 0) return ret;

    /* Set RX window 2 frequency and data rate */
    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=%.3f,DR%d\r\n", AT_COMMAND_STRING[AT_RXWIN2], rxwin2_freq, rxwin2_dr);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_RXWIN2)) != 0) return ret;

    /* Sub-band selection for US915/AU915/CN470 (0 = all channels / not applicable).
     * TODO: implement AT+LW=BAND,<n> for sub-band selection once firmware support is confirmed.
     * For now, log a warning if subband != 0. */
    if (subband != 0) {
        ehs_lorawan_debug("[LoRaWAN_wioe5_connect] WARNING: subband=%d requested but not yet implemented\n", subband);
    }

    return ret;
}

/*
 * Send message to the server, either unconfirmed or confirmed
 *
 * */
ehs_lorawan_api_errno_t LoRaWAN_wioe5_send_msg(char *payload, ehs_uint8 fport, ehs_bool confirmed)
{
    ehs_lorawan_api_errno_t ret = E_LWAPIERRNO_OK;
    char send_buffer[LW_SEND_BUFFER_SIZE];
    if (fport != gLW_localConfig.fport)
    {
        _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=%d\r\n", AT_COMMAND_STRING[AT_PORT], fport);
        ret = sendWaitUntilComplete(send_buffer, AT_PORT);
        if (ret != 0) return ret;
    }
    //// Append LinkCheckReq to the message (This will always reduce the available payload length by 1)
    //_COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=%s\r\n", AT_COMMAND_STRING[AT_LW], LW_CMD_STRING[LW_WIO_E5_LW_CMD_LCR]);
    //if ((ret = sendWaitUntilComplete(send_buffer, AT_LW)) != 0) return ret;
    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=\"%s\"\r\n", AT_COMMAND_STRING[confirmed == EHS_TRUE ? AT_CMSGHEX : AT_MSGHEX], payload);
    ret = sendWaitUntilComplete(send_buffer, confirmed == EHS_TRUE ? AT_CMSGHEX : AT_MSGHEX);
    return ret;
}

/*
 * LoRaWAN module software reset
 *
 * */
ehs_lorawan_api_errno_t LoRaWAN_wioe5_reset( void )
{
    char send_buffer[LW_SEND_BUFFER_SIZE];
    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s\r\n", AT_COMMAND_STRING[AT_RESET]);
    return sendWaitUntilComplete(send_buffer, AT_RESET);
}

/*
 * Get the module's RTC time, core temperature, power supply voltage and DevEui. Pass them as a JSON string into the argument.
 * Note that the passed argument should have memory allocated. If that is
 *
 * */
ehs_lorawan_api_errno_t LoRaWAN_wioe5_get_sysData(char *data, char *DevEui)
{
    ehs_lorawan_api_errno_t ret = E_LWAPIERRNO_OK;
    char send_buffer[LW_SEND_BUFFER_SIZE] = {0};
    lw_sysdata_t sysData;
    char result_data[LW_MISC_BUFFER_SIZE] = {0};

    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s\r\n", AT_COMMAND_STRING[AT_RTC]);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_RTC)) != 0) goto LoRaWAN_wio_e5_get_sysData_Return;
    sysData.rtc = (char *)malloc(strlen(lw_misc_buffer) + 1);
    memcpy(sysData.rtc, lw_misc_buffer, strlen(lw_misc_buffer) + 1);

    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s\r\n", AT_COMMAND_STRING[AT_TEMP]);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_TEMP)) != 0) goto LoRaWAN_wio_e5_get_sysData_Return;
    sysData.temp = (char *)malloc(strlen(lw_misc_buffer) + 1);
    memcpy(sysData.temp, lw_misc_buffer, strlen(lw_misc_buffer) + 1);

    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s\r\n", AT_COMMAND_STRING[AT_VDD]);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_VDD)) != 0) goto LoRaWAN_wio_e5_get_sysData_Return;
    sysData.vdd = (char *)malloc(strlen(lw_misc_buffer) + 1);
    memcpy(sysData.vdd, lw_misc_buffer, strlen(lw_misc_buffer) + 1);

    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s\r\n", AT_COMMAND_STRING[AT_ID]);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_ID)) != 0) goto LoRaWAN_wio_e5_get_sysData_Return;
    sysData.DevEui = (char *)malloc(strlen(gLW_idData.DevEui) + 1);
    memcpy(sysData.DevEui, gLW_idData.DevEui, strlen(gLW_idData.DevEui) + 1);
    //
    /* Format result into string */
    snprintf(result_data, LW_MISC_BUFFER_SIZE - 1, "{\"RTC\":\"%s\",\"Temperature\":%s,\"Voltage\":%s}",
            sysData.rtc, sysData.temp, sysData.vdd);
    if (DevEui != NULL) memcpy(DevEui, sysData.DevEui, strlen(sysData.DevEui) + 1);
    else DevEui = strdup(sysData.DevEui);
    if (data != NULL) memcpy(data, result_data, strlen(result_data) + 1);
    else data = strdup(result_data);
    ehs_lorawan_debug("LoRaWAN_wioe5_get_sysData: data: [%s] DevEui: [%s]\n", data, DevEui);
LoRaWAN_wio_e5_get_sysData_Return:
    if (sysData.rtc    != NULL) free(sysData.rtc);    sysData.rtc = NULL;
    if (sysData.temp   != NULL) free(sysData.temp);   sysData.temp = NULL;
    if (sysData.vdd    != NULL) free(sysData.vdd);    sysData.vdd = NULL;
    if (sysData.DevEui != NULL) free(sysData.DevEui); sysData.DevEui = NULL;
    return ret;
}

/**
 * @brief Set the datarate (DR) of the communication
 *
 * @param datarate The level of datarate to set
 * @return ehs_lorawan_api_errno_t
 */
ehs_lorawan_api_errno_t LoRaWAN_wioe5_set_datarate(ehs_sint32 datarate)
{
    ehs_lorawan_api_errno_t ret = E_LWAPIERRNO_OK;
    char send_buffer[LW_SEND_BUFFER_SIZE];

    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=DR%d\r\n", AT_COMMAND_STRING[AT_DR], datarate);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_DR)) != 0) return ret;
    return ret;
}

/**
 * @brief Get the length of the payload
 *
 * @param length
 * @return ehs_lorawan_api_errno_t
 */
ehs_lorawan_api_errno_t LoRaWAN_wioe5_get_payloadLength(ehs_sint32 *length_out)
{
    ehs_lorawan_api_errno_t ret = E_LWAPIERRNO_OK;
    char send_buffer[LW_SEND_BUFFER_SIZE];

    // Append LinkCheckReq to the message (This will always reduce the available payload length by 1)
    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=%s\r\n", AT_COMMAND_STRING[AT_LW], LW_CMD_STRING[LW_WIO_E5_LW_CMD_LCR]);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_LW)) != 0) return ret;
    // Get the available payload length
    gEhsLoraApiData.p_length_out = length_out;
    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=LEN\r\n", AT_COMMAND_STRING[AT_LW]);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_LW)) != 0) return ret;

    return ret;
}


/**
 * @brief Disable the LoRa connection (This target just unjoins from the network)
 *
 * @return ehs_lorawan_api_errno_t
 */
ehs_lorawan_api_errno_t LoRaWAN_wioe5_disable( void )
{
    ehs_lorawan_api_errno_t ret = E_LWAPIERRNO_OK;
    char send_buffer[LW_SEND_BUFFER_SIZE];

    // This is to make sure the region is indeed changed
    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=%s\r\n", AT_COMMAND_STRING[AT_DR], LW_REGION_STRING[gLW_localConfig.region + 1 == E_LWREGION_MAXVALUE ? gLW_localConfig.region - 1 : gLW_localConfig.region + 1]);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_DR)) != 0) return ret;
    // Now change it back.
    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=%s\r\n", AT_COMMAND_STRING[AT_DR], LW_REGION_STRING[gLW_localConfig.region]);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_DR)) != 0) return ret;

    return ret;
}

/**
 * @brief Set the LoRaWAN device class (A, B or C)
 */
ehs_lorawan_api_errno_t LoRaWAN_wioe5_set_class(e_ehs_lw_class_t class_type)
{
    char send_buffer[LW_SEND_BUFFER_SIZE];
    const char class_char = (class_type == E_LWCLASS_B) ? 'B' : (class_type == E_LWCLASS_C) ? 'C' : 'A';
    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=%c\r\n", AT_COMMAND_STRING[AT_CLASS], class_char);
    return sendWaitUntilComplete(send_buffer, AT_CLASS);
}

/**
 * @brief Set the TX power index (0 = max EIRP for region, higher = lower power)
 */
ehs_lorawan_api_errno_t LoRaWAN_wioe5_set_txpower(ehs_sint32 tx_power)
{
    char send_buffer[LW_SEND_BUFFER_SIZE];
    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=%d\r\n", AT_COMMAND_STRING[AT_POWER], tx_power);
    return sendWaitUntilComplete(send_buffer, AT_POWER);
}

/**
 * @brief Request a LoRaWAN link check (LinkCheckReq MAC command).
 *
 * The WIO-E5 does not support a standalone link-check uplink — the MAC command
 * must be piggybacked on the next data frame via AT+LW=LCR before sending.
 * This function schedules the LCR flag; the result (link_margin, gateway_count)
 * will be populated in gEhsLoraApiData on the next completed send.
 *
 * TODO: send a short empty frame immediately to force the link check uplink
 * without requiring the application to send a data frame first.
 */
ehs_lorawan_api_errno_t LoRaWAN_wioe5_link_check( void )
{
    char send_buffer[LW_SEND_BUFFER_SIZE];
    /* Schedule LinkCheckReq on next message */
    _COPY_INTO_SEND_BUFFER(send_buffer, "AT%s=%s\r\n", AT_COMMAND_STRING[AT_LW], LW_CMD_STRING[LW_WIO_E5_LW_CMD_LCR]);
    return sendWaitUntilComplete(send_buffer, AT_LW);
}

static void LoRaWAN_wioe5_onUart(char *payload, int length)
{
    ehs_lorawan_debug("[%s] {%s}\n", __func__, payload);
    assert(buffer_index + length < LW_BUFFER_SIZE);
    ehs_bool _bufferFlag = EHS_FALSE;
    int16_t split_start[20], split_length[20], split_count, split_max;
    char temp_buf[EHS_STRING_LENGTH_MAX];
    int temp_int = 0;
    uint8_t msg_type = 0;
    // if the payload is not ended by "\r\n", combine the latter one into buffer
    if (strcmp(payload + length - 2, "\r\n") != 0)
    {
        _bufferFlag = EHS_FALSE;
        memcpy(buffer + buffer_index, payload, length);
        buffer_index += length;
        goto LoRaWAN_wioe5_onUart_Return;
    } else {
        _bufferFlag = EHS_TRUE;
        memcpy(buffer + buffer_index, payload, length);
    }

    /* String command handling */
    if (strstr(buffer, AT_COMMAND_STRING[AT_JOIN]) != NULL)
    {
        /* Join request */
        if (strstr(buffer, "ERROR") != NULL)
        {
            loraError[AT_JOIN] = E_LWAPIERRNO_GENERIC_ERROR;
        } else if (strstr(buffer, "Join failed") != NULL) {
            loraError[AT_JOIN] = E_LWAPIERRNO_JOIN_FAILED;
        } else if (strstr(buffer, "Joined already")) {
            loraError[AT_JOIN] = E_LWAPIERRNO_ALREADY_JOINED;
        } else if (strstr(buffer, "LoRaWAN modem is busy") != NULL) {
            loraError[AT_JOIN] = E_LWAPIERRNO_MODEM_BUSY;
        } else {
            if (strstr(buffer, "Done") != NULL) loraDone[AT_JOIN] = EHS_TRUE;
        }
    }
    else if (strstr(buffer, AT_COMMAND_STRING[AT_KEY]) != NULL)
    {
        /* LoRaWAN KEY set */
        if (strstr(buffer, "ERROR") != NULL) loraError[AT_KEY] = E_LWAPIERRNO_GENERIC_ERROR;
        else loraDone[AT_KEY] = EHS_TRUE;
    }
    else if (strstr(buffer, AT_COMMAND_STRING[AT_ADR]) != NULL)
    {
        /* LoRaWAN ADR setting */
        if (strstr(buffer, "ERROR") != NULL) loraError[AT_ADR] = E_LWAPIERRNO_GENERIC_ERROR;
        else {
            if (strstr(buffer, "ON") != NULL)
            {
                gLW_localConfig.adr = EHS_TRUE;
                loraDone[AT_ADR] = EHS_TRUE;
            }
            else if (strstr(buffer, "OFF") != NULL)
            {
                gLW_localConfig.adr = EHS_FALSE;
                loraDone[AT_ADR] = EHS_TRUE;
            }
            else loraError[AT_ADR] = E_LWAPIERRNO_GENERIC_ERROR;
        }
    }
    else if (strstr(buffer, AT_COMMAND_STRING[AT_DR]) != NULL)
    {
        /* LoRaWAN region setup */
        if (strstr(buffer, "ERROR") != NULL) loraError[AT_DR] = E_LWAPIERRNO_GENERIC_ERROR;
        else loraDone[AT_DR] = EHS_TRUE;
    }
    else if (strstr(buffer, AT_COMMAND_STRING[AT_REPT]) != NULL)
    {
        /* LoRaWAN Repeat parameter setup */
        if (strstr(buffer, "ERROR") != NULL) loraError[AT_REPT] = E_LWAPIERRNO_GENERIC_ERROR;
        else loraDone[AT_REPT] = EHS_TRUE;
    }
    else if (strstr(buffer, AT_COMMAND_STRING[AT_RETRY]) != NULL)
    {
        /* LoRaWAN Retry parameter setup */
        if (strstr(buffer, "ERROR") != NULL) loraError[AT_RETRY] = E_LWAPIERRNO_GENERIC_ERROR;
        else loraDone[AT_RETRY] = EHS_TRUE;
    }
    else if (strstr(buffer, AT_COMMAND_STRING[AT_MODE]) != NULL)
    {
        /* LoRaWAN Mode set */
        if (strstr(buffer, "ERROR") != NULL) loraError[AT_MODE] = E_LWAPIERRNO_GENERIC_ERROR;
        else loraDone[AT_MODE] = EHS_TRUE;
    }
    else if (strstr(buffer, AT_COMMAND_STRING[AT_RESET]) != NULL)
    {
        /* Software reset */
        loraDone[AT_RESET] = EHS_TRUE; // Why EHS_FALSE?
    }
    // Check whether this command return starts with the command string
    else if (strstr(buffer, AT_COMMAND_STRING[AT_PORT]) == buffer)
    {
        /* LoRaWAN FPort configuration */
        if (strstr(buffer, "ERROR") != NULL) loraError[AT_PORT] = E_LWAPIERRNO_GENERIC_ERROR;
        else {
            // Save the returned port into the local config
            sscanf(buffer, "+PORT: %d", &gLW_localConfig.fport);
            loraDone[AT_PORT] = EHS_TRUE;
        }
    }
    else
    {
        if (strstr(buffer, AT_COMMAND_STRING[AT_CMSGHEX]) != NULL) msg_type = AT_CMSGHEX;
        else if (strstr(buffer, AT_COMMAND_STRING[AT_MSGHEX]) != NULL) msg_type = AT_MSGHEX;
        else if (strstr(buffer, AT_COMMAND_STRING[AT_CMSG]) != NULL) msg_type = AT_CMSG;
        else if (strstr(buffer, AT_COMMAND_STRING[AT_MSG]) != NULL) msg_type = AT_MSG;
        if (msg_type == AT_MSG || msg_type == AT_MSGHEX || msg_type == AT_CMSGHEX || msg_type == AT_CMSG)
        {
            /* Either sent message or receive a message */
            if (strstr(buffer, "ERROR"))
            {
                /* When command error */
                loraError[msg_type] = E_LWAPIERRNO_GENERIC_ERROR;
            } else if (strstr(buffer, "LoRanWAN modem is busy")) {
                loraError[msg_type] = E_LWAPIERRNO_MODEM_BUSY;
            } else if (strstr(buffer, "Please join network first")) {
                loraError[msg_type] = E_LWAPIERRNO_NOT_JOINED;
            } else if (strstr(buffer, "No free channel")) {
                loraError[msg_type] = E_LWAPIERRNO_ALL_CHANNEL_OCCUPIED;
            } else if (strstr(buffer, "No band in")) {
                loraError[msg_type] = E_LWAPIERRNO_NO_BAND_AVAILABLE;
            } else if (strstr(buffer, "DR error")) {
                loraError[msg_type] = E_LWAPIERRNO_DR_ERROR;
            } else if (strstr(buffer, "Length error")) {
                loraError[msg_type] = E_LWAPIERRNO_PAYLOAD_TOO_LONG;
            } else {
                // Message send complete
                split_max = TEhsLoraWioSplitLine(buffer, strlen(buffer), split_start, split_length, 20);
                split_count = -1; // The next while loop add 1 after comparison, hence need minus 1 in advance
                while (split_count++ < split_max)
                {
                    memset(temp_buf, 0, EHS_STRING_LENGTH_MAX);
                    sprintf(temp_buf, "%.*s", split_length[split_count], buffer + split_start[split_count]);
                    if (strstr(temp_buf, "Start") != NULL)
                    {
                        // Reset message for every message start
                        if (strstr(temp_buf, "MSG") != NULL) memset(gEhsLorawanRxBuffer, 0, LW_RX_BUFFER_SIZE);
                        /* When it starts and then done, it is sent */
                        loraMsgStart[msg_type - AT_MSGHEX] = EHS_TRUE;
                    }
                    else if (strstr(temp_buf, "LINK") != NULL)
                    {
                        /* When it gets the link status */
                        switch (msg_type) {
                            case AT_MSG: {
                                sscanf(temp_buf, "+MSG: LINK %d, %d", &gEhsLoraApiData.link_margin, &gEhsLoraApiData.gateway_count);
                                break;
                            }
                            case AT_CMSG: {
                                sscanf(temp_buf, "+CMSG: LINK %d, %d", &gEhsLoraApiData.link_margin, &gEhsLoraApiData.gateway_count);
                                break;
                            }
                            case AT_MSGHEX: {
                                sscanf(temp_buf, "+MSGHEX: LINK %d, %d", &gEhsLoraApiData.link_margin, &gEhsLoraApiData.gateway_count);
                                break;
                            }
                            case AT_CMSGHEX: {
                                sscanf(temp_buf, "+CMSGHEX: LINK %d, %d", &gEhsLoraApiData.link_margin, &gEhsLoraApiData.gateway_count);
                                break;
                            }
                            default: {
                                loraError[msg_type] = E_LWAPIERRNO_INTERNAL_INVALID_CTRLSEQ;
                                break;
                            }
                        }
                    }
                    else if (strstr(temp_buf, "RSSI") != NULL)
                    {
                        /* When it gets the rx status */
                        switch (msg_type) {
                            case AT_MSG: {
                                sscanf(temp_buf, "+MSG: RXWIN%d, RSSI %d, SNR " EHS_FL_FMT, &gEhsLoraApiData.rxwin, &gEhsLoraApiData.rssi, &gEhsLoraApiData.snr);
                                break;
                            }
                            case AT_CMSG: {
                                sscanf(temp_buf, "+CMSG: RXWIN%d, RSSI %d, SNR " EHS_FL_FMT, &gEhsLoraApiData.rxwin, &gEhsLoraApiData.rssi, &gEhsLoraApiData.snr);
                                break;
                            }
                            case AT_MSGHEX: {
                                sscanf(temp_buf, "+MSGHEX: RXWIN%d, RSSI %d, SNR " EHS_FL_FMT, &gEhsLoraApiData.rxwin, &gEhsLoraApiData.rssi, &gEhsLoraApiData.snr);
                                break;
                            }
                            case AT_CMSGHEX: {
                                sscanf(temp_buf, "+CMSGHEX: RXWIN%d, RSSI %d, SNR " EHS_FL_FMT, &gEhsLoraApiData.rxwin, &gEhsLoraApiData.rssi, &gEhsLoraApiData.snr);
                                break;
                            }
                            default: {
                                loraError[msg_type] = E_LWAPIERRNO_INTERNAL_INVALID_CTRLSEQ;
                                break;
                            }
                        }
                    }
                    else if (strstr(temp_buf, "RX: ") != NULL)
                    {
                        /* When it gets the received message */
                        if (msg_type == AT_MSG) sscanf(temp_buf, "+MSG: PORT: %d; RX: \"%232[0-9A-F]", &temp_int, gEhsLorawanRxBuffer);
                        else if (msg_type == AT_CMSG) sscanf(temp_buf, "+CMSG: PORT: %d; RX: \"%232[0-9A-F]", &temp_int, gEhsLorawanRxBuffer);
                        else if (msg_type == AT_MSGHEX) sscanf(temp_buf, "+MSGHEX: PORT: %d; RX: \"%232[0-9A-F]", &temp_int, gEhsLorawanRxBuffer);
                        else if (msg_type == AT_CMSGHEX) sscanf(temp_buf, "+CMSGHEX: PORT: %d; RX: \"%232[0-9A-F]", &temp_int, gEhsLorawanRxBuffer);
                        gEhsLorawanRxBuffer[strnlen(gEhsLorawanRxBuffer, LW_RX_BUFFER_SIZE - 1)] = 0;
                    }
                    else if (strstr(temp_buf, "Done") != NULL)
                    {
                        /* When the message send or receive is done */
                        if (loraMsgStart[msg_type - AT_MSGHEX] == EHS_TRUE)
                        {
                            loraDone[msg_type] = EHS_TRUE;
                            loraMsgStart[msg_type - AT_MSGHEX] = EHS_FALSE;
                        }
                        else
                        {
                            if (loraConnecting == EHS_FALSE)
                            {
                                ehs_lorawan_debug("gEhsLorawanRxBuffer: %s", gEhsLorawanRxBuffer);
                                if (strnlen(gEhsLorawanRxBuffer, LW_RX_BUFFER_SIZE - 1) > 0)
                                {
                                    Common_LoRaWAN_onReceive(gEhsLorawanRxBuffer, EHS_TRUE);
                                }
                                else Common_LoRaWAN_onReceive("", EHS_FALSE);
                            }
                        }
                    }
                }
            }
        }
        msg_type = 0;
        if (strstr(buffer, AT_COMMAND_STRING[AT_ID]) != NULL)
        {
            /* ID set or retreive */
            if (strstr(buffer, "ERROR") != NULL) loraError[AT_ID] = E_LWAPIERRNO_INVALID_ID;
            else {
                split_count = TEhsLoraWioSplitLine(buffer, strlen(buffer), split_start, split_length, 3);
                while (split_count-- > 0) {
                    memset(temp_buf, 0, EHS_STRING_LENGTH_MAX);
                    sprintf(temp_buf, "%.*s", split_length[split_count], buffer + split_start[split_count]);
                    if (strstr(temp_buf, "AppEui,") != NULL) {
                        sscanf(temp_buf, "+ID: AppEui, %s", gLW_idData.AppEui);
                        TEhsLoraRemoveAllOccurance(gLW_idData.AppEui, ':');
                    } else if (strstr(temp_buf, "DevEui,") != NULL) {
                        sscanf(temp_buf, "+ID: DevEui, %s", gLW_idData.DevEui);
                        TEhsLoraRemoveAllOccurance(gLW_idData.DevEui, ':');
                    } else if (strstr(temp_buf, "DevAddr,") != NULL) {
                        sscanf(temp_buf, "+ID: DevAddr, %s", gLW_idData.DevAddr);
                        TEhsLoraRemoveAllOccurance(gLW_idData.DevAddr, ':');
                    }
                }
                loraDone[AT_ID] = EHS_TRUE;
            }

        }
        if (strstr(buffer, AT_COMMAND_STRING[AT_CLASS]) != NULL)
        {
            /* LoRaWAN Class Setup or Retrieve */
            if (strstr(buffer, "ERROR") != NULL) loraError[AT_CLASS] = E_LWAPIERRNO_INVALID_CLASS;
            else {
                sscanf(buffer, "+CLASS: %s\r\n", lw_misc_buffer);
                loraDone[AT_CLASS] = EHS_TRUE;
            }
        }
        if (strstr(buffer, AT_COMMAND_STRING[AT_LW]) != NULL)
        {
            /* LoRaWAN MAC Command */
            //TODO Complete all LW commands
            if (strstr(buffer, "ERROR") != NULL) loraError[AT_LW] = E_LWAPIERRNO_GENERIC_ERROR;
            else
            {
                if (strstr(buffer, LW_CMD_STRING[LW_WIO_E5_LW_CMD_LEN]) != NULL)
                {
                    // Get payload length
                    sscanf(buffer, "+LW: LEN, %d", gEhsLoraApiData.p_length_out);
                }
                loraDone[AT_LW] = EHS_TRUE;
            }
        }
        if (strstr(buffer, AT_COMMAND_STRING[AT_RTC]) != NULL)
        {
            /* RTC retreive */
            char _lora_rtc_date[11];
            char _lora_rtc_time[9];
            sscanf(buffer, "+RTC: %s %s\r\n", _lora_rtc_date, _lora_rtc_time);
            snprintf(lw_misc_buffer, LW_MISC_BUFFER_SIZE - 1, "%s %s", _lora_rtc_date, _lora_rtc_time);
            loraDone[AT_RTC] = EHS_TRUE;
        }
        if (strstr(buffer, AT_COMMAND_STRING[AT_TEMP]) != NULL)
        {
            /* Core temperture retreive */
            sscanf(buffer, "+TEMP: %s\r\n", lw_misc_buffer);
            loraDone[AT_TEMP] = EHS_TRUE;
        }
        if (strstr(buffer, AT_COMMAND_STRING[AT_VDD]) != NULL)
        {
            /* Module VDD level */
            sscanf(buffer, "+VDD: %s\r\n", lw_misc_buffer);
            loraDone[AT_VDD] = EHS_TRUE;
        }
        if (strstr(buffer, AT_COMMAND_STRING[AT_VER]) != NULL)
        {
            /* Module Firmware version */
            sscanf(buffer, "+VER: %s\r\n", lw_misc_buffer);
            loraDone[AT_VER] = EHS_TRUE;
        }
        if (strstr(buffer, AT_COMMAND_STRING[AT_POWER]) != NULL)
        {
            /* TX power set or retrieve */
            if (strstr(buffer, "ERROR") != NULL) loraError[AT_POWER] = E_LWAPIERRNO_GENERIC_ERROR;
            else {
                sscanf(buffer, "+POWER: %d", &gEhsLoraApiData.tx_power);
                loraDone[AT_POWER] = EHS_TRUE;
            }
        }
        if (strstr(buffer, AT_COMMAND_STRING[AT_RXWIN2]) != NULL)
        {
            /* RX window 2 set or retrieve */
            if (strstr(buffer, "ERROR") != NULL) loraError[AT_RXWIN2] = E_LWAPIERRNO_GENERIC_ERROR;
            else loraDone[AT_RXWIN2] = EHS_TRUE;
        }
    }

LoRaWAN_wioe5_onUart_Return:
    if (_bufferFlag == EHS_TRUE)
    {
        memset(buffer, 0, LW_BUFFER_SIZE);
        buffer_index = 0;
    }
}
