#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# Makefile fragment to build target specific component and support code.
# Called by ../../platform/<platform-type>

#  OBJ - File extension for object files

# This flag is used for Zephyr-specific build conditionals
EHS_ZEPHYR=yes

# Zephyr targets don't build with 'make' / 'make all' / 'make all_docker' —
# those are the legacy direct-compile-and-link path, which has no Zephyr
# kernel/RTOS headers or startup code, so it would fail deep into compilation
# with a confusing missing-toolchain error. Catch it here, at parse time,
# before any recipe runs. This file is only ever included for Zephyr targets,
# so non-Zephyr builds never see this.
#
# The clean one-line guidance is printed with $(info) (the only make function
# that doesn't prepend 'file:line:'); $(error) then just halts the parse — its
# own 'file:line: *** ...  Stop.' line is unavoidable in GNU make, so we keep
# its text to a short summary and put the real message above it.
ifneq ($(filter all all_docker,$(MAKECMDGOALS))$(if $(MAKECMDGOALS),,x),)
$(info )
$(info $(TXT_FG_BRIGHT_RED)Zephyr targets don't build with 'make' / 'make all' / 'make all_docker'.$(TXT_RESET) Run '$(TXT_FG_WHITE)make zephyr_cmake_gen$(TXT_RESET)' once, then '$(TXT_FG_WHITE)make zephyr_build_docker$(TXT_RESET)' to build. See docs/llm-dev-contexts/CLAUDE-zephyr.md.)
$(info )
$(error wrong build command for a Zephyr target)
endif

# Export board identifier so shell scripts (zephyr_build.sh) can read it
# without needing to re-parse the platform config.mk.
export ERT_ZEPHYR_BOARD

# Expected variables
#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code
#  EHS_TARGET_OS_HW_PATH - path to the current directory (set by platform makefile)

# Default OS Features Supported
ifneq ($(EHS_FILESYSTEM_SUPPORT),none)
ifndef EHS_FILESYSTEM_SUPPORT
	EHS_FILESYSTEM_SUPPORT=posix
endif
endif

# Target path
_TARGET_PATH = $(EHS_TARGETS_ROOT_PATH)/os-arch/zephyr-arm/

# Include source code from this dir in build
INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/os-arch/zephyr-arm/
VPATH += $(EHS_TARGETS_ROOT_PATH)/os-arch/zephyr-arm/

# Zephyr-specific defines
DEFS += EHS_ZEPHYR_RTOS=1
DEFS += EHS_ZEPHYR_SUPPORT=1
EHS_ZEPHYR_SUPPORT=1

# Thread names in fault dumps. Without this Zephyr prints
# "Current thread: 0x20011a80 (unknown)" for every EHS dynamic thread (Wi-Fi
# station SM, Lucid console server, FB threads), which is exactly the
# information a crash report needs. EhsHThread_execute passes the name through
# to k_thread_name_set. Costs CONFIG_THREAD_MAX_NAME_LEN bytes per thread
# struct - trimmed from the default 32 since eRT's names are short.
ERT_ZEPHYR_KCONFIG += CONFIG_THREAD_NAME=y
ERT_ZEPHYR_KCONFIG += CONFIG_THREAD_MAX_NAME_LEN=16

# Add Zephyr build include paths if available
ifdef ZEPHYR_BUILD_DIR
INC_DIRS += $(ZEPHYR_BUILD_DIR)/zephyr/include/generated
endif

# Include version string
-include TARGET.cfg
ifdef TARGET
DEFS += 'TARGET_OS_VERSION_STRING="$(shell head -c -1 ./Releases/version_strings 2>/dev/null | tr '\n' '.')\x20:$(TARGET)"'
endif

# Always needed objects for Zephyr targets
OBJECTS += target_main.$(OBJ)
OBJECTS += targetos_init.$(OBJ)
OBJECTS += target_process.$(OBJ)
OBJECTS += target_time.$(OBJ)
OBJECTS += target_math.$(OBJ)

# Per-target serial-console HAL — backs Common/Ehs/serial_console.c.
# Contract: Common/HAL/include/hal_serial.h.
ifdef EHS_SERIAL_CONSOLE_SUPPORT
ifneq ($(EHS_SERIAL_CONSOLE_SUPPORT),none)
OBJECTS += target_serial.$(OBJ)
endif
endif

# Filesystem support: include target_file.c when EHS_FILESYSTEM_SUPPORT is
# set to a real value (posix or similar) — not "none" or "stubbed".
ifdef EHS_FILESYSTEM_SUPPORT
ifneq ($(EHS_FILESYSTEM_SUPPORT),none)
ifneq ($(EHS_FILESYSTEM_SUPPORT),stubbed)
OBJECTS += target_file.$(OBJ)
endif
endif
endif

# Optional objects depending on platform build
ifdef EHS_UART_SUPPORT
ifneq ($(EHS_UART_SUPPORT),none)
OBJECTS += target_uart.${OBJ}
endif
endif

ifdef EHS_GUI_SUPPORT
ifneq ($(EHS_GUI_SUPPORT),none)
OBJECTS += target_display.$(OBJ)
endif
endif

# Wi-Fi driver/Kconfig selector (see target/os-arch/zephyr-arm/wifi_nrf70.mk
# for the Kconfig side). This only says "this board has a Wi-Fi chip wired up";
# EHS_NETWORK_WIFI_SUPPORT below is what pulls in the eRT Wi-Fi station
# subsystem on top of it.
ifdef EHS_WIFI_SUPPORT
ifneq ($(EHS_WIFI_SUPPORT),none)
DEFS += EHS_WIFI_SUPPORT

# Standalone Wi-Fi connect + DHCP bring-up test (net_mgmt WIFI_CONNECT_STORED,
# credentials from CONFIG_WIFI_CREDENTIALS_STATIC_*). Superseded by the real
# HAL in target_wifi.c and OFF by default - the two both call net_if_up() and
# issue their own connect, so enabling both at once makes the SM race a
# connection it does not know about. Set EHS_ZEPHYR_WIFI_CONNECT_TEST=yes in a
# platform config.mk to bring the radio up without the eRT stack involved.
ifeq ($(EHS_ZEPHYR_WIFI_CONNECT_TEST),yes)
OBJECTS += wifi_connect_test.$(OBJ)
DEFS += EHS_ZEPHYR_WIFI_CONNECT_TEST
endif
endif
endif

# eRT Wi-Fi station HAL - the target half of the cross-target contract in
# target/Component-HAL/wifi/wifi_station.h, driving the shared connect state
# machine in Common/Components/networking/inx-wifi_station.c.
# EHS_NETWORK_WIFI_SUPPORT also gates component-hal.mk's wifi/wifi.mk include
# (which supplies the wifi_station.h include path and the
# EHS_NETWORK_WIFI_SUPPORT define) and the inx-wifi_station.c component.
ifdef EHS_NETWORK_WIFI_SUPPORT
ifneq ($(EHS_NETWORK_WIFI_SUPPORT),none)
ifeq ($(filter-out none,$(EHS_WIFI_SUPPORT)),)
$(error EHS_NETWORK_WIFI_SUPPORT is set but EHS_WIFI_SUPPORT is unset/none - the Wi-Fi station subsystem needs a driver backend (e.g. EHS_WIFI_SUPPORT = zephyr_nrf70))
endif
OBJECTS += target_wifi.$(OBJ)
# TEST_FUNC harness for the above (test_wifi_station). Compiled unconditionally
# so that 'make TEST_FUNC=test_wifi_station ...' needs no makefile change; the
# whole body is behind #ifdef EHS_TEST_FUNC_OVERRIDE, so a normal build gets an
# effectively empty object.
OBJECTS += target_wifi_test.$(OBJ)
endif
endif

# eRT cellular (LTE-M / NB-IoT) HAL - the target half of the cross-target
# contract in target/Component-HAL/cellular/cellular.h, driving the shared
# connect state machine in Common/Components/networking/inx-cellular_link.c.
# EHS_NETWORK_CELLULAR_SUPPORT also gates component-hal.mk's cellular.mk
# include (which supplies the cellular.h include path and the
# EHS_NETWORK_CELLULAR_SUPPORT define) and the inx-cellular_link.c component.
#
# Same driver/subsystem split as Wi-Fi above: EHS_CELLULAR_SUPPORT says "this
# board has a modem", EHS_NETWORK_CELLULAR_SUPPORT pulls in the eRT subsystem.
ifdef EHS_NETWORK_CELLULAR_SUPPORT
ifneq ($(EHS_NETWORK_CELLULAR_SUPPORT),none)
ifeq ($(filter-out none stubbed,$(EHS_CELLULAR_SUPPORT)),)
$(error EHS_NETWORK_CELLULAR_SUPPORT is set but EHS_CELLULAR_SUPPORT is unset/none - the cellular subsystem needs a modem backend (e.g. EHS_CELLULAR_SUPPORT = zephyr_nrf91))
endif
OBJECTS += target_cellular.$(OBJ)
# TEST_FUNC harness for the above (test_cellular). Compiled unconditionally so
# that 'make TEST_FUNC=test_cellular ...' needs no makefile change; the whole
# body is behind #ifdef EHS_TEST_FUNC_OVERRIDE, so a normal build gets an
# effectively empty object.
OBJECTS += target_cellular_test.$(OBJ)
endif
endif
