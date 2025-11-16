#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#


# config.mk - Configuration properties of the current platform
# 
# Called by ../../../Makefile
#

# @author: inx limited


#DEBUG OPTIONS
EHS_DEBUGALL=true

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# MUST SET the following for any component config: 
#EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.
EHS_ARCH=x86
EHS_OS=linux

EHS_GNU_ARCH=amd64
EHS_GNU_OS=linux-gnu

TOOLCHAIN_NAME=HOST
CC_OVERRIDE=clang
LINK_OVERRIDE=llvm-ar
LD_SWITCHES=-rv ${TARGET_NAME}.${FINAL}
#todo the above should be sone in ert-components if the LINK OVERRIDE = llvm?

