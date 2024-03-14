/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-application_run.h"
#include "hal_devapps.h"
#include "hal_string.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER


/* My Component state data structure. - Use this in your code! */
typedef struct
{
} inx_application_run_state_type; //Reference this, maybe store your config parameters in here too.


//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(application_run)

EHS_FB_FUNCTION_ENTRY("runApp", 0x01, application_run_runApp)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_application_run_ARG_runApp_Name 1
#define INX_application_run_ARG_runApp_xxxrun 1
#define INX_application_run_ARG_runApp_errorrun 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(application_run)
{
    /* Uncomment the following if you need to parse the parameters to calculate memory required */
    /*
    	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_application_run_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(application_run)
{
    ehs_bool bRet = EHS_TRUE; /* assume success */
    //this is the reference to the object data for this instance of the function block
    //inx_application_run_state_type* inx_application_run_state = (inx_application_run_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
    //EhsSscanf(EHS_FB_INIT_PARAMETERS,"");
    //todo this should support a fixed paramters qith an application name too.
    /* Add any further intialisation code here */
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB FUNCTION runApp MACRO START -- DO NOT ALTER
/**
 * Definition of application_run_runApp.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(application_run_runApp)
{
    ehs_bool bSucc = EHS_FALSE;
    ehs_char cAppCanonicalName[EHS_MAXPATHLENGTH];
    // not used: inx_application_run_state_type* inx_application_run_state = (inx_application_run_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    if (EHS_FB_IN_CONNECTED_API2(INX_application_run_ARG_runApp_Name))
    {
        EhsStrcpy(cAppCanonicalName,EHS_FB_IN_S_API2(INX_application_run_ARG_runApp_Name));

        EhsStrTrimLR(cAppCanonicalName);

        //@todo - test for path in appdata before try to load app
        if (EhsStrcmp(cAppCanonicalName,"") != 0)
        {
            EhsHMetaSetNextAppToRun(cAppCanonicalName);
            EhsHFSMForceInternallyRequestedCommand(EHS_RELOAD_EHS_FROM_FILE); //@todo - any way to check for success?
            //		EHSH_LOG_ERROR("Failed to start app, canonical name[%s]\n",cAppCanonicalName);
            bSucc = EHS_TRUE;
        }
    }

    if (bSucc == EHS_TRUE)
    {
        EHS_FB_FINISH(INX_application_run_ARG_runApp_xxxrun);
    }
    else
    {
        EHS_FB_FINISH(INX_application_run_ARG_runApp_errorrun);
    }
    return;
}
//ICB FUNCTION runApp MACRO END -- DO NOT ALTER
