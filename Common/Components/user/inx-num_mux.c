//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-num_mux.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_NumMultiplexReal_state
{
} inx_NumMultiplexReal_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(NumMultiplexReal)
EHS_FB_FUNCTION_ENTRY("Run_Input", 0x01, NumMultiplexReal_Run_Input)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_NumMultiplexReal_ARG_Run_Input_n3 1
#define INX_NumMultiplexReal_ARG_Run_Input_n5 2
#define INX_NumMultiplexReal_ARG_Run_Input_n6 3
#define INX_NumMultiplexReal_ARG_Run_Input_d1 4
#define INX_NumMultiplexReal_ARG_Run_Input_d2 5
#define INX_NumMultiplexReal_ARG_Run_Input_n4 6
#define INX_NumMultiplexReal_ARG_Run_Input_n1 7
#define INX_NumMultiplexReal_ARG_Run_Input_n2 8
#define INX_NumMultiplexReal_ARG_Run_Input_negative 9
#define INX_NumMultiplexReal_ARG_Run_Input_o1 1
#define INX_NumMultiplexReal_ARG_Run_Input_o 1
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
EHS_FB_IDENTIFY_FUNCTION(NumMultiplexReal)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_NumMultiplexReal_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(NumMultiplexReal)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	//inx_NumMultiplexReal_state_type* inx_NumMultiplexReal_state = (inx_NumMultiplexReal_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
	//EhsSscanf(EHS_FB_INIT_PARAMETERS,"");

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(NumMultiplexReal)
{
	//inx_NumMultiplexReal_state_type *inx_NumMultiplexReal_state = (inx_NumMultiplexReal_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION Run_Input MACRO START -- DO NOT ALTER
/**
 * Definition of NumMultiplexReal_Run_Input.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(NumMultiplexReal_Run_Input)
{
	//inx_NumMultiplexReal_state_type* inx_NumMultiplexReal_state = (inx_NumMultiplexReal_state_type*)EHS_FB_RUN_CONTEXT;

	ehs_char number_str[20] = {0};
    ehs_sint32 pos = 0;
	ehs_bool negative = EHS_FALSE;

	if (EHS_FB_IN_CONNECTED_API2(INX_NumMultiplexReal_ARG_Run_Input_negative)){
		negative = EHS_FB_IN_B_API2(INX_NumMultiplexReal_ARG_Run_Input_negative);
	}

	if (EHS_FB_IN_CONNECTED_API2(INX_NumMultiplexReal_ARG_Run_Input_n1)){
		ehs_sint32 val = (ehs_sint32)EHS_FB_IN_F_API2(INX_NumMultiplexReal_ARG_Run_Input_n1);
		val = (val < 0) ? val*-1 : val; 
		val = (val < 0 || val > 9) ? 0 : val;
		number_str[pos++] = val + '0';
	}else{
		number_str[pos++] = 0 + '0';
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_NumMultiplexReal_ARG_Run_Input_n2)){
		ehs_sint32 val = (ehs_sint32)EHS_FB_IN_F_API2(INX_NumMultiplexReal_ARG_Run_Input_n2) ;
		val = (val < 0) ? val*-1 : val; 
		val = (val < 0 || val > 9) ? 0 : val;
		number_str[pos++] = val + '0';
	}else{
		number_str[pos++] = 0 + '0';
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_NumMultiplexReal_ARG_Run_Input_n3)){
		ehs_sint32 val = (ehs_sint32)EHS_FB_IN_F_API2(INX_NumMultiplexReal_ARG_Run_Input_n3) ;
		val = (val < 0) ? val*-1 : val; 
		val = (val < 0 || val > 9) ? 0 : val;
		number_str[pos++] = val + '0';
	}else{
		number_str[pos++] = 0 + '0';
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_NumMultiplexReal_ARG_Run_Input_n4)){
		ehs_sint32 val = (ehs_sint32)EHS_FB_IN_F_API2(INX_NumMultiplexReal_ARG_Run_Input_n4) ;
		val = (val < 0) ? val*-1 : val; 
		val = (val < 0 || val > 9) ? 0 : val;
		number_str[pos++] = val + '0';
	}else{
		number_str[pos++] = 0 + '0';
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_NumMultiplexReal_ARG_Run_Input_n5)){
		ehs_sint32 val = (ehs_sint32)EHS_FB_IN_F_API2(INX_NumMultiplexReal_ARG_Run_Input_n5) ;
		val = (val < 0) ? val*-1 : val; 
		val = (val < 0 || val > 9) ? 0 : val;
		number_str[pos++] = val + '0';
	}else{
		number_str[pos++] = 0 + '0';
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_NumMultiplexReal_ARG_Run_Input_n6)){
		ehs_sint32 val = (ehs_sint32)EHS_FB_IN_F_API2(INX_NumMultiplexReal_ARG_Run_Input_n6) ;
		val = (val < 0) ? val*-1 : val; 
		val = (val < 0 || val > 9) ? 0 : val;
		number_str[pos++] = val + '0';
	}else{
		number_str[pos++] = 0 + '0';
	}

	number_str[pos++] = '.'; // add decimal place
	
	if (EHS_FB_IN_CONNECTED_API2(INX_NumMultiplexReal_ARG_Run_Input_d1)){
		ehs_sint32 val = (ehs_sint32)EHS_FB_IN_F_API2(INX_NumMultiplexReal_ARG_Run_Input_d1) ;
		val = (val < 0) ? val*-1 : val; 
		val = (val < 0 || val > 9) ? 0 : val;
		number_str[pos++] = val + '0';
	}else{
		number_str[pos++] = 0 + '0';
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_NumMultiplexReal_ARG_Run_Input_d2)){
		ehs_sint32 val = (ehs_sint32)EHS_FB_IN_F_API2(INX_NumMultiplexReal_ARG_Run_Input_d2) ;
		val = (val < 0) ? val*-1 : val; 
		val = (val < 0 || val > 9) ? 0 : val;
		number_str[pos++] = val + '0';
	}else{
		number_str[pos++] = 0 + '0';
	}

	if (EHS_FB_OUT_CONNECTED_API2(INX_NumMultiplexReal_ARG_Run_Input_o1)){

		float result=0.0;
    	EhsSscanf(number_str, "%f", &result);

		if(negative==EHS_TRUE){
			result *= -1;
		}

		EHS_FB_OUT_F_API2(INX_NumMultiplexReal_ARG_Run_Input_o1) = result;
		EHS_FB_FINISH(INX_NumMultiplexReal_ARG_Run_Input_o);
	}
	
}//ICB FUNCTION Run_Input MACRO END -- DO NOT ALTER THIS LINE
