/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_viewport.h
 * The target-specific declarations required to support the HAL for viewport services
 * are defined here. This file should only be included by hal_viewport.h
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_VIEWPORT_H
#define EHS_TARGET_VIEWPORT_H

#ifndef EHS_HAL_VIEWPORT_H
#error "This file should only be included by hal_viewport.h"
#endif

/*****************************************************************************/
/* Included files */

#include <windows.h>

/*****************************************************************************/
/* Define macros  */

/**
 * Provide access to pixels representing the surface
 */
#define EhsTVSurface_pixels(pSurface) ((pSurface)->uSurface.pPixels)

/**
 * Provide access to the bitmap representing the surface
 */
#define EhsTVSurface_bitmap(pSurface) ((pSurface)->uSurface.pBitmap)

/**
 * Return the number of bytes across (allows direct addressing in the array of pixels -
 * index = x + y * pitch
 * @todo - this assumes a 32 bit colour depth is this valid?
 */
#define EhsTVSurface_pitch(pSurface) ((pSurface)->nWid*4)


/**
 * Return the width and height of the current surface
 *
 * Use macros on pixels sizes rather that target library calls
 */
#define EhsTVSurface_width(pSurface) ((pSurface)->nWid)

#define EhsTVSurface_height(pSurface) ((pSurface)->nHt)

/*****************************************************************************/
/* Define types */

/**
 * Define a surface - using for blitting from
 */
struct EhsTVSurfaceStruct
{
    union
    {
        EhsGraphicsColourClass* pPixels;	/**< Contains the array of colours used to represent the bitmap */
        ehs_uint8* pBitmap;					/**< contains the array of pixels used for LUT-based surfaces */
    } uSurface;
    EhsGraphicsColourClass* pPalette;		/**< Contains the array of colours required for a lookup table */
    ehs_uint16 nPaletteSize;				/**< Contains the number of entries in the lookup table */
    EhsGraphicsColourFormatEnum eFormat;	/**< The format in which the pixels are presented */
    ehs_uint16 nWid;						/**< Width of the surface */
    ehs_uint16 nHt;							/**< Height of the surface */
};


/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */

#ifdef EHS_TARGET_CODE
/**
 * Repaint all viewports
 */
EHS_GLOBAL void EhsTV_paint(HWND hWnd, LPPAINTSTRUCT lpPS);
#endif

#endif /* EHS_TARGET_VIEWPORT_H */
