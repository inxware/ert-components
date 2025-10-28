/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-mqtt_publish.h"
#include "hal_mqtt.h"
//ICB HEADER MACRO END -- DO NOT ALTER

// implement a stub function for mqtt devman mon support
#if !defined(EHS_DEVMAN_SUPPORT)
// make sure that this function returns NULL when devman mon is not set to mqtt
inx_mqtt_publish_state_type* EhsMqttDevmanMonPublishNeedProcessing()
{
    return NULL;
}
#endif

static inx_mqtt_publish_state_type* gpFirstWidget=NULL;

static inx_mqtt_publish_state_type* inxMQTTPublishGetLastWidget()
{
    inx_mqtt_publish_state_type* widget=gpFirstWidget;
    while(widget!=NULL && widget->pNext!=NULL)
    {
        widget=widget->pNext;
        if(widget==widget->pNext)
        {
            EHSH_LOG_ERROR("inxMQTTpublishGetLastWidget infinite loop found");
            widget->pNext=NULL;
            break;
        }
    }
    return widget;
}

static ehs_bool inxMQTTpubNeedProc(inx_mqtt_publish_state_type* widget)
{
    ehs_bool np;
    EhsTPMutex_lock(EhsTPMutex_pubMQTT);
    np = widget->needProcessing;
    EhsTPMutex_unlock(EhsTPMutex_pubMQTT);
    return np;
}

static inx_mqtt_publish_state_type* inxMQTTPublishGetFirstWidgetNeedProcessing()
{
    // process mqtt publish objects from the devamn mon
    // @TODO - this will always have priority since it's being set from a different thread! we need to some how make this shared equally with objects in fb thread!
#if EHS_DEVMAN_SUPPORT == EHS_DEVMAN_MQTT
    inx_mqtt_publish_state_type* pEhsMqttDevmanMonPublish = EhsMqttDevmanMonPublishNeedProcessing();
    if(pEhsMqttDevmanMonPublish != NULL){
        return pEhsMqttDevmanMonPublish;
    }
#endif
    // process mqtt publish objects from the function blocks
    inx_mqtt_publish_state_type* widget=gpFirstWidget;
    while(widget!=NULL && inxMQTTpubNeedProc(widget)==EHS_FALSE && widget->pNext!=NULL)
    {
        widget=widget->pNext;
        if(widget==widget->pNext)
        {
            EHSH_LOG_ERROR("inxMQTTPublishGetLastWidget infinite loop found");
            widget->pNext=NULL;
            break;
        }
    }
    if(widget==NULL)
    {

    }
    else
    {
        if(inxMQTTpubNeedProc(widget)==EHS_FALSE)
        {
            widget=NULL;
        }
    }
    return widget;
}

static void inxMQTTPublishRegisterWidget(inx_mqtt_publish_state_type* pState)
{
    if(gpFirstWidget==NULL)
    {
        gpFirstWidget=pState;
        return;
    }

    inx_mqtt_publish_state_type* lastWidget=inxMQTTPublishGetLastWidget();
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

/*static void inxMQTTPublishRemoveWidget(inx_mqtt_publish_state_type* pState){
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
EHS_FB_FUNCTIONS_START(mqtt_publish)
EHS_FB_FUNCTION_ENTRY("publish", 0x01, mqtt_publish_publish)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_mqtt_publish_ARG_publish_topic 1
#define INX_mqtt_publish_ARG_publish_payload 2
#define INX_mqtt_publish_ARG_publish_qos 3
#define INX_mqtt_publish_ARG_publish_finishpublish 1
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
EHS_FB_IDENTIFY_FUNCTION(mqtt_publish)
{
    /* Uncomment the following if you need to parse the parameters to calculate memory required */
    /*
    	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_mqtt_publish_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(mqtt_publish)
{
    ehs_bool bRet = EHS_TRUE; /* assume success */

    //this is the reference to the object data for this instance of the function block
    inx_mqtt_publish_state_type* inx_mqtt_publish_state = (inx_mqtt_publish_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
    //EhsSscanf(EHS_FB_INIT_PARAMETERS,"");
    inx_mqtt_publish_state->pFIdata=NULL;
    inx_mqtt_publish_state->pNext=NULL;
    inx_mqtt_publish_state->pPrev=NULL;
    inx_mqtt_publish_state->qos=0;
    inx_mqtt_publish_state->needProcessing=EHS_FALSE;
    inx_mqtt_publish_state->topic[0]='\0';
    inx_mqtt_publish_state->message[0]='\0';
    inx_mqtt_publish_state->always_read = EHS_FALSE;

    const char* pParams = EHS_FB_INIT_PARAMETERS;
    if (pParams) {
        pParams = EhsGetWordFromString(inx_mqtt_publish_state->topic, pParams);
        pParams = EhsGetUint8FromString(&inx_mqtt_publish_state->qos, pParams);
        handle_mqtt_param_string(inx_mqtt_publish_state->topic, EHS_STRING_LENGTH_MAX);
    }
    /* Add any further intialisation code here */
    inxMQTTPublishRegisterWidget(inx_mqtt_publish_state);
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(mqtt_publish)
{
    /*
    inx_mqtt_publish_state_type *inx_mqtt_publish_state = (inx_mqtt_publish_state_type*)EHS_FB_DESTROY_CONTEXT;
    */
    //Your code below here
    gpFirstWidget=NULL;
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION publish MACRO START -- DO NOT ALTER
/**
 * Definition of mqtt_publish_publish.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(mqtt_publish_publish)
{
    inx_mqtt_publish_state_type* inx_mqtt_publish_state = (inx_mqtt_publish_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    //EHSH_LOG_INFO("PBB mqtt_publish_publish");
    //create a pointer to our run data so that we can process events from unity later
    inx_mqtt_publish_state->pFIdata = (void*)EHS_FB_RUN_CONTEXT_REF;
    EhsTPMutex_lock(EhsTPMutex_pubMQTT);
    if (EHS_FB_IN_CONNECTED_API2(INX_mqtt_publish_ARG_publish_topic))
    {
        EhsSprintf(inx_mqtt_publish_state->topic,"%s",EHS_FB_IN_S_API2(INX_mqtt_publish_ARG_publish_topic));
    }
    if (EHS_FB_IN_CONNECTED_API2(INX_mqtt_publish_ARG_publish_payload))
    {
        EhsSprintf(inx_mqtt_publish_state->message,"%s",EHS_FB_IN_S_API2(INX_mqtt_publish_ARG_publish_payload));
    }
    if (EHS_FB_IN_CONNECTED_API2(INX_mqtt_publish_ARG_publish_qos))
    {
        inx_mqtt_publish_state->qos=EHS_FB_IN_I_API2(INX_mqtt_publish_ARG_publish_qos);
    }
    inx_mqtt_publish_state->needProcessing=EHS_TRUE;
    EhsTPMutex_unlock(EhsTPMutex_pubMQTT);
    //we'll run the finish when we actually poll this one
    /*
    EHS_FB_FINISH(INX_mqtt_publish_ARG_publish_finishpublish);
    */
}//ICB FUNCTION publish MACRO END -- DO NOT ALTER THIS LINE



ehs_bool EhsMQTTPublishWritePoll(ehs_char* topic, ehs_char* payload, ehs_uint8* qos)
{
    ehs_bool success=EHS_TRUE;
    
    inx_mqtt_publish_state_type* pState=inxMQTTPublishGetFirstWidgetNeedProcessing();
    if(pState==NULL)
    {
        success=EHS_FALSE;
    }
    else
    {
        EhsFunctionInstanceDataType* pFIdata=(EhsFunctionInstanceDataType*)pState->pFIdata;
        if(pFIdata==NULL && pState->always_read==EHS_FALSE)
        {
            success=EHS_FALSE;
        }
        if(success==EHS_TRUE){
            EhsTPMutex_lock(EhsTPMutex_pubMQTT);
            EhsSprintf(topic,"%s",pState->topic);
            EhsSprintf(payload,"%s",pState->message);
            *qos=pState->qos;
            pState->needProcessing=EHS_FALSE;
            EhsTPMutex_unlock(EhsTPMutex_pubMQTT);
            EhsTPMutex_lock(EhsTPMutex_fbIO);
            if(pFIdata){
                EHS_FB_FINISH(INX_mqtt_publish_ARG_publish_finishpublish);
            }
            EhsTPMutex_unlock(EhsTPMutex_fbIO);
        }
    }
    return success;
}
