# This file can be included bny any linux hardware toolchain.mk file



export CC_SWITCHES+=  -DANDROID 
#-MMD -MP -MF -fpic -ffunction-sections -funwind-tables -fstack-protector -D__ARM_ARCH_5__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__  -Wno-psabi -march=armv5te -mtune=xscale -msoft-float -mthumb -Os -fomit-frame-pointer -fno-strict-aliasing -finline-limit=64 -Wa,--noexecstack -O0 -g

#if buolding dll need this: -fpic
#-MMD -MP -MF -fpic -ffunction-sections -funwind-tables -fstack-protector -D__ARM_ARCH_5__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__  -Wno-psabi -march=armv5te -mtune=xscale -msoft-float -mthumb -Os -fomit-frame-pointer -fno-strict-aliasing -finline-limit=64 -Wa,--noexecstack -O0 -g 
#NDKs usual: -MMD -MP -MF -fpic -ffunction-sections -funwind-tables -fstack-protector -D__ARM_ARCH_5__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__  -Wno-psabi -march=armv5te -mtune=xscale -msoft-float -mthumb -Os -fomit-frame-pointer -fno-strict-aliasing -finline-limit=64 -Wa,--noexecstack -O0 -g

export LD_SWITCHES+= -lz -shared -lgcc -no-canonical-prefixes  -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now 


#-Wl,-soname,libnative_activity.so

#-Wl,--no-add-needed -Wl,-soname,libehs.so.1   -Wl,--as-needed -Wl,-whole-archive
#LIB+=gcc

include $(EHS_TARGETS_ROOT_PATH)/os-arch//gnu_ALL/toolchain.mk

override EXE = so
