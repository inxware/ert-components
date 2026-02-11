#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for linux_x86_gtk_gst_deb
# @author: inx limited


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

include ./target/platform/linux_x86_gtk_gst_debian8/config.mk


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# CPU architecture, operating system, toolchain selection, and middleware dependencies
#################################################################################################################

#overrides the deb only variant var
INXWARE_TARGETENV_HACKS=debian_tools
EHS_DEBIAN_VERSION=8


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

# Debug/Production mode
EHS_DEBUGALL=true
#EHS_RUNTIME_LOGGER_ENABLED-yes


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

# (Inherits from parent config)


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

# (Inherits from parent config)


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

# (Inherits from parent config)


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

#this needed to report the large cached download JSONs.
#DEFS+=EHS_STRING_LENGTH_MAX=8096

#DO we still need this?
DEFS += EHS_GST_010


################################### END OF CONFIGURATION ###################################################
