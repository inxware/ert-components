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

include ./target/platform/arduino_arduino-mbed-nano_base/config.mk

##########################################
####### EHS_NETWORKING_SUPPORT ###########
##########################################
EHS_COMPONENT_NETWORKING_SUPPORT=no-curl
# exclude components which contain curl
EHS_COMPONENTS_NETWORK_URL_GET=none
EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER=none
# @TODO - implement later
EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT=none
# use offical Arduino WiFiNINA for comms
EHS_COMMS_API_SUPPORT=arduino_nina
# @TODO - fix abstraction so this can be again yes/no
EHS_DEBUG_TCPIP_CONSOLE=target_specific
EHS_COMMS_TASK=tcp_server_common

EHS_PERIPHERALS_PWM_SUPPORT=stubbed

# include tcp/ip clinet & server socket fb components
#EHS_COMPONENTS_NETWORK_TCPIP_SOCKET=yes
DEFS += EHS_DEBUG_CONSOLE_BUFFER_SIZE=256
DEFS += EHS_DEBUG_CONSOLE_THREAD_STACK_SIZE=2048
DEFS += EHS_TGT_TCP_IN_BUFF_SIZE=256
DEFS += EHS_TGT_TCP_OUT_BUFF_SIZE=256
##########################################

# Bake-in WiFi credentials for Arduino device.
# Replace 'ssid' and 'password' in the DEFS below.
#DEFS += EHS_CONFIG_WIFI_SSID="\"ssid\""
#DEFS += EHS_CONFIG_WIFI_PASSWORD="\"password\""

EHS_MQTT_SUPPORT=arduino

# Use LEDs from RP2040 WiFiNINA
EHS_PERIPHERALS_LED_SUPPORT=arduino_nina
# Use accelometer and gyroscope library (Arduino_LSM6DS3@1.0.3)
EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT=arduino

# appland deployment config
EHS_APPLAND_INST_SUPPORT=yes
EHS_APPLAND_INST_DEPLOY_NAME=Arduino-MBED-Nano-Community
EHS_APPLAND_INST_OS_NAME=arduino-mbed-nano
