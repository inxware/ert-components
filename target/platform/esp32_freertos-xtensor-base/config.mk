#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for esp32_freertos-xtensor-base
# @author: inx limited

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# MUST SET the following for any component config: 
# EHS Section 
# ehs is more generic
EHS_ARCH=xtensa
EHS_OS=esp32_freertos
# TOOLCHAIN_NAME is an optional alternative location to find the toolchain. 
# Toolchain path defaults ../ert-build-support/<BUILD HOST TYPE>/$EHS_ 
TOOLCHAIN_NAME=xtensa-esp32-elf-4.4.1

################################################################################################################
# Configure debug/production levels
################################################################################################################

export EHS_ESP32=yes

DEFS += EHS_ESP32

EHS_DEBUGALL=true
#todo remove the following DEFS if 
DEFS += EHS_RUNTIME_LOGGER_ENABLED
# set EHS_DEBUG_AV for verbose debugg from the media sub system
DEFS += EHS_DEBUG_AV
# Set ALL debug use this:
EHS_DEBUGALL=yes


################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################

# Note the following might not remain lwip - we could use esp32's native mqtt component instead of ert#s lwip based one (TBC if this is difficult
DEFS += EHS_NO_LIBXML2_SUPPORT=1
EHS_MQTT_SUPPORT=lwip
export EHS_PERIPHERAL_DEVICE_SUPPORT=all
export EHS_PERIPHERALS_GPIO=yes

# we may also no use any lwip stuff sirectly in EHS
DEFS += EHS_LWIP

################################### END OF TOOLBOX CONFIGURATION ###################################################
