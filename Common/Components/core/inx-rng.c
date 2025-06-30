//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-rng.h"
//ICB HEADER MACRO END -- DO NOT ALTER

#include <stdlib.h>

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_rng_state
{
	ehs_sint32 seed;
	ehs_sint32 min;
	ehs_sint32 max;
} inx_rng_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(rng)
EHS_FB_FUNCTION_ENTRY("run", 0x01, rng_run)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_rng_ARG_run_seed 1
#define INX_rng_ARG_run_num 1
#define INX_rng_ARG_run_gen_ok 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_rng_seed 
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(rng)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_sint32 seed;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d",&seed); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_rng_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(rng)
{
	ehs_sint32 seed;
	ehs_sint32 min;
	ehs_sint32 max;
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_rng_state_type* inx_rng_state = (inx_rng_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d %d %d", &seed, &min, &max);
	inx_rng_state->seed = seed;
	srand(inx_rng_state->seed);
	if (min > max)
	{
		inx_rng_state->min = max;
		inx_rng_state->max = min;
	}
	else
	{
		inx_rng_state->min = min;
		inx_rng_state->max = max;
	}
	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(rng)
{
	//inx_rng_state_type *inx_rng_state = (inx_rng_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION run MACRO START -- DO NOT ALTER
/**
 * Definition of rng_run.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(rng_run)
{
	inx_rng_state_type* inx_rng_state = (inx_rng_state_type*)EHS_FB_RUN_CONTEXT;
	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_rng_ARG_run_seed)){
		if (EHS_FB_IN_I_API2(INX_rng_ARG_run_seed) != inx_rng_state->seed) srand(inx_rng_state->seed); // only reset the seed if the seed value changes not every time as we just get the same number otherwise
		inx_rng_state->seed = EHS_FB_IN_I_API2(INX_rng_ARG_run_seed);
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_rng_ARG_run_num)){
		//ehs_sint32 num = (ehs_sint32) (rand()- RAND_MAX/2); /* Crete zero mean uniform sign 32 bit valuesvalues */
		ehs_sint32 num = ((ehs_sint32)rand()) % (inx_rng_state->max + 1 - inx_rng_state->min) + inx_rng_state->min; /* Generate value bounded by the min/max */
		EHS_FB_OUT_I_API2(INX_rng_ARG_run_num) = num;
		/* todo2023 We should output the value RAND_MAX also on another port as this will vary between platforms, but we only use 32bit ints for now... */
	}
	EHS_FB_FINISH(INX_rng_ARG_run_gen_ok);
}//ICB FUNCTION run MACRO END -- DO NOT ALTER THIS LINE
