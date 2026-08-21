#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_android_arm_unity-tellisign
# @author: inx limited

# Target Platform Uses
# Digital Signage Platform for Tellisign server (formerlly s-h-o-w.online.)

include ./target/platform/linux_android_arm_legacy/config.mk

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# SYSTEM_VARIANT optionally indicates specific target environment confgurations.
# See target/envbuildscripts/targetenv_hacks_*.sh scripts).
#SYSTEM_VARIANT= system varients wil inherit this file

################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:
#EHS_DEBUGALL=true
#SYSTEM_VARIANT=


################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################

# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target   #
EHS_GUI_SUPPORT=unity3d
# Set EHS_VIDEO_SUPPORT to "no" to disable video rndering support in the media payer (e.g. for audio only devies) 
#EHS_VIDEO_SUPPORT=unity3d

# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
#This include RCUs, text displays, etc.
EHS_PERIPHERAL_DEVICE_SUPPORT=all

# Include GPIO support
EHS_PERIPHERALS_GPIO_SUPPORT=stubbed

#this needed to report the large cached download JSONs.
#DEFS+=EHS_STRING_LENGTH_MAX=8096

################################### END OF TOOLBOX CONFIGURATION ###################################################
################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################
EHS_PLUGIN_LIBRARY_DEPENDENCY=linux_android6_arm64_unity-lib

EHS_ANDROID_API=23
EHS_ANDROID_INSTALL_VERSION=7.1

EHS_DEFAULT_APP=customer-apps/SimpleSignOn/sso-unity-v1.1.0

EHS_UNITY_PROJECT_EXPORT_SUPPORT=yes
EHS_UNITY_VERSION=2022.3.62f3

#BUILD_WITH_ANDROID_SUPERVISOR=yes

DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=yes
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=yes

EHS_ANDROID_PACKAGE_SIGNING_PATH=show.online.google.play

include ./target/devman-configs/tellisign.com.mk

