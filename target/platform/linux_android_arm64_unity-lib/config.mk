#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_android_arm64_unity-lib
# @author: inx limited

# This target siimply builds a standard linux version of EHS with android toolchain.
# It does not manage any events or resources such as graphics, so is unlikely to be useful
# other than to test compilation of standard code. It would run an exe using sysecec I expect.

include ./target/platform/linux_android_arm64/config.mk
################################################################################################################
# Configure debug/production levels
################################################################################################################
EHS_DEBUGALL=true

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################

# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target   #
EHS_GUI_SUPPORT=unity3d
# Set EHS_VIDEO_SUPPORT to "no" to disable video rndering support in the media payer (e.g. for audio only devies) 
#EHS_VIDEO_SUPPORT=unity3d

#This include RCUs, text displays, etc.
# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
EHS_PERIPHERAL_DEVICE_SUPPORT=all

# this builds only .so plugin
EHS_PLUGIN_LIBRARY_DEPENDENCY=yes

################################### END OF TOOLBOX CONFIGURATION ###################################################
#DEVMAN_SERVER_DOMAIN=devman.inx-systems.com

include ./target/devman-configs/inx-systems.com.mk

DEVMAN_SERVER_PROTOCOL=https

EHS_DEFAULT_APP=customer-apps/SimpleSignOn/sso-unity-v1.1.0
EHS_UNITY_VERSION=6000.4.1f1

# This target produces a Unity .so plugin, not an installable package, so there is nothing for
# 'make targetenv_package' to build. Set explicitly: the os-arch default would
# otherwise select targetenv_apk, and a plugin has no APK, which fails here.
# A platform '=' overrides the os-arch '?=' (target/os-arch/*/config.mk).
EHS_PACKAGER_TYPE=none
