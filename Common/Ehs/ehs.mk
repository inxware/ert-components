#
# Makefile fragment to build the common kernel files for EHS.

# Called by ../../Makefile
#

# @author: inx limited
# @version: $Revision: 43 $
# @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006) $
# 
# Copyright (c) inx limited, 2007. All rights reserved.
#
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
OBJECTS+=   callback_queue.$(OBJ)


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
DEFS += EHS_DEBUG_TCPIP_CONSOLE
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
