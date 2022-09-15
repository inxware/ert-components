#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#


#  Uses $(INC_DIRS), $(TARGET_NAME), $(DEFS)
#  Defines $(CC), $(LINK), $(CFLAGS), $(LNKFLAGS), $(INC), $(LIB), $(EXE), $(OBJ), $(FINAL)

# Toolset specific 

#We have floating Point
LNKFLAGS+= -Wl,-lm #@todo this isn't for all arms - should move to platform

##
## Linker Options
##

include $(EHS_TARGETS_ROOT_PATH)/os-arch/android_ALL/toolchain.mk # toolchain is gnu
include $(EHS_TARGETS_ROOT_PATH)/os-arch/linux_ALL/toolchain.mk # in case we have any linux extras..
