/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_wifi_test.c
 * TEST_FUNC harness for the Zephyr / nRF70 Wi-Fi station HAL.
 *
 *   ./configure zephyr_arm-nrf9151_thingy91x-wifi
 *   make TEST_FUNC=test_wifi_station zephyr_cmake_gen
 *   make TEST_FUNC=test_wifi_station zephyr_build_docker
 *
 * Runs instead of EhsMain() (see the EHS_TEST_FUNC_OVERRIDE block in
 * target_main.c) so the radio can be exercised with no SODL, no filesystem and
 * no application in the way. Everything it touches is the PUBLIC Wi-Fi API in
 * target/Component-HAL/wifi/wifi_station.h — the shared connect state machine
 * in Common/Components/networking/inx-wifi_station.c and this OS-arch's HAL
 * backend in target_wifi.c. It deliberately does NOT call Zephyr net_mgmt
 * directly; that is what wifi_connect_test.c does, and it tests the driver
 * rather than the eRT layer sitting on it.
 *
 * >>> EDIT THE TWO CREDENTIAL DEFINES BELOW BEFORE BUILDING. <<<
 *
 * Test sequence:
 *   1. Bring the interface and radio up (netif init → driver init → wait ready).
 *   2. Full scan; print every AP found.
 *   3. Connect with the credentials below; follow the SM through
 *      Scanning → Associating → Authenticated → Online.
 *   4. Report IP + RSSI, then monitor the link, reporting every state change
 *      and printing a periodic status line.
 *
 * @author: inx limited
 */

/* Module-scoped logger ID. Must precede every include: several headers pull in
 * hal_logger.h, which latches EHSL_MODULE_ID to UNDEFINED if it is not set yet. */
#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

#include <zephyr/kernel.h>

#include <stdio.h>
#include <string.h>

#include "globals.h"
#include "hal_logger.h"

#ifdef EHS_TEST_FUNC_OVERRIDE

#include "wifi_station.h"
#include "targetos_init.h"

/* ==========================================================================
 *                    >>>  Wi-Fi CREDENTIALS — EDIT THESE  <<<
 * ==========================================================================
 *
 * Replace the two placeholder strings with the network under test.
 *
 * EHS_WIFI_TEST_PSK must be 8..64 characters (the 802.11 PSK range that
 * zephyr/subsys/net/l2/wifi/wifi_mgmt.c enforces); the build-time check below
 * rejects an out-of-range or still-unedited value rather than letting it fail
 * at run time as an unexplained association failure.
 *
 * For an OPEN network, set EHS_WIFI_TEST_OPEN to 1 and leave the PSK alone.
 *
 * These are compiled into the image in the clear. Fine for a lab network;
 * do not build a production image with real credentials this way.
 */
#define EHS_WIFI_TEST_SSID  "INX_11AC"
#define EHS_WIFI_TEST_PSK   "WSINX909wIfIPW*"
#define EHS_WIFI_TEST_OPEN  0        /* 1 = open network, PSK ignored */

/* ========================================================================== */

/* sizeof("literal") includes the null, hence the -1 for the character count. */
#define EHS_WIFI_TEST_SSID_LEN (sizeof(EHS_WIFI_TEST_SSID) - 1)
#define EHS_WIFI_TEST_PSK_LEN  (sizeof(EHS_WIFI_TEST_PSK) - 1)

BUILD_ASSERT(EHS_WIFI_TEST_SSID_LEN > 0 && EHS_WIFI_TEST_SSID_LEN <= 32,
             "EHS_WIFI_TEST_SSID must be 1..32 characters - edit it in target_wifi_test.c");
#if !EHS_WIFI_TEST_OPEN
BUILD_ASSERT(EHS_WIFI_TEST_PSK_LEN >= 8 && EHS_WIFI_TEST_PSK_LEN <= 64,
             "EHS_WIFI_TEST_PSK must be 8..64 characters - edit it in target_wifi_test.c");
#endif

/* The console on this board is USB-CDC via the nRF5340 bridge and takes a few
 * seconds to enumerate on the host; anything printed before then is lost. Same
 * reasoning (and same value) as the delay in target_main.c's EHS thread. */
#define WIFI_TEST_CONSOLE_SETTLE_MS 6000

/* Radio bring-up budget: net_if_up() → RPU firmware load → wpa_supplicant
 * interface registration. Matches the HAL's own wifi_ready wait so the test
 * does not give up while the HAL is still legitimately waiting. */
#define WIFI_TEST_RADIO_READY_TIMEOUT_MS 30000

/* Scan and connect budgets. The connect budget must exceed the shared SM's own
 * retry burst, or the test reports failure while the SM is still working:
 * WIFI_SM_TIMEOUT_SCAN_MS + WIFI_SM_TIMEOUT_ASSOC_MS is one full scan+assoc
 * cycle, and the SM will run several of those. Three cycles is a reasonable
 * "it should have worked by now" line for a test. */
#define WIFI_TEST_SCAN_TIMEOUT_MS    (WIFI_SM_TIMEOUT_SCAN_MS + 5000U)
#define WIFI_TEST_CONNECT_TIMEOUT_MS (3U * (WIFI_SM_TIMEOUT_SCAN_MS + WIFI_SM_TIMEOUT_ASSOC_MS))

#define WIFI_TEST_POLL_MS       250
#define WIFI_TEST_STATUS_EVERY_MS 10000

#define TEST_LOG(fmt, ...) do { \
	printf("WIFI-TEST: " fmt "\n", ##__VA_ARGS__); \
	fflush(stdout); \
} while (0)

/**
 * Poll Wifi_GetState() until it reaches one of the wanted states, printing
 * every transition on the way.
 *
 * @param wanted_a   first accepted state
 * @param wanted_b   second accepted state (pass wanted_a again if only one)
 * @param timeout_ms give up after this long
 * @return the state reached, or the last state seen on timeout
 */
static EhsWifiState wifi_test_wait_state(EhsWifiState wanted_a,
                                         EhsWifiState wanted_b,
                                         ehs_uint32 timeout_ms)
{
	EhsWifiState last = Wifi_GetState();
	ehs_uint32 waited = 0;

	TEST_LOG("  state: %s", Wifi_GetStateName(last));

	while (last != wanted_a && last != wanted_b) {
		EhsWifiState now;

		if (waited >= timeout_ms) {
			TEST_LOG("  TIMEOUT after %u ms in state %s",
			         (unsigned)waited, Wifi_GetStateName(last));
			break;
		}
		k_msleep(WIFI_TEST_POLL_MS);
		waited += WIFI_TEST_POLL_MS;

		now = Wifi_GetState();
		if (now != last) {
			last = now;
			TEST_LOG("  state: %s (+%u ms)", Wifi_GetStateName(last), (unsigned)waited);
		}
	}
	return last;
}

/** Print the AP list from the last user scan. */
static void wifi_test_print_scan(void)
{
	ehs_uint16 total = Wifi_UserScanResultCount();
	ehs_uint16 i;

	TEST_LOG("scan found %u AP(s)", (unsigned)total);

	for (i = 0; i < total; i++) {
		/* WifiStationScanResult/Wifi_UserScanResultGet require a >= 33-byte
		 * SSID buffer and a >= 6-byte BSSID buffer. */
		ehs_char   ssid[33];
		ehs_uint8  bssid[6];
		ehs_sint32 channel = 0;
		ehs_sint32 rssi = 0;

		if (Wifi_UserScanResultGet(i, ssid, sizeof(ssid), bssid, sizeof(bssid),
		                           &channel, &rssi) != EHS_TRUE) {
			continue;
		}
		TEST_LOG("  [%2u] %-32s %02x:%02x:%02x:%02x:%02x:%02x  ch %3d  %4d dBm%s",
		         (unsigned)i, ssid,
		         bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5],
		         (int)channel, (int)rssi,
		         (strcmp((const char *)ssid, EHS_WIFI_TEST_SSID) == 0) ? "   <-- target" : "");
	}
}

/**
 * TEST_FUNC entry point. Never returns — MCU test functions loop forever so the
 * board stays in a known state for inspection.
 */
void test_wifi_station(void)
{
#ifdef EHS_TEST_FUNC_NO_ERT_INIT
	/* ERT_INIT=none skipped target_main.c's EhsTOsSys_init(). The shared Wi-Fi
	 * state machine runs on an eRT thread and takes eRT mutexes, so bring that
	 * one layer up even in "bare metal" mode. Nothing else from eRT is needed. */
	EhsTOsSys_init();
#endif

	k_msleep(WIFI_TEST_CONSOLE_SETTLE_MS);

	TEST_LOG("==================================================");
	TEST_LOG(" eRT Wi-Fi station HAL test (Zephyr / nRF70)");
	TEST_LOG(" target SSID : '%s' (%u chars)",
	         EHS_WIFI_TEST_SSID, (unsigned)EHS_WIFI_TEST_SSID_LEN);
#if EHS_WIFI_TEST_OPEN
	TEST_LOG(" security    : OPEN");
#else
	TEST_LOG(" security    : PSK (%u chars)", (unsigned)EHS_WIFI_TEST_PSK_LEN);
#endif
	TEST_LOG("==================================================");

	/* Placeholders build fine (so the harness can be compiled before the
	 * network is known) but obviously cannot connect. Say so loudly rather
	 * than letting it look like a driver fault two minutes later. */
	if (strcmp(EHS_WIFI_TEST_SSID, "PUT_SSID_HERE") == 0) {
		TEST_LOG("");
		TEST_LOG("  !! EHS_WIFI_TEST_SSID is still the placeholder.");
		TEST_LOG("  !! Edit EHS_WIFI_TEST_SSID / EHS_WIFI_TEST_PSK near the top of");
		TEST_LOG("  !! target/os-arch/zephyr-arm/target_wifi_test.c and rebuild.");
		TEST_LOG("  !! Continuing anyway - scan results below are still meaningful,");
		TEST_LOG("  !! the connect step will fail with 'AP not found'.");
		TEST_LOG("");
	}

	/* ---- 1. Interface + radio bring-up ---------------------------------- */
	TEST_LOG("[1/4] bringing the interface up");
	if (doWifiStationNetifInit(NULL) != EHS_TRUE) {
		TEST_LOG("FAIL: doWifiStationNetifInit() - no Wi-Fi interface found.");
		TEST_LOG("      Check CONFIG_WIFI_NRF70=y and SB_CONFIG_WIFI_NRF70=y actually");
		TEST_LOG("      resolved to y (sysbuild has its own Kconfig namespace).");
		goto park;
	}
	if (doWifiStationInit() != WifiStation_Connected) {
		TEST_LOG("FAIL: doWifiStationInit()");
		goto park;
	}

	TEST_LOG("[2/4] waiting for the radio (RPU firmware + supplicant), up to %u ms",
	         (unsigned)WIFI_TEST_RADIO_READY_TIMEOUT_MS);
	{
		ehs_uint32 waited = 0;

		while (isWifiStationInitalised() != EHS_TRUE) {
			if (Wifi_GetState() == EhsWifi_Failed) {
				TEST_LOG("FAIL: bring-up reported failure after %u ms - see the "
				         "driver errors above.", (unsigned)waited);
				goto park;
			}
			if (waited >= WIFI_TEST_RADIO_READY_TIMEOUT_MS) {
				TEST_LOG("FAIL: radio not ready after %u ms - the wifi_ready", (unsigned)waited);
				TEST_LOG("      callback never fired. Scan the log ABOVE for the first");
				TEST_LOG("      <err>, which says which stage did not complete:");
				TEST_LOG("        'Init RX failed'                  -> RPU init; check");
				TEST_LOG("             CONFIG_NRF70_RX_NUM_BUFS (>=3, multiple of 3).");
				TEST_LOG("        'No space for allocating RX buffer' -> heap too small.");
				TEST_LOG("        'Failed to add iface'             -> supplicant could not");
				TEST_LOG("             register the interface; heap, or the 1 s");
				TEST_LOG("             IFACE_NOTIFY_TIMEOUT_MS in supp_main.c.");
				TEST_LOG("        'Failed to initialize supplicant control interface'");
				TEST_LOG("                                          -> heap: each");
				TEST_LOG("             socketpair costs 2 x CONFIG_NET_SOCKETPAIR_BUFFER_SIZE");
				TEST_LOG("             and the supplicant opens two.");
				TEST_LOG("      All of these are heap-related except the first - see the");
				TEST_LOG("      HEAP_MEM_POOL_ADD_SIZE note in wifi_nrf70.mk.");
				goto park;
			}
			k_msleep(WIFI_TEST_POLL_MS);
			waited += WIFI_TEST_POLL_MS;
		}
		/* "Ready" here means net_if_up() succeeded and wifi_ready fired.
		 * Neither is conclusive - the nRF70 driver reports success from
		 * net_if_up() even when RPU bring-up failed (see the comment in
		 * target_wifi.c worker_iface_up). The scan below is the first thing
		 * that actually exercises the radio, so treat it as the real verdict. */
		TEST_LOG("  radio reported ready after %u ms (unverified until the scan)",
		         (unsigned)waited);
	}

	/* ---- 2. Scan --------------------------------------------------------- */
	TEST_LOG("[3/4] scanning (this is what proves the radio is really up)");
	if (Wifi_Scan() != EHS_TRUE) {
		/* Not fatal by itself: a scan can be refused because one is already
		 * running. A dead radio is caught by the Failed check below instead. */
		TEST_LOG("  WARN: Wifi_Scan() refused - continuing to the connect step");
	} else {
		ehs_uint32 waited = 0;

		while (WifiStationIsScanResultReady() != EHS_TRUE &&
		       waited < WIFI_TEST_SCAN_TIMEOUT_MS) {
			k_msleep(WIFI_TEST_POLL_MS);
			waited += WIFI_TEST_POLL_MS;
		}
		if (WifiStationIsScanResultReady() != EHS_TRUE) {
			TEST_LOG("  WARN: no SCAN_DONE within %u ms", (unsigned)waited);
		} else {
			wifi_test_print_scan();
		}
		/* Clears the ready/print flags. Prints nothing unless the print flag
		 * was set (Wifi_Scan sets it) - we have already printed the list
		 * ourselves above, so this is only here to clear the flags for the
		 * connect-SM scans that follow. */
		EhsWifiStationSetPrintOnScanDone(EHS_FALSE);
		WifiStationPrintAndClearScanResults();
	}

	/* The HAL demotes to Failed if the driver refused the scan while we were
	 * not associated - that is the real "the radio never came up" verdict,
	 * whatever net_if_up() and wifi_ready claimed earlier. Stop here rather
	 * than spending the whole connect budget on a radio that cannot work. */
	if (Wifi_GetState() == EhsWifi_Failed) {
		TEST_LOG("FAIL: the radio is not operational - the scan was rejected.");
		TEST_LOG("      Look further up the log for 'Init RX failed' /");
		TEST_LOG("      'nrf_wifi_fmac_dev_add_zep failed'. The usual cause is");
		TEST_LOG("      CONFIG_NRF70_RX_NUM_BUFS < 3 or not a multiple of 3 -");
		TEST_LOG("      see ERT_ZEPHYR_NRF70_RX_NUM_BUFS in wifi_nrf70.mk.");
		goto park;
	}

	/* ---- 3. Connect ------------------------------------------------------ */
	TEST_LOG("[4/4] connecting to '%s'", EHS_WIFI_TEST_SSID);
	/* Wifi_Connect stores the credentials and starts the shared station
	 * thread; the rest of the sequence is driven by that SM plus the HAL's
	 * net_mgmt event handlers. Everything below is observation only. */
#if EHS_WIFI_TEST_OPEN
	Wifi_Connect(EHS_WIFI_TEST_SSID, "");
#else
	Wifi_Connect(EHS_WIFI_TEST_SSID, EHS_WIFI_TEST_PSK);
#endif

	if (wifi_test_wait_state(EhsWifi_Online, EhsWifi_Failed,
	                         WIFI_TEST_CONNECT_TIMEOUT_MS) != EhsWifi_Online) {
		TEST_LOG("FAIL: not online. last disconnect reason %d (%s)",
		         (int)Wifi_GetLastDisconnectReason(),
		         Wifi_GetLastDisconnectReasonText());
		TEST_LOG("      Reason 2 on this backend is WIFI_STATUS_CONN_WRONG_PASSWORD,");
		TEST_LOG("      4 is AP not found. See enum wifi_conn_status in wifi_mgmt.h.");
		goto park;
	}

	TEST_LOG("==================================================");
	TEST_LOG(" PASS - online");
	TEST_LOG("   IP   : %s", Wifi_GetIp());
	{
		ehs_sint8 rssi = 0;

		if (Wifi_GetRssi(&rssi) == EHS_TRUE) {
			/* ehs_sint8 is a plain `char`, which is UNSIGNED on arm-zephyr-eabi,
			 * so print it through a signed cast or every RSSI reads as ~200.
			 * See the note on doWifiStationGetRssi in target_wifi.c. */
			TEST_LOG("   RSSI : %d dBm", (int)(signed char)rssi);
		} else {
			TEST_LOG("   RSSI : unavailable");
		}
	}
	TEST_LOG("==================================================");

	/* ---- 4. Monitor ------------------------------------------------------ */
	TEST_LOG("monitoring the link - reporting every state change");
	{
		EhsWifiState last = Wifi_GetState();
		ehs_uint32 since_status = 0;

		for (;;) {
			EhsWifiState now;

			k_msleep(WIFI_TEST_POLL_MS);
			since_status += WIFI_TEST_POLL_MS;

			now = Wifi_GetState();
			if (now != last) {
				last = now;
				TEST_LOG("state -> %s%s", Wifi_GetStateName(now),
				         (now == EhsWifi_Online) ? "" : "  (link event)");
				if (now != EhsWifi_Online) {
					TEST_LOG("  last disconnect reason %d (%s)",
					         (int)Wifi_GetLastDisconnectReason(),
					         Wifi_GetLastDisconnectReasonText());
				}
				since_status = WIFI_TEST_STATUS_EVERY_MS; /* force a status line */
			}

			if (since_status >= WIFI_TEST_STATUS_EVERY_MS) {
				ehs_sint8 rssi = 0;
				ehs_bool have_rssi = Wifi_GetRssi(&rssi);

				since_status = 0;
				TEST_LOG("status: %-13s ip=%-15s rssi=%d dBm",
				         Wifi_GetStateName(now),
				         (Wifi_GetIp()[0] != '\0') ? Wifi_GetIp() : "-",
				         have_rssi ? (int)(signed char)rssi : 0);
			}
		}
	}

park:
	/* MCU test functions never return - park so the board stays inspectable. */
	TEST_LOG("test finished - parking");
	for (;;) {
		k_msleep(5000);
		printf("W");        /* repeating life-sign, survives a late console attach */
		fflush(stdout);
	}
}

#else /* !EHS_TEST_FUNC_OVERRIDE */

/* ISO C forbids an empty translation unit. This file is in OBJECTS
 * unconditionally (alongside target_wifi.c) so that a TEST_FUNC build needs no
 * makefile change; keep it legal when the test is compiled out. */
typedef int ehs_wifi_test_translation_unit_not_empty;

#endif /* EHS_TEST_FUNC_OVERRIDE */
