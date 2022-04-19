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

VPATH += $(HEATROD_CONTROLLER_DIR)/drivers

OBJECTS += fsl_adc16.$(OBJ)
OBJECTS += fsl_clock.$(OBJ)
OBJECTS += fsl_common.$(OBJ)
OBJECTS += fsl_enet.$(OBJ)
OBJECTS += fsl_ftfx_cache.$(OBJ)
OBJECTS += fsl_ftfx_controller.$(OBJ)
OBJECTS += fsl_ftfx_flash.$(OBJ)
OBJECTS += fsl_ftfx_flexnvm.$(OBJ)
OBJECTS += fsl_gpio.$(OBJ)
OBJECTS += fsl_lptmr.$(OBJ)
OBJECTS += fsl_pmc.$(OBJ)
OBJECTS += fsl_rcm.$(OBJ)
OBJECTS += fsl_rnga.$(OBJ)
OBJECTS += fsl_rtc.$(OBJ)
OBJECTS += fsl_sim.$(OBJ)
OBJECTS += fsl_smc.$(OBJ)
OBJECTS += fsl_uart.$(OBJ)
OBJECTS += fsl_wdog.$(OBJ)
OBJECTS += fsl_ftm.$(OBJ)
