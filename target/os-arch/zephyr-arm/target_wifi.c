/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_wifi.c
 * Zephyr Wi-Fi station HAL — the target-side half of the cross-target Wi-Fi
 * station contract declared in target/Component-HAL/wifi/wifi_station.h.
 *
 * The shared connect state machine lives in
 * Common/Components/networking/inx-wifi_station.c and is identical on every
 * port; this file only has to (a) implement the do* primitives and (b) drive
 * the SM's state / cbSource variables from the platform's native event source.
 * The ESP32-S3 port (target/os-arch/esp32s3_freertos-xtensa/target_wifi.c) is
 * the reference implementation — the mapping used here is:
 *
 *   ESP-IDF                                Zephyr / nRF70
 *   ─────────────────────────────────────  ────────────────────────────────────
 *   esp_wifi_init + esp_wifi_start         net_if_up() + wifi_ready callback
 *   WIFI_EVENT_STA_START                   wifi_ready fired (RPU + supplicant up)
 *   esp_wifi_scan_start                    NET_REQUEST_WIFI_SCAN
 *   WIFI_EVENT_SCAN_DONE + get_ap_records  NET_EVENT_WIFI_SCAN_RESULT (per AP)
 *                                            then NET_EVENT_WIFI_SCAN_DONE
 *   esp_wifi_connect                       NET_REQUEST_WIFI_CONNECT
 *   WIFI_EVENT_STA_CONNECTED               NET_EVENT_WIFI_CONNECT_RESULT (ok)
 *   WIFI_EVENT_STA_DISCONNECTED            NET_EVENT_WIFI_DISCONNECT_RESULT
 *   IP_EVENT_STA_GOT_IP                    NET_EVENT_IPV4_DHCP_BOUND
 *   esp_wifi_sta_get_rssi                  NET_REQUEST_WIFI_IFACE_STATUS
 *
 * WHY A DEDICATED WORKER THREAD
 * -----------------------------
 * Every net_mgmt(NET_REQUEST_WIFI_*) call runs the whole wifi_mgmt →
 * wifi_mgmt_ext → wpa_supplicant CLI chain synchronously on the CALLING
 * thread's stack. Nordic's own station sample sizes that thread at 5200 bytes
 * (CONFIG_STA_SAMPLE_START_WIFI_THREAD_STACK_SIZE); calling it from the system
 * workqueue overflowed the stack outright during bring-up (see the file header
 * of wifi_connect_test.c). The shared SM runs on an EHS dynamic thread, which
 * on this port gets EHS_ZEPHYR_DEFAULT_STACK_SIZE (4096) from target_process.c
 * — not enough, and not something the Wi-Fi HAL is entitled to change for
 * every other FB thread on the system. So the do* entry points below only
 * enqueue a command and return immediately; a worker thread owned by this file
 * (with a stack sized for the supplicant call chain) performs the actual
 * net_mgmt calls. That also matches the contract the SM already expects from
 * ESP-IDF, where every do* is asynchronous and progress arrives via events.
 *
 * The net_mgmt event callbacks run on Zephyr's net_mgmt event thread. Like the
 * ESP-IDF event handlers they only touch flags, the scan buffers and the SM
 * state variables — no blocking work, no printf.
 *
 * @author: inx limited
 */

/* Module-scoped logger ID. Must precede every include: several headers pull in
 * hal_logger.h, which latches EHSL_MODULE_ID to UNDEFINED if it is not set yet. */
#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

#include <zephyr/kernel.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_event.h>
#include <zephyr/net/hostname.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/wifi_mgmt.h>
#if defined(CONFIG_NET_DHCPV4)
#include <zephyr/net/dhcpv4.h>
#endif
#ifdef CONFIG_WIFI_READY_LIB
#include <net/wifi_ready.h>
#endif

#include <stdio.h>
#include <string.h>

#include "globals.h"
#include "hal.h"
#include "hal_logger.h"
#include "hal_string.h"

#include "wifi_station.h"

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/* Worker stack. 5200 is the value Nordic's own nrf/samples/wifi/sta uses for
 * the thread that issues NET_REQUEST_WIFI_CONNECT; the supplicant CLI chain
 * below it is the deepest consumer. Do not trim without re-measuring with
 * CONFIG_INIT_STACKS + k_thread_stack_space_get(). */
#ifndef EHS_ZEPHYR_WIFI_THREAD_STACK_SIZE
#define EHS_ZEPHYR_WIFI_THREAD_STACK_SIZE 5200
#endif

/* Cooperative-range priority is wrong here (the worker blocks on net_mgmt for
 * seconds at a time); use a low-ish preemptible priority like the sample. */
#ifndef EHS_ZEPHYR_WIFI_THREAD_PRIORITY
#define EHS_ZEPHYR_WIFI_THREAD_PRIORITY K_PRIO_PREEMPT(7)
#endif

/* How long to wait for the RPU firmware load + wpa_supplicant interface
 * registration to complete after net_if_up(). Only used when
 * CONFIG_WIFI_READY_LIB is enabled (it is, via wifi_nrf70.mk). */
#ifndef EHS_ZEPHYR_WIFI_READY_TIMEOUT_MS
#define EHS_ZEPHYR_WIFI_READY_TIMEOUT_MS 30000
#endif

/* Deadline handed to the supplicant for one association attempt, in seconds.
 * Derived from the SM's association watchdog (wifi_station.h) rather than
 * hand-picked: it has to be comfortably SHORTER, so a failed association
 * surfaces as NET_EVENT_WIFI_CONNECT_RESULT(fail) and the SM retries through
 * its normal Reconnect path, instead of the watchdog firing first and queuing
 * a second connect on top of one that is still in flight. Two thirds leaves
 * room for the event to propagate through the net_mgmt queue. */
#define EHS_ZEPHYR_WIFI_CONNECT_TIMEOUT_S ((WIFI_SM_TIMEOUT_ASSOC_MS / 1000U) * 2U / 3U)
#if EHS_ZEPHYR_WIFI_CONNECT_TIMEOUT_S < 5
#error "WIFI_SM_TIMEOUT_ASSOC_MS is too small to derive a usable supplicant connect timeout"
#endif

/* Per-scan AP record capacity. Three static tables are sized from this: the SM
 * scan buffer, the user scan buffer (41 bytes/record each) and the dedup table
 * in WifiStationPrintAndClearScanResults (33 bytes/record), so the total cost
 * is ~115 bytes per record. 12 is a deliberate trim from the ESP port's 20 -
 * this part is RAM-critical (see the budget note in wifi_nrf70.mk) and the
 * buffer keeps the STRONGEST results rather than the first ones (see
 * scan_record_add), so a smaller table costs visibility of weak/distant APs
 * rather than risking the one we are trying to associate with. */
#ifndef EHS_ZEPHYR_MAX_AP_RECORDS
#define EHS_ZEPHYR_MAX_AP_RECORDS 12
#endif

/* Enough for "255.255.255.255" and "xx:xx:xx:xx:xx:xx" plus the null. */
#define MACIP_LENGTH 20

/* Depth of the worker command queue. Connect/scan/disconnect requests arrive
 * at most one per SM tick (1 s), so this only has to absorb a short burst. */
#define EHS_ZEPHYR_WIFI_CMDQ_DEPTH 8

typedef enum {
	Wifi_Cmd_IfaceUp = 0,   /* net_if_up + wait for wifi_ready               */
	Wifi_Cmd_ScanSm,        /* connect-SM scan   → s_ap_records              */
	Wifi_Cmd_ScanUser,      /* user 'list' scan  → s_user_ap_records         */
	Wifi_Cmd_Connect,       /* NET_REQUEST_WIFI_CONNECT with stashed creds   */
	Wifi_Cmd_Disconnect,    /* NET_REQUEST_WIFI_DISCONNECT                   */
	Wifi_Cmd_IfaceDown      /* net_if_down (full teardown)                   */
} eWifiCmd;

typedef struct {
	ehs_uint8 id;                            /* eWifiCmd                     */
	ehs_uint8 channel;                       /* Connect only; 0 = any        */
	ehs_uint8 bssid[WIFI_MAC_ADDR_LEN];      /* Connect only                 */
	ehs_uint8 bssid_set;                     /* Connect only                 */
} sWifiCmd;

/* Compact scan record. The Zephyr driver hands us a struct wifi_scan_result
 * per AP (≈56 bytes with padding); we only need four fields, and holding two
 * buffers of the full struct is a meaningful chunk of RAM on a 256 KB part. */
typedef struct {
	ehs_char  ssid[WIFI_SSID_MAX_LEN + 1];
	ehs_uint8 bssid[WIFI_MAC_ADDR_LEN];
	ehs_uint8 channel;
	ehs_sint8 rssi;
} sWifiApRecord;

/* Which buffer NET_EVENT_WIFI_SCAN_RESULT should fill, and who consumes
 * NET_EVENT_WIFI_SCAN_DONE. Mirrors ScanPurpose_t in the ESP-IDF port. */
typedef enum { Scan_None = 0, Scan_Sm, Scan_User } eScanPurpose;

/*****************************************************************************/
/* Declare file scope (static) variables */

static struct net_if *s_iface = NULL;

static struct net_mgmt_event_callback s_wifi_cb;
static struct net_mgmt_event_callback s_ipv4_cb;

static sWifiApRecord s_ap_records[EHS_ZEPHYR_MAX_AP_RECORDS];
static ehs_uint16    s_ap_count = 0;
static sWifiApRecord s_user_ap_records[EHS_ZEPHYR_MAX_AP_RECORDS];
static ehs_uint16    s_user_ap_count = 0;
static volatile eScanPurpose s_scan_purpose = Scan_None;

static ehs_char IP_Address[MACIP_LENGTH] = {0};
static ehs_char mac_output[MACIP_LENGTH] = {0};

static volatile ehs_bool gTargetWifiStationConnected = EHS_FALSE;
static volatile ehs_bool gWifiStationInitalised      = EHS_FALSE;  /* radio + supplicant up */
static volatile ehs_bool gWifiStationConfigured      = EHS_FALSE;  /* credentials stashed   */
static volatile ehs_bool gWifiStationBringupPosted   = EHS_FALSE;  /* IfaceUp enqueued      */
static volatile ehs_bool gWifiStationScanning        = EHS_FALSE;
static volatile ehs_bool gWifiStationScanResultPrint = EHS_FALSE;
static volatile ehs_bool gWifiStationScanResultReady = EHS_FALSE;
static volatile ehs_bool gEhsWifiStationConnecting   = EHS_FALSE;

/* Credentials stashed by doWifiStationStart for the worker to hand to
 * NET_REQUEST_WIFI_CONNECT. Unlike ESP-IDF there is no driver-side config to
 * write, so this is simply where the SM's credentials live between the
 * doWifiStationStart call and the doWifiStationConnect that follows the scan.
 * WIFI_PSK_MAX_LEN is 64 (the hex-PSK form); +1 for our null terminator. */
static ehs_char  s_pending_ssid[WIFI_SSID_MAX_LEN + 1] = {0};
static ehs_char  s_pending_psk[WIFI_PSK_MAX_LEN + 1]   = {0};
static ehs_uint8 s_pending_type  = Type_WifiStation_PSK;
static ehs_bool  s_pending_valid = EHS_FALSE;

K_THREAD_STACK_DEFINE(s_wifi_worker_stack, EHS_ZEPHYR_WIFI_THREAD_STACK_SIZE);
static struct k_thread s_wifi_worker_thread;
static ehs_bool        s_wifi_worker_started = EHS_FALSE;

K_MSGQ_DEFINE(s_wifi_cmdq, sizeof(sWifiCmd), EHS_ZEPHYR_WIFI_CMDQ_DEPTH, 4);

#ifdef CONFIG_WIFI_READY_LIB
static K_SEM_DEFINE(s_wifi_ready_sem, 0, 1);
#endif

/*****************************************************************************/
/* Forward declarations */

static void wifi_worker_entry(void *p1, void *p2, void *p3);
static ehs_bool wifi_post(eWifiCmd id, const ehs_uint8 *bssid, ehs_uint8 channel);

/*****************************************************************************/
/* Scan result buffer handling (net_mgmt event thread context) */

/* Insert one AP into the given buffer. Deduplicates on BSSID (the driver can
 * report the same beacon more than once across a multi-band scan), and once
 * the buffer is full keeps the strongest results rather than the first ones —
 * Zephyr, unlike ESP-IDF, does not sort scan results by RSSI, so plain
 * truncation would drop the target AP for no better reason than scan order. */
static void scan_record_add(sWifiApRecord *buf, ehs_uint16 *count,
                            const struct wifi_scan_result *entry)
{
	ehs_uint16 i;
	ehs_uint16 weakest = 0;
	ehs_uint8  ssid_len = entry->ssid_length;

	if (ssid_len == 0 || ssid_len > WIFI_SSID_MAX_LEN) {
		/* Hidden AP (zero-length SSID) or a malformed record — the SM
		 * matches on SSID, so neither is usable. */
		return;
	}

	for (i = 0; i < *count; i++) {
		if (memcmp(buf[i].bssid, entry->mac, WIFI_MAC_ADDR_LEN) == 0) {
			buf[i].rssi    = entry->rssi;
			buf[i].channel = entry->channel;
			return;
		}
		if (buf[i].rssi < buf[weakest].rssi) {
			weakest = i;
		}
	}

	if (*count < EHS_ZEPHYR_MAX_AP_RECORDS) {
		i = (*count)++;
	} else if (entry->rssi > buf[weakest].rssi) {
		i = weakest;
	} else {
		return;
	}

	memcpy(buf[i].ssid, entry->ssid, ssid_len);
	buf[i].ssid[ssid_len] = '\0';
	memcpy(buf[i].bssid, entry->mac, WIFI_MAC_ADDR_LEN);
	buf[i].channel = entry->channel;
	buf[i].rssi    = entry->rssi;
}

/*****************************************************************************/
/* net_mgmt event handlers — run on the net_mgmt event thread */

static void wifi_mgmt_event_handler(struct net_mgmt_event_callback *cb,
                                    uint32_t mgmt_event, struct net_if *iface)
{
	ARG_UNUSED(iface);

	switch (mgmt_event) {
	case NET_EVENT_WIFI_SCAN_RESULT: {
		const struct wifi_scan_result *entry =
			(const struct wifi_scan_result *)cb->info;

		if (entry == NULL) {
			break;
		}
		if (s_scan_purpose == Scan_User) {
			scan_record_add(s_user_ap_records, &s_user_ap_count, entry);
		} else {
			scan_record_add(s_ap_records, &s_ap_count, entry);
		}
		break;
	}

	case NET_EVENT_WIFI_SCAN_DONE: {
		eScanPurpose purpose = s_scan_purpose;

		gWifiStationScanning = EHS_FALSE;
		s_scan_purpose = Scan_None;

		if (purpose == Scan_User) {
			/* Signals the print path only; must NOT advance the SM. */
			gWifiStationScanResultReady = EHS_TRUE;
		} else {
			EhsWifiStationSetCBSource(eWifiStationCallbackSource_ScanResult);
		}
		break;
	}

	case NET_EVENT_WIFI_CONNECT_RESULT: {
		const struct wifi_status *status = (const struct wifi_status *)cb->info;
		int st = (status != NULL) ? status->status : -1;

		gEhsWifiStationConnecting = EHS_FALSE;

		if (st == 0) {
			/* Associated + authenticated, but NOT yet online: DHCP is
			 * still to run. Same split as the ESP port's
			 * STA_CONNECTED → AUTHENTICATED → GOT_IP sequence. */
			setWifiStationConnectState(WifiStationConnectState_AUTHENTICATED);
			EHSH_LOG_INFO("Associated - waiting for IP (DHCP)");
#if defined(CONFIG_NET_DHCPV4)
			/* CONFIG_NRF_WIFI_IF_AUTO_START=n sets NET_IF_NO_AUTO_START on
			 * the nRF70 interface, and net_config_init_by_iface() returns
			 * -ENETDOWN on that flag BEFORE it reaches setup_dhcpv4() — so
			 * CONFIG_NET_CONFIG_SETTINGS never starts the DHCP client for
			 * us. Start it here instead. net_dhcpv4_start() is a no-op if
			 * the client is already past DISABLED, so the reconnect path
			 * costs nothing. */
			if (s_iface != NULL) {
				net_dhcpv4_start(s_iface);
			}
#endif
		} else {
			/* A failed association is reported through the same channel as
			 * a dropped link so the SM's retry/back-off logic applies. */
			enum eWifiStationCallbackSource pending = EhsWifiStationGetCBSource();

			gTargetWifiStationConnected = EHS_FALSE;
			Common_WifiStation_onDisconnected(EHS_TRUE, (ehs_uint8)st, 0);
			EHSH_LOG_WARNING("Connect failed, status=%d", st);
			if (pending == eWifiStationCallbackSource_Internal ||
			    pending == eWifiStationCallbackSource_Connected ||
			    pending == eWifiStationCallbackSource_Reconnect) {
				setWifiStationConnectState(WifiStationConnectState_RECONNECTING);
				EhsWifiStationSetCBSource(eWifiStationCallbackSource_Reconnect);
			}
		}
		break;
	}

	case NET_EVENT_WIFI_DISCONNECT_RESULT: {
		const struct wifi_status *status = (const struct wifi_status *)cb->info;
		int reason = (status != NULL) ? status->status : 0;
		enum eWifiStationCallbackSource pending = EhsWifiStationGetCBSource();

		gTargetWifiStationConnected = EHS_FALSE;
		gEhsWifiStationConnecting   = EHS_FALSE;
		IP_Address[0] = '\0';
		Common_WifiStation_onDisconnected(EHS_TRUE, (ehs_uint8)reason, 0);
		EHSH_LOG_WARNING("Disconnected, reason=%d", reason);

		/* Only steer into a reconnect for a dropped session. A queued
		 * Connect, an in-flight scan, or an explicit Disconnect must not be
		 * hijacked — same guard as the ESP-IDF port. */
		if (pending == eWifiStationCallbackSource_Internal ||
		    pending == eWifiStationCallbackSource_Connected ||
		    pending == eWifiStationCallbackSource_Reconnect) {
			setWifiStationConnectState(WifiStationConnectState_RECONNECTING);
			EhsWifiStationSetCBSource(eWifiStationCallbackSource_Reconnect);
		}
		break;
	}

	default:
		break;
	}
}

static void ipv4_mgmt_event_handler(struct net_mgmt_event_callback *cb,
                                    uint32_t mgmt_event, struct net_if *iface)
{
	ARG_UNUSED(iface);

	if (mgmt_event != NET_EVENT_IPV4_DHCP_BOUND) {
		return;
	}

	{
		const struct net_if_dhcpv4 *dhcpv4 = (const struct net_if_dhcpv4 *)cb->info;

		if (dhcpv4 == NULL) {
			return;
		}
		net_addr_ntop(AF_INET, &dhcpv4->requested_ip, IP_Address, sizeof(IP_Address));
	}

	gEhsWifiStationConnecting   = EHS_FALSE;
	gTargetWifiStationConnected = EHS_TRUE;
	setWifiStationConnectState(WifiStationConnectState_CONNECTING_GOT_IP);
	EhsWifiStationSetCBSource(eWifiStationCallbackSource_Connected);
	EHSH_LOG_INFO("Got IP: %s", IP_Address);
	EhsHMetaUpdateDynamic(); /* update network metadata with a new IP */
}

/*****************************************************************************/
/* Worker thread */

#ifdef CONFIG_WIFI_READY_LIB
/* Dispatched from the SYSTEM workqueue by the wifi_ready library — must stay
 * tiny (a semaphore give and nothing else). */
static void wifi_ready_cb(bool wifi_ready)
{
	if (wifi_ready) {
		k_sem_give(&s_wifi_ready_sem);
	}
}
#endif /* CONFIG_WIFI_READY_LIB */

static void worker_iface_up(void)
{
	int ret;

	if (gWifiStationInitalised == EHS_TRUE) {
		return;
	}

	if (!net_if_is_up(s_iface)) {
		ret = net_if_up(s_iface);
		/* CAUTION: a 0 return here does NOT mean the radio came up. In NCS
		 * v2.9.0, nrf_wifi_if_start_zep() sets `ret = k_mutex_lock(...)` (0)
		 * partway through and its later error paths `goto out` WITHOUT
		 * reassigning it, so an RPU bring-up failure - the whole
		 * fmac_fw_init / fmac_dev_init / fmac_dev_add_zep cascade - is
		 * returned to Ethernet L2 and to us as success. NET_IF_UP then gets
		 * set and even the wifi_ready callback fires. The only thing that
		 * still knows is the driver's private if_op_state, which is what
		 * makes the first scan fail with "Interface not UP". See the
		 * scan-rejection handling in worker_scan() for how that is caught. */
		if (ret != 0 && ret != -EALREADY) {
			EHSH_LOG_ERROR("net_if_up failed: %d", ret);
			setWifiStationConnectState(WifiStationConnectState_FAILED);
			gWifiStationBringupPosted = EHS_FALSE;
			return;
		}
	}

#ifdef CONFIG_WIFI_READY_LIB
	/* Blocks until the RPU firmware is loaded and wpa_supplicant has
	 * registered the interface. Issuing NET_REQUEST_WIFI_SCAN/CONNECT before
	 * this point fails with "RPU context not initialized". */
	if (k_sem_take(&s_wifi_ready_sem, K_MSEC(EHS_ZEPHYR_WIFI_READY_TIMEOUT_MS)) != 0) {
		EHSH_LOG_ERROR("Wi-Fi did not become ready within %d ms",
		               EHS_ZEPHYR_WIFI_READY_TIMEOUT_MS);
		setWifiStationConnectState(WifiStationConnectState_FAILED);
		gWifiStationBringupPosted = EHS_FALSE;
		return;
	}
#endif

	gWifiStationInitalised = EHS_TRUE;
	EHSH_LOG_INFO("Wi-Fi radio up and ready");

	/* Equivalent of ESP-IDF's WIFI_EVENT_STA_START: the radio is live, so if
	 * a connect is pending advance the SM into its scan phase, otherwise sit
	 * idle with the radio up (scan-only mode). */
	if (s_pending_valid == EHS_TRUE && s_pending_ssid[0] != '\0') {
		setWifiStationConnectState(WifiStationConnectState_SCANNING);
		EhsWifiStationSetCBSource(eWifiStationCallbackSource_Scan);
	} else if (getWifiStationConnectState() != WifiStationConnectState_CONNECT) {
		setWifiStationConnectState(WifiStationConnectState_IDLE);
	}
}

static void worker_scan(eScanPurpose purpose)
{
	/* Full, unfiltered active scan for BOTH purposes. Filtering the SM's scan
	 * by SSID would send directed probe requests only, and plenty of APs
	 * answer beacons but not directed probes — the ESP port hit exactly that
	 * and now filters client-side in the ScanResult case instead. */
	struct wifi_scan_params params;
	int ret;

	if (gWifiStationScanning == EHS_TRUE) {
		/* A scan is still in flight. Zephyr has no scan-abort request, so
		 * reissuing here would just be rejected by the driver; let the
		 * pending SCAN_DONE deliver the results and drive the SM. */
		EHSH_LOG_INFO("Scan already in flight; ignoring new request");
		return;
	}

	memset(&params, 0, sizeof(params));
	params.scan_type = WIFI_SCAN_TYPE_ACTIVE;
	/* bands = 0, dwell times = 0 and max_bss_cnt = 0 all mean "driver
	 * default" — see struct wifi_scan_params in zephyr/net/wifi_mgmt.h. */

	if (purpose == Scan_User) {
		s_user_ap_count = 0;
	} else {
		s_ap_count = 0;
	}
	s_scan_purpose = purpose;
	gWifiStationScanning = EHS_TRUE;

	ret = net_mgmt(NET_REQUEST_WIFI_SCAN, s_iface, &params, sizeof(params));
	if (ret != 0) {
		gWifiStationScanning = EHS_FALSE;
		s_scan_purpose = Scan_None;

		if (gTargetWifiStationConnected == EHS_FALSE) {
			/* Not associated, and the driver refused a scan: the radio is
			 * not actually usable. This is the ONLY point at which a failed
			 * RPU bring-up becomes visible to us - net_if_up() and the
			 * wifi_ready callback both report success even when
			 * nrf_wifi_fmac_dev_add_zep() failed (see worker_iface_up).
			 * Demote rather than retry: retrying just hides a dead radio
			 * behind a scan loop that can never succeed. */
			EHSH_LOG_ERROR("Scan rejected (%d) while not associated - "
			               "the radio is not operational", ret);
			EHSH_LOG_ERROR("  'Interface not UP' from nrf_wifi_disp_scan_zep means RPU "
			               "bring-up failed earlier; look for 'Init RX failed' further up "
			               "the log and check CONFIG_NRF70_RX_NUM_BUFS (must be >= 3 and a "
			               "multiple of 3 - see wifi_nrf70.mk).");
			gWifiStationInitalised = EHS_FALSE;
			gWifiStationBringupPosted = EHS_FALSE;
			setWifiStationConnectState(WifiStationConnectState_FAILED);
			EhsWifiStationSetCBSource(eWifiStationCallbackSource_Internal);
			return;
		}

		/* Associated: a refused scan is transient (the driver rejects scans
		 * during some connect/roam phases). Let the SM retry. */
		EHSH_LOG_WARNING("Scan request failed: %d", ret);
		if (purpose == Scan_Sm) {
			EhsWifiStationSetCBSource(eWifiStationCallbackSource_Scan);
		}
	}
}

static void worker_connect(const sWifiCmd *cmd)
{
	struct wifi_connect_req_params params;
	size_t psk_len;
	int ret;

	if (s_pending_valid != EHS_TRUE || s_pending_ssid[0] == '\0') {
		EHSH_LOG_WARNING("Connect requested with no credentials stashed");
		gEhsWifiStationConnecting = EHS_FALSE;
		setWifiStationConnectState(WifiStationConnectState_IDLE);
		return;
	}

	psk_len = strlen((const char *)s_pending_psk);

	memset(&params, 0, sizeof(params));
	params.ssid        = (const uint8_t *)s_pending_ssid;
	params.ssid_length = (uint8_t)strlen((const char *)s_pending_ssid);
	params.mfp         = WIFI_MFP_OPTIONAL;
	params.band        = WIFI_FREQ_BAND_UNKNOWN;
	/* 0 is not a valid Wi-Fi channel; WIFI_CHANNEL_ANY is how the mgmt layer
	 * spells "let the supplicant pick" (see __stored_creds_to_params in
	 * nrf/subsys/net/lib/wifi_mgmt_ext/wifi_mgmt_ext.c). */
	params.channel     = (cmd->channel != 0) ? cmd->channel : WIFI_CHANNEL_ANY;
	params.timeout     = (int)EHS_ZEPHYR_WIFI_CONNECT_TIMEOUT_S;

	if (s_pending_type == Type_WifiStation_Open || psk_len == 0) {
		params.security = WIFI_SECURITY_TYPE_NONE;
	} else {
		/* WPA_AUTO_PERSONAL covers WPA2-PSK and WPA3-SAE from one set of
		 * credentials (supp_api.c sets both psk and sae_password for it),
		 * so we don't have to know the AP's generation up front. */
		params.security         = WIFI_SECURITY_TYPE_WPA_AUTO_PERSONAL;
		params.psk              = (const uint8_t *)s_pending_psk;
		params.psk_length       = (uint8_t)psk_len;
	}

	if (cmd->bssid_set) {
		memcpy(params.bssid, cmd->bssid, WIFI_MAC_ADDR_LEN);
	}

	gEhsWifiStationConnecting = EHS_TRUE;
	ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, s_iface, &params, sizeof(params));
	if (ret != 0) {
		EHSH_LOG_WARNING("Connect request failed: %d", ret);
		gEhsWifiStationConnecting = EHS_FALSE;
		/* No CONNECT_RESULT event will follow a rejected request, so drive
		 * the retry directly. */
		setWifiStationConnectState(WifiStationConnectState_RECONNECTING);
		EhsWifiStationSetCBSource(eWifiStationCallbackSource_Reconnect);
	}
}

static void worker_disconnect(void)
{
	int ret = net_mgmt(NET_REQUEST_WIFI_DISCONNECT, s_iface, NULL, 0);

	/* -EALREADY / -ENOTCONN just mean there was nothing to drop. */
	if (ret != 0 && ret != -EALREADY && ret != -ENOTCONN) {
		EHSH_LOG_WARNING("Disconnect request failed: %d", ret);
	}
	gTargetWifiStationConnected = EHS_FALSE;
	gEhsWifiStationConnecting   = EHS_FALSE;
}

static void worker_iface_down(void)
{
	int ret;

	worker_disconnect();
	ret = net_if_down(s_iface);
	if (ret != 0 && ret != -EALREADY) {
		EHSH_LOG_WARNING("net_if_down failed: %d", ret);
	}
	gWifiStationInitalised    = EHS_FALSE;
	gWifiStationBringupPosted = EHS_FALSE;
	IP_Address[0] = '\0';
}

static void wifi_worker_entry(void *p1, void *p2, void *p3)
{
	sWifiCmd cmd;

	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	for (;;) {
		if (k_msgq_get(&s_wifi_cmdq, &cmd, K_FOREVER) != 0) {
			continue;
		}
		if (s_iface == NULL) {
			continue;
		}

		switch ((eWifiCmd)cmd.id) {
		case Wifi_Cmd_IfaceUp:    worker_iface_up();          break;
		case Wifi_Cmd_ScanSm:     worker_scan(Scan_Sm);       break;
		case Wifi_Cmd_ScanUser:   worker_scan(Scan_User);     break;
		case Wifi_Cmd_Connect:    worker_connect(&cmd);       break;
		case Wifi_Cmd_Disconnect: worker_disconnect();        break;
		case Wifi_Cmd_IfaceDown:  worker_iface_down();        break;
		default:                                              break;
		}
	}
}

static ehs_bool wifi_post(eWifiCmd id, const ehs_uint8 *bssid, ehs_uint8 channel)
{
	sWifiCmd cmd;

	memset(&cmd, 0, sizeof(cmd));
	cmd.id      = (ehs_uint8)id;
	cmd.channel = channel;
	if (bssid != NULL) {
		memcpy(cmd.bssid, bssid, WIFI_MAC_ADDR_LEN);
		cmd.bssid_set = 1;
	}

	/* Never block the caller: the SM thread posts these from its 1 s tick and
	 * a full queue means a request is already in flight. */
	if (k_msgq_put(&s_wifi_cmdq, &cmd, K_NO_WAIT) != 0) {
		EHSH_LOG_WARNING("Wi-Fi command queue full; dropped cmd %d", (int)id);
		return EHS_FALSE;
	}
	return EHS_TRUE;
}

/*****************************************************************************/
/* Target HAL contract — see target/Component-HAL/wifi/wifi_station.h */

ehs_bool doWifiStationNetifInit(const ehs_char *host_name)
{
	if (s_iface != NULL) {
		return EHS_TRUE; /* idempotent — both the boot path and the SM call this */
	}

	s_iface = net_if_get_first_wifi();
	if (s_iface == NULL) {
		EHSH_LOG_ERROR("No Wi-Fi interface found");
		return EHS_FALSE;
	}

	if (host_name != NULL && host_name[0] != '\0') {
#if defined(CONFIG_NET_HOSTNAME_ENABLE)
		net_hostname_set((char *)host_name, strlen((const char *)host_name));
#else
		/* CONFIG_NET_HOSTNAME_ENABLE is off for this target — there is no
		 * per-interface hostname to set, and DHCP will not advertise one. */
		EHSH_LOG_INFO("Hostname '%s' ignored (CONFIG_NET_HOSTNAME_ENABLE off)",
		              host_name);
#endif
	}

	net_mgmt_init_event_callback(&s_wifi_cb, wifi_mgmt_event_handler,
	                             NET_EVENT_WIFI_SCAN_RESULT |
	                             NET_EVENT_WIFI_SCAN_DONE |
	                             NET_EVENT_WIFI_CONNECT_RESULT |
	                             NET_EVENT_WIFI_DISCONNECT_RESULT);
	net_mgmt_add_event_callback(&s_wifi_cb);

	net_mgmt_init_event_callback(&s_ipv4_cb, ipv4_mgmt_event_handler,
	                             NET_EVENT_IPV4_DHCP_BOUND);
	net_mgmt_add_event_callback(&s_ipv4_cb);

#ifdef CONFIG_WIFI_READY_LIB
	{
		wifi_ready_callback_t cb = { .wifi_ready_cb = wifi_ready_cb };

		if (register_wifi_ready_callback(cb, s_iface) != 0) {
			EHSH_LOG_ERROR("Failed to register wifi_ready callback");
			return EHS_FALSE;
		}
	}
#endif

	EHSH_LOG_INFO("Wi-Fi interface registered");
	return EHS_TRUE;
}

void doWifiStationNetifDestroy(void)
{
	if (s_iface == NULL) {
		return;
	}
	wifi_post(Wifi_Cmd_IfaceDown, NULL, 0);
	net_mgmt_del_event_callback(&s_ipv4_cb);
	net_mgmt_del_event_callback(&s_wifi_cb);
}

eWifiStationStatus doWifiStationInit(void)
{
	/* Idempotent: both the boot path (target_main.c) and the first tick of
	 * wifi_station_thread call this. */
	if (doWifiStationNetifInit(NULL) == EHS_FALSE) {
		setWifiStationConnectState(WifiStationConnectState_FAILED);
		return WifiStation_InitFailed;
	}

	if (s_wifi_worker_started == EHS_FALSE) {
		s_wifi_worker_started = EHS_TRUE;
		k_thread_create(&s_wifi_worker_thread, s_wifi_worker_stack,
		                K_THREAD_STACK_SIZEOF(s_wifi_worker_stack),
		                wifi_worker_entry, NULL, NULL, NULL,
		                EHS_ZEPHYR_WIFI_THREAD_PRIORITY, 0, K_NO_WAIT);
		k_thread_name_set(&s_wifi_worker_thread, "ehs_wifi");
	}

	/* Bring the radio up in the background. Unlike esp_wifi_start() this can
	 * take seconds (RPU firmware load + supplicant iface registration), so it
	 * must not block the caller — the SM polls doWifiStationStart until the
	 * worker flips the state on. */
	if (gWifiStationInitalised == EHS_FALSE && gWifiStationBringupPosted == EHS_FALSE) {
		gWifiStationBringupPosted = EHS_TRUE;
		if (wifi_post(Wifi_Cmd_IfaceUp, NULL, 0) == EHS_FALSE) {
			gWifiStationBringupPosted = EHS_FALSE;
			return WifiStation_InitFailed;
		}
	}

	if (getWifiStationConnectState() != WifiStationConnectState_CONNECT) {
		setWifiStationConnectState(WifiStationConnectState_IDLE);
	}

	/* WifiStation_Connected is this enum's "no error" value — see
	 * doWifiStationInit in the ESP-IDF port, which returns the same. */
	return WifiStation_Connected;
}

eWifiStationStatus doWifiStationStart(
	ehs_char  *ssid,
	ehs_uint8  type,
	ehs_char  *PSKPass,
	ehs_uint8  EnterpriseType,
	ehs_uint8  EAP,
	ehs_uint8  TTLS2,
	ehs_bool   needServerCert,
	ehs_char  *serverCert,
	ehs_char  *tlsCert,
	ehs_char  *tlsKey,
	ehs_char  *eapID,
	ehs_char  *eapUser,
	ehs_char  *eapPass,
	ehs_char  *ip_address,
	ehs_char  *mac_address,
	ehs_sint8 *rssi)
{
	eWifiStationConnectState conn_state = getWifiStationConnectState();

	/* Enterprise (802.1X) is not wired up on this port. The Zephyr side needs
	 * CONFIG_WIFI_NM_WPA_SUPPLICANT_CRYPTO_ENTERPRISE plus a certificate
	 * store, neither of which exists for this target yet — reject it
	 * explicitly rather than silently attempting a PSK association. */
	ARG_UNUSED(EnterpriseType);
	ARG_UNUSED(EAP);
	ARG_UNUSED(TTLS2);
	ARG_UNUSED(needServerCert);
	ARG_UNUSED(serverCert);
	ARG_UNUSED(tlsCert);
	ARG_UNUSED(tlsKey);
	ARG_UNUSED(eapID);
	ARG_UNUSED(eapUser);
	ARG_UNUSED(eapPass);

	if (conn_state == WifiStationConnectState_CONNECT) {
		if (type == Type_WifiStation_Enterprise) {
			EHSH_LOG_ERROR("Enterprise Wi-Fi is not supported on this target");
			setWifiStationConnectState(WifiStationConnectState_FAILED);
			return WifiStation_NotImplemented;
		}
		if (ssid == NULL || ssid[0] == '\0') {
			setWifiStationConnectState(WifiStationConnectState_IDLE);
			return WifiStation_NotConfigured;
		}
		if (type == Type_WifiStation_PSK) {
			size_t psk_len = (PSKPass != NULL) ? strlen((const char *)PSKPass) : 0;

			/* wifi_connect() in zephyr/subsys/net/l2/wifi/wifi_mgmt.c rejects
			 * a PSK outside 8..64 with -EINVAL, so catch it here where we can
			 * report a meaningful status instead. */
			if (psk_len < 8 || psk_len > WIFI_PSK_MAX_LEN) {
				EHSH_LOG_ERROR("PSK length %u out of range (8..%d)",
				               (unsigned)psk_len, WIFI_PSK_MAX_LEN);
				setWifiStationConnectState(WifiStationConnectState_FAILED);
				return WifiStation_AuthFailed;
			}
		}

		/* Drop any current association before re-pointing at new creds. */
		if (gTargetWifiStationConnected == EHS_TRUE ||
		    gEhsWifiStationConnecting == EHS_TRUE) {
			wifi_post(Wifi_Cmd_Disconnect, NULL, 0);
		}

		memset(s_pending_ssid, 0, sizeof(s_pending_ssid));
		memset(s_pending_psk, 0, sizeof(s_pending_psk));
		strncpy((char *)s_pending_ssid, (const char *)ssid, sizeof(s_pending_ssid) - 1);
		if (type == Type_WifiStation_PSK) {
			strncpy((char *)s_pending_psk, (const char *)PSKPass, sizeof(s_pending_psk) - 1);
		}
		s_pending_type         = type;
		s_pending_valid        = EHS_TRUE;
		gWifiStationConfigured = EHS_TRUE;

		/* Make sure the radio bring-up has been kicked off (no-op if it has). */
		doWifiStationInit();

		/* Order matters. Set CONNECTING unconditionally FIRST, then promote
		 * to SCANNING if the radio is already up. worker_iface_up() performs
		 * the same promotion when wifi_ready fires, so whichever of the two
		 * runs last, every interleaving converges on SCANNING + cbSource=Scan
		 * — whereas testing first and writing after would let a bring-up that
		 * completes inside the window overwrite SCANNING with CONNECTING. */
		setWifiStationConnectState(WifiStationConnectState_CONNECTING);
		if (gWifiStationInitalised == EHS_TRUE) {
			EHSH_LOG_INFO("Credentials stashed for '%s'; starting scan",
			              s_pending_ssid);
			setWifiStationConnectState(WifiStationConnectState_SCANNING);
			EhsWifiStationSetCBSource(eWifiStationCallbackSource_Scan);
		} else {
			/* Radio still coming up; the SM polls this function each tick
			 * until worker_iface_up() promotes us, and its
			 * WIFI_SM_TIMEOUT_START_MS watchdog covers a stuck bring-up. */
			EHSH_LOG_INFO("Credentials stashed for '%s'; waiting for radio",
			              s_pending_ssid);
		}
		return WifiStation_Connecting;
	}

	if (conn_state == WifiStationConnectState_CONNECTING) {
		return WifiStation_Connecting;
	}

	if (conn_state == WifiStationConnectState_CONNECTING_GOT_IP) {
		struct wifi_iface_status status;

		memset(&status, 0, sizeof(status));
		if (s_iface != NULL &&
		    net_mgmt(NET_REQUEST_WIFI_IFACE_STATUS, s_iface,
		             &status, sizeof(status)) == 0) {
			snprintf((char *)mac_output, MACIP_LENGTH, "%02x:%02x:%02x:%02x:%02x:%02x",
			         (unsigned)(ehs_uint8)status.bssid[0], (unsigned)(ehs_uint8)status.bssid[1],
			         (unsigned)(ehs_uint8)status.bssid[2], (unsigned)(ehs_uint8)status.bssid[3],
			         (unsigned)(ehs_uint8)status.bssid[4], (unsigned)(ehs_uint8)status.bssid[5]);
			if (rssi != NULL) {
				*rssi = (ehs_sint8)status.rssi;
			}
		} else {
			mac_output[0] = '\0';
		}
		if (ip_address != NULL) {
			EhsStrcpy(ip_address, IP_Address);
		}
		if (mac_address != NULL) {
			EhsStrcpy(mac_address, mac_output);
		}
		EHSH_LOG_INFO("Connected to '%s', BSSID %s, IP %s",
		              s_pending_ssid, mac_output, IP_Address);
		return WifiStation_Connected;
	}

	if (conn_state == WifiStationConnectState_FAILED) {
		return WifiStation_StartFailed;
	}

	return WifiStation_StateError;
}

eWifiStationStatus doWifiStationConnect(ehs_uint8 *bssid, ehs_uint8 channel)
{
	if (gWifiStationConfigured == EHS_FALSE) {
		return WifiStation_NotConfigured;
	}
	if (gWifiStationInitalised == EHS_FALSE) {
		return WifiStation_NotStarted;
	}
	if (wifi_post(Wifi_Cmd_Connect, bssid, channel) == EHS_FALSE) {
		return WifiStation_InternalError;
	}
	return WifiStation_Connecting;
}

void doWifiStationDisconnect(void)
{
	wifi_post(Wifi_Cmd_Disconnect, NULL, 0);
	gTargetWifiStationConnected = EHS_FALSE;
}

void doWifiStationDestroy(void)
{
	/* Deliberately NOT a net_if_down(): unlike ESP-IDF there is nothing to be
	 * gained from tearing the driver down, and re-running the RPU firmware
	 * load is the slowest and most failure-prone step in the whole bring-up.
	 * Drop the association and forget the credentials; the radio stays up so
	 * scans keep working and a fresh connect is immediate. Full teardown is
	 * doWifiStationNetifDestroy(). */
	wifi_post(Wifi_Cmd_Disconnect, NULL, 0);
	gTargetWifiStationConnected = EHS_FALSE;
	gWifiStationConfigured      = EHS_FALSE;
	s_pending_valid             = EHS_FALSE;
	memset(s_pending_ssid, 0, sizeof(s_pending_ssid));
	memset(s_pending_psk, 0, sizeof(s_pending_psk));
	IP_Address[0] = '\0';
}

void doWifiStationScan(ehs_char *ssid)
{
	(void)ssid; /* unfiltered scan, filtered client-side — see worker_scan() */
	wifi_post(Wifi_Cmd_ScanSm, NULL, 0);
}

eWifiStationStatus doWifiStationFullScan(ehs_bool print)
{
	if (gWifiStationInitalised == EHS_FALSE) {
		return WifiStation_NotStarted;
	}
	if (gWifiStationScanning == EHS_TRUE) {
		return WifiStation_StateError; /* a scan is already in flight */
	}
	if (wifi_post(Wifi_Cmd_ScanUser, NULL, 0) == EHS_FALSE) {
		return WifiStation_InternalError;
	}
	gWifiStationScanResultPrint = (print != EHS_FALSE);
	return WifiStation_Connected; /* this enum's "no error" value */
}

ehs_bool doWifiStationScanStop(void)
{
	if (gWifiStationScanning == EHS_FALSE) {
		/* Nothing running — the caller's intent is already satisfied. */
		return EHS_TRUE;
	}
	/* Zephyr's Wi-Fi management API has no scan-abort request (there is no
	 * NET_REQUEST_WIFI_CMD_SCAN_STOP in enum net_request_wifi_cmd), and the
	 * nRF70 driver exposes none either. An in-flight scan runs to completion;
	 * SCAN_DONE will clear the flag. */
	EHSH_LOG_INFO("Scan abort not supported on this target; scan will run to completion");
	return EHS_FALSE;
}

/* NOTE on the ehs_sint8 out-parameter: base_types.h declares ehs_sint8 as a
 * plain `char`, which arm-zephyr-eabi treats as UNSIGNED (Zephyr does not pass
 * -fsigned-char). A negative dBm therefore survives the store here but reads
 * back positive once a caller widens it to ehs_sint32. That is a cross-target
 * typedef defect in the base_full / base_small base_types.h, not something
 * this backend can correct locally — the fix is "typedef signed char
 * ehs_sint8", which changes the ABI for every target using those headers. */
ehs_bool doWifiStationGetRssi(ehs_sint8 *rssi)
{
	struct wifi_iface_status status;

	if (rssi == NULL || s_iface == NULL || gTargetWifiStationConnected == EHS_FALSE) {
		return EHS_FALSE;
	}

	memset(&status, 0, sizeof(status));
	if (net_mgmt(NET_REQUEST_WIFI_IFACE_STATUS, s_iface, &status, sizeof(status)) != 0) {
		return EHS_FALSE;
	}
	*rssi = (ehs_sint8)status.rssi;
	return EHS_TRUE;
}

/*****************************************************************************/
/* Scan result accessors */

ehs_sint32 WifiStationScanResultCount(void)
{
	return (ehs_sint32)s_ap_count;
}

ehs_bool WifiStationScanResult(ehs_uint32 index, ehs_char *ssid, ehs_uint16 ssid_size,
                               ehs_char *bssid, ehs_uint16 bssid_size,
                               ehs_sint32 *channel, ehs_sint32 *rssi)
{
	const sWifiApRecord *p;

	if (ssid_size < (WIFI_SSID_MAX_LEN + 1) || bssid_size < WIFI_MAC_ADDR_LEN) {
		return EHS_FALSE;
	}
	if (index >= s_ap_count) {
		return EHS_FALSE;
	}
	p = &s_ap_records[index];
	if (p->ssid[0] == '\0') {
		return EHS_FALSE;
	}
	if (ssid != NULL) {
		EhsStrcpy(ssid, (const ehs_char *)p->ssid);
	}
	if (bssid != NULL) {
		memcpy(bssid, p->bssid, WIFI_MAC_ADDR_LEN);
	}
	if (channel != NULL) {
		*channel = (ehs_sint32)p->channel;
	}
	if (rssi != NULL) {
		*rssi = (ehs_sint32)p->rssi;
	}
	return EHS_TRUE;
}

ehs_uint16 Wifi_UserScanResultCount(void)
{
	return s_user_ap_count;
}

ehs_bool Wifi_UserScanResultGet(ehs_uint16 index,
                                ehs_char *ssid, ehs_uint16 ssid_size,
                                ehs_uint8 *bssid, ehs_uint16 bssid_size,
                                ehs_sint32 *channel, ehs_sint32 *rssi)
{
	const sWifiApRecord *p;

	if (ssid_size < (WIFI_SSID_MAX_LEN + 1) || bssid_size < WIFI_MAC_ADDR_LEN) {
		return EHS_FALSE;
	}
	if (index >= s_user_ap_count) {
		return EHS_FALSE;
	}
	p = &s_user_ap_records[index];
	if (p->ssid[0] == '\0') {
		return EHS_FALSE;
	}
	if (ssid != NULL) {
		EhsStrcpy(ssid, (const ehs_char *)p->ssid);
	}
	if (bssid != NULL) {
		memcpy(bssid, p->bssid, WIFI_MAC_ADDR_LEN);
	}
	if (channel != NULL) {
		*channel = (ehs_sint32)p->channel;
	}
	if (rssi != NULL) {
		*rssi = (ehs_sint32)p->rssi;
	}
	return EHS_TRUE;
}

void EhsWifiStationSetPrintOnScanDone(ehs_bool enable)
{
	gWifiStationScanResultPrint = enable;
}

ehs_bool WifiStationIsScanResultReady(void)
{
	return gWifiStationScanResultReady;
}

void WifiStationPrintAndClearScanResults(void)
{
	ehs_bool do_print = gWifiStationScanResultPrint;
	ehs_uint16 index;
	ehs_uint16 printed = 0;
	/* Static, not automatic: this runs on the console task, whose stack is
	 * small on MCU targets. */
	static ehs_char seen[EHS_ZEPHYR_MAX_AP_RECORDS][WIFI_SSID_MAX_LEN + 1];
	ehs_uint16 seen_count = 0;

	gWifiStationScanResultPrint = EHS_FALSE;
	gWifiStationScanResultReady = EHS_FALSE;
	if (!do_print) {
		return;
	}

	for (index = 0; index < s_user_ap_count; index++) {
		const sWifiApRecord *p = &s_user_ap_records[index];
		ehs_bool duplicate = EHS_FALSE;
		ehs_uint16 j;

		if (p->ssid[0] == '\0') {
			continue;
		}
		for (j = 0; j < seen_count; j++) {
			if (strcmp((const char *)seen[j], (const char *)p->ssid) == 0) {
				duplicate = EHS_TRUE;
				break;
			}
		}
		if (duplicate) {
			continue;
		}
		if (seen_count < EHS_ZEPHYR_MAX_AP_RECORDS) {
			strncpy((char *)seen[seen_count], (const char *)p->ssid, WIFI_SSID_MAX_LEN);
			seen[seen_count][WIFI_SSID_MAX_LEN] = '\0';
			seen_count++;
		}
		printf("SSID=%s, BSSID(MAC)=%02x:%02x:%02x:%02x:%02x:%02x, Channel=%d, RSSI=%d dBm\n",
		       p->ssid, p->bssid[0], p->bssid[1], p->bssid[2],
		       p->bssid[3], p->bssid[4], p->bssid[5],
		       (int)p->channel, (int)p->rssi);
		printed++;
	}
	if (printed == 0) {
		printf("No SSID found!\n");
	}
}

/*****************************************************************************/
/* State accessors */

const ehs_char *WifiStationIpAddress(void)
{
	return IP_Address;
}

ehs_bool isWifiStationInitalised(void)
{
	return gWifiStationInitalised;
}

ehs_bool isWifiStationConnected(void)
{
	return gTargetWifiStationConnected;
}

ehs_bool isWifiStationScanning(void)
{
	return gWifiStationScanning;
}

ehs_bool isWifiStationConnecting(void)
{
	return gEhsWifiStationConnecting;
}
