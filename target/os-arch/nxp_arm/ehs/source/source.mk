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

VPATH += $(HEATROD_CONTROLLER_DIR)/source

OBJECTS += zones.$(OBJ)
OBJECTS += utz.$(OBJ)
OBJECTS += IEC60730_Safety.$(OBJ)
OBJECTS += RNG1.$(OBJ)
OBJECTS += app_dhcp.$(OBJ)
OBJECTS += certificate.$(OBJ)
OBJECTS += inx_iec.$(OBJ)
OBJECTS += inx_led.$(OBJ)
OBJECTS += inx_buttons.$(OBJ)
OBJECTS += inx_display.$(OBJ)
OBJECTS += ksdk_mbedtls.$(OBJ)
OBJECTS += main.$(OBJ)
OBJECTS += mqtt.$(OBJ)
OBJECTS += net.$(OBJ)
OBJECTS += project_setup_frdmk3s_cm4.$(OBJ)
OBJECTS += semihost_hardfault.$(OBJ)
OBJECTS += inx_uart.$(OBJ)
OBJECTS += inx_default_sodl.$(OBJ)