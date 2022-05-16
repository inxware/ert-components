#
# Mafile for theb console server code
#todo2022 this probably shouldn't be included if EHS_DEBUG_TCPIP_CONSOLE isn't defined - hence conditional build below

# Called by ../../platform/<platform-type>
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

include $(EHS_TARGET_COMMS_API_PATH)/../tcp_server_common/deps.mk

#todo2022 - shouldn't these be done in the console.mk file
ifdef EHS_DEBUG_TCPIP_CONSOLE
OBJECTS+= console_server.$(OBJ)
OBJECTS += target_console.$(OBJ)
endif

VPATH+=: $(EHS_TARGET_SERVER_PATH)

INC_DIRS += $(EHS_TARGET_SERVER_PATH)

