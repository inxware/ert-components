#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_android_arm_p64_a6_ambifier
# @author: inx limited

#Target Platform Uses
# Ambifier MSG200 and MSG300 audio channel - connecting to devman.ambifier.com.

include ./target/platform/linux_android_arm_legacy/config.mk

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################
# Remove this when tested DEFS += EHS_AMBIFIER

# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features 
# SYSTEM_VARIANT optionally indicates specific target environment confgurations.
# See target/envbuildscripts/targetenv_hacks_*.sh scripts).
SYSTEM_VARIANT=pine64_a6
#this needed to report the large cached download JSONs.
#DEFS+=EHS_STRING_LENGTH_MAX=8096
################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:
#EHS_DEBUGALL=true
DEFS+=EHS_REMOVE_TEMP_APPS_ON_BOOT

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################

# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target   #
# Set this to match one of the graphics types in EHS/target/graphics/
EHS_GUI_SUPPORT=android_stub

# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
#This include RCUs, text displays, etc.
EHS_PERIPHERAL_DEVICE_SUPPORT=all
EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER=yes

#TEMPORARY - CHECK FOR GSK
#EHS_HTTP_TLS_PEER_AUTHENTICATION=none

# Include GPIO support
EHS_PERIPHERALS_GPIO_SUPPORT=sysfs_linux_arm

DEFS += EHS_AUDIO_INPUT_LEVEL_SUPPORT
EHS_AUDIO_INPUT_LEVEL_SUPPORT=all

################################### END OF TOOLBOX CONFIGURATION ###################################################
################################################################################################################
# Define the Application and IoT server configuration
################################################################################################################

include ./target/devman-configs/ambifier.com.mk
DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=yes
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=yes

EHS_ANDROID_INSTALL_VERSION=7.1
#Build upload package with supervisor scripts & downloader apk and tell the adb installer to install these directly also.
BUILD_WITH_ANDROID_SUPERVISOR=yes

#Any other apps to packaged with the Devman uploader
ANDROID_SUPPLEMENTARY_APP_REPO=ssh://tech-data@dev.inx-systems.net:8822/home/inx-data/data/Repos/Ambifier.git
ANDROID_SUPPLEMENTARY_APP_PATH=Ambifier/Ambifier2/builds/Android/ambifier.apk

#Application Selection
EHS_DEFAULT_APP=customer-apps/TSA/ambifier-v0.6.0-integrated
