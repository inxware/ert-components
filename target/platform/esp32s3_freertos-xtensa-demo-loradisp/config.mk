#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for esp32_freertos-xtensor-base
# @author: inx limited

#################################################################################################################
# Set general architecture and OS version
#################################################################################################################

include ./target/platform/esp32s3_freertos-xtensa-base-debug/config.mk


# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features 
# SYSTEM_VARIANT optionally indicates specific target environment confgurations.
# See target/envbuildscripts/targetenv_hacks_*.sh scripts).
SYSTEM_VARIANT=esp32s3-hrdcv2

# HRDCV2 product config
## "1" means Rev. A
# DEFS += EHS_PLATFORM_ESP32S3_HRDCV2=1
# unset EHS_NETWORK_WIFI_SUPPORT

#This should be in the os-arch make file for esp32? Or is it more specific for Ethernet? If so lets say so.
EHS_NETWORK_CONFIG_TYPE_SUPPORT=esp32

EHS_DEFAULT_APP=events/microelectronics-uk-2025/lorawan-demo
EHS_DEVMAN_SUPPORT=mqtt

# this target connects to nibe mqtt broker
include ./target/devman-configs/esp32s3-hrdcv2-inx-systems.com.mk

# Enable the serial command prompt interface
DEFS += EHS_ESP32_CMD_PROMPT_SUPPORT
# Disable esp logger
DEFS += EHS_ESP32_DISABLE_LOGS=1

DEFS += EHS_MAIN_ESP32_TASK_STACK_SIZE=16000

# WIFI
EHS_NETWORK_WIFI_SUPPORT=yes
# TODO2025 We need to know if this clobbers saved files are are defaults if none are saved.
DEFS += EHS_CONFIG_WIFI_SSID="\"Wireless-INX\""
DEFS += EHS_CONFIG_WIFI_PASSWORD="\"HelloInx101\""
# Maximum wi-fi SSID records buffer size
DEFS += ESP32_MAX_AP_RECORDS=20

# Enable nano printf support for isr safe vsnprintf
DEFS += EHS_NANOPRINTF_SUPPORT=1

# Do NOT restart after loading the app
EHS_TARGET_APPLOAD_RESTARTING_REBOOT=yes
################################### END OF TOOLBOX CONFIGURATION ###################################################
