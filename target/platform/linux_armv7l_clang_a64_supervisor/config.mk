#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_armv7l_clang_gtk_msg200_supervisor
# @author: inx limited

#Target Platform Uses
# This is built on the horrific arm / raspberry pi config with scraped components and uses a clang compiler 
# !!!!!!!! I don't think this is the version we actiually USE!!!!!!!!!!!!!
# !!!!!!!! Use this one: linux_arm64_clang_a64_supervisor !!!!!!!!!!!!!!!!!!!!!!!!!!
# it is for the MSG200 supervisor channel for WDS support of devices

include ./target/platform/linux_armv7l_clang_debian10/config.mk

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################


EHS_PERIPHERALS_GPIO_SUPPORT=sysfs_linux_arm
EHS_PERIPHERALS_ADC_DAC_SUPPORT=SPI_A6_LTC241X

#todo - what does this do and can we get rid of it?
DEFS += EHS_ADC_ARMBIAN_MSG200


#DEVMAN_SERVER_DOMAIN=devman.inx-systems.com
#DEVMAN_SERVER_PROTOCOL=https

include ./target/devman-configs/inx-systems.com.mk
