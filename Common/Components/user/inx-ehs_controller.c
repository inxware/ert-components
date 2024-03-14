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
#include "inx-ehs_controller.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct
{
} inx_ehs_controller_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(ehs_controller)

EHS_FB_FUNCTION_ENTRY("exit_ehs", 0x01, ehs_controller_exit_ehs)

EHS_FB_FUNCTION_ENTRY("move_to_foreground", 0x02, ehs_controller_move_to_foreground)

EHS_FB_FUNCTION_ENTRY("move_to_background", 0x03, ehs_controller_move_to_background)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_ehs_controller_ARG_exit_ehs_errorexit 1
#define INX_ehs_controller_ARG_move_to_foreground_errorfore 1
#define INX_ehs_controller_ARG_move_to_foreground_xxxxxxfore 2
#define INX_ehs_controller_ARG_move_to_background_xxxxxxback 1
#define INX_ehs_controller_ARG_move_to_background_errorback 2
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
EHS_FB_IDENTIFY_FUNCTION(ehs_controller)
{
    /* Uncomment the following if you need to parse the parameters to calculate memory required */
    /*
    	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_ehs_controller_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(ehs_controller)
{
    ehs_bool bRet = EHS_TRUE; /* assume success */
    //this is the reference to the object data for this instance of the function block
    //inx_ehs_controller_state_type* inx_ehs_controller_state = (inx_ehs_controller_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
    //EhsSscanf(EHS_FB_INIT_PARAMETERS,"");

    /* Add any further intialisation code here */
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB FUNCTION exit_ehs MACRO START -- DO NOT ALTER
/**
 * Definition of ehs_controller_exit_ehs.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ehs_controller_exit_ehs)
{
    //inx_ehs_controller_state_type* inx_ehs_controller_state = (inx_ehs_controller_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    EhsExit(0);
    EHS_FB_FINISH(INX_ehs_controller_ARG_exit_ehs_errorexit);
    return;
}
//ICB FUNCTION exit_ehs MACRO END -- DO NOT ALTER
//ICB FUNCTION move_to_foreground MACRO START -- DO NOT ALTER
/**
 * Definition of ehs_controller_move_to_foreground.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ehs_controller_move_to_foreground)
{
    //inx_ehs_controller_state_type* inx_ehs_controller_state = (inx_ehs_controller_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    EHS_FB_FINISH(INX_ehs_controller_ARG_move_to_foreground_errorfore);
    EHS_FB_FINISH(INX_ehs_controller_ARG_move_to_foreground_xxxxxxfore);
    return;
}
//ICB FUNCTION move_to_foreground MACRO END -- DO NOT ALTER
//ICB FUNCTION move_to_background MACRO START -- DO NOT ALTER
/**
 * Definition of ehs_controller_move_to_background.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ehs_controller_move_to_background)
{
    //inx_ehs_controller_state_type* inx_ehs_controller_state = (inx_ehs_controller_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    EHS_FB_FINISH(INX_ehs_controller_ARG_move_to_background_xxxxxxback);
    EHS_FB_FINISH(INX_ehs_controller_ARG_move_to_background_errorback);
    return;
}
//ICB FUNCTION move_to_background MACRO END -- DO NOT ALTER
