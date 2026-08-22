#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for esp32s3_freertos-xtensa-hrdcv2C-debug-wifi
# @author: inx limited


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

include ./target/platform/esp32s3_freertos-xtensa-hrdcv2D/config.mk

EHS_DEBUGALL=yes

#----- Networking Features -----
EHS_NETWORK_ETHERNET_SUPPORT=yes
EHS_NETWORK_WIFI_SUPPORT=yes

#----- Console Features -----
EHS_SERIAL_CONSOLE_SUPPORT=yes


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

EHS_DEFAULT_APP=customer-apps/Backer/HRDC2D-hardware-test

#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

## This should follow the production hrdcv2D platform target
# # Set the ESP32S3-to-FTDI chip SPI clock (in Hz)
# DEFS += EHS_CONFIG_OVERRIDE_LCD_SPI_CLK=29000000
# 
# # Set the FTDI-to-LCD PCLK divider (60MHz / X)
# DEFS += EHS_CONFIG_OVERRIDE_LCD_PCLK=6
# 
# # Set the ESP32S3-to-FTDI chip SPI GPIO lines pin drive (from 0 (weakest) to 3 (strongest))
# DEFS += EHS_CONFIG_OVERRIDE_LCD_SPI_PINDRIVE=0
# 
# # Set the FTDI-to-LCD GPIO lines pin drive (from 0 (weakest) to 3 (strongest))
# DEFS += EHS_CONFIG_OVERRIDE_LCD_PINDRIVE=0

################################### END OF CONFIGURATION ###################################################
