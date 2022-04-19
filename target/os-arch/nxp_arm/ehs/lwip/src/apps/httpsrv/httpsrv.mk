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

VPATH += $(HEATROD_CONTROLLER_DIR)/lwip/src/apps/httpsrv

OBJECTS += httpsrv.$(OBJ)
OBJECTS += httpsrv_base64.$(OBJ)
OBJECTS += httpsrv_fs.$(OBJ)
OBJECTS += httpsrv_fs_data.$(OBJ)
OBJECTS += httpsrv_script.$(OBJ)
OBJECTS += httpsrv_sha1.$(OBJ)
OBJECTS += httpsrv_supp.$(OBJ)
OBJECTS += httpsrv_task.$(OBJ)
OBJECTS += httpsrv_tls.$(OBJ)
OBJECTS += httpsrv_utf8.$(OBJ)
OBJECTS += httpsrv_ws.$(OBJ)
OBJECTS += httpsrv_ws_api.$(OBJ)
