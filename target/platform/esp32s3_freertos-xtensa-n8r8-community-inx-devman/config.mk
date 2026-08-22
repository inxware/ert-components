#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration for ESP32-S3 N8R8 variant with community devman features.
# Combines the N8R8 hardware base (8 MB Flash, 8 MB PSRAM) with the Wi-Fi + MQTT
# devman stack from esp32s3_freertos-xtensa-community-inx-devman.
# @author: inx limited


#################################################################################################################
# Baseline Configuration Includes
#################################################################################################################

include ./target/platform/esp32s3_freertos-xtensa-base_n8r8/config.mk


#################################################################################################################
# LoRaWAN module override — Assume none (We could add an AT controlled option. AT Modules are low-overhead 
#  and could be connected to a SOM UART.
#################################################################################################################

EHS_LORAWAN_SUPPORT=stubbed

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
EHS_APPLAND_INST_DEPLOY_NAME=Xtensa-ESP32-S3-N8R8-Community-Devman
EHS_APPLAND_INST_OS_NAME=xtensa-esp32-s3-n8r8


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
# App-reload reboot policy
#################################################################################################################

# Reboot the target when a new application is loaded (console 'F' / reload-from-file),
# instead of tearing down and re-initialising in place. Necessary here because the
# in-place teardown path crashes in the LVGL render loop (see docs/ert-porting-guide.md
# § "LVGL → Known Issue: KILL APP / app-reload crashes the LVGL render path"). By the
# time EHS_APP_LOAD_RESTARTING fires, the 'L' command has already committed the new
# SODL to flash, so a reboot is safe — the new app is loaded from storage on restart.
# Tradeoff: users wait for WiFi (and MQTT) to reconnect after each upload.
EHS_TARGET_APPLOAD_RESTARTING_REBOOT=no


################################### END OF CONFIGURATION ###################################################
