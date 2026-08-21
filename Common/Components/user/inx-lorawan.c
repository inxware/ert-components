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

/*
 * FB-level session state machine.
 *
 * The HAL shim (target/Component-HAL/lorawan/lorawan.c) only tracks "worker
 * idle vs. command in flight" via g_lorawan_cmd; it doesn't know whether the
 * modem has joined. Without an FB-side gate, send_msg / set_class /
 * set_datarate / link_check are accepted as soon as the worker frees up,
 * even if the join is still in progress or the previous connect failed.
 *
 * Blocking-time notes for the RAK3112 backend (target/Component-HAL/lorawan/
 * rak3112/lorawan-rak3112.cpp). All these run on the dedicated taskLoRaWAN_*
 * worker thread, NOT on the EHS event thread, so they do not stall the main
 * recipe scheduler — but the HAL serialises commands via g_lorawan_cmd, so a
 * slow operation also delays every other queued command for its duration:
 *
 *   LoRaWAN_module_connect (OTAA join)   up to 60 s — delay(500) x 120 poll
 *                                        of lmh_join_status_get()
 *   LoRaWAN_module_connect (init)        ~hundreds of ms — lmh_init()
 *   LoRaWAN_module_send_msg              up to seconds for confirmed retries
 *   LoRaWAN_module_set_class             tens–hundreds of ms — MAC class swap
 *   LoRaWAN_module_link_check            seconds (when implemented)
 *   other module_*                       generally <100 ms
 *
 * The state machine below short-circuits impossible operations (e.g. send
 * before join) without entering the HAL at all, so a stuck join doesn't
 * silently swallow MAC-parameter changes either.
 */
typedef enum {
    INX_LW_FB_UNINIT = 0,    /* EHS_FB_INIT not run, or HAL init failed.   */
    INX_LW_FB_INITIALISED,   /* Modem initialised; never joined yet.       */
    INX_LW_FB_JOINING,       /* connect queued; awaiting connect_cb.       */
    INX_LW_FB_JOINED,        /* Joined; uplinks + MAC ops allowed.         */
    INX_LW_FB_DISABLED,      /* User disabled; needs connect to leave.     */
    INX_LW_FB_FAILED,        /* Last connect failed; retry via connect.    */
} inx_lw_fb_state_t;

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
	ehs_sint32 Class; // Consider uint8
	ehs_sint32 SubBand; // Consider uint8
	ehs_sint32 RXWIN2_DR; // Consider uint8
 	ehs_sint32 TxPower; // Consider uint8
	ehs_sint32 ComPort; // Consider uint8

	ehs_char devAddrOut[EHS_LORAWAN_ID_STRLEN + 1];
	/* sys_status holds the JSON status string returned by LoRaWAN_get_sysData.
	 * Was EHS_STRING_LENGTH_MAX+1 (=2048) which dominated the state struct
	 * (~84%) and made the kernel's permanent allocator fail at boot on tight
	 * platforms (rak3112 / esp32s3-n8r8 with WiFi+MQTT+BLE). Both backends
	 * write much smaller payloads — wio_e5 caps at LW_MISC_BUFFER_SIZE=70,
	 * rak3112 at snprintf(data,128,...). 256 leaves comfortable headroom for
	 * future status fields without busting the heap.
	 *
	 * @todo Mirror this size into the ICB template so re-generating the FB
	 * doesn't restore the 2 KB buffer. */
	ehs_char sys_status[256];
	ehs_char devEui[EHS_LORAWAN_ID_STRLEN + 1];
	ehs_sint32 pl_length_out;
	inx_lw_fb_state_t fb_state;
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
EHS_FB_FUNCTION_ENTRY("set_class", 0x10, lorawan_set_class)
EHS_FB_FUNCTION_ENTRY("set_class_cb", 0x11, lorawan_set_class_cb)
EHS_FB_FUNCTION_ENTRY("set_tx_power", 0x12, lorawan_set_tx_power)
EHS_FB_FUNCTION_ENTRY("set_tx_power_cb", 0x13, lorawan_set_tx_power_cb)
EHS_FB_FUNCTION_ENTRY("link_check", 0x14, lorawan_link_check)
EHS_FB_FUNCTION_ENTRY("link_check_cb", 0x15, lorawan_link_check_cb)
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
	EHS_FB_RUN_NAME(lorawan_set_class_cb),
	EHS_FB_RUN_NAME(lorawan_set_tx_power_cb),
	EHS_FB_RUN_NAME(lorawan_link_check_cb),
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
/* connect — new class_in port (arg 9) */
#define INX_lorawan_ARG_connect_class_in 9
/* on_receive_msg — new signal quality output ports */
#define INX_lorawan_ARG_on_receive_msg_rssi 4
#define INX_lorawan_ARG_on_receive_msg_snr 5
#define INX_lorawan_ARG_on_receive_msg_rxwin 6
/* get_statusData_cb — new status output ports */
#define INX_lorawan_ARG_get_statusData_cb_linkMargin 3
#define INX_lorawan_ARG_get_statusData_cb_gateways 4
#define INX_lorawan_ARG_get_statusData_cb_txPower 5
#define INX_lorawan_ARG_get_statusData_cb_currentDR 6
/* set_class */
#define INX_lorawan_ARG_set_class_class_in 1
#define INX_lorawan_ARG_set_class_set_class_sent 1
#define INX_lorawan_ARG_set_class_set_class_busy 2
#define INX_lorawan_ARG_set_class_cb_set_class_ok 1
/* set_tx_power */
#define INX_lorawan_ARG_set_tx_power_tx_power_in 1
#define INX_lorawan_ARG_set_tx_power_set_tx_power_sent 1
#define INX_lorawan_ARG_set_tx_power_set_tx_power_busy 2
#define INX_lorawan_ARG_set_tx_power_cb_set_tx_power_ok 1
/* link_check */
#define INX_lorawan_ARG_link_check_link_check_sent 1
#define INX_lorawan_ARG_link_check_link_check_busy 2
#define INX_lorawan_ARG_link_check_cb_link_check_done 1
#define INX_lorawan_ARG_link_check_cb_link_margin_out 1
#define INX_lorawan_ARG_link_check_cb_gateway_count_out 2
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
#define INX_FB_lorawan_Class 0
#define INX_FB_lorawan_SubBand 0
#define INX_FB_lorawan_RXWIN2_DR 0
#define INX_FB_lorawan_TxPower 0
#define INX_FB_lorawan_ComPort 3
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
	/* SODL → pCallbackTable slot mapping. The kernel parser appends one
	 * pCallbackTable entry per InternalPort, in the order the InternalPort
	 * <Port> elements appear in the CDF <Ports> section (Lucid's
	 * inxware-gui-builder-mfc/LucidApplicationBuilder/SODL.cpp walks
	 * blob->internalport[] 0..N).
	 *
	 * The mapping below assumes the CDF lists InternalPorts in
	 * e_ehs_lorawan_api_cmd_t order (ccbi, smcbi, rcbi, gsdcbi, sfrcbi,
	 * gplcbi, dcbi, sccbi, stpcbi, lccbi) followed by ormsgi for
	 * on_receive_msg. If a future CDF edit inserts a new InternalPort
	 * mid-list, every cb after it shifts by one, the FB silently dispatches
	 * each cb on the wrong FB instance, and runtime faults on a stale
	 * pOut/pFinishPort (LoadProhibited / EXCVADDR=0x18). KEEP THE ORDER
	 * OF <PortType>InternalPort</PortType> ENTRIES IN lorawan.cdf
	 * SYNCHRONISED WITH e_ehs_lorawan_api_cmd_t. */
	for (_i = 0 ; _i < E_LORAWAN_API__MAX_VALUE ; _i++)
	{
		EhsCallbackQueue_register(
			&xLorawanApiCallbackQueue[_i],
			gfEhsLorawanFBCBFuncs[_i],
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
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d %hhu %d %d %d %d %hhu %d %f %s %s %s %s %s %d %d %d %d %d",
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
		inx_lorawan_state->devAddrABP,
		&(inx_lorawan_state->Class),
		&(inx_lorawan_state->SubBand),
		&(inx_lorawan_state->RXWIN2_DR),
		&(inx_lorawan_state->TxPower),
		&(inx_lorawan_state->ComPort)
	);
	lorawan_error = LoRaWAN_init(inx_lorawan_state->Target, inx_lorawan_state->ComPort);
	// This should never set to EHS_FALSE. Failure to initialisation can be reflected in the connect errno output
	//bRet = lorawan_error == E_LWAPIERRNO_OK ? EHS_TRUE : EHS_FALSE;
	inx_lorawan_state->fb_state = (lorawan_error == E_LWAPIERRNO_OK)
		? INX_LW_FB_INITIALISED
		: INX_LW_FB_UNINIT;

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
	inx_lorawan_state->Region = 0;

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
	if (EHS_FB_IN_CONNECTED_API2(INX_lorawan_ARG_connect_class_in))
		inx_lorawan_state->Class = EHS_FB_IN_I_API2(INX_lorawan_ARG_connect_class_in);

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
			inx_lorawan_state->devAddrOut,
			(e_ehs_lw_class_t)inx_lorawan_state->Class,
			inx_lorawan_state->SubBand,
			inx_lorawan_state->RXWIN2,
			inx_lorawan_state->RXWIN2_DR,
			inx_lorawan_state->TxPower
		);
	else
		connect_ret = E_LWAPIERRNO_INVALID_PARAMETERS;
	if (connect_ret != E_LWAPIERRNO_OK)
		EHS_FB_FINISH(INX_lorawan_ARG_connect_connectFail);
	else
	{
		/* HAL has queued the join; wait for connect_cb to confirm result. */
		inx_lorawan_state->fb_state = INX_LW_FB_JOINING;
		EHS_FB_FINISH(INX_lorawan_ARG_connect_connectDone);
	}
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
		EhsSnprintf(EHS_FB_OUT_S_API2(INX_lorawan_ARG_on_receive_msg_link_status), EHS_FB_OUT_S_CAP_API2(INX_lorawan_ARG_on_receive_msg_link_status), "{\"wt\":%d,\"rssi\":%d,\"snr\":%.2f}", gEhsLoraApiData.rxwin, gEhsLoraApiData.rssi, gEhsLoraApiData.snr);
	if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_on_receive_msg_recv_msg))
		EHS_FB_OUT_S_SET_API2(INX_lorawan_ARG_on_receive_msg_recv_msg, (ehs_char *) EhsLorawanRecvMsg) ;
	if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_on_receive_msg_rssi))
		EHS_FB_OUT_I_API2(INX_lorawan_ARG_on_receive_msg_rssi) = gEhsLoraApiData.rssi;
	if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_on_receive_msg_snr))
		EHS_FB_OUT_F_API2(INX_lorawan_ARG_on_receive_msg_snr) = gEhsLoraApiData.snr;
	if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_on_receive_msg_rxwin))
		EHS_FB_OUT_I_API2(INX_lorawan_ARG_on_receive_msg_rxwin) = gEhsLoraApiData.rxwin;
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

	/* JOINED gate — restored after the bypass-debug session.
	 *
	 * Observed behaviour with the gate bypassed (rak3112, manual graph
	 * test ~2026-04-27):
	 *
	 *   Phase 1 — fb_state=1 (INITIALISED, connect never called):
	 *     Bypass let the send through to the HAL. HAL saw g_lorawan_cmd=IDLE,
	 *     queued the cmd, returned OK, and the FB asserted send_done. The
	 *     graph thinks the message went out — but the modem is not joined,
	 *     so the actual radio-side send (worker thread, _LoRaWAN_send_msg →
	 *     LoRaWAN_module_send_msg) returns NOT_JOINED. That failure is
	 *     reported via the send_msg_cb errno port, not the run-side
	 *     send_done port. The graph cannot tell from send_done alone
	 *     whether the message was actually transmitted.
	 *
	 *   Phase 2 — fb_state=2 (JOINING, worker mid-LoRaWAN_module_connect):
	 *     Bypass let the send through to the HAL. HAL saw g_lorawan_cmd=
	 *     CONNECT (worker stuck in the lmh_join_status_get poll loop, up to
	 *     60 s for OTAA on RAK3112), returned BUSY (106), and the FB
	 *     asserted send_fail with errCode=106. Correct rejection — but the
	 *     graph round-tripped through the HAL for nothing.
	 *
	 *   Phase 3 — after the 60 s join attempt completes (success or
	 *     failure) the worker drops g_lorawan_cmd back to COMPLETE, and the
	 *     next send is accepted again — flipping back to send_done in the
	 *     same "ack but might not actually transmit" mode as Phase 1.
	 *
	 * The gate below short-circuits Phase 1 and Phase 3-when-join-failed at
	 * the FB layer, so send_fail fires immediately without disturbing the
	 * HAL or the worker. This is what the original state machine was for.
	 *
	 * KNOWN LIMITATION — autojoin. AutoJoin (state->AutoJoin) tells the
	 * modem to re-join autonomously when the session drops, without the
	 * application driving a fresh connect. When that happens, no
	 * lorawan_connect_cb fires, so fb_state never advances past
	 * INITIALISED / never returns from FAILED — even though the modem may
	 * actually be joined and able to send. With this gate enabled, all
	 * sends after an autojoin are rejected by the FB despite being
	 * possible at the radio layer.
	 *
	 * Resolving this cleanly is awkward because of the FB's dual
	 * control-flow paths:
	 *   - Run functions (lorawan_connect, lorawan_send_msg, …) execute on
	 *     the EHS scheduler thread, driven by input-port events from the
	 *     graph.
	 *   - Cb functions (lorawan_connect_cb, lorawan_send_msg_cb, …)
	 *     execute on the LoRaWAN worker thread, driven by
	 *     Common_LoRaWAN_FBCBs() — which the worker calls at the end of
	 *     each _LoRaWAN_<cmd>() it processes.
	 * State transitions today live in whichever path naturally observes
	 * them: connect-run sets JOINING, connect_cb sets JOINED/FAILED,
	 * disable_cb sets DISABLED, reset_cb sets INITIALISED. An autojoin
	 * event has no natural observer in either path — neither a run func
	 * nor an existing cb runs in response to it.
	 *
	 * Two viable fixes (future work):
	 *   (a) Add a HAL-side status callback that the lmh stack invokes
	 *       when join state changes asynchronously (autojoin success,
	 *       link loss, modem-driven re-join). The callback would push
	 *       onto a new xLorawanStatusCallbackQueue handled by an FB-side
	 *       lorawan_status_changed cb that updates fb_state. This is the
	 *       structurally clean fix but needs lmh-side integration.
	 *   (b) Have the FB-side periodically poll LoRaWAN_get_join_status()
	 *       and update fb_state from the result. Cheap to implement, but
	 *       turns every send into "poll-then-send" and adds latency.
	 *
	 * For now, applications using autojoin should leave a connect call in
	 * the graph at startup so the FB observes the initial JOINED via
	 * connect_cb, then leave the modem to maintain the session — sends
	 * after that point assume the session is still up. */
	if (inx_lorawan_state->fb_state != INX_LW_FB_JOINED)
	{
		ehs_lorawan_debug("[%s] not joined, fb_state=%d\n", __func__, inx_lorawan_state->fb_state);
		EHS_FB_FINISH(INX_lorawan_ARG_send_msg_send_fail);
		return;
	}

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
		inx_lorawan_state->fb_state = INX_LW_FB_FAILED;
		EHS_FB_FINISH(INX_lorawan_ARG_connect_cb_connect_fail);
		ehs_lorawan_debug("%s fail: %d\n", __func__, gEhsLoraApiData.error_ret[E_LORAWAN_API_CONNECT]);
	}
	else
	{
		inx_lorawan_state->fb_state = INX_LW_FB_JOINED;
		if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_connect_cb_DevAddr))
			EHS_FB_OUT_S_SET_API2(INX_lorawan_ARG_connect_cb_DevAddr, inx_lorawan_state->devAddrOut);
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

	/* Defensive: the LoRaWAN HAL dispatches this cb from the worker thread
	 * (Common_LoRaWAN_FBCBs → EhsCallbackQueue_execute), so the FB struct
	 * may be in flight of teardown / monitor-write from the EHS thread when
	 * the debugger is enabled. EHS_FB_OUT_CONNECTED_API2 only does a NULL
	 * test on pOut, so a corrupted small value (we've seen pOut == 0x18,
	 * matching offsetof(EhsFunctionInstanceDataType, dMonitorType)) sails
	 * past it and faults on the pOut[0] load. Refuse to proceed if pOut is
	 * obviously not a valid pointer; deny the app and reboot under
	 * EHS_APP_TRUST_MODEL == 0. */
	EHS_TRUSTLESS_PTR_SANE_FATAL(EHS_FB_RUN_FUNCTION_INSTANCE->pOut,
	    "lorawan_send_msg_cb: pFIdata->pOut corrupted (cross-thread race during debug enable?)");

	// Your code here
	if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_send_msg_cb_send_errCode)) {
		EHS_FB_OUT_I_API2(INX_lorawan_ARG_send_msg_cb_send_errCode) = gEhsLoraApiData.error_ret[E_LORAWAN_API_SEND_MSG];
	}
	if (gEhsLoraApiData.error_ret[E_LORAWAN_API_SEND_MSG] == E_LWAPIERRNO_OK)
	{
		//ehs_lorawan_debug("%s OK: %d\n", __func__, gEhsLoraApiData.error_ret[E_LORAWAN_API_SEND_MSG]);
		if (EHS_FB_FINISH_CONNECTED_API2(INX_lorawan_ARG_send_msg_cb_msg_sent))
			EHS_FB_FINISH(INX_lorawan_ARG_send_msg_cb_msg_sent);
	}
	else
	{
		//ehs_lorawan_debug("%s fail: %d\n", __func__, gEhsLoraApiData.error_ret[E_LORAWAN_API_SEND_MSG]);
		if (EHS_FB_FINISH_CONNECTED_API2(INX_lorawan_ARG_send_msg_cb_send_failed))
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

	/* Hard reset wipes session state — caller must reconnect to re-join. */
	if (inx_lorawan_state->fb_state != INX_LW_FB_UNINIT)
		inx_lorawan_state->fb_state = INX_LW_FB_INITIALISED;
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
		EHS_FB_OUT_S_SET_API2(INX_lorawan_ARG_get_statusData_cb_status, inx_lorawan_state->sys_status);
	if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_get_statusData_cb_DevEui))
		EHS_FB_OUT_S_SET_API2(INX_lorawan_ARG_get_statusData_cb_DevEui, inx_lorawan_state->devEui);
	if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_get_statusData_cb_linkMargin))
		EHS_FB_OUT_I_API2(INX_lorawan_ARG_get_statusData_cb_linkMargin) = gEhsLoraApiData.link_margin;
	if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_get_statusData_cb_gateways))
		EHS_FB_OUT_I_API2(INX_lorawan_ARG_get_statusData_cb_gateways) = gEhsLoraApiData.gateway_count;
	if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_get_statusData_cb_txPower))
		EHS_FB_OUT_I_API2(INX_lorawan_ARG_get_statusData_cb_txPower) = gEhsLoraApiData.tx_power;
	if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_get_statusData_cb_currentDR))
		EHS_FB_OUT_I_API2(INX_lorawan_ARG_get_statusData_cb_currentDR) = gEhsLoraApiData.current_dr;
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

	/* Bail out before err is read if the dr_in port isn't wired — EHS_FB_FINISH
	 * doesn't return from the recipe, so falling through would read uninit. */
	if (!EHS_FB_IN_CONNECTED_API2(INX_lorawan_ARG_set_datarate_dr_in)) {
		EHS_FB_FINISH(INX_lorawan_ARG_set_datarate_set_datarate_busy);
		return;
	}
	/* DR is a per-session MAC param — only meaningful once joined. */
	if (inx_lorawan_state->fb_state != INX_LW_FB_JOINED) {
		EHS_FB_FINISH(INX_lorawan_ARG_set_datarate_set_datarate_busy);
		return;
	}
	ehs_lorawan_api_errno_t err = LoRaWAN_set_datarate(EHS_FB_IN_I_API2(INX_lorawan_ARG_set_datarate_dr_in));
	if (err == E_LWAPIERRNO_OK) EHS_FB_FINISH(INX_lorawan_ARG_set_datarate_set_datarate_sent);
	else                        EHS_FB_FINISH(INX_lorawan_ARG_set_datarate_set_datarate_busy);
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

	/* Max payload depends on the active datarate, which is only set post-join. */
	if (inx_lorawan_state->fb_state != INX_LW_FB_JOINED) {
		EHS_FB_FINISH(INX_lorawan_ARG_get_payload_length_get_pl_len_busy);
		return;
	}
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
	(void)EHS_FB_RUN_CONTEXT;

	if (gEhsLoraApiData.error_ret[E_LORAWAN_API_SET_DATARATE] == E_LWAPIERRNO_OK)
	{
		ehs_lorawan_debug("%s OK: %d\n", __func__, gEhsLoraApiData.error_ret[E_LORAWAN_API_SET_DATARATE]);
		EHS_FB_FINISH(INX_lorawan_ARG_set_datarate_cb_set_datarate_ok);
	}
	else
	{
		ehs_lorawan_debug("%s fail: %d\n", __func__, gEhsLoraApiData.error_ret[E_LORAWAN_API_SET_DATARATE]);
		/* @todo add a dedicated failure output port for set_datarate_cb in the CDF
		 * (and a matching INX_lorawan_ARG_set_datarate_cb_* label) so graphs can
		 * branch on it. For now reuse the OK branch — swallowing the error keeps
		 * the callback queue drained, which is better than hanging the graph. */
		EHS_FB_FINISH(INX_lorawan_ARG_set_datarate_cb_set_datarate_ok);
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

	if (gEhsLoraApiData.error_ret[E_LORAWAN_API_GET_PAYLOADLENGTH] != E_LWAPIERRNO_OK)
	{
		ehs_lorawan_debug("%s fail: %d\n", __func__, gEhsLoraApiData.error_ret[E_LORAWAN_API_GET_PAYLOADLENGTH]);
		/* @todo add a failure output port for get_payload_length_cb in the CDF.
		 * Falling through to the OK finish below with pl_length_out = 0 is
		 * defensive — the caller sees a zero length and treats it as "not
		 * ready yet" — but it's not a proper error signal. */
		inx_lorawan_state->pl_length_out = 0;
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

	inx_lorawan_state->fb_state = INX_LW_FB_DISABLED;
	ehs_lorawan_debug("%s OK: %d\n", __func__, gEhsLoraApiData.error_ret[E_LORAWAN_API_DISABLE]);
	EHS_FB_FINISH(INX_lorawan_ARG_disable_cb_disabled);
}//ICB FUNCTION disable_cb MACRO END -- DO NOT ALTER THIS LINE

EHS_FB_RUN_FUNCTION(lorawan_set_class)
{
	inx_lorawan_state_type* inx_lorawan_state = (inx_lorawan_state_type*)EHS_FB_RUN_CONTEXT;
	if (!EHS_FB_IN_CONNECTED_API2(INX_lorawan_ARG_set_class_class_in)) {
		EHS_FB_FINISH(INX_lorawan_ARG_set_class_set_class_busy);
		return;
	}
	if (inx_lorawan_state->fb_state != INX_LW_FB_JOINED) {
		EHS_FB_FINISH(INX_lorawan_ARG_set_class_set_class_busy);
		return;
	}
	ehs_lorawan_api_errno_t err = LoRaWAN_set_class((e_ehs_lw_class_t)EHS_FB_IN_I_API2(INX_lorawan_ARG_set_class_class_in));
	if (err == E_LWAPIERRNO_OK) EHS_FB_FINISH(INX_lorawan_ARG_set_class_set_class_sent);
	else                        EHS_FB_FINISH(INX_lorawan_ARG_set_class_set_class_busy);
}

EHS_FB_RUN_FUNCTION(lorawan_set_class_cb)
{
	(void)EHS_FB_RUN_CONTEXT;
	/* @todo the CDF only defines a success output port (set_class_ok); failure
	 * is silent. Same story as set_datarate_cb / set_tx_power_cb — finish on
	 * the OK branch either way so the callback queue drains. A dedicated
	 * failure port is a CDF-side change. */
	if (gEhsLoraApiData.error_ret[E_LORAWAN_API_SET_CLASS] != E_LWAPIERRNO_OK)
		ehs_lorawan_debug("%s fail: %d\n", __func__, gEhsLoraApiData.error_ret[E_LORAWAN_API_SET_CLASS]);
	EHS_FB_FINISH(INX_lorawan_ARG_set_class_cb_set_class_ok);
}

EHS_FB_RUN_FUNCTION(lorawan_set_tx_power)
{
	inx_lorawan_state_type* inx_lorawan_state = (inx_lorawan_state_type*)EHS_FB_RUN_CONTEXT;
	if (!EHS_FB_IN_CONNECTED_API2(INX_lorawan_ARG_set_tx_power_tx_power_in)) {
		EHS_FB_FINISH(INX_lorawan_ARG_set_tx_power_set_tx_power_busy);
		return;
	}
	if (inx_lorawan_state->fb_state != INX_LW_FB_JOINED) {
		EHS_FB_FINISH(INX_lorawan_ARG_set_tx_power_set_tx_power_busy);
		return;
	}
	ehs_lorawan_api_errno_t err = LoRaWAN_set_txpower(EHS_FB_IN_I_API2(INX_lorawan_ARG_set_tx_power_tx_power_in));
	if (err == E_LWAPIERRNO_OK) EHS_FB_FINISH(INX_lorawan_ARG_set_tx_power_set_tx_power_sent);
	else                        EHS_FB_FINISH(INX_lorawan_ARG_set_tx_power_set_tx_power_busy);
}

EHS_FB_RUN_FUNCTION(lorawan_set_tx_power_cb)
{
	(void)EHS_FB_RUN_CONTEXT;
	/* @todo dedicated failure output port in the CDF — see set_class_cb. */
	if (gEhsLoraApiData.error_ret[E_LORAWAN_API_SET_TXPOWER] != E_LWAPIERRNO_OK)
		ehs_lorawan_debug("%s fail: %d\n", __func__, gEhsLoraApiData.error_ret[E_LORAWAN_API_SET_TXPOWER]);
	EHS_FB_FINISH(INX_lorawan_ARG_set_tx_power_cb_set_tx_power_ok);
}

EHS_FB_RUN_FUNCTION(lorawan_link_check)
{
	inx_lorawan_state_type* inx_lorawan_state = (inx_lorawan_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_lorawan_state->fb_state != INX_LW_FB_JOINED) {
		EHS_FB_FINISH(INX_lorawan_ARG_link_check_link_check_busy);
		return;
	}
	ehs_lorawan_api_errno_t err = LoRaWAN_link_check();
	if (err == E_LWAPIERRNO_OK) EHS_FB_FINISH(INX_lorawan_ARG_link_check_link_check_sent);
	else EHS_FB_FINISH(INX_lorawan_ARG_link_check_link_check_busy);
}

EHS_FB_RUN_FUNCTION(lorawan_link_check_cb)
{
	inx_lorawan_state_type* inx_lorawan_state = (inx_lorawan_state_type*)EHS_FB_RUN_CONTEXT;
	if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_link_check_cb_link_margin_out))
		EHS_FB_OUT_I_API2(INX_lorawan_ARG_link_check_cb_link_margin_out) = gEhsLoraApiData.link_margin;
	if (EHS_FB_OUT_CONNECTED_API2(INX_lorawan_ARG_link_check_cb_gateway_count_out))
		EHS_FB_OUT_I_API2(INX_lorawan_ARG_link_check_cb_gateway_count_out) = gEhsLoraApiData.gateway_count;
	EHS_FB_FINISH(INX_lorawan_ARG_link_check_cb_link_check_done);
}

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
	/* Serialise cb dispatch with other non-EHS-thread FB writers via the
	 * fbIO mutex. EhsCallbackQueue_execute calls the cb run-func
	 * synchronously here on the LoRaWAN HAL worker thread; matching the
	 * pattern in mqtt_publish.c / mqtt_subscribe.c / devman_mon_mqtt.c. */
	EhsTPMutex_lock(EhsTPMutex_fbIO);
	EhsCallbackQueue_execute(&xLorawanCallbackQueue);
	EhsTPMutex_unlock(EhsTPMutex_fbIO);
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
		case E_LORAWAN_API_SET_CLASS:
		{
			break;
		}
		case E_LORAWAN_API_SET_TXPOWER:
		{
			break;
		}
		case E_LORAWAN_API_LINK_CHECK:
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
	/* Same fbIO serialisation as Common_LoRaWAN_onReceive — see comment there. */
	EhsTPMutex_lock(EhsTPMutex_fbIO);
	EhsCallbackQueue_execute(&xLorawanApiCallbackQueue[cmd]);
	EhsTPMutex_unlock(EhsTPMutex_fbIO);
}
