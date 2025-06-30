//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-sample2str.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_sample2str_state
{
	ehs_sint32 sample_count;
	ehs_sint32 decimal_places;
	const char* fmt_str;
	ehs_float* samples_buff1;
	ehs_float* samples_buff2;
	ehs_float* samples_buff3;
} inx_sample2str_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(sample2str)
EHS_FB_FUNCTION_ENTRY("Run_Input", 0x01, sample2str_Run_Input)
EHS_FB_FUNCTION_ENTRY("Clear", 0x02, sample2str_Clear)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_sample2str_ARG_Run_Input_i1 1
#define INX_sample2str_ARG_Run_Input_i2 2
#define INX_sample2str_ARG_Run_Input_i3 3
#define INX_sample2str_ARG_Run_Input_o1 1
#define INX_sample2str_ARG_Run_Input_o2 2
#define INX_sample2str_ARG_Run_Input_o3 3
#define INX_sample2str_ARG_Run_Input_o 1
#define INX_sample2str_ARG_Clear_cleared 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_sample2str_Sample_Count 10
#define INX_FB_sample2str_Decimal_Places 2
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(sample2str)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_sample2str_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(sample2str)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_sample2str_state_type* inx_sample2str_state = (inx_sample2str_state_type*)EHS_FB_INIT_CONTEXT;
	inx_sample2str_state->sample_count = INX_FB_sample2str_Sample_Count;
	inx_sample2str_state->decimal_places = INX_FB_sample2str_Decimal_Places;
	/* read the initialisation parameters */
	const ehs_char* pParams = EHS_FB_INIT_PARAMETERS;
	pParams = EhsGetSint32FromString(&inx_sample2str_state->sample_count, pParams);
	pParams = EhsGetSint32FromString(&inx_sample2str_state->decimal_places, pParams);

	inx_sample2str_state->samples_buff1 = (ehs_float*)EhsHMem_writeableAlloc(sizeof(ehs_float)*inx_sample2str_state->sample_count);
	inx_sample2str_state->samples_buff2 = (ehs_float*)EhsHMem_writeableAlloc(sizeof(ehs_float)*inx_sample2str_state->sample_count);
	inx_sample2str_state->samples_buff3 = (ehs_float*)EhsHMem_writeableAlloc(sizeof(ehs_float)*inx_sample2str_state->sample_count);

	// set inital samples
	for(ehs_sint32 i = 0; i < inx_sample2str_state->sample_count; i++){
		inx_sample2str_state->samples_buff1[i] = 0.0f;
		inx_sample2str_state->samples_buff2[i] = 0.0f;
		inx_sample2str_state->samples_buff3[i] = 0.0f;
	}

	// assign number of decimal points
	const char* fmt_str = "%.2f";
	switch (inx_sample2str_state->decimal_places)
	{
		case 0:{
			fmt_str = "%.0f";
			break;
		}
		case 1:{
			fmt_str = "%.1f";
			break;
		}
		case 2:{
			fmt_str = "%.2f";
			break;
		}
		case 3:{
			fmt_str = "%.3f";
			break;
		}
		case 4:{
			fmt_str = "%.4f";
			break;
		}
		case 5:{
			fmt_str = "%.5f";
			break;
		}
		case 6:{
			fmt_str = "%.6f";
			break;
		}
		default:{
			fmt_str = "%.2f";
			break;
		}
	}

	inx_sample2str_state->fmt_str = fmt_str;

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(sample2str)
{
	inx_sample2str_state_type *inx_sample2str_state = (inx_sample2str_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

#define EHS_SAMPLE_SEPERATOR ' '

void add_sample_to_float_buffer(ehs_float* sample_buffer, ehs_sint32 sample_buffer_size, ehs_float sample)
{
	// shift data
	for(ehs_uint16 i = 0; i < sample_buffer_size-1; i++){
		sample_buffer[i] = sample_buffer[i+1];
	}
	// assign latest data
	sample_buffer[sample_buffer_size-1] = sample;
}

void float_buffer_to_string(ehs_char* sample_str, const ehs_float* sample_buffer, ehs_sint32 sample_buffer_size, const char* fmt_str)
{
	ehs_uint32 total_len = 0;
	for (ehs_uint32 i = 0; i < sample_buffer_size; i++) {
		ehs_uint32 len = EhsSnprintf(sample_str + total_len, EHS_STRING_LENGTH_MAX-total_len, fmt_str, sample_buffer[i]);
		if(len < 0 || len >= EHS_STRING_LENGTH_MAX - total_len){ // make sure we don't exceed max str len
			sample_str[0] = '\0';
			return;
		}
		total_len += len;
		if (i < sample_buffer_size - 1) {
			if (total_len >= EHS_STRING_LENGTH_MAX - 2) {
				// Not enough space left in the buffer for a space character
				sample_str[0] = '\0';
				return;
			}
			sample_str[total_len++] = EHS_SAMPLE_SEPERATOR;
		}
	}
	sample_str[total_len] = '\0';
}

//ICB FUNCTION Run_Input MACRO START -- DO NOT ALTER
/**
 * Definition of sample2str_Run_Input.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(sample2str_Run_Input)
{
	inx_sample2str_state_type* inx_sample2str_state = (inx_sample2str_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_sample2str_ARG_Run_Input_i1) && EHS_FB_OUT_CONNECTED_API2(INX_sample2str_ARG_Run_Input_o1)){
		ehs_float sample = EHS_FB_IN_F_API2(INX_sample2str_ARG_Run_Input_i1) ;
		add_sample_to_float_buffer(inx_sample2str_state->samples_buff1, inx_sample2str_state->sample_count, sample);
		ehs_char* str = EHS_FB_OUT_S_API2(INX_sample2str_ARG_Run_Input_o1) ;
		float_buffer_to_string(str, inx_sample2str_state->samples_buff1, inx_sample2str_state->sample_count, inx_sample2str_state->fmt_str);
	}

	if (EHS_FB_IN_CONNECTED_API2(INX_sample2str_ARG_Run_Input_i2) && EHS_FB_OUT_CONNECTED_API2(INX_sample2str_ARG_Run_Input_o2)){
		ehs_float sample = EHS_FB_IN_F_API2(INX_sample2str_ARG_Run_Input_i2) ;
		add_sample_to_float_buffer(inx_sample2str_state->samples_buff2, inx_sample2str_state->sample_count, sample);
		ehs_char* str = EHS_FB_OUT_S_API2(INX_sample2str_ARG_Run_Input_o2) ;
		float_buffer_to_string(str, inx_sample2str_state->samples_buff2, inx_sample2str_state->sample_count, inx_sample2str_state->fmt_str);
	}

	if (EHS_FB_IN_CONNECTED_API2(INX_sample2str_ARG_Run_Input_i3) && EHS_FB_OUT_CONNECTED_API2(INX_sample2str_ARG_Run_Input_o3)){
		ehs_float sample = EHS_FB_IN_F_API2(INX_sample2str_ARG_Run_Input_i3) ;
		add_sample_to_float_buffer(inx_sample2str_state->samples_buff3, inx_sample2str_state->sample_count, sample);
		ehs_char* str = EHS_FB_OUT_S_API2(INX_sample2str_ARG_Run_Input_o3) ;
		float_buffer_to_string(str, inx_sample2str_state->samples_buff3, inx_sample2str_state->sample_count, inx_sample2str_state->fmt_str);
	}

	EHS_FB_FINISH(INX_sample2str_ARG_Run_Input_o);
}//ICB FUNCTION Run_Input MACRO END -- DO NOT ALTER THIS LINE
/**
 * Definition of sample2str_Clear.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(sample2str_Clear)
{
	inx_sample2str_state_type* inx_sample2str_state = (inx_sample2str_state_type*)EHS_FB_RUN_CONTEXT;

	for(ehs_sint32 i = 0; i < inx_sample2str_state->sample_count; i++){
		inx_sample2str_state->samples_buff1[i] = 0.0f;
		inx_sample2str_state->samples_buff2[i] = 0.0f;
		inx_sample2str_state->samples_buff3[i] = 0.0f;
	}

	// Your code here
	EHS_FB_FINISH(INX_sample2str_ARG_Clear_cleared);
}//ICB FUNCTION Clear MACRO END -- DO NOT ALTER THIS LINE