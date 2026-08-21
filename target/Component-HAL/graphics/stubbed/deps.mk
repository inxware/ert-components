#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license.
#---------------------------------------------------------------#
#
# deps.mk — header dependencies for the stubbed graphics HAL.
# Hand-maintained; mirrors the structure used by the other graphics backends.

targetgfx_init.h  : target.h

target_viewport.h : graphics.h

targetgfx_init.$(OBJ) : targetgfx_init.h hal_viewport.h

target_viewport.$(OBJ) : hal_string.h hal_viewport.h hal_process.h widget.h \
                         graphics.h hal.h hal_mem.h hal_time.h
