#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_arm64_clang_A6_supervisor
# @author: inx limited

# Usage Description
# This is for Raspberry Pis - it is aimed at being used with inxware demo

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

include ./target/platform/linux_arm64_gtk_gst_gg_debian11/config.mk

#DEVMAN_SERVER_DOMAIN=devman.inx-systems.com
#DEVMAN_SERVER_PROTOCOL=https

include ./target/devman-configs/inx-systems.com.mk
DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=yes

#Packager
