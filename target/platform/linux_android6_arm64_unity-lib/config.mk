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


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

include ./target/platform/linux_android_arm64/config.mk


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# CPU architecture, operating system, toolchain selection, and middleware dependencies
#################################################################################################################

EHS_ANDROID_API=23

# this builds only .so plugin
EHS_PLUGIN_LIBRARY_DEPENDENCY=yes


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

EHS_DEBUGALL=true


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

#----- GUI Features -----
EHS_GUI_SUPPORT=unity3d
#EHS_VIDEO_SUPPORT=unity3d

#----- Peripheral Features -----
EHS_PERIPHERAL_DEVICE_SUPPORT=all


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

EHS_DEFAULT_APP=customer-apps/SimpleSignOn/sso-unity-v1.1.0


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

#DEVMAN_SERVER_DOMAIN=devman.inx-systems.com

include ./target/devman-configs/inx-systems.com.mk

DEVMAN_SERVER_PROTOCOL=https


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# (Inherits from parent config)


################################### END OF CONFIGURATION ###################################################
