/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-numeric_display_char.h"
#include "inxware_hrx_config.h"
#include "ehs_main.h" // we run th main from here!
#include "inx_display.h"
//ICB HEADER MACRO END -- DO NOT ALTER

#define INX_MQTT_PUBLISH_FIFO_SIZE 1

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_ndc_state
{
} inx_ndc_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(ndc)

EHS_FB_FUNCTION_ENTRY("set", 0x01, ndc_set)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_ndc_ARG_set_char 1
#define INX_ndc_ARG_set_displayno 2
#define INX_ndc_ARG_set_index 3
#define INX_ndc_ARG_set_finishset 1
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
EHS_FB_IDENTIFY_FUNCTION(ndc)
{
    /* Uncomment the following if you need to parse the parameters to calculate memory required */
    /*
    	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_ndc_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(ndc)
{
    ehs_bool bRet = EHS_TRUE; /* assume success */

    //this is the reference to the object data for this instance of the function block
    /*
    inx_ndc_state_type* inx_ndc_state = (inx_ndc_state_type*)EHS_FB_INIT_CONTEXT;
    */
    /* read the initialisation parameters */
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"");

    /* Add any further intialisation code here */
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(ndc)
{
    /*
    inx_ndc_state_type *inx_ndc_state = (inx_ndc_state_type*)EHS_FB_DESTROY_CONTEXT;
    */
    //Your code below here
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION set MACRO START -- DO NOT ALTER
/**
 * Definition of ndc_set.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ndc_set)
{
    /*
    inx_ndc_state_type* inx_ndc_state = (inx_ndc_state_type*)EHS_FB_RUN_CONTEXT;
    */

    // Your code here
    if(EHS_FB_IN_CONNECTED_API2(INX_ndc_ARG_set_char) && EHS_FB_IN_CONNECTED_API2(INX_ndc_ARG_set_displayno) && EHS_FB_IN_CONNECTED_API2(INX_ndc_ARG_set_index))
    {
        inxDisplaySetChar(EHS_FB_IN_I_API2(INX_ndc_ARG_set_displayno)-1,EHS_FB_IN_I_API2(INX_ndc_ARG_set_index)-1,EHS_FB_IN_I_API2(INX_ndc_ARG_set_char));
    }
    /*
    if (EHS_FB_IN_CONNECTED_API2(INX_ndc_ARG_set_char))
    if (EHS_FB_IN_CONNECTED_API2(INX_ndc_ARG_set_displayno))
    	EHS_FB_IN_I_API2(INX_ndc_ARG_set_displayno) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_ndc_ARG_set_index))
    	EHS_FB_IN_I_API2(INX_ndc_ARG_set_index) ;
    */
    EHS_FB_FINISH(INX_ndc_ARG_set_finishset);
}//ICB FUNCTION set MACRO END -- DO NOT ALTER THIS LINE
