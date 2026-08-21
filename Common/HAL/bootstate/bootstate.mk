#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# Persistent boot-state flag HAL.
# Used by the kernel's failed-boot detector — see ehs_main.c
# EhsAppLoadingStateMachine and the porting guide § "Failed-load detection".

# Predefined variables
#  OBJ - File extension for object files

# Expected variables
#  OBJECTS - list of object files added by this makefile
#  VPATH   - where to look for source code

VPATH    += $(EHS_COMMON_HAL_PATH)/bootstate
OBJECTS  += hal_bootstate.$(OBJ)
