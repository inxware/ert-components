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

EHS_DEBIAN_VERSION=8

SYSTEM_VARIANT=debian

#this needed to report the large cached download JSONs.
#DEFS+=EHS_STRING_LENGTH_MAX=8096

# MUST SET the following for any component config: 
#EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.

################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:
#DEBUG OPTIONS
EHS_DEBUGALL=yes

#use gst 0.10 fpr this build
DEFS += EHS_GST_010

################################### END OF TOOLBOX CONFIGURATION ###################################################
