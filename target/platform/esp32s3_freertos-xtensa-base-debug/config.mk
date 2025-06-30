#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for esp32_freertos-xtensor-base-debug
# @author: inx limited

#################################################################################################################
# Set general architecture and OS version
#################################################################################################################

include ./target/platform/esp32s3_freertos-xtensa-base/config.mk

# Enable debug mode
EHS_DEBUGALL=yes

EHS_RUNTIME_LOGGER_ENABLED=no

# Enable linked-list based memory allocator managment in the hal_mem
#EHS_MEMORY_MANAGMENT=yes

################################### END OF TOOLBOX CONFIGURATION ###################################################
