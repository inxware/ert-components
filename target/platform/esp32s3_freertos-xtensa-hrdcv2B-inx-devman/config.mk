#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for esp32s3_freertos-xtensa-hrdcv2B-inx-devman
# @author: inx limited


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

include ./target/platform/esp32s3_freertos-xtensa-hrdcv2B/config.mk


EHS_NETWORK_BLE_SUPPORT=nimble


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# CPU architecture, operating system, toolchain selection, and middleware dependencies
#################################################################################################################

# Libraries built with 32k cache
TOOLCHAIN_NAME=xtensa-esp32s3-elf-5.1
# Libraries built with 16k cache (alternative)
#TOOLCHAIN_NAME=xtensa-esp32s3-elf-5.1-16k


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

# This target connects to inx-systems mqtt broker
include ./target/devman-configs/esp32s3-hrdcv2-inx-systems.com.mk


################################### END OF CONFIGURATION ###################################################
