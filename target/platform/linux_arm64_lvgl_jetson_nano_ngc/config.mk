#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for linux_arm64_lvgl_jetson_nano_ngc
# @author: inx limited
#
# NVIDIA Jetson Orin Nano — JetPack 6.x / Ubuntu 22.04 (Jammy).
# Build environment: NVIDIA NGC cross-compilation container.
# See Dockerfile and README.md for NGC authentication and setup.
#
# This target is functionally identical to linux_arm64_lvgl_jetson_nano but
# uses the official NGC cross-compilation image as its Docker base, which
# provides CUDA and TensorRT headers from the JetPack sysroot rather than
# via NVIDIA apt repositories.


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

# (No parent config — this is a self-contained base for Jetson targets)


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# CPU architecture, operating system, toolchain selection, and middleware dependencies
#################################################################################################################

EHS_ARCH         = arm64
EHS_OS           = linux
EHS_GNU_ARCH     = arm64
EHS_GNU_OS       = linux-gnu

# Ubuntu 22.04 (Jammy) — cross-compilation from Ubuntu 22.04 NGC container.
# EHS_DEBIAN_VERSION is used by some component makefiles for version-specific choices;
# Ubuntu 22.04 is ABI-compatible with Debian 12.
EHS_DEBIAN_VERSION = 12

#This determines the /usr/<arch> when using the host's toolchain
EHS_HOST_DEBIAN_BUILD = arm64

# Toolchain: clang from the NGC cross-compilation Docker container.
EHS_TOOLCHAIN_TYPE  = clang
EHS_GNU_OS_VERSION  = -clang15_ubuntu2204
TOOLCHAIN_NAME      = HOST
LINK_OVERRIDE       = clang

# Contributed library dependencies variant
COMPONENT_VARIANT = base


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

EHS_DEBUGALL = true


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

#----- Networking Features -----
EHS_NETWORKING_SUPPORT          = all
EHS_COMPONENT_NETWORKING_SUPPORT = all
EHS_MQTT_SUPPORT                = paho
EHS_DEVMAN_SUPPORT              = http

#----- GUI Features -----
EHS_GUI_SUPPORT  = lvgl
EHS_AV_SUPPORT   = devmanonly
EHS_MEDIA_SUPPORT = all

#----- Machine Vision — Jetson GStreamer HAL -----
# Uses nvarguscamerasrc (CSI) or v4l2src (USB) via GStreamer + nvvidconv.
# No OpenCV dependency required.
EHS_MV_SUPPORT   = jetson

#----- Machine Learning — TensorRT inference on Jetson GPU -----
# Requires a pre-built .engine file on the target (see README.md).
EHS_ML_SUPPORT                      = yes
# TFLite is not needed on Jetson — TensorRT handles all inference via the
# EHS_ML_HWACCEL_NVIDIA dispatch path.
EHS_ML_IE_IMAGE_SUPPORT      = none
EHS_ML_HARDWARE_ACCELERATION        = nvidia

EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET  = yes
EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET  = yes

#----- Peripheral Features -----
EHS_PERIPHERAL_DEVICE_SUPPORT   = all
EHS_PERIPHERALS_GPIO_SUPPORT     = stubbed
EHS_PERIPHERALS_UART_SUPPORT     = linux
EHS_PERIPHERALS_ADC_DAC_SUPPORT  = none

# New peripheral HAL defaults — stubbed unless a Jetson-specific HAL is added later
EHS_WATCHDOG_SUPPORT      ?= stubbed
EHS_UPS_SUPPORT           ?= stubbed
EHS_BUZZER_SUPPORT        ?= stubbed
EHS_SD_SELECT_SUPPORT     ?= stubbed
EHS_USB_POWER_SUPPORT     ?= stubbed
EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT ?= stubbed
EHS_RS485_CONFIG_SUPPORT  ?= stubbed


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

EHS_DEFAULT_APP  = tutorials/hello_world
EHS_PACKAGER_TYPE = deb


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# Ethernet is the primary network interface on Jetson Nano / Orin Nano.
# Wi-Fi is optional (USB or M.2 adapter) — enable if present.
#DEFS += EHS_USE_WIFI_INTERFACE=1

################################### END OF CONFIGURATION ###################################################
