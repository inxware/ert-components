#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_android_arm_p64_h6_ambifier
# @author: inx limited

#Target Platform Uses
# Ambifier mini - connecting to devman.ambifier.com.

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

include ./target/platform/linux_android_arm_legacy/config.mk


################################################################################################################
# Configure debug/production levels
################################################################################################################

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################


# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
#This include RCUs, text displays, etc.
EHS_PERIPHERAL_DEVICE_SUPPORT=all

# Include GPIO support
EHS_PERIPHERALS_GPIO_SUPPORT=stubbed

DEFS+=EHS_USE_WIFI_INTERFACE=1

################################### END OF TOOLBOX CONFIGURATION ###################################################

DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=none
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=no

EHS_DEFAULT_APP=systemapps/Home

include ./target/devman-configs/devman.inxware.io.mk

# appland deployment config
EHS_APPLAND_INST_SUPPORT=yes
EHS_APPLAND_INST_DEPLOY_NAME=eRT-Android-ARM-Community
EHS_APPLAND_INST_OS_NAME=android-arm