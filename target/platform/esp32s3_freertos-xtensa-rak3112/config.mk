#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# eRT platform: RAK3112 (RAKwireless) — ESP32-S3-WROOM-1-N16R8 + SX1262 LoRa.
# Inherits the n8r8 base (8 MB Flash partition, 8 MB octal PSRAM) and swaps
# LoRaWAN from the WIO-E5 AT-modem to the in-chip SX1262 via SX126x-Arduino.
# The N16 flash on the real module gives spare room for OTA, but we stay on
# the 8 MB partition for now because the ert-contrib-middleware bootloader
# and partition table are shared with the n8r8 base.


#################################################################################################################
# Baseline Configuration Includes
#################################################################################################################

include ./target/platform/esp32s3_freertos-xtensa-base_n8r8/config.mk


#################################################################################################################
# LoRaWAN module override — SX126x-Arduino in-chip stack (not an external AT modem)
#################################################################################################################

EHS_LORAWAN_SUPPORT=rak3112


#################################################################################################################
# GUI override — RAK3112 carrier here has no display
#################################################################################################################

# Override the n8r8 baseline (lvgl + ft81x). Pulls in
# target/Component-HAL/graphics/stubbed/ instead — no-op viewport primitives
# so the FB graph can construct gui_viewport / widget components without a
# panel attached. Also frees the LVGL footprint from the binary (~hundreds of
# KB), helpful for the 8 MB partition envelope.
EHS_GUI_SUPPORT=stubbed

#################################################################################################################
# Default APP override
#################################################################################################################
EHS_DEFAULT_APP=demos/lorawan

#################################################################################################################
# Networking overrides — enable Wi-Fi (disabled by default in n8r8 base)
#################################################################################################################

EHS_NETWORK_WIFI_SUPPORT=yes

#################################################################################################################
# Device Management
#################################################################################################################

EHS_DEVMAN_SUPPORT=mqtt

#################################################################################################################
# Appland deployment
#################################################################################################################

EHS_APPLAND_INST_SUPPORT=yes
EHS_APPLAND_INST_DEPLOY_NAME=Xtensa-ESP32-S3-RAK3112
EHS_APPLAND_INST_OS_NAME=xtensa-esp32-s3-rak3112


#################################################################################################################
# Debug / Lucid tooling
#################################################################################################################

# Enable the TCP/IP debugger console required by Lucid for local connection,
# app updates, and remote debugging.  See docs/ert-build-guide.md § "Configuring Debug Builds".
EHS_DEBUG_TCPIP_CONSOLE=yes

# Enable the runtime logger so EHSH_LOG_ERROR / _WARNING / _INFO calls
# actually emit (the base_n8r8 config disables this by default).
# Runtime verbosity per module via EhsHLogger_setLogLevel().
EHS_RUNTIME_LOGGER_ENABLED=yes

#################################################################################################################
# Kernel event-queue locking (advanced)
#################################################################################################################

# By default the kernel takes EhsTPMutex_eventQueue around EhsTrigger_fire's
# push loop, which is required whenever more than one thread can fire
# triggers — and on this target the LoRaWAN HAL worker (taskLoRaWAN) does
# fire CB triggers via EHS_FB_FINISH from inside its own thread, so the lock
# must stay enabled here. The line below is left commented as a reminder of
# the opt-out (single-producer-only targets that need to call EhsTrigger_fire
# from a true ISR can set it). DO NOT enable on rak3112.
# EHS_KERNEL_LOCKFREE_EVQ=yes


#################################################################################################################
# App-reload reboot policy
#################################################################################################################

# Reboot-on-reload is only needed when EHS_GUI_SUPPORT=lvgl — the LVGL render
# path crashes during in-place teardown (see docs/ert-porting-guide.md § "LVGL
# → Known Issue: KILL APP / app-reload crashes the LVGL render path"). With
# EHS_GUI_SUPPORT=stubbed above the in-place path is safe, so we explicitly
# disable the inherited =yes from the n8r8 baseline. =none is the canonical
# off form (commenting the line would silently keep the parent's =yes); see
# docs/ert-porting-guide.md § "Configuration variable conventions".
EHS_TARGET_APPLOAD_RESTARTING_REBOOT=no


################################### END OF CONFIGURATION ###################################################
