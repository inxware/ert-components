/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_viewport.c
 * This file provides the definitions for EhsTargetViewportClass, which
 * drives the target's graphic device 
 * 
 * NOTE: The standard EHS GUI functions are currently stubbed !
 *
 * @author: inx limited
 *
 */

/**
 * @page Verification Verification report
 * @section target_viewport
 * @anchor target_viewport
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on 2007-10-12
 * Last modified on $Date$
 *
 * This file contained no derogations to the MISRA standard.
 *  *
 */


/**
 * Provides access to the target-specific declarations of header files
 */
#define EHS_TARGET_CODE

/*****************************************************************************/
/* Included files */


//#include "target_viewport.h"
#include "globals.h"
#include "hal_string.h"
#include "hal_viewport.h"
#include "hal_process.h"
#include "widget.h"
#include "graphics.h"
#include "hal-api.h"
#include "messages.h"


/* Include OpenGL headers here */
#include "hal_viewport.h"
/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/* Some generic functions copied from other graphics code */

/* Debugging Tools */
//#define TRACE_VIEWPORT
#undef TRACE_VIEWPORT
#ifdef TRACE_VIEWPORT
    int EhsTraceLevel = 0;
    static char* currentFunc;
    #define INDENT(x) ((x==0)?"":((x==1)?">":((x==2)?">>":((x==3)?">>>":(">..>")))))
    #define ENTER(x) {ehs_uint32 ticks = EhsTgtTimer_tickTous(EhsTgtTimer_now());EhsTraceLevel++;currentFunc = #x;}
    #define LEAVE(x) {ehs_uint32 ticks = EhsTgtTimer_tickTous(EhsTgtTimer_now());--EhsTraceLevel;}
#else
    #define ENTER(x)
    #define LEAVE(x)
#endif

/* Global handle to the view port object */
EhsTVClass EhsTV; // Global handle of viewport strcuture

struct EhsTVStruct
{
  int a;
};

/* todo - It seems most of the following should be in the common code area */

/**
 *  Reset the use of the target viewport. This function
 * releases resources occupied by the viewport.
 */
void EhsTV_reset(EhsTVClass* pViewport)
{
    ENTER(EhsTV_reset);
    EhsTPMutex_lock(EhsTPMutex_widgetTable);

    EhsTPMutex_unlock(EhsTPMutex_widgetTable);
    LEAVE(EhsTV_reset);
}

/**
 * Called after widgets are created
 * These are tasks that are delayed until after the widgets are created
 * This is specifically so that the viewport widget attributes can be applied to the window before it is shown
 */
void EhsTV_show(EhsTVClass* pViewport)
{

}

void* EhsTVSurface_pixels(EhsTVSurfaceClass* pSurface) 
{
    return NULL;
}

void EhsTV_fillRect(EhsTVClass* pViewport, const EhsGraphicsRectangleClass* pRect, const EhsGraphicsColourClass* pColour)
{

}

/**
 *  Shutdown the target viewport. This function
 * releases resources occupied by the viewport.
 */
void EhsTV_term(EhsTVClass* pViewport)
{
    ENTER(EhsTV_term);
    /* Kill any rendering threads here if you need to */
    LEAVE(EhsTV_term);
}

/*Change size and position of viewport*/
void EhsTV_move(EhsTVClass* pViewport, EhsDataflowIntType nX, EhsDataflowIntType nY, EhsDataflowIntType nDeltaWid, EhsDataflowIntType nDeltaHt)
{
    ENTER(EhsTV_move);
    LEAVE(EhsTV_move);
}


/*Change size and position of viewport*/
void EhsTV_setwindow(EhsTVClass* pViewport, ehs_bool bIsView, EhsDataflowIntType nX, EhsDataflowIntType nY, EhsDataflowIntType nWid, EhsDataflowIntType nHt)
{
    ENTER(EhsTV_move);
    LEAVE(EhsTV_move);
}

/*Change alpha value of viewport*/
void EhsTV_fade(EhsTVClass* pViewport, EhsGraphicsColourClass nColour)
{
    ENTER(EhsTV_fade);
    EhsTV_update(pViewport);
    LEAVE(EhsTV_fade);
}


/* show a frame on the viewport */
void EhsTV_showFrame(EhsTVClass* pViewport, ehs_bool bAddFrame)
{

}

/**
 * Indicate to the target that the entire viewport needs updating.
 *
 */

void EhsTV_update(EhsTVClass* pViewport)
{
    ENTER(EhsTV_update);
 
    LEAVE(EhsTV_update);
}

/**
 * Redraw a specified part of the OSD layer.global
 *
 */
void EhsTV_updateRect(EhsTVClass* pViewport, ehs_sint32 nX, ehs_sint32 nY, ehs_sint32 nWidth, ehs_sint32 nHeight)
{
   LEAVE(EhsTV_updateRect);
}

/**
 * Clear the target viewport
 */
void EhsTV_clear(EhsTVClass* pViewport)
{
    ENTER(EhsTV_clear);
  
    LEAVE(EhsTV_clear);
}

/**
 * OpenGL This function should get texture object, bind it to OpenGL then render it.
 *
 * General blitting function used by the three specific blitting functions.
 *
 * @param[in] pViewport Viewport to blit into
 * @param[in] pImgData The content of the image represented as a 2d array of colours.
 * @param[in] pDst The bounding box for the image
 * @param[in] nAlpha The global alpha level
 * @param[in] bAlpha Does the image require alpha blending (i.e. some pixels are partially see-through)
 * @param[in] bSprite Are some image pixels completely transparent?
 *
 */
void EhsTV_blit(EhsTVClass* pViewport, const EhsTVSurfaceClass* pImgData, const EhsGraphicsRectangleClass* pDst, const EhsGraphicsRectangleClass* pSrc, ehs_uint8 nAlpha)
{
 
    LEAVE(EhsTV_fillRect);
}

/**
 * Release memory provided for images.  Assumes that this function is never called for memory
 * allocated when bTemporary = FALSE in Surface_create
 */
void EhsTVSurface_destroy(EhsTVClass* pViewport, EhsTVSurfaceClass* pSurface)
{
    ENTER(EhsTVSurface_destroy);
    LEAVE(EhsTVSurface_destroy);
}

/**
 * Allocate OpenGL Textures for images (to subsequently render to screen) - this must be maintained for the off-screen intermediate images
 *
 * @param[in] pViewport Viewport where surface will be used
 * @param[in] nWidth Width of the viewport
 * @param[in] nHeight Height of the viewport
 * @param[in] eFormat format that is to be used for the bitmap
 * @param[in] pPalette array of ARGB8888 colours to use for the palette (NULL if not required)
 * @param[in] nPaletteSize number of entries in the palette
 * @return pointer to the surface, or null if an error occured.
 */
EhsTVSurfaceClass* EhsTVSurface_create(EhsTVClass* pViewport,ehs_uint16 nWidth, ehs_uint16 nHeight, EhsGraphicsColourFormatEnum eFormat,EhsGraphicsColourClass* pPalette, ehs_uint16 nPaletteSize)
{
    return NULL;
}



void EhsTVSurface_finalisePixels (EhsTVSurfaceClass* pSurface)
{
  
}

ehs_uint16 EhsTVSurface_height(EhsTVSurfaceClass* pSurface)
{
    return 0;
}

/**
 * Return the number of pixles (not bytes!!) across (allows direct addressing in the array of pixels -
 * index = x + y * pitch
 *
 */
ehs_uint16 EhsTVSurface_pitch(EhsTVSurfaceClass* pSurface)
{

    return 0;
}

/**
 * Return the width of the bitmap
 */
ehs_uint16 EhsTVSurface_width(EhsTVSurfaceClass* pSurface)
{
 
    return 0;
}


void EhsTV_hideViewport()
{

}

/**
 * show viewport and set zorder
 * viewport bottommost: zorder=0
 * viewport normal zorder: zorder=1
 * viewport topmost: zorder=2
 */
void EhsTV_showViewport(ehs_uint16 zorder)
{
   
    EhsTV_setZOrder(zorder);
}

/**
 * set zorder
 * viewport bottommost: zorder=0
 * viewport normal zorder: zorder=1
 * viewport topmost: zorder=2
 */
void EhsTV_setZOrder(ehs_uint16 zorder)
{
   
}

/**
 * Callbacks function for handling mouse button press events from the mouse.
 */
void EhsT_android_event_button_press(ehs_uint32 x, ehs_uint32 y)
{

    EhsTPMutex_lock(EhsTPMutex_widgetTable);
    EhsWidgetTable_triggerViewportMouseDown(&EhsWidgetTable,x,y);
    EhsWidgetTable_registerMouseDownOnWidgetMatchCoords(&EhsWidgetTable,x,y);
    EhsTPMutex_unlock(EhsTPMutex_widgetTable);

}


/* todo these should really be moved to an input module - not the viewport module - this would require a fair amount of general refactoring */
/**
 * Callback function for handling mouse button release events from the mouse.
 */
void EhsT_android_event_button_release(ehs_uint32 x, ehs_uint32 y)
{
    EhsTPMutex_lock(EhsTPMutex_widgetTable);
    EhsWidgetTable_triggerViewportMouseUp(&EhsWidgetTable,x,y);
    EhsWidgetTable_registerMouseUpOnWidgetMatchCoords(&EhsWidgetTable,x,y);
    EhsTPMutex_unlock(EhsTPMutex_widgetTable);

}

/**
 * Callback function for handling mouse button hold down and move events from the mouse.
 */
void EhsT_android_event_motion_notify( ehs_uint32 x, ehs_uint32 y)
{

    EhsTPMutex_lock(EhsTPMutex_widgetTable);
    EhsWidgetTable_triggerViewportMouseDrag(&EhsWidgetTable,x,y);
    EhsTPMutex_unlock(EhsTPMutex_widgetTable);

}

ehs_bool EhsTV_getScreenSize(ehs_sint32 *nScreenWidth, ehs_sint32 *nScreenHeight)
{

    *nScreenWidth = 0; // in pixels
    *nScreenHeight = 0;
    return EHS_TRUE;
}


