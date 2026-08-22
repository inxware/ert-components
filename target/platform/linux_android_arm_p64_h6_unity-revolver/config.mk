#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_android_arm_p64_h6_unity-tellisign
# @author: inx limited

# Target Platform Uses
# Digital Signage Platform for Tellisign server (formerlly s-h-o-w.online.)

include ./target/platform/linux_android_arm_unity-tellisign/config.mk

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# SYSTEM_VARIANT optionally indicates specific target environment confgurations.
# See target/envbuildscripts/targetenv_hacks_*.sh scripts).
# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features
SYSTEM_VARIANT=pine64_h6

#TODO2023 - Use this as a template for how we should be configuring devman to begin removing the 100s of bash scripts that reinvent this
#note this is not currently implemented - needs exporting and bash scripts check for defaults
SSHPORT=8822
#Todo get a list of these - not sure what this actually does. it might help pick up the right certs (but these are usually URL based)?

################################### END OF TOOLBOX CONFIGURATION ###################################################

DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=none

include ./target/devman-configs/revolverdevman.inx-systems.net.mk

EHS_ANDROID_INSTALL_VERSION=9.0

BUILD_WITH_ANDROID_SUPERVISOR=yes

EHS_DEFAULT_APP=customer-apps/SimpleSignOn/sso-unity-v1.1.0
