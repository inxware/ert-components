#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

#
# Makefile fragment to build the HAL file support for EHS.

# Called by ../kernel.mk
#

# @author: inx limited
# Predefined variables


include $(EHS_COMMON_HAL_PATH)/configs/deps.mk

OBJECTS+= hal_configs.$(OBJ)
VPATH+=: $(EHS_COMMON_HAL_PATH)/configs

