//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-wifi_station.h"
#include "wifi_station.h"
#include <stdio.h>
#include <stdlib.h>
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_wifi_station_state
{
	EhsCallbackQueueEntryType xEntry;
	ehs_char PSKPass[EHS_STRING_LENGTH_MAX];
	EhsFunctionInstanceDataType* pFIdata;
	ehs_char SSID[EHS_STRING_LENGTH_MAX];
	ehs_sint32 type;
	ehs_bool tryReconnect;
	ehs_sint32 retry;
	ehs_sint32 EntType;
	ehs_sint32 EAP;
	ehs_sint32 TTLS2;
	ehs_bool needServerCert;
	ehs_char serverCert[EHS_STRING_LENGTH_MAX];
	ehs_char tlsCert[EHS_STRING_LENGTH_MAX];
	ehs_char tlsKey[EHS_STRING_LENGTH_MAX];
	ehs_char eapID[EHS_STRING_LENGTH_MAX];
	ehs_char eapUser[EHS_STRING_LENGTH_MAX];
	ehs_char eapPass[EHS_STRING_LENGTH_MAX];
} inx_wifi_station_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER

static inx_wifi_station_state_type* gpInxWifiStationState = NULL;

enum eWifiStationCallbackSource {
	eWifiStationCallbackSource_Connect = 0,
	eWifiStationCallbackSource_Internal,
	eWifiStationCallbackSource_Disconnect,
	eWifiStationCallbackSource_MAX
};

static EhsDataflowStringType EhsWifiStationIpAddr = NULL;
static EhsDataflowStringType EhsWifiStationApMac = NULL;
static EhsDataflowIntType EhsWifiStationRssi = 0;
static EhsDataflowIntType EhsWifiStationErrCode = 0;
static EhsDataflowIntType EhsWifiStationReason = 0;
static volatile enum eWifiStationCallbackSource sWifiStationCallbackSource = eWifiStationCallbackSource_Connect;
static EhsCallbackQueueType xWifiStationCallbackQueue;

static ehs_bool gEhsStatusWifiStationDisconnected = EHS_FALSE;

#define EHS_WIFISTATION_THREADLOOP_SLEEP_TIME_S 1

/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(wifi_station)
EHS_FB_FUNCTION_ENTRY("do_connect",1,wifi_station_do_connect)
EHS_FB_FUNCTION_ENTRY("do_disconnect",2,wifi_station_do_disconnect)
EHS_FB_FUNCTION_ENTRY("internal_thread",3,wifi_station_internal_thread)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_wifi_station_ARG_do_connect_auth_type 1
#define INX_wifi_station_ARG_do_connect_ssid 2
#define INX_wifi_station_ARG_do_connect_PSKPass 3
#define INX_wifi_station_ARG_do_connect_EAP 4
#define INX_wifi_station_ARG_do_connect_Enterprise_type 5
#define INX_wifi_station_ARG_do_connect_ttls_phase2 6
#define INX_wifi_station_ARG_do_connect_needCert 7
#define INX_wifi_station_ARG_do_connect_serverCert 8
#define INX_wifi_station_ARG_do_connect_tlsCert 9
#define INX_wifi_station_ARG_do_connect_tlsKey 10
#define INX_wifi_station_ARG_do_connect_eapID 11
#define INX_wifi_station_ARG_do_connect_eapUser 12
#define INX_wifi_station_ARG_do_connect_eapPass 13
#define INX_wifi_station_ARG_do_connect_do_connect_OK 1
#define INX_wifi_station_ARG_do_disconnect_do_disconnect_OK 1
#define INX_wifi_station_ARG_internal_thread_errCode 1
#define INX_wifi_station_ARG_internal_thread_disconnect_reason 2
#define INX_wifi_station_ARG_internal_thread_ip_address 3
#define INX_wifi_station_ARG_internal_thread_rssi 4
#define INX_wifi_station_ARG_internal_thread_ap_mac_address 5
#define INX_wifi_station_ARG_internal_thread_connectFail 1
#define INX_wifi_station_ARG_internal_thread_connect_OK 2
#define INX_wifi_station_ARG_internal_thread_disconnected 3
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_wifi_station_SSID 
#define INX_FB_wifi_station_type 0
#define INX_FB_wifi_station_tryReconnect 0
#define INX_FB_wifi_station_retry 1
#define INX_FB_wifi_station_EntType 0
#define INX_FB_wifi_station_EAP 1
#define INX_FB_wifi_station_TTLS2 1
#define INX_FB_wifi_station_needServerCert 0
#define INX_FB_wifi_station_serverCert 
#define INX_FB_wifi_station_tlsCert 
#define INX_FB_wifi_station_tlsKey 
#define INX_FB_wifi_station_eapID 
#define INX_FB_wifi_station_eapUser 
#define INX_FB_wifi_station_eapPass 
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */

static void EhsWifiStationSetState(inx_wifi_station_state_type* state)
{
	gpInxWifiStationState = state;
}
static inx_wifi_station_state_type* EhsWifiStationGetState()
{
	return gpInxWifiStationState;
}

static void EhsWifiStationSetCBSource(enum eWifiStationCallbackSource source)
{
	sWifiStationCallbackSource = source;
}
static enum eWifiStationCallbackSource EhsWifiStationGetCBSource()
{
	return sWifiStationCallbackSource;
}

EHS_FB_THREAD_FUNCTION(wifi_station_thread)
{
	Ehs_FB_ThreadStarted();
	enum eWifiStationCallbackSource sWifiStationCallbackSource_Internal;
	inx_wifi_station_state_type* inx_wifi_station_state;
	eWifiStationStatus sWifiStationStatus = WifiStation_Disconnected;
	ehs_bool run = EHS_TRUE;
	ehs_char *ip_address;
	ehs_char *mac_address;
	ehs_sint32 rssi;

	ehs_char canonicalAppdataPath[EHS_STRING_LENGTH_MAX];
	ehs_char canonicalClientCertFileName[EHS_STRING_LENGTH_MAX];
	ehs_char canonicalClientKeyFileName[EHS_STRING_LENGTH_MAX];
	ehs_char canonicalRootCAFileName[EHS_STRING_LENGTH_MAX];
	do
	{
		inx_wifi_station_state = EhsWifiStationGetState();
		sWifiStationCallbackSource_Internal = EhsWifiStationGetCBSource();
		if (inx_wifi_station_state != NULL && inx_wifi_station_state->pFIdata != NULL)
		{
			switch (sWifiStationCallbackSource_Internal){
				case eWifiStationCallbackSource_Connect:
					EhsHMetagetCurrentAppDir(canonicalAppdataPath);
					if (inx_wifi_station_state->type == Type_WifiStation_Enterprise && inx_wifi_station_state->EAP == EAP_WifiStation_TLS)
					{
						canonicalClientCertFileName[0] = '\0';
						EhsStrcat(canonicalClientCertFileName, canonicalAppdataPath);
						EhsStrcat(canonicalClientCertFileName, EHS_TD_FILES_SEPARATOR_STR);
						EhsStrcat(canonicalClientCertFileName, inx_wifi_station_state->tlsCert);
						canonicalClientKeyFileName[0] = '\0';
						EhsStrcat(canonicalClientKeyFileName, canonicalAppdataPath);
						EhsStrcat(canonicalClientKeyFileName, EHS_TD_FILES_SEPARATOR_STR);
						EhsStrcat(canonicalClientKeyFileName, inx_wifi_station_state->tlsKey);
					}
					if (inx_wifi_station_state->type == Type_WifiStation_Enterprise && inx_wifi_station_state->needServerCert == EHS_TRUE)
					{
						canonicalRootCAFileName[0] = '\0';
						EhsStrcat(canonicalRootCAFileName, canonicalAppdataPath);
						EhsStrcat(canonicalRootCAFileName, EHS_TD_FILES_SEPARATOR_STR);
						EhsStrcat(canonicalRootCAFileName, inx_wifi_station_state->serverCert);
					}

					EhsSleep(EHS_TIME_s(1));
					sWifiStationStatus = doWifiStationConnect(inx_wifi_station_state->SSID, 
					       inx_wifi_station_state->type, 
					       inx_wifi_station_state->PSKPass, 
					       inx_wifi_station_state->EntType, 
					       inx_wifi_station_state->EAP, 
					       inx_wifi_station_state->TTLS2, 
					       inx_wifi_station_state->needServerCert, 
					       canonicalRootCAFileName, 
					       canonicalClientCertFileName,
					       canonicalClientKeyFileName, 
					       inx_wifi_station_state->eapID, 
					       inx_wifi_station_state->eapUser, 
					       inx_wifi_station_state->eapPass, 
					       ip_address,
					       mac_address, 
					       &rssi);
					if (sWifiStationStatus == WifiStation_Connected)
					{
						if (EHS_FB_OUT_CONNECTED_API2(INX_wifi_station_ARG_internal_thread_ip_address))
							EhsStrncpy(EHS_FB_OUT_S_API2(INX_wifi_station_ARG_internal_thread_ip_address), ip_address, 16);
						if (EHS_FB_OUT_CONNECTED_API2(INX_wifi_station_ARG_internal_thread_rssi))
							EHS_FB_OUT_I_API2(INX_wifi_station_ARG_internal_thread_rssi) = rssi;
						if (EHS_FB_OUT_CONNECTED_API2(INX_wifi_station_ARG_internal_thread_ap_mac_address))
							EhsStrncpy(EHS_FB_OUT_S_API2(INX_wifi_station_ARG_internal_thread_ap_mac_address), mac_address, 18);
						EhsTPMutex_lock(EhsTPMutex_fbIO);
						EhsWifiStationSetCBSource(eWifiStationCallbackSource_Internal);
						EhsTPMutex_unlock(EhsTPMutex_fbIO);
						EHS_FB_FINISH(INX_wifi_station_ARG_internal_thread_connect_OK);
					}
					else
					{
						if (EHS_FB_OUT_CONNECTED_API2(INX_wifi_station_ARG_internal_thread_errCode))
							EHS_FB_OUT_I_API2(INX_wifi_station_ARG_internal_thread_errCode) = sWifiStationStatus;
						run = EHS_FALSE;
						EHS_FB_FINISH(INX_wifi_station_ARG_internal_thread_connectFail);
					}
					break;
				case eWifiStationCallbackSource_Internal:
					if (gEhsStatusWifiStationDisconnected == EHS_TRUE)
					{
						if (EHS_FB_OUT_CONNECTED_API2(INX_wifi_station_ARG_internal_thread_disconnect_reason))
							EHS_FB_OUT_I_API2(INX_wifi_station_ARG_internal_thread_disconnect_reason)  = EhsWifiStationReason;
						if (EHS_FB_OUT_CONNECTED_API2(INX_wifi_station_ARG_internal_thread_rssi))
							EHS_FB_OUT_I_API2(INX_wifi_station_ARG_internal_thread_rssi) = EhsWifiStationRssi;
						EHS_FB_FINISH(INX_wifi_station_ARG_internal_thread_disconnected);
						run = EHS_FALSE;
					}
					break;
				case eWifiStationCallbackSource_Disconnect:
					doWifiStationDisconnect();
					if (EHS_FB_OUT_CONNECTED_API2(INX_wifi_station_ARG_internal_thread_disconnect_reason))
						EHS_FB_OUT_I_API2(INX_wifi_station_ARG_internal_thread_disconnect_reason)  = 0;
					EHS_FB_FINISH(INX_wifi_station_ARG_internal_thread_disconnected);

					break;
				default: run = EHS_FALSE; break;
			}
		}
		EhsSleep(EHS_TIME_s(EHS_WIFISTATION_THREADLOOP_SLEEP_TIME_S));
	} while (run && sWifiStationCallbackSource_Internal < eWifiStationCallbackSource_Disconnect);
	Ehs_FB_ThreadComplete();
	EhsHThread_exit();
}

EHS_FB_IDENTIFY_FUNCTION(wifi_station)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_char* SSID;
	ehs_sint32 type;
	ehs_bool tryReconnect;
	ehs_sint32 retry;
	ehs_sint32 EntType;
	ehs_sint32 EAP;
	ehs_sint32 TTLS2;
	ehs_bool needServerCert;
	ehs_char* serverCert;
	ehs_char* tlsCert;
	ehs_char* tlsKey;
	ehs_char* eapID;
	ehs_char* eapUser;
	ehs_char* eapPass;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%s %d %d %d %d %d %d %d %s %s %s %s %s %s",&SSID,&type,&tryReconnect,&retry,&EntType,&EAP,&TTLS2,&needServerCert,&serverCert,&tlsCert,&tlsKey,&eapID,&eapUser,&eapPass); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_wifi_station_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(wifi_station)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_wifi_station_state_type* inx_wifi_station_state = (inx_wifi_station_state_type*)EHS_FB_INIT_CONTEXT;
	inx_wifi_station_state->pFIdata = NULL;

	EhsCallbackQueue_register(&xWifiStationCallbackQueue,
			   EHS_FB_RUN_NAME(wifi_station_internal_thread),
			   EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(-1),
			   &(inx_wifi_station_state->xEntry)
	);
//	inx_wifi_station_state->SSID = (ehs_char *) calloc(EHS_STRING_LENGTH_MAX, 1);
//	inx_wifi_station_state->PSKPass = (ehs_char *) calloc(EHS_STRING_LENGTH_MAX, 1);
//	inx_wifi_station_state->serverCert = (ehs_char *) calloc(EHS_STRING_LENGTH_MAX, 1);
//	inx_wifi_station_state->tlsCert = (ehs_char *) calloc(EHS_STRING_LENGTH_MAX, 1);
//	inx_wifi_station_state->tlsKey = (ehs_char *) calloc(EHS_STRING_LENGTH_MAX, 1);
//	inx_wifi_station_state->eapID = (ehs_char *) calloc(EHS_STRING_LENGTH_MAX, 1);
//	inx_wifi_station_state->eapUser = (ehs_char *) calloc(EHS_STRING_LENGTH_MAX, 1);
//	inx_wifi_station_state->eapPass = (ehs_char *) calloc(EHS_STRING_LENGTH_MAX, 1);
	/* read the initialisation parameters */
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"%s %d %d %d %d %d %d %d %s %s %s %s %s %s",inx_wifi_station_state->SSID,&(inx_wifi_station_state->type),&(inx_wifi_station_state->tryReconnect),&(inx_wifi_station_state->retry),&(inx_wifi_station_state->EntType),&(inx_wifi_station_state->EAP),&(inx_wifi_station_state->TTLS2),&(inx_wifi_station_state->needServerCert),inx_wifi_station_state->serverCert,inx_wifi_station_state->tlsCert,inx_wifi_station_state->tlsKey,inx_wifi_station_state->eapID,inx_wifi_station_state->eapUser,inx_wifi_station_state->eapPass);

	configWifiStationSetReconnect(inx_wifi_station_state->tryReconnect, inx_wifi_station_state->retry);

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
// static void freeNull(void **p)
// {
// 	if (*p != NULL)
// 	{
// 		free(*p);
// 		*p = NULL;
// 	}
// }
EHS_FB_DESTROY_FUNCTION(wifi_station)
{
	inx_wifi_station_state_type *inx_wifi_station_state = (inx_wifi_station_state_type*)EHS_FB_DESTROY_CONTEXT;
	doWifiStationDestroy();
//	freeNull(&inx_wifi_station_state->SSID);
//	freeNull(&inx_wifi_station_state->PSKPass);
//	freeNull(&inx_wifi_station_state->serverCert);
//	freeNull(&inx_wifi_station_state->tlsCert);
//	freeNull(&inx_wifi_station_state->tlsKey);
//	freeNull(&inx_wifi_station_state->eapID);
//	freeNull(&inx_wifi_station_state->eapUser);
//	freeNull(&inx_wifi_station_state->eapPass);
	//Your code below here
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION do_connect MACRO START -- DO NOT ALTER
/**
 * Definition of wifi_station_do_connect.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(wifi_station_do_connect)
{
	inx_wifi_station_state_type* inx_wifi_station_state = (inx_wifi_station_state_type*)EHS_FB_RUN_CONTEXT;
	EhsTPMutex_lock(EhsTPMutex_fbIO);
	EhsWifiStationSetCBSource(eWifiStationCallbackSource_Connect);
	EhsTPMutex_unlock(EhsTPMutex_fbIO);
	/********************* Test *********************/
	doWifiStationDisconnect();
	/********************* Test *********************/
	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_connect_auth_type))
		inx_wifi_station_state->type = EHS_FB_IN_I_API2(INX_wifi_station_ARG_do_connect_auth_type) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_connect_ssid))
		EhsStrcpy( inx_wifi_station_state->SSID, EHS_FB_IN_S_API2(INX_wifi_station_ARG_do_connect_ssid) ) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_connect_PSKPass))
		EhsStrcpy( inx_wifi_station_state->PSKPass, EHS_FB_IN_S_API2(INX_wifi_station_ARG_do_connect_PSKPass) ) ;
	else inx_wifi_station_state->PSKPass[0] = '\0';
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_connect_EAP))
		inx_wifi_station_state->EAP = EHS_FB_IN_I_API2(INX_wifi_station_ARG_do_connect_EAP) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_connect_Enterprise_type))
		inx_wifi_station_state->EntType = EHS_FB_IN_I_API2(INX_wifi_station_ARG_do_connect_Enterprise_type) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_connect_ttls_phase2))
		inx_wifi_station_state->TTLS2 = EHS_FB_IN_I_API2(INX_wifi_station_ARG_do_connect_ttls_phase2) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_connect_needCert))
		inx_wifi_station_state->needServerCert = EHS_FB_IN_B_API2(INX_wifi_station_ARG_do_connect_needCert) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_connect_serverCert))
		EhsStrcpy( inx_wifi_station_state->serverCert, EHS_FB_IN_S_API2(INX_wifi_station_ARG_do_connect_serverCert) ) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_connect_tlsCert))
		EhsStrcpy( inx_wifi_station_state->tlsCert, EHS_FB_IN_S_API2(INX_wifi_station_ARG_do_connect_tlsCert) ) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_connect_tlsKey))
		EhsStrcpy( inx_wifi_station_state->tlsKey, EHS_FB_IN_S_API2(INX_wifi_station_ARG_do_connect_tlsKey) ) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_connect_eapID))
		EhsStrcpy( inx_wifi_station_state->eapID, EHS_FB_IN_S_API2(INX_wifi_station_ARG_do_connect_eapID) ) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_connect_eapUser))
		EhsStrcpy( inx_wifi_station_state->eapUser, EHS_FB_IN_S_API2(INX_wifi_station_ARG_do_connect_eapUser) ) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_connect_eapPass))
		EhsStrcpy( inx_wifi_station_state->eapPass, EHS_FB_IN_S_API2(INX_wifi_station_ARG_do_connect_eapPass) ) ;
	//EhsCallbackQueue_execute(&xWifiStationCallbackQueue);
	EHS_FB_START_RUN_FUNCTION(wifi_station_internal_thread);
	EHS_FB_FINISH(INX_wifi_station_ARG_do_connect_do_connect_OK);
}//ICB FUNCTION do_connect MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION do_disconnect MACRO START -- DO NOT ALTER
/**
 * Definition of wifi_station_do_disconnect.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(wifi_station_do_disconnect)
{
	inx_wifi_station_state_type* inx_wifi_station_state = (inx_wifi_station_state_type*)EHS_FB_RUN_CONTEXT;
	EhsTPMutex_lock(EhsTPMutex_fbIO);
	EhsWifiStationSetCBSource(eWifiStationCallbackSource_Disconnect);
	EhsTPMutex_unlock(EhsTPMutex_fbIO);

	// Your code here
	EHS_FB_FINISH(INX_wifi_station_ARG_do_disconnect_do_disconnect_OK);
}//ICB FUNCTION do_disconnect MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION internal_thread MACRO START -- DO NOT ALTER
/**
 * Definition of wifi_station_internal_thread.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(wifi_station_internal_thread)
{
	inx_wifi_station_state_type* inx_wifi_station_state = (inx_wifi_station_state_type*)EHS_FB_RUN_CONTEXT;
	inx_wifi_station_state->pFIdata = EHS_FB_RUN_CONTEXT_REF;
	EhsWifiStationSetState(inx_wifi_station_state);

	// Your code here
	EHS_FB_START_THREAD(wifi_station_thread, -99);
	
}//ICB FUNCTION internal_thread MACRO END -- DO NOT ALTER THIS LINE

void Common_WifiStation_onDisconnected(ehs_bool disconnected, ehs_uint8 reason, ehs_sint32 rssi)
{
	gEhsStatusWifiStationDisconnected = disconnected;
	EhsWifiStationReason = (ehs_sint32) reason;
	EhsWifiStationRssi = rssi;
}
