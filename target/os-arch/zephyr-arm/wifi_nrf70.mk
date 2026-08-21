#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------

# Nordic nRF70-series Wi-Fi (station mode) Kconfig for NCS-based Zephyr targets.
# Included from config.mk when EHS_WIFI_SUPPORT=zephyr_nrf70 - the platform
# config.mk only carries the on/off selector; the actual Kconfig values live
# here so they're shared (and only need updating in one place) across every
# nRF70-equipped board, rather than copy-pasted per platform.
#
# Starting point: Nordic's own nrf/samples/wifi/sta/prj.conf (a "normally
# configured" station sample), minus the sample's own hardcoded SSID/static-IP/
# debug-coredump bits, which are application concerns, not board/Kconfig ones.
#
# CRITICAL: CONFIG_WIFI_NRF70=y in prj.conf is NOT enough on its own. sysbuild
# has its OWN separate Kconfig namespace (SB_CONFIG_*, from nrf/sysbuild/Kconfig.wifi)
# with its own `menuconfig WIFI_NRF70` that defaults to n; whatever it computes
# gets merged into the per-image .config *after* prj.conf, silently overriding
# a plain CONFIG_WIFI_NRF70=y with no warning. The sta sample's own
# sysbuild.conf sets SB_CONFIG_WIFI_NRF70=y - see the matching
# ERT_ZEPHYR_SYSBUILD_KCONFIG line below and write_sysbuild_conf() in
# scripts/zephyr_cmake_gen.py. Getting this backwards (missing the sysbuild.conf
# entirely) is what looked like an NCS v3.3.0-specific upstream Kconfig bug
# during initial bring-up - it almost certainly wasn't; that test never had a
# sysbuild.conf either, so it never actually exercised this gap. NCS v2.9.0
# was still kept as the pinned version (see ERT_ZEPHYR_VERSION in the platform
# config.mk) since it's a confirmed-working reference and this fix landed after
# the switch - v3.3.0 may well work fine with this fix too, untested since.
#
# NOTE: symbol names here are pinned to NCS v2.9.0 - v2.9.0's sta sample uses a
# single unified CONFIG_HEAP_MEM_POOL_SIZE (no split ctrl/data nRF_WIFI heaps -
# those Kconfig symbols don't exist yet in this version), CONFIG_NET_SOCKETS_POLL_MAX
# (not CONFIG_ZVFS_POLL_MAX), and CONFIG_POSIX_CLOCK (not CONFIG_POSIX_TIMERS).
# If ERT_ZEPHYR_VERSION is ever bumped, re-diff against that version's own
# nrf/samples/wifi/sta/{prj.conf,sysbuild.conf} before assuming these names
# still apply. CONFIG_HEAP_MEM_POOL_SIZE below is a known-reducible starting
# point (see nRF9151 RAM-budget review) - trim once real traffic/connection
# patterns are known, don't just copy this onto every board unchanged.

ERT_ZEPHYR_SYSBUILD_KCONFIG += SB_CONFIG_WIFI_NRF70=y

ERT_ZEPHYR_KCONFIG += CONFIG_WIFI=y
ERT_ZEPHYR_KCONFIG += CONFIG_WIFI_NRF70=y
ERT_ZEPHYR_KCONFIG += CONFIG_WIFI_NM_WPA_SUPPLICANT=y
ERT_ZEPHYR_KCONFIG += CONFIG_WIFI_READY_LIB=y
ERT_ZEPHYR_KCONFIG += CONFIG_NRF_WIFI_RPU_RECOVERY=y
ERT_ZEPHYR_KCONFIG += CONFIG_WIFI_MGMT_EXT=y
# Default (y): the nRF70 net_if auto-starts (net_if_up(), triggering the RPU
# firmware bring-up in nrf_wifi_if_start_zep()) very early in boot - too early
# for our console/log backend to be alive yet, so any output from that
# sequence (including a failure) is silently lost. Confirmed via
# net_if_up() returning -EALREADY from our own (much later, 6s-delayed)
# explicit call - the auto-start had already run and set the admin-up flag,
# but the interface was still operationally down and wpa_supplicant's own
# calls kept failing with "RPU context not initialized". Disabling auto-start
# makes wifi_connect_test.c's own (console-confirmed-alive) net_if_up() call
# the first and only one, so RPU bring-up finally happens somewhere we can
# actually see its output.
ERT_ZEPHYR_KCONFIG += CONFIG_NRF_WIFI_IF_AUTO_START=n

ERT_ZEPHYR_KCONFIG += CONFIG_NETWORKING=y
ERT_ZEPHYR_KCONFIG += CONFIG_NET_SOCKETS=y
ERT_ZEPHYR_KCONFIG += CONFIG_NET_IPV4=y
ERT_ZEPHYR_KCONFIG += CONFIG_NET_UDP=y
ERT_ZEPHYR_KCONFIG += CONFIG_NET_TCP=y
ERT_ZEPHYR_KCONFIG += CONFIG_NET_DHCPV4=y

ERT_ZEPHYR_KCONFIG += CONFIG_NET_PKT_RX_COUNT=6
ERT_ZEPHYR_KCONFIG += CONFIG_NET_PKT_TX_COUNT=6

# --- nRF70 driver RX buffers -------------------------------------------------
# NOT the Zephyr net_buf pool below: these are allocated with k_calloc out of
# CONFIG_HEAP_MEM_POOL_SIZE, one per descriptor, each
# CONFIG_NRF70_RX_MAX_DATA_SIZE (1600 B) + headroom. So the count below costs
# roughly count x 1.65 KB of HEAP - budget for it there, not in static RAM.
#
# CRITICAL - THE VALUE MUST BE A MULTIPLE OF 3, AND AT LEAST 3.
# zephyr/drivers/wifi/nrf_wifi/src/fmac_main.c splits it across the three RX
# queues with plain integer division and no rounding:
#     rx1_num_bufs = rx2_num_bufs = rx3_num_bufs = CONFIG_NRF70_RX_NUM_BUFS / 3
# and fmac_api.c then sums those three into def_priv->num_rx_bufs. Kconfig
# declares NO range for this symbol, so anything 1..2 is accepted silently,
# divides to 0 per queue, and makes num_rx_bufs 0. nrf_wifi_fmac_init_rx() then
# loops zero times and returns its NRF_WIFI_STATUS_FAIL initialiser WITHOUT
# logging a reason, so the only symptom is a bare
#     <err> wifi_nrf: nrf_wifi_fmac_fw_init: Init RX failed
# followed by dev_init/dev_add_zep/if_start_zep failing in cascade, and every
# later supplicant call reporting "RPU context not initialized". This is what
# the previous value of 2 did. The guard below makes that impossible to
# reintroduce silently.
#
# 6 (2 per queue) is Nordic's own low-memory figure - it is what both
# nrf/samples/wifi/throughput/overlay-memory-optimized.conf and
# overlay-iot-devices.conf use. The sta sample uses 16 and the Kconfig default
# is 48 (~79 KB of heap), neither of which fits this part's budget.
ERT_ZEPHYR_NRF70_RX_NUM_BUFS ?= 3

ifneq ($(shell expr $(ERT_ZEPHYR_NRF70_RX_NUM_BUFS) % 3),0)
$(error ERT_ZEPHYR_NRF70_RX_NUM_BUFS=$(ERT_ZEPHYR_NRF70_RX_NUM_BUFS) is not a multiple of 3. \
The nRF70 driver divides it across 3 RX queues with integer division, so a non-multiple \
silently wastes buffers and a value below 3 yields 0 per queue and fails RPU init with a bare \
"Init RX failed". Use 3, 6, 9, ...)
endif
ifeq ($(shell test $(ERT_ZEPHYR_NRF70_RX_NUM_BUFS) -ge 3 && echo ok),)
$(error ERT_ZEPHYR_NRF70_RX_NUM_BUFS=$(ERT_ZEPHYR_NRF70_RX_NUM_BUFS) is below the minimum of 3 \
(one buffer per RX queue). See the comment above - this is the "Init RX failed" trap.)
endif

ERT_ZEPHYR_KCONFIG += CONFIG_NRF70_RX_NUM_BUFS=$(ERT_ZEPHYR_NRF70_RX_NUM_BUFS)
# MAX_TX_TOKENS is at its Kconfig-stated minimum (range 5-12 outside radio-test).
ERT_ZEPHYR_KCONFIG += CONFIG_NRF70_MAX_TX_TOKENS=5
# 4 each was the pre-TCP minimum. With a TCP server accepting a connection,
# buffers stay pinned in the connection's queues, and at 4 the pools can be
# fully consumed by one client - after which NOTHING else can be received,
# including ICMP. That is the "device answers Lucid once, then stops
# responding to everything, ping included" signature. 6 is Nordic's own
# low-memory figure (nrf/samples/wifi/throughput/overlay-memory-optimized.conf)
# and costs ~48 bytes per buffer, measured.
ERT_ZEPHYR_KCONFIG += CONFIG_NET_BUF_RX_COUNT=6
ERT_ZEPHYR_KCONFIG += CONFIG_NET_BUF_TX_COUNT=6
ERT_ZEPHYR_KCONFIG += CONFIG_NET_TC_TX_COUNT=1

ERT_ZEPHYR_KCONFIG += CONFIG_NET_IF_UNICAST_IPV4_ADDR_COUNT=1
# One net_context per active socket. 2 covers a Wi-Fi station doing DHCP with
# one socket spare. A platform that also runs a TCP server (the Lucid debug
# console, say) needs one for the listener plus one per accepted connection -
# raise it via ERT_ZEPHYR_NET_MAX_CONTEXTS in the platform config.mk.
#
# It has to be a variable rather than a plain override in the platform file:
# this .mk is included AFTER the platform config.mk, so its ERT_ZEPHYR_KCONFIG
# entries land later in prj.conf and win. A second
# "ERT_ZEPHYR_KCONFIG += CONFIG_NET_MAX_CONTEXTS=..." in the platform config is
# silently ignored (measured - it cost a probe cycle to notice).
#
# Cost, measured on nRF9151: ~771 bytes of RAM per context.
ERT_ZEPHYR_NET_MAX_CONTEXTS ?= 2
ERT_ZEPHYR_KCONFIG += CONFIG_NET_MAX_CONTEXTS=$(ERT_ZEPHYR_NET_MAX_CONTEXTS)
ERT_ZEPHYR_KCONFIG += CONFIG_NET_CONTEXT_SYNC_RECV=y

ERT_ZEPHYR_KCONFIG += CONFIG_NET_L2_ETHERNET=y

ERT_ZEPHYR_KCONFIG += CONFIG_NET_CONFIG_SETTINGS=y
ERT_ZEPHYR_KCONFIG += CONFIG_NET_CONFIG_INIT_TIMEOUT=0
ERT_ZEPHYR_KCONFIG += CONFIG_NET_MGMT_EVENT_QUEUE_TIMEOUT=5000

ERT_ZEPHYR_KCONFIG += CONFIG_NET_SOCKETS_POLL_MAX=10

# --- wpa_supplicant control-interface socketpairs --------------------------
# Zephyr defaults this to 4096 *because* WIFI_NM_WPA_SUPPLICANT is enabled
# (subsys/net/lib/sockets/Kconfig: "default 4096 if WIFI_NM_WPA_SUPPLICANT",
# range 1..4096). That default is far too expensive here.
#
# struct spair embeds the whole buffer inline - `uint8_t buf[BUFFER_SIZE]` -
# and socketpair.c k_mallocs one struct PER ENDPOINT, so a single socketpair
# costs 2 x BUFFER_SIZE of heap. The supplicant opens TWO per interface: the
# control channel in wpa_supplicant_ctrl_iface_init(), then the monitor
# channel in wpa_cli_open_connection(). At 4096 that is ~16.8 KB of heap for
# the control plane alone.
#
# That is what broke the second bring-up attempt: with the RX buffers above
# now actually being allocated (~10 KB, they never were while RPU init was
# failing), the first socketpair succeeded and the second could not, so
# zephyr_wpa_ctrl_init() failed with
#     <err> wifi_supplicant: Failed to initialize supplicant control interface
# and - because supp_main.c only emits NET_EVENT_SUPPLICANT_CMD_READY *after*
# that call - the wifi_ready callback never fired and bring-up hung until the
# HAL's timeout. Note the interface itself had already been added fine, so
# this looks nothing like a driver fault.
#
# 1024 is 2x the supplicant's own message bound: wpa_cli_zephyr.c caps both
# directions at MAX_CMD_SIZE / MAX_RESPONSE_SIZE = 512, and scan results do
# NOT travel this path in the Zephyr integration (they arrive via the
# scan_result callback and net_mgmt). It is also fail-safe rather than
# fail-hard: these are SOCK_STREAM pairs with reader and writer on different
# threads, so an oversized message is chunked, not dropped. Saves ~12.4 KB of
# heap - more than the RX buffers above consume.
# Overridable, because this file is included AFTER the platform config.mk and
# would otherwise silently win - the same trap the heap has. A platform sharing
# the k_heap with another stack (the Wi-Fi+cellular combo) needs to hand these
# 2 KB back to wpa_supplicant's own allocations.
ERT_ZEPHYR_SOCKETPAIR_BUF ?= 1024
ERT_ZEPHYR_KCONFIG += CONFIG_NET_SOCKETPAIR_BUFFER_SIZE=$(ERT_ZEPHYR_SOCKETPAIR_BUF)
# Sample's stack sizes (5200/4096/4096) assumed the Wi-Fi connect logic runs on
# the Zephyr main thread (CONFIG_STA_SAMPLE_START_WIFI_THREAD_STACK_SIZE) -
# we don't have that yet (main() only spawns the separate EHS thread, sized
# independently in target_main.c). Trimmed as part of the viability probe;
# raise back if this causes a stack-overflow crash rather than a clean boot.
ERT_ZEPHYR_KCONFIG += CONFIG_MAIN_STACK_SIZE=2048
ERT_ZEPHYR_KCONFIG += CONFIG_NET_TX_STACK_SIZE=2048
ERT_ZEPHYR_KCONFIG += CONFIG_NET_RX_STACK_SIZE=2048

ERT_ZEPHYR_KCONFIG += CONFIG_ENTROPY_GENERATOR=y
ERT_ZEPHYR_KCONFIG += CONFIG_POSIX_CLOCK=y

# Single unified heap (no dedicated nRF_WIFI ctrl/data heaps in this NCS
# version) - covers net stack + wpa_supplicant + nRF70 driver allocations.
# Nordic's sta sample uses 120000, tuned for performance on boards with more
# RAM headroom. The 24000 bare-minimum viability probe produced a concrete,
# specific failure - "wifi_supplicant: Failed to add iface wlan0" within
# hostap's 1000ms IFACE_NOTIFY_TIMEOUT_MS - consistent with wpa_supplicant's
# own interface-registration allocations silently failing/stalling under heap
# pressure in that 1s window, not a hard "doesn't fit" wall.
#
# The nRF70 RX buffers come out of THIS pool - each descriptor is a k_calloc of
# CONFIG_NRF70_RX_MAX_DATA_SIZE + headroom, so the 6 buffers above want roughly
# 10 KB of it. They are allocated during RPU init, i.e. BEFORE wpa_supplicant
# registers its interface, so they get first call on the pool.
#
# Left at 60000 rather than 60000 + 10 KB, deliberately. 60000 is the only
# figure with any evidence behind it (24000 failed supplicant iface
# registration, 60000 did not), but that evidence was gathered while RPU init
# was failing and therefore NO RX buffers were ever allocated - so it bounds
# the supplicant's needs at <= 60000, not at 60000. Adding a 10 KB margin on
# top overflows this part's RAM by ~800 bytes (measured), and the nRF9151 has
# nothing spare to give: the production image is at ~96% before any of this.
#
# The failure mode if 60000 turns out to be too tight is specific and easy to
# recognise - the driver logs
#     <err> wifi_nrf: nrf_wifi_fmac_rx_cmd_send: No space for allocating RX buffer
#     <err> wifi_nrf: nrf_wifi_fmac_fw_init: Init RX failed
# Note the FIRST line: that is what distinguishes heap exhaustion from the
# divide-to-zero trap above, which produces "Init RX failed" with no reason
# line at all. If you see it, raise ERT_ZEPHYR_NRF70_HEAP_EXTRA below and find
# the RAM elsewhere (dropping CONFIG_LOG_MODE_IMMEDIATE is NOT the answer -
# deferred mode allocates a log ring buffer and costs ~2 KB MORE, measured).
ERT_ZEPHYR_NRF70_HEAP_EXTRA ?= 0
ERT_ZEPHYR_KCONFIG += CONFIG_HEAP_MEM_POOL_SIZE=$(shell expr 60000 + $(ERT_ZEPHYR_NRF70_HEAP_EXTRA))

# READ THIS BEFORE DEBUGGING ANY "allocation failed" ON THIS TARGET.
# IGNORE_MIN suppresses Zephyr's build-time check that HEAP_MEM_POOL_SIZE is at
# least the sum of what every subsystem declared it needs via
# CONFIG_HEAP_MEM_POOL_ADD_SIZE_*. On this build that sum is:
#     HOSTAP      30000
#     NRF70      150000
#     SOCKETPAIR  32000
#     MQUEUE       1024
#     ----------------
#     total      213024   ... against a 216 KB RAM region, and we give it 60000.
# Those figures are worst-case declarations, not measurements, and the board
# demonstrably runs on a fraction of them - but it does mean the heap is the
# first thing to suspect for any runtime allocation failure here, and that
# there is NO build-time protection. Check the sum after any Kconfig change:
#     grep HEAP_MEM_POOL_ADD_SIZE <build>/app/zephyr/.config
ERT_ZEPHYR_KCONFIG += CONFIG_HEAP_MEM_POOL_IGNORE_MIN=y
