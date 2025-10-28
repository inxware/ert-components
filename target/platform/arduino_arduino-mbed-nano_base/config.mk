#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_amd64
# @author: inx limited

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# MUST SET the following for any component config: 
#EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.
EHS_ARCH=arm_mbednano
EHS_OS=arduino

# TOOLCHAIN_NAME is an optional alternative location to find the toolchain. 
# Toolchain path defaults ../ert-build-support/<BUILD HOST TYPE>/$EHS_ 
TOOLCHAIN_NAME=arm-none-eabi
# apply arduino specific targetenv
INXWARE_TARGETENV_HACKS=arduino
# # Note: This is a host build so we don't ned it but will add it in case we fdo have any bits we may build for the target.
# COMPONENT_VARIANT=gtk_gst

################################################################################################################
# Configure debug/production levels
################################################################################################################
#DEBUG OPTIONS
# Set ALL debug use this:
#EHS_DEBUGALL=

################################################################################################################
# Enable or disable non-component networking support (e.g. socket debugging or Devman or none)
################################################################################################################

# EHS_NETWORKING_SUPPORT=all
# # To enable full TCPIP networking toolbox ("netx" DCC=3)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                   #
# EHS_COMPONENT_NETWORKING_SUPPORT=all
# 

# 
# ################################### END OF TOOLBOX CONFIGURATION ###################################################
# #DEVMAN_SERVER_DOMAIN=devman.inx-systems.com
# #DEVMAN_SERVER_PROTOCOL=https
# 
# include ./target/devman-configs/inx-systems.com.mk

EHS_DEFAULT_APP=NONE

EHS_DEBUGALL=

# Add arduin GPIO support
EHS_PERIPHERAL_DEVICE_SUPPORT=all
# this is causing memory corruption for some reason at the moment in arduino
EHS_PERIPHERALS_ADC_DAC_SUPPORT=arduino
EHS_PERIPHERALS_GPIO_SUPPORT=arduino
EHS_PERIPHERALS_PWM_SUPPORT=arduino
# disable continous, as it's not supported on arduino atm
EHS_PERIPHERALS_ADC_CONTINUOUS_SUPPORT=none

# enable console I/O e.g. 'console print' function block
EHS_COMPONENTS_CONSOLE_IO=yes

#This is not a good way to do things
# This is the default for all Arduino targets (for now) DEFS += EHS_MCU_TARGET=1
# we use mbed file system for this traget
EHS_FILESYSTEM_SUPPORT=yes
#EHS_FILESYSTEM_SUPPORT=stubbed

#We will build all of ehs as C++ so we don't have linking nightmares.
#todo2026 change these to soemthing that fits our normal pattern for building/not building things..
EHS_SKIP_APPLICATION_INFO_GETTER=1

#todo the following should be changed to a ="none" intead like below
EHS_EXCLUDE_XML_PARSER=1

#EHS_PERIPHERALS_BACKLIGHT_SUPPORT=stubbed

DEFS += EHS_NO_LIBXML2_SUPPORT=1
EHS_NO_LIBXML2_SUPPORT=1 

EHS_SKIP_GNULIBRARIES=yes

DEFS+=EHS_ARDUINO_SUPPORT=1

# ehs_float as float, not as double
DEFS += EHS_FLOAT_AS_FLOAT_TYPE=1

# We don't use main args
EHS_TARGET_NO_MAIN_ARGS=yes
