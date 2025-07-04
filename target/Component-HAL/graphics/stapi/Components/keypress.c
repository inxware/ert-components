/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file keypress.c
 *
 * Implementation of the keypress function in the Windows toolkit.
 * Includes target-specific code.
 *
 *
 * @author: inx limited
 */

#include "target.h"

#include "keypress.h"
#include "globals.h"
#include <string.h>
#include "setCompletes.h"


EHS_FB_FUNCTIONS_START(gtk_keypress)
EHS_FB_FUNCTION_ENTRY("hit", 0x01, gtk_keypress_hit)
EHS_FB_FUNCTIONS_END

#define EHS_FB_GTKX86_KEYPRESS -1 /**< Callback id for keypress event */

/*
 * Provides a callback chain to call whenever a key gets pressed.
 *
 * @todo ensure that this gets reset every time SODL is reloaded.
 */
EhsCallbackQueueType EhsGtkKeypressCallback = NULL;

/**
 * Character received from keyboard input
 */
EhsDataflowIntType EhsGtkKbHitChar;

/**
 * Define the identify function. If multiple keypress objects appear in the application,
 * we create a linked list between them.
 */
EHS_FB_IDENTIFY_FUNCTION(gtk_keypress)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsCallbackQueueEntryType);
}

/**
 * Load the GUI parameters into the params structure, load
 * the associated bitmap into memory
 */
EHS_FB_INIT_FUNCTION(gtk_keypress)
{
    EhsCallbackQueueEntryType *pParams = (EhsCallbackQueueEntryType*)EHS_FB_INIT_CONTEXT;

    /* put this item into the keypress callback queue */
    EhsCallbackQueue_register(&EhsGtkKeypressCallback,
                              EHS_FB_RUN_NAME(gtk_keypress_hit),
                              EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(EHS_FB_GTKX86_KEYPRESS),
                              pParams);
    return EHS_TRUE; /* initialise always succeeds */
}

/**
 * A key has been pressed. Send out the output value, and see if there are any other
 * instances of this function block to tell about it.
 *
 * Note: This is called from the main Windows thread, rather than the EHS
 * thread, so we need to be sure that we don't trample over EHS.
 */
EHS_FB_RUN_FUNCTION(gtk_keypress_hit)
{
    if (NCAPSA_nNumOuts > 0)
    {
        NCAPSA_nOut(0) = EhsGtkKbHitChar;
    }

    SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    return;
}

/**
 * Function called by main windows loop when a character has been hit
 *
 * @param nCharacter Value of character that has been hit
 */
void EhsGtkKbHit(ehs_uint32 nCharacter)
{
    EhsGtkKbHitChar = nCharacter;
    EhsCallbackQueue_execute(&EhsGtkKeypressCallback);
}
