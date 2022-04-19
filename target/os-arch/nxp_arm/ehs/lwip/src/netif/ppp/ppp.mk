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

VPATH += $(HEATROD_CONTROLLER_DIR)/lwip/src/netif/ppp

OBJECTS += auth.$(OBJ)
OBJECTS += ccp.$(OBJ)
OBJECTS += chap-md5.$(OBJ)
OBJECTS += chap-new.$(OBJ)
OBJECTS += chap_ms.$(OBJ)
OBJECTS += demand.$(OBJ)
OBJECTS += eap.$(OBJ)
OBJECTS += eui64.$(OBJ)
OBJECTS += fsm.$(OBJ)
OBJECTS += ipcp.$(OBJ)
OBJECTS += ipv6cp.$(OBJ)
OBJECTS += lcp.$(OBJ)
OBJECTS += lwip_ecp.$(OBJ)
OBJECTS += magic.$(OBJ)
OBJECTS += mppe.$(OBJ)
OBJECTS += multilink.$(OBJ)
OBJECTS += ppp.$(OBJ)
OBJECTS += pppapi.$(OBJ)
OBJECTS += pppcrypt.$(OBJ)
OBJECTS += pppoe.$(OBJ)
OBJECTS += pppol2tp.$(OBJ)
OBJECTS += pppos.$(OBJ)
OBJECTS += upap.$(OBJ)
OBJECTS += lwiputils.$(OBJ)
OBJECTS += vj.$(OBJ)
