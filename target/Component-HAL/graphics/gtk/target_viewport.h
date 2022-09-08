/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
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
#include <gtk/gtk.h>
#include <cairo.h>
#include "graphics.h"

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
    union
    {
        cairo_surface_t* Cargb8888;
        struct
        {
            ehs_uint8* pBitmap; /**< bitmap representing individual pixels, one-per-bit, points to pixels of surBitmap*/
            cairo_surface_t* surBitmap; /*Cairo surface for text. Format is A1*/
            EhsGraphicsColourClass pColour[2];	/**< palette containing both colours */
            ehs_uint16 nWidth;				/**< bitmap width */
            ehs_uint16 nHeight;				/**< bitmap height */
        } __attribute__((packed)) A1;								/**< Used for A1 images */
    } __attribute__((packed)) fmt;
    //ehs_uint16 row_stride; /* Convenient to store this rather than calculate each time (currently only used by cairo) */
    struct EhsTVSurfaceStruct* pNext;	/**< Used to support a linked list of allocated surfaces - needed for global destroy */
} __attribute__((packed));








/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */

#endif /* EHS_TARGET_VIEWPORT_H */
