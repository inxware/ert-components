#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_x86_gtk_gst_ambifier2_debian11
# @author: inx limited

#Target Platform Uses
# For 64 bit linux running on Debian 11, which can run the normal Unity ambifier2 player
# All contributed middlware is provided by the container container inxware/inx-debian11

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

#todo we need to know if this is still needed
INXWARE_TARGETENV_HACKS=ambifier2
EHSRT1=yes
EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER=yes


################### Configure the app and server ##################################################
include ./target/devman-configs/ambifier.com.mk

SYSTEM_VARIANT=ambifier2

DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=yes
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=yes
EHS_DEFAULT_APP=customer-apps/TSA/ambifier-v0.6.0-integrated
