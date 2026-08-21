/***************************************************************
* Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license.
****************************************************************/

#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-ble_service.h"
//ICB HEADER MACRO END -- DO NOT ALTER

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* State type defined in inx-ble_service.h */
//ICB STATE VAR MACRO END -- DO NOT ALTER

/* Include HAL glue layer functions */
#include "inx-ble_service_hal_glue.h"

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(ble_service)
EHS_FB_FUNCTION_ENTRY("init", 0x01, ble_service_init)
EHS_FB_FUNCTION_ENTRY("start_adv", 0x02, ble_service_start_adv)
EHS_FB_FUNCTION_ENTRY("stop_adv", 0x03, ble_service_stop_adv)
EHS_FB_FUNCTION_ENTRY("write_char", 0x04, ble_service_write_char)
EHS_FB_FUNCTION_ENTRY("read_char", 0x05, ble_service_read_char)
EHS_FB_FUNCTION_ENTRY("notify_char", 0x06, ble_service_notify_char)
EHS_FB_FUNCTION_ENTRY("on_client_write", 0x07, ble_service_on_client_write)
EHS_FB_FUNCTION_ENTRY("on_connect", 0x08, ble_service_on_connect)
EHS_FB_FUNCTION_ENTRY("on_disconnect", 0x09, ble_service_on_disconnect)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER

//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Per-port argument macros. Numbers come from ble_service.cdf <Function argument="N">.
 * OutputPorts, DataInputs and FinishPorts each have their own argument-number
 * space inside a function, so the same N can appear in more than one row. */

/* init */
#define INX_ble_service_ARG_init_status      1  /* OutputPort/I */
#define INX_ble_service_ARG_init_init_done   1  /* FinishPort */
#define INX_ble_service_ARG_init_init_error  2  /* FinishPort */

/* start_adv */
#define INX_ble_service_ARG_start_adv_adv_started  1  /* FinishPort */
#define INX_ble_service_ARG_start_adv_adv_error    2  /* FinishPort */

/* stop_adv */
#define INX_ble_service_ARG_stop_adv_adv_stopped   1  /* FinishPort */

/* write_char */
#define INX_ble_service_ARG_write_char_char_idx      1  /* DataInput/I */
#define INX_ble_service_ARG_write_char_data          2  /* DataInput/S */
#define INX_ble_service_ARG_write_char_length        3  /* DataInput/I */
#define INX_ble_service_ARG_write_char_write_status  1  /* OutputPort/I */
#define INX_ble_service_ARG_write_char_write_done    1  /* FinishPort */
#define INX_ble_service_ARG_write_char_write_error   2  /* FinishPort */

/* read_char */
#define INX_ble_service_ARG_read_char_read_idx     1  /* DataInput/I */
#define INX_ble_service_ARG_read_char_read_data    1  /* OutputPort/S */
#define INX_ble_service_ARG_read_char_read_length  2  /* OutputPort/I */
#define INX_ble_service_ARG_read_char_read_status  3  /* OutputPort/I */
#define INX_ble_service_ARG_read_char_read_done    1  /* FinishPort */
#define INX_ble_service_ARG_read_char_read_error   2  /* FinishPort */

/* notify_char */
#define INX_ble_service_ARG_notify_char_notify_idx     1  /* DataInput/I */
#define INX_ble_service_ARG_notify_char_notify_data    2  /* DataInput/S */
#define INX_ble_service_ARG_notify_char_notify_len     3  /* DataInput/I */
#define INX_ble_service_ARG_notify_char_notify_status  1  /* OutputPort/I */
#define INX_ble_service_ARG_notify_char_notify_sent    1  /* FinishPort */
#define INX_ble_service_ARG_notify_char_notify_error   2  /* FinishPort */

/* on_client_write */
#define INX_ble_service_ARG_on_client_write_wrote_idx         1  /* OutputPort/I */
#define INX_ble_service_ARG_on_client_write_wrote_data        2  /* OutputPort/S */
#define INX_ble_service_ARG_on_client_write_wrote_len         3  /* OutputPort/I */
#define INX_ble_service_ARG_on_client_write_client_wrote_evt  1  /* FinishPort */

/* on_connect */
#define INX_ble_service_ARG_on_connect_conn_handle  1  /* OutputPort/I */
#define INX_ble_service_ARG_on_connect_connect_evt  1  /* FinishPort */

/* on_disconnect */
#define INX_ble_service_ARG_on_disconnect_disc_handle     1  /* OutputPort/I */
#define INX_ble_service_ARG_on_disconnect_disc_reason     2  /* OutputPort/I */
#define INX_ble_service_ARG_on_disconnect_disconnect_evt  1  /* FinishPort */

//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER

/* todo - this should go in the BLE common hal so it can be used in the HAL */
typedef enum  {
    INX_BLE_SERVICE_CONFIG_ERROR,
    INX_BLE_SERVICE_CONFIG_PARSED_OK,
    INX_BLE_SERVICE_INITIALISED_OK,
    INX_BLE_SERVICE_RUNNING,
    INX_BLE_SERVICE_STOPPED
} inxBleServicesStatus_e;

/* Service function block */
typedef struct
{
    /* Service parameters */
    inx_ble_uuid_t service_uuid;
    ehs_char       service_name[INX_BLE_NAME_MAX + 1];
    ehs_sint32     num_chars;
    ehs_sint32     adv_interval_ms;
    
    /* Characteristic configurations — indexed 0..num_chars-1 */
    inx_ble_char_config_t chars[INX_BLE_SERVICE_MAX_CHARACTERSTICS];

    /* Service State*/
    inxBleServicesStatus_e status;

    /* Callback data storage */
    ehs_uint8  cb_char_idx;
    ehs_char   cb_data[INX_BLE_CALLBACK_OBJECT_SIZE_MAX];
    ehs_uint16 cb_data_len;
    ehs_uint16 cb_conn_handle;
    ehs_uint8  cb_reason;

    /* Per-callback EhsFunctionInstanceDataType*. Each EHS function has its own
     * pIn/pOut/pFinishPort — captured in EHS_FB_INIT_FUNCTION from
     * pCallbackTable (one slot per CDF InternalPort, document order). */
    void*      pFI_on_client_write;
    void*      pFI_on_connect;
    void*      pFI_on_disconnect;
} inx_ble_service_state_type;



/* Convert a hex nibble character to 0-15. Returns 0xff on bad input. */
static ehs_uint8 hex_nibble(ehs_char c)
{
    if (c >= '0' && c <= '9') return (ehs_uint8)(c - '0');
    if (c >= 'a' && c <= 'f') return (ehs_uint8)(c - 'a' + 10);
    if (c >= 'A' && c <= 'F') return (ehs_uint8)(c - 'A' + 10);
    return 0xff;
}

/* Convert a 2-char hex pair at s to a byte. Returns EHS_FALSE if invalid. */
static ehs_bool hex_byte(const ehs_char* s, ehs_uint8* out)
{
    ehs_uint8 hi = hex_nibble(s[0]);
    ehs_uint8 lo = hex_nibble(s[1]);
    if (hi == 0xff || lo == 0xff) return EHS_FALSE;
    *out = (ehs_uint8)((hi << 4) | lo);
    return EHS_TRUE;
}

/* Parse a UUID string into the binary inx_ble_uuid_t representation.
 * Accepts:
 *   4-char  "180A"                                  → 16-bit UUID - only for built in services.
 *   36-char "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"  → 128-bit UUID
 * Bytes in u128 are stored in network (big-endian) order.
 * Returns EHS_FALSE and leaves *out unchanged on bad input. */
static ehs_bool ble_uuid_parse_str(const ehs_char* str, inx_ble_uuid_t* out)
{
    if (!str || !out) return EHS_FALSE;

    ehs_sint32 len = 0;
    while (str[len]) len++;

    if (len == 4) {
        ehs_uint8 hi, lo;
        if (!hex_byte(str, &hi) || !hex_byte(str + 2, &lo)) return EHS_FALSE;
        out->type = INX_BLE_UUID_TYPE_16;
        out->value.u16 = (ehs_uint16)((hi << 8) | lo);
        return EHS_TRUE;
    }

    if (len == 36) {
        /* Offsets of each byte pair in "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX" */
        static const ehs_uint8 kOff[16] = {0,2,4,6,9,11,14,16,19,21,24,26,28,30,32,34};
        ehs_uint8 bytes[16];
        for (ehs_sint32 i = 0; i < 16; i++) {
            if (!hex_byte(str + kOff[i], &bytes[i])) return EHS_FALSE;
        }
        out->type = INX_BLE_UUID_TYPE_128;
        for (ehs_sint32 i = 0; i < 16; i++) out->value.u128[i] = bytes[i];
        return EHS_TRUE;
    }

    return EHS_FALSE;
}

#define EHS_BLE_DEBUG
#ifdef EHS_BLE_DEBUG
/* Print a UUID via the logger for debug. */
static void print_uuid(const ehs_char* label, const inx_ble_uuid_t* uuid)
{
    if (uuid->type == INX_BLE_UUID_TYPE_16) {
        printf("%s = 16-bit:0x%04x\n", label, (unsigned)uuid->value.u16);
    } else if (uuid->type == INX_BLE_UUID_TYPE_128) {
        const ehs_uint8* b = uuid->value.u128;
        printf("%s = 128-bit:%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x\n",
               label,
               b[0],b[1],b[2],b[3],b[4],b[5],b[6],b[7],
               b[8],b[9],b[10],b[11],b[12],b[13],b[14],b[15]);
    } else {
        printf("%s = (unset)\n", label);
    }
}

/* Print a full inx_ble_char_config_t via the logger for debug. */
static void print_char_config(ehs_sint32 idx, const inx_ble_char_config_t* c)
{
    if (c->uuid.type == INX_BLE_UUID_TYPE_16) {
        EHSH_LOG_INFO("  char[%d]: name=%-16s props=0x%02x max_len=%d uuid=16-bit:0x%04x\n",
               idx, c->name, (unsigned)c->properties, c->max_len, (unsigned)c->uuid.value.u16);
    } else if (c->uuid.type == INX_BLE_UUID_TYPE_128) {
        const ehs_uint8* b = c->uuid.value.u128;
        EHSH_LOG_INFO("  char[%d]: name=%-16s props=0x%02x max_len=%d uuid=128-bit:%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x\n",
               idx, c->name, (unsigned)c->properties, c->max_len,
               b[0],b[1],b[2],b[3],b[4],b[5],b[6],b[7],
               b[8],b[9],b[10],b[11],b[12],b[13],b[14],b[15]);
    } else {
        EHSH_LOG_INFO("  char[%d]: name=%-16s props=0x%02x max_len=%d uuid=(unset)\n",
               idx, c->name, (unsigned)c->properties, c->max_len);
    }
}
#endif

/* EHS-aware BLE event callbacks — bridge between the HAL's plain-C callbacks
 * and the EHS function block event dispatch.
 * ctx is the FB state pointer; each bridge picks the matching per-callback pFI
 * captured in EHS_FB_INIT_FUNCTION. The HAL never dereferences ctx. */

static void ble_fb_on_client_write(void* ctx, ehs_uint8 char_idx,
                                   const char* data, ehs_uint16 length)
{
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)ctx;
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    state->cb_char_idx = char_idx;
    state->cb_data_len = (length < (ehs_uint16)sizeof(state->cb_data))
                         ? length : (ehs_uint16)sizeof(state->cb_data);
    if (data && state->cb_data_len > 0)
        EhsMemcpy(state->cb_data, data, state->cb_data_len);
    EhsRunble_service_on_client_write((EhsFunctionInstanceDataType*)state->pFI_on_client_write);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
}

static void ble_fb_on_connect(void* ctx, ehs_uint16 conn_handle)
{
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)ctx;
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    state->cb_conn_handle = conn_handle;
    EhsRunble_service_on_connect((EhsFunctionInstanceDataType*)state->pFI_on_connect);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
}

static void ble_fb_on_disconnect(void* ctx, ehs_uint16 conn_handle, ehs_uint8 reason)
{
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)ctx;
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    state->cb_conn_handle = conn_handle;
    state->cb_reason = reason;
    EhsRunble_service_on_disconnect((EhsFunctionInstanceDataType*)state->pFI_on_disconnect);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
}

//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(ble_service)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_ble_service_state_type);
}
//ICB IDENTIFY FUNCTION MACRO END -- DO NOT ALTER

//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ble_service)
{
    ehs_bool bRet = EHS_TRUE;
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)EHS_FB_INIT_CONTEXT;
    const char* pParams = EHS_FB_INIT_PARAMETERS;
    ehs_char uuid_str[INX_BLE_SERVICE_UUID_STR_MAX + 1];

    /* Per-callback FB-instance pointers. The kernel hands the FB-init a
     * pCallbackTable that points to the LAST callback slot belonging to this
     * FB (identify.c pre-increments callBackIndex then passes the post-loop
     * value, and InitialiseObj does &pFunctions[that_index] — see also the
     * cross-FB drift TODO in repo CLAUDE.md). So pCallbackTable[0] is the
     * last <InternalPort> in CDF document order; earlier callbacks are at
     * negative offsets.
     *
     * CDF order in ble_service.cdf:
     *   1st <InternalPort> = client_wrote  (on_client_write)  -> pCT[-2]
     *   2nd <InternalPort> = connect       (on_connect)       -> pCT[-1]
     *   3rd <InternalPort> = disconnect    (on_disconnect)    -> pCT[ 0]
     *
     * The macro is &(pCallbackTable[-1-x]), so the x value below is
     * (-1 - desired_offset). Re-derive these if the CDF InternalPort order
     * changes or another InternalPort is inserted. */
    state->pFI_on_client_write = (void*)EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(1);   /* &pCT[-2] */
    state->pFI_on_connect      = (void*)EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(0);   /* &pCT[-1] */
    state->pFI_on_disconnect   = (void*)EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(-1);  /* &pCT[ 0] */

    state->status  = INX_BLE_SERVICE_CONFIG_ERROR;

    pParams = EhsGetWordFromString(uuid_str, pParams, sizeof(uuid_str));
    if (!ble_uuid_parse_str(uuid_str, &state->service_uuid)) {
        bRet = EHS_FALSE;
#ifdef EHS_BLE_DEBUG
            printf ("Error with Service [%s]\n",uuid_str );
#endif
    }
    pParams = EhsGetWordFromString(state->service_name,   pParams, sizeof(state->service_name));
    pParams = EhsGetSint32FromString(&state->num_chars,       pParams);
    pParams = EhsGetSint32FromString(&state->adv_interval_ms, pParams);
    // todo check for premature null returns and set  a flag to return an init fatal error.
    for (ehs_sint32 i = 0; i < INX_BLE_SERVICE_MAX_CHARACTERSTICS; i++) {
        pParams = EhsGetWordFromString(uuid_str, pParams, sizeof(uuid_str));
        if (!ble_uuid_parse_str(uuid_str, &state->chars[i].uuid)) {
            bRet = EHS_FALSE;
#ifdef EHS_BLE_DEBUG
            printf ("Error with Charactersitic [%s]\n",uuid_str );
#endif
        }
        pParams = EhsGetWordFromString(state->chars[i].name, pParams, sizeof(state->chars[i].name));
        pParams = EhsGetSint32FromString(&state->chars[i].properties, pParams);
        pParams = EhsGetSint32FromString(&state->chars[i].max_len,    pParams);
    }

    if (bRet) state->status = INX_BLE_SERVICE_INITIALISED_OK;
#ifdef EHS_BLE_DEBUG
    print_uuid("service_uuid", &state->service_uuid);
    EHSH_LOG_INFO("service_name=%s num_chars=%d adv_interval_ms=%d",
                  state->service_name, state->num_chars, state->adv_interval_ms);
    for (ehs_sint32 i = 0; i < INX_BLE_SERVICE_MAX_CHARACTERSTICS; i++) {
        print_char_config(i, &state->chars[i]);
    }
#endif
    return EHS_TRUE;// We would only return false if the paramters are corrupt - i.e. fatal
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER

//ICB FUNCTION init MACRO START -- DO NOT ALTER
/**
 * Definition of ble_service_init.
 * This function can access the object data using:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ble_service_init)
{
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)EHS_FB_RUN_CONTEXT;

    ehs_uint8 num_chars = (ehs_uint8)state->num_chars;
    if (num_chars > INX_BLE_SERVICE_MAX_CHARACTERSTICS)
        num_chars = INX_BLE_SERVICE_MAX_CHARACTERSTICS;

    inx_ble_service_callbacks_t callbacks = {
        .on_client_write = ble_fb_on_client_write,
        .on_connect      = ble_fb_on_connect,
        .on_disconnect   = ble_fb_on_disconnect,
    };

    ehs_sint32 rc = inx_ble_service_hal_glue_init(
        &state->service_uuid,
        state->service_name,
        num_chars,
        (ehs_uint32)state->adv_interval_ms,
        state->chars,
        &callbacks,
        state);  /* HAL forwards as-is to ble_fb_on_* bridges */

    if (EHS_FB_OUT_CONNECTED_API2(INX_ble_service_ARG_init_status))
        EHS_FB_OUT_I_API2(INX_ble_service_ARG_init_status) = rc;

    //printf("[BLE_FB] init: rc=%d, firing %s\n", rc,
    //       (rc == 0) ? "init_done" : "init_error");
    if (rc == 0)
        EHS_FB_FINISH(INX_ble_service_ARG_init_init_done);
    else
        EHS_FB_FINISH(INX_ble_service_ARG_init_init_error);
}
//ICB FUNCTION init MACRO END -- DO NOT ALTER

//ICB FUNCTION start_adv MACRO START -- DO NOT ALTER
/**
 * Definition of ble_service_start_adv.
 * This function can access the object data using:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ble_service_start_adv)
{
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)EHS_FB_RUN_CONTEXT;

    //printf("[BLE_FB] start_adv: called\n");
    /* Start BLE advertising */
    int rc = inx_ble_service_hal_glue_start_adv();

    /* Trigger appropriate finish event */
    if (rc == 0) {
        EHS_FB_FINISH(INX_ble_service_ARG_start_adv_adv_started);
    } else {
        EHS_FB_FINISH(INX_ble_service_ARG_start_adv_adv_error);
    }
}
//ICB FUNCTION start_adv MACRO END -- DO NOT ALTER

//ICB FUNCTION stop_adv MACRO START -- DO NOT ALTER
/**
 * Definition of ble_service_stop_adv.
 * This function can access the object data using:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ble_service_stop_adv)
{
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)EHS_FB_RUN_CONTEXT;

    /* Stop BLE advertising */
    int rc = inx_ble_service_hal_glue_stop_adv();

    /* Trigger finish event (only adv_stopped, no error event in CDF) */
    EHS_FB_FINISH(INX_ble_service_ARG_stop_adv_adv_stopped);
}
//ICB FUNCTION stop_adv MACRO END -- DO NOT ALTER

//ICB FUNCTION write_char MACRO START -- DO NOT ALTER
/**
 * Definition of ble_service_write_char.
 * This function can access the object data using:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ble_service_write_char)
{
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)EHS_FB_RUN_CONTEXT;

    /* Read input ports */
    uint8_t char_idx = 0;
    const char* data = NULL;
    uint16_t length = 0;

    if (EHS_FB_IN_CONNECTED_API2(INX_ble_service_ARG_write_char_char_idx))
    {
        char_idx = (uint8_t)EHS_FB_IN_I_API2(INX_ble_service_ARG_write_char_char_idx);
    }

    if (EHS_FB_IN_CONNECTED_API2(INX_ble_service_ARG_write_char_data))
    {
        data = EHS_FB_IN_S_API2(INX_ble_service_ARG_write_char_data);
    }

    if (EHS_FB_IN_CONNECTED_API2(INX_ble_service_ARG_write_char_length))
    {
        length = (uint16_t)EHS_FB_IN_I_API2(INX_ble_service_ARG_write_char_length);
    }

    /* Write to characteristic */
    int rc = inx_ble_service_hal_glue_write_char(char_idx, data, length);

    /* Write status output port */
    if (EHS_FB_OUT_CONNECTED_API2(INX_ble_service_ARG_write_char_write_status))
    {
        EHS_FB_OUT_I_API2(INX_ble_service_ARG_write_char_write_status) = rc;
    }

    /* Trigger appropriate finish event */
    if (rc == 0) {
        EHS_FB_FINISH(INX_ble_service_ARG_write_char_write_done);
    } else {
        EHS_FB_FINISH(INX_ble_service_ARG_write_char_write_error);
    }
}
//ICB FUNCTION write_char MACRO END -- DO NOT ALTER

//ICB FUNCTION read_char MACRO START -- DO NOT ALTER
/**
 * Definition of ble_service_read_char.
 * This function can access the object data using:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ble_service_read_char)
{
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)EHS_FB_RUN_CONTEXT;

    /* Read input port */
    uint8_t char_idx = 0;
    if (EHS_FB_IN_CONNECTED_API2(INX_ble_service_ARG_read_char_read_idx))
    {
        char_idx = (uint8_t)EHS_FB_IN_I_API2(INX_ble_service_ARG_read_char_read_idx);
    }

    /* Read from characteristic */
    char read_buffer[512];
    uint16_t read_length = 0;
    int rc = inx_ble_service_hal_glue_read_char(char_idx, read_buffer, &read_length, sizeof(read_buffer));

    /* Write output ports */
    if (EHS_FB_OUT_CONNECTED_API2(INX_ble_service_ARG_read_char_read_data))
    {
        char* out_str = EHS_FB_OUT_S_API2(INX_ble_service_ARG_read_char_read_data);
        if (out_str && rc == 0) {
            EhsMemcpy(out_str, read_buffer, read_length);
            out_str[read_length] = '\0';
        }
    }

    if (EHS_FB_OUT_CONNECTED_API2(INX_ble_service_ARG_read_char_read_length))
    {
        EHS_FB_OUT_I_API2(INX_ble_service_ARG_read_char_read_length) = read_length;
    }

    if (EHS_FB_OUT_CONNECTED_API2(INX_ble_service_ARG_read_char_read_status))
    {
        EHS_FB_OUT_I_API2(INX_ble_service_ARG_read_char_read_status) = rc;
    }

    /* Trigger appropriate finish event */
    if (rc == 0) {
        EHS_FB_FINISH(INX_ble_service_ARG_read_char_read_done);
    } else {
        EHS_FB_FINISH(INX_ble_service_ARG_read_char_read_error);
    }
}
//ICB FUNCTION read_char MACRO END -- DO NOT ALTER

//ICB FUNCTION notify_char MACRO START -- DO NOT ALTER
/**
 * Definition of ble_service_notify_char.
 * This function can access the object data using:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ble_service_notify_char)
{
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)EHS_FB_RUN_CONTEXT;

    /* Read input ports */
    uint8_t char_idx = 0;
    const char* data = NULL;
    uint16_t length = 0;

    if (EHS_FB_IN_CONNECTED_API2(INX_ble_service_ARG_notify_char_notify_idx))
    {
        char_idx = (uint8_t)EHS_FB_IN_I_API2(INX_ble_service_ARG_notify_char_notify_idx);
    }

    if (EHS_FB_IN_CONNECTED_API2(INX_ble_service_ARG_notify_char_notify_data))
    {
        data = EHS_FB_IN_S_API2(INX_ble_service_ARG_notify_char_notify_data);
    }

    if (EHS_FB_IN_CONNECTED_API2(INX_ble_service_ARG_notify_char_notify_len))
    {
        length = (uint16_t)EHS_FB_IN_I_API2(INX_ble_service_ARG_notify_char_notify_len);
    }

    /* Send notification */
    int rc = inx_ble_service_hal_glue_notify(char_idx, data, length);

    /* Write status output port */
    if (EHS_FB_OUT_CONNECTED_API2(INX_ble_service_ARG_notify_char_notify_status))
    {
        EHS_FB_OUT_I_API2(INX_ble_service_ARG_notify_char_notify_status) = rc;
    }

    /* Trigger appropriate finish event */
    if (rc == 0) {
        EHS_FB_FINISH(INX_ble_service_ARG_notify_char_notify_sent);
    } else {
        EHS_FB_FINISH(INX_ble_service_ARG_notify_char_notify_error);
    }
}
//ICB FUNCTION notify_char MACRO END -- DO NOT ALTER

//ICB FUNCTION on_client_write MACRO START -- DO NOT ALTER
/**
 * Definition of ble_service_on_client_write.
 * INTERNAL EVENT: This function can be called from external sources (ISR, threads, etc.)
 * This function can access the object data using:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ble_service_on_client_write)
{
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)EHS_FB_RUN_CONTEXT;

    /* Populate output ports from callback data stored in state */
    if (EHS_FB_OUT_CONNECTED_API2(INX_ble_service_ARG_on_client_write_wrote_idx)) {
        EHS_FB_OUT_I_API2(INX_ble_service_ARG_on_client_write_wrote_idx) = (ehs_sint32)state->cb_char_idx;
    }

    if (EHS_FB_OUT_CONNECTED_API2(INX_ble_service_ARG_on_client_write_wrote_data)) {
        ehs_char* out_str = EHS_FB_OUT_S_API2(INX_ble_service_ARG_on_client_write_wrote_data);
        if (out_str) {
            ehs_uint16 len = state->cb_data_len < EHS_STRING_LENGTH_MAX ? state->cb_data_len : EHS_STRING_LENGTH_MAX - 1;
            EhsMemcpy(out_str, state->cb_data, len);
            out_str[len] = '\0';
        }
    }

    if (EHS_FB_OUT_CONNECTED_API2(INX_ble_service_ARG_on_client_write_wrote_len)) {
        EHS_FB_OUT_I_API2(INX_ble_service_ARG_on_client_write_wrote_len) = (ehs_sint32)state->cb_data_len;
    }

    /* Trigger finish event */
    EHS_FB_FINISH(INX_ble_service_ARG_on_client_write_client_wrote_evt);
}
//ICB FUNCTION on_client_write MACRO END -- DO NOT ALTER

//ICB FUNCTION on_connect MACRO START -- DO NOT ALTER
/**
 * Definition of ble_service_on_connect.
 * INTERNAL EVENT: This function can be called from external sources (ISR, threads, etc.)
 * This function can access the object data using:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ble_service_on_connect)
{
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)EHS_FB_RUN_CONTEXT;

    /* Populate output ports from callback data stored in state */
    if (EHS_FB_OUT_CONNECTED_API2(INX_ble_service_ARG_on_connect_conn_handle)) {
        EHS_FB_OUT_I_API2(INX_ble_service_ARG_on_connect_conn_handle) = (ehs_sint32)state->cb_conn_handle;
    }

    /* Trigger finish event */
    EHS_FB_FINISH(INX_ble_service_ARG_on_connect_connect_evt);
}
//ICB FUNCTION on_connect MACRO END -- DO NOT ALTER

//ICB FUNCTION on_disconnect MACRO START -- DO NOT ALTER
/**
 * Definition of ble_service_on_disconnect.
 * INTERNAL EVENT: This function can be called from external sources (ISR, threads, etc.)
 * This function can access the object data using:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ble_service_on_disconnect)
{
    inx_ble_service_state_type* state = (inx_ble_service_state_type*)EHS_FB_RUN_CONTEXT;

    /* Populate output ports from callback data stored in state */
    if (EHS_FB_OUT_CONNECTED_API2(INX_ble_service_ARG_on_disconnect_disc_handle)) {
        EHS_FB_OUT_I_API2(INX_ble_service_ARG_on_disconnect_disc_handle) = (ehs_sint32)state->cb_conn_handle;
    }

    if (EHS_FB_OUT_CONNECTED_API2(INX_ble_service_ARG_on_disconnect_disc_reason)) {
        EHS_FB_OUT_I_API2(INX_ble_service_ARG_on_disconnect_disc_reason) = (ehs_sint32)state->cb_reason;
    }

    /* Trigger finish event */
    EHS_FB_FINISH(INX_ble_service_ARG_on_disconnect_disconnect_evt);
}
//ICB FUNCTION on_disconnect MACRO END -- DO NOT ALTER

//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Destroy the function block. Clean up any resources.
 */
EHS_FB_DESTROY_FUNCTION(ble_service)
{
    // inx_ble_service_state_type* state = (inx_ble_service_state_type*)EHS_FB_DESTROY_CONTEXT;

    /* Deinitialize HAL */
    inx_ble_service_hal_glue_deinit();

    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER
