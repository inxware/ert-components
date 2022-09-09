#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#


# @file config.mk 
# inxware ERT configuration file for nxp_arm_inx_hri_ehs_debug
# @author: inx limited


#################################################################################################################
# Set general architecture and OS version
#################################################################################################################

export EHS_ARCH=arm
export EHS_OS=nxp-redlib-freertos
#export EHS_SKIP_GNULIBRARIES=1

export TOOLCHAIN_NAME=arm-nxp

export CC_OVERRIDE=arm-none-eabi-gcc


################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:
#todo2022 the following should be in the os_arch? 
export EHS_DEBUG_TCPIP_CONSOLE ## this and the next need to be tied together

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################

# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
#Select toptional toolboxes
export EHS_PERIPHERAL_DEVICE_SUPPORT=all
export EHS_PERIPHERALS_GPIO=yes

export EHS_MQTT_SUPPORT=lwip

################################### END OF TOOLBOX CONFIGURATION ###################################################

DEFS += INX_HEATROD_IOT_BOARD_VARIANT=INX_HR_HRi
DEFS += CONFIG_MQTT_LOCATION=CONFIG_MQTT_LOCATION_INX
DEFS += CONFIG_LIFESTYLE=INX_LIFESTYLE_EHS

#todo2022 rmov th folloing should b don in os-arch
#export EHS_COMMS_TASK=tcp_server_common
#export EHS_COMMS_API_SUPPORT=lwip

