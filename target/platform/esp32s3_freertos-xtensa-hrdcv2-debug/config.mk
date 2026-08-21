#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for esp32s3_freertos-xtensa-hrdcv2-debug
# @author: inx limited


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

include ./target/platform/esp32s3_freertos-xtensa-base-debug/config.mk


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# CPU architecture, operating system, toolchain selection, and middleware dependencies
#################################################################################################################

# SYSTEM_VARIANT for conditional compilation for very specific features
# See target/envbuildscripts/targetenv_hacks_*.sh scripts
SYSTEM_VARIANT=esp32s3-hrdcv2


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

# HRDCV2 product config ("1" means Rev. A)
DEFS += EHS_PLATFORM_ESP32S3_HRDCV2=1

#----- Networking Features -----
EHS_NETWORK_ETHERNET_SUPPORT=yes

# Devman support
EHS_DEVMAN_SUPPORT=mqtt

#----- Peripheral Features -----
EHS_I2C_SUPPORT=yes
DEFS += EHS_MAX31343_SUPPORT


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

EHS_DEFAULT_APP=customer-apps/Backer/hrdcv2_app


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

# This target connects to nibe mqtt broker
include ./target/devman-configs/esp32s3-hrdcv2-iot-vs1.nibe.se.mk


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# Thermocouple circuit gain
# EHS_THERMOCOUPLE_AMP_GAIN_DIV_MULTIPLIER_CUSTOM has the bitlength of EHS_THERMOCOUPLE_GAIN_SHIFT_BITS
# The calculation result involving the multiplier would be shifted right by the shift bits
DEFS += EHS_THERMOCOUPLE_AMP_GAIN_DIV_MULTIPLIER_CUSTOM=748
DEFS += EHS_THERMOCOUPLE_GAIN_SHIFT_BITS=17

# Maximum ADC reading in 10-bit fixed point (used to check thermocouple parameters during build time)
DEFS += EHS_THERMOCOUPLE_ADC_MAX_FP=1843200

# PT100 amplifier gain
DEFS += EHS_PT100_AMP_GAIN_SCALE=7652
DEFS += EHS_PT100_AMP_GAIN_DIVIDER=137

# PID amplifier gain factors
DEFS += EHS_PID_10V_AMP_GAIN_DIVIDER_FACTOR=3277
DEFS += EHS_PID_10V_AMP_GAIN_CORRECTION_FACTOR=1024
DEFS += EHS_PID_20MA_AMP_GAIN_DIVIDER_FACTOR=10
DEFS += EHS_PID_20MA_AMP_GAIN_CORRECTION_FACTOR=1049


################################### END OF CONFIGURATION ###################################################
