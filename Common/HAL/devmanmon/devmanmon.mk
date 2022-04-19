# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code
#  EHS_TARGET_DEVMAN_MON_PATH - path to the current directory (set by platform makefile)

# Make sure any code switches for this are active
DEFS+= EHS_DEVMAN_MON_SUPPORT

# include sourcecode from this dir in build
include $(EHS_COMMON_HAL_PATH)/devmanmon/deps.mk
VPATH+=$(EHS_COMMON_HAL_PATH)/devmanmon
OBJECTS += devman_mon.$(OBJ) 

#@todo these are already included use =: ?
##LIB+=pthread
