#include <stdio.h>
#include <string.h>
#include "target.h"
#include "hal.h"
#include "lorawan.h"

#define EHS_TARGET_LORAWAN_THREADING_SUPPORT

/* START - LoRaWAN target includes - START */
#include "lorawan-wio_e5.h"

/* END - LoRaWAN target includes - END */

/* START - LoRaWAN function prototypes - START */
// Only for targets with threading support
#ifdef  EHS_TARGET_LORAWAN_THREADING_SUPPORT
static void taskLoRaWAN_execute_cmd( void *context );
static void LoRaWAN_run_threadInLoop();
#endif//EHS_TARGET_LORAWAN_THREADING_SUPPORT
/* END - LoRaWAN function prototypes - END */

/* START - LoRaWAN global variables - START */
// Only for targets with threading support
#ifdef  EHS_TARGET_LORAWAN_THREADING_SUPPORT
struct ehs_lorawan_api_data_connect_s {
    //
    ehs_char AppKey[EHS_LORAWAN_KEY_STRLEN + 1];
    ehs_char AppEui[EHS_LORAWAN_ID_STRLEN + 1];
    ehs_bool mode;
    ehs_char DevAddr_ABP[EHS_LORAWAN_ID_STRLEN + 1];
    ehs_char AppSKey[EHS_LORAWAN_KEY_STRLEN + 1];
    ehs_char NwkSKey[EHS_LORAWAN_KEY_STRLEN + 1];
    ehs_sint32 REPT;
    ehs_sint32 RETRY;
    e_ehs_lw_region_t region;
    ehs_bool ADR;
    ehs_sint32 DR;
    ehs_sint32 autoJoin;
    // This is a pointer to be written (i.e. output)
    ehs_char *DevAddr_OUT;
};

struct ehs_lorawan_api_data_send_msg_s {
    // Worst case of payload. This is the length of 242-byte payload in hex format.
    ehs_char payload[485];
    ehs_bool confirmed;
    ehs_uint8 fport;
};

struct ehs_lorawan_api_data_reset_s {
    ehs_bool reserved;
};

struct ehs_lorawan_api_data_get_sysdata_s {
    // Pointer output
    ehs_char *sysData;
    // Pointer output
    ehs_char *DevEui;
    // Reserved for later
    ehs_bool reserved;
};

struct ehs_lorawan_api_data_set_datarate_s {
    ehs_sint32 datarate;
};

struct ehs_lorawan_api_data_get_payloadLength_s {
    ehs_sint32 *length_out;
};

struct ehs_lorawan_api_data_disable_s {
    ehs_bool reserved;
};

// Union type to put the data for different lorawan API cmd data in the same memory space
//  Note that this should only apply when only one command in executed at a time
typedef union {
    struct ehs_lorawan_api_data_connect_s 		connect;
    struct ehs_lorawan_api_data_send_msg_s 		send_msg;
    struct ehs_lorawan_api_data_reset_s 		reset;
    struct ehs_lorawan_api_data_get_sysdata_s 	get_sysdata;
    struct ehs_lorawan_api_data_set_datarate_s  set_datarate;
    struct ehs_lorawan_api_data_get_payloadLength_s get_payloadLength;
    struct ehs_lorawan_api_data_disable_s       disable;
} ehs_lorawan_api_data_ut;

static ehs_lorawan_api_data_ut g_lorawan_api_data;
static e_ehs_lorawan_api_cmd_t g_lorawan_cmd = E_LORAWAN_API__NOT_INTIALISED;
#endif//EHS_TARGET_LORAWAN_THREADING_SUPPORT
/* END - LoRaWAN global variables - END */

static e_ehs_lw_target_t gLorawanTarget;
ehs_lorawan_api_data_t gEhsLoraApiData;

// rx_buffer
char gEhsLorawanRxBuffer[LW_RX_BUFFER_SIZE] = { 0 };

ehs_lorawan_api_errno_t LoRaWAN_init(e_ehs_lw_target_t target)
{
    if (g_lorawan_cmd != E_LORAWAN_API__NOT_INTIALISED) return E_LWAPIERRNO_ALREADY_INITIALISED;

    ehs_lorawan_api_errno_t ret = E_LWAPIERRNO_OK;
    gLorawanTarget = target;
    // Initialise the API return code
    e_ehs_lorawan_api_cmd_t temp_cmd;
    for (temp_cmd = 0 ; temp_cmd < E_LORAWAN_API__MAX_VALUE ; temp_cmd++) gEhsLoraApiData.error_ret[temp_cmd] = E_LWAPIERRNO_OK;
    #ifdef  EHS_TARGET_LORAWAN_THREADING_SUPPORT
    // Start thread
    EhsHThread_execute(&taskLoRaWAN_execute_cmd, NULL, 0, 3072);
    #endif//EHS_TARGET_LORAWAN_THREADING_SUPPORT;
    switch (gLorawanTarget) {
        case E_EHS_LWTARGET_WIO_E5:
            /**/
            ret = LoRaWAN_wioe5_init();
            if (ret == E_LWAPIERRNO_OK) g_lorawan_cmd = E_LORAWAN_API__IDLE;
            break;
        default:
            ret = -100;
            break;
    }
    return ret;
}

ehs_lorawan_api_errno_t LoRaWAN_deinit()
{
    if (g_lorawan_cmd == E_LORAWAN_API__NOT_INTIALISED) return E_LWAPIERRNO_NOT_INITIALISED;
    if (g_lorawan_cmd != E_LORAWAN_API__IDLE && g_lorawan_cmd != E_LORAWAN_API__COMPLETE)
        return E_LWAPIERRNO_BUSY;

    ehs_lorawan_api_errno_t ret = E_LWAPIERRNO_OK;
    switch (gLorawanTarget) {
        case E_EHS_LWTARGET_WIO_E5:
            ret = LoRaWAN_wioe5_deinit();
            if (ret == E_LWAPIERRNO_OK) g_lorawan_cmd = E_LORAWAN_API__NOT_INTIALISED;
            break;
        default:
            ret = -100;
            break;
    }
    return ret;
}

static ehs_lorawan_api_errno_t _LoRaWAN_connect(char *AppKey, char *AppEui, ehs_bool mode, char *DevAddr_ABP, char *AppSKey, char *NwkSKey, ehs_sint32 REPT, ehs_sint32 RETRY, e_ehs_lw_region_t region, ehs_bool ADR, ehs_sint32 DR, ehs_sint32 autoJoin, char *DevAddr_OUT)
{
    if (AppKey == NULL || AppEui == NULL || DevAddr_ABP == NULL || AppSKey == NULL || NwkSKey == NULL || DevAddr_OUT == NULL)
    {
        return E_LWAPIERRNO_INTERNAL_ERROR;
    }
    ehs_lorawan_api_errno_t ret = E_LWAPIERRNO_OK;
    switch (gLorawanTarget) {
        case E_EHS_LWTARGET_WIO_E5:
            ret = LoRaWAN_wioe5_connect(AppKey, AppEui, mode, DevAddr_ABP, AppSKey, NwkSKey, REPT, RETRY, region, ADR, DR, autoJoin, DevAddr_OUT);
            break;
        default:
            ret = -100;
            break;
    }
    if (ret == E_LWAPIERRNO_OK) g_lorawan_cmd = E_LORAWAN_API__COMPLETE;
    gEhsLoraApiData.error_ret[E_LORAWAN_API_CONNECT] = ret;
    Common_LoRaWAN_FBCBs(E_LORAWAN_API_CONNECT);
    return ret;
}

ehs_lorawan_api_errno_t LoRaWAN_connect(char *AppKey, char *AppEui, ehs_bool mode, char *DevAddr_ABP, char *AppSKey, char *NwkSKey, ehs_sint32 REPT, ehs_sint32 RETRY, e_ehs_lw_region_t region, ehs_bool ADR, ehs_sint32 DR, ehs_sint32 autoJoin, char *DevAddr_OUT)
{
    if (g_lorawan_cmd == E_LORAWAN_API__NOT_INTIALISED) return E_LWAPIERRNO_NOT_INITIALISED;
    if (AppKey == NULL || AppEui == NULL || DevAddr_ABP == NULL || AppSKey == NULL || NwkSKey == NULL || DevAddr_OUT == NULL)
    {
        return E_LWAPIERRNO_INTERNAL_ERROR;
    }
    // The region enum index is out-of-range
    if (region < 0 || region >= E_LWREGION_MAXVALUE) return E_LWAPIERRNO_INVALID_REGION;

    if (g_lorawan_cmd != E_LORAWAN_API__IDLE && g_lorawan_cmd != E_LORAWAN_API__COMPLETE)
        return E_LWAPIERRNO_BUSY;

    #ifdef  EHS_TARGET_LORAWAN_THREADING_SUPPORT
    g_lorawan_api_data.connect.mode = mode;
    g_lorawan_api_data.connect.REPT = REPT;
    g_lorawan_api_data.connect.RETRY = RETRY;
    g_lorawan_api_data.connect.region = region;
    g_lorawan_api_data.connect.ADR = ADR;
    g_lorawan_api_data.connect.DR = DR;
    g_lorawan_api_data.connect.autoJoin = autoJoin;
    g_lorawan_api_data.connect.DevAddr_OUT = DevAddr_OUT;
    memcpy(g_lorawan_api_data.connect.AppKey, AppKey, EHS_LORAWAN_KEY_STRLEN + 1);
    memcpy(g_lorawan_api_data.connect.AppEui, AppEui, EHS_LORAWAN_ID_STRLEN + 1);
    memcpy(g_lorawan_api_data.connect.DevAddr_ABP, DevAddr_ABP, EHS_LORAWAN_ID_STRLEN + 1);
    memcpy(g_lorawan_api_data.connect.AppSKey, AppSKey, EHS_LORAWAN_KEY_STRLEN + 1);
    memcpy(g_lorawan_api_data.connect.NwkSKey,NwkSKey, EHS_LORAWAN_KEY_STRLEN + 1);
    #endif//#ifdef EHS_TARGET_LORAWAN_THREADING_SUPPORT

    g_lorawan_cmd = E_LORAWAN_API_CONNECT;

    #ifndef  EHS_TARGET_LORAWAN_THREADING_SUPPORT
    return _LoRaWAN_connect(AppKey, AppEui, mode, DevAddr_ABP, AppSKey, NwkSKey, REPT, RETRY, region, DevAddr_OUT);
    #endif//#ifndef EHS_TARGET_LORAWAN_THREADING_SUPPORT

    return E_LWAPIERRNO_OK;
}

static ehs_lorawan_api_errno_t _LoRaWAN_send_msg(char *payload, int fport, ehs_bool confirmed)
{
    if (payload == NULL) return E_LWAPIERRNO_INTERNAL_ERROR;
    ehs_lorawan_api_errno_t ret = E_LWAPIERRNO_OK;
    switch (gLorawanTarget) {
        case E_EHS_LWTARGET_WIO_E5:
            ret = LoRaWAN_wioe5_send_msg(payload, fport, confirmed);
            break;
        default:
            ret = -100;
            break;
    }
    if (ret == E_LWAPIERRNO_OK) g_lorawan_cmd = E_LORAWAN_API__COMPLETE;
    gEhsLoraApiData.error_ret[E_LORAWAN_API_SEND_MSG] = ret;
    Common_LoRaWAN_FBCBs(E_LORAWAN_API_SEND_MSG);
    return ret;
}

ehs_lorawan_api_errno_t LoRaWAN_send_msg(char *payload, int fport, ehs_bool confirmed)
{
    if (g_lorawan_cmd == E_LORAWAN_API__NOT_INTIALISED) return E_LWAPIERRNO_NOT_INITIALISED;
    if (payload == NULL) return E_LWAPIERRNO_INTERNAL_ERROR;

    if (g_lorawan_cmd != E_LORAWAN_API__IDLE && g_lorawan_cmd != E_LORAWAN_API__COMPLETE)
        return E_LWAPIERRNO_BUSY;
    
    #ifdef  EHS_TARGET_LORAWAN_THREADING_SUPPORT
    g_lorawan_api_data.send_msg.confirmed = confirmed;
    g_lorawan_api_data.send_msg.fport = fport;
    memcpy(g_lorawan_api_data.send_msg.payload, payload, 485);
    #endif//#ifdef EHS_TARGET_LORAWAN_THREADING_SUPPORT
    
    g_lorawan_cmd = E_LORAWAN_API_SEND_MSG;

    #ifndef  EHS_TARGET_LORAWAN_THREADING_SUPPORT
    return _LoRaWAN_send_msg(payload, fport, confirmed);
    #endif//#ifndef EHS_TARGET_LORAWAN_THREADING_SUPPORT

    return E_LWAPIERRNO_OK;
}

static ehs_lorawan_api_errno_t _LoRaWAN_reset()
{
    ehs_lorawan_api_errno_t ret = E_LWAPIERRNO_OK;
    switch (gLorawanTarget) {
        case E_EHS_LWTARGET_WIO_E5:
            ret = LoRaWAN_wioe5_reset();
            break;
        default:
            ret = -100;
            break;
    }
    if (ret == E_LWAPIERRNO_OK) g_lorawan_cmd = E_LORAWAN_API__COMPLETE;
    gEhsLoraApiData.error_ret[E_LORAWAN_API_RESET] = ret;
    Common_LoRaWAN_FBCBs(E_LORAWAN_API_RESET);
    return ret;
}

ehs_lorawan_api_errno_t LoRaWAN_reset()
{
    if (g_lorawan_cmd == E_LORAWAN_API__NOT_INTIALISED) return E_LWAPIERRNO_NOT_INITIALISED;
    if (g_lorawan_cmd != E_LORAWAN_API__IDLE && g_lorawan_cmd != E_LORAWAN_API__COMPLETE)
        return E_LWAPIERRNO_BUSY;

    #ifdef  EHS_TARGET_LORAWAN_THREADING_SUPPORT
    // Dummy code. Reserved for later
    ;
    #endif//#ifdef EHS_TARGET_LORAWAN_THREADING_SUPPORT
    
    g_lorawan_cmd = E_LORAWAN_API_RESET;
    
    #ifndef  EHS_TARGET_LORAWAN_THREADING_SUPPORT
    return _LoRaWAN_reset();
    #endif//#ifndef EHS_TARGET_LORAWAN_THREADING_SUPPORT

    return E_LWAPIERRNO_OK;
}

static ehs_lorawan_api_errno_t _LoRaWAN_get_sysData(char *sysData_out, char *DevEui_out)
{
    if (sysData_out == NULL || DevEui_out == NULL) return E_LWAPIERRNO_INTERNAL_ERROR;
    ehs_lorawan_api_errno_t ret = E_LWAPIERRNO_OK;
    switch (gLorawanTarget) {
        case E_EHS_LWTARGET_WIO_E5:
            ret = LoRaWAN_wioe5_get_sysData(sysData_out, DevEui_out);
            break;
        default:
            ret = -100;
            break;
    }
    if (ret == E_LWAPIERRNO_OK) g_lorawan_cmd = E_LORAWAN_API__COMPLETE;
    gEhsLoraApiData.error_ret[E_LORAWAN_API_GET_SYSDATA] = ret;
    Common_LoRaWAN_FBCBs(E_LORAWAN_API_GET_SYSDATA);
    return ret;
}

ehs_lorawan_api_errno_t LoRaWAN_get_sysData(char *sysData_out, char *DevEui_out)
{
    if (g_lorawan_cmd == E_LORAWAN_API__NOT_INTIALISED) return E_LWAPIERRNO_NOT_INITIALISED;
    if (sysData_out == NULL || DevEui_out == NULL) return E_LWAPIERRNO_INTERNAL_ERROR;

    if (g_lorawan_cmd != E_LORAWAN_API__IDLE && g_lorawan_cmd != E_LORAWAN_API__COMPLETE)
        return E_LWAPIERRNO_BUSY;

    #ifdef  EHS_TARGET_LORAWAN_THREADING_SUPPORT
    g_lorawan_api_data.get_sysdata.sysData = sysData_out;
    g_lorawan_api_data.get_sysdata.DevEui = DevEui_out;
    #endif//#ifdef EHS_TARGET_LORAWAN_THREADING_SUPPORT
    
    g_lorawan_cmd = E_LORAWAN_API_GET_SYSDATA;
    
    #ifndef  EHS_TARGET_LORAWAN_THREADING_SUPPORT
    return _LoRaWAN_get_sysData(sysData_out, DevEui_out);
    #endif//#ifndef EHS_TARGET_LORAWAN_THREADING_SUPPORT

    return E_LWAPIERRNO_OK;
}

static ehs_lorawan_api_errno_t _LoRaWAN_set_datarate(ehs_sint32 dr)
{
    ehs_lorawan_api_errno_t ret = E_LWAPIERRNO_OK;
    switch (gLorawanTarget) {
        case E_EHS_LWTARGET_WIO_E5:
            ret = LoRaWAN_wioe5_set_datarate(dr);
            break;
        default:
            ret = -100;
            break;
    }
    if (ret == E_LWAPIERRNO_OK) g_lorawan_cmd = E_LORAWAN_API__COMPLETE;
    gEhsLoraApiData.error_ret[E_LORAWAN_API_SET_DATARATE] = ret;
    Common_LoRaWAN_FBCBs(E_LORAWAN_API_SET_DATARATE);
    return ret;
}

ehs_lorawan_api_errno_t LoRaWAN_set_datarate(ehs_sint32 dr)
{
    if (g_lorawan_cmd == E_LORAWAN_API__NOT_INTIALISED) return E_LWAPIERRNO_NOT_INITIALISED;
    if (g_lorawan_cmd != E_LORAWAN_API__IDLE && g_lorawan_cmd != E_LORAWAN_API__COMPLETE)
        return E_LWAPIERRNO_BUSY;

    #ifdef  EHS_TARGET_LORAWAN_THREADING_SUPPORT
    g_lorawan_api_data.set_datarate.datarate = dr;
    #endif//#ifdef EHS_TARGET_LORAWAN_THREADING_SUPPORT
    
    g_lorawan_cmd = E_LORAWAN_API_SET_DATARATE;
    
    #ifndef  EHS_TARGET_LORAWAN_THREADING_SUPPORT
    return _LoRaWAN_set_datarate(dr);
    #endif//#ifndef EHS_TARGET_LORAWAN_THREADING_SUPPORT

    return E_LWAPIERRNO_OK;
}

static ehs_lorawan_api_errno_t _LoRaWAN_get_payloadLength(ehs_sint32 *length_out)
{
    if (length_out == NULL) return E_LWAPIERRNO_INTERNAL_ERROR;
    ehs_lorawan_api_errno_t ret = E_LWAPIERRNO_OK;
    switch (gLorawanTarget) {
        case E_EHS_LWTARGET_WIO_E5:
            ret = LoRaWAN_wioe5_get_payloadLength(length_out);
            break;
        default:
            ret = -100;
            break;
    }
    if (ret == E_LWAPIERRNO_OK) g_lorawan_cmd = E_LORAWAN_API__COMPLETE;
    gEhsLoraApiData.error_ret[E_LORAWAN_API_GET_PAYLOADLENGTH] = ret;
    Common_LoRaWAN_FBCBs(E_LORAWAN_API_GET_PAYLOADLENGTH);
    return ret;
}

ehs_lorawan_api_errno_t LoRaWAN_get_payloadLength(ehs_sint32 *length_out)
{
    if (g_lorawan_cmd == E_LORAWAN_API__NOT_INTIALISED) return E_LWAPIERRNO_NOT_INITIALISED;
    // NULL length_out pointer
    if (length_out == NULL) return E_LWAPIERRNO_INTERNAL_ERROR;

    if (g_lorawan_cmd != E_LORAWAN_API__IDLE && g_lorawan_cmd != E_LORAWAN_API__COMPLETE)
        return E_LWAPIERRNO_BUSY;

    #ifdef  EHS_TARGET_LORAWAN_THREADING_SUPPORT
    g_lorawan_api_data.get_payloadLength.length_out = length_out;
    #endif//#ifdef EHS_TARGET_LORAWAN_THREADING_SUPPORT

    g_lorawan_cmd = E_LORAWAN_API_GET_PAYLOADLENGTH;

    #ifndef  EHS_TARGET_LORAWAN_THREADING_SUPPORT
    return LoRaWAN_get_payloadLength(length);
    #endif//#ifndef EHS_TARGET_LORAWAN_THREADING_SUPPORT

    return E_LWAPIERRNO_OK;
}

static ehs_lorawan_api_errno_t _LoRaWAN_disable( void )
{
    ehs_lorawan_api_errno_t ret = E_LWAPIERRNO_OK;
    switch (gLorawanTarget) {
        case E_EHS_LWTARGET_WIO_E5:
            ret = LoRaWAN_wioe5_disable( );
            break;
        default:
            ret = -100;
            break;
    }
    if (ret == E_LWAPIERRNO_OK) g_lorawan_cmd = E_LORAWAN_API__COMPLETE;
    gEhsLoraApiData.error_ret[E_LORAWAN_API_DISABLE] = ret;
    Common_LoRaWAN_FBCBs(E_LORAWAN_API_DISABLE);
    return ret;
}

ehs_lorawan_api_errno_t LoRaWAN_disable( void )
{
    if (g_lorawan_cmd == E_LORAWAN_API__NOT_INTIALISED) return E_LWAPIERRNO_NOT_INITIALISED;
    if (g_lorawan_cmd != E_LORAWAN_API__IDLE && g_lorawan_cmd != E_LORAWAN_API__COMPLETE)
        return E_LWAPIERRNO_BUSY;

    #ifdef  EHS_TARGET_LORAWAN_THREADING_SUPPORT
    // Dummy code. Reserved for later
    ;
    #endif//#ifdef EHS_TARGET_LORAWAN_THREADING_SUPPORT

    g_lorawan_cmd = E_LORAWAN_API_DISABLE;
    
    #ifndef  EHS_TARGET_LORAWAN_THREADING_SUPPORT
    return _LoRaWAN_disable( );
    #endif//#ifndef EHS_TARGET_LORAWAN_THREADING_SUPPORT

    return E_LWAPIERRNO_OK;
}

// Only for targets with threading support
#ifdef  EHS_TARGET_LORAWAN_THREADING_SUPPORT
static void LoRaWAN_run_threadInLoop()
{
    switch (g_lorawan_cmd) {
        case E_LORAWAN_API__IDLE:
        case E_LORAWAN_API__COMPLETE:
        {
            // Do nothing when the status is idle or just completed a command
            g_lorawan_cmd = E_LORAWAN_API__IDLE;
            return;
            break;
        }
        case E_LORAWAN_API_CONNECT:
        {
            _LoRaWAN_connect(
                g_lorawan_api_data.connect.AppKey, 
                g_lorawan_api_data.connect.AppEui, 
                g_lorawan_api_data.connect.mode, 
                g_lorawan_api_data.connect.DevAddr_ABP, 
                g_lorawan_api_data.connect.AppSKey, 
                g_lorawan_api_data.connect.NwkSKey, 
                g_lorawan_api_data.connect.REPT, 
                g_lorawan_api_data.connect.RETRY, 
                g_lorawan_api_data.connect.region, 
                g_lorawan_api_data.connect.ADR, 
                g_lorawan_api_data.connect.DR, 
                g_lorawan_api_data.connect.autoJoin, 
                g_lorawan_api_data.connect.DevAddr_OUT
            );
            g_lorawan_cmd = E_LORAWAN_API__COMPLETE;
            break;
        }
        case E_LORAWAN_API_SEND_MSG:
        {
            _LoRaWAN_send_msg(
                g_lorawan_api_data.send_msg.payload, 
                g_lorawan_api_data.send_msg.fport,
                g_lorawan_api_data.send_msg.confirmed
            );
            g_lorawan_cmd = E_LORAWAN_API__COMPLETE;
            break;
        }
        case E_LORAWAN_API_RESET:
        {
            _LoRaWAN_reset();
            g_lorawan_cmd = E_LORAWAN_API__COMPLETE;
            break;
        }
        case E_LORAWAN_API_GET_SYSDATA:
        {
            _LoRaWAN_get_sysData(
                g_lorawan_api_data.get_sysdata.sysData, 
                g_lorawan_api_data.get_sysdata.DevEui
            );
            g_lorawan_cmd = E_LORAWAN_API__COMPLETE;
            break;
        }
        case E_LORAWAN_API_SET_DATARATE:
        {
            _LoRaWAN_set_datarate(
                g_lorawan_api_data.set_datarate.datarate
            );
            g_lorawan_cmd = E_LORAWAN_API__COMPLETE;
            break;
        }
        case E_LORAWAN_API_GET_PAYLOADLENGTH:
        {
            _LoRaWAN_get_payloadLength(
                g_lorawan_api_data.get_payloadLength.length_out
            );
            g_lorawan_cmd = E_LORAWAN_API__COMPLETE;
            break;
        }
        case E_LORAWAN_API_DISABLE:
        {
            _LoRaWAN_disable( );
            g_lorawan_cmd = E_LORAWAN_API__COMPLETE;
            break;
        }
        default:
        {
            g_lorawan_cmd = E_LORAWAN_API__IDLE;
            return;
            break;
        }
    }
}

static void taskLoRaWAN_execute_cmd( void *context )
{
    while (1) {
        LoRaWAN_run_threadInLoop();
        EhsSleep(EHS_TIME_ms(10));
    }
}
#endif//EHS_TARGET_LORAWAN_THREADING_SUPPORT