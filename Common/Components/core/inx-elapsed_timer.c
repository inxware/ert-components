//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-elapsed_timer.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_elapsed_timer_state
{
	EhsTickType start;
} inx_elapsed_timer_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(elapsed_timer)
EHS_FB_FUNCTION_ENTRY("count_elasped", 0x01, elapsed_timer_count_elasped)
EHS_FB_FUNCTION_ENTRY("stop_elapsed", 0x02, elapsed_timer_stop_elapsed)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_elapsed_timer_ARG_count_elasped_started 1
#define INX_elapsed_timer_ARG_stop_elapsed_elasped 1
#define INX_elapsed_timer_ARG_stop_elapsed_stopped 1
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
EHS_FB_IDENTIFY_FUNCTION(elapsed_timer)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_elapsed_timer_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(elapsed_timer)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	// inx_elapsed_timer_state_type* inx_elapsed_timer_state = (inx_elapsed_timer_state_type*)EHS_FB_INIT_CONTEXT;
	// /* read the initialisation parameters */
	// EhsSscanf(EHS_FB_INIT_PARAMETERS,"");

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(elapsed_timer)
{
	inx_elapsed_timer_state_type *inx_elapsed_timer_state = (inx_elapsed_timer_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION count_elasped MACRO START -- DO NOT ALTER
/**
 * Definition of elapsed_timer_count_elasped.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(elapsed_timer_count_elasped)
{
	inx_elapsed_timer_state_type* inx_elapsed_timer_state = (inx_elapsed_timer_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	inx_elapsed_timer_state->start = EHS_CURRENT_TIME;
	EHS_FB_FINISH(INX_elapsed_timer_ARG_count_elasped_started);
}//ICB FUNCTION count_elasped MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION stop_elapsed MACRO START -- DO NOT ALTER
/**
 * Definition of elapsed_timer_stop_elapsed.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(elapsed_timer_stop_elapsed)
{
	inx_elapsed_timer_state_type* inx_elapsed_timer_state = (inx_elapsed_timer_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	EhsTickType duration = EHS_CURRENT_TIME - inx_elapsed_timer_state->start;
	if (EHS_FB_OUT_CONNECTED_API2(INX_elapsed_timer_ARG_stop_elapsed_elasped))
		EHS_FB_OUT_I_API2(INX_elapsed_timer_ARG_stop_elapsed_elasped) = (EhsDataflowIntType) EhsTgtTimer_tickTous(duration);
	EHS_FB_FINISH(INX_elapsed_timer_ARG_stop_elapsed_stopped);
}//ICB FUNCTION stop_elapsed MACRO END -- DO NOT ALTER THIS LINE
