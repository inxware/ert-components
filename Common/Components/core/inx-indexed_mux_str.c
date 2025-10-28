//ICB HEADER MACRO START -- DO NOT ALTER
#include "globals.h"
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-indexed_mux_str.h"
#include "hal-api.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_indexed_mux_str_state
{
	ehs_char *in[8];
} inx_indexed_mux_str_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(indexed_mux_str)
EHS_FB_FUNCTION_ENTRY("mux", 0x01, indexed_mux_str_mux)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_indexed_mux_str_ARG_mux_index 1
#define INX_indexed_mux_str_ARG_mux_in1 2
#define INX_indexed_mux_str_ARG_mux_in2 3
#define INX_indexed_mux_str_ARG_mux_in3 4
#define INX_indexed_mux_str_ARG_mux_in4 5
#define INX_indexed_mux_str_ARG_mux_in5 6
#define INX_indexed_mux_str_ARG_mux_in6 7
#define INX_indexed_mux_str_ARG_mux_in7 8
#define INX_indexed_mux_str_ARG_mux_in8 9
#define INX_indexed_mux_str_ARG_mux_overflow 1
#define INX_indexed_mux_str_ARG_mux_output 2
#define INX_indexed_mux_str_ARG_mux_event_out 1
#define INX_indexed_mux_str_ARG_mux_event_overflow 2
#define INX_indexed_mux_str_ARG_mux_err 3
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_indexed_mux_str_in1 
#define INX_FB_indexed_mux_str_in2 
#define INX_FB_indexed_mux_str_in3 
#define INX_FB_indexed_mux_str_in4 
#define INX_FB_indexed_mux_str_in5 
#define INX_FB_indexed_mux_str_in6 
#define INX_FB_indexed_mux_str_in7 
#define INX_FB_indexed_mux_str_in8 
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(indexed_mux_str)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_char* in1;
	ehs_char* in2;
	ehs_char* in3;
	ehs_char* in4;
	ehs_char* in5;
	ehs_char* in6;
	ehs_char* in7;
	ehs_char* in8;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%s %s %s %s %s %s %s %s",&in1,&in2,&in3,&in4,&in5,&in6,&in7,&in8); */
	EHS_FB_IDENTIFY_MEMORY = EhsStrlen(EHS_FB_IDENTIFY_PARAMETERS) + 1;//sizeof(inx_indexed_mux_str_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(indexed_mux_str)
{
	ehs_uint8 i = 0;
	//TODO place it back to be a pointer once it's fixed
	// ehs_char *in_temp = NULL;
	ehs_char in_temp[EHS_STRING_LENGTH_MAX];
	ehs_char *pFbInitParam = (ehs_char *) EHS_FB_INIT_PARAMETERS;
	ehs_uint16 str_count = 0;
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_indexed_mux_str_state_type* inx_indexed_mux_str_state = (inx_indexed_mux_str_state_type*)EHS_FB_INIT_CONTEXT;
	/* Allocate memory to read the parameter */
	//TODO place it back to EhsHMem_tempAlloc once it's fixed
	// in_temp = (ehs_char *) EhsHMem_permAlloc(EhsStrlen(EHS_FB_INIT_PARAMETERS) + 1);
	// Init error if there is no enough memory left to be allocated
	if (in_temp == NULL) return EHS_FALSE;
	/* read the initialisation parameters */
	for (i = 0 ; i < 8 && pFbInitParam ; i++)
	{
		EhsMemset(in_temp, '\0', str_count + 1);
		pFbInitParam = EhsStrTrimL(pFbInitParam);
		if (pFbInitParam[0] != '"')
		{
			/* When it's not a string containing spaces */
			pFbInitParam = EhsGetWordFromString(in_temp, pFbInitParam);
			str_count = EhsStrlen(in_temp);
			if (str_count == 4 && EhsStrncmp(in_temp, "NULL", 4) == 0)
			{
				inx_indexed_mux_str_state->in[i] = NULL;
				continue;
			}
			else
			{
				goto jump_point;
			}
		}
		else
		{
			/* When it's a string containing spaces */
			pFbInitParam = EhsGetQuoteDelimFromString(in_temp, pFbInitParam, EHS_STRING_LENGTH_MAX);
			str_count = EhsStrlen(in_temp);
			goto jump_point;
		}
jump_point:
		if (str_count > 0)
		{
			inx_indexed_mux_str_state->in[i] = EhsHMem_writeableAlloc(str_count + 1);
			if (inx_indexed_mux_str_state->in[i] != NULL)
			{
				EhsStrcpy(inx_indexed_mux_str_state->in[i], in_temp);
				inx_indexed_mux_str_state->in[i][str_count] = '\0';
			}
		}
		else inx_indexed_mux_str_state->in[i] = NULL;
	}

	/* Free the temperory allocated memory for parsing */
	//TODO place it back to EhsHMem_tempAlloc once it's fixed
	// EhsHMem_permFree(in_temp);

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(indexed_mux_str)
{
	inx_indexed_mux_str_state_type *inx_indexed_mux_str_state = (inx_indexed_mux_str_state_type*)EHS_FB_DESTROY_CONTEXT;
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION mux MACRO START -- DO NOT ALTER
/**
 * Definition of indexed_mux_str_mux.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(indexed_mux_str_mux)
{
	if (!EHS_FB_IN_CONNECTED_API2(INX_indexed_mux_str_ARG_mux_index)) return;
	if (EHS_FB_IN_I_API2(INX_indexed_mux_str_ARG_mux_index) < 1)
	{
		EHS_FB_FINISH(INX_indexed_mux_str_ARG_mux_err);
		return;
	}
	if (EHS_FB_IN_I_API2(INX_indexed_mux_str_ARG_mux_index) > 8)
	{
		if (EHS_FB_OUT_CONNECTED_API2(INX_indexed_mux_str_ARG_mux_overflow))
			EHS_FB_OUT_I_API2(INX_indexed_mux_str_ARG_mux_overflow) = EHS_FB_IN_I_API2(INX_indexed_mux_str_ARG_mux_index) - 8;
		EHS_FB_FINISH(INX_indexed_mux_str_ARG_mux_event_overflow);
		return;
	}
	inx_indexed_mux_str_state_type* inx_indexed_mux_str_state = (inx_indexed_mux_str_state_type*)EHS_FB_RUN_CONTEXT;
	EhsDataflowStringType *session[8] = {
		(EhsDataflowStringType *) inx_indexed_mux_str_state->in[0],
		(EhsDataflowStringType *) inx_indexed_mux_str_state->in[1],
		(EhsDataflowStringType *) inx_indexed_mux_str_state->in[2],
		(EhsDataflowStringType *) inx_indexed_mux_str_state->in[3],
		(EhsDataflowStringType *) inx_indexed_mux_str_state->in[4],
		(EhsDataflowStringType *) inx_indexed_mux_str_state->in[5],
		(EhsDataflowStringType *) inx_indexed_mux_str_state->in[6],
		(EhsDataflowStringType *) inx_indexed_mux_str_state->in[7],
	};

	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_indexed_mux_str_ARG_mux_in1))
		session[0] = (EhsDataflowStringType *) EHS_FB_IN_S_API2(INX_indexed_mux_str_ARG_mux_in1) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_indexed_mux_str_ARG_mux_in2))
		session[1] = (EhsDataflowStringType *) EHS_FB_IN_S_API2(INX_indexed_mux_str_ARG_mux_in2) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_indexed_mux_str_ARG_mux_in3))
		session[2] = (EhsDataflowStringType *) EHS_FB_IN_S_API2(INX_indexed_mux_str_ARG_mux_in3) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_indexed_mux_str_ARG_mux_in4))
		session[3] = (EhsDataflowStringType *) EHS_FB_IN_S_API2(INX_indexed_mux_str_ARG_mux_in4) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_indexed_mux_str_ARG_mux_in5))
		session[4] = (EhsDataflowStringType *) EHS_FB_IN_S_API2(INX_indexed_mux_str_ARG_mux_in5) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_indexed_mux_str_ARG_mux_in6))
		session[5] = (EhsDataflowStringType *) EHS_FB_IN_S_API2(INX_indexed_mux_str_ARG_mux_in6) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_indexed_mux_str_ARG_mux_in7))
		session[6] = (EhsDataflowStringType *) EHS_FB_IN_S_API2(INX_indexed_mux_str_ARG_mux_in7) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_indexed_mux_str_ARG_mux_in8))
		session[7] = (EhsDataflowStringType *) EHS_FB_IN_S_API2(INX_indexed_mux_str_ARG_mux_in8) ;
	if (EHS_FB_OUT_CONNECTED_API2(INX_indexed_mux_str_ARG_mux_output))
	{
		EhsMemset(EHS_FB_OUT_S_API2(INX_indexed_mux_str_ARG_mux_output), 0, EHS_STRING_LENGTH_MAX);
		if (session[EHS_FB_IN_I_API2(INX_indexed_mux_str_ARG_mux_index) - 1] == NULL)
		{
			EhsMemcpy(
				EHS_FB_OUT_S_API2(INX_indexed_mux_str_ARG_mux_output),
				"",
				1
			);
		}
		else
		{
			EhsMemcpy(
				EHS_FB_OUT_S_API2(INX_indexed_mux_str_ARG_mux_output),
				session[EHS_FB_IN_I_API2(INX_indexed_mux_str_ARG_mux_index) - 1],
				EhsStrlen((const char*)session[EHS_FB_IN_I_API2(INX_indexed_mux_str_ARG_mux_index) - 1])
			);
		}
	}
	EHS_FB_FINISH(INX_indexed_mux_str_ARG_mux_event_out);
}//ICB FUNCTION mux MACRO END -- DO NOT ALTER THIS LINE
