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

VPATH += $(HEATROD_CONTROLLER_DIR)/lwip/src/core

OBJECTS += def.$(OBJ)
OBJECTS += dns.$(OBJ)
OBJECTS += inet_chksum.$(OBJ)
OBJECTS += init.$(OBJ)
OBJECTS += ip.$(OBJ)
OBJECTS += mem.$(OBJ)
OBJECTS += memp.$(OBJ)
OBJECTS += netif.$(OBJ)
OBJECTS += pbuf.$(OBJ)
OBJECTS += raw.$(OBJ)
OBJECTS += stats.$(OBJ)
OBJECTS += sys.$(OBJ)
OBJECTS += tcp.$(OBJ)
OBJECTS += tcp_in.$(OBJ)
OBJECTS += tcp_out.$(OBJ)
OBJECTS += timeouts.$(OBJ)
OBJECTS += udp.$(OBJ)
