/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
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

#include "target.h"
#ifdef EHS_GUI_SUPPORT

#include "guitextbox_orig.h"
#include "guiparams.h"
#include "globals.h"
#include "widget.h"
/** @todo check which includes are required here */
#include <string.h>
#include <ctype.h>
#include "setCompletes.h"
#include "hal_string.h"
#include "font.h"

#define EHS_L_DEFAULT_INDENT 10u
#define EHS_L_DEFAULT_LINESEP 20u

EHS_FB_FUNCTIONS_START(GUI_TextboxString)

EHS_FB_FUNCTION_ENTRY("CreateGUI_WriteTextBoxString", 0x01, GUI_Textbox_create)

EHS_FB_FUNCTION_ENTRY("DestroyGUI_WriteTextBoxString", 0x02, GUI_Textbox_destroy)

EHS_FB_FUNCTION_ENTRY("RunGUI_WriteTextBoxString", 0x03, GUI_TextboxString_write)
EHS_FB_FUNCTIONS_END

#ifdef EHS_TARGET_FP_SUPPORT
EHS_FB_FUNCTIONS_START(GUI_TextboxFloat)

EHS_FB_FUNCTION_ENTRY("CreateGUI_WriteTextBoxFloat", 0x01, GUI_Textbox_create)

EHS_FB_FUNCTION_ENTRY("DestroyGUI_WriteTextBoxFloat", 0x02, GUI_Textbox_destroy)

EHS_FB_FUNCTION_ENTRY("RunGUI_WriteTextBoxFloat", 0x03, GUI_TextboxFloat_write)
EHS_FB_FUNCTIONS_END
#endif

EHS_FB_FUNCTIONS_START(GUI_TextboxInt)

EHS_FB_FUNCTION_ENTRY("CreateGUI_WriteTextBoxInt", 0x01, GUI_Textbox_create)

EHS_FB_FUNCTION_ENTRY("DestroyGUI_WriteTextBoxInt", 0x02, GUI_Textbox_destroy)

EHS_FB_FUNCTION_ENTRY("RunGUI_WriteTextBoxInt", 0x03, GUI_TextboxInt_write)
EHS_FB_FUNCTIONS_END

EHS_FB_FUNCTIONS_START(GUI_TextboxBool)

EHS_FB_FUNCTION_ENTRY("CreateGUI_WriteTextBoxBool",0x01, GUI_Textbox_create)

EHS_FB_FUNCTION_ENTRY("DestroyGUI_WriteTextBoxBool", 0x02, GUI_Textbox_destroy)

EHS_FB_FUNCTION_ENTRY("RunGUI_WriteTextBoxInt", 0x03, GUI_TextboxBool_write)
EHS_FB_FUNCTIONS_END

/**
 * Define the identify function
 */
EHS_FB_IDENTIFY_FUNCTION(GUI_Textbox)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsWidgetClass**);
}

/**
 * Load the GUI parameters into the params structure
 */
EHS_FB_INIT_FUNCTION(GUI_Textbox)
{
    EhsGuiParamsType xParams;
    EhsGraphicsFontClass* pFont = NULL;
    ehs_bool bRet = EHS_FALSE; /* assume initialisation fails */
    EhsParseGuiParameters(EHS_FB_INIT_PARAMETERS,&xParams);
#ifndef EHS_DONT_USE_BASIC_FONTS
    pFont = EhsGraphicsFont_load(EHSHG_FONT_DEFAULT);
#endif
    if (xParams.eClass == EHS_WIDGET_CLASS_TEXTBOX)
    {
        *(EhsWidgetClass**)EHS_FB_INIT_CONTEXT = EhsWidgetTextbox_init(&(xParams.xRect),
                EHS_L_DEFAULT_INDENT, EHS_L_DEFAULT_INDENT, EHS_L_DEFAULT_INDENT, EHS_L_DEFAULT_INDENT,
                EHS_L_DEFAULT_LINESEP,
                xParams.nZorder, xParams.uClass.xTextbox.xFgColour,
                xParams.uClass.xTextbox.xBgColour,pFont);
        bRet = EHS_TRUE;
    }
    return bRet; /* initialisation always succeeds */
}

/**
 * Create the GUI image. Cause a handle to be created in the GUI system, but don't display
 * the item.
 */
EHS_FB_RUN_FUNCTION(GUI_Textbox_create)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    EhsWidget_create(pWidget);
    /* SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);  -- there are no setCompletes for bitmaps */
    return;
}

/**
 * Destroy the textbox - remove the text from the screen
 * if it's visible.
 */
EHS_FB_RUN_FUNCTION(GUI_Textbox_destroy)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    EhsWidget_destroy(pWidget);
    /* SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);  -- there are no setCompletes for bitmaps */
    return;
}

/**
 * Update the textbox string
 */
EHS_FB_RUN_FUNCTION(GUI_TextboxString_write)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    char *p;
    char *str = NCAPSA_szIn(0);
    ehs_bool bDisplay = EHS_FALSE;

    for (p = str; p && *p; p++)
    {
        if (isprint(*p) && !isspace(*p))
        {
            bDisplay = EHS_TRUE;
            break;
        }
    }
    if (bDisplay)
    {
        EhsWidgetTextbox_write(pWidget,str);
        EhsWidget_show(pWidget);
    }
    else
    {
        EhsWidget_hide(pWidget);
    }


    SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    return;
}

#ifdef EHS_TARGET_FP_SUPPORT
/**
 * Update the textbox float
 */
EHS_FB_RUN_FUNCTION(GUI_TextboxFloat_write)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    EhsDataflowFloatType fInput = NCAPSA_dIn(0);
    char str[EHS_STRING_LENGTH_MAX];

    EhsSprintf(str,"%f",fInput);
    EhsWidgetTextbox_write(pWidget,str);
    EhsWidget_show(pWidget);
    SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    return;
}
#endif

/**
 * Update the textbox Int
 */
EHS_FB_RUN_FUNCTION(GUI_TextboxInt_write)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    char str[EHS_STRING_LENGTH_MAX];
    EhsDataflowIntType nInput = NCAPSA_nIn(0);

    EhsSprintf(str,"%d",nInput);
    EhsWidgetTextbox_write(pWidget,str);
    EhsWidget_show(pWidget);
    SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    return;
}
/**
 * Update the textbox bool
 */
EHS_FB_RUN_FUNCTION(GUI_TextboxBool_write)
{
    EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
    char str[2]; /* character + line terminator */
    EhsDataflowBoolType bInput = NCAPSA_bIn(0);

    str[0] = (bInput)?'T':'F';
    str[1] = '\0';
    EhsWidgetTextbox_write(pWidget,str);
    EhsWidget_show(pWidget);
    SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    return;
}

#endif /* EHS_GUI_SUPPORT */
