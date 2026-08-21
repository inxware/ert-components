#ifndef _COMPONENT_HAL_WIFI_STATION_H_
#define _COMPONENT_HAL_WIFI_STATION_H_

/* ============================================================================
 *                     WiFi Station Subsystem State Machine
 * ============================================================================
 *
 * This header is the canonical, cross-target definition of the WiFi station
 * connection state machine.  It is intentionally not target-specific — any
 * port (ESP32, ESP32-S3, future NXP/Nordic/STM32 WiFi parts, Linux wpa_sup
 * wrappers, etc.) must conform to these state names, transitions, and event
 * contract.  Targets may add their own internal flags, but must surface their
 * behaviour through the enums declared here.
 *
 * STATE VARIABLES
 * ---------------
 *   eWifiStationConnectState  (gEhsWifiStationConnectState)
 *       The authoritative state of the connection attempt; see enum below.
 *
 *   eWifiStationCallbackSource (sWifiStationCallbackSource)
 *       "What should the component thread do next."  Advanced by ISR/event
 *       handlers in the target HAL; consumed by the shared wifi_station_thread
 *       in Common/Components/networking/inx-wifi_station.c.
 *
 *   Boolean accessors (target-agnostic API):
 *       isWifiStationInitalised()   — driver loaded, radio up
 *       isWifiStationScanning()     — scan in flight
 *       isWifiStationConnecting()   — esp_wifi_connect (or equivalent) issued
 *       isWifiStationConnected()    — truly online (has IP via DHCP/static)
 *
 * STATES (eWifiStationConnectState) and the natural transitions
 * -------------------------------------------------------------
 *
 *   IDLE ──[EhsStartWifiStationThread / wifi_station_do_connect]──► CONNECT
 *
 *   CONNECT ──[doWifiStationStart()]──► CONNECTING
 *      driver brought up; esp_wifi_start() issued
 *      ⏰ WIFI_SM_TIMEOUT_START_MS   (10 s): no STA_START → restart
 *
 *   CONNECTING (scan-for-target phase) ──[STA_START event]──► SCANNING
 *      targeted scan for configured SSID
 *      ⏰ WIFI_SM_TIMEOUT_SCAN_MS    (30 s): no SCAN_DONE → retry scan
 *
 *   SCANNING ──[SCAN_DONE event, SSID match]──► ASSOCIATING
 *             ──[SCAN_DONE event, no match]──► back to SCANNING (retry_num++)
 *
 *   ASSOCIATING ──[STA_CONNECTED event]──► AUTHENTICATED
 *      esp_wifi_connect() issued; waiting for 802.11 association/auth
 *      ⏰ WIFI_SM_TIMEOUT_ASSOC_MS   (30 s): no STA_CONNECTED → Reconnect
 *
 *   AUTHENTICATED (associated but NOT yet online) ──[GOT_IP event]──►
 *                                                      CONNECTING_GOT_IP
 *      802.11 layer up; DHCP in flight (or static IP apply)
 *      ⏰ WIFI_SM_TIMEOUT_ASSOC_MS extends through this phase
 *
 *   CONNECTING_GOT_IP ──[thread picks up GOT_IP]──► CONNECTED
 *      IP obtained; transitional state that the thread converts to CONNECTED
 *      after notifying the FB / console
 *
 *   CONNECTED ──[STA_DISCONNECTED event]──► RECONNECTING
 *      fully online until the link drops
 *
 *   RECONNECTING ──[doWifiStationConnect / events]──► ASSOCIATING / FAILED
 *      retry_num fast retries, then reconnectPeriod slow retries
 *
 *   FAILED — terminal unless user re-issues a connect command
 *
 * KEY RULE: every transitory state must have exactly one active timer.  The
 * watchdog in wifi_station_thread arms/disarms sm_drv_armed / sm_scan_armed /
 * sm_assoc_armed as states are entered/left.  States that persist indefinitely
 * (IDLE, CONNECTED, FAILED) arm no timer.
 *
 * KNOWN GAPS (unhandled without extending the event contract)
 *   - AUTH_FAIL disconnect reason is not distinguished from transient drop;
 *     reconnect retries immediately and will loop against a bad password.
 *     Fix would need the target HAL to surface the reason code and the SM
 *     to route AUTH_FAIL to FAILED.
 *   - No absolute "give up after N total minutes" cap on the slow reconnect.
 *
 * PORTING CONTRACT
 *   - Each target's HAL implements the do* functions and fires cbSource
 *     transitions from its native event source.  The shared component thread
 *     (inx-wifi_station.c) drives the logic without any target-specific code.
 *   - See docs/ert-porting-guide.md § "WiFi Subsystem" for the full contract
 *     a new port must satisfy.
 * ============================================================================
 */

#include "globals.h"


#ifdef EHS_NETWORK_WIFI_SUPPORT
    #define EHS_WIFI_SSID_BUFF_MAX 64
#endif //#ifdef EHS_NETWORK_WIFI_SUPPORT

/* ── Connection state-machine timeout constants (milliseconds) ────────────
 * These are the watchdog deadlines enforced by wifi_station_thread and
 * referenced by the transition diagram at the top of this file.  They live
 * here, not in inx-wifi_station.c, because they are part of the porting
 * contract: a target HAL that needs its own driver-level deadline (a
 * supplicant connect timeout, say) must DERIVE it from these so the two
 * cannot silently drift apart. */
#define WIFI_SM_TIMEOUT_START_MS   10000U  /* driver start → radio-up event   */
#define WIFI_SM_TIMEOUT_SCAN_MS    30000U  /* scan start   → SCAN_DONE event  */
#define WIFI_SM_TIMEOUT_ASSOC_MS   30000U  /* connect      → GOT_IP event     */

/* ============================================================================
 *            UNIFIED WiFi STATION API (v2) — preferred surface
 * ============================================================================
 *
 * Single state enum replacing the eWifiStationConnectState / ...CallbackSource /
 * gWifiStationScanning / gEhsWifiStationConnecting / gTargetWifiStationConnected
 * tangle.  Callers (console, function blocks, applications) should use these
 * entry points; the older do* / isW* functions below are kept as the target-HAL
 * contract and legacy adapters. */

typedef enum {
	EhsWifi_Idle = 0,        /* stopped / not configured                         */
	EhsWifi_Starting,        /* driver bringup in progress                       */
	EhsWifi_Scanning,        /* connect-SM scanning for configured SSID          */
	EhsWifi_Associating,     /* esp_wifi_connect issued; awaiting STA_CONNECTED  */
	EhsWifi_Authenticated,   /* STA_CONNECTED received; awaiting DHCP / GOT_IP   */
	EhsWifi_Online,          /* fully connected (has IP)                         */
	EhsWifi_Reconnecting,    /* auto-reconnect after link loss                   */
	EhsWifi_Failed,          /* terminal — manual recovery required              */
	EhsWifi_MAX
} EhsWifiState;

/* Request connect using these credentials; returns immediately.  State
 * machine runs asynchronously — poll Wifi_GetState() for progress. */
ehs_bool         Wifi_Connect(const ehs_char* ssid, const ehs_char* psk);

/* Tear down any active connection / ongoing attempt. */
ehs_bool         Wifi_Disconnect(void);

/* Kick off a one-shot user scan (full, unfiltered, show hidden).  Results
 * go to a SEPARATE buffer from the connect-SM's filtered scans and can
 * be retrieved via Wifi_UserScanResult* without interference. */
ehs_bool         Wifi_Scan(void);

/* Current SM state and a human-readable name. */
EhsWifiState     Wifi_GetState(void);
const ehs_char*  Wifi_GetStateName(EhsWifiState s);

/* Current IP address string, or "" if not online. */
const ehs_char*  Wifi_GetIp(void);

/* Last ESP-IDF disconnect reason code + short human text. */
ehs_sint32       Wifi_GetLastDisconnectReason(void);
const ehs_char*  Wifi_GetLastDisconnectReasonText(void);

/* RSSI of the currently associated AP, in dBm. Returns EHS_TRUE if the
 * value was filled in; EHS_FALSE if not connected, or the target's
 * driver couldn't supply a reading. */
ehs_bool         Wifi_GetRssi(ehs_sint8* rssi);

/* User-scan result accessors (separate from SM scan). */
ehs_uint16       Wifi_UserScanResultCount(void);
ehs_bool         Wifi_UserScanResultGet(ehs_uint16 index,
                                         ehs_char* ssid, ehs_uint16 ssid_size,
                                         ehs_uint8* bssid, ehs_uint16 bssid_size,
                                         ehs_sint32* channel, ehs_sint32* rssi);

#define EHS_NVS_WIFI_NAMESPACE "wifi_station"
#define EHS_NVS_WIFI_KEY_SSID "ssid"
#define EHS_NVS_WIFI_KEY_PASS "pass"
#define EHS_NVS_WIFI_KEY_ONSTARTUP "startup"
#define EHS_NVS_WIFI_KEY_RECONNECTPERIOD "p_reconn"
#define EHS_NVS_WIFI_KEY_TRYRECONNECT "b_reconn_try"
#define EHS_NVS_WIFI_KEY_RETRY "retry"

typedef enum {
	WifiStation_Connected = 0,
	WifiStation_Disconnected,
	WifiStation_NotStopped,
	WifiStation_NotFound,
	WifiStation_InitFailed,
	WifiStation_StartFailed,
	WifiStation_Timeout,
	WifiStation_NotStarted,
	WifiStation_StateError,
	WifiStation_InvalidArg,
	WifiStation_FileNotFound,
	WifiStation_AuthFailed,
	WifiStation_InternalError,
	WifiStation_NotImplemented,
	WifiStation_Connecting,
	WifiStation_NotConfigured,
	WifiStation_MAX
} eWifiStationStatus;

enum {
	Type_WifiStation_PSK = 0,
	Type_WifiStation_Open,
	Type_WifiStation_Enterprise
};

enum {
	Enterprise_WifiStation_WPA2 = 0,
	Enterprise_WifiStation_WPA3,
	Enterprise_WifiStation_WPA3_192Bit
};

enum {
	EAP_WifiStation_TLS = 0,
	EAP_WifiStation_PEAP,
	EAP_WifiStation_TTLS
};

enum {
	TTLS2_WifiStation_MIN = 0,
	TTLS2_WifiStation_MSCHAPV2 = 1,
	TTLS2_WifiStation_SCHAP,
	TTLS2_WifiStation_PAP,
	TTLS2_WifiStation_CHAP,
	TTLS2_WifiStation_MAX
};

/* Canonical state machine — see the header-top block comment for the
 * transition diagram and timeout contract.  Every port must honour these
 * state names; targets may add internal flags but must not invent new
 * public states without extending this enum. */
typedef enum {
	WifiStationConnectState_IDLE = 0,            /* subsystem stopped                       */
	WifiStationConnectState_INIT,                /* HAL init in progress                    */
	WifiStationConnectState_CONFIGURE,           /* applying credentials                    */
	WifiStationConnectState_CONNECT,             /* connect requested, about to start       */
	WifiStationConnectState_CONNECTING,          /* radio starting, awaiting STA_START      */
	WifiStationConnectState_SCANNING,            /* targeted scan for configured SSID       */
	WifiStationConnectState_ASSOCIATING,         /* esp_wifi_connect issued                 */
	WifiStationConnectState_AUTHENTICATED,       /* associated, waiting for DHCP/GOT_IP     */
	WifiStationConnectState_CONNECTING_GOT_IP,   /* GOT_IP received, pending thread pickup  */
	WifiStationConnectState_CONNECTED,           /* fully online                            */
	WifiStationConnectState_RECONNECTING,        /* retry after link loss                   */
	WifiStationConnectState_DISCONNECTING,       /* user-requested disconnect in progress   */
	WifiStationConnectState_FAILED,              /* terminal — manual recovery              */
	WifiStationConnectState_MAX
} eWifiStationConnectState;

enum eWifiStationCallbackSource {
	eWifiStationCallbackSource_Connect = 0,
	eWifiStationCallbackSource_Scan,
	eWifiStationCallbackSource_ScanResult,
	eWifiStationCallbackSource_Reconnect,
	eWifiStationCallbackSource_Internal,
	eWifiStationCallbackSource_Connected,
	eWifiStationCallbackSource_Disconnect,
	eWifiStationCallbackSource_MAX
};

/*
 * Internally defined function. Do not re-implement this in the target code!
 * Pass the status and information when the devce as a Wi-Fi station is disconnected from the AP
 *  after created connection.
 *
 * */
void Common_WifiStation_onDisconnected(ehs_bool disconnected, ehs_uint8 reason, ehs_sint32 rssi);

ehs_bool doWifiStationNetifInit(const ehs_char* host_name);
void doWifiStationNetifDestroy();

void doWifiStationScan(ehs_char *ssid);

eWifiStationStatus doWifiStationInit();

eWifiStationStatus doWifiStationStart(
	ehs_char*	ssid,
	ehs_uint8	type,
	ehs_char*	PSKPass,
	ehs_uint8	EnterpriseType,
	ehs_uint8	EAP,
	ehs_uint8	TTLS2,
	ehs_bool	needServerCert,
	ehs_char*	serverCert,
	ehs_char*	tlsCert,
	ehs_char*	tlsKey,
	ehs_char*	eapID,
	ehs_char*	eapUser,
	ehs_char*	eapPass,
	ehs_char*	ip_address,
	ehs_char*	mac_address,
	ehs_sint8*	rssi
);

/**
 * @brief Connect to an existing Wi-Fi network using previously saved settings
 *
 * @param bssid Optional BSSID to connect to a specific access point. NULL to ignore.
 * @param channel Optional channel to connect on. 0 to ignore.
 * 
 * @return eWifiStationStatus 
 *			WifiStation_Connecting - Connection in progress
 *			WifiStation_NotConfigured - No saved settings found
 *			Other error codes as defined in eWifiStationStatus
 */
eWifiStationStatus doWifiStationConnect(ehs_uint8 *bssid, ehs_uint8 channel);

void doWifiStationDisconnect();

void doWifiStationDestroy();

eWifiStationConnectState getWifiStationConnectState();

void setWifiStationConnectState(eWifiStationConnectState state);

/* Extract all relevant wifi scan results
ehs_uint32 index - 0 to N
ehs_char* ssid - outputs SSID for the index
ehs_uint16 ssid_size - size of ssid array (min=33)
ehs_char* bssid - outputs BSSID (mac) for the index
ehs_uint16 bssid_size - size of bssid array (min=6)
ehs_sint32* channel - primary channel for the index
ehs_sint32* rssi - rssi for the index
return - function returns EHS_FALSE once nothing availble for the index
*/
ehs_bool WifiStationScanResult(ehs_uint32 index, ehs_char* ssid, ehs_uint16 ssid_size, ehs_char* bssid, ehs_uint16 bssid_size, 
                               ehs_sint32* channel, ehs_sint32* rssi);

/**
 * @brief Return the number of Wi-Fi access points found in the last scan
 * 
 * @return ehs_sint32 Number of access points found
 */
ehs_sint32 WifiStationScanResultCount();

ehs_sint32 WifiStationSetSSIDPSK(ehs_char* ssid, ehs_uint16 ssid_size, ehs_char* psk, ehs_uint16 psk_size);

const ehs_char* WifiStationIpAddress();

ehs_bool isWifiStationInitalised();

ehs_bool isWifiStationConnected();

ehs_bool isEhsWiFiManagedByComponent();

void EhsWifiStationSetCBSource(enum eWifiStationCallbackSource source);

/* Current pending callback source.  Target HAL event handlers use this to
 * avoid clobbering a queued request (e.g. a Connect) with a lower-priority
 * transition. */
enum eWifiStationCallbackSource EhsWifiStationGetCBSource(void);

void EhsStartWifiStationThread();

void WifiStationGetCurrentSsid(ehs_char *ssid);

ehs_bool isWifiStationScanning();

/**
 * @brief Stop any ongoing WiFi scan.
 *
 * Handles the race where the driver's SCAN_DONE event fires just before
 * the stop request arrives: a "no scan was running" indication from the
 * underlying driver is treated as a successful stop, on the basis that
 * the scan has already finished naturally.
 *
 * @return EHS_TRUE   scan stopped (or had already finished naturally)
 * @return EHS_FALSE  the driver reported an unexpected error
 */
ehs_bool doWifiStationScanStop(void);

/**
 * @brief Get the RSSI of the currently associated AP, in dBm.
 *
 * Target-HAL primitive backing the unified Wifi_GetRssi wrapper. Targets
 * without a native getter (or with no association) must return EHS_FALSE
 * and leave *rssi unmodified.
 *
 * @return EHS_TRUE  *rssi populated
 * @return EHS_FALSE not connected, or driver could not supply a value
 */
ehs_bool doWifiStationGetRssi(ehs_sint8* rssi);

/**
 * @brief Start a full (all-channel) WiFi scan.
 *
 * Results go to the user-scan buffer (Wifi_UserScanResult*), never the
 * connect-SM's buffer.
 *
 * @param print  EHS_TRUE to have the target print the results to its own
 *               stdio when the scan completes.
 *
 * @return WifiStation_Connected      scan started successfully (this enum's
 *                                    "no error" value, as used by
 *                                    doWifiStationInit)
 * @return WifiStation_StateError     a scan is already in progress
 * @return WifiStation_NotStarted     driver not started (call doWifiStationStart first)
 * @return WifiStation_InitFailed     driver not initialised
 * @return WifiStation_InternalError  driver rejected the request
 */
eWifiStationStatus doWifiStationFullScan(ehs_bool print);

ehs_bool isWifiStationConnecting();

/**
 * @brief Request that scan results be printed to the serial console when the next WIFI_EVENT_SCAN_DONE fires.
 *        Use this when a scan is already in progress and the caller still wants console output.
 */
void EhsWifiStationSetPrintOnScanDone(ehs_bool enable);

/** @brief Returns EHS_TRUE if a scan has completed and results are waiting to be consumed. */
ehs_bool WifiStationIsScanResultReady();

/**
 * @brief Returns EHS_TRUE (once, self-clearing) if the connection state machine
 *        fired a timeout on a transitory state (driver start, scan, or association).
 *        Poll from command_prompt_task to print a user-facing notice.
 */
ehs_bool WifiStationIsConnectTimedOut(void);

/**
 * @brief Print all scan results to stdout and clear the ready/print flags.
 *        Call only from a task with sufficient stack (not from an event handler).
 */
void WifiStationPrintAndClearScanResults();

#endif
