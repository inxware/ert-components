#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for esp32s3_freertos-xtensa-base-debug
# @author: inx limited


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

include ./target/platform/esp32s3_freertos-xtensa-base/config.mk


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# CPU architecture, operating system, toolchain selection, and middleware dependencies
#################################################################################################################

# (Inherited from base)


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

# Enable full debug mode
EHS_DEBUGALL=yes
EHS_DEBUG_TCPIP_CONSOLE=yes

# Runtime logger
EHS_RUNTIME_LOGGER_ENABLED=no

# Memory management (uncomment to disable linked-list based allocator)
#EHS_MEMORY_MANAGMENT=none


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

# (Inherited from base)

EHS_NETWORK_WIFI_SUPPORT=yes
EHS_SERIAL_CONSOLE_SUPPORT=yes
EHS_NETWORK_BLE_SUPPORT=nimble

#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

# (Inherited from base)


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

# (Inherited from base)


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# (Inherited from base)


################################### END OF CONFIGURATION ###################################################

