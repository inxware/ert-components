#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_x86_64-lucid-debian11
# @author: inx limited

# this target gets included in inxware lucid tools

include ./target/platform/linux_x86_64_clang_lvgl_debian11/config.mk

EHS_DEBUGALL=true

# use gui gpio for the lucid target
EHS_PERIPHERALS_GPIO_SUPPORT=gui
EHS_PID_SUPPORT=gnu
EHS_SCHEDULER_SUPPORT=1
# this forces eRT to always launch app in default directory when the program launches
#DEFS += EHS_ALWAYS_START_DEFAULT_APP

# enable eRT1 support
ERT_SODL_VERSION=1

