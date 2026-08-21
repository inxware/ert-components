/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license.
 ***************************************************************/

/**
 * @file banner.c
 * @brief Common (cross-target) inxware banner + key-command help printer.
 *
 * Used by per-target serial-console implementations (e.g. command_prompt_task
 * on esp32s3_freertos-xtensa). Outputs ASCII art, tagline and command list
 * via a target-supplied println callback so this file stays free of any
 * target-specific I/O dependency.
 */

#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "banner.h"

#ifdef EHS_LORAWAN_SUPPORT
#include "lorawan.h"   /* LoRaWAN_module_peekDevEui */
#endif

/* ANSI SGR colour codes. Printed inline; if the host terminal is dumb the
 * sequences just appear as literal text. Future work could gate emission
 * on a target #define (EHS_BANNER_NO_ANSI) — not needed today. */
#define ANSI_RESET   "\x1b[0m"
#define ANSI_PINK    "\x1b[95m"   /* bright magenta — letter "i"     */
#define ANSI_GREEN   "\x1b[32m"   /* letter "n"                       */
#define ANSI_PURPLE  "\x1b[35m"   /* letter "x"                       */
#define ANSI_WHITE   "\x1b[97m"   /* bright white — "ware"            */

void EhsBanner_PrintHelp(ehs_console_println_fn println)
{
    if (println == NULL) return;
#if EHS_START_BANNER__ASCIIART
    /* 8-row block-letter "inxware". Letter widths (cols): i=5, n=8, x=8,
     * w=11, a=8, r=7, e=8 — adjust all 8 rows together if changed. */
    println(ANSI_PINK   " ###  "
            ANSI_GREEN  "         "
            ANSI_PURPLE "         "
            ANSI_WHITE  "                                                 " ANSI_RESET);
    println(ANSI_PINK   "      "
            ANSI_GREEN  "######## "
            ANSI_PURPLE "##    ## "
            ANSI_WHITE  "##         ##   ######    ######    ######  " ANSI_RESET);
    println(ANSI_PINK   " ###  "
            ANSI_GREEN  "##    ## "
            ANSI_PURPLE " ##  ##  "
            ANSI_WHITE  "##         ##  ##    ##  ##    ##  ##    ## " ANSI_RESET);
    println(ANSI_PINK   " ###  "
            ANSI_GREEN  "##    ## "
            ANSI_PURPLE "  ####   "
            ANSI_WHITE  "##  ###  ##  ########  ########   ########  " ANSI_RESET);
    println(ANSI_PINK   " ###  "
            ANSI_GREEN  "##    ## "
            ANSI_PURPLE " ##  ##  "
            ANSI_WHITE  "## ## ## ##  ##    ##  ##  ##     ##        " ANSI_RESET);
    println(ANSI_PINK   " ###  "
            ANSI_GREEN  "##    ## "
            ANSI_PURPLE "##    ## "
            ANSI_WHITE  "###    ###   ##    ##  ##   ##     ######   " ANSI_RESET);

    println("  ...integrate at the speed of thought...");
#else
    println("===== starting inxware =====");
#endif
    println("");

    /* Key-command help. Hand-maintained against the per-target serial
     * console dispatcher (currently command_prompt_task in
     * target/os-arch/esp32s3_freertos-xtensa/target_main.c). When adding
     * a new serial-console command, update both. */
    println("Commands:");
    println("  h/? - this help");
    println("  i   - get IP address");
    println("  r   - reboot device");
    println("  D   - delete currently-selected app (with y/n confirm)");
#ifdef EHS_NETWORK_WIFI_SUPPORT
    println("  w   - WiFi config");
    println("  c   - reconnect WiFi");
    println("  d   - disconnect WiFi");
    println("  f   - forget credentials");
    println("  s   - get WiFi SSID");
    println("  l   - list SSIDs");
    println("  x   - stop WiFi scan");
#endif
#ifdef EHS_LORAWAN_SUPPORT
    println("  L   - LoRaWAN info (DevEUI / join state / DevAddr)");
    {
        ehs_uint8 dev_eui[8] = {0};
        char line[96];
        LoRaWAN_module_peekDevEui(dev_eui);
        snprintf(line, sizeof(line),
                 "LoRaWAN DevEUI: %02X%02X%02X%02X%02X%02X%02X%02X",
                 dev_eui[0], dev_eui[1], dev_eui[2], dev_eui[3],
                 dev_eui[4], dev_eui[5], dev_eui[6], dev_eui[7]);
        println(line);
    }
#endif
}
