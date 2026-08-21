#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for nxp_arm_inx_hrcdispv1_ehs_debug
# @author: inx limited

#################################################################################################################
# Set general architecture and OS version
#################################################################################################################

#don't build the normal HR controller
HEATROD_CONTROLLER_PROJECT=0
# CPU and OS Type Selection 
EHS_ARCH=arm
EHS_OS=nxp-redlib-freertos

CC_OVERRIDE=arm-none-eabi-gcc

TOOLCHAIN_NAME=arm-nxp

################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:
#todo2022 the following should be in the os_arch? 
EHS_DEBUG_TCPIP_CONSOLE=yes
#EHS_DEBUGALL=yes

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################

# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )    
EHS_PERIPHERAL_DEVICE_SUPPORT=all
EHS_PERIPHERALS_GPIO_SUPPORT=NXP_K64
EHS_MQTT_SUPPORT=lwip_nxp

#todo2022 we need to fix the common info getter code to note use sys/types.h and other libc headers that aren't normal. 
EHS_SKIP_APPLICATION_INFO_GETTER=y

################################### END OF TOOLBOX CONFIGURATION ###################################################
EHS_NXP_SUPPORT=yes
DEFS += EHS_NXP_SUPPORT=1
DEFS += EHS_LWIP=1

DEFS += INX_NXP_KENETIS_BOARD_VARIANT=INX_HR_HRc
DEFS += CONFIG_MQTT_LOCATION=CONFIG_MQTT_LOCATION_INX
DEFS += CONFIG_LIFESTYLE=INX_LIFESTYLE_EHS

# enable eRT1 support
ERT_SODL_VERSION=1

################################### END OF TOOLBOX CONFIGURATION ###################################################
