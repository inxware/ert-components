//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-lorawan.h"
#include "lorawan.h"
#include <stdlib.h>
#include <string.h>
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_lorawan_state
{
	EhsCallbackQueueEntryType xEntry;
	ehs_sint32 Target;
	ehs_bool Mode;
	ehs_char* Region;
	ehs_sint32 REPT;
	ehs_sint32 RETRY;
} inx_lorawan_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER

static EhsDataflowStringType EhsLorawanRecvMsg = NULL;
static const ehs_char EhsLorawanRecvMsg_empty[2] = "";
static EhsCallbackQueueType xLorawanCallbackQueue;

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(lorawan)
EHS_FB_FUNCTION_ENTRY("connect", 0x01, lorawan_connect)
EHS_FB_FUNCTION_ENTRY("on_receive_msg", 0x02, lorawan_on_receive_msg)
EHS_FB_FUNCTION_ENTRY("send_msg", 0x03, lorawan_send_msg)
EHS_FB_FUNCTION_ENTRY("reset", 0x04, lorawan_reset)
EHS_FB_FUNCTION_ENTRY("get_statusData", 0x05, lorawan_get_statusData)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_lorawan_ARG_connect_AppKey 1
#define INX_lorawan_ARG_connect_AppEui 2
#define INX_lorawan_ARG_connect_AppSKey 3
#define INX_lorawan_ARG_connect_NwkSKey 4
#define INX_lorawan_ARG_connect_DevAddr_ABP 5
#define INX_lorawan_ARG_connect_DevAddr 1
#define INX_lorawan_ARG_connect_connectFail 1
#define INX_lorawan_ARG_connect_connectDone 2
#define INX_lorawan_ARG_on_receive_msg_recv_msg 1
#define INX_lorawan_ARG_on_receive_msg_received 1
#define INX_lorawan_ARG_send_msg_payload 1
#define INX_lorawan_ARG_send_msg_msg_confirmed 2
#define INX_lorawan_ARG_send_msg_send_errCode 1
#define INX_lorawan_ARG_send_msg_send_fail 1
#define INX_lorawan_ARG_send_msg_send_done 2
#define INX_lorawan_ARG_reset_reset_done 1
#define INX_lorawan_ARG_get_statusData_status 1
#define INX_lorawan_ARG_get_statusData_DevEui 2
#define INX_lorawan_ARG_get_statusData_data_got 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_lorawan_Target 0
#define INX_FB_lorawan_Mode 0
#define INX_FB_lorawan_Region EU868
#define INX_FB_lorawan_REPT 1
#define INX_FB_lorawan_RETRY 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(lorawan)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_sint32 Target;
	ehs_bool Mode;
	ehs_char* Region;
	ehs_sint32 REPT;
	ehs_sint32 RETRY;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d %d %s %d %d",&Target,&Mode,&Region,&REPT,&RETRY); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_lorawan_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(lorawan)
{
	/*
	ehs_sint32 Target;
	ehs_bool Mode;
	ehs_char* Region = (ehs_char *)malloc(sizeof(EHS_FB_INIT_PARAMETERS));
	ehs_sint32 REPT;
	ehs_sint32 RETRY;*/
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_lorawan_state_type* inx_lorawan_state = (inx_lorawan_state_type*)EHS_FB_INIT_CONTEXT;
	/*
	inx_lorawan_state->Target = Target;
	inx_lorawan_state->Mode = Mode;
	inx_lorawan_state->REPT = REPT;
	inx_lorawan_state->RETRY = RETRY;
	inx_lorawan_state->Region = Region;*/
	EhsLorawanRecvMsg = NULL;
	EhsCallbackQueue_register(&xLorawanCallbackQueue,
			   EHS_FB_RUN_NAME(lorawan_on_receive_msg),
			   EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(-1), //TODO
			   &(inx_lorawan_state->xEntry)
		);
	/* read the initialisation parameters */
	// Need to allocate memory?
	inx_lorawan_state->Region = (char *)malloc(sizeof(EHS_FB_INIT_PARAMETERS));
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d %d %s %d %d",&(inx_lorawan_state->Target),&(inx_lorawan_state->Mode),inx_lorawan_state->Region,&(inx_lorawan_state->REPT),&(inx_lorawan_state->RETRY));
	bRet = LoRaWAN_init(inx_lorawan_state->Target) == 0 ? EHS_TRUE : EHS_FALSE;

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(lorawan)
{
	inx_lorawan_state_type *inx_lorawan_state = (inx_lorawan_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
	// Need to free the memory?
	if (inx_lorawan_state->Region != NULL)
	{
		free(inx_lorawan_state->Region);
		inx_lorawan_state->Region = NULL;
	}

	// We should make them as robust as possible because it may be called multiple times
	//  So we need to handle this properly in the target code.
	LoRaWAN_deinit();
	EhsCallbackQueue_clear(&xLorawanCallbackQueue);
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION connect MACRO START -- DO NOT ALTER
/**
 * Definition of lorawan_connect.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(lorawan_connect)
{
	inx_lorawan_state_type* inx_lorawan_state = (inx_lorawan_state_type*)EHS_FB_RUN_CONTEXT;
	ehs_char *AppKey = NULL, *AppEui = NULL, *DevAddr_OUT;
	ehs_char *AppSKey = NULL, *NwkSKey = NULL, *DevAddr_ABP = NULL;
	int connect_ret = 0;

	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_lorawan_ARG_connect_AppKey))
		AppKey = EHS_FB_IN_S_API2(INX_lorawan_ARG_connect_AppKey);
	if (EHS_FB_IN_CONNECTED_API2(INX_lorawan_ARG_connect_AppEui))
		AppEui = EHS_FB_IN_S_API2(INX_lorawan_ARG_connect_AppEui);
	if (EHS_FB_IN_CONNECTED_API2(INX_lorawan_ARG_connect_AppSKey))
		AppSKey = EHS_FB_IN_S_API2(INX_lorawan_ARG_connect_AppSKey);
	if (EHS_FB_IN_CONNECTED_API2(INX_lorawan_ARG_connect_NwkSKey))
		NwkSKey = EHS_FB_IN_S_API2(INX_lorawan_ARG_connect_NwkSKey);
	if (EHS_FB_IN_CONNECTED_API2(INX_lorawan_ARG_connect_DevAddr_ABP))
		DevAddr_ABP = EHS_FB_IN_S_API2(INX_lorawan_ARG_connect_DevAddr_ABP);
	
	if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_connect_DevAddr))
		DevAddr_OUT = EHS_FB_OUT_S_API2(INX_lorawan_ARG_connect_DevAddr) ;
	
	if (((AppKey != NULL && AppEui != NULL && inx_lorawan_state->Mode == EHS_FALSE) || (AppSKey != NULL && NwkSKey != NULL && DevAddr_ABP != NULL && inx_lorawan_state->Mode == EHS_TRUE)) && DevAddr_OUT != NULL)
		connect_ret = LoRaWAN_connect(AppKey, AppEui, inx_lorawan_state->Mode, DevAddr_ABP, AppSKey, NwkSKey, inx_lorawan_state->REPT, inx_lorawan_state->RETRY, inx_lorawan_state->Region, DevAddr_OUT);
	else
		connect_ret = -1000;
	if (connect_ret != 0)
		EHS_FB_FINISH(INX_lorawan_ARG_connect_connectFail);
	else
		EHS_FB_FINISH(INX_lorawan_ARG_connect_connectDone);
}//ICB FUNCTION connect MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION on_receive_msg MACRO START -- DO NOT ALTER
/**
 * Definition of lorawan_on_receive_msg.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(lorawan_on_receive_msg)
{
	inx_lorawan_state_type* inx_lorawan_state = (inx_lorawan_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_on_receive_msg_recv_msg))
		EhsStrcpy(EHS_FB_OUT_S_API2(INX_lorawan_ARG_on_receive_msg_recv_msg), (ehs_char *) EhsLorawanRecvMsg) ;
	EHS_FB_FINISH(INX_lorawan_ARG_on_receive_msg_received);
}//ICB FUNCTION on_receive_msg MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION send_msg MACRO START -- DO NOT ALTER
/**
 * Definition of lorawan_send_msg.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(lorawan_send_msg)
{
	inx_lorawan_state_type* inx_lorawan_state = (inx_lorawan_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	ehs_char *payload = NULL;
	ehs_sint32 errCode = 0;
	ehs_bool confirmed = EHS_FALSE;
	if (EHS_FB_IN_CONNECTED_API2(INX_lorawan_ARG_send_msg_payload))
		payload = EHS_FB_IN_S_API2(INX_lorawan_ARG_send_msg_payload);
	if (EHS_FB_IN_CONNECTED_API2(INX_lorawan_ARG_send_msg_msg_confirmed))
		confirmed = EHS_FB_IN_B_API2(INX_lorawan_ARG_send_msg_msg_confirmed) ;
	if (payload != NULL)
		errCode = LoRaWAN_send_msg(payload, confirmed);
	else
		errCode = -1000;
	if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_send_msg_send_errCode))
		EHS_FB_OUT_I_API2(INX_lorawan_ARG_send_msg_send_errCode) = errCode ;
	if (errCode != 0)
		EHS_FB_FINISH(INX_lorawan_ARG_send_msg_send_fail);
	else
		EHS_FB_FINISH(INX_lorawan_ARG_send_msg_send_done);
}//ICB FUNCTION send_msg MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION reset MACRO START -- DO NOT ALTER
/**
 * Definition of lorawan_reset.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(lorawan_reset)
{
	inx_lorawan_state_type* inx_lorawan_state = (inx_lorawan_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	LoRaWAN_reset();
	EHS_FB_FINISH(INX_lorawan_ARG_reset_reset_done);
}//ICB FUNCTION reset MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION get_statusData MACRO START -- DO NOT ALTER
/**
 * Definition of lorawan_get_statusData.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(lorawan_get_statusData)
{
	inx_lorawan_state_type* inx_lorawan_state = (inx_lorawan_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	ehs_char *status, *DevEui;
	if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_get_statusData_status))
		status = EHS_FB_OUT_S_API2(INX_lorawan_ARG_get_statusData_status) ;
	if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_get_statusData_DevEui))
		DevEui = EHS_FB_OUT_S_API2(INX_lorawan_ARG_get_statusData_DevEui) ;
	LoRaWAN_get_sysData(status, DevEui);
	
	EHS_FB_FINISH(INX_lorawan_ARG_get_statusData_data_got);
}//ICB FUNCTION get_statusData MACRO END -- DO NOT ALTER THIS LINE

/*
 * When the LoRaWAN receives the messages. This is called in target's receive callback
 *
 * */
void Common_LoRaWAN_onReceive(char *recv_msg, ehs_bool has_message)
{
//	EhsLorawanRecvMsg = (EhsDataflowStringType) malloc(EhsStrlen(recv_msg) + 1);
//	EhsStrcpy(EhsLorawanRecvMsg, recv_msg);
	if (recv_msg == NULL)
	{
		printf("NULL pointer");
		return;
	}
	printf("recv_msg pointer %p\n", recv_msg);
	if (has_message)
		EhsLorawanRecvMsg = (EhsDataflowStringType) recv_msg;
	else EhsLorawanRecvMsg = (EhsDataflowStringType) EhsLorawanRecvMsg_empty;
	EhsCallbackQueue_execute(&xLorawanCallbackQueue);
}
