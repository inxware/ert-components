/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/* system_exec.c
 *
 * Source for the system exec function block. This function block runs the
 * executable specified by the parameter with the arguments specified by
 * the input string.
 * @todo Move target-specific code into the target hierarchy.
 *
 * @author: inx limited
 *
 *
 *
*/


#include "globals.h"
#include "ehs_fb_types.h"
#include "system_exec.h"
#include "setCompletes.h"
#include "app_data.h"

#include "hal-api.h" /* Required for logging */

#include <stdlib.h>

/**
 * @page FunctionReference Lucid Function Block Reference Guide
 * @section System
 * @anchor Exec
 * @subsection appliesTo Applies To:
 *
 * @subsection properties Properties:
 * <b>Exexutable.</b> This is the executable filename we want to run
 *
 * @subsection description Description:
 * This function block runs the executable specified by the parameter
 * with the arguments specified by the input string.
 *
 */

/******************************************************************************/
/* Define Exec function block */

EHS_FB_FUNCTIONS_START(Exec)
EHS_FB_FUNCTION_ENTRY("run", 0x01, Exec)
EHS_FB_FUNCTIONS_END


/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(Exec)
{
    /* we need space for the length of the string plus one NULL...*/
    EHS_FB_IDENTIFY_MEMORY = (ehs_uint32)EhsStrlen(EHS_FB_IDENTIFY_PARAMETERS) + 1;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(Exec)
{
    EHS_FB_INIT_CONTEXT = EhsStrcpy((char *)EHS_FB_INIT_CONTEXT, EHS_FB_INIT_PARAMETERS);
    /* Need to make sure that bin is in search path to allow shell exec fuctions to call functions in the canonical bin directory */
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Contents of the thread handling the exec call
 */
EHS_FB_THREAD_FUNCTION(Exec)
{
    char szData1[EHS_STRING_LENGTH_MAX*2] = {'\0'};//TODO:STRINGLENGTH!
    char szData2[EHS_STRING_LENGTH_MAX]= {'\0'};//TODO:STRINGLENGTH!
    char szStdout[EHS_STRING_LENGTH_MAX]= {'\0'};//TODO:STRINGLENGTH!

    Ehs_FB_ThreadStarted(); //shouldn't this be in the calling func!!
    EhsStrcpy(szData2,EHS_FB_IN_S(0));

    /* Get the whole command string into szData1 */
    if (EHS_FB_IN_CONNECTED(0))
    {
        EhsSprintf(szData1,"export LD_LIBRARY_PATH=; %s %s",(char*)EHS_FB_RUN_CONTEXT,szData2);
    }
    else
    {
        EhsSprintf(szData1,"export LD_LIBRARY_PATH=; %s",(char*)EHS_FB_RUN_CONTEXT);
    }
    //EhsTP_shellExecute(szData1); //this has a hardwired use if /bin/sh ..
    if (EhsTP_shellExecuteStdout(szStdout,szData1,EHS_STRING_LENGTH_MAX) == EHS_TRUE)
    {
        EhsTPMutex_lock(EhsTPMutex_fbIO);
        if (EHS_FB_OUT_CONNECTED(0))
        {
            EhsStrcpy(EHS_FB_OUT_S(0), szStdout);
        }

        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
        EhsTPMutex_unlock(EhsTPMutex_fbIO);
    }
    else   // signal an error
    {
        EhsTPMutex_lock(EhsTPMutex_fbIO);
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
        EhsTPMutex_unlock(EhsTPMutex_fbIO);
    }
    Ehs_FB_ThreadComplete();
    EhsHThread_exit();
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(Exec)
{
    EHSH_LOG_ERROR(" DELETEME: RUNNING_EXEC");
    EHS_FB_START_THREAD(Exec,-100); // as a default for legacy calls we will execute at -100
}
