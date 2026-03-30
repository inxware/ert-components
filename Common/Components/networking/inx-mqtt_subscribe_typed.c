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
#define INX_mqtt_subscribe_typed_ARG_subscribe_value_i     2
#define INX_mqtt_subscribe_typed_ARG_subscribe_value_f     3
#define INX_mqtt_subscribe_typed_ARG_subscribe_value_b     4
#define INX_mqtt_subscribe_typed_ARG_unsubscribe_done      1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER

/* Parse a received MQTT payload string into typed values and fire "received". */
static ehs_bool mqtt_subscribe_typed_callback(struct inx_mqtt_subscribe_state* pState,
                                              char* payload, ehs_sint32 payloadSize)
{
    (void)payloadSize;
    if (pState == NULL || pState->pFIdata == NULL) return EHS_FALSE;

    EhsFunctionInstanceDataType* pFIdata = (EhsFunctionInstanceDataType*)pState->pFIdata;

    /* Integer: parse decimal */
    ehs_sint32 ival = 0;
    EhsSscanf(payload, "%d", &ival);

    /* Float: parse decimal */
    ehs_float fval = 0.0f;
    EhsSscanf(payload, "%f", &fval);

    /* Boolean: "true"/"1"/non-zero = true; "false"/"0" = false */
    ehs_bool bval;
    if (EhsStrcmp(payload, "true") == 0 || EhsStrcmp(payload, "True") == 0 ||
        EhsStrcmp(payload, "TRUE") == 0) {
        bval = EHS_TRUE;
    } else if (EhsStrcmp(payload, "false") == 0 || EhsStrcmp(payload, "False") == 0 ||
               EhsStrcmp(payload, "FALSE") == 0) {
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
        pParams = EhsGetWordFromString(state->topic, pParams);
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
