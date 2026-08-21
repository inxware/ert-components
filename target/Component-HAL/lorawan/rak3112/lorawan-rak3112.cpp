/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license.
 ***************************************************************/

/**
 * @file lorawan-rak3112.cpp
 * @brief RAK3112 (ESP32-S3 + SX1262 on SPI) LoRaWAN module HAL.
 *
 * Wraps SX126x-Arduino (beegee-tokyo) — the Semtech reference LoRaMac-node
 * stack with an Arduino-flavoured API. `lora_rak3112_init()` in that library
 * hard-codes the RAK3112 pin map (GPIO5/3/6 SPI, GPIO7 NSS, GPIO8 RST,
 * GPIO47 DIO1, GPIO48 BUSY, GPIO4 RXEN, USE_DIO2_ANT_SWITCH + USE_DIO3_TCXO).
 *
 * The module keeps state between init and connect because lmh_init() needs
 * the region + class up front (unlike WIO-E5 AT mode, which just fires off
 * commands in sequence). The public entry points match the generic
 * LoRaWAN_module_* API declared in lorawan_module.h.
 */

/* TODO:
 *  1. rename LoRaWAN_module_* to LoRaWAN_rak3112_* functions.
 *  2. rename "lorawan_module.h"to "lorawan_rak3112.h".
 */
#include "SX126x-Arduino.h"
#include "LoRaWan-Arduino.h"

extern "C" {
#include "globals.h"
#include "lorawan_module.h"
#include "lorawan.h"           /* gEhsLoraApiData — populated from cb_rx() */
#include "esp_mac.h"           /* esp_efuse_mac_get_default() — seed DevEUI  */
}

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/* Compile with -DRAK3112_TRACE on the command line */
//#define RAK3112_TRACE
#ifdef RAK3112_TRACE
#  define RAK3112_LOG(fmt, ...) printf("[RAK3112] " fmt "\n", ##__VA_ARGS__)
#else
#  define RAK3112_LOG(fmt, ...) do {} while (0)
#endif

#define RAK3112_LW_APP_PORT_DEFAULT 2
#define RAK3112_MAX_PAYLOAD        242

static bool     g_radio_inited = false;  /* lora_rak3112_init() done */
static bool     g_lmh_inited   = false;  /* lmh_init() called at least once — sticky for firmware lifetime */
static bool     g_joined       = false;  /* MAC is joined / has a session */

static uint8_t  g_dev_eui[8]   = {0};
static uint8_t  g_app_eui[8]   = {0};
static uint8_t  g_app_key[16]  = {0};
static uint8_t  g_nwk_skey[16] = {0};
static uint8_t  g_app_skey[16] = {0};
static uint32_t g_dev_addr     = 0;

static uint8_t  g_tx_buffer[RAK3112_MAX_PAYLOAD];

/* Convert a hex ASCII string of 2*byte_count chars into a raw byte array.
 * Returns true on success, false if any non-hex char is seen. */
static bool hex_to_bytes(const char *src, uint8_t *dst, size_t byte_count)
{
    RAK3112_LOG("hex_to_bytes: byte_count=%zu", byte_count);
    if (src == NULL || dst == NULL) return false;
    for (size_t i = 0; i < byte_count; ++i) {
        char hi = src[2*i], lo = src[2*i + 1];
        if (hi == '\0' || lo == '\0') return false;
        uint8_t h = (hi >= '0' && hi <= '9') ? hi - '0'
                  : (hi >= 'a' && hi <= 'f') ? hi - 'a' + 10
                  : (hi >= 'A' && hi <= 'F') ? hi - 'A' + 10 : 0xFF;
        uint8_t l = (lo >= '0' && lo <= '9') ? lo - '0'
                  : (lo >= 'a' && lo <= 'f') ? lo - 'a' + 10
                  : (lo >= 'A' && lo <= 'F') ? lo - 'A' + 10 : 0xFF;
        if (h == 0xFF || l == 0xFF) return false;
        dst[i] = (uint8_t)((h << 4) | l);
    }
    return true;
}

static void bytes_to_hex(const uint8_t *src, char *dst, size_t byte_count)
{
    RAK3112_LOG("bytes_to_hex: byte_count=%zu", byte_count);
    static const char hx[] = "0123456789ABCDEF";
    for (size_t i = 0; i < byte_count; ++i) {
        dst[2*i]     = hx[(src[i] >> 4) & 0x0F];
        dst[2*i + 1] = hx[ src[i]       & 0x0F];
    }
    dst[2*byte_count] = '\0';
}

static LoRaMacRegion_t map_region(e_ehs_lw_region_t r)
{
    RAK3112_LOG("map_region: r=%d", (int)r);
    switch (r) {
        case E_LWREGION_EU868: return LORAMAC_REGION_EU868;
        case E_LWREGION_US915: return LORAMAC_REGION_US915;
        case E_LWREGION_CN779: return LORAMAC_REGION_CN779;
        case E_LWREGION_EU433: return LORAMAC_REGION_EU433;
        case E_LWREGION_AU915: return LORAMAC_REGION_AU915;
        case E_LWREGION_CN470: return LORAMAC_REGION_CN470;
        case E_LWREGION_AS923: return LORAMAC_REGION_AS923;
        case E_LWREGION_KR920: return LORAMAC_REGION_KR920;
        case E_LWREGION_IN865: return LORAMAC_REGION_IN865;
        case E_LWREGION_RU864: return LORAMAC_REGION_RU864;
        default:               return LORAMAC_REGION_EU868;
    }
}

static DeviceClass_t map_class(e_ehs_lw_class_t c)
{
    RAK3112_LOG("map_class: c=%d", (int)c);
    switch (c) {
        case E_LWCLASS_A: return CLASS_A;
        case E_LWCLASS_B: return CLASS_B;
        case E_LWCLASS_C: return CLASS_C;
        default:          return CLASS_A;
    }
}

/* ----- SX126x-Arduino callbacks ---------------------------------------- */

static void cb_rx(lmh_app_data_t *app_data)
{
    RAK3112_LOG("cb_rx: buffsize=%u port=%u rssi=%d snr=%u",
                app_data ? app_data->buffsize : 0,
                app_data ? app_data->port     : 0,
                app_data ? app_data->rssi      : 0,
                app_data ? app_data->snr       : 0);
    if (!app_data || !app_data->buffer || app_data->buffsize == 0) {
        Common_LoRaWAN_onReceive((char *)"", EHS_FALSE);
        return;
    }
    /* Propagate signal quality / port to the upper layer before the FB's
     * on_receive_msg run-function reads them. sx126x-arduino's app_data only
     * carries RSSI (int16), SNR (uint8) and port; rxwin is not exposed here --
     * leave whatever the previous TX confirm set and mark it TODO below. */
    gEhsLoraApiData.rssi  = (ehs_sint32)app_data->rssi;
    gEhsLoraApiData.snr   = (ehs_float)app_data->snr;
    gEhsLoraApiData.fport = app_data->port;
    /* @todo gEhsLoraApiData.rxwin -- sx126x-arduino doesn't surface which
     * RX window the packet arrived on via lmh_app_data_t. Would need a
     * deeper hook into LoRaMac's McpsIndication (see mac/LoRaMac.c). */

    /* Deliver as hex ASCII (2x length + null). Allocated on stack bounded by
     * LORAWAN_APP_DATA_MAX_SIZE=242 -> 485 chars incl. terminator. */
    static char rx_hex[RAK3112_MAX_PAYLOAD * 2 + 1];
    size_t n = app_data->buffsize > RAK3112_MAX_PAYLOAD ? RAK3112_MAX_PAYLOAD : app_data->buffsize;
    bytes_to_hex(app_data->buffer, rx_hex, n);
    Common_LoRaWAN_onReceive(rx_hex, EHS_TRUE);
}

static void cb_has_joined(void)
{
    RAK3112_LOG("cb_has_joined");
    g_joined = true;
    g_dev_addr = lmh_getDevAddr();
}

static void cb_join_failed(void)
{
    RAK3112_LOG("cb_join_failed");
    g_joined = false;
}

/* @todo propagate class-change confirmation to the upper layer if the FB
 * ever grows a class-change completed event. sx126x-arduino invokes this
 * after the MAC has actually switched. Silent is harmless for now. */
static void cb_confirm_class(DeviceClass_t /*c*/)
{
    RAK3112_LOG("cb_confirm_class");
}

/* @todo real battery reporting. RAK3112 has a battery-sense ADC that can
 * read VBAT via a voltage divider (channel / divider ratio depends on the
 * carrier board). Needs an ADC-HAL call and conversion to the LoRaMac
 * 0..254 scale (255 = "powered from external source"). Placeholder keeps
 * the MAC happy and lets the device join. */
static uint8_t cb_battery_level(void)
{
    RAK3112_LOG("cb_battery_level");
    return 254;
}

static void cb_unique_id(uint8_t *id)
{
    RAK3112_LOG("cb_unique_id");
    memcpy(id, g_dev_eui, 8);
}

static uint32_t cb_random_seed(void)
{
    RAK3112_LOG("cb_random_seed");
    return esp_random();
}

static lmh_callback_t g_callbacks = {
    cb_battery_level,
    cb_unique_id,
    cb_random_seed,
    cb_rx,
    cb_has_joined,
    cb_confirm_class,
    cb_join_failed,
};

/* ----- Public HAL API -------------------------------------------------- */

#define EHS_RAK3112_DEVEUI_FROM_MAC 1 /* Seed DevEUI from ESP32's base MAC by default, for plug-and-play OTAA. */

#if EHS_RAK3112_DEVEUI_FROM_MAC
/* Seed g_dev_eui from the ESP32 base MAC using the RFC 4291 EUI-48 → EUI-64
 * conversion (insert 0xFF 0xFE in the middle). Preserves the Espressif OUI
 * so the resulting DevEUI is globally unique without an explicit allocation.
 * Without this step `cb_unique_id()` would hand the stack eight zero bytes
 * and OTAA would fail at every network server. Callers that have a real
 * factory-programmed DevEUI should overwrite g_dev_eui AFTER LoRaWAN_module_init
 * and BEFORE LoRaWAN_module_connect (a dedicated setter is a @todo). */
static void seed_dev_eui_from_mac(void)
{
    RAK3112_LOG("seed_dev_eui_from_mac");
    uint8_t mac[6] = {0};
    if (esp_efuse_mac_get_default(mac) != 0) {
        /* Fall back to deterministic-but-non-unique ID if efuse read fails
         * so we at least get past join on a dev-bench unit. */
        memset(g_dev_eui, 0, 8);
        RAK3112_LOG("seed_dev_eui_from_mac: efuse read failed, DevEUI zeroed");
        return;
    }
    g_dev_eui[0] = mac[0];
    g_dev_eui[1] = mac[1];
    g_dev_eui[2] = mac[2];
    g_dev_eui[3] = 0xFF;
    g_dev_eui[4] = 0xFE;
    g_dev_eui[5] = mac[3];
    g_dev_eui[6] = mac[4];
    g_dev_eui[7] = mac[5];
    RAK3112_LOG("seed_dev_eui_from_mac: DevEUI=%02X%02X%02X%02X%02X%02X%02X%02X",
                g_dev_eui[0], g_dev_eui[1], g_dev_eui[2], g_dev_eui[3],
                g_dev_eui[4], g_dev_eui[5], g_dev_eui[6], g_dev_eui[7]);
}
#endif

extern "C" ehs_lorawan_api_errno_t LoRaWAN_module_init(ehs_sint32 com_port)
{
    RAK3112_LOG("LoRaWAN_module_init: com_port=%d", (int)com_port);
    (void)com_port;  /* SPI pins are fixed by lora_rak3112_init() */
    if (g_radio_inited) return E_LWAPIERRNO_OK;
#if EHS_RAK3112_DEVEUI_FROM_MAC
    seed_dev_eui_from_mac();
#endif
    if (lora_rak3112_init() != 0) {
        return E_LWAPIERRNO_INTERNAL_ERROR;
    }
    g_radio_inited = true;
    return E_LWAPIERRNO_OK;
}

/* ----- Synchronous diagnostic accessors (see lorawan_module.h) ---------- */

extern "C" void LoRaWAN_module_peekDevEui(ehs_uint8 out_8[8])
{
    if (out_8 == NULL) return;
    memcpy(out_8, g_dev_eui, 8);
}

extern "C" ehs_bool LoRaWAN_module_peekJoined(void)
{
    return g_joined ? EHS_TRUE : EHS_FALSE;
}

extern "C" ehs_uint32 LoRaWAN_module_peekDevAddr(void)
{
    return g_joined ? (ehs_uint32)lmh_getDevAddr() : 0u;
}

extern "C" ehs_lorawan_api_errno_t LoRaWAN_module_deinit(void)
{
    RAK3112_LOG("LoRaWAN_module_deinit");
    /* @todo actually power down the SX1262. sx126x-arduino's public lmh_*
     * API exposes no teardown, but the lower-level SX126xSetSleep() /
     * SX126xSetStandby() in boards/mcu/ would work if we include that
     * header directly. Left as-is for now — deinit on rak3112 is a rare
     * path and leaving the radio idle (not transmitting) is safe.
     *
     * Deliberately do NOT clear g_lmh_inited or g_joined here. sx126x-arduino
     * has no clean teardown; the radio + MAC + join survive this call. The
     * matching LoRaWAN_module_connect on a later app reload checks
     * g_lmh_inited / g_joined and skips re-running lmh_init / lmh_join
     * (the library does not tolerate being re-called on the same radio
     * without a power cycle — silent rst:0x3 reboots otherwise). */
    return E_LWAPIERRNO_OK;
}

extern "C" ehs_lorawan_api_errno_t LoRaWAN_module_connect(
    char *AppKey, char *AppEui, ehs_bool mode,
    char *DevAddr_ABP, char *AppSKey, char *NwkSKey,
    ehs_sint32 REPT, ehs_sint32 RETRY,
    e_ehs_lw_region_t region, ehs_bool ADR, ehs_sint32 DR,
    ehs_sint32 autoJoin, char *DevAddr_OUT,
    e_ehs_lw_class_t class_type, ehs_sint32 subband,
    ehs_float rxwin2_freq, ehs_sint32 rxwin2_dr, ehs_sint32 tx_power)
{
    RAK3112_LOG("LoRaWAN_module_connect: mode=%s region=%d class=%d DR=%d ADR=%d",
                (mode == EHS_FALSE) ? "OTAA" : "ABP",
                (int)region, (int)class_type, (int)DR, (int)ADR);
    /*
    RAK3112_LOG("LoRaWAN_module_connect: REPT=%d RETRY=%d autoJoin=%d subband=%d tx_power=%d",
                (int)REPT, (int)RETRY, (int)autoJoin, (int)subband, (int)tx_power);
    RAK3112_LOG("LoRaWAN_module_connect: rxwin2_freq=%.3f rxwin2_dr=%d DevAddr_OUT=%s",
                (double)rxwin2_freq, (int)rxwin2_dr, DevAddr_OUT ? "(provided)" : "(null)");
    RAK3112_LOG("LoRaWAN_module_connect: AppEui=%s AppKey=%s",
                AppEui ? AppEui : "(null)", AppKey ? AppKey : "(null)");
    RAK3112_LOG("LoRaWAN_module_connect: DevAddr_ABP=%s AppSKey=%s NwkSKey=%s",
                DevAddr_ABP ? DevAddr_ABP : "(null)",
                AppSKey     ? AppSKey     : "(null)",
                NwkSKey     ? NwkSKey     : "(null)");
    */
    /* @todo honour the remaining parameters once the stack exposes them:
     *   REPT         — per-tx repeat count (unconfirmed uplinks); no direct
     *                  lmh_ API, would need LoRaMacSetChannelsNbTrans().
     *   autoJoin     — if EHS_FALSE, the caller expects a manual lmh_join()
     *                  later; currently we always attempt to join when OTAA.
     *   rxwin2_freq  — override RX2 frequency (region default usually fine).
     *                  Requires MibRequestConfirm_t MIB_RX2_CHANNEL.
     *   rxwin2_dr    — override RX2 datarate. Same MIB path.
     * Ignoring them is safe for a stock join to a well-configured network. */
    (void)REPT; (void)autoJoin;
    (void)rxwin2_freq; (void)rxwin2_dr;

    if (!g_radio_inited) return E_LWAPIERRNO_NOT_INITIALISED;

    const bool is_otaa = (mode == EHS_FALSE);

    /* State machine: sx126x-arduino exposes no lmh_deinit, and re-running
     * lmh_init on the same radio leaves the MAC in an inconsistent state
     * (silent rst:0x3 reboots from inside the library). So we treat
     * lmh_init and OTAA join as one-shots that survive across app reloads.
     *
     *   g_lmh_inited == false                      → first-time path:
     *                                                 keys, lmh_init,
     *                                                 OTAA join.
     *   g_lmh_inited == true && g_joined == true   → fast path: reuse the
     *                                                 existing session and
     *                                                 publish DevAddr.
     *   g_lmh_inited == true && g_joined == false  → re-join only:
     *                                                 lmh_join() but skip
     *                                                 lmh_init.
     *
     * Caveat: a second app with different region / AppEUI / class etc.
     * silently keeps the existing session. Acceptable for "reload of the
     * same app"; loading a fundamentally different LoRaWAN app needs a
     * power cycle. (See CLAUDE.md TODO for the param-drift refinement.)
     *
     * See LoRaWAN_module_deinit for the matching no-op teardown. */

    /* Fast path — already inited and joined: just hand back DevAddr. */
    if (g_lmh_inited && g_joined) {
        RAK3112_LOG("LoRaWAN_module_connect: reusing existing session, DevAddr=%08lX",
                    (unsigned long)lmh_getDevAddr());
        if (DevAddr_OUT != NULL) {
            uint32_t addr = lmh_getDevAddr();
            uint8_t b[4] = {
                (uint8_t)(addr >> 24),
                (uint8_t)(addr >> 16),
                (uint8_t)(addr >>  8),
                (uint8_t)(addr      )
            };
            bytes_to_hex(b, DevAddr_OUT, 4);
        }
        return E_LWAPIERRNO_OK;
    }

    /* First-init path — only run lmh_init and key plumbing once. */
    if (!g_lmh_inited) {
        /* Always publish the MAC-derived DevEUI to the stack before it is used;
         * cb_unique_id() reads g_dev_eui for MLME and the network server also
         * cross-checks via the helper's internal copy. */
        #if EHS_RAK3112_DEVEUI_FROM_MAC
        lmh_setDevEui(g_dev_eui);
        RAK3112_LOG("LoRaWAN_module_connect: DevEUI=%02X%02X%02X%02X%02X%02X%02X%02X",
                    g_dev_eui[0], g_dev_eui[1], g_dev_eui[2], g_dev_eui[3],
                    g_dev_eui[4], g_dev_eui[5], g_dev_eui[6], g_dev_eui[7]);
        #endif

        /* Confirmed-uplink retry count — maps to LoRaMacTestSetNbTrans() internally.
         * Must be set BEFORE lmh_init so it's in effect on the first uplink. */
        if (RETRY > 0 && RETRY <= 15) {
            (void)lmh_setConfRetries((uint8_t)RETRY);
        }

        if (is_otaa) {
            if (!hex_to_bytes(AppEui, g_app_eui, 8))  return E_LWAPIERRNO_INVALID_PARAMETERS;
            if (!hex_to_bytes(AppKey, g_app_key, 16)) return E_LWAPIERRNO_INVALID_PARAMETERS;
            lmh_setAppEui(g_app_eui);
            lmh_setAppKey(g_app_key);
            RAK3112_LOG("LoRaWAN_module_connect: OTAA AppEUI=%s", AppEui);
        } else {
            uint8_t addr_bytes[4];
            if (!hex_to_bytes(DevAddr_ABP, addr_bytes, 4)) return E_LWAPIERRNO_INVALID_PARAMETERS;
            if (!hex_to_bytes(AppSKey, g_app_skey, 16))    return E_LWAPIERRNO_INVALID_PARAMETERS;
            if (!hex_to_bytes(NwkSKey, g_nwk_skey, 16))    return E_LWAPIERRNO_INVALID_PARAMETERS;
            g_dev_addr = ((uint32_t)addr_bytes[0] << 24)
                       | ((uint32_t)addr_bytes[1] << 16)
                       | ((uint32_t)addr_bytes[2] <<  8)
                       |  (uint32_t)addr_bytes[3];
            lmh_setDevAddr(g_dev_addr);
            lmh_setAppSKey(g_app_skey);
            lmh_setNwkSKey(g_nwk_skey);
            RAK3112_LOG("LoRaWAN_module_connect: ABP DevAddr=%s", DevAddr_ABP);
        }

        lmh_param_t lora_param;
        lora_param.adr_enable           = (ADR == EHS_TRUE);
        lora_param.tx_data_rate         = (int8_t)DR;
        lora_param.enable_public_network = true;
        lora_param.nb_trials            = 8;
        lora_param.tx_power             = (int8_t)tx_power;
        lora_param.duty_cycle           = true;

        if (subband > 0) {
            lmh_setSubBandChannels((uint8_t)subband);
        }

        lmh_error_status rc = lmh_init(&g_callbacks, lora_param, is_otaa,
                                        map_class(class_type), map_region(region));
        RAK3112_LOG("LoRaWAN_module_connect: lmh_init rc=%d", (int)rc);
        if (rc != LMH_SUCCESS) return E_LWAPIERRNO_INTERNAL_ERROR;
        g_lmh_inited = true;
    } else {
        /* Already inited but lost the join (g_joined is false here). Leave
         * keys / lmh_init alone — re-issue the join only. */
        RAK3112_LOG("LoRaWAN_module_connect: already initialised, re-joining only");
    }

    /* Join (or re-join) phase. */
    if (is_otaa) {
        if (!g_joined) {
            RAK3112_LOG("LoRaWAN_module_connect: starting OTAA join (timeout 60s)");
            lmh_join();
            /* Poll for join outcome (blocking, bounded). Keeps the HAL call
             * synchronous — matches what LoRaWAN_connect callers expect. */
            const uint32_t deadline_ms = 60000;
            uint32_t elapsed = 0;
            while (elapsed < deadline_ms) {
                lmh_join_status s = lmh_join_status_get();
                if (s == LMH_SET)    { g_joined = true;  break; }
                if (s == LMH_FAILED) {
                    RAK3112_LOG("LoRaWAN_module_connect: join failed at %lu ms", (unsigned long)elapsed);
                    return E_LWAPIERRNO_JOIN_FAILED;
                }
                delay(500);
                elapsed += 500;
            }
            if (!g_joined) {
                RAK3112_LOG("LoRaWAN_module_connect: join timed out after %lu ms", (unsigned long)elapsed);
                return E_LWAPIERRNO_JOIN_FAILED;
            }
            RAK3112_LOG("LoRaWAN_module_connect: join succeeded after %lu ms, DevAddr=%08lX",
                        (unsigned long)elapsed, (unsigned long)lmh_getDevAddr());
        }
    } else {
        g_joined = true;
        RAK3112_LOG("LoRaWAN_module_connect: ABP activated, DevAddr=%08lX",
                    (unsigned long)g_dev_addr);
    }

    if (DevAddr_OUT != NULL) {
        uint32_t addr = lmh_getDevAddr();
        uint8_t b[4] = {
            (uint8_t)(addr >> 24),
            (uint8_t)(addr >> 16),
            (uint8_t)(addr >>  8),
            (uint8_t)(addr      )
        };
        bytes_to_hex(b, DevAddr_OUT, 4);
    }

    return E_LWAPIERRNO_OK;
}

extern "C" ehs_lorawan_api_errno_t LoRaWAN_module_send_msg(
    char *payload, ehs_uint8 fport, ehs_bool confirmed)
{
    RAK3112_LOG("LoRaWAN_module_send_msg: fport=%u confirmed=%d payload_len=%zu",
                (unsigned)fport, (int)confirmed, payload ? strlen(payload) : 0);
    if (!g_joined) return E_LWAPIERRNO_NOT_JOINED;
    if (payload == NULL) return E_LWAPIERRNO_INVALID_PARAMETERS;

    size_t hex_len = strlen(payload);
    if ((hex_len & 1) != 0) return E_LWAPIERRNO_INVALID_PARAMETERS;
    size_t bytes = hex_len / 2;
    if (bytes == 0 || bytes > RAK3112_MAX_PAYLOAD) return E_LWAPIERRNO_PAYLOAD_TOO_LONG;
    if (!hex_to_bytes(payload, g_tx_buffer, bytes)) return E_LWAPIERRNO_INVALID_PARAMETERS;

    lmh_app_data_t app_data = {0};
    app_data.buffer   = g_tx_buffer;
    app_data.buffsize = (uint8_t)bytes;
    app_data.port     = (fport == 0) ? RAK3112_LW_APP_PORT_DEFAULT : fport;

    lmh_error_status rc = lmh_send(&app_data,
        (confirmed == EHS_TRUE) ? LMH_CONFIRMED_MSG : LMH_UNCONFIRMED_MSG);
    if (rc == LMH_BUSY)    return E_LWAPIERRNO_MODEM_BUSY;
    if (rc != LMH_SUCCESS) return E_LWAPIERRNO_INTERNAL_ERROR;
    return E_LWAPIERRNO_OK;
}

extern "C" ehs_lorawan_api_errno_t LoRaWAN_module_reset(void)
{
    RAK3112_LOG("LoRaWAN_module_reset");
    lmh_reset_mac();
    g_joined = false;
    return E_LWAPIERRNO_OK;
}

extern "C" ehs_lorawan_api_errno_t LoRaWAN_module_get_sysData(char *data, char *DevEui)
{
    RAK3112_LOG("LoRaWAN_module_get_sysData");
    if (data != NULL) {
        /* @todo expand: RSSI/SNR (available from gEhsLoraApiData), current
         * DR/class (lmh_class_get), region (tracked at connect()), link
         * margin + gateway count (populated by MLME link-check confirm,
         * which is a @todo below). Current form is the minimum the FB's
         * get_statusData_cb needs to produce a non-empty string. */
        DeviceClass_t cur = CLASS_A;
        lmh_class_get(&cur);
        snprintf(data, 128,
                 "{\"devaddr\":\"%08lX\",\"joined\":%s,\"class\":\"%c\"}",
                 (unsigned long)lmh_getDevAddr(),
                 g_joined ? "true" : "false",
                 (cur == CLASS_C) ? 'C' : (cur == CLASS_B) ? 'B' : 'A');
    }
    if (DevEui != NULL) {
        bytes_to_hex(g_dev_eui, DevEui, 8);
    }
    return E_LWAPIERRNO_OK;
}

extern "C" ehs_lorawan_api_errno_t LoRaWAN_module_set_datarate(ehs_sint32 datarate)
{
    RAK3112_LOG("LoRaWAN_module_set_datarate: datarate=%d", (int)datarate);
    lmh_datarate_set((uint8_t)datarate, false);
    return E_LWAPIERRNO_OK;
}

extern "C" ehs_lorawan_api_errno_t LoRaWAN_module_get_payloadLength(ehs_sint32 *length)
{
    RAK3112_LOG("LoRaWAN_module_get_payloadLength");
    if (length == NULL) return E_LWAPIERRNO_INVALID_PARAMETERS;
    /* @todo the real max-payload varies with region + current DR + FOpts
     * length. LoRaMac has the info internally (MIB_MAX_PAYLOAD_OF_DATARATE),
     * but sx126x-arduino doesn't expose it. Reporting the spec maximum is
     * safe for sizing the caller's buffer; it may just cause the actual
     * transmit to be rejected if the caller exceeds the per-DR cap. */
    *length = RAK3112_MAX_PAYLOAD;
    return E_LWAPIERRNO_OK;
}

extern "C" ehs_lorawan_api_errno_t LoRaWAN_module_disable(void)
{
    RAK3112_LOG("LoRaWAN_module_disable");
    /* @todo actually stop TX/RX and put the radio to sleep. Currently this
     * only clears the joined flag so the FB reports disconnected; the radio
     * itself stays in whatever idle state lmh_ leaves it in. See deinit
     * TODO — both need direct access to the boards/mcu sleep API. */
    g_joined = false;
    return E_LWAPIERRNO_OK;
}

extern "C" ehs_lorawan_api_errno_t LoRaWAN_module_set_class(e_ehs_lw_class_t class_type)
{
    RAK3112_LOG("LoRaWAN_module_set_class: class_type=%d", (int)class_type);
    lmh_error_status rc = lmh_class_request(map_class(class_type));
    return (rc == LMH_SUCCESS) ? E_LWAPIERRNO_OK : E_LWAPIERRNO_INVALID_CLASS;
}

extern "C" ehs_lorawan_api_errno_t LoRaWAN_module_set_txpower(ehs_sint32 tx_power)
{
    RAK3112_LOG("LoRaWAN_module_set_txpower: tx_power=%d", (int)tx_power);
    lmh_tx_power_set((uint8_t)tx_power);
    return E_LWAPIERRNO_OK;
}

extern "C" ehs_lorawan_api_errno_t LoRaWAN_module_link_check(void)
{
    RAK3112_LOG("LoRaWAN_module_link_check");
    /* @todo request an explicit link-check and plumb the answer through.
     * LoRaMac supports it via LoRaMacMlmeRequest(MLME_LINK_CHECK) and the
     * confirm callback carries DemodMargin + NbGateways — the exact fields
     * the FB wants in gEhsLoraApiData.link_margin / .gateway_count. The
     * sx126x-arduino lmh_callback_t has no MLME-confirm hook though, so
     * this needs either (a) extending the helper, or (b) including
     * mac/LoRaMac.h directly and installing a MlmeConfirm callback on top
     * of what lmh_init registers. Returning OK keeps the FB happy for now;
     * link_margin / gateway_count will stay at their last set values. */
    return E_LWAPIERRNO_OK;
}
