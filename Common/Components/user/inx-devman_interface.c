/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

#include <errno.h>

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-devman_interface.h"
#include "hal_devman.h"
#if EHS_DEVMAN_MON_SUPPORT == EHS_DEVMAN_MON_MQTT
#include "hal_mqtt.h"
#endif

//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct
{
    //ehs_char AppName[EHS_STRING_LENGTH_MAX]; // This may be a default for the app that Devman will monitor for us (e.g. cpu and RAM) - todo2025 - not currently implemented - see rtinfo.
    //ehs_char ProcessName[EHS_STRING_LENGTH_MAX]; // This may also be a default for the app.
    ehs_bool instance_running;
} inx_DevmanInterface_state_type; //Reference this, maybe store your config parameters in here too.

//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(DevmanInterface)
EHS_FB_FUNCTION_ENTRY("send", 0x01, DevmanInterface_send)
EHS_FB_FUNCTION_ENTRY("getServerUrl", 0x02, DevmanInterface_getServerUrl)
EHS_FB_FUNCTION_ENTRY("setServerUrl", 0x05, DevmanInterface_setServerUrl)
EHS_FB_FUNCTION_ENTRY("listen4New", 0x03, DevmanInterface_listen4New)
EHS_FB_FUNCTION_ENTRY("getMiscData", 0x04, DevmanInterface_getMiscData)
EHS_FB_FUNCTION_ENTRY("getNetStatus", 0x06, DevmanInterface_getNetStatus)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_DevmanInterface_ARG_send_miscInfo 1
#define INX_DevmanInterface_ARG_send_sendDone 1
#define INX_DevmanInterface_ARG_getServerUrl_serverURL 1
#define INX_DevmanInterface_ARG_getServerUrl_getAllDone 1
#define INX_DevmanInterface_ARG_setServerUrl_serverURL 1
#define INX_DevmanInterface_ARG_setServerUrl_serverURLDone 1
#define INX_DevmanInterface_ARG_listen4New_JSON 1
#define INX_DevmanInterface_ARG_listen4New_gotAll 1
#define INX_DevmanInterface_ARG_getMiscData_newJSON 1
#define INX_DevmanInterface_ARG_getMiscData_new 1
#define INX_DevmanInterface_ARG_listen4New_newJSON 1
#define INX_DevmanInterface_ARG_listen4New_new 1
#define INX_DevmanInterface_ARG_getMiscData_JSON 1
#define INX_DevmanInterface_ARG_getMiscData_gotAll 1
#define INX_DevmanInterface_ARG_getNetStatus_connected 1
#define INX_DevmanInterface_ARG_getNetStatus_net_error 2
#define INX_DevmanInterface_ARG_getNetStatus_net_status_done 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_DevmanInterface_AppName
#define INX_FB_DevmanInterface_ProcessName
#define INX_FB_DevmanInterface_App Name
#define INX_FB_DevmanInterface_Process Name
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(DevmanInterface)
{
    /* Uncomment the following if you need to parse the parameters to calculate memory required */
    /*
    	ehs_char* App Name;
    	ehs_char* Process Name;
    	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%s %s",&App Name,&Process Name); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_DevmanInterface_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(DevmanInterface)
{
    ehs_bool bRet = EHS_TRUE; /* assume success */
    //this is the reference to the object data for this instance of the function block
    inx_DevmanInterface_state_type* inx_DevmanInterface_state = (inx_DevmanInterface_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */

    // todo 2025 we may  want ot use this as a default instead of rtinfo's app to monitor EhsSscanf(EHS_FB_INIT_PARAMETERS,"%s %s",inx_DevmanInterface_state->AppName,inx_DevmanInterface_state->ProcessName);
    inx_DevmanInterface_state->instance_running = EHS_FALSE;
    /* Add any further intialisation code here */
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER


//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(DevmanInterface)
{
    inx_DevmanInterface_state_type *inx_DevmanInterface_state = (inx_DevmanInterface_state_type*)EHS_FB_DESTROY_CONTEXT;
    //Your code below here
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION send MACRO START -- DO NOT ALTER
/**
 * Definition of DevmanInterface_send.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(DevmanInterface_send)
{
    inx_DevmanInterface_state_type* inx_DevmanInterface_state = (inx_DevmanInterface_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    if (EHS_FB_IN_CONNECTED_API2(INX_DevmanInterface_ARG_send_miscInfo))
    {
        EhsHMetaSetDevmanMiscULData(EHS_FB_IN_S_API2(INX_DevmanInterface_ARG_send_miscInfo));
    }
    EHS_FB_FINISH(INX_DevmanInterface_ARG_send_sendDone);
}//ICB FUNCTION send MACRO END -- DO NOT ALTER THIS LINE

#if EHS_DEVMAN_MON_SUPPORT != EHS_DEVMAN_MON_MQTT

extern EhsMetaDataType EhsMetaData;

// todo remove the following (see below))
#include <sys/time.h>

/* Polls the Devman Monitor thread for System data (not player data) from Devman */
/* todo this function block should be using HAL processing not pthreads */
EHS_FB_THREAD_FUNCTION(DevmanInterface_listen4New_thread)
{

    inx_DevmanInterface_state_type* inx_DevmanInterface_state = (inx_DevmanInterface_state_type*)EHS_FB_RUN_CONTEXT;
    EhsTPMutexClass semlock = EhsHMetaGetDevmanMiscDLDataMutex();
    EhsTPConditionClass sema = EhsHMetaGetDevmanMiscDLDataSemaphor();
    int error =0;
    int rc;
    struct timespec ts;
    struct timeval tp;

    while (1)
    {
        if (*bNewSodlFlagRef == EHS_TRUE)
        {
            break;
        }
        /* If the sempahores are not intialsed yet then try and initalise them */
        if (semlock == NULL || sema == NULL) {
            EhsSleepUs(200000); /* sleep a while before trying again. */
            semlock = EhsHMetaGetDevmanMiscDLDataMutex();
            sema = EhsHMetaGetDevmanMiscDLDataSemaphor();
        }
        else {
            pthread_mutex_lock((pthread_mutex_t *)semlock);
    
            rc =  gettimeofday(&tp, NULL);
            ts.tv_sec  = tp.tv_sec;
            ts.tv_nsec = tp.tv_usec * 1000;
            ts.tv_sec += 2; // wait 2 s
    
            if ((error=pthread_cond_timedwait ((pthread_cond_t *) sema, (pthread_mutex_t *)semlock,&ts )) == 0)
            {
                if (EHS_FB_OUT_CONNECTED_API2(INX_DevmanInterface_ARG_listen4New_JSON))
                {
                    EhsHMetaGetCpyDevmanNewMiscDLData(EHS_FB_OUT_S_API2(INX_DevmanInterface_ARG_listen4New_JSON)) ;
                }

                EHS_FB_FINISH(INX_DevmanInterface_ARG_listen4New_gotAll);

            }
            else
            {
    //#define EHS_DEBUG_MUTEX
    #ifdef EHS_DEBUG_MUTEX
                switch (error)
                {
                case  ETIMEDOUT:
                    EHSH_LOG_ERROR("Exited condition wait because timed out");
                    break;
                case EINVAL :
                    EHSH_LOG_ERROR(" Invalid.= arguement");

                    break;
                case EPERM:
                    EHSH_LOG_ERROR("Mutex no ownwed by thread");
                    break;
                default:
                    break;
                }
    #endif
            }
            if (error != 0 && error != ETIMEDOUT)
            {
                EHSH_LOG_ERROR("Bad mutex for JSON checks\n");
                break;
            }
            pthread_mutex_unlock((pthread_mutex_t *)semlock);
        }/* end of test for initialised sempahores */  
    }
}

#endif
//ICB FUNCTION listen4New MACRO START -- DO NOT ALTER
/**
 * Definition of DevmanInterface_listen4New.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(DevmanInterface_listen4New)
{
#if EHS_DEVMAN_MON_SUPPORT != EHS_DEVMAN_MON_MQTT
    inx_DevmanInterface_state_type* inx_DevmanInterface_state = (inx_DevmanInterface_state_type*)EHS_FB_RUN_CONTEXT;
    if (inx_DevmanInterface_state->instance_running == EHS_FALSE) {
        EHS_FB_START_THREAD(DevmanInterface_listen4New_thread, -90);
        //EHSH_LOG_INFO("Devman FB Listening socket");
    }
    else EHSH_LOG_WARNING("Already running listening thread not restarting");
#endif
    // Your code here
}//ICB FUNCTION listen4New MACRO END -- DO NOT ALTER THIS LINE


//ICB FUNCTION getMiscData MACRO START -- DO NOT ALTER
/**
 * Definition of DevmanInterface_getMiscData.
 * gets all the stored Misc Data.
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(DevmanInterface_getMiscData)
{
    inx_DevmanInterface_state_type* inx_DevmanInterface_state = (inx_DevmanInterface_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    if (EHS_FB_OUT_CONNECTED_API2(INX_DevmanInterface_ARG_getMiscData_newJSON))
        EhsStrcpy(EHS_FB_OUT_S_API2(INX_DevmanInterface_ARG_getMiscData_newJSON),"Not Implemented") ;
    EHS_FB_FINISH(INX_DevmanInterface_ARG_getMiscData_new);
}//ICB FUNCTION getMiscData MACRO END -- DO NOT ALTER THIS LINE



//ICB FUNCTION setServerUrl MACRO START -- DO NOT ALTER
/**
 * Definition of DevmanInterface_setServerUrl.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */

EHS_FB_RUN_FUNCTION(DevmanInterface_setServerUrl)
{
    inx_DevmanInterface_state_type* inx_DevmanInterface_state = (inx_DevmanInterface_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    if (EHS_FB_IN_CONNECTED_API2(INX_DevmanInterface_ARG_setServerUrl_serverURL))
    {
         EhsHDevmanAddURLtoHeadList(EHS_DEVMAN_COREURLS,EHS_FB_IN_S_API2(INX_DevmanInterface_ARG_setServerUrl_serverURL));
    }
    EHS_FB_FINISH(INX_DevmanInterface_ARG_setServerUrl_serverURLDone);
}//ICB FUNCTION send MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION getServerUrl MACRO START -- DO NOT ALTER
/**
 * Definition of DevmanInterface_getServerUrl.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(DevmanInterface_getServerUrl)
{
    inx_DevmanInterface_state_type* inx_DevmanInterface_state = (inx_DevmanInterface_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    //ehs_char xx[2000];
    //EhsHDevmanGetURL(xx, EHS_DEVMAN_COREURLS,EHS_MAXDEVMANNAMELEN, 0);
    //printf ("XXXXXX:%s\n",xx);
    //todo 2022 #if DEMVNA MAX_STRLEN > EHS_MAXSTRING_LEN #error (Max Devman Str is too long!)
    //todo we need to make sure this doesn't tuncate and just fails enirely intead
    if (EHS_FB_OUT_CONNECTED_API2(INX_DevmanInterface_ARG_getServerUrl_serverURL))
        EhsHDevmanGetURL(EHS_FB_OUT_S_API2(INX_DevmanInterface_ARG_getServerUrl_serverURL), EHS_DEVMAN_COREURLS,EHS_MAXDEVMANNAMELEN, 0); //reporting status the first in the list
    EHS_FB_FINISH(INX_DevmanInterface_ARG_getServerUrl_getAllDone);
}//ICB FUNCTION getServerUrl MACRO END -- DO NOT ALTER THIS LINE

/**
 * Definition of DevmanInterface_getNetStatus.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(DevmanInterface_getNetStatus)
{
	inx_DevmanInterface_state_type* inx_DevmanInterface_state = (inx_DevmanInterface_state_type*)EHS_FB_RUN_CONTEXT;

    ehs_sint32 net_err_id = 0;
    ehs_bool devman_connected = EHS_FALSE;

#if EHS_DEVMAN_MON_SUPPORT == EHS_DEVMAN_MON_MQTT
    EhsMqttDevmanMon_t* pEhsMqttDevmanMon = EhsMqttDevmanMonSupport();
    if(pEhsMqttDevmanMon != NULL){
        devman_connected = pEhsMqttDevmanMon->mqtt_client_connected;
    } else {
        net_err_id = 1;
    }
#else
    // @TODO - check status of non-mqtt interface
    net_err_id = 1;
#endif

	// Your code here
	if (EHS_FB_OUT_CONNECTED_API2(INX_DevmanInterface_ARG_getNetStatus_connected)){
		EHS_FB_OUT_B_API2(INX_DevmanInterface_ARG_getNetStatus_connected) = devman_connected;
    }
	if (EHS_FB_OUT_CONNECTED_API2(INX_DevmanInterface_ARG_getNetStatus_net_error)){
		EHS_FB_OUT_I_API2(INX_DevmanInterface_ARG_getNetStatus_net_error) = net_err_id;
    }
	EHS_FB_FINISH(INX_DevmanInterface_ARG_getNetStatus_net_status_done);
}//ICB FUNCTION getNetStatus MACRO END -- DO NOT ALTER THIS LINE