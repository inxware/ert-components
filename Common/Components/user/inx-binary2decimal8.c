//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-binary2decimal8.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_bin2dec8_state
{
} inx_bin2dec8_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(bin2dec8)
EHS_FB_FUNCTION_ENTRY("convert", 0x01, bin2dec8_convert)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_bin2dec8_ARG_convert_in1 1
#define INX_bin2dec8_ARG_convert_in2 2
#define INX_bin2dec8_ARG_convert_in3 3
#define INX_bin2dec8_ARG_convert_in4 4
#define INX_bin2dec8_ARG_convert_in5 5
#define INX_bin2dec8_ARG_convert_in6 6
#define INX_bin2dec8_ARG_convert_in7 7
#define INX_bin2dec8_ARG_convert_in0 8
#define INX_bin2dec8_ARG_convert_out 1
#define INX_bin2dec8_ARG_convert_event_out 1
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
EHS_FB_IDENTIFY_FUNCTION(bin2dec8)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_bin2dec8_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(bin2dec8)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_bin2dec8_state_type* inx_bin2dec8_state = (inx_bin2dec8_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"");

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(bin2dec8)
{
	inx_bin2dec8_state_type *inx_bin2dec8_state = (inx_bin2dec8_state_type*)EHS_FB_DESTROY_CONTEXT;
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION convert MACRO START -- DO NOT ALTER
/**
 * Definition of bin2dec8_convert.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(bin2dec8_convert)
{
	inx_bin2dec8_state_type* inx_bin2dec8_state = (inx_bin2dec8_state_type*)EHS_FB_RUN_CONTEXT;
	EhsDataflowIntType output = 0;

	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_bin2dec8_ARG_convert_in0))
		output |= (EHS_FB_IN_B_API2(INX_bin2dec8_ARG_convert_in0) & 1) << 0;
	if (EHS_FB_IN_CONNECTED_API2(INX_bin2dec8_ARG_convert_in1))
		output |= (EHS_FB_IN_B_API2(INX_bin2dec8_ARG_convert_in1) & 1) << 1;
	if (EHS_FB_IN_CONNECTED_API2(INX_bin2dec8_ARG_convert_in2))
		output |= (EHS_FB_IN_B_API2(INX_bin2dec8_ARG_convert_in2) & 1) << 2;
	if (EHS_FB_IN_CONNECTED_API2(INX_bin2dec8_ARG_convert_in3))
		output |= (EHS_FB_IN_B_API2(INX_bin2dec8_ARG_convert_in3) & 1) << 3;
	if (EHS_FB_IN_CONNECTED_API2(INX_bin2dec8_ARG_convert_in4))
		output |= (EHS_FB_IN_B_API2(INX_bin2dec8_ARG_convert_in4) & 1) << 4;
	if (EHS_FB_IN_CONNECTED_API2(INX_bin2dec8_ARG_convert_in5))
		output |= (EHS_FB_IN_B_API2(INX_bin2dec8_ARG_convert_in5) & 1) << 5;
	if (EHS_FB_IN_CONNECTED_API2(INX_bin2dec8_ARG_convert_in6))
		output |= (EHS_FB_IN_B_API2(INX_bin2dec8_ARG_convert_in6) & 1) << 6;
	if (EHS_FB_IN_CONNECTED_API2(INX_bin2dec8_ARG_convert_in7))
		output |= (EHS_FB_IN_B_API2(INX_bin2dec8_ARG_convert_in7) & 1) << 7;
	if (EHS_FB_OUT_CONNECTED_API2(INX_bin2dec8_ARG_convert_out))
		EHS_FB_OUT_I_API2(INX_bin2dec8_ARG_convert_out) = output;
	EHS_FB_FINISH(INX_bin2dec8_ARG_convert_event_out);
}//ICB FUNCTION convert MACRO END -- DO NOT ALTER THIS LINE
