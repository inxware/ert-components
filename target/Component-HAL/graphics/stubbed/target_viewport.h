/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license.
 ***************************************************************/

/** @file target_viewport.h
 * Stubbed viewport / surface types for the headless graphics HAL.
 * Same shape as gtk_stub — Common HAL code (hal_viewport.h) only needs the
 * struct names defined; nothing reads the contents.
 *
 * Must only be included by hal_viewport.h.
 */

#ifndef EHS_TARGET_VIEWPORT_H
#define EHS_TARGET_VIEWPORT_H

#ifndef EHS_HAL_VIEWPORT_H
#error "This file should only be included by hal_viewport.h"
#endif

/* Empty placeholder — the stubbed HAL holds no per-viewport state. */
struct EhsTVStruct
{
    int _unused;
};

/* Surface placeholder — kept so Common code that references EhsTVSurfaceClass*
 * (e.g. for cache linked-lists) still compiles. Linked-list pointer included
 * because some Common helpers walk it. */
struct EhsTVSurfaceStruct
{
    EhsGraphicsColourFormatEnum eFormat;
    struct EhsTVSurfaceStruct *pNext;
};

#endif /* EHS_TARGET_VIEWPORT_H */
