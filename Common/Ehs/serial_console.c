/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file serial_console.c
 * @brief Cross-target interactive serial-console implementation.
 *
 * Defines the human-facing command prompt: banner display, key
 * dispatch, line input with echo / password / hidden modes, WiFi
 * configuration helpers, and the periodic state poller that prints
 * authenticated/online/failed transitions.
 *
 * The file is hardware-agnostic. Anything that touches the UART, the
 * scheduler, or stdio buffering is delegated to the per-target HAL
 * declared in Common/HAL/include/hal_serial_console.h (implemented in
 * each target's target/os-arch/<arch>/target_serial.c).
 *
 * The split mirrors banner.c: this file owns the *content and
 * behaviour* of the console; target_serial.c owns the UART quirks
 * (e.g. ESP-IDF's USB-Serial-JTAG partial-line flush behaviour, or
 * the FIFO-drain wait that fflush(stdout) doesn't cover).
 */


#include "hal.h"                  /* EHS metadata, threadnames, EhsTargetReboot */
#include "hal_string.h"
/* No hal_console.h: EhsConsolePrintf is kernel-only. Serial output is stdio. */
#include "hal_network.h"          /* EhsNetworkInterfaceWifiIsEnabled, EthIsEnabled */
#include "hal_serial_console.h"   /* EhsSerial_FlushTx / Write / Init             */
#include "hal_devapps_api.h"      /* EhsAppDenyCurrentApp                         */
#include "hal_appstorage.h"       /* EHS_SYS_APP_DEFAULT_NAME                     */
#include "banner.h"               /* EhsBanner_PrintHelp                          */
#include "serial_console.h"

#ifdef EHS_NETWORK_WIFI_SUPPORT
#include "wifi_station.h"
#include "hal_nvs.h"              /* shared WiFi credential store (forget cmd) */
#endif

#ifdef EHS_LORAWAN_SUPPORT
#include "lorawan.h"              /* LoRaWAN_module_peek* for the 'L' key */
#endif

/*****************************************************************************/
/* Tunables  */

/* Pause after each println so the user can read short messages
 * before the next prompt overwrites them. Was EHS_PROMPT_READ_SLEEP
 * in target_main.c. */
#define EHS_PROMPT_PRINT_SLEEP_MS 100

/* Polling period for character-at-a-time input. Was
 * EHS_PROMPT_CHAR_POLL_MS in target_main.c. */
#define EHS_PROMPT_CHAR_POLL_MS   10

/* Top-level loop tick. Pacing for the WiFi state poller. */
#define EHS_PROMPT_LOOP_SLEEP_MS  50

/*****************************************************************************/
/* Externs into target_main.c  */

/* Set/cleared by the target's network event handlers. Live in
 * target_main.c rather than behind an accessor for historical
 * reasons — see EhsNetworkInterfaceWifiIsEnabled / EthIsEnabled in
 * hal_network.h for the equivalents that *are* function-wrapped. */
extern volatile ehs_bool gNetworkConnected;

/* WiFi-scan deduplication flag — set true while a list-ssid request
 * is in flight so a second 'l' keypress doesn't double-launch the
 * scan. Local to this translation unit; only the 'l' command path
 * touches it. */
static ehs_bool g_cmd_list_ssid_bssid = EHS_FALSE;

/*****************************************************************************/
/* Output helpers  */

void command_prompt_println(const char* text)
{
    printf("%s\n", text);
    EhsSerial_FlushTx();
    /* Brief delay for readability before any subsequent prompt. */
    EhsSleep(EHS_TIME_ms(EHS_PROMPT_PRINT_SLEEP_MS));
}

void command_prompt_print(const char* text)
{
    printf("%s", text);
    EhsSerial_FlushTx();
}

void command_prompt_show_prompt(void)
{
    printf("> ");
    EhsSerial_FlushTx();
}

void command_prompt_echo_command(char cmd)
{
    if (cmd >= 32 && cmd < 127) {
        printf("%c\n", cmd);
    } else if (cmd == '\n' || cmd == '\r') {
        printf("\n");
    }
    /* Other control characters: silent. */
    EhsSerial_FlushTx();
}

/*****************************************************************************/
/* Input helpers  */

ehs_bool command_prompt_read_char(char* ch, command_prompt_echo_mode_t echo_mode)
{
    /* Skip any leftover newlines / carriage returns from a previous read. */
    do {
        fscanf(stdin, "%c", ch);
    } while (*ch == '\n' || *ch == '\r');

    switch (echo_mode) {
        case ECHO_NORMAL:
            printf("%c\n", *ch);
            EhsSerial_FlushTx();
            break;
        case ECHO_PASSWORD:
            printf("*\n");
            EhsSerial_FlushTx();
            break;
        case ECHO_HIDDEN:
            break;
    }
    return EHS_TRUE;
}

ehs_uint32 command_prompt_read_with_echo(char* buffer,
                                         command_prompt_echo_mode_t echo_mode)
{
    ehs_uint32 pos = 0;
    char ch;

    buffer[0] = '\0';

    while (1) {
        if (fscanf(stdin, "%c", &ch) == 1) {
            /* Enter — terminate the line. */
            if (ch == '\n' || ch == '\r') {
                printf("\n");
                EhsSerial_FlushTx();
                buffer[pos] = '\0';
                return pos;
            }
            /* Backspace / DEL — erase one character. */
            else if (ch == '\b' || ch == 127) {
                if (pos > 0) {
                    pos--;
                    buffer[pos] = '\0';
                    if (echo_mode != ECHO_HIDDEN) {
                        EhsSerial_Write("\b \b", 3);
                    }
                }
            }
            /* Ctrl+C — cancel. */
            else if (ch == 3) {
                printf(" ^C\n");
                EhsSerial_FlushTx();
                buffer[0] = '\0';
                return 0;
            }
            /* Ctrl+U — clear the line. */
            else if (ch == 21) {
                while (pos > 0) {
                    pos--;
                    if (echo_mode != ECHO_HIDDEN) {
                        EhsSerial_Write("\b \b", 3);
                    }
                }
                buffer[0] = '\0';
            }
            /* Esc — cancel. */
            else if (ch == 27) {
                printf(" [ESC]\n");
                EhsSerial_FlushTx();
                buffer[0] = '\0';
                return 0;
            }
            /* Printable. */
            else if (ch >= 32 && ch < 127) {
                if (pos < EHS_PROMPT_READ_MAX - 1) {
                    buffer[pos++] = ch;
                    buffer[pos] = '\0';
                    switch (echo_mode) {
                        case ECHO_NORMAL:
                            EhsSerial_Write(&ch, 1);
                            break;
                        case ECHO_PASSWORD:
                            EhsSerial_Write("*", 1);
                            break;
                        case ECHO_HIDDEN:
                            break;
                    }
                } else {
                    /* Buffer full — bell. */
                    EhsSerial_Write("\a", 1);
                }
            }
            /* Other control characters: ignored. */
        }
        EhsSleep(EHS_TIME_ms(EHS_PROMPT_CHAR_POLL_MS));
    }
}

ehs_uint32 command_prompt_read(char* buffer)
{
    return command_prompt_read_with_echo(buffer, ECHO_NORMAL);
}

ehs_uint32 command_prompt_read_password(char* buffer)
{
    return command_prompt_read_with_echo(buffer, ECHO_PASSWORD);
}

ehs_bool command_prompt_ask_yes_no(const char* question)
{
    char ch = 0;

    command_prompt_println(question);

    while (1) {
        if (fscanf(stdin, "%c", &ch) == 1) {
            if (ch == '\n' || ch == '\r' || ch == ' ' || ch == '\t') {
                continue;
            }
            if (ch == 'y' || ch == 'Y') {
                printf("%c\n", ch);
                EhsSerial_FlushTx();
                return EHS_TRUE;
            }
            else if (ch == 'n' || ch == 'N') {
                printf("%c\n", ch);
                EhsSerial_FlushTx();
                return EHS_FALSE;
            }
            else {
                printf("%c - please enter 'y' or 'n': ", ch);
                EhsSerial_FlushTx();
            }
        }
        EhsSleep(EHS_TIME_ms(EHS_PROMPT_CHAR_POLL_MS));
    }
}

/*****************************************************************************/
/* Help / Reboot / IP  */

void command_prompt_help(void)
{
    /* Banner + tagline + key-command help all live in the common
     * Common/Ehs/banner.c so the layout / colours / DevEUI line are
     * identical across every target with a serial console. */
    EhsBanner_PrintHelp(command_prompt_println);
}

void command_prompt_reboot(void)
{
    command_prompt_println("rebooting...");
    EhsTargetReboot();
}

void command_prompt_ip_addr(void)
{
    const ehs_char* ip = NULL;
#ifdef EHS_NETWORK_WIFI_SUPPORT
    if (EhsNetworkInterfaceWifiIsEnabled()) {
        ip = isWifiStationConnected() ? WifiStationIpAddress() : NULL;
        printf("WiFi ");
    }
#endif
#ifdef EHS_NETWORK_ETHERNET_SUPPORT
    if (EhsNetworkInterfaceEthIsEnabled() && ip == NULL) {
        ip = gNetworkConnected ? EhsHMetaGetIPAddr() : NULL;
        printf("Ethernet ");
    }
#endif
    printf("IP: %s\n", (ip && EhsStrlen(ip) > 0) ? ip : "N/A");
}

void command_prompt_delete_current_app(void)
{
    const ehs_char* cur = EhsHMetaAppGetCurrent();
    if (cur == NULL || cur[0] == '\0') {
        command_prompt_println("No app currently selected.");
        return;
    }
    if (EhsStrcmp(cur, EHS_SYS_APP_DEFAULT_NAME) == 0) {
        command_prompt_println("Refusing to delete the default app.");
        return;
    }

    char line[96];
    snprintf(line, sizeof(line), "Delete currently-selected app '%s'? (y/n)", cur);
    if (command_prompt_ask_yes_no(line) == EHS_FALSE) {
        command_prompt_println("Cancelled.");
        return;
    }

    /* EhsAppDenyCurrentApp() removes the live dir; if a _prev exists it is
     * reinstated (returns TRUE) and that becomes the new current app, else
     * metadata is rolled to the default (returns FALSE). */
    if (EhsAppDenyCurrentApp() == EHS_TRUE) {
        command_prompt_println("App deleted; previous version reinstated. Reboot to run it.");
    } else {
        command_prompt_println("App deleted; default app will run on next boot.");
    }
}

/*****************************************************************************/
/* WiFi command handlers  */

#ifdef EHS_NETWORK_WIFI_SUPPORT

/* Both the console and an app WiFi function block may drive the shared
 * station state machine; they read/write the same NVS credentials.  Warn
 * (but don't block) when a block is active, since the app may override
 * whatever the operator sets here. */
static void command_prompt_wifi_managed_notice(void)
{
    if (isEhsWiFiManagedByComponent() == EHS_TRUE)
        command_prompt_println("Note: a WiFi function block is active; the app may override these settings.");
}

/* Gate for the WiFi commands: tell the operator when the WiFi interface is
 * disabled (e.g. Ethernet-only configuration) instead of silently ignoring
 * the key. */
static ehs_bool command_prompt_wifi_enabled_check(void)
{
    if (EhsNetworkInterfaceWifiIsEnabled()) return EHS_TRUE;
    command_prompt_println("WiFi is not enabled on this device.");
    return EHS_FALSE;
}

void command_prompt_wifi_conf(void)
{
    command_prompt_wifi_managed_notice();
    char ssid[EHS_PROMPT_READ_MAX] = {0};
    char pass[EHS_PROMPT_READ_MAX] = {0};
    ehs_bool yes = EHS_FALSE;
    ehs_uint32 ssid_len, pass_len;

    command_prompt_println("**** WiFi config ****");
    command_prompt_println("Enter SSID:");
    ssid_len = command_prompt_read(ssid);
    if (ssid_len == 0) {
        command_prompt_println("Cancelled.");
        return;
    }

    command_prompt_println("Enter Password:");
    pass_len = command_prompt_read_password(pass);
    if (pass_len == 0) {
        command_prompt_println("Cancelled.");
        return;
    }

    /* Confirmation (password masked) */
    printf("SSID: %s\n", ssid);
    printf("Password: ");
    for (ehs_uint32 i = 0; i < pass_len; i++) {
        printf("*");
    }
    printf(" (%d chars)\n", (int)pass_len);
    EhsSerial_FlushTx();

    yes = command_prompt_ask_yes_no("Are these correct? (y/n)");
    if (yes == EHS_TRUE) {
        /* Wifi_Connect persists the credentials to NVS itself. */
        command_prompt_println("Saving WiFi credentials...");
        command_prompt_println("Connecting to WiFi, please wait...");
        Wifi_Connect(ssid, pass);
    } else {
        command_prompt_println("WiFi NOT configured. Type 'w' to try again.");
    }
}

void command_prompt_wifi_reconnect(void)
{
    command_prompt_wifi_managed_notice();
    /* Reject reconnect when there are no credentials to reconnect with —
     * otherwise we'd kick off a scan loop against an empty SSID and the
     * user gets no useful feedback. */
    ehs_char current_ssid[EHS_STRING_LENGTH_MAX] = {0};
    WifiStationGetCurrentSsid(current_ssid);
    if (current_ssid[0] == '\0') {
        command_prompt_println("No WiFi credentials saved. Type 'w' to configure SSID and password first.");
        return;
    }
    printf("WiFi: reconnecting to '%s'\n", current_ssid);
    setWifiStationConnectState(WifiStationConnectState_CONNECT);
    EhsWifiStationSetCBSource(eWifiStationCallbackSource_Connect);
    EhsStartWifiStationThread();
}

void command_prompt_wifi_ssid(void)
{
    char ssid[EHS_PROMPT_READ_MAX] = {0};
    WifiStationGetCurrentSsid(ssid);
    ehs_bool connected = isWifiStationConnected();
    printf("SSID: %s  (%s)\n", ssid, connected ? "connected" : "not connected");
}

void command_prompt_wifi_disconnect(void)
{
    command_prompt_wifi_managed_notice();
    if (isWifiStationConnected() == EHS_FALSE) {
        command_prompt_println("WiFi is not connected.");
        return;
    }
    command_prompt_println("Disconnecting from WiFi...");
    doWifiStationDisconnect();
    command_prompt_println("WiFi disconnected.");
}

void command_prompt_wifi_forget(void)
{
    command_prompt_wifi_managed_notice();
    ehs_bool yes = command_prompt_ask_yes_no("Are you sure you want to forget saved WiFi credentials? (y/n)");
    if (yes == EHS_TRUE) {
        if (isWifiStationConnected() == EHS_TRUE) {
            command_prompt_println("Disconnecting from WiFi...");
            doWifiStationDisconnect();
        }
        /* Clear the shared NVS credentials — the store both the console and
         * the WiFi function block read at boot. */
        {
            ehs_nvs_obj_t nvs;
            EhsNvsOpen(&nvs, EHS_NVS_WIFI_NAMESPACE);
            EhsNvsSetString(&nvs, EHS_NVS_WIFI_KEY_SSID, "", 0);
            EhsNvsSetString(&nvs, EHS_NVS_WIFI_KEY_PASS, "", 0);
            EhsNvsCommit(&nvs);
            EhsNvsClose(&nvs);
        }
        command_prompt_println("WiFi credentials forgotten.");
    } else {
        command_prompt_println("Credentials NOT forgotten.");
    }
}

void command_prompt_list_ssid_bssid(void)
{
    if (isWifiStationInitalised() == EHS_FALSE) {
        command_prompt_println("WiFi is not initalised yet. You can connect to a dummy SSID first.");
        return;
    }
    if (isWifiStationConnecting() == EHS_TRUE) {
        command_prompt_println("Wi-Fi Connection in process. Please try again later.");
        return;
    }
    if (g_cmd_list_ssid_bssid) {
        printf("List SSID in process...\n");
        return;
    }
    g_cmd_list_ssid_bssid = EHS_TRUE;
    EhsSleep(EHS_TIME_ms(EHS_PROMPT_PRINT_SLEEP_MS));

    /* If we're online, show the current AP's signal strength. */
    ehs_sint8 rssi = 0;
    if (Wifi_GetRssi(&rssi) == EHS_TRUE) {
        printf("Current connected AP RSSI: %d dBm\n", (int)rssi);
        EhsSerial_FlushTx();
    }

    /* Wifi_Scan() uses the separated user-scan buffer, so results
     * won't be clobbered by any in-flight connect-SM scan. */
    if (Wifi_Scan() == EHS_TRUE) {
        command_prompt_println("Scanning...");
    } else {
        /* Fall-back: maybe a scan is already running; request print
         * on done. */
        EhsWifiStationSetPrintOnScanDone(EHS_TRUE);
        command_prompt_println("Scan request rejected (likely a scan already in progress) — will print when it finishes.");
    }
    g_cmd_list_ssid_bssid = EHS_FALSE;
}

void command_prompt_wifi_stop_scan(void)
{
    /* Call doWifiStationScanStop() directly rather than checking
     * isWifiStationScanning() first: the scan-done event can clear
     * the flag between a separate check and the actual stop call,
     * causing a spurious error.  doWifiStationScanStop() handles
     * that race internally and returns EHS_TRUE for both "stopped"
     * and "had already finished naturally". */
    if (doWifiStationScanStop() == EHS_TRUE) {
        command_prompt_println("WiFi scan stopped.");
    } else {
        command_prompt_println("Failed to stop WiFi scan.");
    }
}

#endif /* EHS_NETWORK_WIFI_SUPPORT */

/*****************************************************************************/
/* LoRaWAN command handler  */

#ifdef EHS_LORAWAN_SUPPORT
void command_prompt_lorawan_info(void)
{
    ehs_uint8 dev_eui[8] = {0};
    LoRaWAN_module_peekDevEui(dev_eui);
    char line[96];
    if (LoRaWAN_module_peekJoined() == EHS_TRUE) {
        snprintf(line, sizeof(line),
                 "LoRaWAN: DevEUI=%02X%02X%02X%02X%02X%02X%02X%02X joined DevAddr=%08lX",
                 dev_eui[0], dev_eui[1], dev_eui[2], dev_eui[3],
                 dev_eui[4], dev_eui[5], dev_eui[6], dev_eui[7],
                 (unsigned long)LoRaWAN_module_peekDevAddr());
    } else {
        snprintf(line, sizeof(line),
                 "LoRaWAN: DevEUI=%02X%02X%02X%02X%02X%02X%02X%02X not joined",
                 dev_eui[0], dev_eui[1], dev_eui[2], dev_eui[3],
                 dev_eui[4], dev_eui[5], dev_eui[6], dev_eui[7]);
    }
    command_prompt_println(line);
}
#endif

/*****************************************************************************/
/* Top-level interactive task
 *
 * Priority : EHS_PRI_SERIAL_CMD (lowest) — never blocks higher work
 * Stack    : sized by the target main when xTaskCreate-ing this
 * Period   : EHS_PROMPT_LOOP_SLEEP_MS (~50 ms) polling sleep
 *
 * Also drives the periodic WiFi state-change announcer
 * (Authenticated / Online / Failed) and the deferred scan-result /
 * connect-timeout printers, so they can't overflow shorter-stacked
 * tasks like sys_evt or MCU_SLOW_LP_THR.
 */
void command_prompt_task(void* params)
{
    (void)params;
    ehs_threadname_t threadname = EHSTHREADNAME_EHS_CONSOLE_THR;

    /* Disable stdio buffering so single-character writes (echo) reach
     * the UART immediately. */
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin,  NULL, _IONBF, 0);

    /* One-shot per-target hardware init (UART pins, driver install,
     * USB CDC attach, …). No-op on targets where stdio is pre-wired. */
    (void)EhsSerial_Init();

    EhsBanner_PrintHelp(command_prompt_println);
    if (EhsNetworkInterfaceWifiIsEnabled() == EHS_TRUE) {
        command_prompt_println("Type 'w' to configure WiFi or 'h' for help.");
    } else {
        command_prompt_println("Type 'h' for help.");
    }

    char command;
    ehs_bool show_prompt = EHS_TRUE;

    while (1) {
        EhsHStatisticsLoopStart(threadname);

        if (show_prompt) {
            /* command_prompt_show_prompt(); */
            show_prompt = EHS_FALSE;
        }

        if (fscanf(stdin, "%c", &command) == 1) {
            /* Skip whitespace; just newline-and-reprompt on Enter. */
            if (command == '\n' || command == '\r' ||
                command == ' '  || command == '\t') {
                if (command == '\n' || command == '\r') {
                    printf("\n");
                    show_prompt = EHS_TRUE;
                }
                EhsHStatisticsLoopEnd(threadname);
                EhsSleep(EHS_TIME_ms(EHS_PROMPT_CHAR_POLL_MS));
                continue;
            }

            command_prompt_echo_command(command);
            show_prompt = EHS_TRUE;

            if (command == 'h') {
                command_prompt_help();
            }
#ifdef EHS_NETWORK_WIFI_SUPPORT
            else if (command == 'w') {
                if (command_prompt_wifi_enabled_check())
                    command_prompt_wifi_conf();
            }
            else if (command == 'c') {
                if (command_prompt_wifi_enabled_check())
                    command_prompt_wifi_reconnect();
            }
            else if (command == 'd') {
                if (command_prompt_wifi_enabled_check())
                    command_prompt_wifi_disconnect();
            }
            else if (command == 'f') {
                if (command_prompt_wifi_enabled_check())
                    command_prompt_wifi_forget();
            }
            else if (command == 's') {
                if (command_prompt_wifi_enabled_check())
                    command_prompt_wifi_ssid();
            }
            else if (command == 'l') {
                if (command_prompt_wifi_enabled_check())
                    command_prompt_list_ssid_bssid();
            }
            else if (command == 'x') {
                if (command_prompt_wifi_enabled_check())
                    command_prompt_wifi_stop_scan();
            }
#endif
            else if (command == 'r') {
                command_prompt_reboot();
            }
            else if (command == 'i') {
                command_prompt_ip_addr();
            }
            else if (command == 'D') {
                command_prompt_delete_current_app();
            }
#ifdef EHS_LORAWAN_SUPPORT
            else if (command == 'L') {
                command_prompt_lorawan_info();
            }
#endif
            else if (command == '?') {
                command_prompt_help();
            }
            else {
                printf("Unknown command '%c'. Type 'h' for help.\n", command);
            }
        }

#ifdef EHS_NETWORK_WIFI_SUPPORT
        {
            /* State-poller only announces transitions the SM doesn't
             * already cover with a more informative message. The SM
             * prints
             *   - "'SSID' found on ch N (R dBm) — associating"
             *   - "disconnected, reason=N (text) — retrying"
             * so announcing "WiFi: Associating" or "WiFi: Reconnecting"
             * here would be redundant. We still print states the SM
             * is silent on: Authenticated (DHCP pending), Online
             * (final) and Failed (terminal). */
            static EhsWifiState _prev_wifi_state = EhsWifi_Idle;
            EhsWifiState _now_state = Wifi_GetState();
            if (_now_state != _prev_wifi_state) {
                switch (_now_state) {
                    case EhsWifi_Authenticated:
                        printf("WiFi: Authenticated - waiting for IP\n");
                        break;
                    case EhsWifi_Online:
                        printf("WiFi: Online - IP %s\n", Wifi_GetIp());
                        break;
                    case EhsWifi_Failed:
                        printf("WiFi: Failed\n");
                        break;
                    default:
                        break;
                }
            }
            _prev_wifi_state = _now_state;
        }
        if (WifiStationIsConnectTimedOut())
            printf("WiFi: connection attempt timed out.\n");
        if (WifiStationIsScanResultReady())
            WifiStationPrintAndClearScanResults();
#endif
        EhsSerial_FlushTx();
        EhsHStatisticsLoopEnd(threadname);
        EhsSleep(EHS_TIME_ms(EHS_PROMPT_LOOP_SLEEP_MS));
    }
    /* Unreachable in current design; kept for explicit teardown.
     * vTaskDelete-equivalent would belong in target_serial.c if ever
     * needed. */
}
