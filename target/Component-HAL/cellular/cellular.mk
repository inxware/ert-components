#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------

# Cellular (LTE-M / NB-IoT) HAL.
#
# Two variables, mirroring the Wi-Fi split:
#   EHS_NETWORK_CELLULAR_SUPPORT  - on/off for the eRT subsystem (this file)
#   EHS_CELLULAR_SUPPORT          - which backend implements the do* contract
#
# The backend's source lives in its own subdirectory, except for the real
# hardware backends which live under target/os-arch/<os>/ alongside the rest of
# the OS port (target_cellular.c) — same arrangement as Wi-Fi, where
# wifi/wifi.mk only supplies the DEFS and the OS port carries the code.

EHS_TARGET_CELLULAR_PATH = $(EHS_TARGET_COMPONENT_HAL_PATH)/cellular
INC_DIRS += $(EHS_TARGET_CELLULAR_PATH)

DEFS += EHS_NETWORK_CELLULAR_SUPPORT

# Default backend. A platform that sets EHS_NETWORK_CELLULAR_SUPPORT without
# naming a backend gets the no-op one, which fails every call cleanly and
# advertises no capabilities (see cellular_stubbed.c).
ifndef EHS_CELLULAR_SUPPORT
	EHS_CELLULAR_SUPPORT = stubbed
endif

########################################################################################################
## Wi-Fi / cellular mutual exclusion on nRF91
########################################################################################################
# nRF91 parts cannot host both the nRF70 Wi-Fi stack and nrf_modem usefully.
# These are sibling platform variants over a shared baseline, not a target plus
# an add-on.
#
# THIS WAS ACTUALLY TRIED, so the conclusion is measured rather than assumed.
# A combined variant CAN be made to link, but only like this:
#   * Wi-Fi WPA3/SAE removed                      (-32 KB flash)
#   * modem shared memory at its Kconfig FLOOR (TX 2176 / RX 2616), capping
#     cellular throughput to telemetry rates
#   * supplicant socketpairs halved; lte_lc PSM/eDRX/coneval dropped, so
#     cell_power_manager and cell_status lose most of what they report
#   * Zephyr NET_NATIVE forced back on (Wi-Fi needs it), costing ~60 KB that a
#     cellular-only build saves by turning it off
# and it STILL landed at 93% flash / 97.5% RAM with no application loaded, with
# the k_heap tuned by trial against a live "Failed to add iface wlan0" failure.
# The siblings sit at 95% and 40%. Judged not worth carrying; variant removed.
#
# Failing here rather than at link time is the whole point: the linker's
# "region RAM overflowed by N bytes" reads like a tuning problem and sends you
# hunting for buffers to shrink. It is not one.
ifeq ($(EHS_CELLULAR_SUPPORT),zephyr_nrf91)
  ifneq ($(filter-out none,$(EHS_WIFI_SUPPORT)),)
    ifneq ($(ERT_ALLOW_WIFI_CELLULAR_COEXIST),yes)
      $(error EHS_CELLULAR_SUPPORT=zephyr_nrf91 and EHS_WIFI_SUPPORT=$(EHS_WIFI_SUPPORT) \
do not fit together usefully on nRF9151. Use the sibling variants \
zephyr_arm-nrf9151_thingy91x-wifi OR ...-cellular. Override with \
ERT_ALLOW_WIFI_CELLULAR_COEXIST = yes only if you have read the note above.)
    endif
  endif
endif

########################################################################################################
## Backend selection
########################################################################################################
# 'stubbed' is the only backend whose source lives here. Real backends are part
# of their OS port and are added to OBJECTS by that port's own makefile, exactly
# as target_wifi.c is.
#
# VPATH as well as the path-qualified object: the CMake-master Zephyr build
# resolves OBJECTS back to sources by looking the bare stem up in VPATH (see
# _ert_find_src in the top-level Makefile), so a path-qualified object alone
# would leave the source out of the Zephyr build and every doCellular* call
# unresolved at link time. Same trap documented in nvs/hal_nvs.mk.
ifeq ($(EHS_CELLULAR_SUPPORT),stubbed)
	VPATH += $(EHS_TARGET_CELLULAR_PATH)
	OBJECTS += $(EHS_TARGET_CELLULAR_PATH)/cellular_stubbed.$(OBJ)
endif
