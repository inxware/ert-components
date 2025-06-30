#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# This file can be included bny any linux hardware toolchain.mk file
# It does the following:
# 1. Setup the toolchain path (defaulting to HOST or EHS_DISTRO_VERSION [or EHS_DISTRO_VERSION] if TOOLCHAIN_NAME=HOST) 
#    This step identifies the build host's machine's architecture and selects the target from there 
# 2. Sets the basic compiler and linker flags to build and create an executable 
# 3. Sets the sysroot to eithr the toolchain's, ert-build-support or ert-contrib-middleware target_libs 
#    (Priority is given in this order depending on what is found at the expected paths)

CC_OVERRIDE=arm-none-eabi-g++
CXX_OVERRIDE=arm-none-eabi-g++

ifdef EHS_PLUGIN_LIBRARY_DEPENDENCY
# building as a static library
LINK_OVERRIDE=arm-none-eabi-ar -rv ehs_${TARGET}.a
else
LD_SWITCHES += -T $(EHS_TARGETS_ROOT_PATH)/os-arch/arduino-arm_mbednano/arduino_rp2040.ld
endif
include $(EHS_TARGETS_ROOT_PATH)/os-arch/arduino_ALL/toolchain.mk


# The final image needs a 256 2nd stage bootloader prepended as described here:
# https://blog.usedbytes.com/2021/12/pico-serial-bootloader/
# The general info for the PICO family of chips is here:
# https://github.com/raspberrypi/pico-sdk/blob/master/README.md

# Things that need to be added to the source:
# - secondary boot loader and script to concatenate it.
# - pinmap_common.c from the mbed SDK 
# - Still some memory align things - possibly need libc explicitly adding for malloc?
