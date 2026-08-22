#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_x86_gtk_gst_ambifier
# @author: inx limited

#Target Platform Uses
# For 32 bit linux running on Debian 7-9. This will not run ambifier2 capable OSs
# Built with compiled gtk and gstreamer libraries in ert-contrib-middleware
# Inherets the config for standard linux gtk and gst 

include ./target/platform/linux_x86_gtk_gst_debian8/config.mk

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################
# MUST SET the following for any component config: 
#EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.

SYSTEM_VARIANT=ambifier
EHS_DEBIAN_VERSION=8

################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:
#DEBUG OPTIONS
EHS_DEBUGALL=yes
DEFS+=EHS_LOGLEVEL_VERBOSE
################################### END OF TOOLBOX CONFIGURATION ###################################################

#Note this is supposed be the default but the default isn't working!
EHS_PERIPHERALS_GPIO_SUPPORT=stubbed

#EHS_GUI_SUPPORT=gtk_stub
#EHS_VIDEO_SUPPORT=no

#this needed to report the large cached download JSONs.
#DEFS+=EHS_STRING_LENGTH_MAX=8096

################################################################################################################
# Define any OS install scripts that should be run on first install
################################################################################################################
# see target/envtree/$EHS_OS-$EHS_ARCH/ for scripts available for configurating
#Removed install patches as will no loner be installing new version of this confif - just updating legacy devices
HOST_OS_CONFIG_SCRIPTS+= \
0101-configure-ambifier-deb8-gstreamer
#0090-update-debian8-npt-www-ambifier-com \
#0090-configure-X-hotplug-inteli915 \
#0050-configure-jessie-repo-debian \
#0100-configure-ambifier-debian \
#0500-update-kernel-4.9-backport \
#0450-update-Realtek-net-firmware \
#0200-snd-add-csound-volume \
#0210-snd-tsa_std_volume \
#0501-patch-J1800-cstate-and-sata-speed

################################################################################################################
# Define the Application and IoT server configuration
################################################################################################################

include ./target/devman-configs/devman-tsa.inx-systems.net.mk

DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=yes
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=yes
EHS_DEFAULT_APP=customer-apps/TSA/ambifier-v0.6.0-integrated
