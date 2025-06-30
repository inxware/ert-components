//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-console_print.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_console_print_state
{
	ehs_bool New_Line;
} inx_console_print_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(console_print)
EHS_FB_FUNCTION_ENTRY("PrintFunc", 0x01, console_print_PrintFunc)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_console_print_ARG_PrintFunc_data 1
#define INX_console_print_ARG_PrintFunc_done 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_console_print_New_Line 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(console_print)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_bool New_Line;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d",&New Line); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_console_print_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(console_print)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_console_print_state_type* inx_console_print_state = (inx_console_print_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
	inx_console_print_state->New_Line = (ehs_bool)INX_FB_console_print_New_Line;
	const char* pParams = EHS_FB_INIT_PARAMETERS;
	pParams = (pParams) ? EhsGetUint8FromString(&inx_console_print_state->New_Line, pParams) : pParams;

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(console_print)
{
	inx_console_print_state_type *inx_console_print_state = (inx_console_print_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION PrintFunc MACRO START -- DO NOT ALTER
/**
 * Definition of console_print_PrintFunc.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(console_print_PrintFunc)
{
	inx_console_print_state_type* inx_console_print_state = (inx_console_print_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_console_print_ARG_PrintFunc_data)){
		const char* str = (const char*)EHS_FB_IN_S_API2(INX_console_print_ARG_PrintFunc_data);
		if(inx_console_print_state->New_Line == EHS_TRUE){
			EhsStdioPrintf("%s\n",str); 
		}else{
			EhsStdioPrintf("%s",str);
		}
	}
	EHS_FB_FINISH(INX_console_print_ARG_PrintFunc_done);
}//ICB FUNCTION PrintFunc MACRO END -- DO NOT ALTER THIS LINE
