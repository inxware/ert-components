#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for esp32s3_freertos-xtensa-hrdcv2C-BA-debug-app
# @author: inx limited

# This is for the intermediate 2C boards that have Original display board but driven by 2C processor board.


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

include ./target/platform/esp32s3_freertos-xtensa-base_n8r2/config.mk


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# CPU architecture, operating system, toolchain selection, and middleware dependencies
#################################################################################################################

# SYSTEM_VARIANT for conditional compilation for very specific features
# See target/envbuildscripts/targetenv_hacks_*.sh scripts
SYSTEM_VARIANT=esp32s3-hrdcv2


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

#EHS_DEBUGALL=yes
EHS_DEBUG_TCPIP_CONSOLE=yes
EHS_RUNTIME_LOGGER_ENABLED=no

# Serial console support
EHS_SERIAL_CONSOLE_SUPPORT=yes
# For Production: EHS_SERIAL_CONSOLE_SUPPORT=none


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

# HRDCV2 product config ("3" for Rev. C)
# TODO 2025 - shouldn't this be another system variant rather creating more hacks we don't understand?
DEFS += EHS_PLATFORM_ESP32S3_HRDCV2=3

#----- Networking Features -----
EHS_NETWORK_ETHERNET_SUPPORT=yes
EHS_NETWORK_WIFI_SUPPORT=yes

# Devman support
EHS_DEVMAN_SUPPORT=mqtt

#----- Peripheral Features -----
EHS_I2C_SUPPORT=yes
EHS_RTC_SUPPORT=yes
DEFS += EHS_MAX31343_SUPPORT


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

EHS_DEFAULT_APP=customer-apps/Backer/hrdcv2_app
#EHS_DEFAULT_APP=tutorials/hello_world
#EHS_DEFAULT_APP=hardware-test-apps/HRDC2C-hardware-test
#EHS_DEFAULT_APP=hardware-test-apps/HRDC2C-calibration (for production only)


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

#include ./target/devman-configs/devport-local.mk
include ./target/devman-configs/esp32s3-hrdcv2-iot-vs1.nibe.se.mk


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# Thermocouple circuit gain
# EHS_THERMOCOUPLE_AMP_GAIN_DIV_MULTIPLIER_CUSTOM has the bitlength of EHS_THERMOCOUPLE_GAIN_SHIFT_BITS
# The calculation result involving the multiplier would be shifted right by the shift bits
DEFS += EHS_THERMOCOUPLE_AMP_GAIN_DIV_MULTIPLIER_CUSTOM=963
DEFS += EHS_THERMOCOUPLE_GAIN_SHIFT_BITS=15

# Maximum ADC reading in 10-bit fixed point (used to check thermocouple parameters during build time)
DEFS += EHS_THERMOCOUPLE_ADC_MAX_FP=1843200

# PT100 amplifier gain
DEFS += EHS_PT100_AMP_GAIN_SCALE=2731
DEFS += EHS_PT100_AMP_GAIN_DIVIDER=384

# Thermocouple opamp offset and result offset
DEFS += INX_HEATROD_HRC_THERMOCOUPLE_AVG_OPAMP_OFFSET_MV=111492
DEFS += EHS_THERMOCOUPLE_RESULT_OFFSET=3092

# PID amplifier gain factors
DEFS += EHS_PID_10V_AMP_GAIN_DIVIDER_FACTOR=6380
DEFS += EHS_PID_10V_AMP_GAIN_CORRECTION_FACTOR=1024
DEFS += EHS_PID_20MA_AMP_GAIN_DIVIDER_FACTOR=13
DEFS += EHS_PID_20MA_AMP_GAIN_CORRECTION_FACTOR=1008


################################### END OF CONFIGURATION ###################################################
