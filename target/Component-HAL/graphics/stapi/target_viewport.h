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
#include <stgxobj.h>

/*****************************************************************************/
/* Define macros  */

/**
 * Macro used to rescale alpha value if target cannot support full 8 bit range of alphas.
 * STAPI only supports an alpha range of 0 to 127
 */
#define EHS_TV_ALPHA8_SCALE(x) ((x) >> 1)

/**
 * Provide access to pixels representing the surface
 */
#define EhsTVSurface_pixels(pSurface) ((EhsGraphicsColourClass*)((pSurface)->xBitmap.Data1_p))

/**
 * Provide access to the bitmap representing the surface
 */
#define EhsTVSurface_bitmap(pSurface) ((ehs_uint8*)((pSurface)->xBitmap.Data1_p))


/**
 * Return the number of bytes across (allows direct addressing in the array of pixels - 
 * index = x + y * pitch
 */
#define EhsTVSurface_pitch(pSurface) ((pSurface)->xBitmap.Width*4)

/**
 * Return the width of the bitmap
 */
#define EhsTVSurface_width(pSurface) ((pSurface)->xBitmap.Width)

/**
 * Return the height of the bitmap
 */
#define EhsTVSurface_height(pSurface) ((pSurface)->xBitmap.Height)

/*****************************************************************************/
/* Define types */

/**
 * Define a surface - using for blitting from
 */

struct EhsTVSurfaceStruct
{
	STGXOBJ_Bitmap_t	xBitmap;			/**< STAPI bitmap that corresponds to a surface */
	STGXOBJ_Palette_t xPalette;			/**< STAPI palette for CLUT based image formats */
	struct EhsTVSurfaceStruct* pNext;	/**< Used to support a linked list of allocated surfaces - needed for global destroy */ 
};

/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */

#endif /* EHS_TARGET_VIEWPORT_H */
