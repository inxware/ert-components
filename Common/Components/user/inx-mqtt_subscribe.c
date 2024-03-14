/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-mqtt_subscribe.h"
#include "ehs_main.h" // we run the main from here!
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_mqtt_subscribe_state
{
    ehs_bool needProcessing;
    ehs_bool needSubscribe;
    ehs_uint8 qos;
    ehs_char topic[EHS_STRING_LENGTH_MAX];
    EhsFunctionInstanceDataType* pFIdata;
    struct inx_mqtt_subscribe_state* pNext;
    struct inx_mqtt_subscribe_state* pPrev;
} inx_mqtt_subscribe_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
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

static inx_mqtt_subscribe_state_type* inxMQTTSubscribeGetFirstWidgetNeedProcessing()
{
    inx_mqtt_subscribe_state_type* widget=gpFirstWidget;
    while(widget!=NULL && widget->needProcessing==EHS_FALSE && widget->pNext!=NULL)
    {
        widget=widget->pNext;
        if(widget==widget->pNext)
        {
            EHSH_LOG_ERROR("inxMQTTSubscribeGetLast infinite loop found");
            widget->pNext=NULL;
        }
    }
    if(widget==NULL)
    {

    }
    else
    {
        if(widget->needProcessing==EHS_FALSE)
        {
            widget=NULL;
        }
    }
    return widget;
}

static inx_mqtt_subscribe_state_type* inxMQTTSubscribeGetWidgetById(const char* topic)
{
    inx_mqtt_subscribe_state_type* widget=gpFirstWidget;
    while(widget!=NULL)
    {
        if(EhsStrcmp(widget->topic,topic)==0)
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
    inx_mqtt_subscribe_state->pNext=NULL;
    inx_mqtt_subscribe_state->pPrev=NULL;
    const char* pParams = EHS_FB_INIT_PARAMETERS;
    if (pParams) {
        pParams = EhsGetWordFromString(inx_mqtt_subscribe_state->topic, pParams);
        pParams = EhsGetUint8FromString(&inx_mqtt_subscribe_state->qos, pParams);
        handle_mqtt_param_string(inx_mqtt_subscribe_state->topic, EHS_STRING_LENGTH_MAX);
    }
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    inxMQTTSubscribeRegisterWidget(inx_mqtt_subscribe_state);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(mqtt_subscribe)
{
    /*
    inx_mqtt_subscribe_state_type *inx_mqtt_subscribe_state = (inx_mqtt_subscribe_state_type*)EHS_FB_DESTROY_CONTEXT;
    */
    //Your code below here
    gpFirstWidget=NULL;
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

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
    inx_mqtt_subscribe_state->pFIdata = EHS_FB_RUN_CONTEXT_REF;
    EhsTPMutex_lock(EhsTPMutex_fbIO);
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
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
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
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    inx_mqtt_subscribe_state->needSubscribe=EHS_FALSE;
    inx_mqtt_subscribe_state->needProcessing=EHS_TRUE;
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    EHS_FB_FINISH(INX_mqtt_subscribe_ARG_unsubscribe_finishunsubscribe);
}//ICB FUNCTION unsubscribe MACRO END -- DO NOT ALTER THIS LINE


#ifdef EHS_MINGW
#define EHS_MQTT_SUBSCRIBE_EXPORT __declspec(dllexport)
#else
#define EHS_MQTT_SUBSCRIBE_EXPORT // nothing
#endif

EHS_MQTT_SUBSCRIBE_EXPORT ehs_bool EhsMQTTSubscribeWritePoll(char* buffer,ehs_bool* subscribe,uint8_t* qos)
{
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    ehs_bool success=EHS_FALSE;
    inx_mqtt_subscribe_state_type* pState=inxMQTTSubscribeGetFirstWidgetNeedProcessing();
    if(pState==NULL)
    {
        success=EHS_FALSE;
    }
    else
    {
        EhsSprintf(buffer,"%s",pState->topic);
        *subscribe=pState->needSubscribe;
        *qos=pState->qos;
        pState->needProcessing=EHS_FALSE;
        success=EHS_TRUE;
    }
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    return success;
}

EHS_MQTT_SUBSCRIBE_EXPORT ehs_bool EhsMQTTSubscribeEvent(char* topic, char* payload, int payloadSize)
{
    inx_mqtt_subscribe_state_type* pState=inxMQTTSubscribeGetWidgetById(topic);
    if(pState==NULL || pState->pFIdata==NULL || payloadSize >= EHS_STRING_LENGTH_MAX)
    {
        return EHS_FALSE;
    }
    //create pFIData variable so we can use the APIs
    EhsFunctionInstanceDataType* pFIdata=pState->pFIdata;
    EHS_FB_OUT_I_API2(INX_mqtt_subscribe_ARG_subscribe_data_size) = payloadSize;
    EhsMemcpy(EHS_FB_OUT_S_API2(INX_mqtt_subscribe_ARG_subscribe_event),payload,payloadSize);
    ((ehs_char*)EHS_FB_OUT_S_API2(INX_mqtt_subscribe_ARG_subscribe_event))[payloadSize] = '\0'; // null terminate payload for non-binary read (payloadSize < EHS_STRING_LENGTH_MAX)
    EHS_FB_FINISH(INX_mqtt_subscribe_ARG_subscribe_finishevent);
    return EHS_TRUE;
}
