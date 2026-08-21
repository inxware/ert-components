#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

#
# Makefile fragment to build the common kernel files for EHS.

# Called by ../../Makefile
#

# @author: inx limited
#
# Predefined variables

#  OBJ - File extension for object files

#  NOREV - Defined if no revision number checking is to be performed
# Expected variables

#  OBJECTS - list of object files added by this makefile
#  VPATH - where to look for source code


include $(EHS_COMMON_EHS_PATH)/deps.mk

.PHONY :

revision.h : .PHONY
	@echo Get build number...
#	@cscript //Nologo $(SCRIPT_PATH)/GetVersionNum.vbs $(REVOPT) -T$(EHS_COMMON_KERNEL_PATH)/revision_template.h -O$(EHS_COMMON_KERNEL_PATH)/revision.h $(EHS_ROOT_PATH)

timer.h : target.h callback_queue.h

# The baisc paths for all configs:
VPATH+=$(EHS_COMMON_EHS_PATH)
VPATH+=$(EHS_COMMON_KAPI_PATH)

########################################################################################################
## Serial Console (Used to configure networks etc)
########################################################################################################
ifdef EHS_SERIAL_CONSOLE_SUPPORT
ifneq ($(EHS_SERIAL_CONSOLE_SUPPORT),none)
  DEFS += EHS_SERIAL_CONSOLE_SUPPORT
  OBJECTS+= banner.$(OBJ)
  OBJECTS+= serial_console.$(OBJ)
endif
endif

# Set up any debugging settings.
ifdef EHS_DEBUGALL
	ifneq ($(EHS_DEBUG_TCPIP_CONSOLE),target_specific)
	ifneq ($(EHS_DEBUG_TCPIP_CONSOLE),stubbed)
		EHS_DEBUG_TCPIP_CONSOLE=yes
	endif
	endif
	ifeq ($(EHS_RUNTIME_LOGGER_ENABLED),no)
		undefine EHS_RUNTIME_LOGGER_ENABLED
	else
		EHS_RUNTIME_LOGGER_ENABLED=yes
	endif
	# Runtime only: EhsHProcess_getStackRemaining(), the pre-format-call stack check
	# used by EHSH_LOG_* (see hal_logger.c EhsHLogger_stackOk()). Same
	# on/off-with-override pattern as EHS_RUNTIME_LOGGER_ENABLED above. It has a real
	# (small) per-EHSH_LOG_* cost, which is why it is not on for every build.
	# The build-time .su dumps are a separate opt-in - see EHS_STACK_USAGE_DUMP below.
	ifeq ($(EHS_STACK_MONITORING_ENABLED),no)
		undefine EHS_STACK_MONITORING_ENABLED
	else
		EHS_STACK_MONITORING_ENABLED=yes
	endif
	EHS_DEBUG_AV=yes
	# Assume we will always want to define this build as a debug build and upload and manage on Devamn accordingly
	export BUILD_MODE=debug
endif

ifeq ($(EHS_STACK_MONITORING_ENABLED),yes)
	DEFS += EHS_STACK_MONITORING_ENABLED
endif

# Build-time per-function stack frame dumps (-fstack-usage, one .su per object).
#
# Off unless a platform asks for it. Deliberately NOT implied by EHS_DEBUGALL or
# by EHS_STACK_MONITORING_ENABLED: the dumps are a measurement aid wanted on one
# or two targets per os-arch, not on every debug build, and not every toolchain
# has the flag - i686 gcc 4.4.6 (the linux_x86 ambifier / gtk platforms) rejects
# it outright and fails the build.
#
# To enable, add to the config.mk of a platform whose toolchain is known to
# support it:
#
#     EHS_STACK_USAGE_DUMP=yes
#
# Read the resulting .su files with:
#
#     scripts/software-utilities/stack_report.sh --prebuilt
#
# Enabling it on a toolchain that lacks the flag is a build error, by design -
# the platform config asked for something the compiler cannot do.
ifeq ($(EHS_STACK_USAGE_DUMP),yes)
	CFLAGS += -fstack-usage
endif

# Enable the TCPIP connection to tools for debugging and app upload
ifdef EHS_DEBUG_TCPIP_CONSOLE
	ifeq ($(EHS_DEBUG_TCPIP_CONSOLE),none)
		EHS_DEBUG_TCPIP_CONSOLE=stubbed
	endif
else
	EHS_DEBUG_TCPIP_CONSOLE=stubbed
endif
include $(EHS_TARGET_COMPONENT_HAL_PATH)/comms/tcp_server_common/tcp.mk

#TODO2025 - the following should be moved to a HAL make file.
ifdef EHS_COMMS_API_SUPPORT
	ifneq ($(EHS_COMMS_API_SUPPORT),none)
		EHS_TARGET_COMMS_API_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/comms/$(EHS_COMMS_API_SUPPORT)
		DEFS+=EHS_COMMS_API_SUPPORT
		INC_DIRS+=$(EHS_TARGET_COMMS_API_PATH)
		include $(EHS_TARGET_COMMS_API_PATH)/comms.mk
	endif
endif


# Enable logging to the device's lcal (stdio) logging


#todo this should probably go in the AV module
ifdef EHS_DEBUG_AV
	DEFS += EHS_DEBUG_AV
endif

# This is the very verbose that you will not want to accidently build into anything you release.
ifdef EHS_DEBUG_TRACE
# For specific messages
    DEFS += EHS_BUILDOPT_STDIO_MESSAGE_TRACE
# For legacy tracing @todo remove the argument number specificity
    DEFS += EHS_BUILDOPT_STDIO_ENABLE_FUNCTION_TRACING
# For trace debugging.
	DEFS += EHS_RUNTIME_LOGGER_ENABLED
else
	ifdef EHS_RUNTIME_LOGGER_ENABLED
		DEFS += EHS_RUNTIME_LOGGER_ENABLED
	endif
endif

# for any traget that do not need to store arguments passed to the main
ifdef EHS_TARGET_NO_MAIN_ARGS
	DEFS+=EHS_TARGET_NO_MAIN_ARGS=1
endif

# Only reboot target while loading a new app
ifeq ($(EHS_TARGET_APPLOAD_RESTARTING_REBOOT),yes)
	DEFS += EHS_TARGET_APPLOAD_RESTARTING_REBOOT
endif

# Deny (delete + try-revert-to-previous) the currently-selected app on the boot
# following a software crash (panic, task watchdog, interrupt watchdog, other
# watchdog) — see docs/ert-porting-guide.md § "Kernel Behaviour → Crash-auto-delete".
#
# Default policy: 
# !!!! TODO2026 - this is in the wrong place and usaly overomplex crud from claude. 
# !!!!! just add the default EHS_APP_DELETE_ON_CRASH to each os-arch and allow overrides for paltforms like we do for everything else.
# THen delete this nonsense:
#   - MCU / RTOS targets (ESP32, ESP32-S3, Zephyr, Arduino, NXP FreeRTOS, xcore):
#     default ON. These platforms typically run headless with a single app, no
#     supervisor to auto-restart, and no convenient remote log viewer — a broken
#     app upload would otherwise brick the device with no recovery path.
#   - Linux / Windows / QNX / Android targets: default OFF. A supervisor
#     (systemd, Windows service, Android ActivityManager, launchd) can handle
#     crash recovery, and a developer can inspect logs post-mortem; auto-deleting
#     the app would make crash diagnosis harder.
#
# Explicit target config wins: set EHS_APP_DELETE_ON_CRASH=no (or =yes) in a
# config.mk to override the default.
ifeq ($(EHS_APP_DELETE_ON_CRASH),)
	EHS_MCU_OS_LIST := arduino esp32_freertos esp32s3_freertos nxp-redlib-freertos xcore_freertos zephyr
	ifneq (,$(filter $(EHS_OS),$(EHS_MCU_OS_LIST)))
		EHS_APP_DELETE_ON_CRASH = yes
	endif
endif
# !!!!! End of  crap to delelte when done properly

# Master switch for ALL automatic app-deny behaviour on boot.
#
# When EHS_APP_AUTO_DELETE=no (the default for now, while the feature is being
# stabilised) BOTH of the following kernel-side paths are forced off:
#
#   1. Reset-reason crash-auto-delete (gated by EhsHShouldDeleteAppForCrashReason)
#   2. Failed-boot detection deny     (gated by EhsHFailedBootShouldDenyApp)
#
# The kernel still calls into the gates; they unconditionally return FALSE so
# no app ever gets denied at boot. This is purely an ert-components-side
# disable — no kernel rebuild needed.
#
# A target platform that wants the feature back on (e.g. once it's been
# validated for that platform) sets EHS_APP_AUTO_DELETE=yes in its config.mk.
# The two finer-grained flags (EHS_APP_DELETE_ON_CRASH, EHS_APP_FAILED_BOOT_LIMIT)
# only have effect when EHS_APP_AUTO_DELETE=yes.
#
# TODO 2026 — re-enable per platform once the boot-confirm window and
# crash-reason classifier are working reliably across all targets.
ifeq ($(EHS_APP_AUTO_DELETE),)
	EHS_APP_AUTO_DELETE = no
endif
ifeq ($(EHS_APP_AUTO_DELETE),yes)
	DEFS += EHS_APP_AUTO_DELETE
endif

ifeq ($(EHS_APP_DELETE_ON_CRASH),yes)
	DEFS += EHS_APP_DELETE_ON_CRASH
endif

# Maximum number of unconfirmed app loads tolerated before the app is denied.
# A boot is "unconfirmed" if the load-started flag set by the kernel just
# before SODL parse is still present at the next boot — i.e. the app didn't
# survive the >5 s confirm window (silent hang, power loss during init, hard
# reset). On reaching this count the kernel calls EhsAppDenyApp(current),
# the same primitive used by crash-auto-delete.
#
# Default 1 = deny on first failure (parity with crash-auto-delete).
# Dev / debug targets can set higher in config.mk to allow the app to be
# retried across several restarts before being denied.
ifeq ($(EHS_APP_FAILED_BOOT_LIMIT),)
	EHS_APP_FAILED_BOOT_LIMIT = 1
endif
DEFS += EHS_APP_FAILED_BOOT_LIMIT=$(EHS_APP_FAILED_BOOT_LIMIT)
