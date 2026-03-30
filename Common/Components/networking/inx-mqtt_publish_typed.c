/***************************************************************
* Copyright (C) 2008-2025 inx limited - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license.
****************************************************************/

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-mqtt_publish_typed.h"
#include "hal_mqtt.h"
//ICB HEADER MACRO END -- DO NOT ALTER

/* implemented in inx-mqtt_client.c */
void handle_mqtt_param_string(ehs_char* str, ehs_uint32 size);

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
EHS_FB_FUNCTIONS_START(mqtt_publish_typed)
EHS_FB_FUNCTION_ENTRY("publish", 0x01, mqtt_publish_typed_publish)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER

//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
#define INX_mqtt_publish_typed_ARG_publish_topic    1
#define INX_mqtt_publish_typed_ARG_publish_value_i  2
#define INX_mqtt_publish_typed_ARG_publish_value_f  3
#define INX_mqtt_publish_typed_ARG_publish_value_b  4
#define INX_mqtt_publish_typed_ARG_publish_qos      5
#define INX_mqtt_publish_typed_ARG_publish_done     1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER

EHS_FB_IDENTIFY_FUNCTION(mqtt_publish_typed)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_mqtt_publish_state_type);
}

EHS_FB_INIT_FUNCTION(mqtt_publish_typed)
{
    inx_mqtt_publish_state_type* state = (inx_mqtt_publish_state_type*)EHS_FB_INIT_CONTEXT;

    state->pFIdata       = NULL;
    state->pNext         = NULL;
    state->pPrev         = NULL;
    state->qos           = 0;
    state->needProcessing= EHS_FALSE;
    state->always_read   = EHS_FALSE;
    state->topic[0]      = '\0';
    state->message[0]    = '\0';

    const char* pParams = EHS_FB_INIT_PARAMETERS;
    if (pParams) {
        pParams = EhsGetWordFromString(state->topic, pParams);
        pParams = EhsGetUint8FromString(&state->qos, pParams);
        handle_mqtt_param_string(state->topic, EHS_STRING_LENGTH_MAX);
    }
    EhsMQTTPublishRegisterState(state);
    return EHS_TRUE;
}

EHS_FB_DESTROY_FUNCTION(mqtt_publish_typed)
{
    EhsMQTTPublishClearList();
    return EHS_TRUE;
}

EHS_FB_RUN_FUNCTION(mqtt_publish_typed_publish)
{
    inx_mqtt_publish_state_type* state = (inx_mqtt_publish_state_type*)EHS_FB_RUN_CONTEXT;

    state->pFIdata = (void*)EHS_FB_RUN_CONTEXT_REF;

    EhsTPMutex_lock(EhsTPMutex_pubMQTT);

    if (EHS_FB_IN_CONNECTED_API2(INX_mqtt_publish_typed_ARG_publish_topic))
        EhsSprintf(state->topic, "%s", EHS_FB_IN_S_API2(INX_mqtt_publish_typed_ARG_publish_topic));

    if (EHS_FB_IN_CONNECTED_API2(INX_mqtt_publish_typed_ARG_publish_qos))
        state->qos = (ehs_uint8)EHS_FB_IN_I_API2(INX_mqtt_publish_typed_ARG_publish_qos);

    /* Publish whichever typed value port is connected: I > F > B priority */
    if (EHS_FB_IN_CONNECTED_API2(INX_mqtt_publish_typed_ARG_publish_value_i))
        EhsSprintf(state->message, "%d", EHS_FB_IN_I_API2(INX_mqtt_publish_typed_ARG_publish_value_i));
    else if (EHS_FB_IN_CONNECTED_API2(INX_mqtt_publish_typed_ARG_publish_value_f))
        EhsSprintf(state->message, "%g", (double)EHS_FB_IN_F_API2(INX_mqtt_publish_typed_ARG_publish_value_f));
    else if (EHS_FB_IN_CONNECTED_API2(INX_mqtt_publish_typed_ARG_publish_value_b))
        EhsSprintf(state->message, "%s", EHS_FB_IN_B_API2(INX_mqtt_publish_typed_ARG_publish_value_b) ? "true" : "false");

    state->needProcessing = EHS_TRUE;
    EhsTPMutex_unlock(EhsTPMutex_pubMQTT);
}
