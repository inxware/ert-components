//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-indexed_demux_str.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_indexed_demux_str_state
{
    ehs_char default_v[EHS_STRING_LENGTH_MAX];
    ehs_bool init_failed;
} inx_indexed_demux_str_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(indexed_demux_str)
EHS_FB_FUNCTION_ENTRY("demux", 0x01, indexed_demux_str_demux)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_indexed_demux_str_ARG_demux_in 1
#define INX_indexed_demux_str_ARG_demux_index 2
#define INX_indexed_demux_str_ARG_demux_out1 1
#define INX_indexed_demux_str_ARG_demux_out2 2
#define INX_indexed_demux_str_ARG_demux_out3 3
#define INX_indexed_demux_str_ARG_demux_out4 4
#define INX_indexed_demux_str_ARG_demux_out5 5
#define INX_indexed_demux_str_ARG_demux_out6 6
#define INX_indexed_demux_str_ARG_demux_out7 7
#define INX_indexed_demux_str_ARG_demux_out8 8
#define INX_indexed_demux_str_ARG_demux_offset 9
#define INX_indexed_demux_str_ARG_demux_passthrough 10
#define INX_indexed_demux_str_ARG_demux_err 1
#define INX_indexed_demux_str_ARG_demux_evt1 2
#define INX_indexed_demux_str_ARG_demux_evt2 3
#define INX_indexed_demux_str_ARG_demux_evt3 4
#define INX_indexed_demux_str_ARG_demux_evt4 5
#define INX_indexed_demux_str_ARG_demux_evt5 6
#define INX_indexed_demux_str_ARG_demux_evt6 7
#define INX_indexed_demux_str_ARG_demux_evt7 8
#define INX_indexed_demux_str_ARG_demux_evt8 9
#define INX_indexed_demux_str_ARG_demux_ovf 10
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_indexed_demux_str_default ""
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(indexed_demux_str)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
    ehs_char* default;
    EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%s",&default); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_indexed_demux_str_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(indexed_demux_str)
{
    ehs_bool bRet = EHS_TRUE; /* assume success */
    //this is the reference to the object data for this instance of the function block
    inx_indexed_demux_str_state_type* inx_indexed_demux_str_state = (inx_indexed_demux_str_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
    // inx_indexed_demux_str_state->default_v = (ehs_char *) EhsHMem_writeableAlloc(EhsStrlen(EHS_FB_INIT_PARAMETERS) + 1);
    inx_indexed_demux_str_state->init_failed = EHS_FALSE;
    if (EhsStrlen(EHS_FB_INIT_PARAMETERS) + 1 > EHS_STRING_LENGTH_MAX)
    {
        inx_indexed_demux_str_state->init_failed = EHS_TRUE;
        return bRet;
    }
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%s",&inx_indexed_demux_str_state->default_v);

    /* Add any further intialisation code here */
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(indexed_demux_str)
{
    inx_indexed_demux_str_state_type *inx_indexed_demux_str_state = (inx_indexed_demux_str_state_type*)EHS_FB_DESTROY_CONTEXT;
    //Your code below here
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION demux MACRO START -- DO NOT ALTER
/**
 * Definition of indexed_demux_str_demux.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(indexed_demux_str_demux)
{
    inx_indexed_demux_str_state_type* inx_indexed_demux_str_state = (inx_indexed_demux_str_state_type*)EHS_FB_RUN_CONTEXT;
    
    ehs_char *input = inx_indexed_demux_str_state->default_v;
    ehs_sint32 index = 0;

    // Your code here
    if (EHS_FB_IN_CONNECTED_API2(INX_indexed_demux_str_ARG_demux_in))
        input = EHS_FB_IN_S_API2(INX_indexed_demux_str_ARG_demux_in) ;
    else
    {
        // The initialise parameter string length is greater than this platform could accept
        //  Error if no input presents
        if (inx_indexed_demux_str_state->init_failed)
        {
            EHS_FB_FINISH(INX_indexed_demux_str_ARG_demux_err);
            return;
        }
    }

    if (EHS_FB_IN_CONNECTED_API2(INX_indexed_demux_str_ARG_demux_index))
        index = EHS_FB_IN_I_API2(INX_indexed_demux_str_ARG_demux_index) ;
    else
    {
        EHS_FB_FINISH(INX_indexed_demux_str_ARG_demux_err);
        return;
    }

    if (index <= 0)
    {
        EHS_FB_FINISH(INX_indexed_demux_str_ARG_demux_err);
        return;
    }

    if (index > 8)
    {
        if (EHS_FB_OUT_CONNECTED_API2(INX_indexed_demux_str_ARG_demux_offset))
            EHS_FB_OUT_I_API2(INX_indexed_demux_str_ARG_demux_offset) = index - 8;
        if (EHS_FB_OUT_CONNECTED_API2(INX_indexed_demux_str_ARG_demux_passthrough))
            EhsStrcpy(EHS_FB_OUT_S_API2(INX_indexed_demux_str_ARG_demux_passthrough), input);
        EHS_FB_FINISH(INX_indexed_demux_str_ARG_demux_ovf);
        return;
    }

    switch (index)
    {
        case 1:
        {
            if (EHS_FB_OUT_CONNECTED_API2(INX_indexed_demux_str_ARG_demux_out1))
                EhsStrcpy(EHS_FB_OUT_S_API2(INX_indexed_demux_str_ARG_demux_out1), input);
            EHS_FB_FINISH(INX_indexed_demux_str_ARG_demux_evt1);
            break;
        }
        case 2:
        {
            if (EHS_FB_OUT_CONNECTED_API2(INX_indexed_demux_str_ARG_demux_out2))
                EhsStrcpy(EHS_FB_OUT_S_API2(INX_indexed_demux_str_ARG_demux_out2), input);
            EHS_FB_FINISH(INX_indexed_demux_str_ARG_demux_evt2);
            break;
        }
        case 3:
        {
            if (EHS_FB_OUT_CONNECTED_API2(INX_indexed_demux_str_ARG_demux_out3))
                EhsStrcpy(EHS_FB_OUT_S_API2(INX_indexed_demux_str_ARG_demux_out3), input);
            EHS_FB_FINISH(INX_indexed_demux_str_ARG_demux_evt3);
            break;
        }
        case 4:
        {
            if (EHS_FB_OUT_CONNECTED_API2(INX_indexed_demux_str_ARG_demux_out4))
                EhsStrcpy(EHS_FB_OUT_S_API2(INX_indexed_demux_str_ARG_demux_out4), input);
            EHS_FB_FINISH(INX_indexed_demux_str_ARG_demux_evt4);
            break;
        }
        case 5:
        {
            if (EHS_FB_OUT_CONNECTED_API2(INX_indexed_demux_str_ARG_demux_out5))
                EhsStrcpy(EHS_FB_OUT_S_API2(INX_indexed_demux_str_ARG_demux_out5), input);
            EHS_FB_FINISH(INX_indexed_demux_str_ARG_demux_evt5);
            break;
        }
        case 6:
        {
            if (EHS_FB_OUT_CONNECTED_API2(INX_indexed_demux_str_ARG_demux_out6))
                EhsStrcpy(EHS_FB_OUT_S_API2(INX_indexed_demux_str_ARG_demux_out6), input);
            EHS_FB_FINISH(INX_indexed_demux_str_ARG_demux_evt6);
            break;
        }
        case 7:
        {
            if (EHS_FB_OUT_CONNECTED_API2(INX_indexed_demux_str_ARG_demux_out7))
                EhsStrcpy(EHS_FB_OUT_S_API2(INX_indexed_demux_str_ARG_demux_out7), input);
            EHS_FB_FINISH(INX_indexed_demux_str_ARG_demux_evt7);
            break;
        }
        case 8:
        {
            if (EHS_FB_OUT_CONNECTED_API2(INX_indexed_demux_str_ARG_demux_out8))
                EhsStrcpy(EHS_FB_OUT_S_API2(INX_indexed_demux_str_ARG_demux_out8), input);
            EHS_FB_FINISH(INX_indexed_demux_str_ARG_demux_evt8);
            break;
        }
		default:
		{
			EHS_FB_FINISH(INX_indexed_demux_str_ARG_demux_err);
			return;
			break;
		}
    }
}//ICB FUNCTION demux MACRO END -- DO NOT ALTER THIS LINE
