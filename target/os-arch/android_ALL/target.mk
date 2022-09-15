# HW independent linux build environment parameters 



#target types are always the same for all linux so just use one file
INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/os-arch/android_ALL/
VPATH += $(EHS_TARGETS_ROOT_PATH)/os-arch/android_ALL/

ifndef EHS_COMMS_API_SUPPORT
	export  EHS_COMMS_API_SUPPORT=bsdsockets
	DEFS += EHS_COMMS_API_SUPPORT_BSDSOCKETS
    DEFS += EHS_COMMS_API_SUPPORT
endif


#OpenGl2.0
LIB += log 

#todo get rid of the EHS_ANDROID_JNI label and use system variant instead
ifdef EHS_ANDROID_JNI
LIB += GLESv2
else
LIB += GLESv1_CM
LIB += android 
LIB += EGL
endif
#if we want OpenGl1.1 options...
OBJECTS += target_file.$(OBJ)
OBJECTS += target_process.$(OBJ) 
OBJECTS += target_main.$(OBJ)
OBJECTS += target_math.$(OBJ) 
OBJECTS += target_net.$(OBJ) 
OBJECTS += JNISysInfoInterface.$(OBJ) 
#todo2023 - do we really want to use the A6 GPIO as a default for all android?
# we proably want to make this GPIO file A6 specific and have a few options depending on the value of EHS_PERIPHERALS_GPIO
ifdef EHS_PERIPHERALS_GPIO
ifneq ($(EHS_PERIPHERALS_GPIO),stubbed)
OBJECTS += target_gpio.$(OBJ)
endif
endif
ifndef EHS_ANDROID_JNI
OBJECTS += android_native_app_glue.$(OBJ)
endif

