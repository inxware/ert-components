#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

#
# Mafile for theb console server code
#todo2022 this probably shouldn't be included if EHS_DEBUG_TCPIP_CONSOLE isn't defined - hence conditional build below

# Called by ../../platform/<platform-type>
#

# @author: inx limited
# Predefined variables

#  OBJ - File extension for object files

#  NOREV - Defined if no revision number checking is to be performed
# Expected variables

#  OBJECTS - list of object files added by this makefile
#  VPATH - where to look for source code

ifdef EHS_TARGET_COMMS_API_PATH
	include $(EHS_TARGET_COMMS_API_PATH)/../tcp_server_common/deps.mk
else 
#	$(info == EHS_TARGET_COMMS_API_PATH is not set - Not using tcpip comms )
endif

#Assume we don't need to check if console is selected by the time the make file is called.
ifdef EHS_DEBUG_TCPIP_CONSOLE
ifneq ($(EHS_DEBUG_TCPIP_CONSOLE),stubbed)
	ifneq ($(EHS_DEBUG_TCPIP_CONSOLE),target_specific)
	OBJECTS += console_server.$(OBJ)
	endif
	OBJECTS += target_console.$(OBJ)
	OBJECTS += console_queue.$(OBJ)
	DEFS += EHS_DEBUG_TCPIP_CONSOLE
else
	OBJECTS += stubbed_console.$(OBJ)
endif
endif

VPATH+=: $(EHS_TARGET_SERVER_PATH)
INC_DIRS += $(EHS_TARGET_SERVER_PATH)

