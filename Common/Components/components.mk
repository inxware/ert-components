#
# Makefile to include built in Component support 

# Called by ../../Makefile
#

# @author: Pierre Drezet
# @version: $Revision: 43 $
# @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006) $
# 
# Copyright (c) inx Ltd, 2010. All rights reserved.
#
#
# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  VPATH - where to look for source code
#  INC_DIRS - for includes
#  EFS -for source switches
#  EHS_GRAPHICS_SUPPORT - defined if this target supports graphics

#Compile the modules loader
OBJECTS+= statmodules.$(OBJ)
VPATH+= $(EHS_COMMON_COMPONENTS_PATH)

include $(EHS_COMMON_COMPONENTS_PATH)/core/components.mk


ifdef EHS_GUI_SUPPORT
ifneq ($(EHS_GUI_SUPPORT), none)
EHS_COMMON_TOOLKIT_GUI_PATH=$(EHS_COMMON_COMPONENTS_PATH)/gui
include $(EHS_COMMON_TOOLKIT_GUI_PATH)/components.mk
endif
endif



ifdef EHS_PERIPHERAL_DEVICE_SUPPORT
ifneq ($(EHS_PERIPHERAL_DEVICE_SUPPORT), "none")
EHS_COMMON_TOOLKIT_TEXT_PATH=$(EHS_COMMON_COMPONENTS_PATH)/peripherals 
include $(EHS_COMMON_COMPONENTS_PATH)/peripherals/components.mk
DEFS+=EHS_PERIPHERAL_DEVICE_SUPPORT
endif
endif

ifdef EHS_AV_SUPPORT
ifneq ($(EHS_AV_SUPPORT), none)
EHS_COMMON_TOOLKIT_DTV_PATH=$(EHS_COMMON_COMPONENTS_PATH)/AV 
include $(EHS_COMMON_COMPONENTS_PATH)/AV/components.mk
DEFS+=EHS_AV_SUPPORT
endif
endif

#warning "Disabled user components because it is breaking the arm build when the block_reftable.h is included"
include $(EHS_COMMON_COMPONENTS_PATH)/user/usercomponents.mk


################ Networking Group - Keep together! ################################
########### Defines for sub components of the above modules ##########################
#this is a subset of networking - we'll just add the compile flag
ifdef EHS_DEVMAN_SUPPORT
ifneq ($(EHS_DEVMAN_SUPPORT), none)
DEFS+= EHS_DEVMAN_SUPPORT
#NOCODE
export EHS_COMPONENT_NETWORKING_SUPPORT
#@ does the above mean that all components will be built?? Shouldn't but need to check
endif
endif

#@todo this is repeated in the HAL-components - should all be here
ifdef EHS_COMPONENT_NETWORKING_SUPPORT
ifneq ($(EHS_COMPONENT_NETWORKING_SUPPORT), none)
#EHS_TARGET_COMPONENT_HAL_PATH sdfsd
include $(EHS_COMMON_COMPONENTS_PATH)/networking/components.mk
DEFS+=EHS_COMPONENT_NETWORKING_SUPPORT
endif
endif

ifdef EHS_MEDIA_SUPPORT
ifneq ($(EHS_MEDIA_SUPPORT), none)
EHS_COMMON_TOOLKIT_MEDIA_PATH=$(EHS_COMMON_COMPONENTS_PATH)/media
include $(EHS_COMMON_TOOLKIT_MEDIA_PATH)/components.mk
DEFS+=EHS_MEDIA_SUPPORT
endif
endif 





################################################################################
# Optional toolkits
#

#
# Deprecated toolkit
#ifeq "$(TK_DEPRECATE)" "yes"
ifeq "$(EHS_TOOLKIT_DEPRECATED)" "yes" 
EHS_COMMON_TOOLKIT_DEPRECATE_PATH=$(EHS_COMMON_COMPONENTS_PATH)/deprecated
INC_DIRS+=$(EHS_COMMON_TOOLKIT_DEPRECATE_PATH)
include $(EHS_COMMON_TOOLKIT_DEPRECATE_PATH)/components.mk
DEFS+=EHS_TOOLKIT_DEPRECATED

endif
