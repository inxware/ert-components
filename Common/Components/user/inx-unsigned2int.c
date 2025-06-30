//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-unsigned2int.h"
#include <limits.h>
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_Unsigned2Int_state
{
	ehs_sint32 bit_width;
} inx_Unsigned2Int_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(Unsigned2Int)
EHS_FB_FUNCTION_ENTRY("convert", 0x01, Unsigned2Int_convert)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_Unsigned2Int_ARG_convert_value_in 1
#define INX_Unsigned2Int_ARG_convert_value_out 1
#define INX_Unsigned2Int_ARG_convert_done 1
#define INX_Unsigned2Int_ARG_convert_error 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_Unsigned2Int_bit_width 8
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(Unsigned2Int)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_sint32 bit_width;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d",&bit_width); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_Unsigned2Int_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(Unsigned2Int)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_Unsigned2Int_state_type* inx_Unsigned2Int_state = (inx_Unsigned2Int_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d",&inx_Unsigned2Int_state->bit_width);

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(Unsigned2Int)
{
	inx_Unsigned2Int_state_type *inx_Unsigned2Int_state = (inx_Unsigned2Int_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION convert MACRO START -- DO NOT ALTER
/**
 * Definition of Unsigned2Int_convert.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(Unsigned2Int_convert)
{
	inx_Unsigned2Int_state_type* inx_Unsigned2Int_state = (inx_Unsigned2Int_state_type*)EHS_FB_RUN_CONTEXT;
	ehs_sint32 output_bit_width = sizeof(EhsDataflowIntType) * CHAR_BIT;
	// Error if the bit width is greater than the supported one
	if (inx_Unsigned2Int_state->bit_width > output_bit_width)
	{
		EHS_FB_FINISH(INX_Unsigned2Int_ARG_convert_error);
		return;
	}
	EhsDataflowIntType input = 0;
	EhsDataflowIntType input_temp = 0;
	EhsDataflowIntType output = 0;
	EhsDataflowIntType bit_width = inx_Unsigned2Int_state->bit_width;
	EhsDataflowIntType m = 1U << (bit_width - 1);
	ehs_sint32 input_bit_width = 1;

	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_Unsigned2Int_ARG_convert_value_in))
		input = EHS_FB_IN_I_API2(INX_Unsigned2Int_ARG_convert_value_in) ;
	// Check the input bit width is actually as defined
	input_temp = input;
	while (input_temp >>= 1)
	{
		if (++input_bit_width > inx_Unsigned2Int_state->bit_width) break;
	}
	if (input_bit_width > inx_Unsigned2Int_state->bit_width)
	{
		input &= ((1 << inx_Unsigned2Int_state->bit_width) - 1);
	}
	// If the input bit width is the same as the output one, just redirect the input to the output
	if (output_bit_width == inx_Unsigned2Int_state->bit_width)
	{
		if (EHS_FB_OUT_CONNECTED_API2(INX_Unsigned2Int_ARG_convert_value_out))
			EHS_FB_OUT_I_API2(INX_Unsigned2Int_ARG_convert_value_out) = input;
		EHS_FB_FINISH(INX_Unsigned2Int_ARG_convert_done);
		// Emit error to indicate that there is an overflow
		if (input_bit_width > inx_Unsigned2Int_state->bit_width)
			EHS_FB_FINISH(INX_Unsigned2Int_ARG_convert_error);
		return;
	}
	input = input & ((1U << bit_width) - 1);
	output = (input ^ m) - m;
	if (EHS_FB_OUT_CONNECTED_API2(INX_Unsigned2Int_ARG_convert_value_out))
		EHS_FB_OUT_I_API2(INX_Unsigned2Int_ARG_convert_value_out) = output;
	EHS_FB_FINISH(INX_Unsigned2Int_ARG_convert_done);
	// Emit error to indicate that there is an overflow
	if (input_bit_width > inx_Unsigned2Int_state->bit_width)
		EHS_FB_FINISH(INX_Unsigned2Int_ARG_convert_error);
}//ICB FUNCTION convert MACRO END -- DO NOT ALTER THIS LINE
