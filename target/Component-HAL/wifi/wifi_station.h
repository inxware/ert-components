#ifndef _COMPONENT_HAL_WIFI_STATION_H_
#define _COMPONENT_HAL_WIFI_STATION_H_

#include "globals.h"

#define COMPONENT_HAL_WIFI_STATION_MAX_RETRY	5

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

/*
 * Internally defined function. Do not re-implement this in the target code!
 * Pass the status and information when the devce as a Wi-Fi station is disconnected from the AP
 *  after created connection.
 *
 * */
void Common_WifiStation_onDisconnected(ehs_bool disconnected, ehs_uint8 reason, ehs_sint32 rssi);

eWifiStationStatus doWifiStationConnect(
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

void doWifiStationDisconnect();

void doWifiStationDestroy();

void configWifiStationSetReconnect(ehs_bool reconnect, ehs_sint32 retry);

#endif
