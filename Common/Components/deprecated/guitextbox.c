/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * @file guitextbox.c
 *
 * generic processing for guitextbox (text) functions.
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
 * @section guitextbox
 * @anchor guitextbox
 * @subsection appliesTo Applies To:
 * - GUI &gt; text &gt; bool
 * - GUI &gt; text &gt; int
 * - GUI &gt; text &gt; real
 * - GUI &gt; text &gt; string
 * @subsection properties Properties:
 * <b>tag.</b> This is the name of the image. Lucid GUI Builder uses this name to refer to the image
 * when it is building the display including this image.
 *
 * <b>Widget-group</b>. This refers to the type of screen layout that will contain this image.
 *
 * @subsection description Description:
 * The <em>create</em>/<em>destroy</em> events are used to allow the text boxes (and other visual widgets)
 * that appear in the same widget group to be "turned on" (or off) all at the same time. At this time
 * it is necessary to manually connect together the create and destroy events for all widgets in the same widget group.
 *
 * The <em>show</em> and <em>hide</em> events are intended to allow individual images to be displayed or
 * removed from the display.
 *
 * The <em>update</em> allows the image to be modified via the <em>x</em>, <em>y</em> and <em>alpha</em> inputs.
 * The x and y inputs provide an offset to the initial screen position (which is defined in Lucid GUI Builder).
 * The alpha input changes the opacity of the image. It ranges between 0 (totally transparent) and 255 (maximum
 * opacity). If Lucid GUI Builder defines a text box as partially transparent, the alpha value of the text box
 * will not increase its opacity beyond that point.
 *
 */

#include "target.h"
#ifdef EHS_GUI_SUPPORT

#include "guitextbox.h"
#include "guiparams.h"
#include "globals.h"
#include "widget.h"
/** @todo check which includes are required here */
#include <string.h>
#include <ctype.h>
#include "setCompletes.h"
#include "app_data.h"
#include "hal_string.h"
#include "hal-api.h"

#define EHS_L_DEFAULT_INDENT 10u
#define EHS_L_DEFAULT_LINESEP 20u

EHS_FB_FUNCTIONS_START(gui_text_string)

EHS_FB_FUNCTION_ENTRY("create", 0x01, gui_text_create)

EHS_FB_FUNCTION_ENTRY("destroy", 0x02, gui_text_destroy)

EHS_FB_FUNCTION_ENTRY("show", 0x03, gui_text_show)

EHS_FB_FUNCTION_ENTRY("hide", 0x04, gui_text_hide)

EHS_FB_FUNCTION_ENTRY("update", 0x05, gui_text_string_update)
EHS_FB_FUNCTIONS_END

#ifdef EHS_TARGET_FP_SUPPORT
EHS_FB_FUNCTIONS_START(gui_text_float)

EHS_FB_FUNCTION_ENTRY("create", 0x01, gui_text_create)

EHS_FB_FUNCTION_ENTRY("destroy", 0x02, gui_text_destroy)

EHS_FB_FUNCTION_ENTRY("show", 0x03, gui_text_show)

EHS_FB_FUNCTION_ENTRY("hide", 0x04, gui_text_hide)

EHS_FB_FUNCTION_ENTRY("update", 0x05, gui_text_float_update)
EHS_FB_FUNCTIONS_END
#endif

EHS_FB_FUNCTIONS_START(gui_text_int)

EHS_FB_FUNCTION_ENTRY("create", 0x01, gui_text_create)

EHS_FB_FUNCTION_ENTRY("destroy", 0x02, gui_text_destroy)

EHS_FB_FUNCTION_ENTRY("show", 0x03, gui_text_show)

EHS_FB_FUNCTION_ENTRY("hide", 0x04, gui_text_hide)

EHS_FB_FUNCTION_ENTRY("update", 0x05, gui_text_int_update)
EHS_FB_FUNCTIONS_END

EHS_FB_FUNCTIONS_START(gui_text_bool)

EHS_FB_FUNCTION_ENTRY("create", 0x01, gui_text_create)

EHS_FB_FUNCTION_ENTRY("destroy", 0x02, gui_text_destroy)

EHS_FB_FUNCTION_ENTRY("show", 0x03, gui_text_show)

EHS_FB_FUNCTION_ENTRY("hide", 0x04, gui_text_hide)

EHS_FB_FUNCTION_ENTRY("update", 0x05, gui_text_bool_update)
EHS_FB_FUNCTIONS_END

EHS_LOCAL void Ehs_gui_text_update(EhsWidgetClass* pWidget, const ehs_char* szString, ehs_bool bAlphaConnected, EhsDataflowIntType nAlpha, ehs_bool bXConnected,
                                   EhsDataflowIntType nXoffset, ehs_bool bYConnected, EhsDataflowIntType nYoffset);

#define EHS_FB_GUI_TEXT_IN_X 0		/**< Function block input for X offset */
#define EHS_FB_GUI_TEXT_IN_Y 1		/**< Function block input for Y offset */
#define EHS_FB_GUI_TEXT_IN_ALPHA 2	/**< Function block input for alpha shading */
#define EHS_FB_GUI_TEXT_IN_VALUE 3  /**< Function block input for value to display */
/**
 * Define the identify function
 */
EHS_FB_IDENTIFY_FUNCTION(gui_text)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsWidgetClass**);
}

/**
 * Load the GUI parameters into the params structure
 */
EHS_FB_INIT_FUNCTION(gui_text)
{
    EhsGuiParamsType xParams;
    EhsGraphicsFontClass* pFont = NULL;
    ehs_bool bRet = EHS_FALSE; /* assume initialisation fails */
    EhsParseGuiParameters(EHS_FB_INIT_PARAMETERS,&xParams);
    if (xParams.eClass == EHS_WIDGET_CLASS_TEXTBOX)
    {
#ifndef EHS_DONT_USE_BASIC_FONTS
        pFont = EhsGraphicsFont_load(EHSHG_FONT_DEFAULT);
#endif
        *(EhsWidgetClass**)EHS_FB_INIT_CONTEXT =
            EhsWidgetTextbox_init(&(xParams.xRect), xParams.nZorder,
                                  EHS_L_DEFAULT_INDENT, EHS_L_DEFAULT_INDENT, EHS_L_DEFAULT_INDENT, EHS_L_DEFAULT_INDENT,
                                  EHS_L_DEFAULT_LINESEP,
                                  xParams.uClass.xTextbox.xFgColour, xParams.uClass.xTextbox.xBgColour, pFont);
        bRet = EHS_TRUE;
    }
    return bRet; /* initialisation always succeeds */
}

/**
 * Create the GUI image. Cause a handle to be created in the GUI system, but don't display
 * the item.
 */
EHS_FB_RUN_FUNCTION(gui_text_create)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    EhsWidget_create(pWidget);
    EHS_FB_FINISH(1);
    return;
}

/**
 * Destroy the textbox - remove the text from the screen
 * if it's visible.
 */
EHS_FB_RUN_FUNCTION(gui_text_destroy)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    EhsWidget_destroy(pWidget);
    EHS_FB_FINISH(1);
    return;
}

/**
 * Make the GUI text visible
 */
EHS_FB_RUN_FUNCTION(gui_text_show)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(gui_text_show));

    EhsWidget_show(pWidget);
    EHS_FB_FINISH(1);
}

/**
 * Remove the GUI text from the screen.
 */
EHS_FB_RUN_FUNCTION(gui_text_hide)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(gui_text_hide));

    EhsWidget_hide(pWidget);
    EHS_FB_FINISH(1);
}

/**
 * General purpose update function, contains the common code for all of the text update functions.
 *
 * @param[in] pWidget Widget to update
 * @param[in] szString text to be displayed
 * @param[in] bAlphaConnected true if the alpha value should be used
 * @param[in] nAlpha current alpha level to use for image
 * @param[in] bXConnected true if the x coordinate should be used
 * @param[in] nXoffset current x coordinate to use (value is offset to default image position)
 * @param[in] bYConnected true if the y coordinate should be used
 * @param[in] nYoffset current y coordinate to use (value is offset to default image position)
 *
 */
void Ehs_gui_text_update(EhsWidgetClass* pWidget, const ehs_char* szString, ehs_bool bAlphaConnected, EhsDataflowIntType nAlpha, ehs_bool bXConnected,
                         EhsDataflowIntType nXoffset, ehs_bool bYConnected, EhsDataflowIntType nYoffset)
{
    ehs_bool bMoveRequired = EHS_FALSE; /* assume that we never need to move, unless inputs are connected */

    if (bAlphaConnected)
    {
        if (nAlpha < 0) nAlpha = 0;
        if (nAlpha > 255) nAlpha = 255;
        EhsWidget_fade(pWidget, (ehs_uint8)nAlpha);
    }
    if (bXConnected)
    {
        bMoveRequired = EHS_TRUE;
    }
    if (bYConnected)
    {
        bMoveRequired = EHS_TRUE;
    }
    if (bMoveRequired)
    {
        EhsWidget_move(pWidget, nXoffset, nYoffset,0,0);
    }

    EhsWidgetTextbox_write(pWidget,szString);
}

/**
 * Update the textbox string using text input, alpha level, x-offset and y-offset (where these are connected)
 */
EHS_FB_RUN_FUNCTION(gui_text_string_update)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    char *str = EHS_FB_IN_S(EHS_FB_GUI_TEXT_IN_VALUE);

    Ehs_gui_text_update(pWidget,str,
                        EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_ALPHA),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_ALPHA),
                        EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_X),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_X),
                        EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_Y),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_Y));
    EHS_FB_FINISH(1);
}

#ifdef EHS_TARGET_FP_SUPPORT
/**
 * Update the textbox float using float input, alpha level, x-offset and y-offset (where these are connected)
 */
EHS_FB_RUN_FUNCTION(gui_text_float_update)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    char str[EHS_STRING_LENGTH_MAX];//TODO:STRINGLENGTH! Or can we always truncate to global (assuming this is the)
    EhsSprintf(str,"%f",EHS_FB_IN_F(EHS_FB_GUI_TEXT_IN_VALUE));

    Ehs_gui_text_update(pWidget,str,
                        EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_ALPHA),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_ALPHA),
                        EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_X),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_X),
                        EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_Y),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_Y));
    EHS_FB_FINISH(1);
}
#endif

/**
 * Update the textbox Int using int input, alpha level, x-offset and y-offset (where these are connected)
 */
EHS_FB_RUN_FUNCTION(gui_text_int_update)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    char str[EHS_STRING_LENGTH_MAX];
    EhsSprintf(str,"%d",EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_VALUE));

    Ehs_gui_text_update(pWidget,str,
                        EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_ALPHA),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_ALPHA),
                        EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_X),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_X),
                        EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_Y),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_Y));
    EHS_FB_FINISH(1);
}
/**
 * Update the textbox bool using bool input, alpha level, x-offset and y-offset (where these are connected)
 */
EHS_FB_RUN_FUNCTION(gui_text_bool_update)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    char str[EHS_STRING_LENGTH_MAX];

    str[0] = (EHS_FB_IN_B(EHS_FB_GUI_TEXT_IN_VALUE))?'T':'F';
    str[1] = '\0';

    Ehs_gui_text_update(pWidget,str,
                        EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_ALPHA),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_ALPHA),
                        EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_X),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_X),
                        EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_Y),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_Y));
    EHS_FB_FINISH(1);
}

#endif /* EHS_GUI_SUPPORT */
