#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for win_x86_32-lucid-win10
# @author: inx limited

# this gets included in inxware lucid tools

# Used for specifing name of the eRT package/executable
ERT_PACKAGE_NAME=ehs
# Used for specifing user facing name of installed application (windows installer)
ERT_NSIS_EXE_NAME=eRT

include ./target/platform/win_x86_lvgl/config.mk
DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=none
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=no

# Machine Leaning & Machine Vision Support
EHS_ML_SUPPORT=stubbed
EHS_MV_SUPPORT=stubbed

# enable eRT1 support
ERT_SODL_VERSION=1

include ./target/devman-configs/devman.inxware.io.mk

# appland deployment config
EHS_APPLAND_INST_SUPPORT=yes
EHS_APPLAND_INST_DEPLOY_NAME=PC-Windows-32bit
EHS_APPLAND_INST_OS_NAME=windows