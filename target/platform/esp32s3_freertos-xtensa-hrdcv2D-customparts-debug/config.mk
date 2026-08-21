#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# Example target demonstrating ert-components-driven ESP32 partition layout.
#
# Setting ESP32_FLASH_SIZE opts this target into the locally-generated
# partition table: targetenv_esp32.sh runs esp32_gen_partitions.py to
# produce partitions.csv from the ESP32_PART_* variables below, then runs
# gen_esp32part.py to assemble partition-table.bin and derives every
# merge_bin offset from the same CSV. Without ESP32_FLASH_SIZE the legacy
# ert-contrib-middleware-supplied partition-table.bin is used unchanged.
#
# The middleware-built bootloader is reused as-is across flash sizes:
# CONFIG_ESPTOOLPY_FLASHSIZE in the IDF bootloader build is a build-system
# string only (no C-code references); the bootloader reads the actual size
# from the image header at runtime via update_flash_config().
#
# @author: inx limited


#################################################################################################################
# Baseline Configuration Includes
#################################################################################################################

include ./target/platform/esp32s3_freertos-xtensa-hrdcv2D-hwtest-debug/config.mk


#################################################################################################################
# ESP32 Custom Partition Layout
# Setting ESP32_FLASH_SIZE activates local partition-table generation.
# All ESP32_PART_* sizes accept esptool size strings: "1.8M", "2M", "0x80000", etc.
#################################################################################################################

ESP32_FLASH_SIZE          := 8MB
ESP32_PART_OTA_ENABLED    := no
ESP32_PART_FACTORY_SIZE   := 4M
ESP32_PART_STORAGE_SIZE   := 2M
ESP32_PART_APPDATA_SIZE   := 0x80000

# Defaults applied when omitted:
#   ESP32_PART_NVS_SIZE      := 0x4000
#   ESP32_PART_PHY_INIT_SIZE := 0x1000
# Required when ESP32_PART_OTA_ENABLED=yes:
#   ESP32_PART_OTA_SIZE      := 1.8M


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# Set the ESP32S3-to-FTDI chip SPI clock (in Hz)
DEFS += EHS_CONFIG_OVERRIDE_LCD_SPI_CLK=29000000

# Set the FTDI-to-LCD PCLK divider (60MHz / X)
DEFS += EHS_CONFIG_OVERRIDE_LCD_PCLK=6

# Set the ESP32S3-to-FTDI chip SPI GPIO lines pin drive (from 0 (weakest) to 3 (strongest))
DEFS += EHS_CONFIG_OVERRIDE_LCD_SPI_PINDRIVE=0

# Set the FTDI-to-LCD GPIO lines pin drive (from 0 (weakest) to 3 (strongest))
DEFS += EHS_CONFIG_OVERRIDE_LCD_PINDRIVE=0

################################### END OF CONFIGURATION ###################################################
