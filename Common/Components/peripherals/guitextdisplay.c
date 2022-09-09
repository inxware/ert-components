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
 * This is for LCD displays or text console outputs
 *
 */

#include "target.h"
//#ifdef EHS_GUI_SUPPORT

#include "guitextdisplay.h"
#include "guiparams.h"
#include "globals.h"
#include "widget.h"
#include "font.h"
/** @todo check which includes are required here */
#include <string.h>
#include <ctype.h>
#include "setCompletes.h"
#include "app_data.h"
#include "hal_string.h"
#include "hal_viewport.h"
#include "messages.h"

/*
struct EhsFbTextdisplayStruct
{
};
*/

EHS_FB_FUNCTIONS_START(gui_textdisplay)

EHS_FB_FUNCTION_ENTRY("update", 0x00, gui_textdisplay_update)

EHS_FB_FUNCTION_ENTRY("clear", 0x01, gui_textdisplay_clear)

EHS_FB_FUNCTION_ENTRY("poll", 0x02, gui_textdisplay_poll)
EHS_FB_FUNCTIONS_END

#define EHS_TEXTDISPLAY_TEXT_IN 0
#define EHS_TEXTDISPLAY_ROW_IN 1
#define EHS_TEXTDISPLAY_COL_IN 2

#define EHS_TEXTDISPLAY_TEXT_OUT 0

//struct EhsTextdisplayStruct
//{
//EhsCallbackQueueType EhsTextdisplayCallback;
//EhsCallbackQueueEntryType xCallbackEnd;					/**< used to callback end when video play is complete */
//unsigned char key;
//};

/**
 * Define the identify function
 */
EHS_FB_IDENTIFY_FUNCTION(gui_textdisplay)
{
    //EHS_FB_IDENTIFY_MEMORY = sizeof(struct EhsTextdisplayStruct);
}


/**
 * Load the GUI parameters into the params structure
 */
EHS_FB_INIT_FUNCTION(gui_textdisplay)
{
    /*Put USB initialisation stuff here */
    ehs_bool bRet = EHS_TRUE;
    //struct EhsTextdisplayStruct* pTextdisplay = (struct EhsTextdisplayStruct *)EHS_FB_INIT_CONTEXT;
    //pTextdisplay->EhsTextdisplayCallback = NULL;
    //EhsCallbackQueue_register(&pTextdisplay->EhsTextdisplayCallback, EHS_FB_RUN_NAME(gui_textdisplay_keypress), EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(-1), &pTextdisplay->xCallbackEnd);
    //EhsInitTextdisplay();
    EhsInitTextdisplay();
    return bRet;
}

/**
 * Thread function that polls for key presses from the lcd panel.
 */
EHS_FB_THREAD_FUNCTION(gui_textdisplay_poll_thread)
{
    //struct EhsTextdisplayStruct* pTextdisplay = (struct EhsTextdisplayStruct *)EHS_FB_RUN_CONTEXT;
    //Ehs_FB_ThreadStarted();
    /*Stuff for getting key presses.*/
    while(1)
    {
        switch(EhsReadKeysTextdisplay())
        {
        case 0x00 :
            EHS_FB_FINISH(13);
            break;

        case 0x01 :
            EHS_FB_FINISH(1);
            break;

        case 0x02 :
            EHS_FB_FINISH(2);
            break;

        case 0x03 :
            EHS_FB_FINISH(3);
            break;

        case 0x04 :
            EHS_FB_FINISH(4);
            break;

        case 0x05 :
            EHS_FB_FINISH(5);
            break;

        case 0x06 :
            EHS_FB_FINISH(6);
            break;

        case 0x07 :
            EHS_FB_FINISH(7);
            break;

        case 0x08 :
            EHS_FB_FINISH(8);
            break;

        case 0x09 :
            EHS_FB_FINISH(9);
            break;

        case 0x0a :
            EHS_FB_FINISH(10);
            break;

        case 0x0b :
            EHS_FB_FINISH(11);
            break;

        case 0x0c :
            EHS_FB_FINISH(12);
            break;
        }
    }
    EhsHThread_exit();
}

/**
 * Update the textbox string using text input, alpha level, x-offset and y-offset (where these are connected)
 */
EHS_FB_RUN_FUNCTION(gui_textdisplay_update)
{
    /*Put text update code here*/
    if((EHS_FB_IN_CONNECTED(EHS_TEXTDISPLAY_TEXT_IN)) && (EHS_FB_IN_CONNECTED(EHS_TEXTDISPLAY_ROW_IN)) && (EHS_FB_IN_CONNECTED(EHS_TEXTDISPLAY_COL_IN)))
    {
        /*All connections needed are present so get values and send to textdisplay writing function.*/
        EhsWriteTextdisplay(EHS_FB_IN_I(EHS_TEXTDISPLAY_ROW_IN), EHS_FB_IN_I(EHS_TEXTDISPLAY_COL_IN), EHS_FB_IN_S(EHS_TEXTDISPLAY_TEXT_IN) );
    }
    EHS_FB_FINISH(1);
}

EHS_FB_RUN_FUNCTION(gui_textdisplay_clear)
{
    EhsClearTextdisplay();
    EHS_FB_FINISH(1);
}

#ifdef EHS_RE_INTRODUCE_THREAD_HANDLES 
EHS_GLOBAL EhsTPThread pLcdThread = NULL;
#endif

EHS_FB_RUN_FUNCTION(gui_textdisplay_poll)
{
    pLcdThread = EHS_FB_START_THREAD(gui_textdisplay_poll_thread,-100);
}

/* @todo - what does this one do? */
EHS_FB_RUN_FUNCTION(gui_textdisplay_plus)
{
    EHS_FB_START_THREAD(gui_textdisplay_poll_thread,-100);
}


//#endif /* EHS_GUI_SUPPORT */
