#
# Makefile fragment to build the comms-specific code for EHS.

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

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code
#  EHS_TARGET_COMMS_API_PATH - path to the current directory (set by platform makefile)


include $(EHS_TARGET_COMMS_API_PATH)/deps.mk
include $(EHS_TARGET_COMMS_API_PATH)/Components/components.mk
#include the common funtions
include $(EHS_TARGET_COMMS_API_PATH)/../tcp_server_common/tcp.mk

VPATH+=: $(EHS_TARGET_COMMS_API_PATH)
INC_DIRS += $(EHS_TARGET_COMMS_API_PATH)


OBJECTS += target_tcp.$(OBJ)
OBJECTS += targetcomms_init.$(OBJ)
