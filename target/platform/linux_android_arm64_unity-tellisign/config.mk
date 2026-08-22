#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for linux_android_arm64_unity-tellisign
# @author: inx limited

# Target Platform Uses
# Digital Signage Platform for Tellisign server - 64-bit Android 12+ targets (e.g. Radxa Rock).
# Uses Unity 6 (arm64-v8a only; Android 12 dropped 32-bit support on many boards).

include ./target/platform/linux_android_arm64/config.mk

#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
#################################################################################################################

# Android 12 = API level 31
EHS_ANDROID_API=31
EHS_ANDROID_INSTALL_VERSION=12.0

# Use API 31 clang toolchain (overrides base arm64 which defaults to API 30)
CC_OVERRIDE=aarch64-linux-android31-clang
LINK_OVERRIDE=aarch64-linux-android31-clang

################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:
#EHS_DEBUGALL=true

################################################################################################################
# Select which toolboxes and supporting middleware options should be used
################################################################################################################

EHS_GUI_SUPPORT=unity3d
EHS_AV_SUPPORT=unity3d
# using unity3d for video, do not specify EHS_VIDEO_SUPPORT
EHS_VIDEO_SUPPORT=

EHS_PERIPHERAL_DEVICE_SUPPORT=all
EHS_PERIPHERALS_GPIO_SUPPORT=stubbed

################################### END OF TOOLBOX CONFIGURATION ###################################################
################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################

# arm64-only target: the app .so is copied directly to arm64/; no 32-bit dependency needed.
# EHS_PLUGIN_LIBRARY_DEPENDENCY is intentionally not set.
EHS_ANDROID_NATIVE_ABI=arm64

EHS_DEFAULT_APP=customer-apps/SimpleSignOn/sso-unity-v1.1.0

EHS_UNITY_PROJECT_EXPORT_SUPPORT=yes
EHS_UNITY_VERSION=6000.4.1f1

# Path to a directory in DevmanSecurity repo with items used for signing android apk and aab files
EHS_ANDROID_PACKAGE_SIGNING_PATH=show.online.google.play

DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=yes
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=yes

include ./target/devman-configs/tellisign.com.mk
