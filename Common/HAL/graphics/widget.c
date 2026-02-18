/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file widget.c
 * Definitions for the generic widget handling functions
 *
 * @author: inx limited
 *
 * @todo Make this module threadsafe - EhsWidget_draw may run in a separate thread to other functions
 */

/**
 * @page Verification Verification report
 * @section widget
 * @anchor widget
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on 2007-10-16
 * Last modified on $Date$
 *
 * This file contained no derogations to the MISRA standard.
 */

/*****************************************************************************/

#define EHSL_MODULE_ID (EHSH_LOG_MODULE_GRAPHICS)

/* Included files */
#include "widget.h"
#include "messages.h"
#include "utils.h"

#include "hal_string.h"
#include "hal_process.h"
#if defined(EHS_GUI_SUPPORT_MODE_B) 
#include "target_viewport_modeB.h"
#endif

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */
/*****************************************************************************/
/* Declare prototypes of local functions */

/**
 * Change the widget's state, and test whether it needs to be redrawn:
 * either because it is now no longer visible, or because it
 * has become visible. Redraw the widget if appropriate.
 *
 * @param pWidget Widget to test
 * @param nNewState Value of the state before now
 */
EHS_LOCAL void EhsWidget_setState(EhsWidgetClass* pWidget, ehs_uint8 nNewState);

/*****************************************************************************/
/* Variables defined with file-scope */
/*****************************************************************************/
/* Variables defined with global-scope */

/**
 * Define the global widget table
 */
EhsWidgetTableClass EhsWidgetTable= {0};

/*****************************************************************************/
/* Function definitions */



/**
 * Create the widget. This is a necessary step prior to showing the widget
 *
 * Works in both render MODE_A anad MODE_B
 */
void EhsWidget_create(EhsWidgetClass* pWidget)
{
    EHSH_LOG_INFO("EhsWidget_create starting initialisation...");

    ehs_bool bIsInit = EHS_WIDGET_STATE_INITIALIZED(pWidget->nState);
#if defined(EHS_GUI_SUPPORT_MODE_A) 
    /** @todo - refactor - it is only for convenience that viewport is using the widget struct  */
    pWidget->mouseClickPortNumber = -1; // default value
    pWidget->mouseDownPortNumber = -1; // default value
    pWidget->mouseUpPortNumber = -1; // default valuepWidget->bContentChanged = EHS_TRUE;
    pWidget->mouseDragPortNumber = -1; // default value as only currently implemented on viewport
    pWidget->mouseUpDownAbsXPortNumber = -1; // default value as only currently implemented on viewport
    pWidget->mouseUpDownAbsYPortNumber = -1; // default value as only currently implemented on viewport
    pWidget->mouseDragOffsetXPortNumber = -1; // default value as only currently implemented on viewport
    pWidget->mouseDragOffsetYPortNumber = -1; // default value as only currently implemented on viewport
    /* @todo this structure seems wasteful. Wouldn't it be better to not lock the viewport unless we were going to act on it, i.e. move mutex functions inside the if statement */
#endif
    /* ensure only this thread can handle the viewport until we unlock it */
    /* EhsTPMutex_lock(EhsTPMutex_viewport); */
    pWidget->bContentChanged = EHS_TRUE;
    /* check that:
     * 1. the widget has been initialised by the subclass
     * 2. the widget has not yet been created */

    if (bIsInit)
    {
        /* ensure only this thread can handle the viewport until we unlock it */
        EhsTPMutex_lock(EhsTPMutex_viewport);
        /* if the widget has been created already, perform widget-specific destroy prior to creating it */
        /* @todo is this efficient? should recreate do nothing? */
        if ((pWidget->nState & EHS_WIDGET_STATE_CREATED) == EHS_WIDGET_STATE_CREATED)
        {
            EHSH_LOG_INFO("Reinitialise widget - don't call pfCreateFunc()");

            //@todo - should this be in or out?
            pWidget->pfDestroyFunc(pWidget);
            /* clear create flag to ensure that setState displays the new image */
            pWidget->nState = pWidget->nState & (~EHS_WIDGET_STATE_CREATED);
        }
        /* widget specific creation */
        else
        {
            EHSH_LOG_INFO("Call widget pfCreateFunc()");

            if (pWidget->pfCreateFunc(pWidget))
            {
                EhsWidget_setState(pWidget, pWidget->nState | EHS_WIDGET_STATE_CREATED);
            }
        }
        EhsTPMutex_unlock(EhsTPMutex_viewport);

    }
    /*EhsTPMutex_unlock(EhsTPMutex_viewport);*/

    if (!bIsInit)
    {
        EHSH_LOG_WARNING(EHS_MSG_ERROR_WIDGET_NOT_INIT);
    }
    EhsWidgetUI_update(pWidget);
    Ehs_widget_commit(pWidget);
}

/* Call this before setting any widt type specific initialisation to set common parameters
    Works in both render MODE_A anad MODE_B
*/

void EhsWidget_init(EhsWidgetClass* pWidget, const EhsGraphicsRectangleClass *pRect, ehs_uint16 nZ, ehs_uint8 nAlpha 
    #ifdef EHS_STORE_WIDGET_NAMES
    , ehs_char * szWidgetName
    #endif
    )
{
    #ifdef EHS_STORE_WIDGET_NAMES
    EhsStrncpy(pWidget->szWidgetName, szWidgetName, EHS_OBJECT_NAME_MAX_SIZE);
    #endif
    
    EhsMemcpy(&(pWidget->xOrigRect),pRect,sizeof(EhsGraphicsRectangleClass));
    EhsMemcpy(&(pWidget->xCurRect),pRect,sizeof(EhsGraphicsRectangleClass));
    EhsMemcpy(&(pWidget->xDesignRect),pRect,sizeof(EhsGraphicsRectangleClass));
    pWidget->UpdatedOffsettRect.nHeight=0;
    pWidget->UpdatedOffsettRect.nWidth=0;
    pWidget->UpdatedOffsettRect.nTop=0;
    pWidget->UpdatedOffsettRect.nLeft=0;
    pWidget->nZ = nZ;
    pWidget->nAlpha = nAlpha;
    pWidget->bContentChanged = EHS_FALSE;
    pWidget->pFIData = NULL;
    pWidget->pfCreateFunc = NULL;
    pWidget->pfDestroyFunc = NULL;
    pWidget->pfDrawFunc = NULL;
    pWidget->pfFadeFunc = NULL;

#if defined(EHS_GUI_SUPPORT_MODE_A) 
    EHS_WIDGET_IMAGE(pWidget).szFilename = NULL;
    //asign call-back functions for render mode B librariies
    pWidget->pfMouseDownEventFunc = NULL;
    pWidget->pMouseDownEventData = NULL;
    pWidget->pfMouseDownEventFunc = NULL;
    pWidget->pMouseDownEventData = NULL;
    pWidget->mouseClickPortNumber = -1; // default value
    pWidget->mouseDownPortNumber = -1; // default value
    pWidget->mouseUpPortNumber = -1; // default value
    pWidget->mouseDragPortNumber = -1; // default value as only currently implemented on viewport
    pWidget->mouseUpDownAbsXPortNumber = -1; // default value as only currently implemented on viewport
    pWidget->mouseUpDownAbsYPortNumber = -1; // default value as only currently implemented on viewport
    pWidget->mouseDragOffsetXPortNumber = -1; // default value as only currently implemented on viewport
    pWidget->mouseDragOffsetYPortNumber = -1; // default value as only currently implemented on viewport
    pWidget->bOptimiseForSpeed = EHS_FALSE; // we only support this in non render mode B cases to sav memory in Redner mode B, which is usually more constraineed.
#endif
}


/**
 * Destroy the widget. Required after the widget has been finished with.
 * Destroy does not hide the widget.
 *
 * Works in both render MODE_A anad MODE_B
 * In Render Mode B we need to call the target specfici widget deleete function - this is done in target code.
 */
void EhsWidget_destroy(EhsWidgetClass* pWidget)
{
    ehs_bool bIsInit = EHS_FALSE;

    EhsTPMutex_lock(EhsTPMutex_viewport);
    pWidget->bContentChanged = EHS_FALSE;
    /* check that:
     * 1. the widget has been initialised by the subclass
     * 2. the widget has previously been created */
    if (EHS_WIDGET_STATE_INITIALIZED(pWidget->nState) &&
            ((pWidget->nState & EHS_WIDGET_STATE_CREATED) == EHS_WIDGET_STATE_CREATED))
    {
        bIsInit = EHS_TRUE;
        /* widget specific destruction */
        pWidget->pfDestroyFunc(pWidget);
        EhsWidget_setState(pWidget, pWidget->nState & ~EHS_WIDGET_STATE_CREATED);
    }
    EhsTPMutex_unlock(EhsTPMutex_viewport);

    if (!bIsInit)
    {
        EHSH_LOG_WARNING(EHS_MSG_ERROR_WIDGET_NOT_INIT);
    }
}


/** @brief This function changes the original coordinates size of a widget depending on the relative value and if the new values are valid.
 *  Used by Target Viewport, but could be used in any widget though (untested!)
 *
 *  Should work in Works in both render MODE_A anad MODE_B - TBC!!
 * */
void EhsWidget_AdjustCoordinates(EhsWidgetClass* pWidget, ehs_bool bRelative, ehs_sint32 nLeft, ehs_sint32 nWidth, ehs_sint32 nTop, ehs_sint32 nHeight )
{
    ehs_sint32 nScreenWidth = EhsPrimaryViewportInfo_getWidth();
    ehs_sint32 nScreenHeight= EhsPrimaryViewportInfo_getHeight();

    if (nLeft >= 0 )
    {
        if (bRelative)
            pWidget->xOrigRect.nLeft = (ehs_coord)((nLeft*nScreenWidth)/100);
        else
            pWidget->xOrigRect.nLeft = (ehs_coord)nLeft;
    }
    else   // else adjust the LGB values if relative
    {
        //if (bRelative)
        //	pWidget->xOrigRect.nLeft = (nLeft*nScreenWidth)/100;
    }

    if (nWidth >= 0 )
    {
        if (bRelative)
            pWidget->xOrigRect.nWidth 	= (ehs_coord)((nWidth*nScreenWidth)/100);
        else
            pWidget->xOrigRect.nWidth = (ehs_coord)nWidth;
    }
    else   // else adjust the LGB values if relative
    {
        //if (bRelative)
        //	pWidget->xOrigRect.nWidth = (nWidth*nScreenWidth)/100;
    }

    if (nTop >= 0 )
    {
        if (bRelative)
            pWidget->xOrigRect.nTop = (ehs_coord)((nTop*nScreenHeight)/100);
        else
            pWidget->xOrigRect.nTop = (ehs_coord)nTop;
    }
    else   // else adjust the LGB values if relative
    {
        //if (bRelative)
        //	pWidget->xOrigRect.nTop = (nTop*nScreenHeight)/100;
    }

    if (nHeight >= 0 )
    {
        if (bRelative)
            pWidget->xOrigRect.nHeight 	= (ehs_coord)((nHeight*nScreenHeight)/100);
        else
            pWidget->xOrigRect.nHeight = (ehs_coord)nHeight;
    }
    else   // else adjust the LGB values if relative
    {
        //if (bRelative)
        //	pWidget->xOrigRect.nHeight = (nHeight*nScreenHeight)/100;
    }

    /* Need to copy this to the other working rectangles also. Leave the xOriginalRect as is!!!! */
    //EhsMemcpy(,pBounds,sizeof(EhsGraphicsRectangleClass));
    EhsMemcpy(&(pWidget->xCurRect),&(pWidget->xOrigRect),sizeof(EhsGraphicsRectangleClass));
    EhsMemcpy(&(pWidget->xDesignRect),&(pWidget->xOrigRect),sizeof(EhsGraphicsRectangleClass));

}


/**
 * General purpose update function, contains the common code for all update functions.
 * Saves offset position and size values if they are needed for new media updated
 * Changes the current window position to the new position
 *
 * @param[in] pWidget Widget to update
 * @param[in] szString text to be displayed
 * @param[in] bAlphaConnected true if the alpha value should be used
 * @param[in] nAlpha current alpha level to use for image
 * @param[in] bXConnected true if the x coordinate should be used
 * @param[in] nXoffset current x coordinate to use (value is offset to default image position)
 * @param[in] bYConnected true if the y coordinate should be used
 * @param[in] nYoffset current y coordinate to use (value is offset to default image position)
 * @param[in] bWConnected true if the width value should be used
 * @param[in] nWoffset current width offset to use (value is offset to default image position)
 * @param[in] bHConnected true if the height offset should be used
 * @param[in] nHoffset current height offset to use (value is offset to default image position)
 * Works in both render MODE_A anad MODE_B
 */
void Ehs_widget_position_update(EhsWidgetClass* pWidget, ehs_bool bAlphaConnected, EhsDataflowIntType nAlpha,
                                ehs_bool bXConnected, EhsDataflowIntType nXoffset,
                                ehs_bool bYConnected, EhsDataflowIntType nYoffset,
                                ehs_bool bWConnected, EhsDataflowIntType nWoffset,
                                ehs_bool bHConnected, EhsDataflowIntType nHoffset)
{

    pWidget->bContentChanged = EHS_TRUE; /* Used only by text bpxes */
    pWidget->bPositionUpdated = EHS_TRUE;

    if (bAlphaConnected)
    {
        if (nAlpha < 0) nAlpha = 0;
        if (nAlpha > 255) nAlpha = 255;
        EhsWidget_fade(pWidget, (ehs_uint8)nAlpha);
    }
    else nAlpha = 255;

    if (!bXConnected)	nXoffset = 0;
    if (!bYConnected)	nYoffset = 0;
    if (!bWConnected) 	nWoffset = 0;
    if (!bHConnected)	nHoffset = 0;

    /* store in case we need to re-apply on new content */
    pWidget->UpdatedOffsettRect.nLeft = nXoffset;
    pWidget->UpdatedOffsettRect.nTop = nYoffset;
    pWidget->UpdatedOffsettRect.nWidth = nWoffset;
    pWidget->UpdatedOffsettRect.nHeight = nHoffset;
    pWidget->nAlpha = (ehs_uint8)nAlpha;

    if (bXConnected || bYConnected || bWConnected || bHConnected)
    {
        EhsWidget_move(pWidget, nXoffset, nYoffset, nWoffset, nHoffset);
    }

}

/**
 * Applies changes to the widget
 *
 * @param[in] pWidget Widget to update
 * */
void Ehs_widget_commit(EhsWidgetClass* pWidget)
{
// this only applies EHS_GUI_SUPPORT_MODE_B
#if defined(EHS_GUI_SUPPORT_MODE_B) 
    EHSH_LOG_INFO("Ehs_widget_commit: pWidget=%p, calling pfDrawFunc=%p",
                  (void*)pWidget, (void*)pWidget->pfDrawFunc);
    pWidget->pfDrawFunc(pWidget, NULL, NULL);
#endif
}


/**
 * Cause the widget to be displayed (assuming it's already been created). If
 * it hasn't been created, there are no ill effects.
 */
void EhsWidget_show(EhsWidgetClass* pWidget)
{
    if (pWidget) {
        EhsTPMutex_lock(EhsTPMutex_viewport);
        EhsWidget_setState(pWidget, pWidget->nState | EHS_WIDGET_STATE_SHOW);
        EhsTPMutex_unlock(EhsTPMutex_viewport);
    }
}

/**
 * Cause the widget to be hidden (assuming it's already been created and shown).
 * No ill effects are caused if this is called before the object is created or shown.
 */
void EhsWidget_hide(EhsWidgetClass* pWidget)
{
    if (pWidget) {
        EhsTPMutex_lock(EhsTPMutex_viewport);
        EhsWidget_setState(pWidget, pWidget->nState & (~EHS_WIDGET_STATE_SHOW));
        EhsTPMutex_unlock(EhsTPMutex_viewport);
    }
}

/**
 * Change the widget's state, and test whether it needs to be redrawn:
 * either because it is now no longer visible, or because it
 * has become visible. Redraw the widget if appropriate.
 *
 * @param pWidget Widget to test
 * @param nNewState Value of the state before now
 */
EHS_LOCAL void EhsWidget_setState(EhsWidgetClass* pWidget, ehs_uint8 nNewState)
{
    if (pWidget) {
        /* Check for state change, and that previous state was showing widget
        * Update viewport if both have occurred */
        if (EHS_WIDGET_STATE_SHOWN(nNewState) != EHS_WIDGET_STATE_SHOWN(pWidget->nState))
        {
            pWidget->nState = nNewState; /* update the state before calling _updateRect */
#if defined(EHS_GUI_SUPPORT_MODE_B) 
            EhsTargetWidget_show(pWidget, pWidget->nState);
#endif
            EhsTV_updateRect(&EhsTV, pWidget->xCurRect.nLeft, pWidget->xCurRect.nTop, pWidget->xCurRect.nWidth, pWidget->xCurRect.nHeight);
        }
        else
        {
            pWidget->nState = nNewState; /* update state but redraw is not necessary as visibility has not changed */
        }
    }
}

/**
 * Cause the widget to be moved and/or resized. We assume that the position that the widget is moved to
 * (almost) always overlaps its current position, thus we do a redraw that includes the original
 * widget position and the new widget position. If we frequently moved the widget outside its
 * original position, it would be more economical to do two draws (old widget position and
 * new widget position).
 *
 * @param[in,out] pWidget Widget data
 * @param[in] nX new relative X position to move widget to
 * @param[in] nY new relative Y position to move widget to
 * @param[in] nWid Change in width for the widget
 * @param[in] nHt change in height for the widget
 *
 * todo2023 - the OpenGL/Android version of this function seems to call rendering functions directly, byt better models like GTK don't - ut sjust updates coords etc.?
 */

void EhsWidget_move(EhsWidgetClass* pWidget, EhsDataflowIntType nX, EhsDataflowIntType nY, EhsDataflowIntType nDeltaWid, EhsDataflowIntType nDeltaHt)
{
    if (pWidget) {
        EhsGraphicsRectangleClass xOldPos; /* contains the previous position of the widget */
        //ehs_float dWHsrc; /* ratio of wid:ht for the two input rectangles */

        EhsTPMutex_lock(EhsTPMutex_viewport);

        /* get the current rectangle position */
        EhsMemcpy(&xOldPos,&(pWidget->xCurRect),sizeof(EhsGraphicsRectangleClass));
        /* calculate the new position of the widget */
        //pWidget->xCurRect.nLeft = EHS_CONVERT_UINT16_SATURATE(pWidget->xOrigRect.nLeft + nX);
        pWidget->xCurRect.nLeft = pWidget->xOrigRect.nLeft + nX;
        //pWidget->xCurRect.nTop = EHS_CONVERT_UINT16_SATURATE(pWidget->xOrigRect.nTop + nY);
        pWidget->xCurRect.nTop = pWidget->xOrigRect.nTop + nY;
        pWidget->xCurRect.nWidth = EHS_CONVERT_UINT16_SATURATE(pWidget->xOrigRect.nWidth + nDeltaWid);
        pWidget->xCurRect.nHeight = EHS_CONVERT_UINT16_SATURATE(pWidget->xOrigRect.nHeight + nDeltaHt);
        /* dk: why did we add this - don't want to prevent widgets from being off the viewport
            if (pWidget->xCurRect.nLeft < 0) pWidget->xCurRect.nLeft=0;
            if (pWidget->xCurRect.nTop < 0) pWidget->xCurRect.nTop =0;
        */
        if (pWidget->xCurRect.nWidth < 0) pWidget->xCurRect.nWidth=0;
        if (pWidget->xCurRect.nHeight  < 0) pWidget->xCurRect.nHeight =0;
        // if flag set to maintain aspect ratio, only delta w parameter is considered and new height is calc'd from this - delta h parameter is ignored
    #ifdef XXXXDONETHISELSWHERE
        if (pWidget->bMaintainAspectRatio)
        {
            dWHsrc = (ehs_float)pWidget->xOrigRect.nWidth/(ehs_float)pWidget->xOrigRect.nHeight;
            pWidget->xCurRect.nHeight = (ehs_uint32) pWidget->xCurRect.nWidth / dWHsrc;
        }
    #endif
        /* check if we have moved or resized - if we haven't we don't need to repaint
        * this avoids unnecessary updates to the viewport.
        *
        * NOTE: We must NOT test "is the widget within the current viewport?" here.
        * Initially this seemed to be an optimization to avoid unnecessary repainting
        * when the widget is no longer visible. However a move
        * function might change a widget from being positioned within the viewport to
        * outside the viewport - this must result in a call to EhsTV_update */
        if ((pWidget->xCurRect.nLeft != xOldPos.nLeft) ||
                (pWidget->xCurRect.nTop != xOldPos.nTop) ||
                (pWidget->xCurRect.nHeight != xOldPos.nHeight) ||
                (pWidget->xCurRect.nWidth != xOldPos.nWidth))
        {
            /* if pWidget is visible, update viewport */
            if (EHS_WIDGET_STATE_SHOWN(pWidget->nState))
            {
            /*  todo XA - Mode B rndering doesn't need any of the EhsTV_updateRect() function #if def this out.
                e.g. #ifndef MODE_B
            */


                /* do we do two updates (clear previous position of widget, set new position of widget)
                * or one update (single rectangle containing new and old position)?
                * That depends if we have an overlap
                */
                /*This should use the proper if statement but it leaves artifacts for some reason so just force two updates*/
                if (EhsGraphicsRectangle_overlap(&(pWidget->xCurRect),&xOldPos)) /* this function needs to be made more choosy*/
                {
                    EhsGraphicsRectangle_union(&xOldPos,&(pWidget->xCurRect),&xOldPos);
                    EhsTV_updateRect(&EhsTV, xOldPos.nLeft, xOldPos.nTop, xOldPos.nWidth, xOldPos.nHeight); /* this might be faster if we passed in a rect reference */
                }
                else
                {
                    EhsTV_updateRect(&EhsTV, xOldPos.nLeft, xOldPos.nTop, xOldPos.nWidth, xOldPos.nHeight);
                    EhsTV_updateRect(&EhsTV, pWidget->xCurRect.nLeft, pWidget->xCurRect.nTop, pWidget->xCurRect.nWidth, pWidget->xCurRect.nHeight);
                }

                // todo XA
                // #else //MODE_B
                //     pWidget->pfDrawFunc(pWidget);

                // # endif
            }
        }
        EhsTPMutex_unlock(EhsTPMutex_viewport);
    }
}

/**
 * Fade the image in or out by scaling its global alpha level by the specified amount
 *
 * @param nOpacity amount of opacity for image. 0 = minimum opacity, 255 maximum opacity.
 *
 * todo XA - Mode B rndering doesn't need any of the EhsTV_updateRect() function #if def this out.
 */
void EhsWidget_fade(EhsWidgetClass* pWidget, ehs_uint8 nOpacity)
{
    if (pWidget) {
        ehs_bool bUpdate;
        EhsTPMutex_lock(EhsTPMutex_viewport);

        bUpdate = pWidget->pfFadeFunc(pWidget, nOpacity);

        if (EHS_WIDGET_STATE_SHOWN(pWidget->nState) && bUpdate)
        {
            EhsTV_updateRect(&EhsTV, pWidget->xCurRect.nLeft, pWidget->xCurRect.nTop, pWidget->xCurRect.nWidth, pWidget->xCurRect.nHeight);
            pWidget->bColourUpdated = EHS_TRUE;
        }
        EhsTPMutex_unlock(EhsTPMutex_viewport);
    }
}

/**
 * Display the widget onto the viewport (if it's ready to be shown).
 *
 * @param pWidget Widget to display
 * @param pViewport Viewport to display it on
 * @param pClipRect Specifies the bounds for drawing the widget
 */
void EhsWidget_draw(EhsWidgetClass* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass* pClipRect)
{
    if (pWidget) {
        EhsTPMutex_lock(EhsTPMutex_viewport);

        if (EHS_WIDGET_STATE_SHOWN(pWidget->nState))
        {
            if (EhsGraphicsRectangle_overlap(&(pWidget->xCurRect), pClipRect))
            {
                /* only display widgets if there is an overlap between the clip rectangle and the bounding rectangle for the widget */
                pWidget->pfDrawFunc(pWidget, pViewport, pClipRect);
            }
        }
        EhsTPMutex_unlock(EhsTPMutex_viewport);
    }
}

/* Reset the widget table and clear the viewport */

void EhsWidgetTable_reset(EhsWidgetTableClass* pWidgetTable)
{
    EhsWidgetTable_init(&EhsWidgetTable); // This should be sourced from the widget table structure
}

/**
 * Initialise the widget table.
 */

void EhsWidgetTable_init(EhsWidgetTableClass* pWidgetTable)
{
    ehs_uint16 nIndex;
    pWidgetTable->nSize = 0;

    pWidgetTable->initialised = EHS_MAGIC_NUMBER;

    if (pWidgetTable->initialised != EHS_MAGIC_NUMBER) return; //What?
    for (nIndex = 0; nIndex < EHS_MAX_WIDGET_INSTANCES; nIndex++)
    {
        pWidgetTable->pZOrderedWidget[nIndex] = NULL;
        EhsMemset(&(pWidgetTable->xWidget[nIndex]),0,sizeof(struct EhsWidgetStruct));
    }
}

/**
 * Insert a widget itno the Z-ordered list of widgets. This is called
 * after a widget has been initialized, but before the index to the widget
 * table has been updated
 */
void EhsWidgetTable_updateZOrder(EhsWidgetTableClass* pWidgetTable, EhsWidgetClass* pWidget)
{
    EhsWidgetClass* pZTable = pWidgetTable->pZOrderedWidget[0];
    ehs_uint16 nIndex = 0;
    ehs_bool bAlreadyPresent = EHS_FALSE; /* Is the widget already in the z-order table? */

    if (pWidgetTable->initialised != EHS_MAGIC_NUMBER) return;
    /*Commented out threadlocking otherwise it locks up the program on the PS3*/
    //EhsTPMutex_lock(EhsTPMutex_viewport);

    /* We don't need to worry about overruns because the number of allocated slots in pZOrderedWidget
     * are equal to nSize-1 at this point */
    while ((pZTable != NULL) &&	(pZTable->nZ <= pWidget->nZ))
    {
        if (pWidgetTable->pZOrderedWidget[nIndex] == pWidget)
        {
            bAlreadyPresent = EHS_TRUE;
            break;
        }
        pZTable = pWidgetTable->pZOrderedWidget[++nIndex];
    }
    if (!bAlreadyPresent)
    {
        /* create a gap in the z-order table to hold the current widget */
        memmove(&(pWidgetTable->pZOrderedWidget[nIndex+1]),
                &(pWidgetTable->pZOrderedWidget[nIndex]),
                ((pWidgetTable->nSize-1) - nIndex)*sizeof(EhsWidgetClass*));
        pWidgetTable->pZOrderedWidget[nIndex] = pWidget;
    }
    //EhsTPMutex_unlock(EhsTPMutex_viewport);
}

/**
 * Draw a widget list in response to a draw command from the target viewport
 * Widgets are drawn in z-order (lowest first).
 *
 * @param pWidgetTable Set of widgets to redraw
 * @param pViewport Physical viewport to write to
 * @param pClipRect Specifies the bounds of what is drawn
 */
void EhsWidgetTable_draw(const EhsWidgetTableClass* pWidgetTable, EhsTVClass *pViewport, EhsGraphicsRectangleClass* pClipRect)
{
    EhsWidgetClass* pWidget;
    ehs_uint16 nIndex;
    if (pWidgetTable->initialised != EHS_MAGIC_NUMBER)
    {
        EHS_TRACE_MESSAGE("\nEhsWidgetTable:Darwing before init \n");
        return;
    }
    /* note this outermost loop is not resource-locked. This isn't a problem so long
     * as widgets aren't being added to the widget table while the application is running.
     * Resource locking takes place at the individual widget-drawing level.
     */
    for (nIndex = 0; nIndex < pWidgetTable->nSize; nIndex++)
    {
        /* composit the widgets from the bottom of the z-order stack up */
        pWidget = (pWidgetTable->pZOrderedWidget[nIndex]);
        if (pWidget)
        {
            EhsWidget_draw(pWidget, pViewport, pClipRect);
        }
        else
        {
            break;
        }
    }
}

/**
 * All widgets from the widget table are marked for update.
 *
 * @param pWidgetTable Set of widgets to redraw
 */
void EhsWidgetTable_dirty(const EhsWidgetTableClass* pWidgetTable)
{
    EhsWidgetClass* pWidget;
    ehs_uint16 nIndex;
    if (pWidgetTable->initialised != EHS_MAGIC_NUMBER)
    {
        EHS_TRACE_MESSAGE("\nEhsWidgetTable:Setting dirty before init \n");
        return;
    }
    for (nIndex = 0; nIndex < pWidgetTable->nSize; nIndex++)
    {
        /* composit the widgets from the bottom of the z-order stack up */
        pWidget = (pWidgetTable->pZOrderedWidget[nIndex]);
        if (pWidget)
        {
            pWidget->bContentChanged = EHS_TRUE;
        }
        else
        {
            break;
        }
    }
}

/**
 * Iterate through widget table and when find the viewport widget, fire its mouse event
 * Assumes there is only one viewport
 */
void EhsWidgetTable_triggerViewportMouseDown(const EhsWidgetTableClass* pWidgetTable, int x, int y)
{
#if defined(EHS_GUI_SUPPORT_MODE_A) 
    EhsFunctionInstanceDataType *pFIdata;
    EhsWidgetClass* pWidget;
    ehs_sint16 nIndex;
    if (pWidgetTable->initialised != EHS_MAGIC_NUMBER) return;

    for(nIndex = 0; nIndex < pWidgetTable->nSize; nIndex++)
    {
        pWidget = (pWidgetTable->pZOrderedWidget[nIndex]);
        if(pWidget)
        {
            /*If widget has its PFIData then that probably means it has a mouse click port*/
            if(pWidget->pFIData)
            {
                /* Check if it is a viewport */
                if(pWidget->eWidgetKind == EHS_WIDGET_KIND_VIEWPORT)
                {
                    pWidget->nMouseDownX = x;
                    pWidget->nMouseDownY = y;
                    pFIdata = pWidget->pFIData;
                    if (pWidget->mouseUpDownAbsXPortNumber > -1 && pWidget->mouseUpDownAbsYPortNumber > -1 && pWidget->mouseDownPortNumber > 1)
                    {
                        EHS_FB_OUT_I(pWidget->mouseUpDownAbsXPortNumber) = x;
                        EHS_FB_OUT_I(pWidget->mouseUpDownAbsYPortNumber) = y;
                        EHS_FB_FINISH(pWidget->mouseDownPortNumber);
                    }
                    break;
                }
            }
        }
    }
#endif
}

/**
 * Iterate through widget table and when find the viewport widget, fire its mouse up event
 * Assumes there is only one viewport
 */
void EhsWidgetTable_triggerViewportMouseUp(const EhsWidgetTableClass* pWidgetTable, int x, int y)
{
#if defined(EHS_GUI_SUPPORT_MODE_A)
    EhsFunctionInstanceDataType *pFIdata;
    EhsWidgetClass* pWidget;
    ehs_sint16 nIndex;
    if (pWidgetTable->initialised != EHS_MAGIC_NUMBER) return;

    for(nIndex = 0; nIndex < pWidgetTable->nSize; nIndex++)
    {
        pWidget = (pWidgetTable->pZOrderedWidget[nIndex]);
        if(pWidget)
        {
            /*If widget has its PFIData then that probably means it has a mouse click port*/
            if(pWidget->pFIData)
            {
                /*Check if it is a viewport */
                if(pWidget->eWidgetKind == EHS_WIDGET_KIND_VIEWPORT)
                {
                    pFIdata = pWidget->pFIData;
                    if (pWidget->mouseUpDownAbsXPortNumber > -1 && pWidget->mouseUpDownAbsYPortNumber > -1 && pWidget->mouseUpPortNumber > 1)
                    {
                        EHS_FB_OUT_I(pWidget->mouseUpDownAbsXPortNumber) = x;
                        EHS_FB_OUT_I(pWidget->mouseUpDownAbsYPortNumber) = y;
                        EHS_FB_FINISH(pWidget->mouseUpPortNumber);
                    }
                    break;
                }
            }
        }
    }
#endif
}

/**
 * Iterate through widget table and when find the viewport widget, fire its mouse drag event
 * Assumes there is only one viewport
 */
void EhsWidgetTable_triggerViewportMouseDrag(const EhsWidgetTableClass* pWidgetTable, int x, int y)
{
    // todo change all the following to #ifdef EHS_GUI_SUPPORT_MODE_A
#if defined(EHS_GUI_SUPPORT_MODE_A)
    EhsWidgetClass* pWidget;
    EhsFunctionInstanceDataType *pFIdata;
    ehs_sint16 nIndex;

    if (pWidgetTable->initialised != EHS_MAGIC_NUMBER) return;

    for(nIndex = 0; nIndex < pWidgetTable->nSize; nIndex++)
    {
        pWidget = (pWidgetTable->pZOrderedWidget[nIndex]);
        if(pWidget)
        {
            /*If widget has its PFIData then that probably means it has a mouse click port*/
            if(pWidget->pFIData)
            {
                /*Check if it is a viewport*/
                if(pWidget->eWidgetKind == EHS_WIDGET_KIND_VIEWPORT)
                {
                    pFIdata = pWidget->pFIData;
                    if (pWidget->mouseDragOffsetXPortNumber > -1 && pWidget->mouseDragOffsetYPortNumber > -1 && pWidget->mouseDragPortNumber > -1)
                    {
                        EHS_FB_OUT_I(pWidget->mouseDragOffsetXPortNumber) = x - pWidget->nMouseDownX;
                        EHS_FB_OUT_I(pWidget->mouseDragOffsetYPortNumber) = y - pWidget->nMouseDownY;
                        EHS_FB_FINISH(pWidget->mouseDragPortNumber);
                    }
                    break;
                }
            }
        }
    }
#endif
}

/**
 * Iterate through widget table and when find widgets that match coords, registers mouse down on widget
 */
void EhsWidgetTable_registerMouseDownOnWidgetMatchCoords(const EhsWidgetTableClass* pWidgetTable, int x, int y)
{
#if defined(EHS_GUI_SUPPORT_MODE_A)
    EhsWidgetClass* pWidget;
    ehs_sint16 nIndex;
    ehs_bool bClickProcessed = EHS_FALSE;
    if (pWidgetTable->initialised != EHS_MAGIC_NUMBER) return;

    // start with highest zorder (top layer) and work down, looking for widgets that are shown and whose boundary overlaps with click coords
    // process first widget we encounter and after that only widgets that have flag set to process clicks ignoring zorder
    for(nIndex = pWidgetTable->nSize - 1; nIndex >= 0; nIndex--)
    {
        pWidget = (pWidgetTable->pZOrderedWidget[nIndex]);
        if(pWidget)
        {
            // clear mouse down on all widgets before setting flag on those under the pointer
            pWidget->bRegisteredMouseDown = EHS_FALSE;
            if (!bClickProcessed || (bClickProcessed && pWidget->bCaptureClicksIgnoringZOrder))
            {
                if(EHS_WIDGET_STATE_SHOWN(pWidget->nState)) /*Only obLOGI("Viewport mousedrag event, %d,%d",x,y);jects that can be seen should fire click events*/
                {
                    /*If widget has its PFIData then that probably means it has a mouse click port*/
                    if(pWidget->pFIData)
                    {
                        /*Check if click coordinates fall inside widgets boundaries*/
                        if(((pWidget->xCurRect.nLeft <= x) && (x <= pWidget->xCurRect.nLeft + pWidget->xCurRect.nWidth)) && ((pWidget->xCurRect.nTop <= y) && (y <= pWidget->xCurRect.nTop + pWidget->xCurRect.nHeight)))
                        {
                            /*Mouse click was within widget's boundaries so fire it's click event*/
                            pWidget->bRegisteredMouseDown = EHS_TRUE;
                            if (pWidget->mouseDownPortNumber > -1)
                                EhsFunctionInstanceData_triggerEvent(pWidget->pFIData,(pWidget->mouseDownPortNumber));;
                            bClickProcessed = EHS_TRUE;
                        }
                    }
                    else
                    {
                        /* this is used for widgets created without pFIData e.g. GPIO widget */
                        if (pWidget->pfMouseDownEventFunc != NULL) {
                            /*Check if click coordinates fall inside widgets boundaries*/
                            if (((pWidget->xCurRect.nLeft <= x) && (x <= pWidget->xCurRect.nLeft + pWidget->xCurRect.nWidth)) && ((pWidget->xCurRect.nTop <= y) && (y <= pWidget->xCurRect.nTop + pWidget->xCurRect.nHeight)))
                            {
                                pWidget->pfMouseDownEventFunc(pWidget);
                            }
                        }
                    }
                }
            }
        }
    }
#endif
}

/**
 * Iterate through widget table and when find widgets that match coords, registers mouse up on widget
 * Triggers mouse click event on widget if mouse down was on same widget as mouse up
 */
void EhsWidgetTable_registerMouseUpOnWidgetMatchCoords(const EhsWidgetTableClass* pWidgetTable, int x, int y)
{
#if defined(EHS_GUI_SUPPORT_MODE_A)
    EhsWidgetClass* pWidget;
    ehs_sint16 nIndex;
    ehs_bool bClickProcessed = EHS_FALSE;
    if (pWidgetTable->initialised != EHS_MAGIC_NUMBER) return;

    // start with highest zorder (top layer) and work down, looking for widgets that are shown and whose boundary overlaps with click coords
    // process first widget we encounter and after that only widgets that have flag set to process clicks ignoring zorder
    for(nIndex = pWidgetTable->nSize - 1; nIndex >= 0; nIndex--)
    {
        pWidget = (pWidgetTable->pZOrderedWidget[nIndex]);
        if(pWidget)
        {
            if (!bClickProcessed || (bClickProcessed && pWidget->bCaptureClicksIgnoringZOrder))
            {
                if(EHS_WIDGET_STATE_SHOWN(pWidget->nState)) /*Only objects that can be seen should fire click events*/
                {
                    /*If widget has its PFIData then that probably means it has a mouse click port*/
                    if(pWidget->pFIData)
                    {
                        /*Check if click coordinates fall inside widgets boundaries*/
                        if(((pWidget->xCurRect.nLeft <= x) && (x <= pWidget->xCurRect.nLeft + pWidget->xCurRect.nWidth)) && ((pWidget->xCurRect.nTop <= y) && (y <= pWidget->xCurRect.nTop + pWidget->xCurRect.nHeight)))
                        {
                            // register as a click, only if mouse down was on same widget
                            if (pWidget->bRegisteredMouseDown)
                            {
                                /*Mouse click was within widget's boundaries so fire it's click event*/
                                if (pWidget->mouseClickPortNumber > -1)
                                    EhsFunctionInstanceData_triggerEvent(pWidget->pFIData,(pWidget->mouseClickPortNumber));
                                bClickProcessed = EHS_TRUE;
                            }
                        }
                    }
                }
            }
        }
    }
#endif
}


/**
 * Resize the widget to display the image at its original aspect ratio by changing either the height or the width
 * so that the resultant widget is bounded by the design rectangle
 *
 * @param[in] pWidget Specifies the widget to resize
 * @param[in] prSrc Specifies the proportions that we wish to keep
 */
void EhsWidget_resizeWidgetToMaintainAspectRatio(EhsWidgetClass* pWidget)  //, const EhsGraphicsRectangleClass* prSrc){
{
   // ehs_float dWHsrc; /* ratio of wid:ht for the two input rectangles */
    EhsGraphicsRectangleClass xDstRect; /* defines the bounds of the rectangle on the target */

    //dWHsrc = (ehs_float)pWidget->MediaRect.nWidth/(ehs_float)pWidget->MediaRect.nHeight;


    EhsGraphicsRectangle_proportionalScale(&xDstRect,&(pWidget->xCurRect)/*&(pWidget->xDesignRect)*/,&(pWidget->MediaRect));
    pWidget->xOrigRect.nHeight = xDstRect.nHeight;
    pWidget->xOrigRect.nWidth = xDstRect.nWidth;
    pWidget->xOrigRect.nTop = xDstRect.nTop;
    pWidget->xOrigRect.nLeft = xDstRect.nLeft;
    pWidget->xCurRect.nHeight = xDstRect.nHeight;
    pWidget->xCurRect.nWidth = xDstRect.nWidth;
    pWidget->xCurRect.nLeft = xDstRect.nLeft;
    pWidget->xCurRect.nTop = xDstRect.nTop;
}

/**
 * Reset the widget to its design time dimensions
 *
 * @param[in] pWidget Specifies the widget to resize
 */
void EhsWidget_resetWidgetSizeToDesignTime(EhsWidgetClass* pWidget)
{
    pWidget->xOrigRect.nHeight = pWidget->xDesignRect.nHeight;
    pWidget->xOrigRect.nWidth = pWidget->xDesignRect.nWidth;
    pWidget->xOrigRect.nTop= pWidget->xDesignRect.nTop;
    pWidget->xOrigRect.nLeft = pWidget->xDesignRect.nLeft;
    pWidget->xCurRect.nHeight = pWidget->xDesignRect.nHeight;
    pWidget->xCurRect.nWidth = pWidget->xDesignRect.nWidth;
    pWidget->xCurRect.nTop = pWidget->xDesignRect.nTop;
    pWidget->xCurRect.nLeft = pWidget->xDesignRect.nLeft;
}

/**
 * \brief Obtain a new widget. The widget comes from EhsWidgetTable
 * and updates the number of allocated widgets.
 *
 * \return Pointer to a new widget, or zero if this fails
 */
EhsWidgetClass* EhsWidgetTable_new(EhsWidgetTableClass* pTable)
{
    EhsWidgetClass* pWidget = NULL;
    if (pTable->initialised != EHS_MAGIC_NUMBER) return  NULL;

    if ((pTable->nSize+1) < EHS_MAX_WIDGET_INSTANCES)
    {
        pWidget = &(pTable->xWidget[pTable->nSize++]);
        pWidget->nState = EHS_WIDGET_STATE_EMPTY; // initize this for re-use
    }
    else
    {
        printf("The number of widgets exceeds maximum %d. Please increase \"EHS_MAX_WIDGET_INSTANCES\" in the platform config.\n", EHS_MAX_WIDGET_INSTANCES);
        EHSH_LOG_ERROR(EHS_MSG_ERROR_WIDGET_TOO_MANY);
    }

    return pWidget;
}

