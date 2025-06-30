#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_amd64
# @author: inx limited

#################################################################################################################
# Set general architecture and OS version
#################################################################################################################

include ./target/platform/arduino_arduino-mbed-nano_lib/config.mk


# appland deployment config
EHS_APPLAND_INST_SUPPORT=yes
EHS_APPLAND_INST_DEPLOY_NAME=Arduino-MBED-Nano-Community-Library
EHS_APPLAND_INST_OS_NAME=arduino-mbed-nano