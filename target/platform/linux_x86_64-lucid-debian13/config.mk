#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_x86_64-lucid-debian11
# @author: inx limited

# this target gets included in inxware lucid tools for linux. It should contain most things as needed for debugging all targets.


#################################################################################################################
# Baseline Configuration Includes
# Inherit the full x86_64 debian13 base (LVGL, OpenCV, ML, etc.)
#################################################################################################################

### TODO WE NEED A BASELINE debian 13 PC docker image.... not done that yet.
include ./target/platform/linux_x86_64_clang_lvgl_debian13-no-certs/config.mk

#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# (Inherited from parent — no changes needed)
#################################################################################################################

# (Inherits from parent config)

#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

# (Inherits from parent config)

#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

# (Inherits from parent config)
# LoRaWAN modem support (WIO-E5 via UART/serial)
EHS_LORAWAN_SUPPORT=wio_e5
EHS_PERIPHERALS_UART_SUPPORT=linux

# Machine Leaning & Machine Vision Support
EHS_ML_SUPPORT=yes
EHS_ML_FRAMEWORK_IMAGE_SUPPORT=tensorflow-lite

EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET=yes
EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET=yes


EHS_MV_SUPPORT=opencv
# enables a floating 'debug_cam_X' window with camera preview
#todo remove this
#DEFS+=EHS_OPENCV_DEBUG_PREVIEW=yes
#DEFS+=EHS_OPENCV_CROP_DEBUG_PREVIEW=yes


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

EHS_PACKAGER_TYPE=deb

# this forces eRT to always launch app in default directory when the program launches
EHS_DEFAULT_APP=systemapps/Home


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

include ./target/devman-configs/inx-systems.com.mk
DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=yes
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=yes

#don't start this on desktops
EHS_AUTO_START=--no-autostart


include ./target/devman-configs/devman.inxware.io.mk
DEVMAN_SERVER_PROTOCOL=http

# appland deployment config
EHS_APPLAND_INST_SUPPORT=yes
EHS_APPLAND_INST_DEPLOY_NAME=PC-Debian-11-64bit
EHS_APPLAND_INST_OS_NAME=linux

#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

