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
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

include ./target/platform/win_x86/config.mk


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# CPU architecture, operating system, toolchain selection, and middleware dependencies
#################################################################################################################

SYSTEM_VARIANT=unity

INXWARE_TARGETENV_HACKS=win_x86_unity

# Contributed library dependencies variant
COMPONENT_VARIANT=sdl2-ffmpeg

export EHS_SKIP_REPO_PULL=true


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

# Debug/Production mode
EHS_DEBUGALL=true


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

#----- Networking Features -----
EHS_NETWORKING_SUPPORT=all# This needs to be enabled for the core EHS - there is a HAL dependency
EHS_COMPONENT_NETWORKING_SUPPORT=all
EHS_MQTT_SUPPORT=aws_green_grass

#unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features
EHS_DEVMAN_SUPPORT=http

#----- GUI Features -----
EHS_GUI_SUPPORT=unity3d
# This (EHS_AV_SUPPORT) is needed for PlayManager
EHS_AV_SUPPORT=unity3d
# using unity3d for video, do not specify EHS_VIDEO_SUPPORT !
EHS_VIDEO_SUPPORT=
EHS_MEDIA_SUPPORT=all# This is for the media handling stuff

#----- Peripheral Features -----
EHS_PERIPHERAL_DEVICE_SUPPORT=all


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

EHS_DEFAULT_APP=customer-apps/SimpleSignOn/sso-unity-v1.1.0

# Used for specifing name of the eRT package/executable
ERT_PACKAGE_NAME=TELLISIGN
# Used for specifing user facing name of installed application (windows installer)
ERT_NSIS_EXE_NAME=Unity-eRT


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

include ./target/devman-configs/tellisign.com.mk

DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=none
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=no


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

DEFS += EHS_RUNTIME_FILELOGGER_ENABLED


################################### END OF CONFIGURATION ###################################################
