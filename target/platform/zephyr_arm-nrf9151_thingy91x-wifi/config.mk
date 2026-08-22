#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------

# @file config.mk
# eRT platform for Nordic Thingy:91 X — WI-FI VARIANT.
#
# nRF7002 Wi-Fi 6 station on the nRF9151 application core. Everything that is
# not Wi-Fi lives in the shared baseline included below.
#
# Sibling: zephyr_arm-nrf9151_thingy91x-cellular (LTE-M / NB-IoT). The two
# CANNOT be combined — see the header of the base config for why, and
# target/Component-HAL/cellular/cellular.mk for the guard that enforces it.
#
# Measured footprint of this variant (the shipping build, `size` on zephyr.elf):
#   FLASH 715,074 B (94.9%)   RAM 198,430 B (89.7%)
# of which the Wi-Fi vendor stack alone is ~255 KB of flash — a 79,852 B
# nRF7002 firmware blob plus a ~157 KB wpa_supplicant. There is very little
# headroom here; check docs/_working_notes/ert-cellular-plan.md § 8 before adding anything.

#################################################################################################################
# Baseline Configuration Includes
#################################################################################################################

include ./target/platform/zephyr_arm-nrf9151_thingy91x-base/config.mk

# Inherit the base platform's zephyr/boards/*.overlay and zephyr/pm_static.yml.
# Files of the same name in this platform's own zephyr/ dir override them
# individually, which is how the .conf below applies without restating the DTS
# overlay or the partition layout.
ERT_ZEPHYR_BASE_PLATFORM = zephyr_arm-nrf9151_thingy91x-base


#################################################################################################################
# Wi-Fi station mode
#################################################################################################################

# nRF7002 is wired via SPI directly to this core (see the base platform's
# thingy91x_nrf9151_ns.overlay for the confirming devicetree trace) — this is
# just the on/off + backend selector; the actual Kconfig list lives in
# target/os-arch/zephyr-arm/wifi_nrf70.mk (included from that OS-arch's
# config.mk), shared across every nRF70-equipped board rather than copy-pasted
# per platform.
EHS_WIFI_SUPPORT = zephyr_nrf70

# Enables the eRT Wi-Fi station subsystem on top of the driver selected above:
# the shared connect state machine + wifi_station function block
# (Common/Components/networking/inx-wifi_station.c) and this OS-arch's HAL
# backend (target/os-arch/zephyr-arm/target_wifi.c).
EHS_NETWORK_WIFI_SUPPORT = yes

# The net/socket subsystem enforces a minimum of 37 file descriptors once
# CONFIG_WIFI_NM_WPA_SUPPLICANT is enabled (auto-corrected with a CMake warning
# otherwise) — set explicitly to avoid the warning. Overrides the base's 16.
ERT_ZEPHYR_KCONFIG += CONFIG_ZVFS_OPEN_MAX=37

# --- Wi-Fi station credentials ---
# Deployment-specific (this network only), so kept here rather than in the
# shared wifi_nrf70.mk. EDIT BEFORE BUILDING — placeholders won't connect to
# anything. WIFI_CREDENTIALS_BACKEND_NONE (implied by STATIC) stores these as
# plain build-time strings, not encrypted at rest — fine for a bring-up test
# on a lab network, not for production credentials.
#
# These are the *seed* credentials only. EHS_NVS_SUPPORT is still 'stubbed' on
# Zephyr, so there is nowhere to persist credentials set at runtime (via the
# wifi_station FB or Wifi_Connect) — they apply for the current boot and are
# lost on reset, and on the next boot target_main.c re-seeds from the values
# below. Once a Zephyr NVS backend exists under target/Component-HAL/nvs/,
# saved credentials will take priority and these become a first-boot default.
ERT_ZEPHYR_KCONFIG += CONFIG_WIFI_CREDENTIALS=y
ERT_ZEPHYR_KCONFIG += CONFIG_WIFI_CREDENTIALS_STATIC=y
ERT_ZEPHYR_KCONFIG += CONFIG_WIFI_CREDENTIALS_STATIC_SSID="INX_11AC"
ERT_ZEPHYR_KCONFIG += CONFIG_WIFI_CREDENTIALS_STATIC_PASSWORD="WSINX909wIfIPW*"

# --- eRT logger: make the Wi-Fi state machine visible ------------------------
# Separate mechanism from the Zephyr CONFIG_LOG_* in the base config. EHSH_LOG_*
# writes straight to stdio (hal_logger.c), and without EHS_LOG_LEVEL_VERBOSE
# every module defaults to ERROR ONLY — which silently suppressed every Wi-Fi
# progress message the state machine emits, including "Got IP: <addr>", the one
# line that tells you whether DHCP bound. Turning the Network module up to
# ERROR|WARNING|INFO (0x01|0x02|0x04) is a handful of lines per connect attempt,
# not spam, and it is the difference between a diagnosable board and a silent
# one. Module IDs and bits: Common/HAL/include/hal_logger.h.
DEFS += EHS_LOG_LEVEL_NETWORK=0x07


#################################################################################################################
# TCP/IP debug console (Lucid over the network)
#################################################################################################################

# Serves the Lucid tools protocol on TCP port 11425 (EHS_TGT_TCP_PORTNUM), so
# apps can be uploaded and debugged over Wi-Fi instead of by reflashing.
# Common/HAL/hal.c's EhsHSys_init() spawns the server thread itself under
# #ifdef EHS_DEBUG_TCPIP_CONSOLE — no target startup code is needed.
EHS_DEBUG_TCPIP_CONSOLE      = yes

# Sockets backend behind the console. bsdsockets is plain POSIX (socket/bind/
# listen/accept, netinet/in.h, netdb.h), which Zephyr supplies via the
# CONFIG_POSIX_API=y + CONFIG_NET_SOCKETS=y already enabled for Wi-Fi — so no
# new Zephyr subsystem, just connection state. This also makes hal.c call
# EhsTCommsSys_init() at boot.
EHS_COMMS_API_SUPPORT        = bsdsockets

# Listener + one accepted connection on top of DHCP's context. ~771 B each,
# measured. Must go through this variable, not a bare CONFIG_ line — see the
# note in target/os-arch/zephyr-arm/wifi_nrf70.mk.
ERT_ZEPHYR_NET_MAX_CONTEXTS  = 5

# Console sizing. EHS_DEBUG_CONSOLE_BUFFER_SIZE must be a power of two and >=
# both TCP buffers (target_console.c and console_server.c #error otherwise).
# The TCP buffers default to 65536/61440 in bsdsockets/target_tcp.h, sized for
# desktop; that is ~124 KB of static RAM against this part's ~9 KB free, so
# they are overridden to the same 128 the lwip/ESP32 MCU backend uses. The
# Lucid protocol is short records, not bulk transfer.
DEFS += EHS_TGT_TCP_IN_BUFF_SIZE=128
DEFS += EHS_TGT_TCP_OUT_BUFF_SIZE=128
DEFS += EHS_DEBUG_CONSOLE_BUFFER_SIZE=256
# Requested stack for the console server thread. Advisory on Zephyr: EHS
# dynamic threads come from the fixed pool in target_process.c, which is
# already allocated whether used or not — so this thread costs no extra RAM.
DEFS += EHS_DEBUG_CONSOLE_THREAD_STACK_SIZE=4096

################################### END OF CONFIGURATION ###################################################
