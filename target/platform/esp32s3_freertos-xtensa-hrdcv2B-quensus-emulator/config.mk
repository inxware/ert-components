#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for esp32s3_freertos-xtensa-hrdcv2B-inx-devman
# @author: inx limited


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

include ./target/platform/esp32s3_freertos-xtensa-hrdcv2B-inx-devman-debug/config.mk

EHS_DEBUGALL=

EHS_TARGET_APPLOAD_RESTARTING_REBOOT=none

#EHS_DEFAULT_APP=customer-apps/Quensus/Quensus_flow_sensor_emulate
EHS_DEFAULT_APP=customer-apps/Quensus/emulator-simple
#EHS_DEFAULT_APP=tutorials/hello_world

################################### END OF CONFIGURATION ###################################################
