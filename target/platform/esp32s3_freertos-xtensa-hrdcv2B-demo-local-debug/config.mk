#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for esp32s3_freertos-xtensa-hrdcv2B-demo-local-debug
# @author: inx limited


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

include ./target/platform/esp32s3_freertos-xtensa-hrdcv2B-debug/config.mk


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# CPU architecture, operating system, toolchain selection, and middleware dependencies
#################################################################################################################

# Libraries built with 32k cache
TOOLCHAIN_NAME=xtensa-esp32s3-elf-5.1
# Libraries built with 16k cache (alternative)
#TOOLCHAIN_NAME=xtensa-esp32s3-elf-5.1-16k


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

# Devman disabled for local demo
EHS_DEVMAN_SUPPORT=none


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

EHS_DEFAULT_APP=events/microelectronics-uk-2025/hello_world
#EHS_DEFAULT_APP=tutorials/hello_world
#EHS_DEFAULT_APP=customer-apps/Backer/hrdcv2_app


################################### END OF CONFIGURATION ###################################################
