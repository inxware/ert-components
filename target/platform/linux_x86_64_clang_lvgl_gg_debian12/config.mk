#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# @author: inx limited

# Usage Description
# Baseline x86_64 Linux (Debian 12) build with the standard clang 14
# toolchain, LVGL GUI, AWS IoT Greengrass MQTT and the basic device
# management / peripheral feature set.  Mirrors the Debian 13 baseline
# (linux_x86_64_clang_lvgl_debian13_axelera) but on the older bookworm
# baseline image and without the Axelera ML stack.


#################################################################################################################
# Baseline Configuration Includes
#################################################################################################################

# (No parent config — standalone base for x86_64 Debian 12)


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
#################################################################################################################

EHS_ARCH=x86
EHS_OS=linux
EHS_GNU_ARCH=x86_64
EHS_GNU_OS=linux-gnu

# Native host build — no cross-compiler needed
EHS_HOST_DEBIAN_BUILD=x86_64

EHS_TARGET_LIB_VARIANT=-clang14_debian12

EHS_DEBIAN_VERSION=12

EHS_TOOLCHAIN_TYPE=clang
TOOLCHAIN_NAME=HOST
LINK_OVERRIDE=clang

COMPONENT_VARIANT=base
ERT_SODL_VERSION=1


#################################################################################################################
# Debug and Startup Modifiers
#################################################################################################################

EHS_DEBUGALL=yes

# Graphics module defaults to ERROR-only otherwise (EHSH_LOG_DEFAULT_LEVEL) —
# needed to see the frame_show / lvgl_camera_frame_renderer / ml_image_inference
# EHSH_LOG_WARNING diagnostics added while chasing the camera-embed issue.
# INFO was too noisy (logs every frame's renderer call) — warnings + errors only.
# ( and | must be backslash-escaped: gnu_ALL/toolchain.mk's
# $(foreach i,$(DEFS),-D$i) adds no quoting, so this reaches the shell
# unquoted — without the backslashes the shell reads "(" as a subshell.
EHS_LOG_LEVEL_GRAPHICS=\(EHSH_LOG_LEVEL_ERROR\|EHSH_LOG_LEVEL_WARNING\)

# The prebuilt kernel .a has R_X86_64_32S relocations; lld requires
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
EHS_PERIPHERALS_PWM_SUPPORT=gui
EHS_PID_SUPPORT=stubbed
EHS_SCHEDULER_SUPPORT=1
EHS_MODBUS_SUPPORT=stubbed

#----- Machine Learning — TFLite CPU baseline (no NPU) -----
EHS_ML_SUPPORT=yes
EHS_ML_IE_IMAGE_SUPPORT=tensorflow-lite
EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET=yes
EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET=yes

EHS_MV_SUPPORT=opencv

EHS_LORAWAN_SUPPORT=wio_e5


#################################################################################################################
# Application and Packaging
#################################################################################################################

EHS_DEFAULT_APP=tutorials/hello_world
EHS_PACKAGER_TYPE=deb

# Runtime packages for the deployed .deb.
# targetenv_make_deb.sh already adds libsdl2-2.0-0, libgl1-mesa-dri, libopencv-dev.
# Debian 12 (bookworm) package versions — no t64 transition (that was D13/U24).
DEBIAN_PACKAGE_PLATFORM_EXTRA= \
    libcurl4 \
    libarchive13 \
    libexpat1 \
    libidn2-0 \
    libxml2 \
    libpng16-16 \
    libavcodec59 \
    libavformat59 \
    libavutil57 \
    libswscale6 \
    libswresample4 \
    libabsl20220623 \
    libtbb12 \
    libopencv-core406 \
    libopencv-highgui406 \
    libopencv-imgproc406 \
    libopencv-videoio406 \
    libopencv-imgcodecs406


#################################################################################################################
# Device Management Credentials
#################################################################################################################

# (No devman config — no-certs variant)

################################### END OF CONFIGURATION ###################################################
