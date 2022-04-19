#
# Makefile fragment to build the GUI component code for EHS.

# Called by ../components.mk
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
#  VPATH - where to look for source code



INC_DIRS+=: $(EHS_COMMON_COMPONENTS_PATH)/peripherals
#INC_DIRS+=$(EHS_COMMON_TOOLKIT_PNG_PATH)
VPATH+=: $(EHS_COMMON_COMPONENTS_PATH)/peripherals

include $(EHS_COMMON_COMPONENTS_PATH)/peripherals/deps.mk
#@todo need proper profile labels for this part 
OBJECTS += peripherals_components.$(OBJ)

ifdef EHS_PERIPHERALS_RCU
DEFS+= EHS_PERIPHERALS_RCU
OBJECTS += dtv_remote.$(OBJ)
endif
ifdef EHS_PERIPHERALS_GUI_KEYBOARD
DEFS+= EHS_PERIPHERALS_GUI_KEYBOARD
OBJECTS += keypress.$(OBJ)
endif
ifdef EHS_PERIPHERALS_USBLCD
DEFS+= EHS_PERIPHERALS_USBLCD
OBJECTS += guitextdisplay.$(OBJ)
endif
ifdef EHS_PERIPHERALS_LEDS
DEFS+= EHS_PERIPHERALS_LEDS
OBJECTS += guileds.$(OBJ)
endif
ifdef EHS_PERIPHERALS_GPIO
DEFS+= EHS_PERIPHERALS_GPIO
OBJECTS += gpio_out.$(OBJ)
OBJECTS += gpio_in.$(OBJ)
endif
