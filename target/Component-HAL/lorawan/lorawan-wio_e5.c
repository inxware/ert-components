#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lorawan-wio_e5.h"
#include "lorawan.h"
#include "target.h"
#include "target_uart.h"
#include "lorawan_helper.h"

#define LORAWAN_WIO_E5_DEBUG

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
    AT_MSG,
    AT_CMSG,
    AT_LW,
    AT_RESET,
    AT_VDD,
    AT_TEMP,
    AT_RTC,
    AT_MAX_VALUE
};
static char AT_COMMAND_STRING[][10] = {
    "+ID",
    "+KEY",
    "+MODE",
    "+CLASS",
    "+DR",
    "+REPT",
    "+RETRY",
    "+JOIN",
    "+MSG",
    "+CMSG",
    "+LW",
    "+RESET",
    "+VDD",
    "+TEMP",
    "+RTC"
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

static lw_iddata_t gLW_idData = {
    .DevAddr = "",
    .DevEui = "",
    .AppEui = ""
};

// LoRaWAN message start indicates the MSG is sent
static ehs_bool loraMsgStart = EHS_FALSE;
// Command done
static volatile ehs_bool loraDone[AT_MAX_VALUE] = {EHS_FALSE};
// Command Error
static volatile ehs_bool loraError[AT_MAX_VALUE] = {EHS_FALSE};
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

// rx_buffer
#define LW_RX_BUFFER_SIZE 1024
static char lw_rx_buffer[LW_RX_BUFFER_SIZE] = {0};

// misc buffer
#define LW_MISC_BUFFER_SIZE 64
static char lw_misc_buffer[LW_MISC_BUFFER_SIZE] = {0};

// static void memset_volatile(volatile void *restrict s, char c, size_t n)
// {
// 	volatile char *p = s;
// 	while (n-- > 0) *p++ = c;
// }
// 
// 
// #define LW_DR_PLAN_SIZE 11
// static char LW_DR_PLAN[][LW_DR_PLAN_SIZE] = {
// 	"EU868",
// 	"US915",
// 	"CN779",
// 	"EU433",
// 	"AU915",
// 	"CN470",
// 	"AS923",
// 	"KR920",
// 	"IN865",
// 	"RU864",
// 	"STE920"
// };
// 
// /*
//  * Find the last occurance of the string in the string list
//  * Return:
//  * 	pointer to the occurance, otherwise NULL
//  *
//  * */
// static char ** checkInStringList(char **haystack, char *needle, size_t haystack_size)
// {
// 	char ** ret = NULL;
// 	while (haystack_size-- > 0)
// 	{
// 		if (strcmp(haystack[haystack_size], needle) == 0)
// 		{
// 			ret = haystack + haystack_size;
// 			break;
// 		}
// 	}
// }

static void LoRaWAN_wioe5_onUart(char *payload, int length);

/*
 * Send the UART message until "DONE" or "ERROR" received.
 * Return:
 * 	0 if "DONE"
 * 	1 if "ERROR"
 * 	< 0 if UART error
 *
 * */
static int sendWaitUntilComplete(char *UART_payload, int at_command)
{
#ifdef LORAWAN_WIO_E5_DEBUG
    printf("payload: %s, at_command: %d\n", UART_payload, at_command);
#endif
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
        if (loraError[at_command] == EHS_TRUE) {
            loraError[at_command] = EHS_FALSE;
            ret = 1;
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
int LoRaWAN_wioe5_init()
{
    int ret = TgtUART_OK;
    if ((ret = TgtUart_Stage0(LORA_UART_PORT)) != TgtUART_OK) return ret;
    // UART default pins, baudrate 9600, 8 data bits, 1 stop bit, no parity check, no flow control
    if ((ret = TgtUart_Start(LORA_UART_PORT,
                             TARGET_UART_PIN_TX_PORT(LORA_UART_PORT),
                             TARGET_UART_PIN_RX_PORT(LORA_UART_PORT),
                             TARGET_UART_PIN_RTS_PORT(LORA_UART_PORT),
                             TARGET_UART_PIN_CTS_PORT(LORA_UART_PORT),
                             9600, 3, 0, 0, 0)) != TgtUART_OK) {printf("Uart Start ret: %d\n", ret); return ret;}
//	if ((ret = TgtUart_Start(LORA_UART_PORT)) != TgtUART_OK) { printf("UART Start ret: %d\n", ret); return ret; }
    if ((ret = TgtUART_Intr_register(LORA_UART_PORT, &LoRaWAN_wioe5_onUart)) != TgtUART_OK) {printf("Uart Intr Reg ret: %d\n", ret); return ret;}

    return ret;
}

/*
 * De-init (destroy) the UART interface
 *
 * */
int LoRaWAN_wioe5_deinit()
{
    int ret = TgtUART_OK;
    ret = TgtUart_Stop(LORA_UART_PORT);
    return ret;
}

/*
 * Initiate the connection to the LoRaWAN server. Pass the resulting DevAddr to DevAddr_OUT.
 *  Note that the DevAddr_OUT should be allocated with sufficient memory.
 *
 * */
int LoRaWAN_wioe5_connect(char *AppKey, char *AppEui, ehs_bool mode, char *DevAddr_ABP, char *AppSKey, char *NwkSKey, ehs_sint32 REPT, ehs_sint32 RETRY, char *region, char *DevAddr_OUT)
{
    loraConnecting = EHS_TRUE;
    int ret = 0;
    char send_buffer[LW_SEND_BUFFER_SIZE];

    // Set essential parameters according to the mode
    if (mode == EHS_FALSE)
    {
        // mode=0 --> OTAA method
        snprintf(send_buffer, LW_SEND_BUFFER_SIZE - 1, "AT%s=AppKey,\"%s\"\r\n", AT_COMMAND_STRING[AT_KEY], AppKey);
        if ((ret = sendWaitUntilComplete(send_buffer, AT_KEY)) != 0) return ret;
        snprintf(send_buffer, LW_SEND_BUFFER_SIZE - 1, "AT%s=AppEui,\"%s\"\r\n", AT_COMMAND_STRING[AT_ID], AppEui);
        if ((ret = sendWaitUntilComplete(send_buffer, AT_ID)) != 0) return ret;
        snprintf(send_buffer, LW_SEND_BUFFER_SIZE - 1, "AT%s=LWOTAA\r\n", AT_COMMAND_STRING[AT_MODE]);
        if ((ret = sendWaitUntilComplete(send_buffer, AT_MODE)) != 0) return ret;
    }
    else if (mode == EHS_TRUE) {
        // mode=1 --> ABP method
        snprintf(send_buffer, LW_SEND_BUFFER_SIZE - 1, "AT%s=DevAddr,\"%s\"\r\n", AT_COMMAND_STRING[AT_ID], DevAddr_ABP);
        if ((ret = sendWaitUntilComplete(send_buffer, AT_ID)) != 0) return ret;
        snprintf(send_buffer, LW_SEND_BUFFER_SIZE - 1, "AT%s=AppSKey,\"%s\"\r\n", AT_COMMAND_STRING[AT_KEY], AppSKey);
        if ((ret = sendWaitUntilComplete(send_buffer, AT_KEY)) != 0) return ret;
        snprintf(send_buffer, LW_SEND_BUFFER_SIZE - 1, "AT%s=NwkSKey,\"%s\"\r\n", AT_COMMAND_STRING[AT_KEY], NwkSKey);
        if ((ret = sendWaitUntilComplete(send_buffer, AT_KEY)) != 0) return ret;
        snprintf(send_buffer, LW_SEND_BUFFER_SIZE - 1, "AT%s=LWABP\r\n", AT_COMMAND_STRING[AT_MODE]);
        if ((ret = sendWaitUntilComplete(send_buffer, AT_MODE)) != 0) return ret;
    }
    //snprintf(send_buffer, LW_SEND_BUFFER_SIZE - 1, "AT%s=%s\r\n", AT_COMMAND_STRING[AT_DR], 
    //   		checkInStringList(LW_DR_PLAN, region, LW_DR_PLAN_SIZE) != NULL ? region : LW_DR_PLAN[0]);
    snprintf(send_buffer, LW_SEND_BUFFER_SIZE - 1, "AT%s=%s\r\n", AT_COMMAND_STRING[AT_DR], region);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_DR)) != 0) return ret;
    snprintf(send_buffer, LW_SEND_BUFFER_SIZE - 1, "AT%s=%d\r\n", AT_COMMAND_STRING[AT_REPT], REPT);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_REPT)) != 0) return ret;
    snprintf(send_buffer, LW_SEND_BUFFER_SIZE - 1, "AT%s=%d\r\n", AT_COMMAND_STRING[AT_RETRY], RETRY);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_RETRY)) != 0) return ret;
    snprintf(send_buffer, LW_SEND_BUFFER_SIZE - 1, "AT%s=C\r\n", AT_COMMAND_STRING[AT_CLASS]);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_CLASS)) != 0) return ret;
    snprintf(send_buffer, LW_SEND_BUFFER_SIZE - 1, "AT%s\r\n", AT_COMMAND_STRING[AT_JOIN]);
    while ((ret = sendWaitUntilComplete(send_buffer, AT_JOIN)) != 0);

    // LoRaWAN MAC command to retreive RTC data
    snprintf(send_buffer, LW_SEND_BUFFER_SIZE - 1, "AT%s=DTR\r\n", AT_COMMAND_STRING[AT_LW]);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_LW)) != 0) return ret;
    snprintf(send_buffer, LW_SEND_BUFFER_SIZE - 1, "AT%s\r\n", AT_COMMAND_STRING[AT_MSG]);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_MSG)) != 0) return ret;

    // Get DevAddr
    snprintf(send_buffer, LW_SEND_BUFFER_SIZE - 1, "AT%s=DevAddr\r\n", AT_COMMAND_STRING[AT_ID]);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_ID)) != 0) return ret;
    ret = strncmp(strcpy(DevAddr_OUT, gLW_idData.DevAddr), gLW_idData.DevAddr, strlen(gLW_idData.DevAddr) + 1);

    loraConnecting = EHS_FALSE;
    return ret;
}

/*
 * Send message to the server, either unconfirmed or confirmed
 *
 * */
int LoRaWAN_wioe5_send_msg(char *payload, ehs_bool confirmed)
{
    int ret = 0;
    char send_buffer[LW_SEND_BUFFER_SIZE];
    snprintf(send_buffer, LW_SEND_BUFFER_SIZE - 1, "AT%s=\"%s\"\r\n", AT_COMMAND_STRING[confirmed == EHS_TRUE ? AT_CMSG : AT_MSG], payload);
    ret = sendWaitUntilComplete(send_buffer, confirmed == EHS_TRUE ? AT_CMSG : AT_MSG);
    return ret;
}

/*
 * LoRaWAN module software reset
 *
 * */
int LoRaWAN_wioe5_reset( void )
{
    char send_buffer[LW_SEND_BUFFER_SIZE];
    snprintf(send_buffer, LW_SEND_BUFFER_SIZE - 1, "AT%s\r\n", AT_COMMAND_STRING[AT_RESET]);
    return sendWaitUntilComplete(send_buffer, AT_RESET);
}

/* 
 * Get the module's RTC time, core temperature, power supply voltage and DevEui. Pass them as a JSON string into the argument.
 * Note that the passed argument should have memory allocated. If that is 
 *
 * */
int LoRaWAN_wioe5_get_sysData(char *data, char *DevEui)
{
    int ret = 0;
    char send_buffer[LW_SEND_BUFFER_SIZE] = {0};
    lw_sysdata_t sysData;
    char result_data[LW_MISC_BUFFER_SIZE] = {0};

    snprintf(send_buffer, LW_SEND_BUFFER_SIZE - 1, "AT%s\r\n", AT_COMMAND_STRING[AT_RTC]);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_RTC)) != 0) goto LoRaWAN_wio_e5_get_sysData_Return;
    sysData.rtc = (char *)malloc(strlen(lw_misc_buffer) + 1);
    memcpy(sysData.rtc, lw_misc_buffer, strlen(lw_misc_buffer) + 1);

    snprintf(send_buffer, LW_SEND_BUFFER_SIZE - 1, "AT%s\r\n", AT_COMMAND_STRING[AT_TEMP]);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_TEMP)) != 0) goto LoRaWAN_wio_e5_get_sysData_Return;
    sysData.temp = (char *)malloc(strlen(lw_misc_buffer) + 1);
    memcpy(sysData.temp, lw_misc_buffer, strlen(lw_misc_buffer) + 1);

    snprintf(send_buffer, LW_SEND_BUFFER_SIZE - 1, "AT%s\r\n", AT_COMMAND_STRING[AT_VDD]);
    if ((ret = sendWaitUntilComplete(send_buffer, AT_VDD)) != 0) goto LoRaWAN_wio_e5_get_sysData_Return;
    sysData.vdd = (char *)malloc(strlen(lw_misc_buffer) + 1);
    memcpy(sysData.vdd, lw_misc_buffer, strlen(lw_misc_buffer) + 1);

    snprintf(send_buffer, LW_SEND_BUFFER_SIZE - 1, "AT%s=DevEui\r\n", AT_COMMAND_STRING[AT_ID]);
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
LoRaWAN_wio_e5_get_sysData_Return:
    if (sysData.rtc    != NULL) free(sysData.rtc);    sysData.rtc = NULL;
    if (sysData.temp   != NULL) free(sysData.temp);   sysData.temp = NULL;
    if (sysData.vdd    != NULL) free(sysData.vdd);    sysData.vdd = NULL;
    if (sysData.DevEui != NULL) free(sysData.DevEui); sysData.DevEui = NULL;
    return ret;
}

static void LoRaWAN_wioe5_onUart(char *payload, int length)
{
    assert(buffer_index + length < LW_BUFFER_SIZE);
    ehs_bool _bufferFlag = EHS_FALSE;
    char **recv_payload = NULL;
    char **id_payload = NULL;
    int token_count, id_payload_count;
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
            loraError[AT_JOIN] = EHS_TRUE;
        } else if (strstr(buffer, "Join failed") != NULL) {
            loraError[AT_JOIN] = EHS_TRUE;
        } else {
            if (strstr(buffer, "Done") != NULL) loraDone[AT_JOIN] = EHS_TRUE;
        }
    }
    else if (strstr(buffer, AT_COMMAND_STRING[AT_KEY]) != NULL)
    {
        /* LoRaWAN KEY set */
        if (strstr(buffer, "ERROR") != NULL) loraError[AT_KEY] = EHS_TRUE;
        else loraDone[AT_KEY] = EHS_TRUE;
    }
    else if (strstr(buffer, AT_COMMAND_STRING[AT_DR]) != NULL)
    {
        /* LoRaWAN region setup */
        if (strstr(buffer, "ERROR") != NULL) loraError[AT_DR] = EHS_TRUE;
        else loraDone[AT_DR] = EHS_TRUE;
    }
    else if (strstr(buffer, AT_COMMAND_STRING[AT_REPT]) != NULL)
    {
        /* LoRaWAN Repeat parameter setup */
        if (strstr(buffer, "ERROR") != NULL) loraError[AT_REPT] = EHS_TRUE;
        else loraDone[AT_REPT] = EHS_TRUE;
    }
    else if (strstr(buffer, AT_COMMAND_STRING[AT_RETRY]) != NULL)
    {
        /* LoRaWAN Retry parameter setup */
        if (strstr(buffer, "ERROR") != NULL) loraError[AT_RETRY] = EHS_TRUE;
        else loraDone[AT_RETRY] = EHS_TRUE;
    }
    else if (strstr(buffer, AT_COMMAND_STRING[AT_MODE]) != NULL)
    {
        /* LoRaWAN Mode set */
        if (strstr(buffer, "ERROR") != NULL) loraError[AT_MODE] = EHS_TRUE;
        else loraDone[AT_MODE] = EHS_TRUE;
    }
    else if (strstr(buffer, AT_COMMAND_STRING[AT_RESET]) != NULL)
    {
        /* Software reset */
        loraDone[AT_RESET] = EHS_TRUE; // Why EHS_FALSE?
    }
    else 
    {
        if (strstr(buffer, AT_COMMAND_STRING[AT_MSG]) != NULL)
        {
            /* Either sent message or receive a message */
            if (strstr(buffer, "ERROR"))
            {
                /* When command error */
                loraError[AT_MSG] = EHS_TRUE;
            } else {
                // Message send complete
                if (strstr(buffer, "Start") != NULL)
                {
                    /* When it starts and then done, it is sent */
                    loraMsgStart = EHS_TRUE;
                }
                if (strstr(buffer, "RX: ") != NULL)
                {
                    /* When it gets the received message */
                    recv_payload = token_split(buffer, "\"", &token_count);
                    assert(strlen(recv_payload[1]) < LW_RX_BUFFER_SIZE);
                    memcpy(lw_rx_buffer, recv_payload[1], strlen(recv_payload[1]) + 1);
                    memset(lw_rx_buffer + strlen(recv_payload[1]), 0, 1);
                    if (recv_payload != NULL) free(recv_payload);
                    recv_payload = NULL;
                }
                if (strstr(buffer, "Done") != NULL)
                {
                    /* When the message send or receive is done */
                    if (loraMsgStart == EHS_TRUE)
                    {
                        loraDone[AT_MSG] = EHS_TRUE;
                        loraMsgStart = EHS_FALSE;
                    }
                    else
                    {
                        if (loraConnecting == EHS_FALSE)
                        {
                                printf("lw_rx_buffer: %s", lw_rx_buffer);
                            if (strnlen(lw_rx_buffer, LW_RX_BUFFER_SIZE - 1) > 0)
                            {
                                Common_LoRaWAN_onReceive(lw_rx_buffer, EHS_TRUE);
                            }
                            else Common_LoRaWAN_onReceive("", EHS_FALSE);
                        }
                    }
                    memset(lw_rx_buffer, 0, LW_RX_BUFFER_SIZE);
                }
            }
        }
        if (strstr(buffer, AT_COMMAND_STRING[AT_ID]) != NULL)
        {
            /* ID set or retreive */
            if (strstr(buffer, "ERROR") != NULL) loraError[AT_ID] = EHS_TRUE;
            else {
                // TODO...
                //TODO Seperate by "\r\n", then by " " for each. Then the 2nd and 3rd are Key and Value
                recv_payload = token_split(buffer, "\r\n", &token_count);
                while (token_count-- > 0) {
                    id_payload = token_split(recv_payload[token_count], " ", &id_payload_count);
                    if (strcmp(id_payload[1], "AppEui,") == 0) {
                        memcpy(gLW_idData.AppEui, id_payload[2], strlen(id_payload[2]) + 1);
                    } else if (strcmp(id_payload[1], "DevEui,") == 0) {
                        memcpy(gLW_idData.DevEui, id_payload[2], strlen(id_payload[2]) + 1);
                    } else if (strcmp(id_payload[1], "DevAddr,") == 0) {
                        memcpy(gLW_idData.DevAddr, id_payload[2], strlen(id_payload[2]) + 1);
                    }
                    if (id_payload != NULL) free(id_payload);
                    id_payload = NULL;
                }
                if (recv_payload != NULL) free(recv_payload);
                recv_payload = NULL;
                loraDone[AT_ID] = EHS_TRUE;
            }

        }
        if (strstr(buffer, AT_COMMAND_STRING[AT_CLASS]) != NULL)
        {
            /* LoRaWAN Class Setup or Retrieve */
            if (strstr(buffer, "ERROR") != NULL) loraError[AT_CLASS] = EHS_TRUE;
            else {
                recv_payload = token_split(buffer, " ", &token_count);
                memcpy(lw_misc_buffer, recv_payload[1], strlen(recv_payload[1]) + 1);
                if (recv_payload != NULL) free(recv_payload);
                recv_payload = NULL;
                loraDone[AT_CLASS] = EHS_TRUE;
            }
        }
        if (strstr(buffer, AT_COMMAND_STRING[AT_LW]) != NULL)
        {
            /* LoRaWAN MAC Command */
            if (strstr(buffer, "ERROR") != NULL) loraError[AT_LW] = EHS_TRUE;
            else loraDone[AT_LW] = EHS_TRUE;
        }
        if (strstr(buffer, AT_COMMAND_STRING[AT_RTC]) != NULL)
        {
            /* RTC retreive */
            recv_payload = token_split(buffer, " ", &token_count);
            snprintf(lw_misc_buffer, LW_MISC_BUFFER_SIZE - 1, "%s %s", recv_payload[1], recv_payload[2]);
            if (recv_payload != NULL) free(recv_payload);
            recv_payload = NULL;
            loraDone[AT_RTC] = EHS_TRUE;
        }
        if (strstr(buffer, AT_COMMAND_STRING[AT_TEMP]) != NULL)
        {
            /* Core temperture retreive */
            recv_payload = token_split(buffer, " ", &token_count);
            memcpy(lw_misc_buffer, recv_payload[1], strlen(recv_payload[1]) + 1);
            if (recv_payload != NULL) free(recv_payload);
            recv_payload = NULL;
            loraDone[AT_TEMP] = EHS_TRUE;
        }
        if (strstr(buffer, AT_COMMAND_STRING[AT_VDD]) != NULL)
        {
            /* Module VDD level */
            recv_payload = token_split(buffer, " ", &token_count);
            memcpy(lw_misc_buffer, recv_payload[1], strlen(recv_payload[1]) + 1);
            if (recv_payload != NULL) free(recv_payload);
            recv_payload = NULL;
            loraDone[AT_VDD] = EHS_TRUE;
        }
    }

LoRaWAN_wioe5_onUart_Return:
    if (_bufferFlag == EHS_TRUE)
    {
        memset(buffer, 0, LW_BUFFER_SIZE);
        buffer_index = 0;
    }
}
