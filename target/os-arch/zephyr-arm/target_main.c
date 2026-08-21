/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_main.c
 * This file represents the main entry point for the Zephyr RTOS target.
 * Zephyr's main() thread is used to initialise and run the EHS kernel.
 *
 * @author: inx limited
 *
 */

#define EHS_TARGET_CODE

/*****************************************************************************/
/* Included files */
#include <zephyr/kernel.h>
#include <stdio.h>

#include "globals.h"

#ifdef EHS_DEBUG_TCPIP_CONSOLE
  #include "console_server.h"
#endif

#include "ehs_main.h"
#include "ehs_types.h"   /* Ehs_ConsoleCommand_Type / EHS_CONTINUE */
#include "targetos_init.h"
#include "hal-api.h"

#ifdef EHS_MQTT_SUPPORT
#include "hal_mqtt.h"
#endif

#ifdef EHS_NETWORK_WIFI_SUPPORT
#include "wifi_station.h"
#endif

#ifdef EHS_DEBUG_TCPIP_CONSOLE
/* For EHS_TGT_TCP_PORTNUM in the heartbeat's NET status line. Included
 * directly rather than relying on hal-api.h: that only pulls in hal_network.h
 * (and hence target_tcp.h) under EHS_NETWORKING_SUPPORT, which is 'none' on
 * this target - the Lucid console does not need the eRT URL/devapps layer. */
#include "target_tcp.h"
#endif

#include "hal_logger.h"

#ifndef TAG
#define TAG "target_main"
#endif

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/* Stack size for the EHS main kernel thread */
#define EHS_MAIN_THREAD_STACK_SIZE 8192

/* Priority for the EHS main kernel thread (preemptive) */
#define EHS_MAIN_THREAD_PRIORITY 5

/* Emit an EHS-loop life-sign roughly every N scheduler passes. Tune to taste —
 * the scheduler pass rate depends on the running app's group activity. */
#define EHS_LOOP_HEARTBEAT_EVERY 2000u

/*****************************************************************************/
/* Forward declarations */

#ifdef EHS_TEST_FUNC_OVERRIDE
/* TEST_FUNC replaces EhsInit/EhsMain entirely, so the normal kernel-thread
 * entry and its heartbeat hook are not built at all - leaving them in would
 * only produce -Wunused-function noise. */
static void ehs_test_thread_entry(void *p1, void *p2, void *p3);
#else
static void ehs_main_thread_entry(void *p1, void *p2, void *p3);
#ifndef EHS_ZEPHYR_THREAD_SMOKETEST
static Ehs_ConsoleCommand_Type ehs_loop_heartbeat(void *env);
#endif
#endif /* EHS_TEST_FUNC_OVERRIDE */

/*****************************************************************************/
/* Thread stack and data for the EHS kernel thread */

K_THREAD_STACK_DEFINE(ehs_main_stack, EHS_MAIN_THREAD_STACK_SIZE);
static struct k_thread ehs_main_thread_data;

/*****************************************************************************/
/* Zephyr main entry point */

/**
 * Zephyr main() - runs in the main thread context.
 *
 * Boot sequence:
 * 1. Print boot banner
 * 2. Initialise EHS OS layer (mutexes, etc.)
 * 3. Spawn the EHS kernel thread
 * 4. Main thread becomes idle (or can be used for console/shell)
 *
 * Note: On Zephyr, main() runs as a cooperative thread. We spawn
 * a separate preemptive thread for the EHS kernel to allow proper
 * scheduling with other Zephyr threads and ISRs.
 */
int main(void)
{
#ifdef EHS_ZEPHYR_UNBUFFERED_STDOUT
    /* Fully unbuffer stdout so every printf/vprintf (incl. the kernel's
     * EhsConsolePrintf) hits the UART immediately with no fflush. ONLY safe when
     * a SINGLE thread owns the console. With the current two-thread model
     * (Zephyr main() heartbeat + separate EHS thread) it races the console
     * driver and hangs — so it is OFF by default. Enable it if/when EhsMain is
     * collapsed onto the main thread (single console writer). */
    setvbuf(stdout, NULL, _IONBF, 0);
#endif

    printf("\n--- eRT Zephyr Target Boot ---\n");
    fflush(stdout);

#if defined(EHS_TEST_FUNC_OVERRIDE) && defined(EHS_TEST_FUNC_NO_ERT_INIT)
    /* Bare-metal test mode (make TEST_FUNC=x ERT_INIT=none): run the test
     * straight from main() with no eRT init at all, then park. A test that
     * needs part of eRT (the Wi-Fi HAL needs the mutex layer, for instance)
     * brings up just that piece itself - see target_wifi_test.c. */
    {
        extern void EHS_TEST_FUNC_NAME(void);

        printf("EHS Bare Metal Test: running %s\n", EHS_MACRO_STRINGIFY(EHS_TEST_FUNC_NAME));
        fflush(stdout);
        EHS_TEST_FUNC_NAME();
        printf("Test completed\n");
        fflush(stdout);
        while (1) {
            k_msleep(1000);
        }
    }
#endif /* EHS_TEST_FUNC_OVERRIDE && EHS_TEST_FUNC_NO_ERT_INIT */

    /* Initialise the OS abstraction layer */
    EhsTOsSys_init();

#ifdef EHS_ZEPHYR_WIFI_CONNECT_TEST
    /* Standalone Wi-Fi connect + DHCP bring-up test - see wifi_connect_test.c.
     * Mutually exclusive with the real HAL below (target.mk enforces the
     * default; both would call net_if_up() and issue their own connect). */
    extern void ehs_wifi_connect_test_start(void);
    ehs_wifi_connect_test_start();
#endif

#if defined(EHS_NETWORK_WIFI_SUPPORT) && !defined(EHS_TEST_FUNC_OVERRIDE)
    /* Skipped under TEST_FUNC: a Wi-Fi test owns the whole bring-up/connect
     * sequence itself, and a second connect started from here would race it.
     *
     * Bring the Wi-Fi radio up and hand the connect state machine its
     * credentials, mirroring the boot path in the ESP32-S3 target_main.c.
     * doWifiStationNetifInit + doWifiStationInit are idempotent and
     * non-blocking here: the RPU firmware load and wpa_supplicant interface
     * registration run on target_wifi.c's own worker thread, so this returns
     * long before the radio is actually ready. */
    if (doWifiStationNetifInit(NULL) == EHS_TRUE) {
        doWifiStationInit();
#ifdef CONFIG_WIFI_CREDENTIALS_STATIC
        /* Seed the build-time credentials. This is the *only* credential
         * source on this target today: EHS_NVS_SUPPORT is 'stubbed', so
         * EhsStartWifiStationThread's NVS read-back yields nothing and there
         * is no serial console here to type an SSID into either. Drop this
         * block once a Zephyr NVS backend lands - saved credentials should
         * win over a compiled-in default.
         * WifiStationSetSSIDPSK also starts the station thread. */
        WifiStationSetSSIDPSK(CONFIG_WIFI_CREDENTIALS_STATIC_SSID,
                              (ehs_uint16)sizeof(CONFIG_WIFI_CREDENTIALS_STATIC_SSID),
                              CONFIG_WIFI_CREDENTIALS_STATIC_PASSWORD,
                              (ehs_uint16)sizeof(CONFIG_WIFI_CREDENTIALS_STATIC_PASSWORD));
#else
        EhsStartWifiStationThread();
#endif
    } else {
        EHSH_LOG_ERROR("Failed to initialise the Wi-Fi interface");
    }
#endif

#ifdef EHS_TEST_FUNC_OVERRIDE
    /* Full-init test mode (make TEST_FUNC=x): everything above has run, but the
     * test function takes the place of EhsInit/EhsMain. It gets the kernel
     * thread's own stack, which is far larger than an EHS dynamic thread's. */
    printf("EHS Test Mode: running %s instead of EhsMain\n",
           EHS_MACRO_STRINGIFY(EHS_TEST_FUNC_NAME));
    fflush(stdout);
    k_thread_create(&ehs_main_thread_data, ehs_main_stack,
                    K_THREAD_STACK_SIZEOF(ehs_main_stack),
                    ehs_test_thread_entry,
                    NULL, NULL, NULL,
                    EHS_MAIN_THREAD_PRIORITY, 0, K_NO_WAIT);
    k_thread_name_set(&ehs_main_thread_data, "ehs_test");
#else
    /* Create and start the EHS kernel thread */
    k_thread_create(&ehs_main_thread_data, ehs_main_stack,
                    K_THREAD_STACK_SIZEOF(ehs_main_stack),
                    ehs_main_thread_entry,
                    NULL, NULL, NULL,
                    EHS_MAIN_THREAD_PRIORITY, 0, K_NO_WAIT);
    k_thread_name_set(&ehs_main_thread_data, "ehs_main");
#endif

    printf("EHS main thread started\n");

    /* Main thread heartbeat loop: output a '.' every second to provide
     * visual life-sign on serial console while the EHS kernel thread runs. */
    int heartbeat_count = 0;
    while (1) {
        k_msleep(1000);
        // printf("M");          /* main-thread heartbeat (was '.') — distinct char confirms new image */
        fflush(stdout);
        heartbeat_count++;
        if (heartbeat_count % 60 == 0) {
            // printf(" [eRT Heartbeat %ds]\n", heartbeat_count);
            fflush(stdout);
#ifdef EHS_NETWORK_WIFI_SUPPORT
            /* Network reachability in one line, unconditionally - NOT via
             * EHSH_LOG_*, which is filtered per module and defaults to
             * ERROR-only, and not via the Zephyr log, which is level-gated
             * too. Whether the board has an IP is the first question asked of
             * every "I can't ping it / Lucid won't connect" report, and it
             * should never require turning diagnostics on to answer. */
            {
                const ehs_char* ip = Wifi_GetIp();

                printf("  NET: wifi=%s ip=%s",
                       Wifi_GetStateName(Wifi_GetState()),
                       (ip != NULL && ip[0] != '\0') ? ip : "<none>");
#ifdef EHS_DEBUG_TCPIP_CONSOLE
                /* The Lucid listener binds INADDR_ANY at kernel init, so it is
                 * up regardless - but it is unreachable until there is an
                 * address, which is the usual reason the tool sees nothing. */
                printf(" lucid=tcp/%u%s", (unsigned)EHS_TGT_TCP_PORTNUM,
                       (ip != NULL && ip[0] != '\0') ? "" : " (unreachable: no IP)");
#endif
                printf("\n");
                fflush(stdout);
            }
#endif /* EHS_NETWORK_WIFI_SUPPORT */
        }
    }

    return 0;
}

#ifdef EHS_TEST_FUNC_OVERRIDE
/**
 * TEST_FUNC thread entry (full-init mode). Adapts the plain void(void) test
 * signature the TEST_FUNC contract specifies onto Zephyr's k_thread_entry_t.
 * MCU test functions are expected never to return; park if one does, so the
 * board stays inspectable rather than silently ending the thread.
 */
static void ehs_test_thread_entry(void *p1, void *p2, void *p3)
{
    extern void EHS_TEST_FUNC_NAME(void);

    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

    EHS_TEST_FUNC_NAME();

    printf("Test completed\n");
    fflush(stdout);
    while (1) {
        k_msleep(1000);
    }
}
#else /* !EHS_TEST_FUNC_OVERRIDE */

/**
 * EHS kernel thread entry point.
 * Calls EhsInit() to initialise the kernel, then enters EhsMain() which
 * runs the eRT main loop and does not return.
 * This mirrors the pattern used by the Linux/GNU targets.
 */
static void ehs_main_thread_entry(void *p1, void *p2, void *p3)
{
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

#ifdef EHS_ZEPHYR_THREAD_SMOKETEST
    /* Smoke test: prove the EHS thread actually starts, runs, and yields —
     * independent of EhsInit()/EhsMain(). Prints a REPEATING 'E' (not a one-shot
     * marker) so it survives the USB-CDC enumeration window; one-shot boot
     * messages are lost before the host opens the port. If you see 'E'
     * interleaved with main's 'M', the thread is fine and any earlier silence
     * was EhsInit stalling and/or lost boot output. Remove EHS_ZEPHYR_THREAD_SMOKETEST
     * (config.mk) to restore the real EhsInit/EhsMain path below. */
    while (1) {
        k_msleep(1000);
        printf("E");
        fflush(stdout);
    }
#else
    /* Let the USB-CDC console (nRF5340 bridge) enumerate on the host AND give
     * time to attach a terminal before the kernel runs (it can fault ~0.5 s in,
     * so a short delay lost the whole sequence). Flush at each stage so nothing
     * is stuck in a buffer when a fault halts the system. */
    k_msleep(6000);

    printf("EHS-THREAD: entry — entering EhsMain (calls EhsInit internally)\n");
    fflush(stdout);
    k_msleep(50);   /* let the UART FIFO drain before the kernel's init/parse/run */

    /* Run the EHS main loop — does not return. EhsMain() calls EhsInit() itself
     * (ehs_main.c) — do NOT call EhsInit() again here: doing so mounted the
     * filesystem and reseeded the default SODL twice in quick succession,
     * racing the parser's read of t.sdl against the second reseed's O_TRUNC
     * write and making a freshly-written file read back as empty/wrong-format.
     * ehs_loop_heartbeat is the per-iteration callback (fires only while the
     * kernel is RUNNING, i.e. an app is loaded), giving an in-loop life-sign
     * without a kernel rebuild. */
    EhsMain(ehs_loop_heartbeat, NULL);
#endif
}

/**
 * Per-scheduler-pass callback passed to EhsMain(). Invoked from
 * EhsKEGroupTable_schedule() once per pass while EhsKEState == RUNNING, so it
 * doubles as a diagnostic: if these markers appear the scheduler is live and an
 * app is running; if only the "entering EhsMain loop" line appears the kernel
 * never reached RUNNING (no app loaded). Rate-limited so it does not flood.
 */
#ifndef EHS_ZEPHYR_THREAD_SMOKETEST
static Ehs_ConsoleCommand_Type ehs_loop_heartbeat(void *env)
{
    ARG_UNUSED(env);
    // static ehs_uint32 pass = 0u;
    // if ((pass++ % EHS_LOOP_HEARTBEAT_EVERY) == 0u) {
    //     printf("<ehs-loop %u>\n", (unsigned)pass);
    //     fflush(stdout);
    // }
    return EHS_CONTINUE;
}
#endif

#endif /* EHS_TEST_FUNC_OVERRIDE */
