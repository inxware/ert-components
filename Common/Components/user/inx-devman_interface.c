#include <errno.h>

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-devman_interface.h"


//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct
{
	ehs_char AppName[EHS_STRING_LENGTH_MAX];
	ehs_char ProcessName[EHS_STRING_LENGTH_MAX];
	ehs_bool instance_running;
} inx_DevmanInterface_state_type; //Reference this, maybe store your config parameters in here too.

//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(DevmanInterface)
EHS_FB_FUNCTION_ENTRY("send", DevmanInterface_send)
EHS_FB_FUNCTION_ENTRY("getServerUrl", DevmanInterface_getServerUrl)
EHS_FB_FUNCTION_ENTRY("listen4New", DevmanInterface_listen4New)
EHS_FB_FUNCTION_ENTRY("getMiscData", DevmanInterface_getMiscData)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_DevmanInterface_ARG_send_miscInfo 1
#define INX_DevmanInterface_ARG_send_sendDone 1
#define INX_DevmanInterface_ARG_getServerUrl_serverURL 1
#define INX_DevmanInterface_ARG_getServerUrl_getAllDone 1
#define INX_DevmanInterface_ARG_listen4New_JSON 1
#define INX_DevmanInterface_ARG_listen4New_gotAll 1
#define INX_DevmanInterface_ARG_getMiscData_newJSON 1
#define INX_DevmanInterface_ARG_getMiscData_new 1
#define INX_DevmanInterface_ARG_listen4New_newJSON 1
#define INX_DevmanInterface_ARG_listen4New_new 1
#define INX_DevmanInterface_ARG_getMiscData_JSON 1
#define INX_DevmanInterface_ARG_getMiscData_gotAll 1
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

	EhsSscanf(EHS_FB_INIT_PARAMETERS,"%s %s",inx_DevmanInterface_state->AppName,inx_DevmanInterface_state->ProcessName);
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
	if (EHS_FB_IN_CONNECTED_API2(INX_DevmanInterface_ARG_send_miscInfo)) {
		EhsHMetaSetDevmanMiscULData(EHS_FB_IN_S_API2(INX_DevmanInterface_ARG_send_miscInfo));
	}
	EHS_FB_FINISH(INX_DevmanInterface_ARG_send_sendDone);
}//ICB FUNCTION send MACRO END -- DO NOT ALTER THIS LINE


extern EhsMetaDataType EhsMetaData;

// todo remove the following (see below))
#include <sys/time.h>

/* Waits for the Devman thread to have set a signal then reads the data out */
EHS_FB_THREAD_FUNCTION(DevmanInterface_listen4New_thread) {

	inx_DevmanInterface_state_type* inx_DevmanInterface_state = (inx_DevmanInterface_state_type*)EHS_FB_RUN_CONTEXT;
	pthread_mutex_t *lock = EhsHMetaGetDevmanMiscDLDataMutex();
	int error =0;
	int rc;
	struct timespec ts;
	struct timeval tp;

	while (1) {
		if (*bNewSodlFlagRef == EHS_TRUE) {
			break;
		}
		pthread_mutex_lock(lock);
// todo - look at the older version of the next bit to be linux non-specific
		rc =  gettimeofday(&tp, NULL);
		ts.tv_sec  = tp.tv_sec;
		ts.tv_nsec = tp.tv_usec * 1000;
		ts.tv_sec += 2; // wait 2 s

		if ((error=pthread_cond_timedwait (EhsHMetaGetDevmanMiscDLDataSemaphor(),lock,&ts )) == 0) {
			if (EHS_FB_OUT_CONNECTED_API2(INX_DevmanInterface_ARG_listen4New_JSON)) {
				EhsHMetaGetCpyDevmanNewMiscDLData(EHS_FB_OUT_S_API2(INX_DevmanInterface_ARG_listen4New_JSON)) ;
			}

			EHS_FB_FINISH(INX_DevmanInterface_ARG_listen4New_gotAll);

		}
		else {
#ifdef EHS_DEBUG_MUTEX
			switch (error) {
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
		if (error != 0 && error != ETIMEDOUT) {
			EHSH_LOG_ERROR("Bad mutex for JSON checks\n");
			break;
		}
		pthread_mutex_unlock(lock);
	}
}


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
	inx_DevmanInterface_state_type* inx_DevmanInterface_state = (inx_DevmanInterface_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_DevmanInterface_state->instance_running == EHS_FALSE)
		EHS_FB_START_THREAD(DevmanInterface_listen4New_thread, -90);
	else EHSH_LOG_WARNING("Already running listening thread not restarting");
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
		EHS_FB_OUT_S_API2(INX_DevmanInterface_ARG_getMiscData_newJSON) ;
	EHS_FB_FINISH(INX_DevmanInterface_ARG_getMiscData_new);
}//ICB FUNCTION getMiscData MACRO END -- DO NOT ALTER THIS LINE




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
	if (EHS_FB_OUT_CONNECTED_API2(INX_DevmanInterface_ARG_getServerUrl_serverURL))
		EhsStrcpy(EHS_FB_OUT_S_API2(INX_DevmanInterface_ARG_getServerUrl_serverURL),"Not Implemented");
	EHS_FB_FINISH(INX_DevmanInterface_ARG_getServerUrl_getAllDone);
}//ICB FUNCTION getServerUrl MACRO END -- DO NOT ALTER THIS LINE
