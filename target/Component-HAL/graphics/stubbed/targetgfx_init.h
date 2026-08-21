/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license.
 ***************************************************************/

/** @file targetgfx_init.h
 * Declarations for the stubbed graphics-init backend.
 * No-op counterpart to gtk/lvgl/qt targetgfx_init.h — same signatures so
 * Common/HAL code links without any conditional compilation.
 */

#ifndef EHS_TARGET_GFX_INIT_H
#define EHS_TARGET_GFX_INIT_H

#include "globals.h"

EHS_GLOBAL void EhsTGfxSys_init(void);
EHS_GLOBAL void EhsTGfxSys_initLate(void *userdata);
EHS_GLOBAL void EhsTGfxSys_term(void);
EHS_GLOBAL void EhsTGfxApp_init(void);
EHS_GLOBAL void EhsTGfxApp_term(void);
EHS_GLOBAL void EhsTGfxApp_reset(void);

#endif /* EHS_TARGET_GFX_INIT_H */
