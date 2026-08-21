/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license.
 ***************************************************************/

/** @file targetgfx_init.c
 * Stubbed graphics init/term — every entry point is a no-op so headless
 * platforms can satisfy the linker without pulling in a real backend.
 */

#define EHS_TARGET_CODE
#include "targetgfx_init.h"
#include "hal_viewport.h"

void EhsTGfxSys_init(void)         { /* no-op (no display) */ }
void EhsTGfxSys_initLate(void *u)  { (void)u; }
void EhsTGfxSys_term(void)         { }
void EhsTGfxApp_init(void)         { }
void EhsTGfxApp_term(void)         { }
void EhsTGfxApp_reset(void)        { }
