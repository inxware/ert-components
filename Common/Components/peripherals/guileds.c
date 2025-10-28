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
 * The <em>update</em> allows the textbox to be modified via the <em>x</em>, <em>y</em>, <em>wid</em>,
 * <em>ht</em> and <em>alpha</em> inputs.
 * The x and y inputs provide an offset to the initial screen position (which is defined in Lucid GUI Builder).
 * The width and height inputs provide an offset to the initial widget height and width.
 * The alpha input changes the opacity of the image. It ranges between 0 (totally transparent) and 255 (maximum
 * opacity). If Lucid GUI Builder defines a text box as partially transparent, the alpha value of the text box
 * will not increase its opacity beyond that point.
 * The x and y inputs provide an offset to the initial screen position (which is defined in Lucid GUI Builder).
 *
 * The <em>x</em>, <em>y</em>, <em>wid</em> and <em>ht</em> outputs return the absolute position and size of the
 * patch. These are updated by the create event and by the update event.
 *
 */

#include "globals.h"
#include "guileds.h"
#include "guiparams.h"
#include "setCompletes.h"
#include "app_data.h"
#include "hal_string.h"
#include "messages.h"

/*
struct EhsFbTextdisplayStruct
{
};
*/

EHS_FB_FUNCTIONS_START(gui_leds)

EHS_FB_FUNCTION_ENTRY("update", 0x01, gui_leds_update)
EHS_FB_FUNCTIONS_END

/**
 * Define the identify function
 */
EHS_FB_IDENTIFY_FUNCTION(gui_leds)
{
    //EHS_FB_IDENTIFY_MEMORY = sizeof(struct EhsTextdisplayStruct);
}

/**
 * Load the GUI parameters into the params structure
 */
EHS_FB_INIT_FUNCTION(gui_leds)
{
    ehs_bool bRet = EHS_TRUE;
    EhsInitLeds();
    return bRet;
}

/**
 * Update the textbox string using text input, alpha level, x-offset and y-offset (where these are connected)
 */
EHS_FB_RUN_FUNCTION(gui_leds_update)
{
    if(EHS_FB_IN_CONNECTED(0))
    {
        EhsSetLed(0, EHS_FB_IN_B(0));
    }
    if(EHS_FB_IN_CONNECTED(1))
    {
        EhsSetLed(1, EHS_FB_IN_B(1));
    }
    if(EHS_FB_IN_CONNECTED(2))
    {
        EhsSetLed(2, EHS_FB_IN_B(2));
    }
    if(EHS_FB_IN_CONNECTED(3))
    {
        EhsSetLed(3, EHS_FB_IN_B(3));
    }
    if(EHS_FB_IN_CONNECTED(4))
    {
        EhsSetLed(4, EHS_FB_IN_B(4));
    }
    if(EHS_FB_IN_CONNECTED(5))
    {
        EhsSetLed(5, EHS_FB_IN_B(5));
    }
    if(EHS_FB_IN_CONNECTED(6))
    {
        EhsSetLed(6, EHS_FB_IN_B(6));
    }
}
