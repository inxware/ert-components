//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-adc_read_single.h"
#include "target_adcdac.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_adc_read_single_state
{
	ehs_uint8 device_id		;//: 4;
	ehs_uint8 channel 		;//: 4;
} inx_adc_read_single_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(adc_read_single)
EHS_FB_FUNCTION_ENTRY("sample", 0x01, adc_read_single_sample)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_adc_read_single_ARG_sample_value 1
#define INX_adc_read_single_ARG_sample_done 1
#define INX_adc_read_single_ARG_sample_error 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_adc_read_single_device_id 0
#define INX_FB_adc_read_single_channel 0
#define INX_FB_adc_read_single_fixed_point 10
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(adc_read_single)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_sint32 device_id;
	ehs_sint32 channel;
	ehs_sint32 fixed_point;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d %d %d",&device_id,&channel,&fixed_point); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_adc_read_single_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(adc_read_single)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_adc_read_single_state_type* inx_adc_read_single_state = (inx_adc_read_single_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
	ehs_uint8 fixed_point;
	const char* pParams = EHS_FB_INIT_PARAMETERS;
	pParams = (pParams) ? EhsGetUint8FromString(&inx_adc_read_single_state->device_id, pParams) : pParams;
	pParams = (pParams) ? EhsGetUint8FromString(&inx_adc_read_single_state->channel, pParams) : pParams;
	pParams = (pParams) ? EhsGetUint8FromString(&fixed_point, pParams) : pParams;
	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(adc_read_single)
{
	//inx_adc_read_single_state_type *inx_adc_read_single_state = (inx_adc_read_single_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION sample MACRO START -- DO NOT ALTER
/**
 * Definition of adc_read_single_sample.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(adc_read_single_sample)
{
	inx_adc_read_single_state_type* inx_adc_read_single_state = (inx_adc_read_single_state_type*)EHS_FB_RUN_CONTEXT;
	/***** Error guarding & handling *****/
	// This unit is not initialised
	if (g_ehs_adc_configs[inx_adc_read_single_state->device_id].unit_config.init == 0)
	{
		EHS_FB_FINISH(INX_adc_read_single_ARG_sample_error);
		return;
	}
	// This ADC is not in single-shot conversion mode
	if (g_ehs_adc_configs[inx_adc_read_single_state->device_id].unit_config.mode != 0)
	{
		EHS_FB_FINISH(INX_adc_read_single_ARG_sample_error);
		return;
	}
	// This ADC channel of the unit is not enabled
	if (g_ehs_adc_configs[inx_adc_read_single_state->device_id].unit_config.channel[inx_adc_read_single_state->channel] == -1)
	{
		EHS_FB_FINISH(INX_adc_read_single_ARG_sample_error);
		return;
	}

	// Your code here
	// Sample the one-shot value here with the fixed-point precision
	if (EHS_FB_OUT_CONNECTED_API2(INX_adc_read_single_ARG_sample_value))
		EHS_FB_OUT_I_API2(INX_adc_read_single_ARG_sample_value) = EhsTAdcChannelSingleRead(inx_adc_read_single_state->device_id, inx_adc_read_single_state->channel);
	EHS_FB_FINISH(INX_adc_read_single_ARG_sample_done);
}//ICB FUNCTION sample MACRO END -- DO NOT ALTER THIS LINE
