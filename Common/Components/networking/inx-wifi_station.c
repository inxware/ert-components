//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-wifi_station.h"
#include "wifi_station.h"
#include <stdio.h>
#include <stdlib.h>

#include "hal_nvs.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_wifi_station_state
{
	EhsCallbackQueueEntryType xEntry;
	ehs_char PSKPass[EHS_STRING_LENGTH_MAX];
	EhsFunctionInstanceDataType* pFIdata;
	ehs_char SSID[EHS_STRING_LENGTH_MAX];
	ehs_bool onStartup;
	ehs_sint32 type;
	ehs_bool tryReconnect;
	ehs_sint32 retry;
	ehs_sint32 reconnectPeriod;
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

static EhsDataflowStringType EhsWifiStationIpAddr = NULL;
static EhsDataflowStringType EhsWifiStationApMac = NULL;
static EhsDataflowIntType EhsWifiStationRssi = 0;
static EhsDataflowIntType EhsWifiStationErrCode = 0;
static EhsDataflowIntType EhsWifiStationReason = 0;
static volatile enum eWifiStationCallbackSource sWifiStationCallbackSource = eWifiStationCallbackSource_Connect;
static EhsCallbackQueueType xWifiStationCallbackQueue;

static ehs_bool gEhsStatusWifiStationDisconnected = EHS_FALSE;

// used for the wifi connecting state machine
static volatile eWifiStationConnectState gEhsWifiStationConnectState = WifiStationConnectState_IDLE;

static volatile ehs_bool gEhsWiFiThreadRunning = EHS_FALSE;

static ehs_bool gEhsWiFiManagedByComponent = EHS_FALSE;

static ehs_bool gEhsWifiStationFirstTime = EHS_TRUE;

static inx_wifi_station_state_type gDefaultWifiStationState =
{
	.xEntry = {0},
	#ifdef EHS_CONFIG_WIFI_PASSWORD
	.PSKPass = EHS_CONFIG_WIFI_PASSWORD,
	#else
	.PSKPass = "",
	#endif
	.pFIdata = NULL,
	#ifdef EHS_CONFIG_WIFI_SSID
	.SSID = EHS_CONFIG_WIFI_SSID,
	#else
	.SSID = "",
	#endif
	.type = Type_WifiStation_PSK,
	.tryReconnect = EHS_TRUE,
	.retry = 10,
	.reconnectPeriod = 300,
	.EntType = 0,
	.EAP = 0,
	.TTLS2 = 0,
	.needServerCert = EHS_FALSE,
	.serverCert = "",
	.tlsCert = "",
	.tlsKey = "",
	.eapID = "",
	.eapUser = "",
	.eapPass = ""
};


#define EHS_WIFISTATION_THREADLOOP_SLEEP_TIME_S 1

#ifndef EHS_WIFISTATION_MAX_AP_RECORDS
#define EHS_WIFISTATION_MAX_AP_RECORDS 20
#endif

/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
/* TODO: Add "do_stop_scan" function (ERT1_ID=0x05) to allow stopping WiFi scans from the function block.
 *       The HAL function doWifiStationScanStop() is already implemented in target_wifi.c.
 *       This requires updating the CDF file with new Function and Port definitions. */
EHS_FB_FUNCTIONS_START(wifi_station)
EHS_FB_FUNCTION_ENTRY("do_connect", 0x01, wifi_station_do_connect)
EHS_FB_FUNCTION_ENTRY("do_disconnect", 0x02, wifi_station_do_disconnect)
EHS_FB_FUNCTION_ENTRY("internal_thread", 0x03, wifi_station_internal_thread)
EHS_FB_FUNCTION_ENTRY("do_set", 0x04, wifi_station_do_set)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
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
#define INX_wifi_station_ARG_do_set_ssid 1
#define INX_wifi_station_ARG_do_set_auth_type 2
#define INX_wifi_station_ARG_do_set_PSKPass 3
#define INX_wifi_station_ARG_do_set_Enterprise_type 4
#define INX_wifi_station_ARG_do_set_EAP 5
#define INX_wifi_station_ARG_do_set_ttls_phase2 6
#define INX_wifi_station_ARG_do_set_needCert 7
#define INX_wifi_station_ARG_do_set_serverCert 8
#define INX_wifi_station_ARG_do_set_tlsCert 9
#define INX_wifi_station_ARG_do_set_tlsKey 10
#define INX_wifi_station_ARG_do_set_eapID 11
#define INX_wifi_station_ARG_do_set_eapUser 12
#define INX_wifi_station_ARG_do_set_eapPass 13
#define INX_wifi_station_ARG_do_set_set_ok 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_wifi_station_SSID 
#define INX_FB_wifi_station_onStartup 1
#define INX_FB_wifi_station_type 0
#define INX_FB_wifi_station_tryReconnect 0
#define INX_FB_wifi_station_retry 1
#define INX_FB_wifi_station_reconnectPeriod 300
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

void EhsWifiStationSetCBSource(volatile enum eWifiStationCallbackSource source)
{
	gEhsWifiStationFirstTime = EHS_TRUE;
	sWifiStationCallbackSource = source;
}
static volatile enum eWifiStationCallbackSource EhsWifiStationGetCBSource()
{
	return sWifiStationCallbackSource;
}

const char* WifiStationConnectStateString(eWifiStationConnectState state)
{
	const char* state_str = "WifiStationConnectState_UNKNOWN";
	switch (state)
	{
	case WifiStationConnectState_IDLE:
		state_str = "WifiStationConnectState_IDLE";
		break;
	case WifiStationConnectState_INIT:
		state_str = "WifiStationConnectState_INIT";
		break;
	case WifiStationConnectState_CONFIGURE:
		state_str = "WifiStationConnectState_CONFIGURE";
		break;
	case WifiStationConnectState_CONNECT:
		state_str = "WifiStationConnectState_CONNECT";
		break;
	case WifiStationConnectState_CONNECTING:
		state_str = "WifiStationConnectState_CONNECTING";
		break;
	case WifiStationConnectState_CONNECTING_GOT_IP:
		state_str = "WifiStationConnectState_CONNECTING_GOT_IP";
		break;
	case WifiStationConnectState_CONNECTED:
		state_str = "WifiStationConnectState_CONNECTED";
		break;
	case WifiStationConnectState_FAILED:
		state_str = "WifiStationConnectState_FAILED";
		break;
	default:
		break;
	};
	return state_str;
}

eWifiStationConnectState getWifiStationConnectState()
{
	return gEhsWifiStationConnectState;
}

void setWifiStationConnectState(eWifiStationConnectState state)
{
	gEhsWifiStationConnectState = state;
#if EHS_ESP32_DISABLE_LOGS != 1
	printf("********* %s *********\n",WifiStationConnectStateString(state));
#endif
}

ehs_bool isEhsWiFiManagedByComponent()
{
	return EHS_FALSE;
    // TODO We allow any other method of managing WiFi in addition to the app - the above is a hack and should be not be called at all instead.
//	return gEhsWiFiManagedByComponent;
}

EHS_FB_THREAD_FUNCTION(wifi_station_thread)
{
	Ehs_FB_ThreadStarted();
	enum eWifiStationCallbackSource sWifiStationCallbackSource_Internal;
	inx_wifi_station_state_type* inx_wifi_station_state;
	eWifiStationStatus sWifiStationStatus = WifiStation_Disconnected;
	ehs_bool run = EHS_TRUE;
	ehs_char ip_address[20];
	ehs_char mac_address[20];
	ehs_sint32 rssi;

	ehs_uint32 retry_num = 0;
	EhsTickType current_time = 0;
	ehs_bool reconnect_in_progress = EHS_FALSE;
	ehs_bool proceed_action = EHS_FALSE;

	ehs_char canonicalAppdataPath[EHS_STRING_LENGTH_MAX];
	ehs_char canonicalClientCertFileName[EHS_STRING_LENGTH_MAX];
	ehs_char canonicalClientKeyFileName[EHS_STRING_LENGTH_MAX];
	ehs_char canonicalRootCAFileName[EHS_STRING_LENGTH_MAX];
	do
	{
		inx_wifi_station_state = EhsWifiStationGetState();
		if (inx_wifi_station_state == NULL)
		{
			inx_wifi_station_state = &gDefaultWifiStationState;
		}
		pFIdata = inx_wifi_station_state->pFIdata;
		sWifiStationCallbackSource_Internal = EhsWifiStationGetCBSource();
		//if (inx_wifi_station_state != NULL && inx_wifi_station_state->pFIdata != NULL)
		{
			switch (sWifiStationCallbackSource_Internal){
				case eWifiStationCallbackSource_Connect:
					// reset retry counter
					retry_num = 0;
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
					// for (int i = 0 ; i < EhsStrlen(inx_wifi_station_state->PSKPass) + 1 ; i++)
					// {
					// 	if (inx_wifi_station_state->PSKPass[i] == 0) printf("<EOL>");
					// 	else printf("%c", inx_wifi_station_state->PSKPass[i]);
					// }
					// printf("]\n");
					sWifiStationStatus = doWifiStationStart(inx_wifi_station_state->SSID, 
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
						gEhsStatusWifiStationDisconnected = EHS_FALSE;
						if (inx_wifi_station_state != NULL && inx_wifi_station_state->pFIdata != NULL)
						{
							if (EHS_FB_OUT_CONNECTED_API2(INX_wifi_station_ARG_internal_thread_ip_address))
								EhsStrncpy(EHS_FB_OUT_S_API2(INX_wifi_station_ARG_internal_thread_ip_address), ip_address, 16);
							if (EHS_FB_OUT_CONNECTED_API2(INX_wifi_station_ARG_internal_thread_rssi))
								EHS_FB_OUT_I_API2(INX_wifi_station_ARG_internal_thread_rssi) = rssi;
							if (EHS_FB_OUT_CONNECTED_API2(INX_wifi_station_ARG_internal_thread_ap_mac_address))
								EhsStrncpy(EHS_FB_OUT_S_API2(INX_wifi_station_ARG_internal_thread_ap_mac_address), mac_address, 18);
						}
						EhsWifiStationSetCBSource(eWifiStationCallbackSource_Internal);
						if (inx_wifi_station_state != NULL && inx_wifi_station_state->pFIdata != NULL)
						{
							EHS_FB_FINISH(INX_wifi_station_ARG_internal_thread_connect_OK);
						}
					}
					else if (sWifiStationStatus == WifiStation_Connecting)
					{
						// still connecting, wait for next event
					}
					else
					{
						EhsWifiStationSetCBSource(eWifiStationCallbackSource_Internal);
						if (inx_wifi_station_state != NULL && inx_wifi_station_state->pFIdata != NULL)
						{
							if (EHS_FB_OUT_CONNECTED_API2(INX_wifi_station_ARG_internal_thread_errCode))
								EHS_FB_OUT_I_API2(INX_wifi_station_ARG_internal_thread_errCode) = sWifiStationStatus;
							//run = EHS_FALSE;
							EHS_FB_FINISH(INX_wifi_station_ARG_internal_thread_connectFail);
						}
					}
					break;
				case eWifiStationCallbackSource_Scan:
					if (gEhsWifiStationFirstTime) 
					{
						proceed_action = EHS_TRUE;
						gEhsWifiStationFirstTime = EHS_FALSE;
					}
					if (retry_num < inx_wifi_station_state->retry && inx_wifi_station_state->tryReconnect == EHS_TRUE)
					{
						retry_num++;
						proceed_action = EHS_TRUE;
					}
					else
					{
						if (reconnect_in_progress == EHS_FALSE)
						{
							reconnect_in_progress = EHS_TRUE;
							current_time = EHS_CURRENT_TIME;
						}
						else
						{
							if ((EHS_CURRENT_TIME - current_time) >= EhsTgtTimer_msToTick(1000 * inx_wifi_station_state->reconnectPeriod))
							{
								proceed_action = EHS_TRUE;
								reconnect_in_progress = EHS_FALSE;
							}
						}
					}
					if (proceed_action)
					{
						EhsWifiStationSetCBSource(eWifiStationCallbackSource_Internal);
						doWifiStationScan(inx_wifi_station_state->SSID);
					}
					proceed_action = EHS_FALSE;
					break;
				case eWifiStationCallbackSource_ScanResult:
					ehs_char ssid[EHS_STRING_LENGTH_MAX];
					ehs_char bssid[6];
					ehs_sint32 channel;
					ehs_sint32 scan_rssi;
					ehs_uint32 index =0;
					ehs_sint32 num_ap = WifiStationScanResultCount();
					for (index = 0 ; index < num_ap ; index++)
					{
						if (WifiStationScanResult(index, ssid, EHS_STRING_LENGTH_MAX, bssid, 6, &channel, &scan_rssi) == EHS_TRUE)
						{
							//printf("SSID: %s, BSSID: %02X:%02X:%02X:%02X:%02X:%02X\n", ssid, bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
							if (inx_wifi_station_state == NULL) break;
							if (inx_wifi_station_state->SSID == NULL) break;
							if (EhsStrcmp(inx_wifi_station_state->SSID, ssid) == 0)
							{
								EhsWifiStationSetCBSource(eWifiStationCallbackSource_Internal);
								doWifiStationConnect(bssid, channel);
								break;
							}
						}
						else
						{
							EhsWifiStationSetCBSource(eWifiStationCallbackSource_Scan);
							break;
						}
					}
					break;
				case eWifiStationCallbackSource_Reconnect:
					if (gEhsWifiStationFirstTime) 
					{
						proceed_action = EHS_TRUE;
						gEhsWifiStationFirstTime = EHS_FALSE;
					}
					if (retry_num < inx_wifi_station_state->retry && inx_wifi_station_state->tryReconnect == EHS_TRUE)
					{
						retry_num++;
						proceed_action = EHS_TRUE;
					}
					else
					{
						if (reconnect_in_progress == EHS_FALSE)
						{
							reconnect_in_progress = EHS_TRUE;
							current_time = EHS_CURRENT_TIME;
						}
						else
						{
							if ((EHS_CURRENT_TIME - current_time) >= EhsTgtTimer_msToTick(1000 * inx_wifi_station_state->reconnectPeriod))
							{
								proceed_action = EHS_TRUE;
								reconnect_in_progress = EHS_FALSE;
							}
						}
					}
					if (proceed_action)
					{
						EhsWifiStationSetCBSource(eWifiStationCallbackSource_Internal);
						//doWifiStationDisconnect();
						doWifiStationConnect(NULL, 0);
					}
					proceed_action = EHS_FALSE;
					break;
				case eWifiStationCallbackSource_Internal:
					break;
				case eWifiStationCallbackSource_Connected:
					retry_num = 0;
					gEhsStatusWifiStationDisconnected = EHS_FALSE;
					if (inx_wifi_station_state != NULL && inx_wifi_station_state->pFIdata != NULL)
					{
						EHS_FB_FINISH(INX_wifi_station_ARG_internal_thread_connect_OK);
					}
					break;
				case eWifiStationCallbackSource_Disconnect:
					doWifiStationDestroy();
					if (inx_wifi_station_state != NULL && inx_wifi_station_state->pFIdata != NULL)
					{
						if (EHS_FB_OUT_CONNECTED_API2(INX_wifi_station_ARG_internal_thread_disconnect_reason))
							EHS_FB_OUT_I_API2(INX_wifi_station_ARG_internal_thread_disconnect_reason)  = 0;
						EHS_FB_FINISH(INX_wifi_station_ARG_internal_thread_disconnected);
					}
					run = EHS_FALSE;
					break;
				default: run = EHS_FALSE; break;
			}
		}
		EhsSleep(EHS_TIME_s(EHS_WIFISTATION_THREADLOOP_SLEEP_TIME_S));
	} while (run && sWifiStationCallbackSource_Internal < eWifiStationCallbackSource_Disconnect);
	gEhsWiFiThreadRunning = EHS_FALSE;
	Ehs_FB_ThreadComplete();
	EhsHThread_exit();
}

EHS_FB_IDENTIFY_FUNCTION(wifi_station)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_char* SSID;
	ehs_bool onStartup;
	ehs_sint32 type;
	ehs_bool tryReconnect;
	ehs_sint32 retry;
	ehs_sint32 reconnectPeriod;
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
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%s %d %d %d %d %d %d %d %d %d %s %s %s %s %s %s",&SSID,&onStartup,&type,&tryReconnect,&retry,&reconnectPeriod,&EntType,&EAP,&TTLS2,&needServerCert,&serverCert,&tlsCert,&tlsKey,&eapID,&eapUser,&eapPass); */
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
	ehs_nvs_obj_t nvs;
	gEhsWiFiManagedByComponent = EHS_TRUE;

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
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d %d %d %d %d",&(inx_wifi_station_state->onStartup),&(inx_wifi_station_state->type),&(inx_wifi_station_state->tryReconnect),&(inx_wifi_station_state->retry),&(inx_wifi_station_state->reconnectPeriod));
		
	EhsStrcpy(inx_wifi_station_state->SSID, gDefaultWifiStationState.SSID);
	inx_wifi_station_state->EntType = gDefaultWifiStationState.EntType;
	inx_wifi_station_state->EAP = gDefaultWifiStationState.EAP;
	inx_wifi_station_state->TTLS2 = gDefaultWifiStationState.TTLS2;
	inx_wifi_station_state->needServerCert = gDefaultWifiStationState.needServerCert;
	EhsStrcpy(inx_wifi_station_state->serverCert, gDefaultWifiStationState.serverCert);
	EhsStrcpy(inx_wifi_station_state->tlsCert, gDefaultWifiStationState.tlsCert);
	EhsStrcpy(inx_wifi_station_state->tlsKey, gDefaultWifiStationState.tlsKey);
	EhsStrcpy(inx_wifi_station_state->eapID, gDefaultWifiStationState.eapID);
	EhsStrcpy(inx_wifi_station_state->eapUser, gDefaultWifiStationState.eapUser);
	EhsStrcpy(inx_wifi_station_state->eapPass, gDefaultWifiStationState.eapPass);

	EhsNvsOpen(&nvs, EHS_NVS_WIFI_NAMESPACE);
	EhsNvsSetBool(&nvs, EHS_NVS_WIFI_KEY_ONSTARTUP, inx_wifi_station_state->onStartup);
	EhsNvsSetBool(&nvs, EHS_NVS_WIFI_KEY_TRYRECONNECT, inx_wifi_station_state->tryReconnect);
	EhsNvsSetUInt32(&nvs, EHS_NVS_WIFI_KEY_RETRY, (ehs_uint32) inx_wifi_station_state->retry);
	EhsNvsSetUInt32(&nvs, EHS_NVS_WIFI_KEY_RECONNECTPERIOD, (ehs_uint32) inx_wifi_station_state->reconnectPeriod);
	EhsNvsCommit(&nvs);
	EhsNvsClose(&nvs);

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
	//doWifiStationDestroy();
	gEhsWiFiManagedByComponent = EHS_FALSE;
//	freeNull(&inx_wifi_station_state->SSID);
//	freeNull(&inx_wifi_station_state->PSKPass);
//	freeNull(&inx_wifi_station_state->serverCert);
//	freeNull(&inx_wifi_station_state->tlsCert);
//	freeNull(&inx_wifi_station_state->tlsKey);
//	freeNull(&inx_wifi_station_state->eapID);
//	freeNull(&inx_wifi_station_state->eapUser);
//	freeNull(&inx_wifi_station_state->eapPass);
	//Your code below here
	return EHS_TRUE;
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
	/********************* Test *********************/
	/********************* Test *********************/
	// Your code here
	doWifiStationDisconnect();
	setWifiStationConnectState(WifiStationConnectState_CONNECT);
	EhsTPMutex_lock(EhsTPMutex_fbIO);
	EhsWifiStationSetCBSource(eWifiStationCallbackSource_Connect);
	EhsTPMutex_unlock(EhsTPMutex_fbIO);
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
// Wi-Fi Station internal thread needs a bigger stack size
#ifdef EHS_THREAD_USE_DEFAULT_STACK_SIZE
#undef EHS_THREAD_USE_DEFAULT_STACK_SIZE
#endif//EHS_THREAD_USE_DEFAULT_STACK_SIZE
#define EHS_THREAD_USE_DEFAULT_STACK_SIZE (4096+1024)
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
	if (gEhsWiFiThreadRunning == EHS_TRUE) return;
	gEhsWiFiThreadRunning = EHS_TRUE;
	EHS_FB_START_THREAD(wifi_station_thread, -99);
	
}//ICB FUNCTION internal_thread MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION do_set MACRO START -- DO NOT ALTER
/**
 * Definition of wifi_station_do_set.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(wifi_station_do_set)
{
	inx_wifi_station_state_type* inx_wifi_station_state = (inx_wifi_station_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_set_auth_type))
		inx_wifi_station_state->type = EHS_FB_IN_I_API2(INX_wifi_station_ARG_do_set_auth_type) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_set_ssid))
		EhsStrcpy( inx_wifi_station_state->SSID, EHS_FB_IN_S_API2(INX_wifi_station_ARG_do_set_ssid) ) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_set_PSKPass))
		EhsStrcpy( inx_wifi_station_state->PSKPass, EHS_FB_IN_S_API2(INX_wifi_station_ARG_do_set_PSKPass) ) ;
	else inx_wifi_station_state->PSKPass[0] = '\0';
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_set_EAP))
		inx_wifi_station_state->EAP = EHS_FB_IN_I_API2(INX_wifi_station_ARG_do_set_EAP) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_set_Enterprise_type))
		inx_wifi_station_state->EntType = EHS_FB_IN_I_API2(INX_wifi_station_ARG_do_set_Enterprise_type) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_set_ttls_phase2))
		inx_wifi_station_state->TTLS2 = EHS_FB_IN_I_API2(INX_wifi_station_ARG_do_set_ttls_phase2) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_set_needCert))
		inx_wifi_station_state->needServerCert = EHS_FB_IN_B_API2(INX_wifi_station_ARG_do_set_needCert) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_set_serverCert))
		EhsStrcpy( inx_wifi_station_state->serverCert, EHS_FB_IN_S_API2(INX_wifi_station_ARG_do_set_serverCert) ) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_set_tlsCert))
		EhsStrcpy( inx_wifi_station_state->tlsCert, EHS_FB_IN_S_API2(INX_wifi_station_ARG_do_set_tlsCert) ) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_set_tlsKey))
		EhsStrcpy( inx_wifi_station_state->tlsKey, EHS_FB_IN_S_API2(INX_wifi_station_ARG_do_set_tlsKey) ) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_set_eapID))
		EhsStrcpy( inx_wifi_station_state->eapID, EHS_FB_IN_S_API2(INX_wifi_station_ARG_do_set_eapID) ) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_set_eapUser))
		EhsStrcpy( inx_wifi_station_state->eapUser, EHS_FB_IN_S_API2(INX_wifi_station_ARG_do_set_eapUser) ) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_wifi_station_ARG_do_set_eapPass))
		EhsStrcpy( inx_wifi_station_state->eapPass, EHS_FB_IN_S_API2(INX_wifi_station_ARG_do_set_eapPass) ) ;
	//EhsCallbackQueue_execute(&xWifiStationCallbackQueue);
	{
		ehs_nvs_obj_t nvs;
		EhsNvsOpen(&nvs, EHS_NVS_WIFI_NAMESPACE);
		EhsNvsSetString(&nvs, EHS_NVS_WIFI_KEY_SSID, inx_wifi_station_state->SSID, EhsStrlen(inx_wifi_station_state->SSID));
		EhsNvsSetString(&nvs, EHS_NVS_WIFI_KEY_PASS, inx_wifi_station_state->PSKPass, EhsStrlen(inx_wifi_station_state->PSKPass));
		EhsNvsCommit(&nvs);
		EhsNvsClose(&nvs);
	}
	EHS_FB_FINISH(INX_wifi_station_ARG_do_set_set_ok);
}//ICB FUNCTION do_set MACRO END -- DO NOT ALTER THIS LINE


void Common_WifiStation_onDisconnected(ehs_bool disconnected, ehs_uint8 reason, ehs_sint32 rssi)
{
	gEhsStatusWifiStationDisconnected = disconnected;
	EhsWifiStationReason = (ehs_sint32) reason;
	EhsWifiStationRssi = rssi;
}

#define _EHS_NVS_GOTO_ON_ERROR(exec, label) do { \
	ret = (exec); \
	if (ret != eEhsNvsOk) { \
		printf("[%s] %s failed: %d\n", __func__, #exec, ret); \
		goto label; \
	} \
} while(0)

void EhsStartWifiStationThread()
{
	EhsFunctionInstanceDataType* pFIdata = NULL;
	ehs_nvs_obj_t obj;
	size_t ssid_required = EHS_STRING_LENGTH_MAX;
	size_t pass_required = EHS_STRING_LENGTH_MAX;
	eEhsNvsError_t ret;
	ehs_uint32 reconnectPeriod = INX_FB_wifi_station_reconnectPeriod;
	gDefaultWifiStationState.tryReconnect = INX_FB_wifi_station_tryReconnect;
	gDefaultWifiStationState.retry = INX_FB_wifi_station_retry;
	//gEhsWiFiManagedByComponent = EHS_TRUE;
	_EHS_NVS_GOTO_ON_ERROR(EhsNvsOpen(&obj, EHS_NVS_WIFI_NAMESPACE), error);
	_EHS_NVS_GOTO_ON_ERROR(EhsNvsGetString(&obj, EHS_NVS_WIFI_KEY_SSID, NULL, &ssid_required), error_opened);
	_EHS_NVS_GOTO_ON_ERROR(EhsNvsGetString(&obj, EHS_NVS_WIFI_KEY_SSID, gDefaultWifiStationState.SSID, &ssid_required), error_opened);
	_EHS_NVS_GOTO_ON_ERROR(EhsNvsGetString(&obj, EHS_NVS_WIFI_KEY_PASS, NULL, &pass_required), error_opened);
	_EHS_NVS_GOTO_ON_ERROR(EhsNvsGetString(&obj, EHS_NVS_WIFI_KEY_PASS, gDefaultWifiStationState.PSKPass, &pass_required), error_opened);
	_EHS_NVS_GOTO_ON_ERROR(EhsNvsGetUInt32(&obj, EHS_NVS_WIFI_KEY_RECONNECTPERIOD, &reconnectPeriod), error_opened);
	_EHS_NVS_GOTO_ON_ERROR(EhsNvsGetBool(&obj, EHS_NVS_WIFI_KEY_TRYRECONNECT, &gDefaultWifiStationState.tryReconnect), error_opened);
	_EHS_NVS_GOTO_ON_ERROR(EhsNvsGetUInt32(&obj, EHS_NVS_WIFI_KEY_RETRY, &gDefaultWifiStationState.retry), error_opened);
error_opened:
	_EHS_NVS_GOTO_ON_ERROR(EhsNvsClose(&obj), error);
error:
	gDefaultWifiStationState.reconnectPeriod = (ehs_sint32)reconnectPeriod;
	if (gpInxWifiStationState != NULL) {
		EhsStrcpy(gpInxWifiStationState->SSID, gDefaultWifiStationState.SSID);
		EhsStrcpy(gpInxWifiStationState->PSKPass, gDefaultWifiStationState.PSKPass);
	}
	setWifiStationConnectState(WifiStationConnectState_CONNECT);
	EhsWifiStationSetCBSource(eWifiStationCallbackSource_Connect);
	//EHS_FB_START_RUN_FUNCTION(wifi_station_internal_thread);
	if (gEhsWiFiThreadRunning == EHS_TRUE) return;
	gEhsWiFiThreadRunning = EHS_TRUE;
	EHS_FB_START_THREAD(wifi_station_thread, -99);
	return;
//error_opened:
//	EhsNvsClose(&obj);
//error:
//	return;
}

ehs_sint32 WifiStationSetSSIDPSK(ehs_char* ssid, ehs_uint16 ssid_size, ehs_char* psk, ehs_uint16 psk_size)
{
	{
		ehs_nvs_obj_t nvs;
		ehs_uint32 reconnectPeriod = INX_FB_wifi_station_reconnectPeriod;
		EhsNvsOpen(&nvs, EHS_NVS_WIFI_NAMESPACE);
		EhsNvsSetString(&nvs, EHS_NVS_WIFI_KEY_SSID, ssid, ssid_size);
		EhsNvsSetString(&nvs, EHS_NVS_WIFI_KEY_PASS, psk, psk_size);
		EhsNvsSetBool(&nvs, EHS_NVS_WIFI_KEY_ONSTARTUP, 1);
		EhsNvsSetBool(&nvs, EHS_NVS_WIFI_KEY_TRYRECONNECT, 1);
		EhsNvsSetUInt32(&nvs, EHS_NVS_WIFI_KEY_RETRY, INX_FB_wifi_station_retry);
		if (EhsNvsGetUInt32(&nvs, EHS_NVS_WIFI_KEY_RECONNECTPERIOD, &reconnectPeriod) != 0)
			EhsNvsSetUInt32(&nvs, EHS_NVS_WIFI_KEY_RECONNECTPERIOD, reconnectPeriod);
		EhsNvsCommit(&nvs);
		EhsNvsClose(&nvs);
	}
	doWifiStationDisconnect();
	EhsStartWifiStationThread();
}

void WifiStationGetCurrentSsid(ehs_char *ssid)
{
	if (ssid == NULL) return;

	if (gpInxWifiStationState != NULL)
	{
		EhsStrcpy(ssid, gpInxWifiStationState->SSID);
	}
	else
	{
		EhsStrcpy(ssid, gDefaultWifiStationState.SSID);
	}
}
