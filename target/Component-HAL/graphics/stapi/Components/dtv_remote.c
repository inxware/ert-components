/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file dtv_remote.c
 *
 * Implementation of the remote control function in the DTV toolkit.
 * Includes target-specific code.
 *
 *
 * @author: inx limited
 */

#include "target.h"

#include "dtv_remote.h"
#include "dtv_remote1.h"
#include "dtv_remote2.h"
#include "globals.h"
#include "fid.h"
//#include "keypress.h"

EHS_FB_FUNCTIONS_START(DtvRemote)
EHS_FB_FUNCTION_ENTRY("run", 0x01, DtvRemote)
EHS_FB_FUNCTIONS_END

EHS_FB_FUNCTIONS_START(DtvRemote1)
EHS_FB_FUNCTION_ENTRY("run", 0x01, DtvRemote)
EHS_FB_FUNCTIONS_END

EHS_FB_FUNCTIONS_START(DtvRemote2)
EHS_FB_FUNCTION_ENTRY("run", 0x01, DtvRemote) /* shares the same run function as DtvRemote */
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
extern EhsDataflowIntType EhsWindowsKbHitChar;

/**
 * Define the identify function. If multiple keypress objects appear in the application,
 * we create a linked list between them.
 */
EHS_FB_IDENTIFY_FUNCTION(DtvRemote)
{
    /*
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsCallbackQueueEntryType);
    */
}
EHS_FB_IDENTIFY_FUNCTION(DtvRemote1)
{
    /*
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsCallbackQueueEntryType);
    */
}

EHS_FB_IDENTIFY_FUNCTION(DtvRemote2)
{
    /*
     *
     EHS_FB_IDENTIFY_MEMORY = sizeof(EhsDtvRemoteDataType);
     */
}


/**
 * Load the GUI parameters into the params structure, load
 * the associated bitmap into memory
 */
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
}
EHS_FB_INIT_FUNCTION(DtvRemote1)
{
}


EHS_FB_INIT_FUNCTION(DtvRemote2)
{
    /*
     *
     *	EhsDtvRemoteDataType* EhsDtvRemoteData = (EhsDtvRemoteDataType*)EHS_FB_INIT_CONTEXT;

    		EhsDtvRemoteData->nVersion = 3; /* only up/down/left/right, menu, ok, exit, play, pause and stop are defined * /

    		/ * put this item into the keypress callback queue * /
    		EhsCallbackQueue_register(&EhsGtkKeypressCallback,
    			EHS_FB_RUN_NAME(DtvRemote),
    			EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(EHS_FB_DTV_REMOTE_RUN),
    			&(EhsDtvRemoteData->xCallback));
    		*/
    return EHS_TRUE; /* initialise always succeeds */

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
    /*
    switch (EhsWindowsKbHitChar)
    {
    case ' ': // menu
    	SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    	break;
    case 0x0d: // ok
    	SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    	break;
    case 0x1b: // exit
    	SetCompletes3((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    	break;
    case 38: // up
    	SetCompletes4((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    	break;
    case 40: // down
    	SetCompletes5((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    	break;
    default:
    	break;
    }
    return;
    */
}

