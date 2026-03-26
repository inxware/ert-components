#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for linux_x86_gtk_gst_debian13 - general build shoud run on most debain13s
# @author: inx limited


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################


### TODO WE NEED A BASELINE debian 13 PC docker image.... not done that yet.
include ./target/platform/linux_x86_64_clang_lvgl_debian13-no-certs/config.mk


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# CPU architecture, operating system, toolchain selection, and middleware dependencies
#################################################################################################################

# (Inherits from parent config)

#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

# (Inherits from parent config)

#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

# (Inherits from parent config)
# LoRaWAN modem support (WIO-E5 via UART/serial)
EHS_LORAWAN_SUPPORT=wio_e5



#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

EHS_PACKAGER_TYPE=deb

# this forces eRT to always launch app in default directory when the program launches
EHS_DEFAULT_APP=systemapps/Home


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################



#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# (Inherits from parent config)


################################### END OF CONFIGURATION ###################################################
