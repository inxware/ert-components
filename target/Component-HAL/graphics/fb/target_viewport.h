/** @file target_viewport.h
 * The target-specific declarations required to support the HAL for viewport services
 * are defined here. This file should only be included by hal_viewport.h
 * 
 * @author: inx limited
 * @version: $Revision: 1271 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

#ifndef EHS_TARGET_VIEWPORT_H
#define EHS_TARGET_VIEWPORT_H

#ifndef EHS_HAL_VIEWPORT_H
#error "This file should only be included by hal_viewport.h"
#endif

/*****************************************************************************/
/* Included files */

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
 * We just use macros for Frame buffer gfx
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
	union {
		EhsGraphicsColourClass* pPixels;	/**< Contains the array of colours used to represent the bitmap */
		ehs_uint8* pBitmap;					/**< contains the array of pixels used for LUT-based surfaces */
	} uSurface;
	EhsGraphicsColourClass* pPalette;		/**< Contains the array of colours required for a lookup table */
	ehs_uint16 nPaletteSize;				/**< Contains the number of entries in the lookup table */
	EhsGraphicsColourFormatEnum eFormat;	/**< The format in which the pixels are presented */
	ehs_uint16 nWid;						/**< Width of the surface - should be duplicated in any parent widget structs for generic access*/
	ehs_uint16 nHt;							/**< Height of the surface  - should be duplicated in any parent widget structs for generic access*/
};

/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */

#endif /* EHS_TARGET_VIEWPORT_H */
