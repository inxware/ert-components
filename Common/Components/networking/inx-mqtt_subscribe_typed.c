/***************************************************************
* Copyright (C) 2008-2025 inx limited - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license.
****************************************************************/

#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-mqtt_subscribe_typed.h"
#include "hal_mqtt.h"
//ICB HEADER MACRO END -- DO NOT ALTER

#include "hal_string.h"

/* implemented in inx-mqtt_client.c */
void handle_mqtt_param_string(ehs_char* str, ehs_uint32 size);

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
EHS_FB_FUNCTIONS_START(mqtt_subscribe_typed)
EHS_FB_FUNCTION_ENTRY("subscribe",   0x01, mqtt_subscribe_typed_subscribe)
EHS_FB_FUNCTION_ENTRY("unsubscribe", 0x02, mqtt_subscribe_typed_unsubscribe)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER

//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
#define INX_mqtt_subscribe_typed_ARG_subscribe_topic       1
#define INX_mqtt_subscribe_typed_ARG_subscribe_qos         2
#define INX_mqtt_subscribe_typed_ARG_subscribe_done        1
#define INX_mqtt_subscribe_typed_ARG_subscribe_received    2
#define INX_mqtt_subscribe_typed_ARG_subscribe_value_i     1
#define INX_mqtt_subscribe_typed_ARG_subscribe_value_f     2
#define INX_mqtt_subscribe_typed_ARG_subscribe_value_b     3
#define INX_mqtt_subscribe_typed_ARG_unsubscribe_done      1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER

/* ---------------------------------------------------------------------------
 * Length-bounded payload parsing
 *
 * The HAL hands the payload over as a pointer into its own receive buffer plus
 * an explicit length — it is NOT null terminated (only the
 * arduino backend happens to terminate its buffer) . Nothing from the sscanf or
 * strcmp family may therefore be pointed at it. The untyped mqtt_subscribe block 
 * fixes this by copying into its string
 * output slot and terminating there. 
 * 
 * The helpers below read at most `len` bytes and need no buffer.
 * ------------------------------------------------------------------------ */

/* Largest power of ten worth constructing. Past the float type's range the
 * result saturates whatever we do, and this keeps the scaling loop bounded.
 * The condition mirrors the ehs_float typedef in <base>/base_types.h. */
#if EHS_FLOAT_AS_FLOAT_TYPE == 1
#define INX_MQTT_TYPED_EXP10_MAX 38
#else
#define INX_MQTT_TYPED_EXP10_MAX 308
#endif

/* Whole-payload comparison against a literal. The null-terminated original
 * used EhsStrcmp, so a match has to span the entire payload, not a prefix. */
static ehs_bool payload_equals(const ehs_char* p, ehs_sint32 len,
                               const ehs_char* literal, ehs_sint32 literalLen)
{
    return ((len == literalLen) && (EhsStrncmp(p, literal, literalLen) == 0)) ? EHS_TRUE : EHS_FALSE;
}

/* Parse "[space][+|-]digits[.digits][(e|E)[+|-]digits]" out of a bounded,
 * non-terminated buffer.
 *
 * *pIval takes the integer part alone, truncated toward zero — what
 * sscanf("%d") yielded before — but saturating at the ehs_sint32 limits rather
 * than overflowing, which sscanf leaves undefined.
 * *pFval takes the full value.
 * Both stay at 0 when the payload carries no digits, matching what the
 * previous sscanf pair produced for a non-numeric payload. */
static void payload_parse_number(const ehs_char* p, ehs_sint32 len,
                                 ehs_sint32* pIval, ehs_float* pFval)
{
    ehs_sint32 i             = 0;
    ehs_bool   negative      = EHS_FALSE;
    ehs_bool   anyDigit      = EHS_FALSE;
    ehs_uint32 ipart         = 0u;
    ehs_bool   ipartSaturated = EHS_FALSE;
    ehs_float  mantissa      = (ehs_float)0.0;
    ehs_sint32 exp10         = 0;  /* power of ten still owed to the mantissa */

    *pIval = 0;
    *pFval = (ehs_float)0.0;

    while ((i < len) && EhsStrIsSpace(p[i])) { i++; }

    if ((i < len) && ((p[i] == '+') || (p[i] == '-')))
    {
        negative = (p[i] == '-') ? EHS_TRUE : EHS_FALSE;
        i++;
    }

    while ((i < len) && (p[i] >= '0') && (p[i] <= '9'))
    {
        ehs_uint32 digit = (ehs_uint32)(p[i] - '0');
        anyDigit = EHS_TRUE;
        if (ipart > ((EHS_UINT32_MAX - digit) / 10u)) { ipartSaturated = EHS_TRUE; }
        else                                         { ipart = (ipart * 10u) + digit; }
        mantissa = (mantissa * (ehs_float)10.0) + (ehs_float)digit;
        i++;
    }

    if ((i < len) && (p[i] == '.'))
    {
        i++;
        while ((i < len) && (p[i] >= '0') && (p[i] <= '9'))
        {
            anyDigit = EHS_TRUE;
            mantissa = (mantissa * (ehs_float)10.0) + (ehs_float)(p[i] - '0');
            exp10--;
            i++;
        }
    }

    if (anyDigit == EHS_FALSE) { return; }

    if ((i < len) && ((p[i] == 'e') || (p[i] == 'E')))
    {
        ehs_sint32 j           = i + 1;
        ehs_bool   expNegative = EHS_FALSE;
        ehs_sint32 expDigits   = 0;
        ehs_sint32 expValue    = 0;

        if ((j < len) && ((p[j] == '+') || (p[j] == '-')))
        {
            expNegative = (p[j] == '-') ? EHS_TRUE : EHS_FALSE;
            j++;
        }
        while ((j < len) && (p[j] >= '0') && (p[j] <= '9'))
        {
            /* Saturate rather than stop accumulating — freezing part way
             * through a multi-digit exponent turns "e-400" into "e-40" and
             * yields a plausible-looking wrong number. Saturating past twice
             * the type's decimal range lets the scaling below drive the result
             * to zero or infinity, which is what such an exponent means. */
            if (expValue < (2 * INX_MQTT_TYPED_EXP10_MAX)) { expValue = (expValue * 10) + (p[j] - '0'); }
            else                                           { expValue = 2 * INX_MQTT_TYPED_EXP10_MAX; }
            expDigits++;
            j++;
        }
        /* A trailing "e" with no digits is not an exponent; sscanf backs off
         * the same way and keeps the mantissa it already has. */
        if (expDigits > 0) { exp10 += (expNegative == EHS_TRUE) ? -expValue : expValue; }
    }

    /* Build 10^|exp10| by repeated multiplication — exact for every magnitude
     * the float type represents exactly — then apply it in one operation, so
     * the result carries a single rounding rather than one per digit.
     * Exponents past the type's decimal range are split into two chunks so the
     * value saturates naturally (to zero or to infinity) instead of clamping
     * the scale and handing back a wrong finite number. */
    {
        ehs_sint32 n = (exp10 < 0) ? -exp10 : exp10;

        if (n > (2 * INX_MQTT_TYPED_EXP10_MAX)) { n = 2 * INX_MQTT_TYPED_EXP10_MAX; }
        while (n > 0)
        {
            ehs_sint32 chunk = (n > INX_MQTT_TYPED_EXP10_MAX) ? INX_MQTT_TYPED_EXP10_MAX : n;
            ehs_float  scale = (ehs_float)1.0;
            ehs_sint32 k     = chunk;

            while (k > 0) { scale = scale * (ehs_float)10.0; k--; }
            mantissa = (exp10 < 0) ? (mantissa / scale) : (mantissa * scale);
            n -= chunk;
        }
    }

    *pFval = (negative == EHS_TRUE) ? -mantissa : mantissa;

    if ((ipartSaturated == EHS_TRUE) || (ipart > (ehs_uint32)EHS_SINT32_MAX))
    {
        *pIval = (negative == EHS_TRUE) ? EHS_SINT32_MIN : EHS_SINT32_MAX;
    }
    else
    {
        *pIval = (negative == EHS_TRUE) ? -(ehs_sint32)ipart : (ehs_sint32)ipart;
    }
}

/* Parse a received MQTT payload string into typed values and fire "received". */
static ehs_bool mqtt_subscribe_typed_callback(struct inx_mqtt_subscribe_state* pState,
                                              char* payload, ehs_sint32 payloadSize)
{
    if (pState == NULL || pState->pFIdata == NULL || payload == NULL) return EHS_FALSE;
    if (payloadSize < 0) return EHS_FALSE;

    EhsFunctionInstanceDataType* pFIdata = (EhsFunctionInstanceDataType*)pState->pFIdata;

    /* Integer and float in one bounded pass over the payload */
    ehs_sint32 ival = 0;
    ehs_float  fval = (ehs_float)0.0;
    payload_parse_number(payload, payloadSize, &ival, &fval);

    /* Boolean: "true"/"1"/non-zero = true; "false"/"0" = false */
    ehs_bool bval;
    if (payload_equals(payload, payloadSize, "true", 4) ||
        payload_equals(payload, payloadSize, "True", 4) ||
        payload_equals(payload, payloadSize, "TRUE", 4)) {
        bval = EHS_TRUE;
    } else if (payload_equals(payload, payloadSize, "false", 5) ||
               payload_equals(payload, payloadSize, "False", 5) ||
               payload_equals(payload, payloadSize, "FALSE", 5)) {
        bval = EHS_FALSE;
    } else {
        bval = (ival != 0) ? EHS_TRUE : EHS_FALSE;
    }

    EhsTPMutex_lock(EhsTPMutex_fbIO);
    EHS_FB_OUT_I_API2(INX_mqtt_subscribe_typed_ARG_subscribe_value_i) = ival;
    EHS_FB_OUT_F_API2(INX_mqtt_subscribe_typed_ARG_subscribe_value_f) = fval;
    EHS_FB_OUT_B_API2(INX_mqtt_subscribe_typed_ARG_subscribe_value_b) = bval;
    EHS_FB_FINISH(INX_mqtt_subscribe_typed_ARG_subscribe_received);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);

    return EHS_TRUE;
}

EHS_FB_IDENTIFY_FUNCTION(mqtt_subscribe_typed)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_mqtt_subscribe_state_type);
}

EHS_FB_INIT_FUNCTION(mqtt_subscribe_typed)
{
    inx_mqtt_subscribe_state_type* state = (inx_mqtt_subscribe_state_type*)EHS_FB_INIT_CONTEXT;

    state->pFIdata              = NULL;
    state->needProcessing       = EHS_FALSE;
    state->needSubscribe        = EHS_FALSE;
    state->topic[0]             = '\0';
    state->qos                  = 0;
    state->pMqttSubscribeCallback = NULL;
    state->pNext                = NULL;
    state->pPrev                = NULL;

    const char* pParams = EHS_FB_INIT_PARAMETERS;
    if (pParams) {
        pParams = EhsGetWordFromString(state->topic, pParams, sizeof(state->topic));
        pParams = EhsGetUint8FromString(&state->qos, pParams);
        handle_mqtt_param_string(state->topic, EHS_STRING_LENGTH_MAX);
    }
    EhsMQTTSubscribeRegisterState(state);
    return EHS_TRUE;
}

EHS_FB_DESTROY_FUNCTION(mqtt_subscribe_typed)
{
    inx_mqtt_subscribe_state_type* state = (inx_mqtt_subscribe_state_type*)EHS_FB_DESTROY_CONTEXT;
    state->pMqttSubscribeCallback = NULL;
    EhsMQTTSubscribeClearList();
    return EHS_TRUE;
}

EHS_FB_RUN_FUNCTION(mqtt_subscribe_typed_subscribe)
{
    inx_mqtt_subscribe_state_type* state = (inx_mqtt_subscribe_state_type*)EHS_FB_RUN_CONTEXT;

    state->pFIdata = (void*)EHS_FB_RUN_CONTEXT_REF;
    state->pMqttSubscribeCallback = mqtt_subscribe_typed_callback;

    EhsTPMutex_lock(EhsTPMutex_subMQTT);
    if (EHS_FB_IN_CONNECTED_API2(INX_mqtt_subscribe_typed_ARG_subscribe_topic))
        EhsStrcpy(state->topic, EHS_FB_IN_S_API2(INX_mqtt_subscribe_typed_ARG_subscribe_topic));
    if (EHS_FB_IN_CONNECTED_API2(INX_mqtt_subscribe_typed_ARG_subscribe_qos))
        state->qos = (ehs_uint8)EHS_FB_IN_I_API2(INX_mqtt_subscribe_typed_ARG_subscribe_qos);
    state->needSubscribe  = EHS_TRUE;
    state->needProcessing = EHS_TRUE;
    EhsTPMutex_unlock(EhsTPMutex_subMQTT);

    EHS_FB_FINISH(INX_mqtt_subscribe_typed_ARG_subscribe_done);
}

EHS_FB_RUN_FUNCTION(mqtt_subscribe_typed_unsubscribe)
{
    inx_mqtt_subscribe_state_type* state = (inx_mqtt_subscribe_state_type*)EHS_FB_RUN_CONTEXT;

    EhsTPMutex_lock(EhsTPMutex_subMQTT);
    state->needSubscribe  = EHS_FALSE;
    state->needProcessing = EHS_TRUE;
    EhsTPMutex_unlock(EhsTPMutex_subMQTT);

    EHS_FB_FINISH(INX_mqtt_subscribe_typed_ARG_unsubscribe_done);
}
