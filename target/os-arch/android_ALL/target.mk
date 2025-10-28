#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

#include the default cofiguration as this a base configuration.
include $(EHS_TARGETS_ROOT_PATH)/os-arch/android_ALL/config.mk

#target types are always the same for all linux so just use one file
INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/os-arch/android_ALL/
VPATH += $(EHS_TARGETS_ROOT_PATH)/os-arch/android_ALL/


#Default to Android 9 if nothing else set.
ifdef EHS_ANDROID_INSTALL_VERSION
	export EHS_ANDROID_INSTALL_VERSION
	ifeq ($(EHS_ANDROID_INSTALL_VERSION) , 7.1)
		DEFS+=EHS_ANDROID_INSTALL_VERSION=7
	endif
else
	export EHS_ANDROID_INSTALL_VERSION=9.0
	DEFS+=EHS_ANDROID_INSTALL_VERSION=9
endif

# Targe speciic libraries needed.

#OpenGl2.0
LIB += log 
#todo get rid of the EHS_ANDROID_JNI label and use system variant instead
ifdef EHS_ANDROID_JNI
	LIB += GLESv2
else
	LIB += GLESv1_CM
	LIB += android 
	LIB += EGL
	LIB += OpenSLES
	LIB += log
endif
LIB+=:libarchive.a

# Generic target options we usually have
ifneq ($(EHS_FILESYSTEM_SUPPORT),stubbed)
OBJECTS += target_file.$(OBJ)
endif
OBJECTS += target_process.$(OBJ) 
OBJECTS += target_main.$(OBJ)
OBJECTS += target_math.$(OBJ) 
OBJECTS += target_net.$(OBJ) 
OBJECTS += JNISysInfoInterface.$(OBJ) 


#This is needed ifwe are using a pure native NDK app.
ifndef EHS_ANDROID_JNI
	OBJECTS += android_native_app_glue.$(OBJ)
endif

# TODO This should be conditional?
OBJECTS += target_audio.${OBJ}

# OBJECTS += target_sys_stat.$(OBJ)
