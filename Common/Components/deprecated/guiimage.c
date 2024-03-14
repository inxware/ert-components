/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/**
 * @file guiimage.c
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


#include "target.h"
#ifdef EHS_GUI_SUPPORT

#include "guiimage.h"
#include "guiparams.h"
#include "globals.h"
#include "app_data.h"
/** @todo check which includes are required here */
#include <string.h>
#include "widget.h"
#include "hal-api.h" /* Needed for logging */

EHS_FB_FUNCTIONS_START(GUI_Image)

EHS_FB_FUNCTION_ENTRY("RunGUI_LoadImage", 0x01, GUI_Image_create)

EHS_FB_FUNCTION_ENTRY("DestroyGUI_Image", 0x02, GUI_Image_destroy)

EHS_FB_FUNCTION_ENTRY("RealizeGUI_Image", 0x03, GUI_Image_show)

EHS_FB_FUNCTION_ENTRY("UnrealizeGUI_Image", 0x04, GUI_Image_hide)

EHS_FB_FUNCTION_ENTRY("RunGUI_MoveImage", 0x05, GUI_Image_update)
EHS_FB_FUNCTIONS_END

#define EHS_FB_GUIIMAGE_IN_X 0		/**< Function block input for X offset */
#define EHS_FB_GUIIMAGE_IN_Y 1		/**< Function block input for Y offset */

/**
 * Define the identify function
 */
EHS_FB_IDENTIFY_FUNCTION(GUI_Image)
{
    EHS_TRACE_FUNCTION(EHS_FB_IDENTIFY_NAME(GUI_Image));
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsWidgetClass**);
}

/**
 * Load the GUI parameters into the params structure, load
 * the associated bitmap into memory
 */
EHS_FB_INIT_FUNCTION(GUI_Image)
{
    EhsGuiParamsType xParams;
    ehs_bool bRet;

    EHS_TRACE_FUNCTION(EHS_FB_INIT_NAME(GUI_Image));
    EhsParseGuiParameters(EHS_FB_INIT_PARAMETERS,&xParams);
    *(EhsWidgetClass**)EHS_FB_INIT_CONTEXT = EhsWidgetImage_init(&xParams.xRect, xParams.nZorder, xParams.uClass.xBitmap.nImageAlpha, xParams.uClass.xBitmap.szBitmapName);
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
EHS_FB_RUN_FUNCTION(GUI_Image_create)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;

    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(GUI_Image_create));
    EhsWidget_create(pWidget);
    EHS_FB_FINISH(1);
}

/**
 * Destroy the GUI image - release the handle and remove the image from the screen
 * if it's visible.
 */
EHS_FB_RUN_FUNCTION(GUI_Image_destroy)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;

    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(GUI_Image_destroy));
    EhsWidget_destroy(pWidget);
    EHS_FB_FINISH(1);
}

/**
 * Make the GUI image visible
 */
EHS_FB_RUN_FUNCTION(GUI_Image_show)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(GUI_Image_show));

    EhsWidget_show(pWidget);
    EHS_FB_FINISH(1);
}

/**
 * Remove the GUI image from the screen.
 */
EHS_FB_RUN_FUNCTION(GUI_Image_hide)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(GUI_Image_hide));

    EhsWidget_hide(pWidget);
    EHS_FB_FINISH(1);
}

/**
 * Update the GUI image. This is a general purpose operation that depends upon
 * which inputs (X,Y,alpha) are connected up.
 */
EHS_FB_RUN_FUNCTION(GUI_Image_update)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    ehs_bool bAlphaChanged = EHS_FALSE; /* determine whether the image alpha has changed */
    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(GUI_Image_update));

    if (EHS_FB_IN_CONNECTED(EHS_FB_GUIIMAGE_IN_X) && EHS_FB_IN_CONNECTED(EHS_FB_GUIIMAGE_IN_Y))
    {
        EhsDataflowIntType nXoffset = NCAPSA_nIn(EHS_FB_GUIIMAGE_IN_X);
        EhsDataflowIntType nYoffset = NCAPSA_nIn(EHS_FB_GUIIMAGE_IN_Y);

        EhsWidget_move(pWidget, nXoffset, nYoffset,0,0);
    }

    EHS_FB_FINISH(1);
}
#endif /* EHS_GUI_SUPPORT */
