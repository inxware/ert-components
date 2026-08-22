#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_android_arm_p64_a6_player-ambifier
# @author: inx limited
# This target siimply builds a standard linux version of EHS with android toolchain.
# It does not manage any events or resources such as graphics, so is unlikely to be useful
# other than to test compilation of standard code. It would run an exe using sysecec I expect.

include ./target/platform/linux_android_arm_legacy/config.mk

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

#TODO What does this instruct the build to do that isn't done by the ambifier hack file?
# Remove this when tested DEFS += EHS_AMBIFIER



# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features 
# SYSTEM_VARIANT optionally indicates specific target environment confgurations.
# See target/envbuildscripts/targetenv_hacks_*.sh scripts). 
SYSTEM_VARIANT=pine64_a6


################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################

# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target   #
# Set this to match one of the graphics types in EHS/target/graphics/                                       #
EHS_PERIPHERAL_DEVICE_SUPPORT=all

# Include GPIO support
EHS_PERIPHERALS_GPIO_SUPPORT=sysfs_linux_arm

################################### END OF TOOLBOX CONFIGURATION ###################################################

#DEVMAN_SERVER_DOMAIN=devman.ambifier.com
#DEVMAN_SERVER_PROTOCOL=https

include ./target/devman-configs/ambifier.com.mk
EHS_PRODUCT_NAME=player
DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=yes
EHS_ANDROID_INSTALL_VERSION=7.1

BUILD_WITH_ANDROID_SUPERVISOR=yes

EHS_DEFAULT_APP=customer-apps/RetailMusic/RetailAudio-v0.1.0
