//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-num_demux.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_NumDemultiplexReal_state
{
} inx_NumDemultiplexReal_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(NumDemultiplexReal)
EHS_FB_FUNCTION_ENTRY("Run_Output", 0x01, NumDemultiplexReal_Run_Output)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_NumDemultiplexReal_ARG_Run_Output_i1 1
#define INX_NumDemultiplexReal_ARG_Run_Output_n1 1
#define INX_NumDemultiplexReal_ARG_Run_Output_n2 2
#define INX_NumDemultiplexReal_ARG_Run_Output_n3 3
#define INX_NumDemultiplexReal_ARG_Run_Output_n4 4
#define INX_NumDemultiplexReal_ARG_Run_Output_n5 5
#define INX_NumDemultiplexReal_ARG_Run_Output_n6 6
#define INX_NumDemultiplexReal_ARG_Run_Output_d1 7
#define INX_NumDemultiplexReal_ARG_Run_Output_d2 8
#define INX_NumDemultiplexReal_ARG_Run_Output_negative 9
#define INX_NumDemultiplexReal_ARG_Run_Output_o 1
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
EHS_FB_IDENTIFY_FUNCTION(NumDemultiplexReal)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_NumDemultiplexReal_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(NumDemultiplexReal)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	//inx_NumDemultiplexReal_state_type* inx_NumDemultiplexReal_state = (inx_NumDemultiplexReal_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
	//EhsSscanf(EHS_FB_INIT_PARAMETERS,"");

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(NumDemultiplexReal)
{
	//inx_NumDemultiplexReal_state_type *inx_NumDemultiplexReal_state = (inx_NumDemultiplexReal_state_type*)EHS_FB_DESTROY_CONTEXT;
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION Run_Output MACRO START -- DO NOT ALTER
/**
 * Definition of NumDemultiplexReal_Run_Output.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(NumDemultiplexReal_Run_Output)
{
	//inx_NumDemultiplexReal_state_type* inx_NumDemultiplexReal_state = (inx_NumDemultiplexReal_state_type*)EHS_FB_RUN_CONTEXT;

	// Define the input float number
    ehs_float input = 0.0;

	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_NumDemultiplexReal_ARG_Run_Output_i1)){
		input = EHS_FB_IN_F_API2(INX_NumDemultiplexReal_ARG_Run_Output_i1) ;
	}

	// handle -ve values
	if (EHS_FB_OUT_CONNECTED_API2(INX_NumDemultiplexReal_ARG_Run_Output_negative)){
		ehs_bool negative = (input < 0.0);
		if(negative==EHS_TRUE){
			input *= -1; // get rid of sign
		}
		EHS_FB_OUT_B_API2(INX_NumDemultiplexReal_ARG_Run_Output_negative) = negative;
	}
    
    // Convert the float number to a string
    ehs_char input_str[20];
    EhsSnprintf(input_str, sizeof(input_str), "%.2f", input); // Limit to 2 decimal places

    // Determine the length of the input string
    ehs_sint32 length = EhsStrlen(input_str);

	// Initialize an array to store the digits
    ehs_uint32 digits[8] = {0};
    ehs_sint32 digit_count = 0;

    // Loop through the input string to extract digits
    for (ehs_sint32 i = length-1; i >= 0 && digit_count < 8; i--) {
        if (input_str[i] >= '0' && input_str[i] <= '9') {
            digits[digit_count] = input_str[i] - '0'; // Convert char to int
            digit_count++;
        }
    }

	if (EHS_FB_OUT_CONNECTED_API2(INX_NumDemultiplexReal_ARG_Run_Output_n1)){
		EHS_FB_OUT_F_API2(INX_NumDemultiplexReal_ARG_Run_Output_n1) = digits[7];
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_NumDemultiplexReal_ARG_Run_Output_n2)){
		EHS_FB_OUT_F_API2(INX_NumDemultiplexReal_ARG_Run_Output_n2) = digits[6];
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_NumDemultiplexReal_ARG_Run_Output_n3)){
		EHS_FB_OUT_F_API2(INX_NumDemultiplexReal_ARG_Run_Output_n3) = digits[5];
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_NumDemultiplexReal_ARG_Run_Output_n4)){
		EHS_FB_OUT_F_API2(INX_NumDemultiplexReal_ARG_Run_Output_n4) = digits[4];
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_NumDemultiplexReal_ARG_Run_Output_n5)){
		EHS_FB_OUT_F_API2(INX_NumDemultiplexReal_ARG_Run_Output_n5) = digits[3];
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_NumDemultiplexReal_ARG_Run_Output_n6)){
		EHS_FB_OUT_F_API2(INX_NumDemultiplexReal_ARG_Run_Output_n6) = digits[2];
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_NumDemultiplexReal_ARG_Run_Output_d1)){
		EHS_FB_OUT_F_API2(INX_NumDemultiplexReal_ARG_Run_Output_d1) = digits[1];
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_NumDemultiplexReal_ARG_Run_Output_d2)){
		EHS_FB_OUT_F_API2(INX_NumDemultiplexReal_ARG_Run_Output_d2) = digits[0];
	}
	EHS_FB_FINISH(INX_NumDemultiplexReal_ARG_Run_Output_o);
}//ICB FUNCTION Run_Output MACRO END -- DO NOT ALTER THIS LINE
