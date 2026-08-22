#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------

# @file config.mk
# eRT platform for Nordic Thingy:91 X — CELLULAR VARIANT (LTE-M / NB-IoT).
#
# nRF9151 integrated modem via NCS's nrf_modem + lte_lc. Everything that is not
# cellular lives in the shared baseline included below.
#
# Sibling: zephyr_arm-nrf9151_thingy91x-wifi. The two CANNOT be combined — see
# the header of the base config, and the guard in
# target/Component-HAL/cellular/cellular.mk that rejects any config setting
# both. That guard fires at configure time; without it the symptom is a linker
# "region RAM overflowed by 57076 bytes" ten minutes into a build, which reads
# like a tuning problem and is not one.
#
# SIM
# ---
# The Thingy:91 X has a nano-SIM (4FF) socket J4 and an UNPOPULATED eSIM
# footprint U15 which share one UICC interface, routed by an analog switch
# driven by the card-detect signal. So: insert a card and it is used; there is
# no software slot selection between physical and embedded, and the HAL clears
# EHS_CELL_CAP_SIM_SELECT accordingly. The kit's bundled pre-activated SIMs
# need no APN — leave ERT_CELL_APN empty for them.
#
# Expected footprint (estimated — see docs/_working_notes/ert-cellular-plan.md § 8.2; the
# firm/ceiling spread is three unresolved attributions that building the
# -base target resolves):
#   FLASH ~366-506 KB (49-67%)   RAM ~118-169 KB (54-76%)
# against the Wi-Fi variant's measured 94.9% / 89.7%. Cellular is by far the
# roomier of the two: its whole vendor stack is ~38 KB of flash where Wi-Fi's
# is ~255 KB.

#################################################################################################################
# Baseline Configuration Includes
#################################################################################################################

include ./target/platform/zephyr_arm-nrf9151_thingy91x-base/config.mk

# Inherit the base platform's zephyr/boards/*.overlay and zephyr/pm_static.yml.
# This variant adds its own boards/*.conf, which is layered on top by name.
ERT_ZEPHYR_BASE_PLATFORM = zephyr_arm-nrf9151_thingy91x-base

#################################################################################################################
# Default Application
#################################################################################################################

EHS_DEFAULT_APP = xiaosheng/cellular_test

#################################################################################################################
# Cellular (LTE-M / NB-IoT)
#################################################################################################################

# Modem backend selector. The actual Kconfig list lives in
# target/os-arch/zephyr-arm/cellular_nrf91.mk (included from that OS-arch's
# config.mk), shared across every nRF91 board rather than copy-pasted here —
# same arrangement as EHS_WIFI_SUPPORT / wifi_nrf70.mk.
EHS_CELLULAR_SUPPORT = zephyr_nrf91

# Enables the eRT cellular subsystem on top of the modem selected above: the
# shared connect state machine and Cell_*/EhsSim_* API
# (Common/Components/networking/inx-cellular_link.c) plus this OS-arch's HAL
# backend (target/os-arch/zephyr-arm/target_cellular.c).
EHS_NETWORK_CELLULAR_SUPPORT = yes

# Default radio access technology: ltem | nbiot | both.
# LTE-M first because its attach is far quicker (seconds to a couple of minutes
# against NB-IoT's minutes to tens of minutes), which makes bring-up and field
# diagnosis very much easier. Switch to 'nbiot' or 'both' once the deployment's
# operator coverage is known. Runtime override is available via Cell_SetRat().
ERT_CELL_RAT = ltem

# Build-time seed APN. Empty means network-assigned, which is normal and
# correct on most modern networks and is what the kit's bundled pre-activated
# Onomondo / Wireless Logic SIMs expect. EHS_NVS_SUPPORT is still 'stubbed' on
# Zephyr so nothing set at runtime persists; this seed applies on every boot.
ERT_CELL_APN =

# --- eRT logger: make the cellular state machine visible ---------------------
# Same mechanism and reasoning as the Wi-Fi variant. Without this every module
# defaults to ERROR ONLY, which suppresses the entire attach progress trace —
# and on a link whose cold attach can legitimately take minutes, "silent" and
# "hung" are indistinguishable without it. ERROR|WARNING|INFO = 0x07.
DEFS += EHS_LOG_LEVEL_NETWORK=0x07


#################################################################################################################
# Sockets
#################################################################################################################

# On nRF91 the IP stack lives IN THE MODEM and BSD sockets are offloaded to it
# (CONFIG_NET_SOCKETS_OFFLOAD, set in cellular_nrf91.mk). The eRT bsdsockets
# backend is plain POSIX socket/bind/connect, so it should ride on that
# unchanged — that is the point of offloading. Enabling it here also makes
# hal.c call EhsTCommsSys_init() at boot, which is what the mqtt_* /
# inx-netsocket / url_get function blocks need.
EHS_COMMS_API_SUPPORT = bsdsockets

# MCU-sized TCP buffers. Not strictly needed while the Lucid console stays
# stubbed (inherited from base — see below), but the bsdsockets defaults are
# 65536/61440 (target_tcp.h, sized for desktop) — about 124 KB of static RAM
# against this part's budget. Set here so that enabling the console later is a
# one-line change rather than a link-time surprise. Buffer size must be a power
# of two and >= both TCP buffers (console_server.c #errors otherwise).
DEFS += EHS_TGT_TCP_IN_BUFF_SIZE=128
DEFS += EHS_TGT_TCP_OUT_BUFF_SIZE=128
DEFS += EHS_DEBUG_CONSOLE_BUFFER_SIZE=256

# --- Lucid TCP console: DELIBERATELY OFF on this variant ---------------------
# Inherited as 'stubbed' from the base config (which must set it explicitly —
# EHS_DEBUGALL would otherwise force it on; see the note there). Two reasons,
# either sufficient on its own:
#  1. Most cellular IoT APNs are NAT'd with no inbound reachability, so a
#     listener on port 11425 is simply not connectable from outside.
#  2. It is a metered link. Running the Lucid protocol over it costs data.
# The UART console remains available for app upload and debugging.
# To enable anyway:  EHS_DEBUG_TCPIP_CONSOLE = yes

# Native-stack net_contexts are largely moot with offloaded sockets — the modem
# enforces its own limit instead (8 sockets on nRF91, TLS counting against it).
# Left at the base default rather than carried over from the Wi-Fi variant's 5.

#################################################################################################################
# MQTT
#################################################################################################################

# Enables the mqtt_client / mqtt_publish / mqtt_subscribe / mqtt_publish_typed /
# mqtt_subscribe_typed function blocks (registered under #ifdef EHS_MQTT_SUPPORT
# in Common/Components/networking/) on top of Zephyr's in-tree socket MQTT
# library. Backend: target/Component-HAL/mqtt/zephyr/.
#
# It rides the offloaded sockets configured above with no cellular-specific
# code — the whole point of CONFIG_NET_SOCKETS_OFFLOAD. None of the other
# backends fit: paho needs a cross-built .so that does not exist for
# arm-zephyr-eabi, lwip needs lwIP, esp_mqtt needs ESP-IDF.
EHS_MQTT_SUPPORT = zephyr

# PLAINTEXT ONLY (port 1883). A connect with TLS enabled is refused with an
# error to the function block rather than silently downgraded — see the TLS
# note in target/Component-HAL/mqtt/zephyr/mqtt.c for why supporting it is not
# just a matter of turning CONFIG_MQTT_LIB_TLS on. Do not point this at a
# public broker with real credentials.

# Publish size cap. tx_buf has to hold a whole PUBLISH — header, topic and
# payload — because mqtt_publish() serialises it in one pass, so this is the
# effective maximum message size. 1024 covers typical JSON telemetry; a publish
# that does not fit is rejected by the library rather than truncated. The rx
# and payload buffers default to 512 each (mqtt.c), which suits the short
# topics and small commands this direction usually carries.
#   DEFS += EHS_MQTT_TX_BUF_SIZE=1024      (default)
#   DEFS += EHS_MQTT_RX_BUF_SIZE=512       (default — sized by topic length)
#   DEFS += EHS_MQTT_PAYLOAD_BUF_SIZE=512  (default — inbound message cap)
# Left at the defaults: ~2 KB of static RAM in total, against a variant sitting
# near 40% RAM.

# The mqtt_client function block starts the client loop on a dynamic thread
# (EHS_MQTT_CLIENT_FB_THREAD, set by the backend's mqtt.mk), taking one of the
# base config's EHS_ZEPHYR_MAX_DYNAMIC_THREADS=4 slots. The cellular worker
# does NOT compete for one — it has a dedicated K_THREAD_STACK_DEFINE in
# target_cellular.c — so 4 still leaves headroom. Raise it there if an app also
# needs FB threads of its own.

################################### END OF CONFIGURATION ###################################################
