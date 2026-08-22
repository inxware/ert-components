#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_android_arm_unity-tellisign-revolver
# @author: inx limited

# Target Platform Uses
# Digital Signage Platform for Tellisign server (formerlly s-h-o-w.online.)

include ./target/platform/linux_android_arm_unity-tellisign/config.mk

EHS_DEFAULT_APP=customer-apps/SimpleSignOn/sso-unity-v1.1.0

# Server config
DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=none

include ./target/devman-configs/revolverdevman.inx-systems.net.mk
