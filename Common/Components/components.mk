#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

#
# Makefile to include built in Component support 

# Called by ../../Makefile
#

# @author: inx limited
# 
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
    export EHS_GUI_SUPPORT
    DEFS += EHS_GUI_SUPPORT
    EHS_COMMON_TOOLKIT_GUI_PATH=$(EHS_COMMON_COMPONENTS_PATH)/gui
    include $(EHS_COMMON_TOOLKIT_GUI_PATH)/components.mk
endif
endif

ifdef EHS_PERIPHERAL_DEVICE_SUPPORT
ifneq ($(EHS_PERIPHERAL_DEVICE_SUPPORT), "none")
    export EHS_PERIPHERAL_DEVICE_SUPPORT
    DEFS+=EHS_PERIPHERAL_DEVICE_SUPPORT
    EHS_COMMON_TOOLKIT_TEXT_PATH=$(EHS_COMMON_COMPONENTS_PATH)/peripherals 
    include $(EHS_COMMON_COMPONENTS_PATH)/peripherals/components.mk
endif
endif

ifdef EHS_AV_SUPPORT
ifneq ($(EHS_AV_SUPPORT), none)
    DEFS+=EHS_AV_SUPPORT
    export EHS_AV_SUPPORT
    EHS_COMMON_TOOLKIT_DTV_PATH=$(EHS_COMMON_COMPONENTS_PATH)/AV
    include $(EHS_COMMON_COMPONENTS_PATH)/AV/components.mk
endif
endif

#warning "Disabled user components because it is breaking the arm build when the block_reftable.h is included"
include $(EHS_COMMON_COMPONENTS_PATH)/user/usercomponents.mk


################ Networking Group - Keep together! ################################
########### Defines for sub components of the above modules ##########################
#this is a subset of networking - we'll just add the compile flag
#TODO this should all go into the network HAL make file.
ifdef EHS_DEVMAN_SUPPORT
ifneq ($(EHS_DEVMAN_SUPPORT), none)
    DEFS+= EHS_DEVMAN_SUPPORT
    #NOCODE
    export EHS_DEVMAN_SUPPORT
    #@ does the above mean that all components will be built?? Shouldn't but need to check
endif
endif

#@todo this is repeated in the HAL-components - should all be here
ifdef EHS_COMPONENT_NETWORKING_SUPPORT
ifneq ($(EHS_COMPONENT_NETWORKING_SUPPORT), none)
    include $(EHS_COMMON_COMPONENTS_PATH)/networking/components.mk
    DEFS+=EHS_COMPONENT_NETWORKING_SUPPORT
    export EHS_COMPONENT_NETWORKING_SUPPORT
endif
endif

ifdef EHS_MEDIA_SUPPORT
ifneq ($(EHS_MEDIA_SUPPORT), none)
    DEFS+=EHS_MEDIA_SUPPORT
    export EHS_MEDIA_SUPPORT
    EHS_COMMON_TOOLKIT_MEDIA_PATH=$(EHS_COMMON_COMPONENTS_PATH)/media
    include $(EHS_COMMON_TOOLKIT_MEDIA_PATH)/components.mk
endif
endif 

# Machine Vision Support
ifdef EHS_MV_SUPPORT
export EHS_MV_SUPPORT
include $(EHS_COMMON_COMPONENTS_PATH)/mv/components.mk
endif

# Machine Learing Support
ifdef EHS_ML_SUPPORT
export EHS_ML_SUPPORT
include $(EHS_COMMON_COMPONENTS_PATH)/ml/components.mk
endif

################################################################################
# Optional toolkits
#

#
# Deprecated toolkit
#ifeq "$(TK_DEPRECATE)" "yes"
ifeq "$(EHS_TOOLKIT_DEPRECATED)" "yes" 
    EHS_COMMON_TOOLKIT_DEPRECATE_PATH=$(EHS_COMMON_COMPONENTS_PATH)/deprecated
    #VPATH+=$(EHS_COMMON_TOOLKIT_DEPRECATE_PATH)
    INC_DIRS+=$(EHS_COMMON_TOOLKIT_DEPRECATE_PATH)
    include $(EHS_COMMON_TOOLKIT_DEPRECATE_PATH)/components.mk
    DEFS+=EHS_TOOLKIT_DEPRECATED
endif

DEFS += EHS_TOOLBOX_HASHES=\"$(EHS_TOOLBOX_HASHES)\"
