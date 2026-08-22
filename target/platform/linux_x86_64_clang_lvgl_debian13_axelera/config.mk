#---------------------------------------------------------------
# Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# @author: inx limited

# Usage Description
# x86_64 Linux (Debian 13) with LVGL, targeting machines fitted with an
# Axelera Metis NPU (PCIe).  libaxruntime.so is loaded at runtime via
# dlopen — the binary runs on machines without the card installed and
# logs a clear error at model-load time instead of refusing to start.


#################################################################################################################
# Baseline Configuration Includes
#################################################################################################################

# (No parent config — standalone base for x86_64 Debian 13)


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
#################################################################################################################

EHS_ARCH=x86
EHS_OS=linux
EHS_GNU_ARCH=x86_64
EHS_GNU_OS=linux-gnu

# Native host build — no cross-compiler needed
EHS_HOST_DEBIAN_BUILD=x86_64

EHS_TARGET_LIB_VARIANT=-clang19_debian13

EHS_DEBIAN_VERSION=13

EHS_TOOLCHAIN_TYPE=clang
TOOLCHAIN_NAME=HOST
LINK_OVERRIDE=clang

COMPONENT_VARIANT=base
ERT_SODL_VERSION=1


#################################################################################################################
# Debug and Startup Modifiers
#################################################################################################################

EHS_DEBUGALL=yes

# The prebuilt kernel .a has R_X86_64_32S relocations; Debian 13's lld requires
# -no-pie to accept them (non-PIE executable is fine for a desktop app).
LD_SWITCHES += -no-pie


#################################################################################################################
# Feature Configuration
#################################################################################################################

#----- Networking -----
EHS_NETWORKING_SUPPORT=all
EHS_COMPONENT_NETWORKING_SUPPORT=all
EHS_MQTT_SUPPORT=aws_green_grass
EHS_DEVMAN_SUPPORT=http

#----- GUI -----
EHS_GUI_SUPPORT=lvgl
EHS_AV_SUPPORT=ffmpeg
EHS_VIDEO_SUPPORT=yes
EHS_MEDIA_SUPPORT=all
EHS_TOOLKIT_DEPRECATED=yes

#----- Peripheral -----
EHS_PERIPHERAL_DEVICE_SUPPORT=all
EHS_PERIPHERALS_ADC_DAC_SUPPORT=stubbed
EHS_PERIPHERALS_GPIO_SUPPORT=gui
EHS_PID_SUPPORT=stubbed
EHS_SCHEDULER_SUPPORT=1
EHS_MODBUS_SUPPORT=stubbed

#----- Machine Learning — Axelera Metis NPU -----
# EHS_ML_IE_IMAGE_SUPPORT intentionally unset — no TFLite CPU fallback;
# the Metis NPU handles all inference.
EHS_ML_SUPPORT=yes
EHS_ML_HARDWARE_ACCELERATION=axelera
EHS_ML_IE_IMAGE_SUPPORT=none

EHS_MV_SUPPORT=opencv

EHS_LORAWAN_SUPPORT=wio_e5


#################################################################################################################
# Application and Packaging
#################################################################################################################

EHS_DEFAULT_APP=tutorials/hello_world
EHS_PACKAGER_TYPE=deb

# Runtime packages for the deployed .deb.
# targetenv_make_deb.sh already adds libsdl2-2.0-0, libgl1-mesa-dri, libopencv-dev.
# Axelera packages require the Axelera APT repo configured on the target machine.
DEBIAN_PACKAGE_PLATFORM_EXTRA= \
    libcurl4t64 \
    libarchive13t64 \
    libexpat1 \
    libidn2-0 \
    libxml2 \
    libpng16-16t64 \
    libavcodec61 \
    libavformat61 \
    libavutil59 \
    libswscale8 \
    libswresample5 \
    libabsl20240722 \
    libtbb12 \
    libopencv-core410 \
    libopencv-highgui410 \
    libopencv-imgproc410 \
    libopencv-videoio410 \
    libopencv-imgcodecs410 \
    axelera-runtime-1.6.0 \
    axelera-device-1.6.0


#################################################################################################################
# Device Management Credentials
#################################################################################################################

# (No devman config — no-certs variant)

################################### END OF CONFIGURATION ###################################################
