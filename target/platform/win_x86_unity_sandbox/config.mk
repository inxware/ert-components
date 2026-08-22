#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for win_x86_unity_revolver
# @author: inx limited

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

include ./target/platform/win_x86_unity/config.mk

ERT_NSIS_EXE_NAME=Unity-eRT-Sandbox

# overwrite server
include ./target/devman-configs/sandbox.inx-systems.net.mk
DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=none
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=no


################################### END OF TOOLBOX CONFIGURATION ###################################################
