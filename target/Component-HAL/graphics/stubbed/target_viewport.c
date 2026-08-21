/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license.
 ***************************************************************/

/** @file target_viewport.c
 * Stubbed viewport backend. Every primitive is a silent no-op so the FB
 * graph can construct gui_viewport / widget components on a headless target
 * (e.g. RAK3112 LoRa-only board with no display) without aborting.
 *
 * Covers the full hal_viewport.h surface so the linker is happy regardless
 * of which Common renderer happens to call through. If you're adding a new
 * EhsTV_* / EhsTVSurface_* entry point upstream, mirror it here as a no-op.
 */

#define EHS_TARGET_CODE

#include "globals.h"
#include "hal_string.h"
#include "hal_viewport.h"
#include "hal_process.h"
#include "widget.h"
#include "graphics.h"
#include "messages.h"

/* Single global viewport — declared extern in hal_viewport.h, defined here. */
EhsTVClass EhsTV;

/* ------------------ Lifecycle --------------------------------------------- */

ehs_bool EhsTV_init(EhsTVClass *pViewport)
{
    (void)pViewport;
    return EHS_TRUE;   /* always "succeeds" — there's nothing to set up */
}

void EhsTV_reset(EhsTVClass *pViewport) { (void)pViewport; }
void EhsTV_show(EhsTVClass *pViewport)  { (void)pViewport; }
void EhsTV_term(EhsTVClass *pViewport)  { (void)pViewport; }
void EhsTV_clear(EhsTVClass *pViewport) { (void)pViewport; }

/* ------------------ Per-frame / damage ----------------------------------- */

void EhsTV_update(EhsTVClass *pViewport) { (void)pViewport; }

void EhsTV_updateRect(EhsTVClass *pViewport,
                      ehs_sint32 nX, ehs_sint32 nY,
                      ehs_sint32 nWidth, ehs_sint32 nHeight)
{
    (void)pViewport; (void)nX; (void)nY; (void)nWidth; (void)nHeight;
}

void EhsTV_showFrame(EhsTVClass *pViewport, ehs_bool bHasFrame)
{
    (void)pViewport; (void)bHasFrame;
}

void EhsTV_hideViewport(void) { }

/* ------------------ Geometry --------------------------------------------- */

void EhsTV_move(EhsTVClass *pViewport,
                EhsDataflowIntType nX, EhsDataflowIntType nY,
                EhsDataflowIntType nDeltaWid, EhsDataflowIntType nDeltaHt)
{
    (void)pViewport; (void)nX; (void)nY; (void)nDeltaWid; (void)nDeltaHt;
}

void EhsTV_setwindow(EhsTVClass *pViewport, ehs_bool isViewportDefined,
                     EhsDataflowIntType nX, EhsDataflowIntType nY,
                     EhsDataflowIntType nWid, EhsDataflowIntType nHt)
{
    (void)pViewport; (void)isViewportDefined;
    (void)nX; (void)nY; (void)nWid; (void)nHt;
}

/* ------------------ Drawing primitives ----------------------------------- */

void EhsTV_blit(EhsTVClass *pViewport,
                const EhsTVSurfaceClass *pSurface,
                const EhsGraphicsRectangleClass *prDst,
                const EhsGraphicsRectangleClass *prSrc,
                ehs_uint8 nImageAlpha)
{
    (void)pViewport; (void)pSurface; (void)prDst; (void)prSrc; (void)nImageAlpha;
}

void EhsTV_fillRect(EhsTVClass *pViewport,
                    const EhsGraphicsRectangleClass *pRect,
                    const EhsGraphicsColourClass *pColour)
{
    (void)pViewport; (void)pRect; (void)pColour;
}

void EhsTV_fade(EhsTVClass *pViewport, EhsGraphicsColourClass nColour)
{
    (void)pViewport; (void)nColour;
}

/* ------------------ Surface management ----------------------------------- */

EhsTVSurfaceClass *EhsTVSurface_create(EhsTVClass *pViewport,
                                       ehs_uint16 nWidth, ehs_uint16 nHeight,
                                       EhsGraphicsColourFormatEnum eFormat,
                                       EhsGraphicsColourClass *pPalette,
                                       ehs_uint16 nPaletteSize)
{
    (void)pViewport; (void)nWidth; (void)nHeight; (void)eFormat;
    (void)pPalette; (void)nPaletteSize;
    /* NULL is a documented "creation failed" return; every caller must
     * already handle it, so this is the safe answer on a headless build. */
    return NULL;
}

void EhsTVSurface_destroy(EhsTVClass *pViewport, EhsTVSurfaceClass *pSurface)
{
    (void)pViewport; (void)pSurface;
}

void *EhsTVSurface_pixels(EhsTVSurfaceClass *pSurface)
{
    (void)pSurface;
    return NULL;
}

void EhsTVSurface_finalisePixels(EhsTVSurfaceClass *pSurface)
{
    (void)pSurface;
}

ehs_uint16 EhsTVSurface_pitch(EhsTVSurfaceClass *pSurface)
{
    (void)pSurface;
    return 0;
}

ehs_uint16 EhsTVSurface_width(EhsTVSurfaceClass *pSurface)
{
    (void)pSurface;
    return 0;
}

ehs_uint16 EhsTVSurface_height(EhsTVSurfaceClass *pSurface)
{
    (void)pSurface;
    return 0;
}

/* ------------------ Z-order / screen geometry --------------------------- */

void EhsTV_showViewport(ehs_uint16 zorder) { (void)zorder; }
void EhsTV_setZOrder(ehs_uint16 zorder)    { (void)zorder; }

ehs_bool EhsTV_getScreenSize(ehs_sint32 *nScreenWidth, ehs_sint32 *nScreenHeight)
{
    /* Headless — report a 0×0 surface and a failure return. Common widget
     * code that auto-sizes to the screen will see the failure and skip
     * the resize, leaving whatever default the FB graph configured. */
    if (nScreenWidth)  *nScreenWidth  = 0;
    if (nScreenHeight) *nScreenHeight = 0;
    return EHS_FALSE;
}
