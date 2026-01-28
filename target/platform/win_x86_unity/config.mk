#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for win_x86_unity
# @author: inx limited

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

include ./target/platform/win_x86/config.mk

SYSTEM_VARIANT=unity


################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:

# Some debug options
EHS_DEBUGALL=true
DEFS += EHS_RUNTIME_FILELOGGER_ENABLED

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################

#
# uncomment this variable if the platform requires NETWORKING e.g. devman plugins 

EHS_NETWORKING_SUPPORT=all# This needs to be enabled for the core EHS - there is a HAL dependency
#@todo we need an entry here for the URL components etc.
EHS_COMPONENT_NETWORKING_SUPPORT=all


#unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features 
EHS_DEVMAN_SUPPORT=http

################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################
# COMPONENT_VARIANT allows a specific variant of contributed ert-contrib-middleware/build directory 
# libraries to be used. The path is defined as follows (without delimietrs if options are not set:)
# $(EHS_GNU_OS_ARCH)_$(COMPONENT_VARIANT)-$(TOOLCHAIN_NAME) 
#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
#Note - for xml2, curl lib etc. we can use the gtk-gst component libraries (I expect!)
COMPONENT_VARIANT=sdl2-ffmpeg

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################

# This  is set to include the rendering features in eRT. It is  nearly always set, so should be removed (default on) and specific platforme xceptionsset instead
EHS_MEDIA_SUPPORT=all# This is for the media handling stuff
EHS_GUI_SUPPORT=unity3d
# This (EHS_AV_SUPPORT) is needed for PlayManager
EHS_AV_SUPPORT=unity3d
# using unity3d for video, do not specify EHS_VIDEO_SUPPORT !
EHS_VIDEO_SUPPORT=

EHS_MQTT_SUPPORT=aws_green_grass

# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
# Keyboard and stuff..
EHS_PERIPHERAL_DEVICE_SUPPORT=all

export EHS_SKIP_REPO_PULL=true

INXWARE_TARGETENV_HACKS=win_x86_unity

EHS_DEFAULT_APP=customer-apps/SimpleSignOn/sso-unity-v1.1.0

# Used for specifing name of the eRT package/executable
ERT_PACKAGE_NAME=TELLISIGN
# Used for specifing user facing name of installed application (windows installer)
ERT_NSIS_EXE_NAME=Unity-eRT

include ./target/devman-configs/tellisign.com.mk

################################### END OF TOOLBOX CONFIGURATION ###################################################

DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=none
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=no