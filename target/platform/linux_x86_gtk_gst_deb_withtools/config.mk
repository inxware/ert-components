#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_x86_gtk_gst_deb
# @author: inx limited

include ./target/platform/linux_x86_gtk_gst/config.mk
#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################
#overrides the deb only variant var
export SYSTEM_VARIANT=debian_tools
export EHS_DEBIAN_VERSION=8

################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:
#DEBUG OPTIONS
#EHS_DEBUGALL=true
ifdef EHS_DEBUGALL
# Or use one of the more fine-grained debug congurations
# Or enable only stdout & serial console logging
DEFS += EHS_RUNTIME_LOGGER_ENABLED
DEFS += EHS_DEBUG_AV
export EHS_DEBUGALL=yes
endif

################################### END OF TOOLBOX CONFIGURATION ###################################################
DEFS += EHS_GST_010
