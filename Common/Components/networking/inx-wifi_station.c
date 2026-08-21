/* Module-scoped logger ID. Must precede every include: several headers pull in
 * hal_logger.h, which latches EHSL_MODULE_ID to UNDEFINED if it is not set yet. */
#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-wifi_station.h"
#include "wifi_station.h"
#include <stdio.h>
#include <stdlib.h>

#include "hal_nvs.h"
//ICB HEADER MACRO END -- DO NOT ALTER

/* Diagnostic output (scan attempts, misses, reason codes) goes through EHSH_LOG_* and
 * can be filtered at runtime via EhsHLogger_setLogLevel("Network", ...).
 * Nothing here writes to the tools console - that queue is kernel-only. */
#include "hal_logger.h"
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

/* Bumped by Common_WifiStation_onDisconnected() on every STA_DISCONNECTED
 * event.  The Reconnect case compares against its last-seen value to print
 * the reason code exactly once per unique disconnect (not every thread
 * iteration while we're retrying). */
static volatile ehs_uint32 gWifiDisconnectSeq = 0;

/* Minimum retry count for scan/reconnect bursts.  The CDF default
 * (INX_FB_wifi_station_retry = 1) is nonsensical now that retry_num is
 * actually enforced — one attempt per burst is nowhere near enough given
 * that unfiltered active scans can legitimately miss an AP on a given pass.
 * If the user / NVS / FB wants more they can still set a higher value. */
#define WIFI_SM_MIN_RETRY_COUNT  10U

/* Cap the post-burst back-off.  The CDF default (300 s / 5 min) makes the
 * console feel dead for a long time after the fast retries exhaust; 30 s is
 * long enough to avoid hammering the AP and short enough to be usable. */
#define WIFI_SM_MAX_RECONNECT_PERIOD_S  30U

/* Map ESP-IDF wifi_err_reason_t values to short human-readable text.
 * Values are stable across ESP-IDF 4.x/5.x (see esp_wifi_types.h).
 * Returns "" for unknown codes so callers can concatenate unconditionally. */
static const char* WifiDisconnectReasonText(ehs_sint32 reason)
{
	switch (reason) {
		/* 802.11 spec-defined reasons (codes 1..39) */
		case 1:   return "unspecified";
		case 2:   return "auth expired — wrong password or AP timed out";
		case 3:   return "peer left / deauth";
		case 4:   return "association timed out — AP unreachable";
		case 5:   return "AP too many clients";
		case 6:   return "not authenticated (state error)";
		case 7:   return "not associated (state error)";
		case 8:   return "deauth: leaving network";
		case 9:   return "not authenticated for association";
		case 13:  return "invalid IE";
		case 14:  return "MIC failure";
		case 15:  return "4-way handshake timeout — wrong password (WPA/WPA2)";
		case 16:  return "group key update timeout";
		case 17:  return "IE in 4-way differs — config mismatch";
		case 18:  return "group cipher invalid";
		case 19:  return "pairwise cipher invalid";
		case 20:  return "AKMP invalid";
		case 23:  return "802.1X auth failed";
		case 24:  return "cipher suite rejected";
		/* ESP-IDF internal / extended reasons (200+) */
		case 200: return "beacon timeout — AP stopped responding";
		case 201: return "no AP found — AP out of range or off-air";
		case 202: return "auth failed — wrong password or incompatible auth mode";
		case 203: return "association failed";
		case 204: return "handshake timeout — wrong password (WPA2/WPA3)";
		case 205: return "connection failed";
		case 206: return "AP TSF reset";
		case 207: return "roaming";
		default:  return "see esp_wifi_types.h wifi_err_reason_t";
	}
}

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

/* Connection state-machine timeout constants (WIFI_SM_TIMEOUT_*_MS) now live
 * in wifi_station.h: they are part of the cross-target contract, and target
 * HALs that need a driver-level deadline have to derive it from them rather
 * than hard-code a number that happens to fit one platform. */

static volatile ehs_bool gWifiSmTimedOut = EHS_FALSE;

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
enum eWifiStationCallbackSource EhsWifiStationGetCBSource(void)
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
	/* SCANNING/ASSOCIATING/AUTHENTICATED/RECONNECTING/DISCONNECTING were
	 * missing, so every one of them logged as "..._UNKNOWN" - which is most of
	 * a connect attempt, and exactly the part you need to read when one goes
	 * wrong. Keep this switch exhaustive against eWifiStationConnectState. */
	case WifiStationConnectState_SCANNING:
		state_str = "WifiStationConnectState_SCANNING";
		break;
	case WifiStationConnectState_ASSOCIATING:
		state_str = "WifiStationConnectState_ASSOCIATING";
		break;
	case WifiStationConnectState_AUTHENTICATED:
		state_str = "WifiStationConnectState_AUTHENTICATED";
		break;
	case WifiStationConnectState_RECONNECTING:
		state_str = "WifiStationConnectState_RECONNECTING";
		break;
	case WifiStationConnectState_DISCONNECTING:
		state_str = "WifiStationConnectState_DISCONNECTING";
		break;
	case WifiStationConnectState_MAX:
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
	/* No printf here — this is called from the wifi/IP event-handler in the
	 * sys_evt task (~2.3 KB stack on ESP-IDF). A raw printf carves ~1 KB out
	 * of that for its format buffer and previously caused silent overflows.
	 * Route diagnostics through the runtime logger, which uses a fixed
	 * internal buffer rather than the caller's stack. */
	EHSH_LOG_INFO("WiFi SM → %s", WifiStationConnectStateString(state));
}

ehs_bool WifiStationIsConnectTimedOut(void)
{
	if (!gWifiSmTimedOut) return EHS_FALSE;
	gWifiSmTimedOut = EHS_FALSE;  /* self-clearing */
	return EHS_TRUE;
}

/* ===================================================================
 * Unified public API (Wifi_*) — preferred surface for new code.
 * Sits on top of the existing target HAL do* / isW* contract.
 * =================================================================== */

const ehs_char* Wifi_GetStateName(EhsWifiState s)
{
	switch (s) {
		case EhsWifi_Idle:          return "Idle";
		case EhsWifi_Starting:      return "Starting";
		case EhsWifi_Scanning:      return "Scanning";
		case EhsWifi_Associating:   return "Associating";
		case EhsWifi_Authenticated: return "Authenticated";
		case EhsWifi_Online:        return "Online";
		case EhsWifi_Reconnecting:  return "Reconnecting";
		case EhsWifi_Failed:        return "Failed";
		default:                    return "Unknown";
	}
}

EhsWifiState Wifi_GetState(void)
{
	/* ConnectState is now the single source of truth — event handlers and
	 * the SM thread update it at every phase transition, so this is just a
	 * direct mapping.  No reading of isScanning/isConnecting/isConnected
	 * side-flags (which were a source of race-y mis-reporting). */
	switch (getWifiStationConnectState()) {
		case WifiStationConnectState_IDLE:              return EhsWifi_Idle;
		case WifiStationConnectState_INIT:
		case WifiStationConnectState_CONFIGURE:
		case WifiStationConnectState_CONNECT:
		case WifiStationConnectState_CONNECTING:        return EhsWifi_Starting;
		case WifiStationConnectState_SCANNING:          return EhsWifi_Scanning;
		case WifiStationConnectState_ASSOCIATING:       return EhsWifi_Associating;
		case WifiStationConnectState_AUTHENTICATED:     return EhsWifi_Authenticated;
		case WifiStationConnectState_CONNECTING_GOT_IP:
		case WifiStationConnectState_CONNECTED:         return EhsWifi_Online;
		case WifiStationConnectState_RECONNECTING:      return EhsWifi_Reconnecting;
		case WifiStationConnectState_DISCONNECTING:     return EhsWifi_Idle;
		case WifiStationConnectState_FAILED:            return EhsWifi_Failed;
		default:                                        return EhsWifi_Idle;
	}
}

const ehs_char* Wifi_GetIp(void)
{
	const ehs_char* ip = WifiStationIpAddress();
	return (ip != NULL) ? ip : "";
}

ehs_sint32 Wifi_GetLastDisconnectReason(void)
{
	return EhsWifiStationReason;
}

ehs_bool Wifi_GetRssi(ehs_sint8* rssi)
{
	if (rssi == NULL) return EHS_FALSE;
	if (Wifi_GetState() != EhsWifi_Online) return EHS_FALSE;
	return doWifiStationGetRssi(rssi);
}

const ehs_char* Wifi_GetLastDisconnectReasonText(void)
{
	return WifiDisconnectReasonText(EhsWifiStationReason);
}

ehs_bool Wifi_Connect(const ehs_char* ssid, const ehs_char* psk)
{
	if (ssid == NULL || ssid[0] == '\0') return EHS_FALSE;
	if (psk == NULL) psk = "";
	/* WifiStationSetSSIDPSK saves to NVS, disconnects any current session,
	 * and starts the connect thread.  We reuse it so all existing glue
	 * (function-block callbacks, NVS persistence) keeps working. */
	WifiStationSetSSIDPSK((ehs_char*)ssid, (ehs_uint16)EhsStrlen((ehs_char*)ssid),
	                      (ehs_char*)psk,  (ehs_uint16)EhsStrlen((ehs_char*)psk));
	return EHS_TRUE;
}

ehs_bool Wifi_Disconnect(void)
{
	doWifiStationDisconnect();
	EhsWifiStationSetCBSource(eWifiStationCallbackSource_Disconnect);
	return EHS_TRUE;
}

ehs_bool Wifi_Scan(void)
{
	/* Kick a one-shot user scan.  Results land in the separate user-scan
	 * buffer and do NOT feed the connect SM (see target HAL event
	 * handler routing). */
	return (doWifiStationFullScan(EHS_TRUE) == WifiStation_Connected);
}

ehs_bool isEhsWiFiManagedByComponent()
{
	return gEhsWiFiManagedByComponent;
}

/*
 * wifi_station_thread — WiFi Station Connection State Machine
 *
 * STATES (driven by eWifiStationCallbackSource, checked each 1-second tick):
 *
 *  Connect ──[doWifiStationStart()]──► ConnectState=CONNECTING, esp_wifi_start()
 *    │  ⏰ WIFI_SM_TIMEOUT_START_MS (10 s): STA_START never fired → restart
 *    └─ WIFI_EVENT_STA_START [event → cbSource=Scan]
 *
 *  Scan ──[doWifiStationScan(ssid)]──► cbSource=Internal, gWifiStationScanning=TRUE
 *    │  ⏰ WIFI_SM_TIMEOUT_SCAN_MS (30 s): SCAN_DONE never fired → retry scan
 *    └─ WIFI_EVENT_SCAN_DONE [event → cbSource=ScanResult]
 *
 *  ScanResult ──[iterate AP list]──►
 *    ├─ SSID not found: cbSource=Scan (existing retry/reconnectPeriod timing)
 *    └─ SSID found: doWifiStationConnect(bssid,ch), cbSource=Internal
 *        ⏰ WIFI_SM_TIMEOUT_ASSOC_MS (30 s): GOT_IP never fired → cbSource=Reconnect
 *
 *  Reconnect ──[doWifiStationConnect(NULL,0)]──►
 *    │  ⏰ WIFI_SM_TIMEOUT_ASSOC_MS (30 s) per attempt
 *    ├─ within retry_num: immediate retry
 *    └─ after retry_num: wait reconnectPeriod then retry (unbounded)
 *
 *  Connected ──► cbSource=Internal (idle; isWifiStationConnected()=TRUE)
 *    └─ WIFI_EVENT_STA_DISCONNECTED [event → cbSource=Reconnect]
 *
 * KNOWN GAPS:
 *  - AUTH_FAIL disconnect reason is not distinguished from transient drop;
 *    reconnect retries immediately and keeps failing. Check reason in
 *    WIFI_EVENT_STA_DISCONNECTED handler to detect and abort.
 *  - No absolute "give up" limit on the slow reconnect cycling phase.
 *  - IDLE/INIT/CONFIGURE/CONNECTED in eWifiStationConnectState are dead code;
 *    eWifiStationConnectState is secondary to cbSource as the state driver.
 */
EHS_FB_THREAD_FUNCTION(wifi_station_thread)
{
	EHSH_LOG_INFO("wifi_station_thread started");
	/* Intentionally NOT calling Ehs_FB_ThreadStarted() / Ehs_FB_ThreadComplete():
	 * this thread is a persistent kernel-level worker, not an app-owned FB thread.
	 * It must survive KILL APP / app reload so the WiFi link stays up, and its
	 * state is managed via EhsWifiStationSetState/GetState rather than the app's
	 * function-block instance lifetime. The FB is just an RPC-style interface. */
	enum eWifiStationCallbackSource sWifiStationCallbackSource_Internal;
	inx_wifi_station_state_type* inx_wifi_station_state;
	eWifiStationStatus sWifiStationStatus = WifiStation_Disconnected;
	ehs_bool run = EHS_TRUE;
	ehs_char ip_address[20];
	ehs_char mac_address[20];
	ehs_sint8 rssi = 0; /* doWifiStationStart writes one byte through ehs_sint8* */

	ehs_uint32 retry_num = 0;
	EhsTickType current_time = 0;
	ehs_bool reconnect_in_progress = EHS_FALSE;
	ehs_bool proceed_action = EHS_FALSE;

	/* ── Timeout watchdog state ─────────────────────────────────────── */
	EhsTickType sm_drv_tick   = 0;  ehs_bool sm_drv_armed   = EHS_FALSE;
	EhsTickType sm_scan_tick  = 0;  ehs_bool sm_scan_armed  = EHS_FALSE;
	EhsTickType sm_assoc_tick = 0;  ehs_bool sm_assoc_armed = EHS_FALSE;

	/* Initialise Wi-Fi Interface */
	doWifiStationInit();

	static ehs_char canonicalAppdataPath[EHS_STRING_LENGTH_MAX];
	static ehs_char canonicalClientCertFileName[EHS_STRING_LENGTH_MAX];
	static ehs_char canonicalClientKeyFileName[EHS_STRING_LENGTH_MAX];
	static ehs_char canonicalRootCAFileName[EHS_STRING_LENGTH_MAX];
	do
	{
		inx_wifi_station_state = EhsWifiStationGetState();
		if (inx_wifi_station_state == NULL)
		{
			inx_wifi_station_state = &gDefaultWifiStationState;
		}
		pFIdata = inx_wifi_station_state->pFIdata;
		sWifiStationCallbackSource_Internal = EhsWifiStationGetCBSource();
		/* ── Timeout watchdog: fire corrective actions for stuck states ──
		 * Short-circuit when nothing is armed — skips EHS_CURRENT_TIME and
		 * three EhsTgtTimer_msToTick() computations on every idle tick.
		 * (Safety against pre-init reads is handled inside EhsTgtTimer_now;
		 * this guard is purely an optimisation.) */
		if (sm_drv_armed || sm_scan_armed || sm_assoc_armed) {
			EhsTickType _now = EHS_CURRENT_TIME;
			/* 1. Driver start: esp_wifi_start() issued but STA_START not yet fired */
			if (sm_drv_armed &&
			    (_now - sm_drv_tick) > EhsTgtTimer_msToTick(WIFI_SM_TIMEOUT_START_MS)) {
				doWifiStationDestroy();
				setWifiStationConnectState(WifiStationConnectState_CONNECT);
				EhsWifiStationSetCBSource(eWifiStationCallbackSource_Connect);
				gWifiSmTimedOut = EHS_TRUE;
				sm_drv_armed = EHS_FALSE;
				sWifiStationCallbackSource_Internal = eWifiStationCallbackSource_Connect;
			}
			/* 2. Scan: scan started but SCAN_DONE not yet received */
			if (sm_scan_armed &&
			    (_now - sm_scan_tick) > EhsTgtTimer_msToTick(WIFI_SM_TIMEOUT_SCAN_MS)) {
				doWifiStationScanStop();
				EhsWifiStationSetCBSource(eWifiStationCallbackSource_Scan);
				sm_scan_armed = EHS_FALSE;
				sWifiStationCallbackSource_Internal = eWifiStationCallbackSource_Scan;
			}
			/* 3. Association: esp_wifi_connect() issued but GOT_IP not yet received */
			if (sm_assoc_armed &&
			    (_now - sm_assoc_tick) > EhsTgtTimer_msToTick(WIFI_SM_TIMEOUT_ASSOC_MS)) {
				EhsWifiStationSetCBSource(eWifiStationCallbackSource_Reconnect);
				sm_assoc_armed = EHS_FALSE;
				sWifiStationCallbackSource_Internal = eWifiStationCallbackSource_Reconnect;
			}
		}
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
					EHSH_LOG_INFO("wifi_station_thread: Connect SSID='%s' managed=%d",
					       inx_wifi_station_state->SSID ? inx_wifi_station_state->SSID : "(null)",
					       (int)gEhsWiFiManagedByComponent);
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
								EHS_FB_OUT_S_SETN_API2(INX_wifi_station_ARG_internal_thread_ip_address, ip_address, 16);
							if (EHS_FB_OUT_CONNECTED_API2(INX_wifi_station_ARG_internal_thread_rssi))
								EHS_FB_OUT_I_API2(INX_wifi_station_ARG_internal_thread_rssi) = (ehs_sint32)rssi;
							if (EHS_FB_OUT_CONNECTED_API2(INX_wifi_station_ARG_internal_thread_ap_mac_address))
								EHS_FB_OUT_S_SETN_API2(INX_wifi_station_ARG_internal_thread_ap_mac_address, mac_address, 18);
						}
						EhsWifiStationSetCBSource(eWifiStationCallbackSource_Internal);
						if (inx_wifi_station_state != NULL && inx_wifi_station_state->pFIdata != NULL)
						{
							EHS_FB_FINISH(INX_wifi_station_ARG_internal_thread_connect_OK);
						}
					}
					else if (sWifiStationStatus == WifiStation_Connecting)
					{
						if (!sm_drv_armed) {
							sm_drv_armed = EHS_TRUE;
							sm_drv_tick  = EHS_CURRENT_TIME;
						}
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
					/* Gate on having a target SSID.  Without one, a SCAN_DONE
					 * from a user 'l' (list-scan) command would otherwise drag
					 * us into ScanResult → Scan → scan → … spin-retry against
					 * nothing.  Go idle instead and wait for the user to
					 * actually configure credentials via 'w'. */
					if (inx_wifi_station_state == NULL ||
					    inx_wifi_station_state->SSID[0] == '\0')
					{
						EhsWifiStationSetCBSource(eWifiStationCallbackSource_Internal);
						sm_drv_armed = sm_scan_armed = sm_assoc_armed = EHS_FALSE;
						break;
					}
					/* Gate scan launches on retry_num ALONE (the previous
					 * gEhsWifiStationFirstTime check was reset on every
					 * SetCBSource, which forced proceed_action=TRUE on every
					 * retry and completely bypassed the retry cap — that's
					 * why the loop could spin forever). */
					gEhsWifiStationFirstTime = EHS_FALSE;
					proceed_action = EHS_FALSE;
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
							/* User-facing: this is an actual wait the operator
							 * will notice — surface it on the console. */
							EHSH_LOG_WARNING("WiFi: %d scan attempts exhausted; backing off %d s before retrying",
							                 (int)inx_wifi_station_state->retry,
							                 (int)inx_wifi_station_state->reconnectPeriod);
							setWifiStationConnectState(WifiStationConnectState_RECONNECTING);
						}
						else if ((EHS_CURRENT_TIME - current_time) >= EhsTgtTimer_msToTick(1000 * inx_wifi_station_state->reconnectPeriod))
						{
							proceed_action = EHS_TRUE;
							reconnect_in_progress = EHS_FALSE;
							retry_num = 1;  /* start a fresh burst */
							setWifiStationConnectState(WifiStationConnectState_SCANNING);
						}
					}
					if (proceed_action)
					{
						/* Diagnostic — one line per attempt is useful during
						 * debugging but spammy in normal operation, so route
						 * through the filterable logger rather than the
						 * console. */
						EHSH_LOG_INFO("scan attempt %d/%d for '%s'",
						              (int)retry_num, (int)inx_wifi_station_state->retry,
						              inx_wifi_station_state->SSID);
						EhsWifiStationSetCBSource(eWifiStationCallbackSource_Internal);
						doWifiStationScan(inx_wifi_station_state->SSID);
						sm_drv_armed  = EHS_FALSE;  /* STA_START fired; past driver-start phase */
						sm_scan_armed = EHS_TRUE;
						sm_scan_tick  = EHS_CURRENT_TIME;
					}
					proceed_action = EHS_FALSE;
					break;
				case eWifiStationCallbackSource_ScanResult:
				{
					ehs_char ssid[EHS_STRING_LENGTH_MAX];
					ehs_char bssid[6];
					ehs_sint32 channel;
					ehs_sint32 scan_rssi;
					ehs_uint32 index = 0;
					ehs_sint32 num_ap = WifiStationScanResultCount();
					ehs_bool match_found = EHS_FALSE;

					for (index = 0; index < num_ap; index++)
					{
						if (WifiStationScanResult(index, ssid, EHS_STRING_LENGTH_MAX, bssid, 6, &channel, &scan_rssi) == EHS_TRUE)
						{
							if (inx_wifi_station_state == NULL) break;
							/* SSID is an array, so never NULL - the real guard is an
							 * unconfigured SSID, which would otherwise match a blank AP. */
							if (inx_wifi_station_state->SSID[0] == '\0') break;
							if (EhsStrcmp(inx_wifi_station_state->SSID, ssid) == 0)
							{
								/* User-facing: the "we found it, connecting
								 * now" milestone the operator is waiting for. */
								EHSH_LOG_INFO("WiFi: '%s' found on ch %d (%d dBm) - associating",
								                 ssid, (int)channel, (int)scan_rssi);
								setWifiStationConnectState(WifiStationConnectState_ASSOCIATING);
								EhsWifiStationSetCBSource(eWifiStationCallbackSource_Internal);
								/* WifiStationScanResult fills ehs_char[6]; doWifiStationConnect takes ehs_uint8* */
								doWifiStationConnect((ehs_uint8*)bssid, channel);
								sm_scan_armed  = EHS_FALSE;
								sm_assoc_armed = EHS_TRUE;
								sm_assoc_tick  = EHS_CURRENT_TIME;
								match_found = EHS_TRUE;
								break;
							}
						}
						/* FALSE return = empty-SSID / hidden-AP record; skip and continue. */
					}
					if (match_found == EHS_FALSE)
					{
						/* Diagnostic — happens every failed scan, can add up;
						 * route through the logger instead of the console. */
						EHSH_LOG_INFO("'%s' not in this scan (%d AP(s) seen); retrying",
						              (inx_wifi_station_state != NULL ? inx_wifi_station_state->SSID : ""),
						              (int)num_ap);
						EhsWifiStationSetCBSource(eWifiStationCallbackSource_Scan);
					}
					break;
				}
				case eWifiStationCallbackSource_Reconnect:
				{
					/* Report the disconnect reason exactly once per unique event.
					 * Common WPA2 codes: 2=AUTH_FAIL, 15=4WAY_HANDSHAKE_TIMEOUT,
					 * 204=HANDSHAKE_TIMEOUT — all "wrong password"; 4/5 = AP gone;
					 * 8 = ASSOC_LEAVE (clean disconnect). */
					static ehs_uint32 s_last_seen_disconn_seq = 0;
					if (gWifiDisconnectSeq != s_last_seen_disconn_seq) {
						s_last_seen_disconn_seq = gWifiDisconnectSeq;
						/* User-facing: tell the operator what happened and
						 * in plain English.  Also mirrored at WARNING level
						 * to the module log for post-mortem inspection. */
						EHSH_LOG_WARNING("WiFi: disconnected, reason=%d (%s) - retrying",
						                 (int)EhsWifiStationReason,
						                 WifiDisconnectReasonText(EhsWifiStationReason));
						EHSH_LOG_WARNING("STA_DISCONNECTED reason=%d (%s)",
						                 (int)EhsWifiStationReason,
						                 WifiDisconnectReasonText(EhsWifiStationReason));
					}
				}
					/* Gate retry launch on retry_num alone — same bug as Scan
					 * case (firstTime was resetting on every SetCBSource and
					 * bypassing the cap). */
					gEhsWifiStationFirstTime = EHS_FALSE;
					proceed_action = EHS_FALSE;
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
						setWifiStationConnectState(WifiStationConnectState_ASSOCIATING);
						EhsWifiStationSetCBSource(eWifiStationCallbackSource_Internal);
						//doWifiStationDisconnect();
						doWifiStationConnect(NULL, 0);
						sm_drv_armed   = EHS_FALSE;  /* defensive: past driver-start phase */
						sm_scan_armed  = EHS_FALSE;  /* defensive: past scan phase        */
						sm_assoc_armed = EHS_TRUE;
						sm_assoc_tick  = EHS_CURRENT_TIME;
					}
					proceed_action = EHS_FALSE;
					break;
				case eWifiStationCallbackSource_Internal:
					break;
				case eWifiStationCallbackSource_Connected:
					retry_num = 0;
					sm_drv_armed   = EHS_FALSE;
					sm_scan_armed  = EHS_FALSE;
					sm_assoc_armed = EHS_FALSE;
					gEhsStatusWifiStationDisconnected = EHS_FALSE;
					if (inx_wifi_station_state != NULL && inx_wifi_station_state->pFIdata != NULL)
					{
						EHS_FB_FINISH(INX_wifi_station_ARG_internal_thread_connect_OK);
					}
					EhsWifiStationSetCBSource(eWifiStationCallbackSource_Internal);
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
	/* No Ehs_FB_ThreadComplete() — see note at top of this function. */
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
	/* onStartup and tryReconnect are ehs_bool (1 byte) - %d would write 4 */
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"%hhu %d %hhu %d %d",&(inx_wifi_station_state->onStartup),&(inx_wifi_station_state->type),&(inx_wifi_station_state->tryReconnect),&(inx_wifi_station_state->retry),&(inx_wifi_station_state->reconnectPeriod));
		
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

	/* CRITICAL - drop our reference to this instance's data before the kernel
	 * frees it.
	 *
	 * wifi_station_internal_thread stored this instance in gpInxWifiStationState
	 * (via EhsWifiStationSetState) so the persistent wifi_station_thread can
	 * reach the FB. That instance lives in the APP_DATA pool, and KILL APP /
	 * app reload frees the whole pool at once - but the thread deliberately
	 * SURVIVES that (see the note at the top of wifi_station_thread; it keeps
	 * the link up across reloads). Left set, the very next tick does
	 *     pFIdata = inx_wifi_station_state->pFIdata;
	 * through freed pool memory and then dereferences whatever garbage came
	 * back in the EHS_FB_OUT_* / EHS_FB_FINISH macros.
	 *
	 * This is the "stale static pointer that survived a pool flush" shape
	 * described in ../EHS-kernel/docs/memory-pools.md. It presents differently
	 * per target: LoadProhibited at a small offset on ESP32, and on nRF9151 /ns
	 * a "SECURE FAULT / Attribution unit violation" at address 0x0, because a
	 * wild pointer from non-secure code that lands in secure-attributed space
	 * trips the SAU rather than faulting as a plain bad read.
	 *
	 * Clearing it makes the thread fall back to gDefaultWifiStationState on its
	 * next tick, whose pFIdata is NULL - every FB output site is already
	 * guarded on pFIdata != NULL, so the link keeps running with no FB to
	 * report to, which is exactly the intended behaviour between apps.
	 *
	 * Guarded on identity: only clear if it is OUR instance being destroyed. */
	if (gpInxWifiStationState == inx_wifi_station_state)
	{
		/* Carry the live configuration over to the fallback state first, so a
		 * reconnect after the app is gone uses the credentials the app was
		 * actually using rather than reverting to the boot defaults. */
		if (inx_wifi_station_state != NULL)
		{
			EhsStrncpy(gDefaultWifiStationState.SSID, inx_wifi_station_state->SSID,
			           EHS_STRING_LENGTH_MAX - 1);
			gDefaultWifiStationState.SSID[EHS_STRING_LENGTH_MAX - 1] = '\0';
			EhsStrncpy(gDefaultWifiStationState.PSKPass, inx_wifi_station_state->PSKPass,
			           EHS_STRING_LENGTH_MAX - 1);
			gDefaultWifiStationState.PSKPass[EHS_STRING_LENGTH_MAX - 1] = '\0';
			gDefaultWifiStationState.type            = inx_wifi_station_state->type;
			gDefaultWifiStationState.retry           = inx_wifi_station_state->retry;
			gDefaultWifiStationState.reconnectPeriod = inx_wifi_station_state->reconnectPeriod;
			gDefaultWifiStationState.tryReconnect    = inx_wifi_station_state->tryReconnect;
		}
		EhsWifiStationSetState(NULL);
	}
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
	/* No doWifiStationDisconnect() here: the async STA_DISCONNECTED event
	 * would race the Connect cbSource set below.  The SM's connect path
	 * drops any existing association itself. */
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
	gWifiDisconnectSeq++;  /* thread reads this to print the reason once per event */
}

/* Key-not-found on first boot is expected (no credentials saved yet), so
 * this helper jumps to the error label WITHOUT printing.  Genuine NVS
 * failures still surface via the open/close calls below. */
#define _EHS_NVS_SILENT_GOTO_ON_ERROR(exec, label) do { \
	ret = (exec); \
	if (ret != eEhsNvsOk) { goto label; } \
} while(0)

#define _EHS_NVS_GOTO_ON_ERROR(exec, label) do { \
	ret = (exec); \
	if (ret != eEhsNvsOk) { \
		/* EHSH_LOG_ERROR already captures __FILE__ / __LINE__ via its
		 * expansion, so the call site is identifiable without us adding
		 * __func__ / #exec source markers. */ \
		EHSH_LOG_ERROR("NVS op failed: %d", (int)ret); \
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
	ehs_uint32 retry = INX_FB_wifi_station_retry; /* NVS reads ehs_uint32; the field is ehs_sint32 */
	gDefaultWifiStationState.tryReconnect = INX_FB_wifi_station_tryReconnect;
	gDefaultWifiStationState.retry = INX_FB_wifi_station_retry;
	//gEhsWiFiManagedByComponent = EHS_TRUE;
	/* NVS namespace open is a real error if it fails */
	_EHS_NVS_GOTO_ON_ERROR(EhsNvsOpen(&obj, EHS_NVS_WIFI_NAMESPACE), error);
	/* Individual key reads are EXPECTED to fail on first boot — silent fallback to defaults */
	_EHS_NVS_SILENT_GOTO_ON_ERROR(EhsNvsGetString(&obj, EHS_NVS_WIFI_KEY_SSID, NULL, &ssid_required), error_opened);
	_EHS_NVS_SILENT_GOTO_ON_ERROR(EhsNvsGetString(&obj, EHS_NVS_WIFI_KEY_SSID, gDefaultWifiStationState.SSID, &ssid_required), error_opened);
	_EHS_NVS_SILENT_GOTO_ON_ERROR(EhsNvsGetString(&obj, EHS_NVS_WIFI_KEY_PASS, NULL, &pass_required), error_opened);
	_EHS_NVS_SILENT_GOTO_ON_ERROR(EhsNvsGetString(&obj, EHS_NVS_WIFI_KEY_PASS, gDefaultWifiStationState.PSKPass, &pass_required), error_opened);
	_EHS_NVS_SILENT_GOTO_ON_ERROR(EhsNvsGetUInt32(&obj, EHS_NVS_WIFI_KEY_RECONNECTPERIOD, &reconnectPeriod), error_opened);
	_EHS_NVS_SILENT_GOTO_ON_ERROR(EhsNvsGetBool(&obj, EHS_NVS_WIFI_KEY_TRYRECONNECT, &gDefaultWifiStationState.tryReconnect), error_opened);
	_EHS_NVS_SILENT_GOTO_ON_ERROR(EhsNvsGetUInt32(&obj, EHS_NVS_WIFI_KEY_RETRY, &retry), error_opened);
error_opened:
	_EHS_NVS_GOTO_ON_ERROR(EhsNvsClose(&obj), error);
error:
	gDefaultWifiStationState.reconnectPeriod = (ehs_sint32)reconnectPeriod;
	gDefaultWifiStationState.retry = (ehs_sint32)retry;
	/* Floor retry count — the CDF default is 1, which is too low to be
	 * useful with real-world scan misses. */
	if ((ehs_uint32)gDefaultWifiStationState.retry < WIFI_SM_MIN_RETRY_COUNT)
		gDefaultWifiStationState.retry = (ehs_sint32)WIFI_SM_MIN_RETRY_COUNT;
	/* Cap reconnect back-off — CDF default (300 s) is too long for
	 * interactive console use. */
	if ((ehs_uint32)gDefaultWifiStationState.reconnectPeriod > WIFI_SM_MAX_RECONNECT_PERIOD_S)
		gDefaultWifiStationState.reconnectPeriod = (ehs_sint32)WIFI_SM_MAX_RECONNECT_PERIOD_S;
	if (gpInxWifiStationState != NULL) {
		EhsStrcpy(gpInxWifiStationState->SSID, gDefaultWifiStationState.SSID);
		EhsStrcpy(gpInxWifiStationState->PSKPass, gDefaultWifiStationState.PSKPass);
		gpInxWifiStationState->retry = gDefaultWifiStationState.retry;
		gpInxWifiStationState->reconnectPeriod = gDefaultWifiStationState.reconnectPeriod;
	}
	setWifiStationConnectState(WifiStationConnectState_CONNECT);
	EhsWifiStationSetCBSource(eWifiStationCallbackSource_Connect);
	EHSH_LOG_INFO("EhsStartWifiStationThread: SSID='%s'", gDefaultWifiStationState.SSID);
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
		/* Persist a sensible retry count — CDF default is 1 which exhausts
		 * after a single flaky scan. */
		{
			ehs_uint32 retry_to_save = (ehs_uint32)INX_FB_wifi_station_retry;
			if (retry_to_save < WIFI_SM_MIN_RETRY_COUNT)
				retry_to_save = WIFI_SM_MIN_RETRY_COUNT;
			EhsNvsSetUInt32(&nvs, EHS_NVS_WIFI_KEY_RETRY, retry_to_save);
		}
		/* Force the NVS copy to the capped value so stale 300 s settings
		 * from older firmware get rewritten on the next 'w'. */
		if (reconnectPeriod > WIFI_SM_MAX_RECONNECT_PERIOD_S) reconnectPeriod = WIFI_SM_MAX_RECONNECT_PERIOD_S;
		EhsNvsSetUInt32(&nvs, EHS_NVS_WIFI_KEY_RECONNECTPERIOD, reconnectPeriod);
		EhsNvsCommit(&nvs);
		EhsNvsClose(&nvs);
	}
	/* Seed the in-RAM credentials directly rather than relying on
	 * EhsStartWifiStationThread reading them straight back out of NVS.
	 * Targets whose EHS_NVS_SUPPORT is 'stubbed' (Zephyr today) have an
	 * EhsNvsGetString that reports success without writing anything, so the
	 * read-back silently leaves the previous SSID in place and the SM would
	 * never see the credentials it was just handed.  The current session's
	 * credentials are authoritative regardless of whether persistence works. */
	if (ssid != NULL) {
		EhsStrncpy(gDefaultWifiStationState.SSID, ssid, EHS_STRING_LENGTH_MAX - 1);
		gDefaultWifiStationState.SSID[EHS_STRING_LENGTH_MAX - 1] = '\0';
	}
	if (psk != NULL) {
		EhsStrncpy(gDefaultWifiStationState.PSKPass, psk, EHS_STRING_LENGTH_MAX - 1);
		gDefaultWifiStationState.PSKPass[EHS_STRING_LENGTH_MAX - 1] = '\0';
	}
	/* No pre-disconnect: the SM's connect path handles dropping any current
	 * session, and the async disconnect event must not race the Connect
	 * request that EhsStartWifiStationThread queues. */
	EhsStartWifiStationThread();
	/* Declared ehs_sint32 in wifi_station.h; every path above succeeds or is a no-op.
	 * Without this the caller read whatever was left in the return register. */
	return EHS_TRUE;
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
