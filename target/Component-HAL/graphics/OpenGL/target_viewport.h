/** @file target_viewport.h
 * The target-specific declarations required to support the HAL for viewport services
 * are defined here. This file should only be included by hal_viewport.h
 *
 * @author: inx limited
 * @version: $Revision: 3622 $
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
/* Define macros  */

/**
 * Macro used to rescale alpha value if target cannot support full 8 bit range of alphas.
 * STAPI only supports an alpha range of 0 to 127
 */

/*****************************************************************************/
/*Definition for pixels should be RGBA*/


/*****************************************************************************/
/* Define types */

/**
 * Define a surface - using for blitting from
 */

struct EhsTVSurfaceStruct
{
	EhsGraphicsColourFormatEnum eFormat;	/**< Type of graphics held in this structure */
	union {
		//cairo_surface_t* Cargb8888;
		struct {
			ehs_uint8* pBitmap; /**< bitmap representing individual pixels, one-per-bit, points to pixels of surBitmap*/
			//cairo_surface_t* surBitmap; /*Cairo surface for text. Format is A1*/
			EhsGraphicsColourClass pColour[2];	/**< palette containing both colours */
			ehs_uint16 nWidth;				/**< bitmap width */
			ehs_uint16 nHeight;				/**< bitmap height */
		} A1;								/**< Used for A1 images */
	} fmt;
	struct EhsTVSurfaceStruct* pNext;	/**< Used to support a linked list of allocated surfaces - needed for global destroy */
};



/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */

#endif /* EHS_TARGET_VIEWPORT_H */
