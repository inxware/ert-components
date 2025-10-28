//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-indexed_mux_real.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_indexed_mux_real_state
{
    ehs_float in1;
    ehs_float in2;
    ehs_float in3;
    ehs_float in4;
    ehs_float in5;
    ehs_float in6;
    ehs_float in7;
    ehs_float in8;
} inx_indexed_mux_real_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(indexed_mux_real)
EHS_FB_FUNCTION_ENTRY("mux", 0x01, indexed_mux_real_mux)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_indexed_mux_real_ARG_mux_index 1
#define INX_indexed_mux_real_ARG_mux_in1 2
#define INX_indexed_mux_real_ARG_mux_in2 3
#define INX_indexed_mux_real_ARG_mux_in3 4
#define INX_indexed_mux_real_ARG_mux_in4 5
#define INX_indexed_mux_real_ARG_mux_in5 6
#define INX_indexed_mux_real_ARG_mux_in6 7
#define INX_indexed_mux_real_ARG_mux_in7 8
#define INX_indexed_mux_real_ARG_mux_in8 9
#define INX_indexed_mux_real_ARG_mux_output 1
#define INX_indexed_mux_real_ARG_mux_overflow 2
#define INX_indexed_mux_real_ARG_mux_event_out 1
#define INX_indexed_mux_real_ARG_mux_event_overflow 2
#define INX_indexed_mux_real_ARG_mux_err 3
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_indexed_mux_real_in1 0
#define INX_FB_indexed_mux_real_in2 0
#define INX_FB_indexed_mux_real_in3 0
#define INX_FB_indexed_mux_real_in4 0
#define INX_FB_indexed_mux_real_in5 0
#define INX_FB_indexed_mux_real_in6 0
#define INX_FB_indexed_mux_real_in7 0
#define INX_FB_indexed_mux_real_in8 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(indexed_mux_real)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_indexed_mux_real_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(indexed_mux_real)
{
    ehs_bool bRet = EHS_TRUE; /* assume success */
    //this is the reference to the object data for this instance of the function block
    inx_indexed_mux_real_state_type* inx_indexed_mux_real_state = (inx_indexed_mux_real_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
    EhsSscanf(EHS_FB_INIT_PARAMETERS,EHS_FL_FMT" "EHS_FL_FMT" "EHS_FL_FMT" "EHS_FL_FMT" "EHS_FL_FMT" "EHS_FL_FMT" "EHS_FL_FMT" "EHS_FL_FMT,
                &(inx_indexed_mux_real_state->in1),
                &(inx_indexed_mux_real_state->in2),
                &(inx_indexed_mux_real_state->in3),
                &(inx_indexed_mux_real_state->in4),
                &(inx_indexed_mux_real_state->in5),
                &(inx_indexed_mux_real_state->in6),
                &(inx_indexed_mux_real_state->in7),
                &(inx_indexed_mux_real_state->in8));

    /* Add any further intialisation code here */
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(indexed_mux_real)
{
    inx_indexed_mux_real_state_type *inx_indexed_mux_real_state = (inx_indexed_mux_real_state_type*)EHS_FB_DESTROY_CONTEXT;
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION mux MACRO START -- DO NOT ALTER
/**
 * Definition of indexed_mux_real_mux.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(indexed_mux_real_mux)
{
    if (!EHS_FB_IN_CONNECTED_API2(INX_indexed_mux_real_ARG_mux_index)) return;
    if (EHS_FB_IN_I_API2(INX_indexed_mux_real_ARG_mux_index) < 1){
        EHS_FB_FINISH(INX_indexed_mux_real_ARG_mux_err);
        return;
    }
    if (EHS_FB_IN_I_API2(INX_indexed_mux_real_ARG_mux_index) > 8){
        if (EHS_FB_OUT_CONNECTED_API2(INX_indexed_mux_real_ARG_mux_overflow))
            EHS_FB_OUT_I_API2(INX_indexed_mux_real_ARG_mux_overflow) = EHS_FB_IN_I_API2(INX_indexed_mux_real_ARG_mux_index) - 8;
        EHS_FB_FINISH(INX_indexed_mux_real_ARG_mux_event_overflow);
        return;
    }
    inx_indexed_mux_real_state_type* inx_indexed_mux_real_state = (inx_indexed_mux_real_state_type*)EHS_FB_RUN_CONTEXT;
    EhsDataflowFloatType session[8] = {
        inx_indexed_mux_real_state->in1,
        inx_indexed_mux_real_state->in2,
        inx_indexed_mux_real_state->in3,
        inx_indexed_mux_real_state->in4,
        inx_indexed_mux_real_state->in5,
        inx_indexed_mux_real_state->in6,
        inx_indexed_mux_real_state->in7,
        inx_indexed_mux_real_state->in8,
    };
    
    // Your code here
    if (EHS_FB_IN_CONNECTED_API2(INX_indexed_mux_real_ARG_mux_in1))
        session[0] = EHS_FB_IN_F_API2(INX_indexed_mux_real_ARG_mux_in1) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_indexed_mux_real_ARG_mux_in2))
        session[1] = EHS_FB_IN_F_API2(INX_indexed_mux_real_ARG_mux_in2) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_indexed_mux_real_ARG_mux_in3))
        session[2] = EHS_FB_IN_F_API2(INX_indexed_mux_real_ARG_mux_in3) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_indexed_mux_real_ARG_mux_in4))
        session[3] = EHS_FB_IN_F_API2(INX_indexed_mux_real_ARG_mux_in4) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_indexed_mux_real_ARG_mux_in5))
        session[4] = EHS_FB_IN_F_API2(INX_indexed_mux_real_ARG_mux_in5) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_indexed_mux_real_ARG_mux_in6))
        session[5] = EHS_FB_IN_F_API2(INX_indexed_mux_real_ARG_mux_in6) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_indexed_mux_real_ARG_mux_in7))
        session[6] = EHS_FB_IN_F_API2(INX_indexed_mux_real_ARG_mux_in7) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_indexed_mux_real_ARG_mux_in8))
        session[7] = EHS_FB_IN_F_API2(INX_indexed_mux_real_ARG_mux_in8) ;
    if (EHS_FB_OUT_CONNECTED_API2(INX_indexed_mux_real_ARG_mux_output))
        EHS_FB_OUT_F_API2(INX_indexed_mux_real_ARG_mux_output) = session[EHS_FB_IN_I_API2(INX_indexed_mux_real_ARG_mux_index) - 1];
    EHS_FB_FINISH(INX_indexed_mux_real_ARG_mux_event_out);
}//ICB FUNCTION mux MACRO END -- DO NOT ALTER THIS LINE
