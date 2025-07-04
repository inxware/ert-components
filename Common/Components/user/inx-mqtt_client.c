/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-mqtt_client.h"
#include "inx-mqtt_subscribe.h"
#include "inx-mqtt_publish.h"
#include "hal_mqtt.h"
//ICB HEADER MACRO END -- DO NOT ALTER

#define INX_MQTT_PUBLISH_FIFO_SIZE 1

ehs_uint32 gMqttClientInstanceCount = 0;

// implement a stub function for mqtt devman mon support
#if !defined(EHS_DEVMAN_MON_SUPPORT) || (EHS_DEVMAN_MON_SUPPORT != EHS_DEVMAN_MON_MQTT)
// make sure that this function returns NULL when devman mon is not set to mqtt
EhsMqttDevmanMon_t* EhsMqttDevmanMonSupport()
{
    return NULL;
}
#endif


static inx_mqtt_client_state_type_mine* gpFirstWidget=NULL;


/* @brief Iterates through the global list pointed to by gpFirstWidget
*/
static inx_mqtt_client_state_type_mine* inxMQTTClientGetLastWidget()
{
    // Check if mqtt devman mon support is enabled. This function aways returns NULL if it isn't.
    EhsMqttDevmanMon_t* pEhsMqttDevmanMon = EhsMqttDevmanMonSupport();
    if(pEhsMqttDevmanMon != NULL){
        // Now return pointer to devman mon's mqtt clinet instance
        // @TODO - note that once we can support more than one client instance this may need chaning
        return &( pEhsMqttDevmanMon->mqtt_client_state );
    }

    inx_mqtt_client_state_type_mine* widget=gpFirstWidget;
    while(widget!=NULL && widget->pNext!=NULL)
    {
        widget=widget->pNext;
        if(widget==widget->pNext)
        {
            EHSH_LOG_ERROR("inxMQTTClientGetLastWidget infinite loop found");
            widget->pNext=NULL;
            break;
        }
    }
    return widget;
}

/*
  @brief Build linked list of MQTT Client Functions function blocks.
*/
static void inxMQTTClientRegister(inx_mqtt_client_state_type_mine* pState)
{
    if(gpFirstWidget==NULL)
    {
        gpFirstWidget=pState;
        return;
    }
    EHSH_LOG_ERROR("More than 1 MQTT client present - I don't think we support this!");

    inx_mqtt_client_state_type_mine* lastWidget=inxMQTTClientGetLastWidget();
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

/* Remove a client widget*/
static void inxMQTTClientRemove(inx_mqtt_client_state_type_mine* pState){
    /* todo2024 again we want this to iterate if we are going to have more than 1 client connection. 
      but for now we are just going to do this: (we only have one)*/
    gpFirstWidget = NULL;
    /* Instead of something a bit like this but with iteration 
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
    */
}

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(mqtt_client)

EHS_FB_FUNCTION_ENTRY("connect", 0x01, mqtt_client_connect)
EHS_FB_FUNCTION_ENTRY("disconnect", 0x02, mqtt_client_disconnect)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_mqtt_client_ARG_connect_password 1
#define INX_mqtt_client_ARG_connect_tls 2
#define INX_mqtt_client_ARG_connect_username 3
#define INX_mqtt_client_ARG_connect_clientid 4
#define INX_mqtt_client_ARG_connect_port 5
#define INX_mqtt_client_ARG_connect_host 6
#define INX_mqtt_client_ARG_connect_err_msg 1
#define INX_mqtt_client_ARG_connect_finishconnect 1
#define INX_mqtt_client_ARG_connect_err_connect 2
#define INX_mqtt_client_ARG_connect_finishdisconnect 3
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER

#if defined(EHS_MQTT_CLIENT_FB_THREAD)
EHS_FB_THREAD_FUNCTION(mqtt_client_thread)
{
    Ehs_FB_ThreadStarted();
    
    EhsMqttClientLoop(NULL);

    Ehs_FB_ThreadComplete();
    EhsHThread_exit();
}
#endif

//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(mqtt_client)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_mqtt_client_state_type_mine);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER

/* Set string set as NULL as 0-length string */
void handle_mqtt_param_string(ehs_char* str, ehs_uint32 size)
{
    if(str && EhsStrcmp(str, "NULL") == 0)
    {
        EhsMemset(str, '\0', size);
    }
}

//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(mqtt_client)
{
    ehs_bool bRet = EHS_TRUE; /* assume success */

    inx_mqtt_client_state_type_mine* inx_mqtt_client_state = (inx_mqtt_client_state_type_mine*)EHS_FB_INIT_CONTEXT;
    inx_mqtt_client_state->pFIdata=NULL;
    inx_mqtt_client_state->connect=EHS_FALSE;
    inx_mqtt_client_state->initialised=EHS_FALSE;
    inx_mqtt_client_state->pNext=NULL;
    inx_mqtt_client_state->pPrev=NULL;
    inx_mqtt_client_state->port = 0;
    inx_mqtt_client_state->tls = 0;
    inx_mqtt_client_state->always_read = EHS_FALSE; // don't allow for reading data in EhsMQTTConnectPoll if pFIdata is NULL

    if(gMqttClientInstanceCount >= EHS_MQTT_CLIENT_INSTANCE_MAX){
        EHSH_LOG_ERROR("Exceeded number of allowed MQTT clinet instance (%d)\n", EHS_MQTT_CLIENT_INSTANCE_MAX);
        return EHS_FALSE;
    }

    const char* pParams = EHS_FB_INIT_PARAMETERS;
    if (pParams) {
        pParams = EhsGetWordFromString(inx_mqtt_client_state->host, pParams);
        pParams = EhsGetUint16FromString(&inx_mqtt_client_state->port, pParams);
        pParams = EhsGetWordFromString(inx_mqtt_client_state->clientid, pParams);
        pParams = EhsGetWordFromString(inx_mqtt_client_state->username, pParams);
        pParams = EhsGetWordFromString(inx_mqtt_client_state->password, pParams);
        pParams = EhsGetUint8FromString(&inx_mqtt_client_state->tls, pParams);
        pParams = EhsGetWordFromString(inx_mqtt_client_state->clientCertFileName, pParams);
        pParams = EhsGetWordFromString(inx_mqtt_client_state->clientKeyFileName, pParams);
        pParams = EhsGetWordFromString(inx_mqtt_client_state->rootCAFileName, pParams);
        /* Check for Lucid tools use of NULL as place holder for missing strings */
        handle_mqtt_param_string(inx_mqtt_client_state->host, EHS_STRING_LENGTH_MAX); //TODO:STRINGLENGTH!
        handle_mqtt_param_string(inx_mqtt_client_state->clientid, EHS_STRING_LENGTH_MAX);//TODO:STRINGLENGTH!
        handle_mqtt_param_string(inx_mqtt_client_state->username, EHS_STRING_LENGTH_MAX);//TODO:STRINGLENGTH!
        handle_mqtt_param_string(inx_mqtt_client_state->password, EHS_STRING_LENGTH_MAX);//TODO:STRINGLENGTH!
        handle_mqtt_param_string(inx_mqtt_client_state->clientCertFileName, INX_MQTT_CERT_FILENAME_LENGTH);
        handle_mqtt_param_string(inx_mqtt_client_state->clientKeyFileName, INX_MQTT_CERT_FILENAME_LENGTH);
        handle_mqtt_param_string(inx_mqtt_client_state->rootCAFileName, INX_MQTT_CERT_FILENAME_LENGTH);

    }
    else {
        EHSH_LOG_ERROR("No MQTT connection client paramters found");
    }
    /* Add any further intialisation code here */
    inxMQTTClientRegister(inx_mqtt_client_state);
#if defined(EHS_MQTT_CLIENT_FB_THREAD)
//todo2024 - could we make this conditional on a DevmanMon thread not running (if thie function block is set to use the Devman config broker and the DevmanMon is enabled)
// or does it not reall matter if we have two clients open on the same broker on the same machine?
    EHS_FB_START_THREAD(mqtt_client_thread,-99);
#endif

    if(bRet){
        inx_mqtt_client_state->initialised=EHS_TRUE;
        gMqttClientInstanceCount++;
        EHSH_LOG_INFO("Increase MQTT clinet instace count (%d) \n", gMqttClientInstanceCount);
    }
    
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(mqtt_client)
{
    /* todo2023 - if we do want handle multiple clients then we will need to find the specific instance and remove it from the linked list */
    //Your code below here
    EHSH_LOG_WARNING("Setting the linked list to NULL - possibly a memory leek?");
    inx_mqtt_client_state_type_mine* inx_mqtt_client_state = (inx_mqtt_client_state_type_mine*)EHS_FB_DESTROY_CONTEXT;
    inxMQTTClientRemove(inx_mqtt_client_state);
    if(inx_mqtt_client_state && inx_mqtt_client_state->initialised==EHS_TRUE){
        if(gMqttClientInstanceCount > 0){
            gMqttClientInstanceCount--;
            EHSH_LOG_INFO("Decrease MQTT clinet instace count (%d) \n", gMqttClientInstanceCount);
        }else{
            EHSH_LOG_ERROR("Attempting to decrease MQTT clinet count from 0 \n");    
        }
    }

    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION connect MACRO START -- DO NOT ALTER
/**
 * Definition of mqtt_publish_publish.
 * Definition of mqtt_client_connect.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(mqtt_client_connect)
{
    /*
    inx_mqtt_client_state_type* inx_mqtt_client_state = (inx_mqtt_client_state_type*)EHS_FB_RUN_CONTEXT;
    */

    // Your code here
    inx_mqtt_client_state_type_mine* inx_mqtt_client_state = (inx_mqtt_client_state_type_mine*)EHS_FB_RUN_CONTEXT;
    //create a pointer to our run data so that we can process events from unity later
    inx_mqtt_client_state->pFIdata = (void*)EHS_FB_RUN_CONTEXT_REF;
    inx_mqtt_client_state->connect=1;

}//ICB FUNCTION connect MACRO END -- DO NOT ALTER THIS LINE

ehs_uint32 EhsMqttClientInstanceCount()
{
    return gMqttClientInstanceCount;
}

/* 
   @brief Reads the data values from inout ports and overides any paramters
*/
void EhsMQTTConnectPoll(ehs_bool* connect, char** host, ehs_uint16* pPort, ehs_bool* pTLS, char** clientid, char** username, char** password, char** clientCertFileName, char** clientKeyFileName, char** rootCAFileName)
{
    inx_mqtt_client_state_type_mine* inx_mqtt_client_state=inxMQTTClientGetLastWidget();
    if(inx_mqtt_client_state==NULL)
    {
        *connect=EHS_FALSE;
    }
    else
    {
        *connect=inx_mqtt_client_state->connect;
        EhsFunctionInstanceDataType* pFIdata=(EhsFunctionInstanceDataType*)inx_mqtt_client_state->pFIdata;
        if(pFIdata==NULL && inx_mqtt_client_state->always_read==EHS_FALSE)
        {
            return;
        }
        *clientCertFileName=inx_mqtt_client_state->clientCertFileName;
        *clientKeyFileName=inx_mqtt_client_state->clientKeyFileName;
        *rootCAFileName=inx_mqtt_client_state->rootCAFileName;
        if(pFIdata && EHS_FB_IN_CONNECTED_API2(INX_mqtt_client_ARG_connect_host))
        {
            *host=EHS_FB_IN_S_API2(INX_mqtt_client_ARG_connect_host);
        }else{
            *host = inx_mqtt_client_state->host;
        }
        if(pFIdata && EHS_FB_IN_CONNECTED_API2(INX_mqtt_client_ARG_connect_port))
        {
            *pPort=EHS_FB_IN_I_API2(INX_mqtt_client_ARG_connect_port);
        }else{
            *pPort=inx_mqtt_client_state->port;
        }
        if(pFIdata && EHS_FB_IN_CONNECTED_API2(INX_mqtt_client_ARG_connect_tls))
        {
            *pTLS=EHS_FB_IN_B_API2(INX_mqtt_client_ARG_connect_tls);
        }else{
            *pTLS=inx_mqtt_client_state->tls;
        }
        if(pFIdata && EHS_FB_IN_CONNECTED_API2(INX_mqtt_client_ARG_connect_clientid))
        {
            *clientid=EHS_FB_IN_S_API2(INX_mqtt_client_ARG_connect_clientid);
        }else{
            *clientid=inx_mqtt_client_state->clientid;
        }
        if(pFIdata && EHS_FB_IN_CONNECTED_API2(INX_mqtt_client_ARG_connect_username))
        {
            *username=EHS_FB_IN_S_API2(INX_mqtt_client_ARG_connect_username);
        }else{
            *username=inx_mqtt_client_state->username;
        }
        if(pFIdata && EHS_FB_IN_CONNECTED_API2(INX_mqtt_client_ARG_connect_password))
        {
            *password=EHS_FB_IN_S_API2(INX_mqtt_client_ARG_connect_password);
        }else{
            *password=inx_mqtt_client_state->password;
        }
    }
}

/* 
@brief Callb ack when the connection is made (just asserts the output event)
todo2024 - this should support instance data for the specific function instance. 
*/

void EhsMQTTConnectEvent(ehs_bool connect)
{
    // todo2024 just using the last one for now.
    inx_mqtt_client_state_type_mine* pState=inxMQTTClientGetLastWidget();
    if(pState==NULL)
    {

    }
    else
    {
        //create pFIData variable so we can use the APIs
        EhsFunctionInstanceDataType* pFIdata=(EhsFunctionInstanceDataType*)pState->pFIdata;
        if(pFIdata==NULL)
        {
            // @TODO - note that once we can support more than one client instance this may need chaning
            EhsMqttDevmanMon_t* pEhsMqttDevmanMon = EhsMqttDevmanMonSupport();
            if(pEhsMqttDevmanMon != NULL){
                pEhsMqttDevmanMon->mqtt_client_connected = connect;
            }
        }
        else
        {
            if(connect)
            {
                EHS_FB_FINISH(INX_mqtt_client_ARG_connect_finishconnect);
            }
            else
            {
                EHS_FB_FINISH(INX_mqtt_client_ARG_connect_finishdisconnect);
            }
        }
    }
}

/* Writes out error messages. 
   todo2023 - this seems to assume we only ever report errors via the the last client FB, which is OK wjile we only have 1 but is a bit random.
*/

void EhsMQTTReportError(const ehs_char* err_msg)
{
    if(err_msg && err_msg[0] != '\0')
    {
        // @TODO - note that once we can support more than one client instance this may need chaning
        EhsMqttDevmanMon_t* pEhsMqttDevmanMon = EhsMqttDevmanMonSupport();
        if(pEhsMqttDevmanMon != NULL){
            if(pEhsMqttDevmanMon->pMqttDevmanMonErrorCallback){
                pEhsMqttDevmanMon->pMqttDevmanMonErrorCallback(err_msg);
            }
            return;
        }
        inx_mqtt_client_state_type_mine* pState=inxMQTTClientGetLastWidget();
        if(pState != NULL)
        {
            //create pFIData variable so we can use the APIs
            EhsFunctionInstanceDataType* pFIdata=(EhsFunctionInstanceDataType*)pState->pFIdata;
            if(pFIdata != NULL)
            {
                EhsTPMutex_lock(EhsTPMutex_fbIO);
                if (EHS_FB_OUT_CONNECTED_API2(INX_mqtt_client_ARG_connect_err_msg))
                {
                    EhsStrcpy(EHS_FB_OUT_S_API2(INX_mqtt_client_ARG_connect_err_msg), err_msg);
                    EHS_FB_FINISH(INX_mqtt_client_ARG_connect_err_connect);
                }
                EhsTPMutex_unlock(EhsTPMutex_fbIO);
            }
        }
    }
}

//ICB FUNCTION disconnect MACRO START -- DO NOT ALTER
/**
 * Definition of mqtt_client_disconnect.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(mqtt_client_disconnect)
{
    // Your code here
    inx_mqtt_client_state_type_mine* inx_mqtt_client_state = (inx_mqtt_client_state_type_mine*)EHS_FB_RUN_CONTEXT;
    inx_mqtt_client_state->connect=0;
}//ICB FUNCTION disconnect MACRO END -- DO NOT ALTER THIS LINE
