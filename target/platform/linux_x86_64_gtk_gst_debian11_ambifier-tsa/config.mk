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

include ./target/platform/linux_x86_64_gtk_gst_debian11/config.mk

# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features 
# SYSTEM_VARIANT optionally indicates specific target environment confgurations.
# See target/envbuildscripts/targetenv_hacks_*.sh scripts).
SYSTEM_VARIANT=ambifier-deb11
DEBIAN_PACKAGE_NAME=ehs

################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:
#DEBUG OPTIONS
EHS_DEBUGALL=yes


################################################################################################################
# Define any OS install scripts that should be run on first install
################################################################################################################
HOST_OS_CONFIG_SCRIPTS+= \
0090-update-debian8-npt-www-ambifier-com \
0200-snd-add-csound-volume \
#0100-configure-ambifier-debian11 \ - not written yet - might need it for automounting USB sticks
#0090-configure-X-hotplug-inteli915 \
#0050-configure-jessie-repo-debian \
#0500-update-kernel-4.9-backport \
#0450-update-Realtek-net-firmware \
#0210-snd-tsa_std_volume \
#0501-patch-J1800-cstate-and-sata-speed

################################################################################################################
# Define the Application and IoT server configuration
################################################################################################################

include ./target/devman-configs/devman-tsa.inx-systems.net.mk
DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=yes
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=yes
#Application Selection
EHS_DEFAULT_APP=customer-apps/TSA/ambifier-v0.6.0-integrated
