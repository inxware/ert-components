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
#include "inx-mqtt_publish.h"
#include "ehs_main.h" // we run th main from here!
//ICB HEADER MACRO END -- DO NOT ALTER

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_mqtt_publish_state
{
    ehs_bool needProcessing;
    ehs_uint8 qos;
    ehs_char topic[EHS_STRING_LENGTH_MAX];
    ehs_char message[EHS_STRING_LENGTH_MAX];
    struct inx_mqtt_publish_state* pNext;
    struct inx_mqtt_publish_state* pPrev;
    EhsFunctionInstanceDataType* pFIdata;
} inx_mqtt_publish_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
static inx_mqtt_publish_state_type* gpFirstWidget=NULL;

static inx_mqtt_publish_state_type* inxMQTTPublishGetLastWidget()
{
    inx_mqtt_publish_state_type* widget=gpFirstWidget;
    while(widget!=NULL && widget->pNext!=NULL)
    {
        widget=widget->pNext;
        if(widget==widget->pNext)
        {
            EHSH_LOG_ERROR("inxMQTTSubscribeGetLastWidget infinite loop found");
            widget->pNext=NULL;
        }
    }
    return widget;
}

static inx_mqtt_publish_state_type* inxMQTTPublishGetFirstWidgetNeedProcessing()
{
    inx_mqtt_publish_state_type* widget=gpFirstWidget;
    while(widget!=NULL && widget->needProcessing==EHS_FALSE && widget->pNext!=NULL)
    {
        widget=widget->pNext;
        if(widget==widget->pNext)
        {
            EHSH_LOG_ERROR("inxMQTTPublishGetLastWidget infinite loop found");
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


//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(mqtt_publish)

EHS_FB_FUNCTION_ENTRY("publish", 0x00, mqtt_publish_publish)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_mqtt_publish_ARG_publish_finishpublish 1
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

    /* Add any further intialisation code here */
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    inxMQTTPublishRegisterWidget(inx_mqtt_publish_state);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
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
    inx_mqtt_publish_state->pFIdata = EHS_FB_RUN_CONTEXT_REF;
    EhsTPMutex_lock(EhsTPMutex_fbIO);
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
    //we'll run the finish when we actually poll this one
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    /*
    EHS_FB_FINISH(INX_mqtt_publish_ARG_publish_finishpublish);
    */
}//ICB FUNCTION publish MACRO END -- DO NOT ALTER THIS LINE

#ifdef EHS_MINGW
#define EHS_MQTT_PUBLISH_EXPORT __declspec(dllexport)
#else
#define EHS_MQTT_PUBLISH_EXPORT // nothing
#endif

EHS_MQTT_PUBLISH_EXPORT ehs_bool EhsMQTTPublishWritePoll(char* topic, char* payload,uint8_t* qos)
{
    ehs_bool success=EHS_FALSE;
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    inx_mqtt_publish_state_type* pState=inxMQTTPublishGetFirstWidgetNeedProcessing();
    if(pState==NULL)
    {
        success=EHS_FALSE;
    }
    else
    {
        EhsFunctionInstanceDataType* pFIdata=pState->pFIdata;
        if(pFIdata==NULL)
        {

        }
        else
        {
            EhsSprintf(topic,"%s",pState->topic);
            EhsSprintf(payload,"%s",pState->message);
            *qos=pState->qos;
            pState->needProcessing=EHS_FALSE;
            success=EHS_TRUE;
            EHS_FB_FINISH(INX_mqtt_publish_ARG_publish_finishpublish);
        }
    }
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    return success;
}
