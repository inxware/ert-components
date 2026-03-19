#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for linux_arm64_lvgl_raspberrypi_debian13
# @author: inx limited

# Raspberry Pi (4B or 5) with LVGL, Debian 13. Includes OpenCV and EdgeML support.


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

include ./target/platform/linux_arm64_debian13_base/config.mk


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# CPU architecture, operating system, toolchain selection, and middleware dependencies
#################################################################################################################

SYSTEM_VARIANT=RASPBERRYPI


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

# (Inherits from parent config)


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

#----- Networking Features -----
# TODO: We need to fix the build when this isn't set.
EHS_DEVMAN_SUPPORT=http

#----- GUI Features -----
# (Inherits LVGL from parent config)

#----- Machine Vision / ML Features -----
EHS_ML_SUPPORT=yes
EHS_ML_IE_IMAGE_SUPPORT=tensorflow-lite
EHS_ML_HARDWARE_ACCELERATION=hailo

EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET=yes
EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET=yes
EHS_MV_SUPPORT=opencv
#EHS_MV_SUPPORT=stubbed

# Use libcamera on top of opencv if supported
EHS_USE_LIBCAMERA=yes
# Enables a floating 'debug_cam_X' window with camera preview
#DEFS+=EHS_OPENCV_FRAMEGRAB_DEBUG_PREVIEW=1

#----- Peripheral Features -----
#EHS_PERIPHERALS_GPIO_SUPPORT=wiringpi
#EHS_PERIPHERALS_PWM_SUPPORT=wiringpi
EHS_PERIPHERALS_GPIO_SUPPORT=stubbed
#EHS_PERIPHERALS_PWM_SUPPORT=pigpio


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

EHS_DEFAULT_APP=tutorials/hello_world


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

# (No devman config for this target)


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# (No legacy DEFS for this target)


# LoRaWAN modem support (WIO-E5 via UART/serial)
EHS_LORAWAN_SUPPORT=wio_e5

################################### END OF CONFIGURATION ###################################################
