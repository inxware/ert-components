#---------------------------------------------------------------
# Copyright (C) 2025 inx limited, UK - All Rights Reserved.
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
# with this file. If not, please visit:
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------

# @file config.mk 
# @author: inx limited

# This is for Raspberry Pi with LVGL - it is aimed at being used with inxware 'hello_world' demo app.
# We're assuming Pi 4B or 5 below. Other flavours may work but haven't been tested.
#
#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

EHS_ARCH=arm64
EHS_OS=linux

EHS_GNU_ARCH=arm64
EHS_GNU_OS=linux-gnu
#
# This determines the /usr/<arch> when using the host's toolchain
EHS_HOST_DEBIAN_BUILD=arm64

# This will indicate what ert-contrib-middleware is used and toolchains if not using a host toolchain
# For the PoC, the clang Debian 11 will do...
# WARNING : Using greengrass built for debian 11 in ert-contrib-middleware!!!!
EHS_GNU_OS_VERSION=-clang11_debian11

# Configure some library version choices and Debian packager specifics
EHS_DEBIAN_VERSION=12
DEFS += EHS_DEBIAN_VERSION=12

# Use host toolchain + Qt headers and static objects from Docker
TOOLCHAIN_NAME=HOST

# Select the os-arch directory with these
EHS_TOOLCHAIN_TYPE=clang
LINK_OVERRIDE=clang

SYSTEM_VARIANT=RASPBERRYPI

# Qt uses iterative event loop pattern in `target_main.c`. This means we poll `EhsMainLoop()` from a Qt timer to enable EHS
# to progress its state, rather than the more traditional (blocking) call into `EhsLoop()`.
#
EHS_MAIN_LOOP_ITERATIVE=yes

# Debugging
EHS_DEBUGALL=true
#EHS_DEBUG_TCPIP_CONSOLE=stubbed

#################################################################################################################
# Set Component Support Configuration 
#################################################################################################################
# Component variant
COMPONENT_VARIANT=base

# Networking (standard)
EHS_NETWORKING_SUPPORT=all
EHS_COMPONENT_NETWORKING_SUPPORT=all
EHS_MQTT_SUPPORT=aws_green_grass

# Unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features
EHS_DEVMAN_SUPPORT=http
#EHS_AV_SUPPORT=devmanonly


EHS_GUI_SUPPORT=qt
 Set this as a modifier for QT not a whole new way of working.
EHS_GUI_SUPPORT_QT6=yes
#TODO2026 -we probablly want QT6 to be the default and the extra flag would be for qt5.

# We should try Wi-Fi by default in Raspberry Pi devices
DEFS += EHS_USE_WIFI_INTERFACE=1

# Peripheral support
EHS_PERIPHERAL_DEVICE_SUPPORT=all
EHS_PERIPHERALS_GPIO_SUPPORT=sysfs_linux_arm
EHS_PERIPHERALS_ADC_DAC_SUPPORT=SPI_A6_LTC241X

# Machine Leaning & Machine Vision Support
##EHS_ML_SUPPORT=yes
##EHS_ML_FRAMEWORK_IMAGE_SUPPORT=tensorflow-lite
##EHS_ML_HARDWARE_ACCELERATION=hailo

##EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET=yes
##EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET=yes
##EHS_MV_SUPPORT=opencv
##EHS_MV_SUPPORT=stubbed
# Enable machine vision support for testing C++ integration, or disable with `stubbed`.
#
EHS_MV_SUPPORT=stubbed

#AV
EHS_MEDIA_SUPPORT=all

# use libcamera on top of opencv if supported
EHS_USE_LIBCAMERA=yes

#EHS_PERIPHERALS_GPIO_SUPPORT=pigpio
#EHS_PERIPHERALS_GPIO_SUPPORT=wiringpi

##TODO PUT GPIO back when we have more non-RPI platforms to run on.
#EHS_PERIPHERALS_PWM_SUPPORT=pigpio
##EHS_PERIPHERALS_PWM_SUPPORT=wiringpi

#SDL_FULLSCREEN=yes

#################################################################################################################
# Packaging
#################################################################################################################

# Application Selection
EHS_DEFAULT_APP=demos/QT_UIs/hello_world-qt
#EHS_DEFAULT_APP=demos/QT_UIs/Particles
#EHS_DEFAULT_APP=demos/simple-qt-socket_webserver
# Enable eRT1 support (binary format SODL files) TODO THIS SHOULD BE THE DEFAULT NOW.
ERT_SODL_VERSION=1


EHS_PACKAGER_TYPE=deb

DEBIAN_PACKAGE_PLATFORM_EXTRA= \
qml6-module-qtquick \
qml6-module-qtquick-timeline \
qt6-base-dev qt6-declarative-dev \
qml6-module-qtquick \
qml6-module-qtquick-timeline \
qml6-module-qtquick-templates \
qml6-module-qtquick-window \
qml6-module-qtquick3d \
qml6-module-quick3d-particles3d \
qml6-module-quick3d-helpers \
qml6-module-quick3d-effects \
qml6-module-qtquick-window \
qml6-module-qtquick-controls \
qml6-module-qtquick-layouts \
qml6-module-qtquick-templates \
qml6-module-qtqml-workerscript

#################################################################################################################
## Platform Hacking 
#################################################################################################################
DEFS += EHS_LOG_LEVEL_VERBOSE
DEFS += EHS_LOG_TO_STDIO

# Force 'unknown' source files to output logging at the logger's logging level
DEFS += EHSL_MODULE_ID=EHSH_LOG_MODULE_LOGGER

# TODO - These are messy things that shouldn'be here or needed at all.:
# EHS_MOCK_GPIO_QT=yes
# DEFS += EHS_MOCK_GPIO_QT

# enables a floating 'debug_cam_X' window with camera preview
#DEFS+=EHS_OPENCV_FRAMEGRAB_DEBUG_PREVIEW=1
