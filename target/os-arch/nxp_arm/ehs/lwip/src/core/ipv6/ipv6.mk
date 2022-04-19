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

VPATH += $(HEATROD_CONTROLLER_DIR)/lwip/src/core/ipv6

OBJECTS += dhcp6.$(OBJ)
OBJECTS += ethip6.$(OBJ)
OBJECTS += icmp6.$(OBJ)
OBJECTS += inet6.$(OBJ)
OBJECTS += ip6.$(OBJ)
OBJECTS += ip6_addr.$(OBJ)
OBJECTS += ip6_frag.$(OBJ)
OBJECTS += mld6.$(OBJ)
OBJECTS += nd6.$(OBJ)
