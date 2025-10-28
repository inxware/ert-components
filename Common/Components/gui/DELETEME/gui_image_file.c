/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * @file gui_image_file.c
 *
 * generic processing for gui_image_file (bitmap) functions.
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
 * @section guiimagefile
 * @anchor guiimagefile
 * @subsection appliesTo Applies To:
 * GUI &gt; Image File
 * @subsection properties Properties:
 * <b>tag.</b> This is the name of the widget. Lucid GUI Builder uses this name to refer to the image
 * when it is building the display including this image.
 *
 * <b>Widget-group</b>. This refers to the type of screen layout that will contain this image.
 *
 * @subsection description Description:
 * The <em>create</em>/<em>destroy</em> events are used to allow the images (and other visual widgets)
 * that appear in the same widget group to be "turned on" (or off) all at the same time. At this time
 * it is necessary to manually connect together the create and destroy events for all widgets in the same widget group.
 * The create event causes the file name input to be read, and for a corresponding image to be displayed.
 * An image must be destroyed before it is possible to create another function block.
 *
 * The <em>show</em> and <em>hide</em> events are intended to allow individual images to be displayed or
 * removed from the display.
 *
 * The <em>update</em> allows the image to be modified via the <em>x</em>, <em>y</em>,
 * <em>width</em>, <em>height</em> and <em>alpha</em> inputs.
 * The x and y inputs provide an offset to the initial screen position (which is defined in Lucid GUI Builder).
 * The alpha input changes the opacity of the image. It ranges between 0 (totally transparent) and 255 (maximum
 * opacity). If Lucid GUI Builder defines a widget as partially transparent, the alpha value of the image block
 * will not increase its opacity beyond that point. The width and height inputs cause the image to be resized.
 *
 */


#include "globals.h"
#include "gui_image_file.h"
#include "guiparams.h"
#include "app_data.h"
/** @todo check which includes are required here */
#include <string.h>
#include "widget.h"
#include "hal-api.h" /* Required for logging */

EHS_FB_FUNCTIONS_START(GUI_Image_File)

EHS_FB_FUNCTION_ENTRY("create", 0x01 , GUI_Image_File_create)

EHS_FB_FUNCTION_ENTRY("destroy", 0x02, GUI_Image_File_destroy)

EHS_FB_FUNCTION_ENTRY("load", 0x03, GUI_Image_File_load)

EHS_FB_FUNCTION_ENTRY("show", 0x04, GUI_Image_File_show)

EHS_FB_FUNCTION_ENTRY("hide", 0x05, GUI_Image_File_hide)

EHS_FB_FUNCTION_ENTRY("update", 0x06, GUI_Image_File_update)
EHS_FB_FUNCTIONS_END

#define EHS_FB_GUIIMAGE_IN_FILENAME 0 	/**< Function block input for filename */
#define EHS_FB_GUIIMAGE_IN_X 0			/**< Function block input for X offset */
#define EHS_FB_GUIIMAGE_IN_Y 1			/**< Function block input for Y offset */
#define EHS_FB_GUIIMAGE_IN_WID 2		/**< Function block input for width */
#define EHS_FB_GUIIMAGE_IN_HT 3			/**< Function block input for height */
#define EHS_FB_GUIIMAGE_IN_ALPHA 4		/**< Function block input for alpha shading */
#define EHS_FB_GUI_TEXT_OUT_X 0		/**< Function block output for X offset */
#define EHS_FB_GUI_TEXT_OUT_Y 1		/**< Function block output for Y offset */
#define EHS_FB_GUI_TEXT_OUT_WID 2	/**< Function block output for width offset */
#define EHS_FB_GUI_TEXT_OUT_HT 3		/**< Function block output for height offset */

/**
 * Define the identify function
 */
EHS_FB_IDENTIFY_FUNCTION(GUI_Image_File)
{
    EHS_TRACE_FUNCTION(EHS_FB_IDENTIFY_NAME(GUI_Image_File));
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsWidgetClass**);
}

/**
 * Load the GUI parameters into the params structure, load
 * the associated bitmap into memory
 */
EHS_FB_INIT_FUNCTION(GUI_Image_File)
{
    EhsGuiParamsType xParams;
    ehs_bool bRet;
    char guiParams[MAX_PARAM_STR_LEN];
    const char* pParams;
    ehs_uint8 nByte;
    ehs_bool bMaintainAspect;
    ehs_bool bCaptureClicks;
    ehs_bool bLoadImageFromAppDir;

    EHS_TRACE_FUNCTION(EHS_FB_INIT_NAME(GUI_Image_File));
    pParams = ReadParmFile(&EHS_FB_INIT_PARAMETERS[4], guiParams);
    if (guiParams) {
        EhsParseGuiParameters(guiParams,&xParams);
        /* parse parameters */
        pParams = EhsGetUint8FromString(&nByte, pParams);
        bMaintainAspect = (ehs_bool)nByte;
        pParams = EhsGetUint8FromString(&nByte, pParams);
        bCaptureClicks = (ehs_bool)nByte;
        pParams = EhsGetUint8FromString(&nByte, pParams);
        bLoadImageFromAppDir = (ehs_bool)nByte;

        /* Set the image widget specific parameters too */
        *(EhsWidgetClass**)EHS_FB_INIT_CONTEXT = EhsWidgetImage_init(&xParams.xRect, xParams.nZorder, xParams.uClass.xBitmap.nImageAlpha,xParams.uClass.xBitmap.szBitmapName, bLoadImageFromAppDir);

        if ((*(EhsWidgetClass**)EHS_FB_INIT_CONTEXT == NULL) ||
                ((*(EhsWidgetClass**)(EHS_FB_INIT_CONTEXT))->nState == EHS_WIDGET_STATE_EMPTY))
        {
            bRet = EHS_FALSE;
        }
        else
        {
            /* set widget parameters */
            (*(EhsWidgetClass**)EHS_FB_RUN_CONTEXT)->bMaintainAspectRatio = bMaintainAspect;
            (*(EhsWidgetClass**)EHS_FB_RUN_CONTEXT)->bCaptureClicksIgnoringZOrder = bCaptureClicks;
            bRet = EHS_TRUE;
        }
    } else {
        *(EhsWidgetClass**)EHS_FB_INIT_CONTEXT = NULL;
    }
    return bRet;
}



/**
 * Create the GUI image. Load the image from a file, if it has not already been created.
 * If the image is already created, this will destroy it before loading another one.
 */
EHS_FB_RUN_FUNCTION(GUI_Image_File_create)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;

    if(!pWidget){
        return;
    }

    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(GUI_Image_File_create));

    /* don't bother checking to see if things have already been created
     * a second create will simply delete the old image and load in a new one */
//	if (EHS_FB_IN_CONNECTED(EHS_FB_GUIIMAGE_IN_FILENAME)) { /* if a string is connected then get a dynamic variable for the filename. */
//		EhsWidgetImage_setFilename(pWidget, EHS_FB_IN_S(EHS_FB_GUIIMAGE_IN_FILENAME), EHS_TRUE);
//	}
//	else { /* Otherwise read in the static name */
    /*  EhsWidgetImage_setFilename(pWidget, XXX)  -- the string is intialised with the static value */
//	}

    EhsWidget_create(pWidget); /* Decode the image if we have one */
    if (pWidget->bMaintainAspectRatio)
    {
        EhsWidget_resizeWidgetToMaintainAspectRatio(pWidget);
    }
    else
    {
        EhsWidget_resetWidgetSizeToDesignTime(pWidget);
    }

    if (pWidget->bMaintainAspectRatio)
    {
        EhsWidget_resizeWidgetToMaintainAspectRatio(pWidget);
    }
    else
    {
        EhsWidget_resetWidgetSizeToDesignTime(pWidget);
    }

    /*Set pointer in widget structure to point at instance data. Used for mouse click.*/
    pWidget->pFIData = EHS_FB_RUN_CONTEXT_REF;
#ifndef EHS_GUI_SUPPORT_MODE_B
    /*Set number of mouseClick port*/
    pWidget->mouseClickPortNumber = 2;
    pWidget->mouseDownPortNumber  = 3;
#endif
    /* set the output values for this widget */
    EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_X) 	= pWidget->xCurRect.nLeft;
    EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_Y) 	= pWidget->xCurRect.nTop;
    EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_WID)	= pWidget->xCurRect.nWidth;
    EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_HT) 	= pWidget->xCurRect.nHeight;

    EHS_FB_FINISH(1);
}

/**
 * Destroy the GUI image - release the handle and remove the image from the screen
 * if it's visible.
 */
EHS_FB_RUN_FUNCTION(GUI_Image_File_destroy)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;

    if(!pWidget){
        return;
    }

    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(GUI_Image_File_destroy));

    EhsWidget_destroy(pWidget);
    EHS_FB_FINISH(1);
}

/**
 * Load the image from the filename specified at runtime
 */
EHS_FB_RUN_FUNCTION(GUI_Image_File_load)
{
    EhsFunctionInstanceDataType *iBeforeVal;
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;

    if(!pWidget){
        return;
    }

    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(GUI_Image_File_load));

    if (EHS_FB_IN_CONNECTED(EHS_FB_GUIIMAGE_IN_FILENAME))   /* if a string is connected then get a dynamic variable for the filename. */
    {
        EhsWidgetImage_setFilename(pWidget, EHS_FB_IN_S(EHS_FB_GUIIMAGE_IN_FILENAME), EHS_TRUE);

        /* check that:
         * 1. the widget has been initialised by the subclass
         * 2. the widget has previously been created */
        if (EHS_WIDGET_STATE_INITIALIZED(pWidget->nState) &&
                ((pWidget->nState & EHS_WIDGET_STATE_CREATED) == EHS_WIDGET_STATE_CREATED))
        {
            // implicit destroy and recreate
            iBeforeVal = pWidget->pFIData;
            EhsWidget_destroy(pWidget);
            EhsWidget_create(pWidget);

            //NOTE - as pfiData is a static associated with each function have to reset pfiData to value set by create function
            pWidget->pFIData = iBeforeVal;
#ifndef EHS_GUI_SUPPORT_MODE_B
            /*Set number of mouseClick port ~todo - this needs making nicer!*/
            pWidget->mouseClickPortNumber = 2;
            pWidget->mouseDownPortNumber = 3;
#endif
            /* Use any dynamic offsets for the widget to position the new image to where eer it may have moved to */
            //pWidget->UpdatedOffsettRect;

            EhsWidget_resetWidgetSizeToDesignTime(pWidget);

            Ehs_widget_position_update(pWidget, EHS_TRUE, pWidget->nAlpha,  EHS_TRUE, pWidget->UpdatedOffsettRect.nLeft, EHS_TRUE,  pWidget->UpdatedOffsettRect.nTop, EHS_TRUE, pWidget->UpdatedOffsettRect.nWidth, EHS_TRUE, pWidget->UpdatedOffsettRect.nHeight);

            if (pWidget->bMaintainAspectRatio)
                EhsWidget_resizeWidgetToMaintainAspectRatio(pWidget);

            Ehs_widget_commit(pWidget);
        }
    }
    EHS_FB_FINISH(1);
}


/**
 * Make the GUI image visible
 */
EHS_FB_RUN_FUNCTION(GUI_Image_File_show)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;

    if(!pWidget){
        return;
    }

    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(GUI_Image_File_show));

    EhsWidget_show(pWidget);
    EHS_FB_FINISH(1);
}

/**
 * Remove the GUI image from the screen.
 */
EHS_FB_RUN_FUNCTION(GUI_Image_File_hide)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;

    if(!pWidget){
        return;
    }

    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(GUI_Image_File_hide));

    EhsWidget_hide(pWidget);
    EHS_FB_FINISH(1);
}

/**
 * Update the GUI image. This is a general purpose operation that depends upon
 * which inputs (X,Y,alpha) are connected up.
 */
EHS_FB_RUN_FUNCTION(GUI_Image_File_update)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;

    if(!pWidget){
        return;
    }
    
    EhsDataflowIntType nXoffset = 0;
    EhsDataflowIntType nYoffset = 0;
    EhsDataflowIntType nWoffset = 0;
    EhsDataflowIntType nHoffset = 0;
    EhsDataflowIntType nAlpha = 128;

    ehs_bool bAlphaConnected = EHS_FALSE;
    ehs_bool bXConnected = EHS_FALSE;
    ehs_bool bYConnected = EHS_FALSE;
    ehs_bool bWConnected = EHS_FALSE;
    ehs_bool bHConnected = EHS_FALSE;

    ehs_bool bMoveRequired = EHS_FALSE; /* assume that we never need to move, unless inputs are connected */
    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(GUI_Image_File_update));

    if (EHS_FB_IN_CONNECTED(EHS_FB_GUIIMAGE_IN_ALPHA))
    {
        nAlpha = EHS_FB_IN_I(EHS_FB_GUIIMAGE_IN_ALPHA);
        if (nAlpha < 0) nAlpha = 0;
        else if (nAlpha > 255) nAlpha = 255;
        bAlphaConnected = EHS_TRUE;

        EhsWidget_fade(pWidget, (ehs_uint8)nAlpha);
    }

    if (EHS_FB_IN_CONNECTED(EHS_FB_GUIIMAGE_IN_X))
    {
        nXoffset = NCAPSA_nIn(EHS_FB_GUIIMAGE_IN_X);
        bXConnected = bMoveRequired = EHS_TRUE;
    }
    if (EHS_FB_IN_CONNECTED(EHS_FB_GUIIMAGE_IN_Y))
    {
        nYoffset = NCAPSA_nIn(EHS_FB_GUIIMAGE_IN_Y);
        bYConnected = bMoveRequired = EHS_TRUE;
    }
    if (EHS_FB_IN_CONNECTED(EHS_FB_GUIIMAGE_IN_WID))
    {
        nWoffset = EHS_FB_IN_I(EHS_FB_GUIIMAGE_IN_WID);
        bWConnected = bMoveRequired = EHS_TRUE;
    }
    if (EHS_FB_IN_CONNECTED(EHS_FB_GUIIMAGE_IN_HT))
    {
        nHoffset = EHS_FB_IN_I(EHS_FB_GUIIMAGE_IN_HT);
        bHConnected = bMoveRequired = EHS_TRUE;
    }
    if (bMoveRequired)
    {
        EhsWidget_resetWidgetSizeToDesignTime(pWidget); // do everything relative to the design position and not the last AR maintained position and size
        Ehs_widget_position_update(pWidget, bAlphaConnected, nAlpha, bXConnected, nXoffset, bYConnected, nYoffset, bWConnected, nWoffset, bHConnected, nHoffset);
        if ((*(EhsWidgetClass**)(EHS_FB_INIT_CONTEXT))->bMaintainAspectRatio)
        {
            EhsWidget_resizeWidgetToMaintainAspectRatio((*(EhsWidgetClass**)(EHS_FB_INIT_CONTEXT)));
        }
        else
        {
            //EhsWidget_resetWidgetSizeToDesignTime((*(EhsWidgetClass**)(EHS_FB_INIT_CONTEXT)));
        }

        // above does this : EhsWidget_move(pWidget, nXoffset, nYoffset, nWoffset, nHoffset);
    }

    if (bMoveRequired || bAlphaConnected){
        Ehs_widget_commit(pWidget);
    }

    /* set the output values for this widget */
    EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_X) 	= pWidget->xCurRect.nLeft;
    EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_Y) 	= pWidget->xCurRect.nTop;
    EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_WID)	= pWidget->xCurRect.nWidth;
    EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_HT) 	= pWidget->xCurRect.nHeight;

    EHS_FB_FINISH(1);
}

