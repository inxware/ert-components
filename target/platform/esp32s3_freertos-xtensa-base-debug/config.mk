#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for esp32_freertos-xtensor-base-debug
# @author: inx limited

#################################################################################################################
# Set general architecture and OS version
#################################################################################################################

include ./target/platform/esp32s3_freertos-xtensa-base/config.mk

# blanket switch everything on.
EHS_DEBUGALL=yes
EHS_DEBUG_TCPIP_CONSOLE=yes

EHS_RUNTIME_LOGGER_ENABLED=no

# Disable Enable linked-list based memory allocator managment in the hal_mem
#EHS_MEMORY_MANAGMENT=none

################################### END OF TOOLBOX CONFIGURATION ###################################################
