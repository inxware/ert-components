/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/**
 * @file guipatch.c
 *
 * generic processing for patch functions.
 * target specific handling is defined elsewhere
 *
 * @todo The handling of completions of GUI operations needs to be
 * handled better in this function. Currently it is assumed that returning
 * from a GUI operation is sufficient to complete the operation. A better approach
 * would be for EHS_FB_RUN_CONTEXT and the completion port to be passed to the GUI
 * operation, and for this to be handled upon actual completion of the GUI operation.
 * This does mean that the EhsFunctionInstanceData_triggerEvent operation needs to be
 * threadsafe, as it might be called by an OS operation.
 *
 * @author: inx limited
 *
 */

/**
 * @page FunctionReference Lucid Function Block Reference Guide
 * @section Viewport
 * @anchor viewport
 * @subsection appliesTo Applies To:
 * - GUI &gt; viewport
 * @subsection properties Properties:
 * <b>tag.</b> This is the name of the area as found in LGB, which uses this name to refer to the image
 * when it is building the display including this image.
 *
 * <b>Widget-group</b>. This refers to the type of screen layout that will contain this image.
 *
 * @subsection description Description:
 * The <em>create</em>/<em>destroy</em> events are used to allow the viewport to be shown in screen (typically as a window).
 *
 * The <em>show</em> and <em>hide</em> events to show or hide the the viewport.
 *
 * The <em>update</em> allows the patch to be modified via the <em>x</em>, <em>y</em>, <em>wid</em>, <em>ht</em>
 * and <em>alpha</em> inputs.
 * The x and y inputs provide an offset to the initial screen position (which is defined in Lucid GUI Builder), or hard coded as parameters.
 * The width and height inputs provide an offset to the initial widget height and width.
 * The alpha input changes the opacity of the window background. It ranges between 0 (totally transparent) and 255 (maximum
 * opacity).
 *
 * The <em>x</em>, <em>y</em>, <em>wid</em> and <em>ht</em> outputs return the absolute position and size of the
 * window, in either pixels or percentage of screen provided by the host. These are updated by the create event and by the update event.
 *
 */

#include "target.h"
#ifndef XEHS_GUI_SUPPORT

#include "guiviewport.h"
#include "guiparams.h"
#include "ehs_fb_types.h"
#include "globals.h"
#include "widget.h"
#include "widget_viewport.h"
/** @todo check which includes are required here */
#include "setCompletes.h"
#include "app_data.h"

#include "hal-api.h" /* Required for logging */

EHS_FB_FUNCTIONS_START(gui_viewport)
EHS_FB_FUNCTION_ENTRY("create", 0x00, gui_viewport_create)

EHS_FB_FUNCTION_ENTRY("destroy", 0x01, gui_viewport_destroy)

EHS_FB_FUNCTION_ENTRY("show", 0x02, gui_viewport_show)

EHS_FB_FUNCTION_ENTRY("hide", 0x03, gui_viewport_hide)

EHS_FB_FUNCTION_ENTRY("update", 0x04, gui_viewport_update)
EHS_FB_FUNCTIONS_END

#define EHS_FB_GUI_VIEWPORT_IN_X 0		/**< Function block input for X offset */
#define EHS_FB_GUI_VIEWPORT_IN_Y 1		/**< Function block input for Y offset */
#define EHS_FB_GUI_VIEWPORT_IN_WID 2	/**< Function block input for width offset */
#define EHS_FB_GUI_VIEWPORT_IN_HT 3	/**< Function block input for height offset */
#define EHS_FB_GUI_VIEWPORT_IN_ALPHA 4	/**< Function block input for alpha shading */
#define EHS_FB_GUI_VIEWPORT_IN_RED 5
#define EHS_FB_GUI_VIEWPORT_IN_GREEN 6
#define EHS_FB_GUI_VIEWPORT_IN_BLUE 7
#define EHS_FB_GUI_VIEWPORT_OUT_X 0	/**< Function block output for absolute X position */
#define EHS_FB_GUI_VIEWPORT_OUT_Y 1	/**< Function block output for absolute Y position */
#define EHS_FB_GUI_VIEWPORT_OUT_WID 2	/**< Function block output for absolute width */
#define EHS_FB_GUI_VIEWPORT_OUT_HT 3	/**< Function block output for absolute height */
#define EHS_FB_GUI_VIEWPORT_OUT_UP_DOWN_X 4	/**< Function block output abs X position of mouse up/down*/
#define EHS_FB_GUI_VIEWPORT_OUT_UP_DOWN_Y 5	/**< Function block output abs Y position of mouse up/down */
#define EHS_FB_GUI_VIEWPORT_OUT_DRAG_OFFSET_X 6	/**< Function block output offset in X position */
#define EHS_FB_GUI_VIEWPORT_OUT_DRAG_OFFSET_Y 7	/**< Function block output offset in Y position */

/*
 * Define the Viewport's generic parameters
 */

//struct EhsGUIViewPortStruct
//{
//	EhsGraphicsRectangleClass xClipRect;	/**< Clipping rectangle - used when drawing all images */
//	ehs_bool bViewportChanged;			/**< The pixbuf has changed, we need to copy it into pPixmap */
//	ehs_uint8 transparency; // transparency of window
//	EhsGraphicsColourClass viewColour; // This contains the colout and alpha value for the background.
//	//@todo the viewColour and xClipRect are duplicated as global variables in the attached.
//};

extern EhsGraphicsColourClass viewColour; // we set the background colour and transparency here
extern EhsGraphicsRectangleClass viewRectangle; //size of the viewport.

/**
 * Define the identify function
 */
EHS_FB_IDENTIFY_FUNCTION(gui_viewport)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsWidgetClass**); //we will borrow the structure of a standatd widget. We use global variable for the rest!!
    //@todo this only allows for a single widget.
}

/**
 * Load the GUI parameters into the params structure
 */

EHS_FB_INIT_FUNCTION(gui_viewport)
{
    EhsGuiParamsType xParams;
    EhsWidgetClass *pWidget;
    ehs_bool bRet = EHS_FALSE; /* assume initialisation fails */
    char guiParams[MAX_PARAM_STR_LEN];
    const char* pParams;
    ehs_uint8 nByte;
    ehs_bool bRelative;
    ehs_sint32 nTop,nHeight,nLeft,nWidth;
    ehs_sint32 nScreenWidth = -1;
    ehs_sint32 nScreenHeight = -1;


    EHS_TRACE_FUNCTION(EHS_FB_INIT_NAME(GUI_ImageFile));
    pParams = ReadParmFile(&EHS_FB_INIT_PARAMETERS[4], guiParams);
    EhsParseGuiParameters(guiParams,&xParams);

    /* parse coordinate block parameters */
    pParams = EhsGetUint8FromString(&nByte, pParams);
    bRelative = (ehs_bool)nByte;
    pParams = EhsGetSint32FromString(&nTop, pParams);
    pParams = EhsGetSint32FromString(&nHeight, pParams);
    pParams = EhsGetSint32FromString(&nLeft, pParams);
    pParams = EhsGetSint32FromString(&nWidth, pParams);

    //if (xParams.eClass == EHS_WIDGET_CLASS_VIEWPORT)
    //{@todo reinstate this when the new types are recognised and specified properly in the tools etc.
    bRet = EHS_TRUE;
    //}
    /* Create a widget struct using the parameters from the LGB generated block */
    *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT =	EhsWidgetViewport_init(&xParams.xRect, xParams.nZorder, xParams.uClass.xPatch);//@todo this should be params

    pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;

    /* if we have valid hard coded values then write these in */
    pWidget->bRelativeCoordinates = bRelative;
    if (bRelative) EhsWidgetsetToScreenSize(&nScreenWidth,&nScreenHeight); //set the widge data sructures to full screen, which EhsWidget_AdjustCoordinates will use and adjust.

    /* we need to set the new coordinates in the global iewport parameter set also */
    EhsWidget_AdjustCoordinates(pWidget, bRelative, nLeft, nWidth, nTop, nHeight ); // updates the widget original position in all rectangles
    // update the primary view incase the size values have changed
    EhsPrimaryViewportInfo_setWidth(pWidget->xOrigRect.nWidth);
    EhsPrimaryViewportInfo_setHeight(pWidget->xOrigRect.nHeight);



    /* Now control the actual graphics using the HAL graphics functions */
    EhsWidgetViewport_setwindow(pWidget, pWidget->xOrigRect.nLeft, pWidget->xOrigRect.nTop, pWidget->xOrigRect.nWidth, pWidget->xOrigRect.nHeight);
    EhsWidgetViewport_fade(pWidget, 255, /* pWidget->specific.patch.nBaseAlpha,*/
                           pWidget->specificWidgetType.patch.xColour.sComp.nRed,
                           pWidget->specificWidgetType.patch.xColour.sComp.nGreen,
                           pWidget->specificWidgetType.patch.xColour.sComp.nBlue);
    EhsWidgetViewport_show(pWidget); // we do this to set the zorder - @todo viewports are always visible from init - these should follow the rules..
    return bRet; /* initialisation always assumed to succeed */
}

/**
 * Create the GUI image. Cause a handle to be created in the GUI system, but don't display
 * the item.
 */
EHS_FB_RUN_FUNCTION(gui_viewport_create)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    EhsWidget_create(pWidget);

    /*Set pointer in widget structure to point at instance data. Used for mouse click.*/
    pWidget->pFIData = EHS_FB_RUN_CONTEXT_REF;

    /*Set number of mouseClick, mouseUp, mouseDrag ports*/
    pWidget->mouseDownPortNumber = 2;
    pWidget->mouseUpPortNumber = 3;
    pWidget->mouseDragPortNumber = 4;
    pWidget->mouseUpDownAbsXPortNumber = EHS_FB_GUI_VIEWPORT_OUT_UP_DOWN_X;
    pWidget->mouseUpDownAbsYPortNumber = EHS_FB_GUI_VIEWPORT_OUT_UP_DOWN_Y;
    pWidget->mouseDragOffsetXPortNumber = EHS_FB_GUI_VIEWPORT_OUT_DRAG_OFFSET_X;
    pWidget->mouseDragOffsetYPortNumber = EHS_FB_GUI_VIEWPORT_OUT_DRAG_OFFSET_Y;


    /* set the output values for this widget */
    EHS_FB_OUT_I(EHS_FB_GUI_VIEWPORT_OUT_X) 	= pWidget->xCurRect.nLeft;
    EHS_FB_OUT_I(EHS_FB_GUI_VIEWPORT_OUT_Y) 	= pWidget->xCurRect.nTop;
    EHS_FB_OUT_I(EHS_FB_GUI_VIEWPORT_OUT_WID)	= pWidget->xCurRect.nWidth;
    EHS_FB_OUT_I(EHS_FB_GUI_VIEWPORT_OUT_HT) 	= pWidget->xCurRect.nHeight;
    EHS_FB_OUT_I(EHS_FB_GUI_VIEWPORT_OUT_UP_DOWN_X)	= 0;
    EHS_FB_OUT_I(EHS_FB_GUI_VIEWPORT_OUT_UP_DOWN_Y) = 0;
    EHS_FB_OUT_I(EHS_FB_GUI_VIEWPORT_OUT_DRAG_OFFSET_X)	= 0;
    EHS_FB_OUT_I(EHS_FB_GUI_VIEWPORT_OUT_DRAG_OFFSET_Y) = 0;

    EHS_FB_FINISH(1);
    return;
}

/**
 * Destroy the textbox - remove the text from the screen
 * if it's visible.
 */
EHS_FB_RUN_FUNCTION(gui_viewport_destroy)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    EhsWidget_destroy(pWidget);
    EHS_FB_FINISH(1);
    return;
}

/**
 * Make the GUI text visible
 */

EHS_FB_RUN_FUNCTION(gui_viewport_show)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;

    //EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(gui_viewport_show)); /* @todo what does this do, and how is gui_text_show defined/scoped? */

    EhsWidgetViewport_show(pWidget);
    EHS_FB_FINISH(1);
}

/**
 * Remove the GUI text from the screen.
 */
EHS_FB_RUN_FUNCTION(gui_viewport_hide)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(gui_viewport_hide));

    EhsWidgetViewport_hide(pWidget);
    EHS_FB_FINISH(1);
}

/**
 * Update the textbox string using text input, alpha level, x-offset and y-offset (where these are connected)
 */
EHS_FB_RUN_FUNCTION(gui_viewport_update)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;

    ehs_bool bMoveRequired = EHS_FALSE; /* assume that we never need to move, unless inputs are connected */
    ehs_bool bFadeRequired = EHS_FALSE;
    EhsDataflowIntType nXoffset = 0;
    EhsDataflowIntType nYoffset = 0;
    EhsDataflowIntType nWidOffset = 0;
    EhsDataflowIntType nHtOffset = 0;
    EhsDataflowIntType nAlpha = 0;
    EhsDataflowIntType nRed = 0;
    EhsDataflowIntType nGreen = 0;
    EhsDataflowIntType nBlue = 0;

    if (EHS_FB_IN_CONNECTED(EHS_FB_GUI_VIEWPORT_IN_ALPHA))
    {
        nAlpha = EHS_FB_IN_I(EHS_FB_GUI_VIEWPORT_IN_ALPHA); /*Added this line to fix alpha blending 18/07/2008 PB*/
        if (nAlpha < 0) nAlpha = 0;
        if (nAlpha > 255) nAlpha = 255;
        bFadeRequired = EHS_TRUE;
    }
    if (EHS_FB_IN_CONNECTED(EHS_FB_GUI_VIEWPORT_IN_X))
    {
        nXoffset = EHS_FB_IN_I(EHS_FB_GUI_VIEWPORT_IN_X);
        bMoveRequired = EHS_TRUE;
    }
    if (EHS_FB_IN_CONNECTED(EHS_FB_GUI_VIEWPORT_IN_Y))
    {
        nYoffset = EHS_FB_IN_I(EHS_FB_GUI_VIEWPORT_IN_Y);
        bMoveRequired = EHS_TRUE;
    }
    if (EHS_FB_IN_CONNECTED(EHS_FB_GUI_VIEWPORT_IN_WID))
    {
        nWidOffset = EHS_FB_IN_I(EHS_FB_GUI_VIEWPORT_IN_WID);
        bMoveRequired = EHS_TRUE;
    }
    if (EHS_FB_IN_CONNECTED(EHS_FB_GUI_VIEWPORT_IN_HT))
    {
        nHtOffset = EHS_FB_IN_I(EHS_FB_GUI_VIEWPORT_IN_HT);
        bMoveRequired = EHS_TRUE;
    }
    /*
    if(EHS_FB_IN_CONNECTED(EHS_FB_GUI_VIEWPORT_IN_RED))
    {
    	nRed = EHS_FB_IN_I(EHS_FB_GUI_VIEWPORT_IN_RED);
    	if(nRed < 0) nRed = 0;
    	if(nRed > 255) nRed = 255;
    	bFadeRequired = EHS_TRUE;
    }
    if(EHS_FB_IN_CONNECTED(EHS_FB_GUI_VIEWPORT_IN_GREEN))
    {
    	nGreen = EHS_FB_IN_I(EHS_FB_GUI_VIEWPORT_IN_GREEN);
    	if(nGreen < 0) nGreen = 0;
    	if(nGreen > 255) nGreen = 255;
    	bFadeRequired = EHS_TRUE;
    }
    if(EHS_FB_IN_CONNECTED(EHS_FB_GUI_VIEWPORT_IN_BLUE))
    {
    	nBlue = EHS_FB_IN_I(EHS_FB_GUI_VIEWPORT_IN_BLUE);
    	if(nBlue < 0) nBlue = 0;
    	if(nBlue > 255) nBlue = 255;
    	bFadeRequired = EHS_TRUE;
    }
    */
    if (bMoveRequired)
    {
        EhsWidgetViewport_update(pWidget, nXoffset, nYoffset, nWidOffset, nHtOffset);
    }
    if(bFadeRequired)
    {
        EhsWidgetViewport_fade(pWidget, (ehs_uint8)nAlpha, (ehs_uint8)nRed, (ehs_uint8)nGreen, (ehs_uint8)nBlue);
    }
    /* set the output values for this widget */
    EHS_FB_OUT_I(EHS_FB_GUI_VIEWPORT_OUT_X) 	= pWidget->xCurRect.nLeft;
    EHS_FB_OUT_I(EHS_FB_GUI_VIEWPORT_OUT_Y) 	= pWidget->xCurRect.nTop;
    EHS_FB_OUT_I(EHS_FB_GUI_VIEWPORT_OUT_WID)	= pWidget->xCurRect.nWidth;
    EHS_FB_OUT_I(EHS_FB_GUI_VIEWPORT_OUT_HT) 	= pWidget->xCurRect.nHeight;

    EHS_FB_FINISH(1);
}

#endif /* EHS_GUI_SUPPORT */
