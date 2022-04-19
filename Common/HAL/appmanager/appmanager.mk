# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code
#  EHS_TARGET_DEVMAN_MON_PATH - path to the current directory (set by platform makefile)

# include sourcecode from this dir in build
include $(EHS_COMMON_HAL_PATH)/appmanager/deps.mk
VPATH+=$(EHS_COMMON_HAL_PATH)/appmanager
ifdef EHS_DEVMAN_MON_SUPPORT
OBJECTS += devappget.$(OBJ) 
endif
OBJECTS += appstorage.$(OBJ) 