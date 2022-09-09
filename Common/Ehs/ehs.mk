#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
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

#OBJECTS+= CBUtils.$(OBJ)
OBJECTS+= callback_queue.$(OBJ)

VPATH+=$(EHS_COMMON_EHS_PATH)
VPATH+=$(EHS_COMMON_KAPI_PATH)

ifdef EHS_DEBUGALL
#todo2022 - This is a bit of amess - seems we want some strucuture to these being set?
DEFS += EHS_RUNTIME_LOGGER_ENABLED
DEFS += EHS_DEBUG_AV
EHS_DEBUG=yes
EHS_DEBUG_TCPIP_CONSOLE=yes
endif

ifdef  EHS_DEBUG_TCPIP_CONSOLE 
#This is the only place this DEF should be set so that the conditional build includes all work too.
DEFS += EHS_DEBUG_TCPIP_CONSOLE
OBJECTS+= console_queue.$(OBJ)
endif

#todo2022 - this should go in the comms.mk file
ifdef EHS_COMMS_API_SUPPORT
ifneq ($(EHS_COMMS_API_SUPPORT), none)
EHS_TARGET_COMMS_API_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/comms/$(EHS_COMMS_API_SUPPORT)
DEFS+=EHS_COMMS_API_SUPPORT
INC_DIRS+=EHS_TARGET_COMMS_API_PATH
include $(EHS_TARGET_COMMS_API_PATH)/comms.mk
#ifeq ($(EHS_COMMS_API_SUPPORT), bsdsockets)
#@todo move the DEF to the comms.mk file
#endif
endif
endif
