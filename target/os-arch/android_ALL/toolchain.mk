#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

ifndef EHS_ANDROID_API
EHS_ANDROID_API=30
endif

# We don't need this assume clang sets this for us: DEFS+=__ANDROID_API__=$(EHS_ANDROID_API)

# todo the following is the same as DEFS+= ?
CFLAGS+=  -DANDROID 


#-MMD -MP -MF -fpic -ffunction-sections -funwind-tables -fstack-protector -D__ARM_ARCH_5__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__  -Wno-psabi -march=armv5te -mtune=xscale -msoft-float -mthumb -Os -fomit-frame-pointer -fno-strict-aliasing -finline-limit=64 -Wa,--noexecstack -O0 -g

#if building dll need this: -fpic
#-MMD -MP -MF -fpic -ffunction-sections -funwind-tables -fstack-protector -D__ARM_ARCH_5__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__  -Wno-psabi -march=armv5te -mtune=xscale -msoft-float -mthumb -Os -fomit-frame-pointer -fno-strict-aliasing -finline-limit=64 -Wa,--noexecstack -O0 -g 
#NDKs usual: -MMD -MP -MF -fpic -ffunction-sections -funwind-tables -fstack-protector -D__ARM_ARCH_5__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__  -Wno-psabi -march=armv5te -mtune=xscale -msoft-float -mthumb -Os -fomit-frame-pointer -fno-strict-aliasing -finline-limit=64 -Wa,--noexecstack -O0 -g


# Note: --no-undefined is intentionally omitted for Android .so builds.
# When liblitert_c.a is linked (compiled with NDK r27c), it references newer
# libc++ vtable symbols (basic_filebuf, basic_ofstream, etc.) that NDK r27c
# moved out-of-line into libc++_shared.so. The ert-build-support clang 9 linker
# stub (NDK r19 era) doesn't advertise these, so --no-undefined would fail at
# link time even though the symbols are present at runtime on Android 11+.
# To use --no-undefined again: align the ert-components Android toolchain with
# NDK r27c (update ert-build-support) or build in the same Docker container.
LD_SWITCHES+=-shared -no-canonical-prefixes  -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now
LIB+=z

override EXE = so

#Pretty much do the same as the GNI toolchain 
include $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/toolchain.mk

