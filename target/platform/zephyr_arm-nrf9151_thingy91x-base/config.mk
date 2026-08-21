#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------

# @file config.mk
# eRT BASELINE platform for Nordic Thingy:91 X — nRF9151 application core
# (Cortex-M33 +FPU, 256 KB RAM, 1 MB Flash).
#
# BOARD SUPPORT ONLY: UART console, GPIO, LittleFS on external flash, SODL app
# loading. NO RADIO of any kind.
#
# This is the shared parent of two sibling variants:
#
#   zephyr_arm-nrf9151_thingy91x-wifi      nRF7002 Wi-Fi 6 station
#   zephyr_arm-nrf9151_thingy91x-cellular  nRF9151 LTE-M / NB-IoT modem
#
# WHY TWO VARIANTS RATHER THAN ONE TARGET WITH BOTH
# --------------------------------------------------
# They do not fit together. Enabling CONFIG_NRF_MODEM_LIB alongside the nRF70
# Wi-Fi stack overflows this part's RAM by a measured 57,076 bytes. That is not
# a tuning problem: the modem's shared memory is an address-constrained linker
# carve-out (~17.8 KB that must live in the lowest 128 KB of RAM), and the
# Wi-Fi side is already at ~95% flash / ~90% RAM on its own. The combination is
# structurally impossible, so Wi-Fi and cellular are peer products over this
# shared baseline, not a target plus an add-on.
#
# A build-time guard in target/Component-HAL/cellular/cellular.mk rejects any
# config that sets both, so the failure is a clear message at configure time
# rather than a linker region-overflow ten minutes into a build.
#
# THIS TARGET IS BUILDABLE AND USEFUL IN ITS OWN RIGHT
# ----------------------------------------------------
# Not an abstract parent. It gives CI a radio-free regression point, and it is
# the measurement baseline that makes each variant's radio cost attributable by
# subtraction (see docs/_working_notes/ert-cellular-plan.md § 8).
#
# Build method: Zephyr-as-master CMake build via 'make zephyr_cmake_gen' +
#               'make zephyr_build_docker', sourced from Nordic's nRF Connect
#               SDK (NCS) rather than upstream Zephyr — NCS is required for the
#               nRF91 cellular stack and the nRF70 Wi-Fi driver, neither of
#               which exists upstream.

#################################################################################################################
# Baseline Configuration Includes
#################################################################################################################

# (No parent config — this is the base configuration.)

#################################################################################################################
# Architecture, OS, Toolchain
#################################################################################################################

EHS_ARCH          = arm
EHS_OS            = zephyr
TOOLCHAIN_NAME    = arm-zephyr-eabi

# EHS_TARGET_LIB_VARIANT selects the matching EHS-kernel build in ert-build-support:
#   arm-zephyr-cortexm33/kernel/libehs_ehrt1.a
# The nRF9151 core is Cortex-M33, ABI-identical to the nRF5340 cpuapp, so the
# existing cortexm33 kernel archive is reused unchanged by all three targets —
# no kernel rebuild is needed for either radio.
EHS_TARGET_LIB_VARIANT = -cortexm33

EHS_MCU_TARGET    = yes

# Use the pre-built kernel library (from EHS-kernel/zephyr_arm_cortexm33_ehrt1).
# This excludes target/os-arch/zephyr-arm/ehs_kernel_stub.c from the build.
EHS_BUILD_MONOLITHIC_KERNEL = yes


#################################################################################################################
# Zephyr / nRF Connect SDK build parameters
#################################################################################################################

# Board identifier passed to 'west build -b <ERT_ZEPHYR_BOARD>'.
# Non-secure (TF-M) variant. Originally adopted because CONFIG_WIFI_NRF70
# silently resolves to n on the secure single-image variant despite every
# devicetree/Kconfig dependency being satisfied; kept for all three targets
# because the /ns variant is also what nrf_modem expects on nRF91, and having
# one board identifier across the family keeps the partition layout shared.
ERT_ZEPHYR_BOARD = thingy91x/nrf9151/ns

# NCS release checked out via 'west init -m <ERT_ZEPHYR_MANIFEST> --mr <ERT_ZEPHYR_VERSION>'.
# Must match the sdk-nrf-toolchain Docker image tag in Dockerimagename.
# Pinned to v2.9.0 (not the latest) because it is the confirmed-working
# nRF9151+nRF7002 combination and because every Kconfig symbol name in
# wifi_nrf70.mk and cellular_nrf91.mk is pinned to this version's spelling.
# Bumping it requires re-diffing both against that release's own samples.
ERT_ZEPHYR_VERSION = v2.9.0

# Build the MCUboot multi-image (sysbuild): signs the app and emits merged.hex +
# dfu_application.zip, so the board can be flashed over USB/MCUboot (no probe).
ERT_ZEPHYR_SYSBUILD = y

# West manifest repository. A non-empty value is also the signal that flags this
# as an NCS target (the sdk-nrf-toolchain image has an
# ENTRYPOINT ["/bin/bash","-c"] that needs the container command passed as a
# single string — handled in zephyr_build_docker.sh).
ERT_ZEPHYR_MANIFEST = https://github.com/nrfconnect/sdk-nrf

# CPU-specific flags for the nRF9151 Cortex-M33 core (used only if the
# direct-compile path is ever activated; ignored by the CMake-master path).
ERT_ZEPHYR_CPU_FLAGS = -mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16

#################################################################################################################
# Core Kconfig
#################################################################################################################

ERT_ZEPHYR_KCONFIG  = CONFIG_POSIX_API=y
ERT_ZEPHYR_KCONFIG += CONFIG_FPU=y
ERT_ZEPHYR_KCONFIG += CONFIG_REBOOT=y
ERT_ZEPHYR_KCONFIG += CONFIG_SERIAL=y
ERT_ZEPHYR_KCONFIG += CONFIG_UART_CONSOLE=y
ERT_ZEPHYR_KCONFIG += CONFIG_UART_INTERRUPT_DRIVEN=y
ERT_ZEPHYR_KCONFIG += CONFIG_GPIO=y
ERT_ZEPHYR_KCONFIG += CONFIG_PRINTK=y
ERT_ZEPHYR_KCONFIG += CONFIG_LOG=y
ERT_ZEPHYR_KCONFIG += CONFIG_LOG_BACKEND_UART=y

# Radio-free default. Both variants override this from their own shared
# Kconfig fragment (wifi_nrf70.mk raises it to 60000; cellular_nrf91.mk sets
# 16384), and those are included AFTER the platform config so they win.
ERT_ZEPHYR_KCONFIG += CONFIG_HEAP_MEM_POOL_SIZE=16384

# main() only spawns the EHS thread (sized independently in target_main.c), so
# this does not need to be large — but Zephyr's 1024 default is tight once the
# boot path touches the filesystem. Set here rather than per-variant because
# main() is identical in all three. wifi_nrf70.mk sets the same value for the
# Wi-Fi variant, so there is no conflict.
ERT_ZEPHYR_KCONFIG += CONFIG_MAIN_STACK_SIZE=2048

# Level 2 = warnings and errors: driver faults still surface, routine chatter
# does not. Do NOT try to recover RAM by switching to deferred logging —
# measured, deferred mode costs ~2 KB MORE because it allocates a log ring
# buffer that immediate mode does not need. Immediate mode is synchronous:
# slower, but no dropped messages.
ERT_ZEPHYR_KCONFIG += CONFIG_LOG_DEFAULT_LEVEL=2
ERT_ZEPHYR_KCONFIG += CONFIG_LOG_MODE_IMMEDIATE=y

# --- Filesystem: LittleFS on external flash, for loading the SODL app (t.sdl) ---
# The kernel opens <install-root>/appdata/default/t.sdl; target_file.c mounts
# LittleFS at /lfs and sets the install root to /lfs. Partition littlefs_storage
# is defined in zephyr/pm_static.yml (external flash, after settings_storage).
ERT_ZEPHYR_KCONFIG += CONFIG_FLASH=y
ERT_ZEPHYR_KCONFIG += CONFIG_FLASH_MAP=y
ERT_ZEPHYR_KCONFIG += CONFIG_SPI=y
ERT_ZEPHYR_KCONFIG += CONFIG_SPI_NOR=y
# Use the SFDP table from the flash_ext DTS node (sfdp-bfp) and a 4 KB erase page
# to match the GD25LE255E sector size — without these the external-flash
# erase/write failed ("Superblock unwritable"), only reads worked.
ERT_ZEPHYR_KCONFIG += CONFIG_SPI_NOR_SFDP_DEVICETREE=y
ERT_ZEPHYR_KCONFIG += CONFIG_SPI_NOR_FLASH_LAYOUT_PAGE_SIZE=4096
# Keep the MCUboot secondary slot on external flash (matches pm_static.yml).
ERT_ZEPHYR_KCONFIG += CONFIG_PM_EXTERNAL_FLASH_MCUBOOT_SECONDARY=y
ERT_ZEPHYR_KCONFIG += CONFIG_FILE_SYSTEM=y
ERT_ZEPHYR_KCONFIG += CONFIG_FILE_SYSTEM_LITTLEFS=y
# File-descriptor table. The default (POSIX_OPEN_MAX=3, ZVFS_OPEN_MAX=0) leaves
# no fds for opening files once stdio takes stdin/out/err — the SODL fopen/fgetc
# then can't get a descriptor. ZVFS_OPEN_MAX is the settable fd-table size;
# POSIX_OPEN_MAX derives from it (it is promptless — assigning it directly is a
# Kconfig error). 16 is ample for console + SODL + sys files with no network;
# the Wi-Fi variant raises it because the net/socket subsystem enforces a
# minimum of 37 with wpa_supplicant enabled.
ERT_ZEPHYR_KCONFIG += CONFIG_ZVFS_OPEN_MAX=16

# The nRF91 modem library. OFF here so that the Wi-Fi variant inherits it off
# (it does not fit alongside the nRF70 stack — see the header of this file) and
# only the cellular variant turns it on. Putting it here rather than in a board
# .conf avoids the merge-order trap: board .conf files merge AFTER prj.conf, so
# an override placed there silently wins over anything a variant sets.
ERT_ZEPHYR_KCONFIG += CONFIG_NRF_MODEM_LIB=n


#################################################################################################################
# Feature flags
#################################################################################################################

EHS_GUI_SUPPORT              = none
EHS_AV_SUPPORT               = none
EHS_MEDIA_SUPPORT            = none
EHS_NETWORKING_SUPPORT       = none
EHS_ML_SUPPORT               = none
EHS_MV_SUPPORT               = none

EHS_FILESYSTEM_SUPPORT       = posix
# Gates compiling+registering Common/Components/peripherals/ (gpio_out, etc.) at
# all — without it the blocks build fine standalone but are never added to the
# toolkit table, so a SODL referencing gpio_out fails to load with "FB ID ... N/A".
EHS_PERIPHERAL_DEVICE_SUPPORT = all
EHS_PERIPHERALS_GPIO_SUPPORT = zephyr
EHS_PERIPHERALS_ADC_DAC_SUPPORT = none
# The generic inx-uart function block (Common/Components/peripherals/inx-uart.c)
# has no target/Component-HAL/uart/zephyr/ backend yet (only esp32s3/linux/win32/
# qnx/nxp-redlib/stubbed exist) — not the same thing as the console UART, which
# already works. Leave disabled until that HAL is written; follow-up work.
EHS_UART_SUPPORT             = none

# Required by both radio variants: it is EHS_COMPONENT_NETWORKING_SUPPORT that
# both compiles the networking component group and registers
# EhsBlockRefTable_networking with the toolkit (statmodules.c) — without it a
# wifi_station or cellular_link block builds but no SODL can reference it
# ("FB ID ... N/A"). Harmless here: every other entry in that group has its own
# additional gate (EHS_MQTT_SUPPORT, EHS_NETWORK_WIFI_SUPPORT, ...), all off in
# this baseline, so it pulls in only the ~100-byte group registration object.
# 'no-curl' matches the value the other MCU targets use — libcurl is not
# available on Zephyr.
EHS_COMPONENT_NETWORKING_SUPPORT = no-curl

# libexpat is not available on Zephyr; exclude the XML stream component
EHS_EXCLUDE_XML_PARSER       = yes

# inx-application_info_getter uses POSIX opendir/readdir not available on Zephyr
EHS_SKIP_APPLICATION_INFO_GETTER = yes

ERT_SODL_VERSION = 1
EHS_DEBUGALL     = true
EHS_RUNTIME_LOGGER_ENABLED = yes

# MUST be set explicitly, and 'stubbed' is the only correct value for a
# radio-free baseline.
#
# EHS_DEBUGALL=true above force-enables the Lucid TCP console:
# Common/Ehs/ehs.mk does `ifdef EHS_DEBUGALL ... EHS_DEBUG_TCPIP_CONSOLE=yes`
# unless the value is already 'stubbed' or 'target_specific'. Leaving it unset
# therefore pulls in console_server.c and the EhsSvcTcp_* call sites in hal.c —
# which then fail to build here because there is no network interface and hence
# no EHS_COMMS_API_SUPPORT backend to supply EhsTgtTcpSocketType. This target is
# the first radio-free Zephyr platform, so it is the first to hit that.
#
# 'stubbed' takes tcp.mk's else-branch: stubbed_console.o only, and crucially
# no `DEFS += EHS_DEBUG_TCPIP_CONSOLE`, so hal.c's #ifdef is false too.
# The Wi-Fi variant overrides this back to 'yes'; the cellular variant
# deliberately inherits 'stubbed' (inbound TCP is NAT-blocked on most cellular
# APNs, and the link is metered).
EHS_DEBUG_TCPIP_CONSOLE = stubbed

EHS_DEFAULT_APP  = tutorials/blinky


#################################################################################################################
# Preprocessor definitions
#################################################################################################################

DEFS += EHS_MCU_TARGET=1
DEFS += ZEPHYR_NRF9151=1
DEFS += EHS_FLOAT_AS_FLOAT_TYPE=1
# TEMP diagnostic: log the exact path Ehs_AppFopen opens (SODL load path debug).
DEFS += EHS_DEBUG_APP_PATH=1

# --- RAM: reclaim the unused half of the EHS dynamic-thread pool -------------
# target_process.c allocates EHS_ZEPHYR_MAX_DYNAMIC_THREADS x 4096 bytes of BSS
# unconditionally, used or not. The default 8 slots = 32 KB; this family spawns
# at most two (a radio state machine and the Lucid console server), so six were
# pure waste. That matters more than it looks: with
# CONFIG_COMMON_LIBC_MALLOC_ARENA_SIZE=-1 the arena is whatever RAM is left
# over, so every idle slot was subtracted directly from the heap the EHS kernel
# uses to parse a SODL — which is why loading even a 321-byte blinky failed
# with "**Error: Out of memory".
#
# 4 leaves two spare slots for app function-block threads. Exhaustion is
# graceful (EhsHThread_execute logs "no free thread slots" and returns
# EHS_FALSE), so raise this if an app needs more threads — at 4 KB each.
DEFS += EHS_ZEPHYR_MAX_DYNAMIC_THREADS=4

################################### END OF CONFIGURATION ###################################################
