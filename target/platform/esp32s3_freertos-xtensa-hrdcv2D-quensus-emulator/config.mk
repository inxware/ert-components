#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for esp32s3_freertos-xtensa-hrdcv2C-debug-wifi
# @author: inx limited


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

include ./target/platform/esp32s3_freertos-xtensa-hrdcv2D-debug/config.mk

#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

EHS_DEFAULT_APP=customer-apps/Quensus/Quensus_flow_sensor_emulate

include ./target/devman-configs/esp32s3-base-inx-systems.com.mk

#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

#DEFS += EHS_MAIN_ESP32_TASK_STACK_SIZE=12000

# EHS_DEBUG_CONSOLE_THREAD_STACK_SIZE - stack size of the "dbgconsole" task (the console
# server thread, EhsSvcTcp_server in console_server.c). CONFIRMED BY REAL DEVICE TEST: even
# with EHS_STACK_MONITORING_ENABLED's low-stack check active (on by default here via
# EHS_DEBUGALL - the check only guards its own EhsSnprintf call, not the rest of the thread,
# see the porting guide's "Stack-aware logging" section), this still overflowed at 2048 with
# DBGCON connection-lifecycle logging (EHS_DBGCON_LIFECYCLE_LOG, console_server.c) enabled.
# DBGCON logging is therefore left OFF (its default) to keep this thread at 2048. If you
# re-enable EHS_DBGCON_LIFECYCLE_LOG, bump this back up to at least 4096.
#DEFS += EHS_DEBUG_CONSOLE_THREAD_STACK_SIZE=4096
DEFS += EHS_DEBUG_CONSOLE_THREAD_STACK_SIZE=2048

# Log every console queue push (bytes pushed, %% of buffer used) via EHSH_LOG_INFO -
# was enabled while diagnosing the "RT-Device Debug Console Overflow" reports on this target;
# switched off for this build so the diagnostic logging itself doesn't perturb the timing
# being measured (drain-to-empty + tighter poll cadence changes are now in place). Re-enable
# (optionally with EHS_CONSOLE_QUEUE_STATS_SKIP_ZERO to quiet the n=0 idle-poll lines) if
# another capture is needed.
##DEFS += EHS_CONSOLE_QUEUE_STATS
##DEFS += EHS_CONSOLE_QUEUE_STATS_SKIP_ZERO

# The runtime logger is off by default on this base (EHS_RUNTIME_LOGGER_ENABLED=no in
# esp32s3_freertos-xtensa-base/config.mk) and the "console" module defaults to ERROR-only
# (see Common/HAL/HAL.mk "Logging" section) - both must be overridden for the
# EHS_CONSOLE_QUEUE_STATS EHSH_LOG_INFO calls above to actually emit anything.
EHS_RUNTIME_LOGGER_ENABLED=yes
EHS_LOG_LEVEL_CONSOLE=EHSH_LOG_LEVEL_ALL

# Drop __FILE__/__LINE__ from the CQ push/pushRecord/pop diagnostics above - the file path
# is always the same 3 console_*.c files here, and this cuts each line down to
# [time][Severity][console]:"msg" while diagnosing the overflow.
EHS_LOGGER_REPORT_FIELDS=\(EHS_LOGGER_REPORT_MODULE\|EHS_LOGGER_REPORT_SEVERITY\)


# Wi-Fi credentials (uncomment to set)
#DEFS += EHS_CONFIG_WIFI_SSID="\"Wireless-INX\""
#DEFS += EHS_CONFIG_WIFI_PASSWORD="\"HelloInx101\""

################################### END OF CONFIGURATION ###################################################
