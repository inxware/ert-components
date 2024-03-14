#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

ifndef EHS_ANDROID_API
EHS_ANDROID_API=30
endif

# We don't need this assume clang sets this for us: DEFS+=__ANDROID_API__=$(EHS_ANDROID_API)

# todo the following is the same as DEFS+= ?
CC_SWITCHES+=  -DANDROID 


#-MMD -MP -MF -fpic -ffunction-sections -funwind-tables -fstack-protector -D__ARM_ARCH_5__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__  -Wno-psabi -march=armv5te -mtune=xscale -msoft-float -mthumb -Os -fomit-frame-pointer -fno-strict-aliasing -finline-limit=64 -Wa,--noexecstack -O0 -g

#if buolding dll need this: -fpic
#-MMD -MP -MF -fpic -ffunction-sections -funwind-tables -fstack-protector -D__ARM_ARCH_5__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__  -Wno-psabi -march=armv5te -mtune=xscale -msoft-float -mthumb -Os -fomit-frame-pointer -fno-strict-aliasing -finline-limit=64 -Wa,--noexecstack -O0 -g 
#NDKs usual: -MMD -MP -MF -fpic -ffunction-sections -funwind-tables -fstack-protector -D__ARM_ARCH_5__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__  -Wno-psabi -march=armv5te -mtune=xscale -msoft-float -mthumb -Os -fomit-frame-pointer -fno-strict-aliasing -finline-limit=64 -Wa,--noexecstack -O0 -g


LD_SWITCHES+=-shared -no-canonical-prefixes  -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now 
LIB+=z

override EXE = so

#Pretty much do the same as the GNI toolchain 
include $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/toolchain.mk

