#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------

# Nordic nRF91-series cellular (LTE-M / NB-IoT) Kconfig for NCS-based Zephyr
# targets. Included from target/os-arch/zephyr-arm/config.mk when
# EHS_CELLULAR_SUPPORT=zephyr_nrf91 — the platform config.mk carries only the
# on/off selector, so this list is maintained once and shared across every
# nRF91 board rather than copy-pasted per platform. Exactly the arrangement
# used by wifi_nrf70.mk.
#
# SYMBOL NAMES ARE PINNED TO NCS v2.9.0. lte_lc was refactored across
# 2.6 -> 3.x: lte_connectivity became lte_net_if, the monolithic library was
# split into the per-feature CONFIG_LTE_LC_*_MODULE symbols used below, and
# lte_lc_reduced_mobility_* / lte_lc_factory_reset were deprecated in 2.8.0.
# If ERT_ZEPHYR_VERSION is ever bumped, re-diff against that version's own
# nrf/samples/cellular/*/prj.conf before assuming these names still apply.

########################################################################################################
## Modem library
########################################################################################################
# The board .conf for the Wi-Fi variant sets CONFIG_NRF_MODEM_LIB=n because
# nrf_modem does not fit alongside the nRF70 stack. The cellular variant's
# board .conf must NOT carry that line — board conf merges AFTER prj.conf and
# would silently win. See target/platform/<...>-cellular/zephyr/boards/.
ERT_ZEPHYR_KCONFIG += CONFIG_NRF_MODEM_LIB=y

# Modem shared memory. These are the NCS defaults, restated explicitly because
# they are the single largest RAM item in a cellular build (~17.8 KB) and the
# region MUST live in the lowest 128 KB of RAM — it is an address-constrained
# linker carve-out, not a heap allocation, and cannot be traded against
# anything else. Trim TX/RX if throughput allows; CTRL is fixed.
#   CTRL 0x4e8 (1256 B, not settable)  TX 8320  RX 8192  TRACE 0
ERT_ZEPHYR_CELL_SHMEM_TX ?= 8320
ERT_ZEPHYR_CELL_SHMEM_RX ?= 8192
ERT_ZEPHYR_KCONFIG += CONFIG_NRF_MODEM_LIB_SHMEM_TX_SIZE=$(ERT_ZEPHYR_CELL_SHMEM_TX)
ERT_ZEPHYR_KCONFIG += CONFIG_NRF_MODEM_LIB_SHMEM_RX_SIZE=$(ERT_ZEPHYR_CELL_SHMEM_RX)

# Modem trace costs a further 16 KB of RAM for the trace region plus ~9.6 KB of
# flash (libmodem_log.a instead of libmodem.a). Off, deliberately — do not
# enable casually on a 256 KB part.
ERT_ZEPHYR_KCONFIG += CONFIG_NRF_MODEM_LIB_TRACE=n

# Let the application decide what to do about a modem fault rather than
# silently rebooting under us.
ERT_ZEPHYR_KCONFIG += CONFIG_NRF_MODEM_LIB_ON_FAULT_APPLICATION_SPECIFIC=y

########################################################################################################
## LTE link control
########################################################################################################
ERT_ZEPHYR_KCONFIG += CONFIG_LTE_LINK_CONTROL=y

# Per-feature modules. Each is an independent size lever (figures are estimates
# from source size; see docs/_working_notes/ert-cellular-plan.md § 8.3):
#   core + cereg + cfun + xsystemmode  ~8.1 KB   always built, no symbol
#   psm         ~2.45 KB  power saving
#   edrx        ~2.6 KB   extended DRX
#   modem_sleep ~0.75 KB  the PSM enter/exit events the SM needs
#   coneval     ~0.8 KB   pre-transmit link evaluation
#   ncellmeas   ~4.35 KB  DEFERRED - cell-based location, needs a cloud service
#
# The EXACT set of symbols that exists in v2.9.0 is:
#   LTE_LC_CONN_EVAL_MODULE   LTE_LC_EDRX_MODULE
#   LTE_LC_MODEM_SLEEP_MODULE LTE_LC_NEIGHBOR_CELL_MEAS_MODULE
#   LTE_LC_PERIODIC_SEARCH_MODULE LTE_LC_PSM_MODULE
#   LTE_LC_RAI_MODULE         LTE_LC_TAU_PRE_WARNING_MODULE
# There is NO LTE_LC_MODEM_EVENTS_MODULE and no LTE_LC_CSCON_MODULE in this
# release - registration status (+CEREG), functional mode (+CFUN), system mode
# (%XSYSTEMMODE), RRC state (+CSCON) and modem events (%MDMEV) are all part of
# the always-built core. Assigning a value to a symbol Kconfig does not know
# aborts the build ("attempt to assign the value 'y' to the undefined symbol"),
# which is how this list was corrected.
#
# CRITICAL: struct lte_lc_evt's union members are individually #if'd on these
# symbols in lte_lc.h. Turning one off does not merely disable a feature - it
# removes the union member, and target_cellular.c's matching #if guard then
# compiles that event handler out. The two stay in step because both key off
# the same CONFIG_LTE_LC_*_MODULE name.
# ERT_CELL_MINIMAL_LTE_LC=yes drops the optional modules (~6.6 KB of flash) for
# builds that cannot afford them - notably the Wi-Fi+cellular combo variant.
# The cost is real and visible at runtime: cell_power_manager loses PSM/eDRX
# entirely, cell_status can never report evalOk=true, and the state machine
# never reaches EhsCell_PsmSleep because the notifications that drive it are
# not compiled. target_cellular.c's #if guards key off the same symbols, so the
# code compiles out in step rather than calling into absent functions.
ERT_CELL_MINIMAL_LTE_LC ?= no
ifneq ($(ERT_CELL_MINIMAL_LTE_LC),yes)
ERT_ZEPHYR_KCONFIG += CONFIG_LTE_LC_PSM_MODULE=y
ERT_ZEPHYR_KCONFIG += CONFIG_LTE_LC_EDRX_MODULE=y
ERT_ZEPHYR_KCONFIG += CONFIG_LTE_LC_CONN_EVAL_MODULE=y
endif

# The sleep MODULE alone is not enough. It compiles the code, but
# LTE_LC_EVT_MODEM_SLEEP_ENTER / _EXIT are only emitted when NOTIFICATIONS is
# also set - without it target_cellular.c's handler is built and never called,
# so the SM would never enter EhsCell_PsmSleep and a device in PSM would look
# like one that had silently stopped responding. Exactly the "capability
# present but nothing happens" failure the contract warns about.
ifneq ($(ERT_CELL_MINIMAL_LTE_LC),yes)
ERT_ZEPHYR_KCONFIG += CONFIG_LTE_LC_MODEM_SLEEP_MODULE=y
ERT_ZEPHYR_KCONFIG += CONFIG_LTE_LC_MODEM_SLEEP_NOTIFICATIONS=y
endif

# Off: ~4.35 KB of flash for a feature that needs a cloud location service to
# be useful. Enable together with a cellular_location function block.
ERT_ZEPHYR_KCONFIG += CONFIG_LTE_LC_NEIGHBOR_CELL_MEAS_MODULE=n

# Default RAT. Derived from ERT_CELL_RAT in the platform config.mk rather than
# hardcoded, so a board can ship NB-IoT-first without editing this shared file.
# The eRT layer can still change it at runtime via Cell_SetRat().
ERT_CELL_RAT ?= ltem
ifeq ($(ERT_CELL_RAT),ltem)
    ERT_ZEPHYR_KCONFIG += CONFIG_LTE_NETWORK_MODE_LTE_M=y
    ERT_ZEPHYR_KCONFIG += CONFIG_LTE_MODE_PREFERENCE_LTE_M=y
else ifeq ($(ERT_CELL_RAT),nbiot)
    ERT_ZEPHYR_KCONFIG += CONFIG_LTE_NETWORK_MODE_NBIOT=y
    ERT_ZEPHYR_KCONFIG += CONFIG_LTE_MODE_PREFERENCE_NBIOT=y
else ifeq ($(ERT_CELL_RAT),both)
    ERT_ZEPHYR_KCONFIG += CONFIG_LTE_NETWORK_MODE_LTE_M_NBIOT=y
    ERT_ZEPHYR_KCONFIG += CONFIG_LTE_MODE_PREFERENCE_AUTO=y
else
    $(error ERT_CELL_RAT=$(ERT_CELL_RAT) is not one of: ltem, nbiot, both)
endif

########################################################################################################
## Modem information (identity + diagnostics)
########################################################################################################
# ~7 KB of flash, and the source for IMEI / ICCID / IMSI / operator / IP /
# band / RSRP / SNR / data counters. Optional: target_cellular.c degrades
# gracefully with #if defined(CONFIG_MODEM_INFO) around every use.
#
# WARNING: do NOT use modem_info_params_get() / struct modem_param_info. That
# struct is ~2,372 bytes (21 x lte_param, each carrying a char[100]) — on a
# part whose malloc arena is single-digit kilobytes that is not a rounding
# error. The individual modem_info_string_get() / typed getters with a
# caller-supplied buffer are what target_cellular.c uses.
ERT_ZEPHYR_KCONFIG += CONFIG_MODEM_INFO=y
ERT_ZEPHYR_KCONFIG += CONFIG_MODEM_INFO_ADD_NETWORK=y
ERT_ZEPHYR_KCONFIG += CONFIG_MODEM_INFO_ADD_SIM=y
ERT_ZEPHYR_KCONFIG += CONFIG_MODEM_INFO_ADD_DEVICE=y
ERT_ZEPHYR_KCONFIG += CONFIG_MODEM_INFO_ADD_DATE_TIME=n

########################################################################################################
## AT interface
########################################################################################################
# nrf_modem_at_printf/scanf come with CONFIG_NRF_MODEM_LIB; no extra symbol is
# needed. What we explicitly do NOT enable is the AT shell — exposing a raw AT
# console lets anything rewrite band masks, PIN state and the modem credential
# store, which is a support and security liability on a shipped device.
ERT_ZEPHYR_KCONFIG += CONFIG_AT_HOST_LIBRARY=n

########################################################################################################
## Networking
########################################################################################################
# On nRF91 the IP stack lives IN THE MODEM and BSD sockets are offloaded to it.
# Existing eRT socket code (EHS_COMMS_API_SUPPORT=bsdsockets, inx-netsocket,
# mqtt_*, url_get) uses plain POSIX calls and should work unchanged — that is
# the point of offloading. CONFIG_NET_NATIVE=n keeps Zephyr's own TCP/IP stack
# out of the image, which is the large flash saving cellular gets over Wi-Fi.
#
# Note that ERT_ZEPHYR_NET_MAX_CONTEXTS (a native-stack knob) is largely moot
# here; the modem enforces its own socket limit instead (8 on nRF91, with TLS
# sockets counting against it).
ERT_ZEPHYR_KCONFIG += CONFIG_NETWORKING=y
ERT_ZEPHYR_KCONFIG += CONFIG_NET_SOCKETS=y
ERT_ZEPHYR_KCONFIG += CONFIG_NET_SOCKETS_OFFLOAD=y
ERT_ZEPHYR_KCONFIG += CONFIG_NET_IPV4=y
ERT_ZEPHYR_KCONFIG += CONFIG_NET_IPV6=n

# CONFIG_NET_NATIVE=n is the single biggest saving a cellular-only build gets:
# the modem owns the IP stack, so Zephyr's own is dead weight. It CANNOT be
# turned off when Wi-Fi is also in the image - the nRF70 driver is an ordinary
# L2 Ethernet device and needs the native stack. Hence the guard.
ifneq ($(filter-out none,$(EHS_WIFI_SUPPORT)),)
$(info cellular_nrf91.mk: Wi-Fi also enabled - keeping CONFIG_NET_NATIVE=y (costs ~60 KB flash))
else
ERT_ZEPHYR_KCONFIG += CONFIG_NET_NATIVE=n
endif

########################################################################################################
## Heap
########################################################################################################
# Far smaller than the Wi-Fi variant's 60000: there is no wpa_supplicant, no
# nRF70 RX buffer pool and no socketpair control plane to feed. The modem's own
# working memory is the shared-memory carve-out above, which is separate.
# Raise via ERT_ZEPHYR_CELL_HEAP_EXTRA if an allocation failure appears.
# Both this file and wifi_nrf70.mk set CONFIG_HEAP_MEM_POOL_SIZE, and this one
# is included LAST - so on a combo build its smaller value would silently win
# and starve wpa_supplicant. ERT_ZEPHYR_CELL_HEAP_OVERRIDE lets such a platform
# state the binding figure explicitly rather than relying on include order.
ERT_ZEPHYR_CELL_HEAP_EXTRA ?= 0
ifdef ERT_ZEPHYR_CELL_HEAP_OVERRIDE
ERT_ZEPHYR_KCONFIG += CONFIG_HEAP_MEM_POOL_SIZE=$(ERT_ZEPHYR_CELL_HEAP_OVERRIDE)
else
ERT_ZEPHYR_KCONFIG += CONFIG_HEAP_MEM_POOL_SIZE=$(shell expr 16384 + $(ERT_ZEPHYR_CELL_HEAP_EXTRA))
endif
