/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-mqtt_subscribe.h"
#include "hal_mqtt.h"

// implement a stub function for mqtt devman mon support
#if !defined(EHS_DEVMAN_MON_SUPPORT) || (EHS_DEVMAN_MON_SUPPORT != EHS_DEVMAN_MON_MQTT)
// make sure that this function returns NULL when devman mon is not set to mqtt
inx_mqtt_subscribe_state_type* EhsMqttDevmanMonSubscribeNeedProcessing()
{
    return NULL;
}
// make sure that this function returns NULL when devman mon is not set to mqtt
inx_mqtt_subscribe_state_type* EhsMqttDevmanMonSubscribeGetWidgetById(const char* topic __attribute__((unused)))
{
    return NULL;
}
#endif

static inx_mqtt_subscribe_state_type* gpFirstWidget=NULL;

static inx_mqtt_subscribe_state_type* inxMQTTSubscribeGetLast()
{
    inx_mqtt_subscribe_state_type* widget=gpFirstWidget;
    while(widget!=NULL && widget->pNext!=NULL)
    {
        widget=widget->pNext;
        if(widget==widget->pNext)
        {
            EHSH_LOG_ERROR("inxMQTTSubscribeGetLast infinite loop found");
            widget->pNext=NULL;
        }
    }
    return widget;
}

static ehs_bool inxMQTTSubNeedProc(inx_mqtt_subscribe_state_type* widget)
{
    ehs_bool np;
    EhsTPMutex_lock(EhsTPMutex_subMQTT);
    np = widget->needProcessing;
    EhsTPMutex_unlock(EhsTPMutex_subMQTT);
    return np;
}

static inx_mqtt_subscribe_state_type* inxMQTTSubscribeGetFirstWidgetNeedProcessing()
{
    // process mqtt subscribe objects from the devamn mon
    inx_mqtt_subscribe_state_type* pEhsMqttDevmanMonSubscribe = EhsMqttDevmanMonSubscribeNeedProcessing();
    if(pEhsMqttDevmanMonSubscribe != NULL){
        return pEhsMqttDevmanMonSubscribe;
    }
    // process mqtt subscribe objects from the function blocks
    inx_mqtt_subscribe_state_type* widget=gpFirstWidget;
    while(widget!=NULL && inxMQTTSubNeedProc(widget)==EHS_FALSE && widget->pNext!=NULL)
    {
        widget=widget->pNext;
        if(widget==widget->pNext)
        {
            EHSH_LOG_ERROR("inxMQTTSubscribeGetFirstWidgetNeedProcessing infinite loop found");
            widget->pNext=NULL;
        }
    }
    if(widget==NULL)
    {

    }
    else
    {
        if(inxMQTTSubNeedProc(widget)==EHS_FALSE)
        {
            widget=NULL;
        }
    }
    return widget;
}

static inx_mqtt_subscribe_state_type* inxMQTTSubscribeGetWidgetById(const char* topic)
{
    // process mqtt subscribe objects from the devamn mon
    inx_mqtt_subscribe_state_type* pEhsMqttDevmanMonSubscribe = EhsMqttDevmanMonSubscribeGetWidgetById(topic);
    if(pEhsMqttDevmanMonSubscribe != NULL){
        return pEhsMqttDevmanMonSubscribe;
    }
    // process mqtt subscribe objects from the function blocks
    inx_mqtt_subscribe_state_type* widget=gpFirstWidget;
    while(widget!=NULL)
    {
        EhsTPMutex_lock(EhsTPMutex_subMQTT);
        int cmp=EhsStrcmp(widget->topic,topic);
        EhsTPMutex_unlock(EhsTPMutex_subMQTT);
        if(cmp==0)
        {
            break;
        }
        widget=widget->pNext;
        //printf("widget %p\n", widget);
        if(widget==widget->pNext)
        {
            EHSH_LOG_ERROR("inxMQTTSubscribeGetWidgetById infinite loop found");
            //printf("inxMQTTSubscribeGetWidgetById infinite loop found\n");
            widget->pNext=NULL;
        }
    }
    return widget;
}

static void inxMQTTSubscribeRegisterWidget(inx_mqtt_subscribe_state_type* pState)
{
    if(gpFirstWidget==NULL)
    {
        gpFirstWidget=pState;
        return;
    }

    inx_mqtt_subscribe_state_type* lastWidget=inxMQTTSubscribeGetLast();
    if(lastWidget==NULL)
    {
        gpFirstWidget=pState;
    }
    else
    {
        lastWidget->pNext=pState;
        pState->pPrev=lastWidget;
    }
}

/*static void inxMQTTSubscribeRemoveWidget(inx_mqtt_subscribe_state_type* pState){
	if(pState->pPrev!=NULL){
		pState->pPrev->pNext=pState->pNext;
	}
	if(pState->pNext!=NULL){
		pState->pNext->pPrev=pState->pPrev;
	}
	//do we need to update the first entry pointer?
	if(gpFirstWidget==pState){
		gpFirstWidget=pState->pNext;
	}
}*/

/* implemented in inx-mqtt_client.c */
void handle_mqtt_param_string(ehs_char* str, ehs_uint32 size);

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(mqtt_subscribe)
EHS_FB_FUNCTION_ENTRY("subscribe", 0x01, mqtt_subscribe_subscribe)
EHS_FB_FUNCTION_ENTRY("unsubscribe", 0x02, mqtt_subscribe_unsubscribe)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_mqtt_subscribe_ARG_subscribe_topic 1
#define INX_mqtt_subscribe_ARG_subscribe_qos 2
#define INX_mqtt_subscribe_ARG_subscribe_event 1
#define INX_mqtt_subscribe_ARG_subscribe_data_size 2
#define INX_mqtt_subscribe_ARG_subscribe_finishsubscribe 1
#define INX_mqtt_subscribe_ARG_subscribe_finishevent 2
#define INX_mqtt_subscribe_ARG_unsubscribe_finishunsubscribe 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(mqtt_subscribe)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_mqtt_subscribe_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(mqtt_subscribe)
{
    ehs_bool bRet = EHS_TRUE; /* assume success */

    //this is the reference to the object data for this instance of the function block
    inx_mqtt_subscribe_state_type* inx_mqtt_subscribe_state = (inx_mqtt_subscribe_state_type*)EHS_FB_INIT_CONTEXT;

    inx_mqtt_subscribe_state->pFIdata=NULL;
    inx_mqtt_subscribe_state->needProcessing=EHS_FALSE;
    inx_mqtt_subscribe_state->needSubscribe=EHS_FALSE;
    inx_mqtt_subscribe_state->topic[0]='\0';
    inx_mqtt_subscribe_state->qos=0;
    inx_mqtt_subscribe_state->pMqttSubscribeCallback=NULL;
    inx_mqtt_subscribe_state->pNext=NULL;
    inx_mqtt_subscribe_state->pPrev=NULL;
    const char* pParams = EHS_FB_INIT_PARAMETERS;
    if (pParams) {
        pParams = EhsGetWordFromString(inx_mqtt_subscribe_state->topic, pParams);
        pParams = EhsGetUint8FromString(&inx_mqtt_subscribe_state->qos, pParams);
        handle_mqtt_param_string(inx_mqtt_subscribe_state->topic, EHS_STRING_LENGTH_MAX);
    }
    inxMQTTSubscribeRegisterWidget(inx_mqtt_subscribe_state);
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(mqtt_subscribe)
{
    inx_mqtt_subscribe_state_type* inx_mqtt_subscribe_state = (inx_mqtt_subscribe_state_type*)EHS_FB_DESTROY_CONTEXT;
    inx_mqtt_subscribe_state->pMqttSubscribeCallback=NULL;
    
    //Your code below here
    gpFirstWidget=NULL;
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

ehs_bool EhsMqttSubscribeCallback(struct inx_mqtt_subscribe_state* pState, char* payload, ehs_sint32 payloadSize)
{
    if(pState == NULL || pState->pFIdata==NULL){
        return EHS_FALSE;
    }
    //create pFIData variable so we can use the APIs
    EhsFunctionInstanceDataType* pFIdata=(EhsFunctionInstanceDataType*)pState->pFIdata;
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    EHS_FB_OUT_I_API2(INX_mqtt_subscribe_ARG_subscribe_data_size) = payloadSize;
    EhsMemcpy(EHS_FB_OUT_S_API2(INX_mqtt_subscribe_ARG_subscribe_event),payload,payloadSize);
    ((ehs_char*)EHS_FB_OUT_S_API2(INX_mqtt_subscribe_ARG_subscribe_event))[payloadSize] = '\0'; // null terminate payload for non-binary read (payloadSize < EHS_STRING_LENGTH_MAX)
    EHS_FB_FINISH(INX_mqtt_subscribe_ARG_subscribe_finishevent);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);

    return EHS_TRUE;
}

//ICB FUNCTION subscribe MACRO START -- DO NOT ALTER
/**
 * Definition of mqtt_subscribe_subscribe.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(mqtt_subscribe_subscribe)
{
    inx_mqtt_subscribe_state_type* inx_mqtt_subscribe_state = (inx_mqtt_subscribe_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    //create a pointer to our run data so that we can process events from unity later
    inx_mqtt_subscribe_state->pFIdata = (void*)EHS_FB_RUN_CONTEXT_REF;
    inx_mqtt_subscribe_state->pMqttSubscribeCallback = EhsMqttSubscribeCallback;
    EhsTPMutex_lock(EhsTPMutex_subMQTT);
    if (EHS_FB_IN_CONNECTED_API2(INX_mqtt_subscribe_ARG_subscribe_topic))
    {
        EhsStrcpy(inx_mqtt_subscribe_state->topic,EHS_FB_IN_S_API2(INX_mqtt_subscribe_ARG_subscribe_topic));
    }
    if(EHS_FB_IN_CONNECTED_API2(INX_mqtt_subscribe_ARG_subscribe_qos))
    {
        inx_mqtt_subscribe_state->qos=EHS_FB_IN_I_API2(INX_mqtt_subscribe_ARG_subscribe_qos);
    }
    inx_mqtt_subscribe_state->needSubscribe=EHS_TRUE;
    inx_mqtt_subscribe_state->needProcessing=EHS_TRUE;
    EhsTPMutex_unlock(EhsTPMutex_subMQTT);
    /*
    if (EHS_FB_IN_CONNECTED_API2(INX_mqtt_subscribe_ARG_subscribe_qos))
    if (EHS_FB_OUT_CONNECTED_API2(INX_mqtt_subscribe_ARG_subscribe_event))
    	EHS_FB_OUT_S_API2(INX_mqtt_subscribe_ARG_subscribe_event) ;
    */
    EHS_FB_FINISH(INX_mqtt_subscribe_ARG_subscribe_finishsubscribe);
    /*
    EHS_FB_FINISH(INX_mqtt_subscribe_ARG_subscribe_finishevent);
    */
}//ICB FUNCTION subscribe MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION unsubscribe MACRO START -- DO NOT ALTER
/**
 * Definition of mqtt_subscribe_unsubscribe.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(mqtt_subscribe_unsubscribe)
{
    inx_mqtt_subscribe_state_type* inx_mqtt_subscribe_state = (inx_mqtt_subscribe_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    //create a pointer to our run data so that we can process events from unity later
    EhsTPMutex_lock(EhsTPMutex_subMQTT);
    inx_mqtt_subscribe_state->needSubscribe=EHS_FALSE;
    inx_mqtt_subscribe_state->needProcessing=EHS_TRUE;
    EhsTPMutex_unlock(EhsTPMutex_subMQTT);
    EHS_FB_FINISH(INX_mqtt_subscribe_ARG_unsubscribe_finishunsubscribe);
}//ICB FUNCTION unsubscribe MACRO END -- DO NOT ALTER THIS LINE


ehs_bool EhsMQTTSubscribeWritePoll(char* buffer, ehs_bool* subscribe, ehs_uint8* qos)
{
    ehs_bool success=EHS_FALSE;
    inx_mqtt_subscribe_state_type* pState=inxMQTTSubscribeGetFirstWidgetNeedProcessing();
    if(pState==NULL)
    {
        success=EHS_FALSE;
    }
    else
    {
        EhsTPMutex_lock(EhsTPMutex_subMQTT);
        EhsSprintf(buffer,"%s",pState->topic);
        *subscribe=pState->needSubscribe;
        *qos=pState->qos;
        pState->needProcessing=EHS_FALSE;
        EhsTPMutex_unlock(EhsTPMutex_subMQTT);
        success=EHS_TRUE;
    }
    return success;
}

ehs_bool EhsMQTTSubscribeEvent(char* topic, char* payload, ehs_sint32 payloadSize)
{
    ehs_bool success=EHS_FALSE;
    inx_mqtt_subscribe_state_type* pState=inxMQTTSubscribeGetWidgetById(topic);
    if(pState != NULL && pState->pMqttSubscribeCallback != NULL && payloadSize < EHS_MQTT_SUBS_RECV_BUFF_MAX_LENGTH)
    {
        success = pState->pMqttSubscribeCallback(pState, payload, payloadSize);
    }
    return success;
}
