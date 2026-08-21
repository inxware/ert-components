#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#


# @file config.mk 
# inxware ERT configuration file for nxp_arm_patrick_hri_debug
# @author: inx limited

#################################################################################################################
# Set general architecture and OS version
#################################################################################################################


EXCLUDE_EHS_COMMON=1
HEATROD_CONTROLLER_PROJECT=1

EHS_ARCH=arm
EHS_OS=nxp-redlib-freertos

TOOLCHAIN_NAME=arm-nxp
CC_OVERRIDE=arm-none-eabi-gcc
#EHS_CLIB_OVERRIDE_PATH=nxp-arm

DEFS += CONFIG_FIRMWARE_VERSION=\"2.0.825\"

DEFS += INX_NXP_KENETIS_BOARD_VARIANT=INX_HR_HRi
DEFS += CONFIG_MQTT_LOCATION=CONFIG_MQTT_LOCATION_PATRICK
DEFS += CONFIG_LIFESTYLE=INX_LIFESTYLE_DEBUG
DEFS += INX_HEATROD_IOT_BOARD_SUBVARIANT=INX_HR_HRi_DISPv1

################################### END OF TOOLBOX CONFIGURATION ###################################################
