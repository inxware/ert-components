/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/**
 * @file keypress.c
 *
 * Implementation of the keypress function in the Windows toolkit.
 * Includes target-specific code.
 *
 *
 * @author: inx limited
 *
 */

#include "target.h"

#include "keypress.h"
#include "globals.h"
//#include <string.h>
#include "setCompletes.h"

#include "inx-parameters.h"
#include "inx-component.h"

EHS_FB_FUNCTIONS_START(GUI_keypress)

EHS_FB_FUNCTION_ENTRY("hit", 0x00, GUI_keypress_hit)

EHS_FB_FUNCTION_ENTRY("enable", 0x01, GUI_keypress_Enable)

EHS_FB_FUNCTION_ENTRY("disable", 0x02, GUI_keypress_Disable)
EHS_FB_FUNCTIONS_END

#define EHS_FB_GUI_KEYPRESS -1 /**< Callback id for keypress event */

/*
 * Provides a callback chain to call whenever a key gets pressed.
 *
 * @todo ensure that this gets reset every time SODL is reloaded.
 */
EhsCallbackQueueType EhsGUIKeypressCallback = NULL;

/**
 * Character received from keyboard input
 */
EhsDataflowIntType EhsGUIKbHitChar;

typedef struct
{
    EhsCallbackQueueEntryType xCallback;
    ehs_bool bEnabled;			// is counter enabled
} EhsKeypressStruct;


/**
 * Define the identify function. If multiple keypress objects appear in the application,
 * we create a linked list between them.
 */
EHS_FB_IDENTIFY_FUNCTION(GUI_keypress)
{
//	EHS_FB_IDENTIFY_MEMORY = sizeof(EhsCallbackQueueEntryType);
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsKeypressStruct);
}



/**
 * Load the GUI parameters into the params structure, load
 * the associated bitmap into memory
 */
EHS_FB_INIT_FUNCTION(GUI_keypress)
{
//	EhsCallbackQueueEntryType *pParams = (EhsCallbackQueueEntryType*)EHS_FB_INIT_CONTEXT;
    EhsKeypressStruct* pKeypressStruct = (EhsKeypressStruct*)EHS_FB_INIT_CONTEXT;

    pKeypressStruct->bEnabled = EHS_FALSE;

    /* put this item into the keypress callback queue */
//	EhsCallbackQueue_register(&EhsGUIKeypressCallback,
//		EHS_FB_RUN_NAME(GUI_keypress_hit),
//		EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(EHS_FB_GUI_KEYPRESS),
//		pParams);

    /* put this item into the keypress callback queue */
    EhsCallbackQueue_register(&EhsGUIKeypressCallback,
                              EHS_FB_RUN_NAME(GUI_keypress_hit),
                              EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(EHS_FB_GUI_KEYPRESS),
                              &(pKeypressStruct->xCallback));

    return EHS_TRUE; /* initialise always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(GUI_keypress_Enable)
{
    EhsKeypressStruct* pKeypressStruct = (EhsKeypressStruct*)EHS_FB_RUN_CONTEXT;

    pKeypressStruct->bEnabled = EHS_TRUE;
    EHS_FB_FINISH(1);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(GUI_keypress_Disable)
{
    EhsKeypressStruct* pKeypressStruct = (EhsKeypressStruct*)EHS_FB_RUN_CONTEXT;

    pKeypressStruct->bEnabled = EHS_FALSE;
    EHS_FB_FINISH(1);
    return;
}

/**
 * A key has been pressed. Send out the output value, and see if there are any other
 * instances of this function block to tell about it.
 *
 * Note: This is called from the main Windows thread, rather than the EHS
 * thread, so we need to be sure that we don't trample over EHS.
 */
EHS_FB_RUN_FUNCTION(GUI_keypress_hit)
{
    EhsKeypressStruct* pKeypressStruct = (EhsKeypressStruct*)EHS_FB_RUN_CONTEXT;
    char c[2];
    ehs_sint32 ctrlkey = -1;
    if( pKeypressStruct->bEnabled == EHS_FALSE ) return;	// lazy return. Disabled, do nothing

    //port 1 should be string output
    //convert EhsGUIKbHitChar to a string
    if(EhsGUIKbHitChar>=32&&EhsGUIKbHitChar<=126)
    {
        //value is printable ascii so turn it into a string
        EHS_FB_OUT_I_API2(1) = EhsGUIKbHitChar;
        c[0]=(char)EhsGUIKbHitChar;
        c[1]='\0';
        EhsStrcpy(EHS_FB_OUT_S_API2(2),c);
        //SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
        EHS_FB_FINISH(1);
    }
    else   /* check for recognised control keys */
    {
        switch (EhsGUIKbHitChar)
        {
        /* Commands */
        case 0xff0d: // ok
            ctrlkey = 1;
            break;
        case 0xff1b: // exit (This is ESCAPE) - don't use to ungrab keyboard
            ctrlkey = 2;
            break;
        case 0xFF08: /* backspace  */
            ctrlkey = 3;
            break;
        case 0xFF09: /* Forward tab*/
            ctrlkey = 4;
            break;
        /* Navigation Keys */
        case 0xff52: // up
            ctrlkey = 10;
            break;
        case 0xff54: // down
            ctrlkey = 11;
            break;
        case 0xff51: /* left */
            ctrlkey = 12;
            break;
        case 0xff53: /* right */
            ctrlkey = 13;
            break;
        case 0xff55: /*pgup*/
            ctrlkey = 14;
            break;
        case 0xff56: /*pgdn*/
            ctrlkey = 15;
            break;
        case 0xff57: /*end*/
            ctrlkey = 16;
            break;
        case 0xff50: /*home*/
            ctrlkey = 17;
            break;
        case 0x020: // menu
            ctrlkey = 18;
            break;
        /* Function Keys */
        case 0xffbe: /* F1 */
            ctrlkey = 101;
            break;
        case 0xffbf: /* F2 */
            ctrlkey = 102;
            break;
        case 0xffc0: /* F3 */
            ctrlkey = 103;
            break;
        case 0xffc1: /* F4 */
            ctrlkey = 104;
            break;
        case 0xffc2: /* F5 */
            ctrlkey = 105;
            break;
        case 0xffc3: /* F6 */
            ctrlkey = 106;
            break;
        case 0xffc4: /* F7 */
            ctrlkey = 107;
            break;
        case 0xffc5: /* F8 */
            ctrlkey = 108;
            break;
        case 0xffc6: /* F9 */
            ctrlkey = 109;
            break;
        case 0xffc7: /* F10 */
            ctrlkey = 110;
            break;
        case 0xffc8: /* F11 */
            ctrlkey = 111;
            break;
        case 0xffc9: /* F12  */
            ctrlkey = 112;
            break;
        /* Modifier keys */
        case 0xffe1: /* Left Shift */
            ctrlkey = 20;
            break;
        case 0xffe2: /* Right Shift */
            ctrlkey = 21;
            break;
        case 0xffe3: /* Left Ctrl */
            ctrlkey = 22;
            break;
        case 0xffe4: /* Right Ctrl */
            ctrlkey = 23;
            break;
        case 0xffe9: /*  alt (left) */
            ctrlkey = 24;
            break;
        case 0xffea: /* alt (right)  */
            ctrlkey = 25;
            break;
        default:
            break;
        }
        if(ctrlkey > -1 )
        {
            EHS_FB_OUT_I_API2(3)=ctrlkey;
            EHS_FB_FINISH(2);
        }
    }

    return;
}


void EhsGUIKbClearCBs(void )
{
    EhsCallbackQueue_clear(&EhsGUIKeypressCallback);
}

/**
 * Function called by main windows loop when a character has been hit
 *
 * @param nCharacter Value of character that has been hit
 */
void EhsGUIKbHit(ehs_uint32 nCharacter)
{
    EhsGUIKbHitChar = nCharacter;
    EhsCallbackQueue_execute(&EhsGUIKeypressCallback);
}
