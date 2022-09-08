/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/**
 * @file dtv_remote.c
 *
 * Implementation of the remote control function in the DTV toolkit.
 * Includes target-specific code.
 *
 *
 * @author: inx limited
 *
 */
//#@todo this file must be made a common code function

#include "target.h"

#ifdef EHS_TOOLKIT_DEPRECATED
#include "dtv_remote.h"
#include "dtv_remote1.h"
#endif

#include "ehs_fb_types.h"
#include "dtv_remote2.h"
#include "globals.h"
#include "fid.h"
#include "callback_queue.h"
#include "keypress.h"

#ifdef EHS_TOOLKIT_DEPRECATED
EHS_FB_FUNCTIONS_START(DtvRemote)
EHS_FB_FUNCTION_ENTRY("run", 0x00, DtvRemote)
EHS_FB_FUNCTIONS_END

EHS_FB_FUNCTIONS_START(DtvRemote1)
EHS_FB_FUNCTION_ENTRY("run", 0x00, DtvRemote)
EHS_FB_FUNCTIONS_END
#endif


EHS_FB_FUNCTIONS_START(DtvRemote2)
EHS_FB_FUNCTION_ENTRY("run", 0x00, DtvRemote) /* shares the same run function as DtvRemote - SO MOVE IT HERE!*/

EHS_FB_FUNCTION_ENTRY("enable", 0x01, DtvRemote_Enable)

EHS_FB_FUNCTION_ENTRY("disable", 0x02, DtvRemote_Disable)
EHS_FB_FUNCTIONS_END

#define EHS_FB_DTV_REMOTE_RUN -1
/*
 * Indicates the first function instance to callback when a keypress is detected
 *
 * @todo ensure that this gets reset every time SODL is reloaded.
 */
//EhsFunctionInstanceDataType* EhsWindowsKeypressCallback = NULL;

/**
 * Character received from keyboard input
 */
extern EhsDataflowIntType EhsGUIKbHitChar;

typedef struct
{
    EhsCallbackQueueEntryType xCallback;
    ehs_uint16 nVersion;
    ehs_bool bEnabled;			// is counter enabled
} EhsDtvRemoteDataType;

/**
 * Define the identify function. If multiple keypress objects appear in the application,
 * we create a linked list between them.
 */
#ifdef EHS_TOOLKIT_DEPRECATED

EHS_FB_IDENTIFY_FUNCTION(DtvRemote)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsDtvRemoteDataType);
}

EHS_FB_IDENTIFY_FUNCTION(DtvRemote1)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsDtvRemoteDataType);
}
#endif

EHS_FB_IDENTIFY_FUNCTION(DtvRemote2)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsDtvRemoteDataType);
}

/**
 * Load the GUI parameters into the params structure, load
 * the associated bitmap into memory
 */
#ifdef EHS_TOOLKIT_DEPRECATED

EHS_FB_INIT_FUNCTION(DtvRemote)
{
    /*
    EhsCallbackQueueEntryType *pParams = (EhsCallbackQueueEntryType*)EHS_FB_INIT_CONTEXT;
    */
    /* put this item into the keypress callback queue */
    /*
    EhsCallbackQueue_register(&EhsWindowsKeypressCallback,
    	EHS_FB_RUN_NAME(DtvRemote),
    	EHS_FB_INIT_FUNCTION_INSTANCE,
    	pParams);
    */

    EhsDtvRemoteDataType* EhsDtvRemoteData = (EhsDtvRemoteDataType*)EHS_FB_INIT_CONTEXT;

    EhsDtvRemoteData->nVersion = 1; /* only up/down, menu, ok and exit are defined */
    EhsDtvRemoteData->bEnabled = EHS_TRUE;

    /* put this item into the keypress callback queue */
    EhsCallbackQueue_register(&EhsGUIKeypressCallback,
                              EHS_FB_RUN_NAME(DtvRemote),
                              EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(EHS_FB_DTV_REMOTE_RUN),
                              &(EhsDtvRemoteData->xCallback));
    return EHS_TRUE; /* initialise always succeeds */
}
EHS_FB_INIT_FUNCTION(DtvRemote1)
{
    EhsDtvRemoteDataType* EhsDtvRemoteData = (EhsDtvRemoteDataType*)EHS_FB_INIT_CONTEXT;

    EhsDtvRemoteData->nVersion = 2; /* only up/down/left/right, menu, ok and exit are defined */
    EhsDtvRemoteData->bEnabled = EHS_TRUE;

    /* put this item into the keypress callback queue */
    EhsCallbackQueue_register(&EhsGUIKeypressCallback,
                              EHS_FB_RUN_NAME(DtvRemote),
                              EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(EHS_FB_DTV_REMOTE_RUN),
                              &(EhsDtvRemoteData->xCallback));
    return EHS_TRUE; /* initialise always succeeds */
}
#endif

EHS_FB_INIT_FUNCTION(DtvRemote2)
{
    EhsDtvRemoteDataType* EhsDtvRemoteData = (EhsDtvRemoteDataType*)EHS_FB_INIT_CONTEXT;

    EhsDtvRemoteData->nVersion = 3; /* only up/down/left/right, menu, ok, exit, play, pause and stop are defined */
    EhsDtvRemoteData->bEnabled = EHS_FALSE; // start disabled

    /* put this item into the keypress callback queue */
    EhsCallbackQueue_register(&EhsGUIKeypressCallback,
                              EHS_FB_RUN_NAME(DtvRemote),
                              EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(EHS_FB_DTV_REMOTE_RUN),
                              &(EhsDtvRemoteData->xCallback));
    return EHS_TRUE; /* initialise always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(DtvRemote_Enable)
{
    EhsDtvRemoteDataType* EhsDtvRemoteData = (EhsDtvRemoteDataType*)EHS_FB_RUN_CONTEXT;

    EhsDtvRemoteData->bEnabled = EHS_TRUE;
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
EHS_FB_RUN_FUNCTION(DtvRemote_Disable)
{
    EhsDtvRemoteDataType* EhsDtvRemoteData = (EhsDtvRemoteDataType*)EHS_FB_RUN_CONTEXT;

    EhsDtvRemoteData->bEnabled = EHS_FALSE;
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

EHS_FB_RUN_FUNCTION(DtvRemote)
{
    EhsDtvRemoteDataType* EhsDtvRemoteData = (EhsDtvRemoteDataType*)EHS_FB_RUN_CONTEXT;

    if( EhsDtvRemoteData->bEnabled == EHS_FALSE ) return;	// lazy return. Disabled, do nothing

    switch (EhsGUIKbHitChar)
    {
    case 0x020: // menu
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
        break;
    case 0xff0d: // ok
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
        break;
    case 0xff1b: // exit (This is ESCAPE) - don't use to ungrab keyboard
        SetCompletes3((structFuncArg*)&EHS_FB_RUN_CONTEXT);
        break;
    case 0xff52: // up
        SetCompletes4((structFuncArg*)&EHS_FB_RUN_CONTEXT);
        break;
    case 0xff54: // down
        SetCompletes5((structFuncArg*)&EHS_FB_RUN_CONTEXT);
        break;
    case 0xff53: /* right */
        EHS_FB_FINISH(7);
        break;
    case 0xff51: /* left */
        EHS_FB_FINISH(6);
        break;
    case 80: /* P for Play */
    case 112: /* p for Play */
        if (EhsDtvRemoteData->nVersion > 2)
        {
            EHS_FB_FINISH(8);
        }
        break;
    case 65: /* A for Pause */
    case 97: /* a for Pause */
        if (EhsDtvRemoteData->nVersion > 2)
        {
            EHS_FB_FINISH(9);
        }
        break;
    case 83: /* S for Stop */
    case 115: /* s for Stop */
        if (EhsDtvRemoteData->nVersion > 2)
        {
            EHS_FB_FINISH(10);
        }
        break;
    /*
    case 112:
    	EHS_FB_FINISH(8);
    	break;
    	*/
    default:
        break;
    }
    return;
}

