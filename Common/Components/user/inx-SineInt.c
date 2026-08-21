//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-SineInt.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_SineInt_state
{
	ehs_bool unit;
} inx_SineInt_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
#define INX_SINE_PI_1000 3142
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(SineInt)
EHS_FB_FUNCTION_ENTRY("calc", 0x01, SineInt_calc)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_SineInt_ARG_calc_input 1
#define INX_SineInt_ARG_calc_output 1
#define INX_SineInt_ARG_calc_done 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_SineInt_unit 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(SineInt)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_bool unit;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d",&unit); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_SineInt_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(SineInt)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_SineInt_state_type* inx_SineInt_state = (inx_SineInt_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"%hhu",&inx_SineInt_state->unit); /* unit is ehs_bool */

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(SineInt)
{
	inx_SineInt_state_type *inx_SineInt_state = (inx_SineInt_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
	return EHS_TRUE; /* destroy functions return ehs_bool */
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION calc MACRO START -- DO NOT ALTER
/**
 * Definition of SineInt_calc.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(SineInt_calc)
{
	inx_SineInt_state_type* inx_SineInt_state = (inx_SineInt_state_type*)EHS_FB_RUN_CONTEXT;
	EhsDataflowIntType input = 0;

	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_SineInt_ARG_calc_input))
		input = EHS_FB_IN_I_API2(INX_SineInt_ARG_calc_input) ;
	if (inx_SineInt_state->unit)
		input = (input * 180) / INX_SINE_PI_1000;
	if (EHS_FB_OUT_CONNECTED_API2(INX_SineInt_ARG_calc_output))
		EHS_FB_OUT_I_API2(INX_SineInt_ARG_calc_output) = EhsTgtInt_sin(1000, input);
	EHS_FB_FINISH(INX_SineInt_ARG_calc_done);
}//ICB FUNCTION calc MACRO END -- DO NOT ALTER THIS LINE
