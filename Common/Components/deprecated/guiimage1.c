/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * @file guiimage1.c
 *
 * generic processing for guiimage (bitmap) functions.
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
 * @section guiimage
 * @anchor guiimage
 * @subsection appliesTo Applies To:
 * GUI &gt; Image
 * @subsection properties Properties:
 * <b>tag.</b> This is the name of the image. Lucid GUI Builder uses this name to refer to the image
 * when it is building the display including this image.
 *
 * <b>Widget-group</b>. This refers to the type of screen layout that will contain this image.
 *
 * @subsection description Description:
 * The <em>create</em>/<em>destroy</em> events are used to allow the images (and other visual widgets)
 * that appear in the same widget group to be "turned on" (or off) all at the same time. At this time
 * it is necessary to manually connect together the create and destroy events for all widgets in the same widget group.
 *
 * The <em>show</em> and <em>hide</em> events are intended to allow individual images to be displayed or
 * removed from the display.
 *
 * The <em>update</em> allows the image to be modified via the <em>x</em>, <em>y</em> and <em>alpha</em> inputs.
 * The x and y inputs provide an offset to the initial screen position (which is defined in Lucid GUI Builder).
 * The alpha input changes the opacity of the image. It ranges between 0 (totally transparent) and 255 (maximum
 * opacity). If Lucid GUI Builder defines a widget as partially transparent, the alpha value of the image block
 * will not increase its opacity beyond that point.
 *
 */

#ifdef EHS_GUI_SUPPORT

#include "globals.h"
#include "guiimage1.h"
#include "guiparams.h"
#include "app_data.h"
/** @todo check which includes are required here */
#include <string.h>
#include "widget.h"
#include "hal-api.h" /* Needed for logging */

EHS_FB_FUNCTIONS_START(GUI_Image1)

EHS_FB_FUNCTION_ENTRY("create", 0x01, GUI_Image1_create)

EHS_FB_FUNCTION_ENTRY("destroy", 0x02, GUI_Image1_destroy)

EHS_FB_FUNCTION_ENTRY("show", 0x03, GUI_Image1_show)

EHS_FB_FUNCTION_ENTRY("hide", 0x04, GUI_Image1_hide)

EHS_FB_FUNCTION_ENTRY("update", 0x05, GUI_Image1_update)
EHS_FB_FUNCTIONS_END

#define EHS_FB_GUIIMAGE_IN_X 0		/**< Function block input for X offset */
#define EHS_FB_GUIIMAGE_IN_Y 1		/**< Function block input for Y offset */
#define EHS_FB_GUIIMAGE_IN_ALPHA 2	/**< Function block input for alpha shading */

/**
 * Define the identify function
 */
EHS_FB_IDENTIFY_FUNCTION(GUI_Image1)
{
    EHS_TRACE_FUNCTION(EHS_FB_IDENTIFY_NAME(GUI_Image1));
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsWidgetClass**);
}

/**
 * Load the GUI parameters into the params structure, load
 * the associated bitmap into memory
 */
EHS_FB_INIT_FUNCTION(GUI_Image1)
{
    EhsGuiParamsType xParams;
    ehs_bool bRet;

    EHS_TRACE_FUNCTION(EHS_FB_INIT_NAME(GUI_Image1));
    EhsParseGuiParameters(EHS_FB_INIT_PARAMETERS,&xParams);
    *(EhsWidgetClass**)EHS_FB_INIT_CONTEXT = EhsWidgetImage_init(&(xParams.xRect), xParams.nZorder, xParams.uClass.xBitmap.nImageAlpha, xParams.uClass.xBitmap.szBitmapName);
    if ((*(EhsWidgetClass**)EHS_FB_INIT_CONTEXT == NULL) ||
            ((*(EhsWidgetClass**)(EHS_FB_INIT_CONTEXT))->nState == EHS_WIDGET_STATE_EMPTY))
        bRet = EHS_FALSE;
    else
        bRet = EHS_TRUE;

    return bRet;
}



/**
 * Create the GUI image. Cause a handle to be created in the GUI system, but don't display
 * the item.
 */
EHS_FB_RUN_FUNCTION(GUI_Image1_create)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;

    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(GUI_Image1_create));
    EhsWidget_create(pWidget);
    EHS_FB_FINISH(1);
}

/**
 * Destroy the GUI image - release the handle and remove the image from the screen
 * if it's visible.
 */
EHS_FB_RUN_FUNCTION(GUI_Image1_destroy)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;

    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(GUI_Image1_destroy));
    EhsWidget_destroy(pWidget);
    EHS_FB_FINISH(1);
}

/**
 * Make the GUI image visible
 */
EHS_FB_RUN_FUNCTION(GUI_Image1_show)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(GUI_Image1_show));

    EhsWidget_show(pWidget);
    EHS_FB_FINISH(1);
}

/**
 * Remove the GUI image from the screen.
 */
EHS_FB_RUN_FUNCTION(GUI_Image1_hide)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(GUI_Image1_hide));

    EhsWidget_hide(pWidget);
    EHS_FB_FINISH(1);
}

/**
 * Update the GUI image. This is a general purpose operation that depends upon
 * which inputs (X,Y,alpha) are connected up.
 */
EHS_FB_RUN_FUNCTION(GUI_Image1_update)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    EhsDataflowIntType nXoffset = 0;
    EhsDataflowIntType nYoffset = 0;
    ehs_bool bMoveRequired = EHS_FALSE; /* assume that we never need to move, unless inputs are connected */
    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(GUI_Image1_update));

    if (EHS_FB_IN_CONNECTED(EHS_FB_GUIIMAGE_IN_ALPHA))
    {
        EhsDataflowIntType nAlpha = EHS_FB_IN_I(EHS_FB_GUIIMAGE_IN_ALPHA);
        if (nAlpha < 0) nAlpha = 0;
        else if (nAlpha > 255) nAlpha = 255;

        EhsWidget_fade(pWidget, (ehs_uint8)nAlpha);
    }

    if (EHS_FB_IN_CONNECTED(EHS_FB_GUIIMAGE_IN_X))
    {
        nXoffset = NCAPSA_nIn(EHS_FB_GUIIMAGE_IN_X);
        bMoveRequired = EHS_TRUE;
    }
    if (EHS_FB_IN_CONNECTED(EHS_FB_GUIIMAGE_IN_Y))
    {
        nYoffset = NCAPSA_nIn(EHS_FB_GUIIMAGE_IN_Y);
        bMoveRequired = EHS_TRUE;
    }
    if (bMoveRequired)
    {
        EhsWidget_move(pWidget, nXoffset, nYoffset, 0, 0);
    }

    EHS_FB_FINISH(1);
}
#endif /* EHS_GUI_SUPPORT */
