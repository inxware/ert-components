# HW independent linux build environment parameters 


#target types are always the same for all linux so just use one file
INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/
VPATH += $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/

OBJECTS += target_file.$(OBJ)
OBJECTS += target_process.$(OBJ) 
OBJECTS += target_main.$(OBJ)
OBJECTS += target_math.$(OBJ) 
