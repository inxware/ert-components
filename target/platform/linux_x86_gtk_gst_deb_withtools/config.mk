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

include ./target/platform/linux_x86_gtk_gst_debian8/config.mk
#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################
#overrides the deb only variant var
INXWARE_TARGETENV_HACKS=debian_tools
EHS_DEBIAN_VERSION=8

#this needed to report the large cached download JSONs.
#DEFS+=EHS_STRING_LENGTH_MAX=8096

################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:
#DEBUG OPTIONS
EHS_DEBUGALL=true
#EHS_RUNTIME_LOGGER_ENABLED-yes

################################### END OF TOOLBOX CONFIGURATION ###################################################
#DO we still need this?
DEFS += EHS_GST_010
