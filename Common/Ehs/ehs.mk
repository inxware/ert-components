#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

#
# Makefile fragment to build the common kernel files for EHS.

# Called by ../../Makefile
#

# @author: inx limited
#
# Predefined variables

#  OBJ - File extension for object files

#  NOREV - Defined if no revision number checking is to be performed
# Expected variables

#  OBJECTS - list of object files added by this makefile
#  VPATH - where to look for source code


include $(EHS_COMMON_EHS_PATH)/deps.mk

.PHONY : 

revision.h : .PHONY
	@echo Get build number...
#	@cscript //Nologo $(SCRIPT_PATH)/GetVersionNum.vbs $(REVOPT) -T$(EHS_COMMON_KERNEL_PATH)/revision_template.h -O$(EHS_COMMON_KERNEL_PATH)/revision.h $(EHS_ROOT_PATH)

timer.h : target.h callback_queue.h

# The baisc paths for all configs:
VPATH+=$(EHS_COMMON_EHS_PATH)
VPATH+=$(EHS_COMMON_KAPI_PATH)

# Set up any debugging settings. 
ifdef EHS_DEBUGALL
ifneq ($(EHS_DEBUG_TCPIP_CONSOLE),target_specific)
	EHS_DEBUG_TCPIP_CONSOLE=yes
endif
ifeq ($(EHS_RUNTIME_LOGGER_ENABLED), no)
	undefine EHS_RUNTIME_LOGGER_ENABLED
else
	EHS_RUNTIME_LOGGER_ENABLED=yes
endif
	EHS_DEBUG_AV=yes
# Assume we will always want to define this build as a debug build and upload and manage on Devamn accordingly 
export 	BUILD_MODE=debug
endif


# Enable the TCPIP connection to tools for debugging and app upload 
ifdef  EHS_DEBUG_TCPIP_CONSOLE
	ifeq ($(EHS_DEBUG_TCPIP_CONSOLE), none)
		EHS_DEBUG_TCPIP_CONSOLE=stubbed
	endif
else
	EHS_DEBUG_TCPIP_CONSOLE=stubbed
endif
include $(EHS_TARGET_COMPONENT_HAL_PATH)/comms/tcp_server_common/tcp.mk

#TODO2025 - the following should be moved to a HAL make file.
ifdef EHS_COMMS_API_SUPPORT
	ifneq ($(EHS_COMMS_API_SUPPORT), none)
		EHS_TARGET_COMMS_API_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/comms/$(EHS_COMMS_API_SUPPORT)
		DEFS+=EHS_COMMS_API_SUPPORT
		INC_DIRS+=$(EHS_TARGET_COMMS_API_PATH)
		include $(EHS_TARGET_COMMS_API_PATH)/comms.mk
	endif
endif


# Enable logging to the device's lcal (stdio) logging


#todo this should probably go in the AV module
ifdef EHS_DEBUG_AV
	DEFS += EHS_DEBUG_AV	
endif

# This is the very verbose that you will not want to accidently build into anything you release.
ifdef EHS_DEBUG_TRACE
    DEFS += EHS_BUILDOPT_STDIO_MESSAGE_TRACE #this is for specific messages
    DEFS += EHS_BUILDOPT_STDIO_ENABLE_FUNCTION_TRACING # this is the legacy tracing @todo remove the argument number specificity
	DEFS += EHS_RUNTIME_LOGGER_ENABLED # This is needed for trace debugging.
else 
	ifdef EHS_RUNTIME_LOGGER_ENABLED
		DEFS += EHS_RUNTIME_LOGGER_ENABLED
	endif
endif

# for any traget that do not need to store arguments passed to the main
ifdef EHS_TARGET_NO_MAIN_ARGS
	DEFS+=EHS_TARGET_NO_MAIN_ARGS=1
endif
