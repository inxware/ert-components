#ifndef _COMPONENT_HAL_WIFI_STATION_H_
#define _COMPONENT_HAL_WIFI_STATION_H_

#include "globals.h"

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

typedef enum {
	WifiStationConnectState_IDLE = 0,
	WifiStationConnectState_INIT,
	WifiStationConnectState_CONFIGURE,
	WifiStationConnectState_CONNECT,
	WifiStationConnectState_CONNECTING,
	WifiStationConnectState_CONNECTING_GOT_IP,
	WifiStationConnectState_CONNECTED,
	WifiStationConnectState_FAILED,
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

const ehs_char* WifiStationIpAddress();

ehs_bool isWifiStationInitalised();

ehs_bool isWifiStationConnected();

ehs_bool isEhsWiFiManagedByComponent();

void EhsWifiStationSetCBSource(enum eWifiStationCallbackSource source);

void EhsStartWifiStationThread();

#endif
