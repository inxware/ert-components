/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_viewport.h
 * In this file, all of the hardware abstraction layer functions relating to graphics are given.
 *
 *
 * @author: inx limited
 */

#ifndef EHS_HAL_VIEWPORT_H
#define EHS_HAL_VIEWPORT_H

/* Extract these into the GUI.h header - possibly just move (and rename) into the graphics HAL directory and include from the gui.h*/

/* only process this file if we provide GUI support */
#ifndef xEHS_GUI_SUPPORT
/*****************************************************************************/
/* Included files */

#include "globals.h"
#include "graphics_types.h"
#include "target_viewport.h"
#include "ehs_fb_types.h"

/*****************************************************************************/
/* Define macros  */

#ifndef EHS_TV_ALPHA8_SCALE
/**
 * Macro used to rescale alpha value if target cannot support full 8 bit range of alphas.
 * Default behaviour is no rescaling.
 */
#define EHS_TV_ALPHA8_SCALE(x) (x)
#endif

/*****************************************************************************/
/* Define types */

/**
 * Indicates the properties contained within the target viewport type
 */

//typedef struct  EhsTVStruct;
typedef struct EhsTVStruct EhsTVClass;

/**
 * Container for the piece of data used to represent images/bitmaps etc.
 */


/*****************************************************************************/
/* Declare global variables */

/**
 * Define the target viewport. Only one viewport is defined at this
 * stage.
 */

//EHS_COMPONENT_API_EXPORT EHS_FB_
extern EhsTVClass EhsTV;

/*****************************************************************************/
/* Declare function prototypes  */



/**
 * Indicate to the target that the viewport needs updating.
 * This will eventually result in EhsTV_paint being called
 */
void EhsTV_update(EhsTVClass* pViewport);

/**
 * Indicate to the target that a rectangle on the viewport needs updating.
 * This will eventually result in EhsTV_paint being called
 */
void EhsTV_updateRect(EhsTVClass* pViewport, ehs_sint32 nX, ehs_sint32 nY, ehs_sint32 nWidth, ehs_sint32 nHeight);

#ifndef EhsTV_blit
/**
 * Blit an arbitrary surface to the screen. Uses the surface's format type to determine how to do the blitting
 *
 * @param[in] pViewport Current viewport
 * @param[in] pSurface The image to blit
 * @param[in] prDst Rectangle on the viewport to blit to
 * @param[in] prSrc Rectangle on the surface to blit from
 * @param[in] nAlpha The global alpha level
 */
#define EhsTV_blit_withlock EhsTV_blit //@todo make a with lock version
void EhsTV_blit(EhsTVClass* pViewport,
                           const EhsTVSurfaceClass* pSurface,
                           const EhsGraphicsRectangleClass* prDst,
                           const EhsGraphicsRectangleClass* prSrc,
                           ehs_uint8 nImageAlpha);
#endif

#ifndef EhsTV_fillRect
/**
 * Fill a rectangle with a specified colour in the specified viewport.
 *
 * @param[in] pViewport Viewport to fill rectangle into
 * @param[in] pRect position and size of rectangle to fill
 * @param[in] pColour Colour to use for filling rectangle (includes global alpha value)
 */
void EhsTV_fillRect(EhsTVClass* pViewport,
                               const EhsGraphicsRectangleClass* pRect, const EhsGraphicsColourClass* pColour);
#endif

/*Change alpha value of viewport*/
void EhsTV_fade(EhsTVClass* pViewport, EhsGraphicsColourClass nColour);

/*
 * show a frame on the viewport
 * @todo implement EhsTV_showFrame for other targets, not just gtk
 */
void EhsTV_showFrame(EhsTVClass* pViewport, ehs_bool bHasFrame);

void EhsTV_hideViewport();

/*Change size and position of viewport*/
void EhsTV_move(EhsTVClass* pViewport, EhsDataflowIntType nX, EhsDataflowIntType nY, EhsDataflowIntType nDeltaWid, EhsDataflowIntType nDeltaHt);

/*Change size and position of viewport*/
void EhsTV_setwindow(EhsTVClass* pViewport, ehs_bool isViewportDefined, EhsDataflowIntType nX, EhsDataflowIntType nY, EhsDataflowIntType nWid, EhsDataflowIntType nHt);

/* reset the widget table - clears all stuff! */

/**
 * Initialise the target viewport.
 *
 * @return true if initialisation was successful.
 */
ehs_bool EhsTV_init(EhsTVClass* pViewport);

/**
 *  reset the use of the target viewport. This function is called between
 * applications. It releases application-specific resources occupied by the viewport.
 */
void EhsTV_reset(EhsTVClass* pViewport);

/**
 * called after widgets are created
 * These are tasks that are delayed until after the widgets are created
 * This is specifically so that the viewport widget attributes can be applied to the window before it is shown
 */
void EhsTV_show(EhsTVClass* pViewport);

/**
 *  Terminate the use of the target viewport. This function is called as part of shutting
 * down EHS. It releases all resources occupied by the viewport.
 */
void EhsTV_term(EhsTVClass* pViewport);

/**
 * Clear the target viewport
 */
void EhsTV_clear(EhsTVClass* pViewport);


/* The following function sets the viewport to the default value in case a viewport widget is not in the application
 * This may not be needed as the hardware init should use defaults and create a window in any case.
 */

ehs_bool EhsWidgetViewport_default_config(void);

/**
 * gets the x coord of the primary viewport - currently there is only 1 viewport
 */
ehs_uint16 EhsPrimaryViewportInfo_getX();

/**
 * sets the x coord of the primary viewport - currently there is only 1 viewport
 */
void EhsPrimaryViewportInfo_setX(ehs_uint16 nX);

/**
 * gets the y coord of the primary viewport - currently there is only 1 viewport
 */
ehs_uint16 EhsPrimaryViewportInfo_getY();

/**
 * sets the y coord of the primary viewport - currently there is only 1 viewport
 */
void EhsPrimaryViewportInfo_setY(ehs_uint16 nY);

/**
 * gets the width of the primary viewport - currently there is only 1 viewport
 */
ehs_uint16 EhsPrimaryViewportInfo_getWidth();

/**
 * sets the width of the primary viewport - currently there is only 1 viewport
 */
void EhsPrimaryViewportInfo_setWidth(ehs_uint16 nWidth);

/**
 * gets the height of the primary viewport - currently there is only 1 viewport
 */
ehs_uint16 EhsPrimaryViewportInfo_getHeight();

/**
 * sets the height of the primary viewport - currently there is only 1 viewport
 */
void EhsPrimaryViewportInfo_setHeight(ehs_uint16 nHeight);

/**
 * gets the colour of the primary viewport - currently there is only 1 viewport
 */
EhsGraphicsColourClass EhsPrimaryViewportInfo_getColour();

/**
 * sets the colour of the primary viewport - currently there is only 1 viewport
 */
void EhsPrimaryViewportInfo_setColour(EhsGraphicsColourClass nColour);

/**
 * sets the colour of the primary viewport - currently there is only 1 viewport
 */
void EhsPrimaryViewportInfo_setColourRGB(ehs_uint16 nRed, ehs_uint16 nGreen, ehs_uint16 nBlue);

/**
 * gets the alpha transparency of the primary viewport - currently there is only 1 viewport
 */
ehs_uint16 EhsPrimaryViewportInfo_getAlpha();

/**
 * sets the alpha transparency of the primary viewport - currently there is only 1 viewport
 */
void EhsPrimaryViewportInfo_setAlpha(ehs_uint16 nAlpha);

/**
 * gets the hasFrame of the primary viewport - currently there is only 1 viewport
 */
ehs_bool EhsPrimaryViewportInfo_getHasFrame();

/**
 * sets the hasFrame of the primary viewport - currently there is only 1 viewport
 */
void EhsPrimaryViewportInfo_setHasFrame(ehs_bool bHasFrame);

/**
 * gets the ZOrder of the primary viewport - currently there is only 1 viewport
 */
ehs_uint16 EhsPrimaryViewportInfo_getZOrder();

/**
 * sets the ZOrder of the primary viewport - currently there is only 1 viewport
 */
void EhsPrimaryViewportInfo_setZOrder(ehs_uint16 nZOrder);



/**
 * Allocate memory for images (to subsequently blit to screen)
 *
 * @param[in] pViewport Viewport where surface will be used
 * @param[in] nWidth Width of the viewport
 * @param[in] nHeight Height of the viewport
 * @param[in] eFormat format that is to be used for the bitmap
 * @param[in] pPalette array of ARGB8888 colours to use for the palette (NULL if not required)
 * @param[in] nPaletteSize number of entries in the palette
 * @param[in] bTemporary Create using the temporary memory (allows it to be destroyed
 * @return pointer to the surface, or null if an error occured.
 */
EhsTVSurfaceClass* EhsTVSurface_create(EhsTVClass* pViewport,
        ehs_uint16 nWidth, ehs_uint16 nHeight, EhsGraphicsColourFormatEnum eFormat,
        EhsGraphicsColourClass* pPalette, ehs_uint16 nPaletteSize);//, ehs_bool bTemporary);

#ifndef EhsTVSurface_destroy
/**
 * Free memory used by a surface
 */
void EhsTVSurface_destroy(EhsTVClass* pViewport, EhsTVSurfaceClass *pSurface);
#endif

#ifndef EhsTVSurface_pixels
/**
 * Provide access to the array of pixels that makes up the surface, these might be any pixel format so return void *
 */
void* EhsTVSurface_pixels(EhsTVSurfaceClass* pSurface);
#endif

#ifndef EhsTVSurface_finalisePixels
/* This will allow the graphics card to copy pixels into video memory or other GFX subsystem ready for blitting */
void EhsTVSurface_finalisePixels(EhsTVSurfaceClass* pSurface);
#endif

#ifndef EhsTVSurface_pitch
/**
 * Return the number of pixels across (allows direct addressing in the array of pixels -
 * index = x + y * pitch
 */
ehs_uint16 EhsTVSurface_pitch(EhsTVSurfaceClass* pSurface);
#endif

//#ifndef EhsTVSurface_bitmap
/**
 * Return a pointer to the bitmap in a surface
 */

// removed as pixels does this
//void* EhsTVSurface_bitmap(EhsTVSurfaceClass* pSurface);
//#endif
#ifndef EhsTVSurface_width
/**
 * Return the width of a surface
 */
ehs_uint16 EhsTVSurface_width(EhsTVSurfaceClass* pSurface);
#endif


//#ifndef EhsTVSurface_width
/**
 * Return the height of a surface
 */
ehs_uint16 EhsTVSurface_height(EhsTVSurfaceClass* pSurface);


/**
 * show viewport and set zorder
 * viewport bottommost: zorder=0
 * viewport normal zorder: zorder=1
 * viewport topmost: zorder=2
 * @todo EhsTV_showViewport allows zorder to be set to bottommost, normal and topmost in gtk, needs to be replicated for other targets
 */
void EhsTV_showViewport(ehs_uint16 zorder);

/**
 * set zorder
 * viewport bottommost: zorder=0
 * viewport normal zorder: zorder=1
 * viewport topmost: zorder=2
 * @todo EhsTV_showViewport allows zorder to be set to bottommost, normal and topmost in gtk, needs to be replicated for other targets
 */
void EhsTV_setZOrder(ehs_uint16 zorder);

/** @brief
 * returns the actual screen size using platform specific APIs.
 */
ehs_bool EhsTV_getScreenSize(ehs_sint32 *nScreenWidth, ehs_sint32 *nScreenHeight);

#endif /* defined(EHS_GUI_SUPPORT) */

#endif /* EHS_HAL_VIEWPORT_H */
