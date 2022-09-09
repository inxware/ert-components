#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
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

include $(EHS_TARGET_COMMS_API_PATH)/../tcp_server_common/deps.mk

ifdef EHS_DEBUG_TCPIP_CONSOLE
OBJECTS+= console_server.$(OBJ)
OBJECTS += target_console.$(OBJ)
endif

VPATH+=: $(EHS_TARGET_SERVER_PATH)

INC_DIRS += $(EHS_TARGET_SERVER_PATH)

