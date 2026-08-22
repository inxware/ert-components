#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_android_arm_p64_h6_player-adnoc
# @author: inx limited
# This target siimply builds a standard linux version of EHS with android toolchain.
# It does not manage any events or resources such as graphics, so is unlikely to be useful
# other than to test compilation of standard code. It would run an exe using sysecec I expect.


include ./target/platform/linux_android_arm_p64_h6_player-ambifier/config.mk
#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################
# Remove this when tested  DEFS += EHS_AMBIFIER
EHS_HTTP_TLS_PEER_AUTHENTICATION=none


# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features 
SYSTEM_VARIANT=pine64_h6

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################
#This include RCUs, text displays, etc.
# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
#This include RCUs, text displays, etc.
EHS_PERIPHERAL_DEVICE_SUPPORT=all
# Include GPIO support
EHS_PERIPHERALS_GPIO_SUPPORT=stubbed

#this needed to report the large cached download JSONs.
#DEFS+=EHS_STRING_LENGTH_MAX=8096

################################### END OF TOOLBOX CONFIGURATION ###################################################

include ./target/devman-configs/adnoc-brown.local.mk

DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=none
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=yes

EHS_ANDROID_INSTALL_VERSION=9.0
EHS_DEFAULT_APP=customer-apps/RetailMusic/RetailAudio-v0.1.0

BUILD_WITH_ANDROID_SUPERVISOR=yes
