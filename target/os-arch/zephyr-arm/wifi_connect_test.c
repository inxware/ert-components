/***************************************************************
 * Copyright (C) 2026 inx limited, UK - All Rights Reserved
 * LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
 ***************************************************************/

/** @file wifi_connect_test.c
 * Minimal Wi-Fi station connect + DHCP bring-up test. Connects using the
 * statically-configured credentials (CONFIG_WIFI_CREDENTIALS_STATIC_SSID/
 * PASSWORD - see the platform config.mk) and prints connect/disconnect/DHCP
 * events to the console. Not an application-level Wi-Fi HAL - just answers
 * "does this board associate and get an IP" before any real FB/HAL work.
 *
 * API pattern (net_mgmt callbacks, NET_REQUEST_WIFI_CONNECT_STORED, the
 * wifi_ready semaphore + dedicated thread) copied from Nordic's own
 * nrf/samples/wifi/sta/src/main.c, trimmed to a one-shot connect (no
 * reconnect-on-disconnect loop).
 *
 * The dedicated thread is not stylistic - it's required. Nordic's wifi_ready
 * library dispatches its ready callback via k_work_submit() (the SYSTEM
 * workqueue), so wifi_ready_cb() itself must stay tiny (just a semaphore
 * give); calling net_mgmt(NET_REQUEST_WIFI_CONNECT_STORED, ...) directly from
 * there runs the whole wifi_mgmt_ext -> supplicant CLI call chain on the
 * system workqueue's small stack and overflows it (confirmed: "Stack
 * overflow on CPU 0" right after "Adding statically configured WiFi network"
 * when this was first wired up via a k_work_delayable instead).
 *
 * @author: inx limited
 */

#include <zephyr/kernel.h>
#include <stdio.h>

#include <zephyr/net/net_if.h>
#include <zephyr/net/net_event.h>
#include <zephyr/net/net_config.h>
#include <zephyr/net/wifi_mgmt.h>
#include <net/wifi_mgmt_ext.h>

#ifdef CONFIG_WIFI_READY_LIB
#include <net/wifi_ready.h>
#endif

#define WIFI_TEST_THREAD_STACK_SIZE 5200

static struct net_mgmt_event_callback wifi_cb;
static struct net_mgmt_event_callback dhcp_cb;

/* CONFIG_NRF_WIFI_IF_AUTO_START=n (see wifi_nrf70.mk) - nothing brings the
 * interface up automatically, so this must run, and must run BEFORE waiting
 * on wifi_ready below: wifi_ready only fires once the RPU/supplicant bring-up
 * that net_if_up() triggers has completed, so waiting for ready before
 * calling this deadlocks forever (found the hard way - no console output at
 * all past the SODL heartbeat, even across a reboot with the monitor already
 * attached, ruling out the usual boot-race explanation). */
static void bring_iface_up(struct net_if *iface)
{
    printf("WIFI-TEST: SSID '%s'\n", CONFIG_WIFI_CREDENTIALS_STATIC_SSID);

    if (net_if_is_up(iface)) {
        printf("WIFI-TEST: iface already up\n");
    } else {
        int ret = net_if_up(iface);

        printf("WIFI-TEST: net_if_up() -> %d\n", ret);
    }
    fflush(stdout);
}

static void wifi_connect_stored(struct net_if *iface)
{
    printf("WIFI-TEST: requesting connect (stored credentials)\n");
    fflush(stdout);
    if (net_mgmt(NET_REQUEST_WIFI_CONNECT_STORED, iface, NULL, 0)) {
        printf("WIFI-TEST: connect request failed\n");
        fflush(stdout);
    }
}

static void wifi_event_handler(struct net_mgmt_event_callback *cb,
                                uint32_t mgmt_event, struct net_if *iface)
{
    ARG_UNUSED(iface);

    if (mgmt_event == NET_EVENT_WIFI_CONNECT_RESULT) {
        const struct wifi_status *status = (const struct wifi_status *)cb->info;

        if (status->status) {
            printf("WIFI-TEST: connect FAILED (%d)\n", status->status);
        } else {
            printf("WIFI-TEST: connected\n");
        }
        fflush(stdout);
    } else if (mgmt_event == NET_EVENT_WIFI_DISCONNECT_RESULT) {
        printf("WIFI-TEST: disconnected\n");
        fflush(stdout);
    }
}

static void dhcp_event_handler(struct net_mgmt_event_callback *cb,
                                uint32_t mgmt_event, struct net_if *iface)
{
    ARG_UNUSED(iface);

    if (mgmt_event == NET_EVENT_IPV4_DHCP_BOUND) {
        const struct net_if_dhcpv4 *dhcpv4 = cb->info;
        char addr_str[128];

        net_addr_ntop(AF_INET, &dhcpv4->requested_ip, addr_str, sizeof(addr_str));
        printf("WIFI-TEST: DHCP bound, IP address: %s\n", addr_str);
        fflush(stdout);
    }
}

#ifdef CONFIG_WIFI_READY_LIB
static K_SEM_DEFINE(wifi_ready_sem, 0, 1);

/* Called on the SYSTEM WORKQUEUE (see file header) - must stay tiny. */
static void wifi_ready_cb(bool wifi_ready)
{
    if (wifi_ready) {
        k_sem_give(&wifi_ready_sem);
    }
}

static void register_wifi_ready(struct net_if *iface)
{
    wifi_ready_callback_t cb = { .wifi_ready_cb = wifi_ready_cb };

    if (register_wifi_ready_callback(cb, iface)) {
        printf("WIFI-TEST: failed to register wifi_ready callback\n");
        fflush(stdout);
    }
}
#endif /* CONFIG_WIFI_READY_LIB */

/* Dedicated thread, not the system workqueue - see file header. Also absorbs
 * the ~6 s USB-CDC enumeration race (main()'s own boot-banner printf is
 * reliably lost before the host's serial monitor attaches - same reasoning
 * as the EHS thread's own startup delay in target_main.c) so the SSID/
 * connect/DHCP prints below land after the monitor is actually attached. */
static void wifi_test_thread_entry(void *p1, void *p2, void *p3)
{
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

    k_msleep(6000);

    struct net_if *iface = net_if_get_first_wifi();

    if (!iface) {
        printf("WIFI-TEST: no Wi-Fi interface found\n");
        fflush(stdout);
        return;
    }

    net_mgmt_init_event_callback(&wifi_cb, wifi_event_handler,
                                  NET_EVENT_WIFI_CONNECT_RESULT | NET_EVENT_WIFI_DISCONNECT_RESULT);
    net_mgmt_add_event_callback(&wifi_cb);

    net_mgmt_init_event_callback(&dhcp_cb, dhcp_event_handler, NET_EVENT_IPV4_DHCP_BOUND);
    net_mgmt_add_event_callback(&dhcp_cb);

#ifdef CONFIG_WIFI_READY_LIB
    register_wifi_ready(iface);
#endif

    bring_iface_up(iface);

#ifdef CONFIG_WIFI_READY_LIB
    k_sem_take(&wifi_ready_sem, K_FOREVER);
#endif
    wifi_connect_stored(iface);
}

K_THREAD_STACK_DEFINE(wifi_test_stack, WIFI_TEST_THREAD_STACK_SIZE);
static struct k_thread wifi_test_thread_data;

void ehs_wifi_connect_test_start(void)
{
    k_thread_create(&wifi_test_thread_data, wifi_test_stack,
                    K_THREAD_STACK_SIZEOF(wifi_test_stack),
                    wifi_test_thread_entry,
                    NULL, NULL, NULL,
                    K_PRIO_PREEMPT(7), 0, K_NO_WAIT);
    k_thread_name_set(&wifi_test_thread_data, "wifi_test");
}
