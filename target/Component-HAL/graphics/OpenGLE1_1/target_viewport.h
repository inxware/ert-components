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



#include "target_main.h" // need to get the engine struct definition


/**
 * Define the target viewport type. Contains target specific data required
 * by the viewport functions.
 */
struct EhsTVStruct
{

    /* Probable need some OpenGLhandle to put in this struct */

    EhsGraphicsRectangleClass xClipRect;	/**< Clipping rectangle - used when drawing all images */
    EhsTVSurfaceClass* pAllocSurface; /**< List of allocated surfaces - used for deallocation purposes */
    //ehs_bool bViewportChanged;			/**< The pixbuf has changed, we need to copy it into pPixmap */
    ehs_uint8 transparency; // transparency of window

    struct engine* engine;
    //int32_t width;
    //int32_t height;

};




int engine_init_display(struct engine* engine/*, EhsTVClass* pViewport*/);
void engine_draw_frame(struct engine* engine);

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
        EhsGraphicsColourClass* pRGBAPixels; /* this will be set to null when unbound */
        struct
        {
            ehs_uint8* pBitmap; /**< bitmap representing individual pixels, one-per-bit, points to pixels of surBitmap*/
            EhsGraphicsColourClass pColour[2];	/**< palette containing both colours */
        } pA1Surface;
    } fmt;
    ehs_uint16 nWidth;				/**< bitmap width */
    ehs_uint16 nHeight;				/**< bitmap height *//**< Used for A1 images */
    ehs_uint16 nBufferWidth;				/**< This will be next pow2 values of image width */
    ehs_uint16 nBufferHeight;				/**< This will be next pow2 values of image height */
    GLuint textureName;			/* named texture to bind for eaxh render */
    struct EhsTVSurfaceStruct* pNext;	/**< Used to support a linked list of allocated surfaces - needed for global destroy */
};



/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */

void EhsT_android_event_button_press(ehs_uint32 x, ehs_uint32 y);
/**
 * Callback function for handling mouse button release events from the mouse.
 */
void EhsT_android_event_button_release(ehs_uint32 x, ehs_uint32 y);
/**
 * Callback function for handling mouse button hold down and move events from the mouse.
 */
void EhsT_android_event_motion_notify(ehs_uint32 x, ehs_uint32 y);

#endif /* EHS_TARGET_VIEWPORT_H */
