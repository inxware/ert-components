#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
# with this file. If not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# Common per-app lifecycle hook (default weak no-op).
# Targets that have UI/HAL state to tear down override this in their
# os-arch directory via a stronger (non-weak) symbol.

include $(EHS_COMMON_HAL_PATH)/applifecycle/deps.mk
VPATH+=$(EHS_COMMON_HAL_PATH)/applifecycle

OBJECTS += app_lifecycle_default.$(OBJ)
