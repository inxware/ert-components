#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_amd64_gtk_gst_ambifier2_vagrant
# @author: inx limited

#Inherits the Debian 9 gcc-based 64 bot build to run Unity Ambifier2 player (docker toolchain)
# This was formely built in a VM and was called "linux_amd64_gtk_gst_ambifier2_vagrant"
#Addes the ambifier server, OS patches and application config

include ./target/platform/linux_amd64_gtk_gst_debian9/config.mk


################################################################################################################
# Define any OS install scripts that should be run on first install
################################################################################################################
HOST_OS_CONFIG_SCRIPTS+= \
0090-update-debian8-npt-www-ambifier-com \
0100-configure-ambifier-debian9 \
0200-snd-add-csound-volume \
#0210-snd-volume_max
#0090-configure-X-hotplug-inteli915 \
#0050-configure-jessie-repo-debian \

#0500-update-kernel-4.9-backport \
#0450-update-Realtek-net-firmware \
#0200-snd-add-csound-volume \
#0210-snd-tsa_std_volume \
#0501-patch-J1800-cstate-and-sata-speed

#Need this so it is not the debug version?
DEBIAN_PACKAGE_NAME=ehs
EHS_DEBUGALL=true

################### Configure the app and server ##################################################
include ./target/devman-configs/devman-tsa.inx-systems.net.mk

SYSTEM_VARIANT=ambifier2

DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=yes
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=yes
EHS_DEFAULT_APP=customer-apps/TSA/ambifier-v0.6.0-integrated
