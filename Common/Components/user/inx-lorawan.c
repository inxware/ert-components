//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-lorawan.h"
#include "lorawan.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//ICB HEADER MACRO END -- DO NOT ALTER
// Worst case -> 11:22:33:44:55:66:77:88
//ICB STATE VAR MACRO START -- DO NOT ALTER

//#define EHS_LORAWAN_DEBUG

#ifdef EHS_LORAWAN_DEBUG
#define ehs_lorawan_debug(...) printf(__VA_ARGS__)
#else
#define ehs_lorawan_debug(...)
#endif

/* My Component state data structure. - Use this in your code! */
typedef struct inx_lorawan_state
{
	EhsCallbackQueueEntryType xEntry[E_LORAWAN_API__MAX_VALUE + 1];
	
	ehs_sint32 Target;
	ehs_bool Mode;
	ehs_sint32 Region;
	ehs_sint32 REPT;
	ehs_sint32 RETRY;
	ehs_sint32 AutoJoin;
	ehs_bool ADR;
	ehs_sint32 DR;
	ehs_float RXWIN2;
	ehs_char appKey[EHS_LORAWAN_KEY_STRLEN + 1];
	ehs_char appEui[EHS_LORAWAN_ID_STRLEN + 1];
	ehs_char nwkSKey[EHS_LORAWAN_KEY_STRLEN + 1];
	ehs_char appSKey[EHS_LORAWAN_KEY_STRLEN + 1];
	ehs_char devAddrABP[EHS_LORAWAN_ID_STRLEN + 1];
	
	ehs_char devAddrOut[EHS_LORAWAN_ID_STRLEN + 1];
	ehs_char sys_status[EHS_STRING_LENGTH_MAX + 1];
	ehs_char devEui[EHS_LORAWAN_ID_STRLEN + 1];
	ehs_sint32 pl_length_out;
} inx_lorawan_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER

static EhsDataflowStringType EhsLorawanRecvMsg = NULL;
static const ehs_char EhsLorawanRecvMsg_empty[2] = "";
/* Callback queue for on_receive */
static EhsCallbackQueueType xLorawanCallbackQueue;
/* Callback queue array for all LoRaWAN callback functions */
static EhsCallbackQueueType xLorawanApiCallbackQueue[E_LORAWAN_API__MAX_VALUE];

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(lorawan)
EHS_FB_FUNCTION_ENTRY("connect", 0x01, lorawan_connect)
EHS_FB_FUNCTION_ENTRY("on_receive_msg", 0x02, lorawan_on_receive_msg)
EHS_FB_FUNCTION_ENTRY("send_msg", 0x03, lorawan_send_msg)
EHS_FB_FUNCTION_ENTRY("reset", 0x04, lorawan_reset)
EHS_FB_FUNCTION_ENTRY("get_statusData", 0x05, lorawan_get_statusData)
EHS_FB_FUNCTION_ENTRY("connect_cb", 0x06, lorawan_connect_cb)
EHS_FB_FUNCTION_ENTRY("send_msg_cb", 0x07, lorawan_send_msg_cb)
EHS_FB_FUNCTION_ENTRY("reset_cb", 0x08, lorawan_reset_cb)
EHS_FB_FUNCTION_ENTRY("get_statusData_cb", 0x09, lorawan_get_statusData_cb)
EHS_FB_FUNCTION_ENTRY("set_datarate", 0x0A, lorawan_set_datarate)
EHS_FB_FUNCTION_ENTRY("get_payload_length", 0x0B, lorawan_get_payload_length)
EHS_FB_FUNCTION_ENTRY("disable", 0x0C, lorawan_disable)
EHS_FB_FUNCTION_ENTRY("set_datarate_cb", 0x0D, lorawan_set_datarate_cb)
EHS_FB_FUNCTION_ENTRY("get_payload_length_cb", 0x0E, lorawan_get_payload_length_cb)
EHS_FB_FUNCTION_ENTRY("disable_cb", 0x0F, lorawan_disable_cb)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER

// Function pointers static declaration for later usage
/// The order should strictly follow the API command enum in `e_ehs_lorawan_api_cmd_t`
///  before `E_LORAWAN_API__MAX_VALUE`
static EhsRunFuncType gfEhsLorawanFBCBFuncs[E_LORAWAN_API__MAX_VALUE] = {
	EHS_FB_RUN_NAME(lorawan_connect_cb),
	EHS_FB_RUN_NAME(lorawan_send_msg_cb),
	EHS_FB_RUN_NAME(lorawan_reset_cb),
	EHS_FB_RUN_NAME(lorawan_get_statusData_cb),
	EHS_FB_RUN_NAME(lorawan_set_datarate_cb),
	EHS_FB_RUN_NAME(lorawan_get_payload_length_cb),
	EHS_FB_RUN_NAME(lorawan_disable_cb),
};

//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_lorawan_ARG_connect_AppKey 1
#define INX_lorawan_ARG_connect_AppEui 2
#define INX_lorawan_ARG_connect_AppSKey 3
#define INX_lorawan_ARG_connect_NwkSKey 4
#define INX_lorawan_ARG_connect_adr_in 5
#define INX_lorawan_ARG_connect_DevAddr_ABP 6
#define INX_lorawan_ARG_connect_miscOpt 7
#define INX_lorawan_ARG_connect_region 8
#define INX_lorawan_ARG_connect_connectDone 1
#define INX_lorawan_ARG_connect_connectFail 2
#define INX_lorawan_ARG_on_receive_msg_fport_rx 1
#define INX_lorawan_ARG_on_receive_msg_link_status 2
#define INX_lorawan_ARG_on_receive_msg_recv_msg 3
#define INX_lorawan_ARG_on_receive_msg_received 1
#define INX_lorawan_ARG_send_msg_payload 1
#define INX_lorawan_ARG_send_msg_frame_port 2
#define INX_lorawan_ARG_send_msg_msg_confirmed 3
#define INX_lorawan_ARG_send_msg_send_done 1
#define INX_lorawan_ARG_send_msg_send_fail 2
#define INX_lorawan_ARG_reset_reset_sent 1
#define INX_lorawan_ARG_reset_reset_send_failed 2
#define INX_lorawan_ARG_get_statusData_get_sd_failed 1
#define INX_lorawan_ARG_get_statusData_data_got 2
#define INX_lorawan_ARG_connect_cb_DevAddr 1
#define INX_lorawan_ARG_connect_cb_connect_errno 2
#define INX_lorawan_ARG_connect_cb_connect_cb_ok 1
#define INX_lorawan_ARG_connect_cb_connect_fail 2
#define INX_lorawan_ARG_send_msg_cb_send_errCode 1
#define INX_lorawan_ARG_send_msg_cb_msg_sent 1
#define INX_lorawan_ARG_send_msg_cb_send_failed 2
#define INX_lorawan_ARG_reset_cb_reset_done 1
#define INX_lorawan_ARG_get_statusData_cb_status 1
#define INX_lorawan_ARG_get_statusData_cb_DevEui 2
#define INX_lorawan_ARG_get_statusData_cb_get_sd_cb_ok 1
#define INX_lorawan_ARG_set_datarate_dr_in 1
#define INX_lorawan_ARG_set_datarate_set_datarate_sent 1
#define INX_lorawan_ARG_set_datarate_set_datarate_busy 2
#define INX_lorawan_ARG_get_payload_length_get_pl_len_sent 1
#define INX_lorawan_ARG_get_payload_length_get_pl_len_busy 2
#define INX_lorawan_ARG_disable_disable_sent 1
#define INX_lorawan_ARG_disable_disable_busy 2
#define INX_lorawan_ARG_set_datarate_cb_set_datarate_ok 1
#define INX_lorawan_ARG_get_payload_length_cb_pl_len 1
#define INX_lorawan_ARG_get_payload_length_cb_pl_len_got 1
#define INX_lorawan_ARG_disable_cb_disabled 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_lorawan_Target 0
#define INX_FB_lorawan_Mode 0
#define INX_FB_lorawan_Region 0
#define INX_FB_lorawan_Repeat 1
#define INX_FB_lorawan_Retry 3
#define INX_FB_lorawan_AutoJoin 300
#define INX_FB_lorawan_ADR 0
#define INX_FB_lorawan_DR 0
#define INX_FB_lorawan_RXWIN2 869.525
#define INX_FB_lorawan_appKey "00000000000000000000000000000000"
#define INX_FB_lorawan_appEui "0000000000000000"
#define INX_FB_lorawan_nwkSKey "00000000000000000000000000000000"
#define INX_FB_lorawan_appSKey "00000000000000000000000000000000"
#define INX_FB_lorawan_devAddrABP "00000000"
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
/**
 * @brief Check whether payload only represents hexidecimal values
 * 
 * @param payload array of characters
 * @param count number of characters to check
 * @return ehs_bool EHS_FALSE if payload is NULL, count is odd or payload contains non-hex character
 */
static ehs_bool check_payload_hex(ehs_char *payload, ehs_uint32 count)
{
	if (payload == NULL) return EHS_FALSE;
	// Check whether count is odd
	if ((count & 1) != 0) return EHS_FALSE;
	ehs_uint32 i = 0;
	ehs_char c;
	for (i = 0 ; i < count ; i++)
	{
		c = payload[i];
		if ((c < '0' || c > '9') && (c < 'A' || c > 'F')) return EHS_FALSE;
	}

	return EHS_TRUE;
}
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
	ehs_sint32 Region;
	ehs_sint32 Repeat;
	ehs_sint32 Retry;
	ehs_sint32 AutoJoin;
	ehs_bool ADR;
	ehs_sint32 DR;
	ehs_float RXWIN2;
	ehs_char* appKey;
	ehs_char* appEui;
	ehs_char* nwkSKey;
	ehs_char* appSKey;
	ehs_char* devAddrABP;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d %d %d %d %d %d %d %d %f %s %s %s %s %s",&Target,&Mode,&Region,&Repeat,&Retry,&AutoJoin,&ADR,&DR,&RXWIN2,&appKey,&appEui,&nwkSKey,&appSKey,&devAddrABP); */
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
	// ehs_sint32 Target;
	// ehs_bool Mode;
	// ehs_sint32 Region;
	// ehs_sint32 Repeat;
	// ehs_sint32 Retry;
	// ehs_sint32 AutoJoin;
	// ehs_bool ADR;
	// ehs_sint32 DR;
	// ehs_float RXWIN2;
	// ehs_char* appKey;
	// ehs_char* appEui;
	// ehs_char* nwkSKey;
	// ehs_char* appSKey;
	// ehs_char* devAddrABP;
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_lorawan_state_type* inx_lorawan_state = (inx_lorawan_state_type*)EHS_FB_INIT_CONTEXT;
	ehs_uint8 _i;
	ehs_lorawan_api_errno_t lorawan_error;
	/*
	inx_lorawan_state->Target = Target;
	inx_lorawan_state->Mode = Mode;
	inx_lorawan_state->REPT = REPT;
	inx_lorawan_state->RETRY = RETRY;
	inx_lorawan_state->Region = Region;*/
	EhsLorawanRecvMsg = NULL;
	for (_i = 0 ; _i < E_LORAWAN_API__MAX_VALUE ; _i++)
	{
		EhsCallbackQueue_register(
			&xLorawanApiCallbackQueue[_i],
			gfEhsLorawanFBCBFuncs[_i],
			//TODO The function instance should appoint to the correct function
			EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(-1 - _i), // Index map 0=-1, 1=-2, 2=-3, ...
			&(inx_lorawan_state->xEntry[_i])
		);
	}
	EhsCallbackQueue_register(&xLorawanCallbackQueue,
			   EHS_FB_RUN_NAME(lorawan_on_receive_msg),
			   EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(-1 - E_LORAWAN_API__MAX_VALUE),
			   &(inx_lorawan_state->xEntry[E_LORAWAN_API__MAX_VALUE])
		);
	/* read the initialisation parameters */
	// Need to allocate memory?
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d %hhu %d %d %d %d %hhu %d %f %s %s %s %s %s",
		&(inx_lorawan_state->Target),
		&(inx_lorawan_state->Mode),
		&(inx_lorawan_state->Region),
		&(inx_lorawan_state->REPT),
		&(inx_lorawan_state->RETRY),
		&(inx_lorawan_state->AutoJoin),
		&(inx_lorawan_state->ADR),
		&(inx_lorawan_state->DR),
		&(inx_lorawan_state->RXWIN2),
		inx_lorawan_state->appKey,
		inx_lorawan_state->appEui,
		inx_lorawan_state->nwkSKey,
		inx_lorawan_state->appSKey,
		inx_lorawan_state->devAddrABP
	);
	lorawan_error = LoRaWAN_init(inx_lorawan_state->Target);
	// This should never set to EHS_FALSE. Failure to initialisation can be reflected in the connect errno output
	//bRet = lorawan_error == E_LWAPIERRNO_OK ? EHS_TRUE : EHS_FALSE;

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
	ehs_uint8 _i;
	for (_i = 0 ; _i < E_LORAWAN_API__MAX_VALUE ; _i++) EhsCallbackQueue_clear(&xLorawanApiCallbackQueue[_i]);
	return EHS_TRUE;
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
	ehs_char *AppKey = inx_lorawan_state->appKey;
	ehs_char *AppEui = inx_lorawan_state->appEui;
	ehs_char *AppSKey = inx_lorawan_state->appSKey;
	ehs_char *NwkSKey = inx_lorawan_state->nwkSKey;
	ehs_char *DevAddr_ABP = inx_lorawan_state->devAddrABP;
	ehs_bool adr = inx_lorawan_state->ADR;
	ehs_char *miscOpt = NULL;
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
	if (EHS_FB_IN_CONNECTED_API2(INX_lorawan_ARG_connect_adr_in))
		adr = EHS_FB_IN_B_API2(INX_lorawan_ARG_connect_adr_in) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_lorawan_ARG_connect_DevAddr_ABP))
		DevAddr_ABP = EHS_FB_IN_S_API2(INX_lorawan_ARG_connect_DevAddr_ABP);
	if (EHS_FB_IN_CONNECTED_API2(INX_lorawan_ARG_connect_miscOpt))
		miscOpt = EHS_FB_IN_S_API2(INX_lorawan_ARG_connect_miscOpt) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_lorawan_ARG_connect_region))
		inx_lorawan_state->Region = EHS_FB_IN_I_API2(INX_lorawan_ARG_connect_region) ;

	if (inx_lorawan_state->Mode == EHS_TRUE) // ABP Mode
	{
		if ((check_payload_hex(AppSKey, EHS_LORAWAN_KEY_STRLEN) == EHS_FALSE) || 
			(check_payload_hex(NwkSKey, EHS_LORAWAN_KEY_STRLEN) == EHS_FALSE) || 
			(check_payload_hex(DevAddr_ABP, EHS_LORAWAN_ID_STRLEN) == EHS_FALSE))
		{
			EHS_FB_FINISH(INX_lorawan_ARG_connect_connectFail);
			return;
		}
	}
	else { // OTAA Mode
		if ((check_payload_hex(AppKey, EHS_LORAWAN_KEY_STRLEN) == EHS_FALSE) || (check_payload_hex(AppEui, EHS_LORAWAN_ID_STRLEN) == EHS_FALSE))
		{
			EHS_FB_FINISH(INX_lorawan_ARG_connect_connectFail);
			return;
		} 
	}

	ehs_lorawan_debug("AppKey: %s, AppEui: %s, Mode: %d\n", AppKey == NULL ? "NULL" : AppKey, AppEui == NULL ? "NULL" : AppEui, inx_lorawan_state->Mode);
	
	if (((AppKey != NULL && AppEui != NULL && inx_lorawan_state->Mode == EHS_FALSE) || (AppSKey != NULL && NwkSKey != NULL && DevAddr_ABP != NULL && inx_lorawan_state->Mode == EHS_TRUE)))
		connect_ret = LoRaWAN_connect(
			AppKey,
			AppEui,
			inx_lorawan_state->Mode,
			DevAddr_ABP,
			AppSKey,
			NwkSKey,
			inx_lorawan_state->REPT,
			inx_lorawan_state->RETRY,
			inx_lorawan_state->Region,
			adr,
			inx_lorawan_state->DR,
			inx_lorawan_state->AutoJoin,
			inx_lorawan_state->devAddrOut
		);
	else
		connect_ret = E_LWAPIERRNO_INVALID_PARAMETERS;
	if (connect_ret != E_LWAPIERRNO_OK)
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
	ehs_lorawan_debug(
		"[%s] {\"wt\":%d,\"rssi\":%d,\"snr\":%.2f}\n{%s}\n",
		__func__,
		gEhsLoraApiData.rxwin,
		gEhsLoraApiData.rssi,
		gEhsLoraApiData.snr,
		EhsLorawanRecvMsg
	);
	if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_on_receive_msg_fport_rx))
		EHS_FB_OUT_I_API2(INX_lorawan_ARG_on_receive_msg_fport_rx) = gEhsLoraApiData.fport;
	if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_on_receive_msg_link_status))
		EhsSprintf(
			EHS_FB_OUT_S_API2(INX_lorawan_ARG_on_receive_msg_link_status),
			"{\"wt\":%d,\"rssi\":%d,\"snr\":%.2f}",
			gEhsLoraApiData.rxwin,
			gEhsLoraApiData.rssi,
			gEhsLoraApiData.snr
		);
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
	ehs_sint32 fport = 0;
	if (EHS_FB_IN_CONNECTED_API2(INX_lorawan_ARG_send_msg_payload))
		payload = EHS_FB_IN_S_API2(INX_lorawan_ARG_send_msg_payload);
	if (EHS_FB_IN_CONNECTED_API2(INX_lorawan_ARG_send_msg_frame_port))
		fport = EHS_FB_IN_I_API2(INX_lorawan_ARG_send_msg_frame_port) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_lorawan_ARG_send_msg_msg_confirmed))
		confirmed = EHS_FB_IN_B_API2(INX_lorawan_ARG_send_msg_msg_confirmed) ;
	if (payload != NULL)
	{
		if (check_payload_hex(payload, EhsStrlen(payload)) == EHS_FALSE)
		{
			ehs_lorawan_debug("[%s] payload is not hex\n", __func__);
			EHS_FB_FINISH(INX_lorawan_ARG_send_msg_send_fail);
			return;
		}
		errCode = LoRaWAN_send_msg(payload, fport, confirmed);
	}
	else
		errCode = -1000;
	if (errCode != 0)
	{
		ehs_lorawan_debug("[%s] fail: %d\n", __func__, errCode);
		EHS_FB_FINISH(INX_lorawan_ARG_send_msg_send_fail);
	}
	else
	{
		ehs_lorawan_debug("[%s] OK: %d\n", __func__, errCode);
		EHS_FB_FINISH(INX_lorawan_ARG_send_msg_send_done);
	}
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
	if (LoRaWAN_reset() == E_LWAPIERRNO_OK) EHS_FB_FINISH(INX_lorawan_ARG_reset_reset_sent);
	else EHS_FB_FINISH(INX_lorawan_ARG_reset_reset_send_failed);
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
	ehs_lorawan_api_errno_t ret = LoRaWAN_get_sysData(inx_lorawan_state->sys_status, inx_lorawan_state->devEui);
	
	if (ret == E_LWAPIERRNO_OK) EHS_FB_FINISH(INX_lorawan_ARG_get_statusData_data_got);
	else EHS_FB_FINISH(INX_lorawan_ARG_get_statusData_get_sd_failed);
}//ICB FUNCTION get_statusData MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION connect_cb MACRO START -- DO NOT ALTER
/**
 * Definition of lorawan_connect_cb.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(lorawan_connect_cb)
{
	inx_lorawan_state_type* inx_lorawan_state = (inx_lorawan_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_connect_cb_connect_errno))
		EHS_FB_OUT_I_API2(INX_lorawan_ARG_connect_cb_connect_errno) = gEhsLoraApiData.error_ret[E_LORAWAN_API_CONNECT];
	if (gEhsLoraApiData.error_ret[E_LORAWAN_API_CONNECT] != E_LWAPIERRNO_OK)
	{
		EHS_FB_FINISH(INX_lorawan_ARG_connect_cb_connect_fail);
		ehs_lorawan_debug("%s fail: %d\n", __func__, gEhsLoraApiData.error_ret[E_LORAWAN_API_CONNECT]);
	}
	else
	{
		if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_connect_cb_DevAddr))
			EhsStrcpy(EHS_FB_OUT_S_API2(INX_lorawan_ARG_connect_cb_DevAddr), inx_lorawan_state->devAddrOut);
		EHS_FB_FINISH(INX_lorawan_ARG_connect_cb_connect_cb_ok);
		ehs_lorawan_debug("%s OK: %d\n", __func__, gEhsLoraApiData.error_ret[E_LORAWAN_API_CONNECT]);
	}
}//ICB FUNCTION connect_cb MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION send_msg_cb MACRO START -- DO NOT ALTER
/**
 * Definition of lorawan_send_msg_cb.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(lorawan_send_msg_cb)
{
	inx_lorawan_state_type* inx_lorawan_state = (inx_lorawan_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_send_msg_cb_send_errCode))
		EHS_FB_OUT_I_API2(INX_lorawan_ARG_send_msg_cb_send_errCode) = gEhsLoraApiData.error_ret[E_LORAWAN_API_SEND_MSG];
	if (gEhsLoraApiData.error_ret[E_LORAWAN_API_SEND_MSG] == E_LWAPIERRNO_OK)
	{
		ehs_lorawan_debug("%s OK: %d\n", __func__, gEhsLoraApiData.error_ret[E_LORAWAN_API_SEND_MSG]);
		EHS_FB_FINISH(INX_lorawan_ARG_send_msg_cb_msg_sent);
	}
	else
	{
		ehs_lorawan_debug("%s fail: %d\n", __func__, gEhsLoraApiData.error_ret[E_LORAWAN_API_SEND_MSG]);
		EHS_FB_FINISH(INX_lorawan_ARG_send_msg_cb_send_failed);
	}
}//ICB FUNCTION send_msg_cb MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION reset_cb MACRO START -- DO NOT ALTER
/**
 * Definition of lorawan_reset_cb.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(lorawan_reset_cb)
{
	inx_lorawan_state_type* inx_lorawan_state = (inx_lorawan_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	ehs_lorawan_debug("%s OK\n", __func__);
	EHS_FB_FINISH(INX_lorawan_ARG_reset_cb_reset_done);
}//ICB FUNCTION reset_cb MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION get_statusData_cb MACRO START -- DO NOT ALTER
/**
 * Definition of lorawan_get_statusData_cb.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(lorawan_get_statusData_cb)
{
	inx_lorawan_state_type* inx_lorawan_state = (inx_lorawan_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_get_statusData_cb_status))
		EhsStrcpy(EHS_FB_OUT_S_API2(INX_lorawan_ARG_get_statusData_cb_status), inx_lorawan_state->sys_status);
	if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_get_statusData_cb_DevEui))
		EhsStrcpy(EHS_FB_OUT_S_API2(INX_lorawan_ARG_get_statusData_cb_DevEui), inx_lorawan_state->devEui);
	ehs_lorawan_debug("%s OK: %d\n", __func__, gEhsLoraApiData.error_ret[E_LORAWAN_API_GET_SYSDATA]);
	EHS_FB_FINISH(INX_lorawan_ARG_get_statusData_cb_get_sd_cb_ok);
}//ICB FUNCTION get_statusData_cb MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION set_datarate MACRO START -- DO NOT ALTER
/**
 * Definition of lorawan_set_datarate.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(lorawan_set_datarate)
{
	inx_lorawan_state_type* inx_lorawan_state = (inx_lorawan_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	ehs_lorawan_api_errno_t err;
	if (EHS_FB_IN_CONNECTED_API2(INX_lorawan_ARG_set_datarate_dr_in))
		err  = LoRaWAN_set_datarate(EHS_FB_IN_I_API2(INX_lorawan_ARG_set_datarate_dr_in));
	else EHS_FB_FINISH(INX_lorawan_ARG_set_datarate_set_datarate_busy);
	if (err == E_LWAPIERRNO_OK) EHS_FB_FINISH(INX_lorawan_ARG_set_datarate_set_datarate_sent);
	else EHS_FB_FINISH(INX_lorawan_ARG_set_datarate_set_datarate_busy);
}//ICB FUNCTION set_datarate MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION get_payload_length MACRO START -- DO NOT ALTER
/**
 * Definition of lorawan_get_payload_length.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(lorawan_get_payload_length)
{
	inx_lorawan_state_type* inx_lorawan_state = (inx_lorawan_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	ehs_lorawan_api_errno_t err = LoRaWAN_get_payloadLength(&(inx_lorawan_state->pl_length_out));
	if (err == E_LWAPIERRNO_OK) EHS_FB_FINISH(INX_lorawan_ARG_get_payload_length_get_pl_len_sent);
	else EHS_FB_FINISH(INX_lorawan_ARG_get_payload_length_get_pl_len_busy);
}//ICB FUNCTION get_payload_length MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION disable MACRO START -- DO NOT ALTER
/**
 * Definition of lorawan_disable.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(lorawan_disable)
{
	inx_lorawan_state_type* inx_lorawan_state = (inx_lorawan_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	ehs_lorawan_api_errno_t err = LoRaWAN_disable();
	if (err == E_LWAPIERRNO_OK) EHS_FB_FINISH(INX_lorawan_ARG_disable_disable_sent);
	else EHS_FB_FINISH(INX_lorawan_ARG_disable_disable_busy);
}//ICB FUNCTION disable MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION set_datarate_cb MACRO START -- DO NOT ALTER
/**
 * Definition of lorawan_set_datarate_cb.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(lorawan_set_datarate_cb)
{
	inx_lorawan_state_type* inx_lorawan_state = (inx_lorawan_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	if (gEhsLoraApiData.error_ret[E_LORAWAN_API_SET_DATARATE] == E_LWAPIERRNO_OK)
	{
		ehs_lorawan_debug("%s OK: %d\n", __func__, gEhsLoraApiData.error_ret[E_LORAWAN_API_SET_DATARATE]);
		EHS_FB_FINISH(INX_lorawan_ARG_set_datarate_cb_set_datarate_ok);
	}
	else
	{
		ehs_lorawan_debug("%s fail: %d\n", __func__, gEhsLoraApiData.error_ret[E_LORAWAN_API_SET_DATARATE]);
		/* TODO */
	}
}//ICB FUNCTION set_datarate_cb MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION get_payload_length_cb MACRO START -- DO NOT ALTER
/**
 * Definition of lorawan_get_payload_length_cb.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(lorawan_get_payload_length_cb)
{
	inx_lorawan_state_type* inx_lorawan_state = (inx_lorawan_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	if (gEhsLoraApiData.error_ret[E_LORAWAN_API_GET_PAYLOADLENGTH] != E_LWAPIERRNO_OK)
	{
		ehs_lorawan_debug("%s fail: %d\n", __func__, gEhsLoraApiData.error_ret[E_LORAWAN_API_GET_PAYLOADLENGTH]);
		/* TODO */
		return;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_get_payload_length_cb_pl_len))
		EHS_FB_OUT_I_API2(INX_lorawan_ARG_get_payload_length_cb_pl_len) = inx_lorawan_state->pl_length_out;
	ehs_lorawan_debug("%s OK: %d\n", __func__, gEhsLoraApiData.error_ret[E_LORAWAN_API_GET_PAYLOADLENGTH]);
	EHS_FB_FINISH(INX_lorawan_ARG_get_payload_length_cb_pl_len_got);
}//ICB FUNCTION get_payload_length_cb MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION disable_cb MACRO START -- DO NOT ALTER
/**
 * Definition of lorawan_disable_cb.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(lorawan_disable_cb)
{
	inx_lorawan_state_type* inx_lorawan_state = (inx_lorawan_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	ehs_lorawan_debug("%s OK: %d\n", __func__, gEhsLoraApiData.error_ret[E_LORAWAN_API_DISABLE]);
	EHS_FB_FINISH(INX_lorawan_ARG_disable_cb_disabled);
}//ICB FUNCTION disable_cb MACRO END -- DO NOT ALTER THIS LINE


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
		ehs_lorawan_debug("[%s] NULL pointer\n", __func__);
		return;
	}
	ehs_lorawan_debug("[%s] recv_msg pointer %p\n", __func__, recv_msg);
	if (has_message)
		EhsLorawanRecvMsg = (EhsDataflowStringType) recv_msg;
	else EhsLorawanRecvMsg = (EhsDataflowStringType) EhsLorawanRecvMsg_empty;
	EhsCallbackQueue_execute(&xLorawanCallbackQueue);
}

// All the data should be from gEhsLoraApiData global variable
void Common_LoRaWAN_FBCBs(e_ehs_lorawan_api_cmd_t cmd)
{
	switch (cmd) {
		case E_LORAWAN_API_CONNECT:
		{
			break;
		}
		case E_LORAWAN_API_SEND_MSG:
		{
			break;
		}
		case E_LORAWAN_API_RESET:
		{
			break;
		}
		case E_LORAWAN_API_GET_SYSDATA:
		{
			break;
		}
		case E_LORAWAN_API_SET_DATARATE:
		{
			break;
		}
		case E_LORAWAN_API_GET_PAYLOADLENGTH:
		{
			break;
		}
		case E_LORAWAN_API_DISABLE:
		{
			break;
		}
		default:
		{
			// No matching command, no execution
			ehs_lorawan_debug("[%s] mismatched command %d\n", __func__, cmd);
			return;
			break;
		}
	}
	ehs_lorawan_debug("[%s] execute %d\n", __func__, cmd);
	EhsCallbackQueue_execute(&xLorawanApiCallbackQueue[cmd]);
}