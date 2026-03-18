# Sfera Labs UART — add the shared Sfera Labs board definitions to the include path.
# VPATH, INC_DIRS for this directory, and OBJECTS are set by uart_common.mk.
INC_DIRS += $(EHS_TARGET_COMPONENT_HAL_PATH)/sferalabs
# Background receive thread requires pthreads.
# Always anyway for linuix :LIBS += -lpthread
