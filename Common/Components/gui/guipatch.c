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
 * @section guipatch
 * @anchor guipatch
 * @subsection appliesTo Applies To:
 * - GUI &gt; patch
 * @subsection properties Properties:
 * <b>tag.</b> This is the name of the image. Lucid GUI Builder uses this name to refer to the image
 * when it is building the display including this image.
 *
 * <b>Widget-group</b>. This refers to the type of screen layout that will contain this image.
 *
 * @subsection description Description:
 * The <em>create</em>/<em>destroy</em> events are used to allow the patches (and other visual widgets)
 * that appear in the same widget group to be "turned on" (or off) all at the same time. At this time
 * it is necessary to manually connect together the create and destroy events for all widgets in the same widget group.
 *
 * The <em>show</em> and <em>hide</em> events are intended to allow individual patches to be displayed or
 * removed from the display.
 *
 * The <em>update</em> allows the patch to be modified via the <em>x</em>, <em>y</em>, <em>wid</em>, <em>ht</em>
 * and <em>alpha</em> inputs.
 * The x and y inputs provide an offset to the initial screen position (which is defined in Lucid GUI Builder).
 * The width and height inputs provide an offset to the initial widget height and width.
 * The alpha input changes the opacity of the image. It ranges between 0 (totally transparent) and 255 (maximum
 * opacity). If Lucid GUI Builder defines a patch as partially transparent, the alpha value of the patch
 * cannot not increase its opacity beyond that point - that is the alpha input can make a patch more transparent,
 * but not more opaque.
 *
 * The <em>x</em>, <em>y</em>, <em>wid</em> and <em>ht</em> outputs return the absolute position and size of the
 * patch. These are updated by the create event and by the update event.
 *
 */

#include "target.h"
#ifdef EHS_GUI_SUPPORT

#include "guipatch.h"
#include "guiparams.h"
#include "ehs_fb_types.h"
#include "globals.h"
#include "widget.h"
/** @todo check which includes are required here */
#include "setCompletes.h"
#include "app_data.h"

#include "hal-api.h"

EHS_FB_FUNCTIONS_START(gui_patch)

EHS_FB_FUNCTION_ENTRY("create", 0x00, gui_patch_create)

EHS_FB_FUNCTION_ENTRY("destroy", 0x01, gui_patch_destroy)

EHS_FB_FUNCTION_ENTRY("show", 0x02, gui_patch_show)

EHS_FB_FUNCTION_ENTRY("hide", 0x03, gui_patch_hide)

EHS_FB_FUNCTION_ENTRY("update", 0x04, gui_patch_update)
EHS_FB_FUNCTIONS_END

#define EHS_FB_GUI_PATCH_IN_X 0		/**< Function block input for X offset */
#define EHS_FB_GUI_PATCH_IN_Y 1		/**< Function block input for Y offset */
#define EHS_FB_GUI_PATCH_IN_WID 2	/**< Function block input for width offset */
#define EHS_FB_GUI_PATCH_IN_HT 3	/**< Function block input for height offset */
#define EHS_FB_GUI_PATCH_IN_ALPHA 4	/**< Function block input for alpha shading */
#define EHS_FB_GUI_PATCH_OUT_X 0	/**< Function block output for absolute X position */
#define EHS_FB_GUI_PATCH_OUT_Y 1	/**< Function block output for absolute Y position */
#define EHS_FB_GUI_PATCH_OUT_WID 2	/**< Function block output for absolute width */
#define EHS_FB_GUI_PATCH_OUT_HT 3	/**< Function block output for absolute height */

/**
 * Define the identify function
 */
EHS_FB_IDENTIFY_FUNCTION(gui_patch)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsWidgetClass**);
}

/**
 * Load the GUI parameters into the params structure
 */
EHS_FB_INIT_FUNCTION(gui_patch)
{
    EhsGuiParamsType xParams;
    ehs_bool bRet = EHS_FALSE; /* assume initialisation fails */
    char guiParams[MAX_PARAM_STR_LEN];
    const char* pParams;
    ehs_uint8 nByte;

    EHS_TRACE_FUNCTION(EHS_FB_INIT_NAME(GUI_ImageFile));
    pParams = ReadParmFile(&EHS_FB_INIT_PARAMETERS[4], guiParams);
    EhsParseGuiParameters(guiParams,&xParams);
    if (xParams.eClass == EHS_WIDGET_CLASS_PATCH)
    {
        *(EhsWidgetClass**)EHS_FB_INIT_CONTEXT = EhsWidgetPatch_init(&xParams.xRect, xParams.nZorder,xParams.uClass.xPatch);
        if ((*(EhsWidgetClass**)EHS_FB_INIT_CONTEXT == NULL) ||
                ((*(EhsWidgetClass**)(EHS_FB_INIT_CONTEXT))->nState == EHS_WIDGET_STATE_EMPTY))
        {
            bRet = EHS_FALSE;
        }
        else
        {

            /* parse LAB parameters */
            pParams = EhsGetUint8FromString(&nByte, pParams);
            (*(EhsWidgetClass**)EHS_FB_RUN_CONTEXT)->bMaintainAspectRatio = (ehs_bool)nByte; //@todo Aspect ratio parameters for text and patches should be removed - only applies to images
            pParams = EhsGetUint8FromString(&nByte, pParams);
            (*(EhsWidgetClass**)EHS_FB_RUN_CONTEXT)->bCaptureClicksIgnoringZOrder = (ehs_bool)nByte;

            bRet = EHS_TRUE;
        }
    }



    return bRet; /* initialisation always succeeds -- why? [NR] */
}

/**
 * Create the GUI image. Cause a handle to be created in the GUI system, but don't display
 * the item.
 */
EHS_FB_RUN_FUNCTION(gui_patch_create)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    EhsWidget_create(pWidget);

    /*Set pointer in widget structure to point at instance data. Used for mouse click.*/
    pWidget->pFIData = EHS_FB_RUN_CONTEXT_REF;

    /*Set number of mouseClick port*/
    pWidget->mouseClickPortNumber = 2;
    pWidget->mouseDownPortNumber = 3;

    /* set the output values for this widget */
    EHS_FB_OUT_I(EHS_FB_GUI_PATCH_OUT_X) 	= pWidget->xCurRect.nLeft;
    EHS_FB_OUT_I(EHS_FB_GUI_PATCH_OUT_Y) 	= pWidget->xCurRect.nTop;
    EHS_FB_OUT_I(EHS_FB_GUI_PATCH_OUT_WID)	= pWidget->xCurRect.nWidth;
    EHS_FB_OUT_I(EHS_FB_GUI_PATCH_OUT_HT) 	= pWidget->xCurRect.nHeight;

    EHS_FB_FINISH(1);
    return;
}

/**
 * Destroy the textbox - remove the text from the screen
 * if it's visible.
 */
EHS_FB_RUN_FUNCTION(gui_patch_destroy)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    EhsWidget_destroy(pWidget);
    EHS_FB_FINISH(1);
    return;
}

/**
 * Make the GUI text visible
 */
EHS_FB_RUN_FUNCTION(gui_patch_show)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;

//	EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(gui_text_show)); /* @todo what does this do, and how is gui_text_show defined/scoped? */

    EhsWidget_show(pWidget);
    EHS_FB_FINISH(1);
}

/**
 * Remove the GUI text from the screen.
 */
EHS_FB_RUN_FUNCTION(gui_patch_hide)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(gui_text_hide));

    EhsWidget_hide(pWidget);
    EHS_FB_FINISH(1);
}

/**
 * Update the textbox string using text input, alpha level, x-offset and y-offset (where these are connected)
 */
EHS_FB_RUN_FUNCTION(gui_patch_update)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;

    ehs_bool bMoveRequired = EHS_FALSE; /* assume that we never need to move, unless inputs are connected */
    EhsDataflowIntType nXoffset = 0;
    EhsDataflowIntType nYoffset = 0;
    EhsDataflowIntType nWOffset = 0;
    EhsDataflowIntType nHOffset = 0;
    EhsDataflowIntType nAlpha = 0;

    ehs_bool bAlphaConnected = EHS_FALSE;
    ehs_bool bXConnected = EHS_FALSE;
    ehs_bool bYConnected = EHS_FALSE;
    ehs_bool bWConnected = EHS_FALSE;
    ehs_bool bHConnected = EHS_FALSE;

    if (EHS_FB_IN_CONNECTED(EHS_FB_GUI_PATCH_IN_ALPHA))
    {
        nAlpha = EHS_FB_IN_I(EHS_FB_GUI_PATCH_IN_ALPHA); /*Added this line to fix alpha blending 18/07/2008 PB*/
        if (nAlpha < 0) nAlpha = 0;
        if (nAlpha > 255) nAlpha = 255;
        EhsWidget_fade(pWidget, (ehs_uint8)nAlpha);
        bAlphaConnected = EHS_TRUE;
    }
    if (EHS_FB_IN_CONNECTED(EHS_FB_GUI_PATCH_IN_X))
    {
        nXoffset = EHS_FB_IN_I(EHS_FB_GUI_PATCH_IN_X);
        bXConnected = bMoveRequired = EHS_TRUE;
    }
    if (EHS_FB_IN_CONNECTED(EHS_FB_GUI_PATCH_IN_Y))
    {
        nYoffset = EHS_FB_IN_I(EHS_FB_GUI_PATCH_IN_Y);
        bYConnected = bMoveRequired = EHS_TRUE;
    }
    if (EHS_FB_IN_CONNECTED(EHS_FB_GUI_PATCH_IN_WID))
    {
        nWOffset = EHS_FB_IN_I(EHS_FB_GUI_PATCH_IN_WID);
        bWConnected = bMoveRequired = EHS_TRUE;
    }
    if (EHS_FB_IN_CONNECTED(EHS_FB_GUI_PATCH_IN_HT))
    {
        nHOffset = EHS_FB_IN_I(EHS_FB_GUI_PATCH_IN_HT);
        bHConnected = bMoveRequired = EHS_TRUE;
    }
    if (bMoveRequired)
    {
        EhsWidget_move(pWidget, nXoffset, nYoffset, nWOffset, nHOffset);
        Ehs_widget_position_update(pWidget, bAlphaConnected, nAlpha, bXConnected, nXoffset, bYConnected, nYoffset, bWConnected, nWOffset, bHConnected, nHOffset);
    }

    /* set the output values for this widget */
    EHS_FB_OUT_I(EHS_FB_GUI_PATCH_OUT_X) 	= pWidget->xCurRect.nLeft;
    EHS_FB_OUT_I(EHS_FB_GUI_PATCH_OUT_Y) 	= pWidget->xCurRect.nTop;
    EHS_FB_OUT_I(EHS_FB_GUI_PATCH_OUT_WID)	= pWidget->xCurRect.nWidth;
    EHS_FB_OUT_I(EHS_FB_GUI_PATCH_OUT_HT) 	= pWidget->xCurRect.nHeight;

    EHS_FB_FINISH(1);
}

#endif /* EHS_GUI_SUPPORT */
