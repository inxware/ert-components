#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for esp32s3_freertos-xtensa-hrdcv2B-debug
# @author: inx limited


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

include ./target/platform/esp32s3_freertos-xtensa-hrdcv2B/config.mk


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

EHS_DEBUGALL=yes
EHS_DEBUG_TCPIP_CONSOLE=yes
EHS_RUNTIME_LOGGER_ENABLED=no

# Nanoprintf support for debug
DEFS += EHS_NANOPRINTF_SUPPORT=1

# Memory management (uncomment to disable linked-list based allocator)
#EHS_MEMORY_MANAGMENT=none


################################### END OF CONFIGURATION ###################################################
