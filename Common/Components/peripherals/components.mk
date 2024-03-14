#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

#
# Makefile fragment to build the GUI component code for EHS.

# Called by ../components.mk
#

# @author: inx limited
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

#todo2022 we need to split the peripheraks toolbox as per the DCC docs where UI peripherals are seperted from MCU IO.
ifdef EHS_PERIPHERALS_RCU
	DEFS+= EHS_PERIPHERALS_RCU
	OBJECTS += dtv_remote.$(OBJ)
endif

ifdef EHS_PERIPHERALS_GUI_KEYBOARD
	DEFS+= EHS_PERIPHERALS_GUI_KEYBOARD
	OBJECTS += keypress.$(OBJ)
endif

ifdef EHS_PERIPHERALS_USBLCD
	INC_DIRS += $(EHS_TARGET_OS_HW_PATH)/textdisplay #@todo this should be moved when win support is available
	DEFS+= EHS_PERIPHERALS_USBLCD
	OBJECTS += guitextdisplay.$(OBJ)
endif

ifdef EHS_PERIPHERALS_LEDS
	DEFS+= EHS_PERIPHERALS_LEDS
	OBJECTS += guileds.$(OBJ)
endif

ifdef EHS_PERIPHERALS_GPIO_SUPPORT
#$(info  $$EHS_PERIPHERALS_GPIO_SUPPORT=[$(EHS_PERIPHERALS_GPIO_SUPPORT)])
#if the hardware doesn't have real GPIO then stubb it if the peripheral toolbox is still needed
	# include the commone GPIO components in the toolbox. 
	OBJECTS += gpio_out.$(OBJ)
	OBJECTS += gpio_in.$(OBJ)
else 

#todo2023 = do we want this to be the default? BEst for now if it is.
	EHS_PERIPHERALS_GPIO_SUPPORT=stubbed
endif
