//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-indexed_demux_int.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_indexed_demux_int_state
{
	ehs_sint32 default_v;
} inx_indexed_demux_int_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(indexed_demux_int)
EHS_FB_FUNCTION_ENTRY("demux", 0x01, indexed_demux_int_demux)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_indexed_demux_int_ARG_demux_in 1
#define INX_indexed_demux_int_ARG_demux_index 2
#define INX_indexed_demux_int_ARG_demux_out1 1
#define INX_indexed_demux_int_ARG_demux_out2 2
#define INX_indexed_demux_int_ARG_demux_out3 3
#define INX_indexed_demux_int_ARG_demux_out4 4
#define INX_indexed_demux_int_ARG_demux_out5 5
#define INX_indexed_demux_int_ARG_demux_out6 6
#define INX_indexed_demux_int_ARG_demux_out7 7
#define INX_indexed_demux_int_ARG_demux_out8 8
#define INX_indexed_demux_int_ARG_demux_offset 9
#define INX_indexed_demux_int_ARG_demux_passthrough 10
#define INX_indexed_demux_int_ARG_demux_err 1
#define INX_indexed_demux_int_ARG_demux_evt1 2
#define INX_indexed_demux_int_ARG_demux_evt2 3
#define INX_indexed_demux_int_ARG_demux_evt3 4
#define INX_indexed_demux_int_ARG_demux_evt4 5
#define INX_indexed_demux_int_ARG_demux_evt5 6
#define INX_indexed_demux_int_ARG_demux_evt6 7
#define INX_indexed_demux_int_ARG_demux_evt7 8
#define INX_indexed_demux_int_ARG_demux_evt8 9
#define INX_indexed_demux_int_ARG_demux_ovf 10
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_indexed_demux_int_default 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(indexed_demux_int)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_sint32 default;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d",&default); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_indexed_demux_int_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(indexed_demux_int)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_indexed_demux_int_state_type* inx_indexed_demux_int_state = (inx_indexed_demux_int_state_type*)EHS_FB_INIT_CONTEXT;
	inx_indexed_demux_int_state->default_v = INX_FB_indexed_demux_int_default;
	/* read the initialisation parameters */
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d",&inx_indexed_demux_int_state->default_v);

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(indexed_demux_int)
{
	inx_indexed_demux_int_state_type *inx_indexed_demux_int_state = (inx_indexed_demux_int_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION demux MACRO START -- DO NOT ALTER
/**
 * Definition of indexed_demux_int_demux.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(indexed_demux_int_demux)
{
	inx_indexed_demux_int_state_type* inx_indexed_demux_int_state = (inx_indexed_demux_int_state_type*)EHS_FB_RUN_CONTEXT;
	
	ehs_sint32 input = inx_indexed_demux_int_state->default_v;
	ehs_sint32 index = 0;

	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_indexed_demux_int_ARG_demux_in))
		input = EHS_FB_IN_I_API2(INX_indexed_demux_int_ARG_demux_in) ;

	if (EHS_FB_IN_CONNECTED_API2(INX_indexed_demux_int_ARG_demux_index))
		index = EHS_FB_IN_I_API2(INX_indexed_demux_int_ARG_demux_index) ;
	else
	{
		EHS_FB_FINISH(INX_indexed_demux_int_ARG_demux_err);
		return;
	}

	if (index <= 0)
	{
		EHS_FB_FINISH(INX_indexed_demux_int_ARG_demux_err);
		return;
	}

	if (index > 8)
	{
		if (EHS_FB_OUT_CONNECTED_API2(INX_indexed_demux_int_ARG_demux_offset))
			EHS_FB_OUT_I_API2(INX_indexed_demux_int_ARG_demux_offset) = index - 8;
		if (EHS_FB_OUT_CONNECTED_API2(INX_indexed_demux_int_ARG_demux_passthrough))
			EHS_FB_OUT_I_API2(INX_indexed_demux_int_ARG_demux_passthrough) = input;
		EHS_FB_FINISH(INX_indexed_demux_int_ARG_demux_ovf);
		return;
	}

	switch (index)
	{
		//
		case 1:
		{
			if (EHS_FB_OUT_CONNECTED_API2(INX_indexed_demux_int_ARG_demux_out1))
				EHS_FB_OUT_I_API2(INX_indexed_demux_int_ARG_demux_out1) = input;
			EHS_FB_FINISH(INX_indexed_demux_int_ARG_demux_evt1);
			break;
		}
		case 2:
		{
			if (EHS_FB_OUT_CONNECTED_API2(INX_indexed_demux_int_ARG_demux_out2))
				EHS_FB_OUT_I_API2(INX_indexed_demux_int_ARG_demux_out2) = input;
			EHS_FB_FINISH(INX_indexed_demux_int_ARG_demux_evt2);
			break;
		}
		case 3:
		{
			if (EHS_FB_OUT_CONNECTED_API2(INX_indexed_demux_int_ARG_demux_out3))
				EHS_FB_OUT_I_API2(INX_indexed_demux_int_ARG_demux_out3) = input;
			EHS_FB_FINISH(INX_indexed_demux_int_ARG_demux_evt3);
			break;
		}
		case 4:
		{
			if (EHS_FB_OUT_CONNECTED_API2(INX_indexed_demux_int_ARG_demux_out4))
				EHS_FB_OUT_I_API2(INX_indexed_demux_int_ARG_demux_out4) = input;
			EHS_FB_FINISH(INX_indexed_demux_int_ARG_demux_evt4);
			break;
		}
		case 5:
		{
			if (EHS_FB_OUT_CONNECTED_API2(INX_indexed_demux_int_ARG_demux_out5))
				EHS_FB_OUT_I_API2(INX_indexed_demux_int_ARG_demux_out5) = input;
			EHS_FB_FINISH(INX_indexed_demux_int_ARG_demux_evt5);
			break;
		}
		case 6:
		{
			if (EHS_FB_OUT_CONNECTED_API2(INX_indexed_demux_int_ARG_demux_out6))
				EHS_FB_OUT_I_API2(INX_indexed_demux_int_ARG_demux_out6) = input;
			EHS_FB_FINISH(INX_indexed_demux_int_ARG_demux_evt6);
			break;
		}
		case 7:
		{
			if (EHS_FB_OUT_CONNECTED_API2(INX_indexed_demux_int_ARG_demux_out7))
				EHS_FB_OUT_I_API2(INX_indexed_demux_int_ARG_demux_out7) = input;
			EHS_FB_FINISH(INX_indexed_demux_int_ARG_demux_evt7);
			break;
		}
		case 8:
		{
			if (EHS_FB_OUT_CONNECTED_API2(INX_indexed_demux_int_ARG_demux_out8))
				EHS_FB_OUT_I_API2(INX_indexed_demux_int_ARG_demux_out8) = input;
			EHS_FB_FINISH(INX_indexed_demux_int_ARG_demux_evt8);
			break;
		}
		default:
		{
			EHS_FB_FINISH(INX_indexed_demux_int_ARG_demux_err);
			return;
			break;
		}
	}
}//ICB FUNCTION demux MACRO END -- DO NOT ALTER THIS LINE
