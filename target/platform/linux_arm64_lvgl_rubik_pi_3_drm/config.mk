#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for linux_arm64_lvgl_rubik_pi_3_drm
# @author: inx limited

# Rubik Pi 3 with LVGL using native DRM/KMS display backend, Debian 12.


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

# (No parent config - this is a base configuration)


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# CPU architecture, operating system, toolchain selection, and middleware dependencies
#################################################################################################################

# CPU and OS Type Selection
EHS_ARCH=arm64
EHS_OS=linux
EHS_GNU_ARCH=arm64
EHS_GNU_OS=linux-gnu

# This determines the /usr/<arch> when using the host's toolchain
EHS_HOST_DEBIAN_BUILD=arm64

# This will indicate what ert-contrib-middleware is used and toolchains if not using a host toolchain
# WARNING: Using greengrass built for debian 11 in ert-contrib-middleware
EHS_GNU_OS_VERSION=-clang11_debian11

# Configure some library version choices and Debian packager specifics
EHS_DEBIAN_VERSION=12

# Use host toolchain
TOOLCHAIN_NAME=HOST

# Select the os-arch directory with these
EHS_TOOLCHAIN_TYPE=clang
LINK_OVERRIDE=clang

# Contributed library dependencies variant
COMPONENT_VARIANT=base


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

# Debug/Production mode
EHS_DEBUGALL=true
#EHS_DEBUG_TCPIP_CONSOLE=stubbed


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

#----- Networking Features -----
EHS_NETWORKING_SUPPORT=all
EHS_COMPONENT_NETWORKING_SUPPORT=all
EHS_MQTT_SUPPORT=aws_green_grass

# Unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features
EHS_DEVMAN_SUPPORT=http

#----- GUI Features -----
EHS_GUI_SUPPORT=lvgl
# Display backend — choose one:
#   drm:     native DRM/KMS, no compositor needed; stop Weston first
#            (sudo kill $(fuser /dev/dri/card0 2>/dev/null))
#   wayland: render into a running Weston compositor via SDL2 Wayland
# If unset will auto-detect between wayland and Xorg
EHS_LVGL_LINUX_DISPLAY_BACKEND=drm
#EHS_LVGL_LINUX_DISPLAY_BACKEND=wayland

#EHS_AV_SUPPORT=devmanonly

#AV
EHS_MEDIA_SUPPORT=all

# Use libcamera on top of opencv if supported
EHS_USE_LIBCAMERA=yes

#----- Machine Vision / ML Features -----
# Enable machine vision support for testing C++ integration, or disable with `stubbed`.
EHS_MV_SUPPORT=stubbed
##EHS_ML_SUPPORT=yes
##EHS_ML_FRAMEWORK_IMAGE_SUPPORT=tensorflow-lite
##EHS_ML_HARDWARE_ACCELERATION=hailo
##EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET=yes
##EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET=yes
##EHS_MV_SUPPORT=opencv

#----- Peripheral Features -----
EHS_PERIPHERAL_DEVICE_SUPPORT=all
EHS_PERIPHERALS_GPIO_SUPPORT=sysfs_linux_arm
EHS_PERIPHERALS_ADC_DAC_SUPPORT=SPI_A6_LTC241X
#EHS_PERIPHERALS_GPIO_SUPPORT=pigpio
#EHS_PERIPHERALS_GPIO_SUPPORT=wiringpi
#EHS_PERIPHERALS_PWM_SUPPORT=pigpio
#EHS_PERIPHERALS_PWM_SUPPORT=wiringpi

SDL_FULLSCREEN=yes


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

EHS_DEFAULT_APP=tutorials/hello_world

EHS_PACKAGER_TYPE=deb


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

# (No devman config for this target - no-certs variant)


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# We should try Wi-Fi by default in Raspberry Pi devices
DEFS += EHS_USE_WIFI_INTERFACE=1
DEFS += EHS_DEBIAN_VERSION=12

# EHS_MOCK_GPIO_QT: Mock GPIO widgets in Qt UI for desktop development before hardware is available.
# EHS_MOCK_GPIO_QT=yes
# DEFS += EHS_MOCK_GPIO_QT

# Enables a floating 'debug_cam_X' window with camera preview
#DEFS+=EHS_OPENCV_FRAMEGRAB_DEBUG_PREVIEW=1


################################### END OF CONFIGURATION ###################################################
