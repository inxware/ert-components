# copyright INX Ltd 2010
# Target-specific definitions .
# This file is called from Makefile
#
#  Uses $(INC_DIRS), $(TARGET_NAME), $(DEFS)
#  Defines $(CC), $(LINK), $(CFLAGS), $(LNKFLAGS), $(INC), $(LIB), $(EXE), $(OBJ), $(FINAL)
#

#
# Toolset specific 

##
## Linker Options
##
#We have floating Point
LNKFLAGS+= -Wl,-lm

include $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/toolchain.mk # toolchain is gnu
include $(EHS_TARGETS_ROOT_PATH)/os-arch/linux_ALL/toolchain.mk # in case we have any linux extras..
