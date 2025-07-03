//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-adc_read_continuous.h"
#include "target_adcdac.h"

//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_adc_read_continuous_state
{
	EhsCallbackQueueEntryType xEntry;
	ehs_uint8 device_id			;//: 4;
	ehs_uint8 channel			;//: 4;
	ehs_uint32 decimation;
	ehs_uint32 decimate_average;
	ehs_float bias;
} inx_adc_read_continuous_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
#ifndef EHS_TARGET_ADC_UNIT_NUMBER
#define EHS_TARGET_ADC_UNIT_NUMBER 1
#endif//EHS_TARGET_ADC_UNIT_NUMBER 
#ifndef EHS_TARGET_ADC_CHANNEL_NUMBER
#define EHS_TARGET_ADC_CHANNEL_NUMBER 1
#endif//EHS_TARGET_ADC_CHANNEL_NUMBER
volatile static EHS_DATA_MEMORY_ATTRIB EhsCallbackQueueType xAdcReadContinuousCallbackQueue[EHS_TARGET_ADC_UNIT_NUMBER][EHS_TARGET_ADC_CHANNEL_NUMBER];
volatile static EHS_DATA_MEMORY_ATTRIB ehs_uint32 gAdcReadContinuousResultMean[EHS_TARGET_ADC_UNIT_NUMBER][EHS_TARGET_ADC_CHANNEL_NUMBER];
volatile static EHS_DATA_MEMORY_ATTRIB ehs_uint32 gAdcReadContinuousResultMeanSquared[EHS_TARGET_ADC_UNIT_NUMBER][EHS_TARGET_ADC_CHANNEL_NUMBER];
volatile static EHS_DATA_MEMORY_ATTRIB ehs_uint32 gAdcReadContinuousResultVariance[EHS_TARGET_ADC_UNIT_NUMBER][EHS_TARGET_ADC_CHANNEL_NUMBER];
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(adc_read_continuous)
EHS_FB_FUNCTION_ENTRY("enable", 0x01, adc_read_continuous_enable)
EHS_FB_FUNCTION_ENTRY("disable", 0x02, adc_read_continuous_disable)
EHS_FB_FUNCTION_ENTRY("convert", 0x03, adc_read_continuous_convert)
EHS_FB_FUNCTION_ENTRY("set_bias", 0x04, adc_read_continuous_set_bias)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_adc_read_continuous_ARG_enable_en_done 1
#define INX_adc_read_continuous_ARG_disable_dis_done 1
#define INX_adc_read_continuous_ARG_convert_mean 1
#define INX_adc_read_continuous_ARG_convert_variance 2
#define INX_adc_read_continuous_ARG_convert_mean_sqared 3
#define INX_adc_read_continuous_ARG_convert_done 1
#define INX_adc_read_continuous_ARG_convert_error 2
#define INX_adc_read_continuous_ARG_set_bias_bias 1
#define INX_adc_read_continuous_ARG_set_bias_setOK 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_adc_read_continuous_device_id 0
#define INX_FB_adc_read_continuous_channel 0
#define INX_FB_adc_read_continuous_decimation 1
#define INX_FB_adc_read_continuous_decimate_average 1
#define INX_FB_adc_read_continuous_bias 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(adc_read_continuous)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_sint32 device_id;
	ehs_sint32 channel;
	ehs_sint32 decimation;
	ehs_sint32 decimate_average;
	ehs_float bias;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d %d %d %d %f",&device_id,&channel,&decimation,&decimate_average,&bias); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_adc_read_continuous_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(adc_read_continuous)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_adc_read_continuous_state_type* inx_adc_read_continuous_state = (inx_adc_read_continuous_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"%hhu %hhu %u %u %f",
		&inx_adc_read_continuous_state->device_id,
		&inx_adc_read_continuous_state->channel,
		&inx_adc_read_continuous_state->decimation,
		&inx_adc_read_continuous_state->decimate_average,
		&inx_adc_read_continuous_state->bias);
	
	// Init failed if Device ID or channel ID is out of range
	if (inx_adc_read_continuous_state->channel > EHS_TARGET_ADC_CHANNEL_NUMBER || inx_adc_read_continuous_state->device_id > EHS_TARGET_ADC_UNIT_NUMBER) return EHS_FALSE;
	EhsCallbackQueue_register(&xAdcReadContinuousCallbackQueue[inx_adc_read_continuous_state->device_id][inx_adc_read_continuous_state->channel],
			   EHS_FB_RUN_NAME(adc_read_continuous_convert),
			   EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(-1), //TODO
			   &(inx_adc_read_continuous_state->xEntry)
		);
	
	g_ehs_adc_configs[inx_adc_read_continuous_state->device_id].channel_configs[inx_adc_read_continuous_state->channel].decimation = inx_adc_read_continuous_state->decimation;
	g_ehs_adc_configs[inx_adc_read_continuous_state->device_id].channel_configs[inx_adc_read_continuous_state->channel].decimate_average = inx_adc_read_continuous_state->decimate_average;
	g_ehs_adc_configs[inx_adc_read_continuous_state->device_id].channel_configs[inx_adc_read_continuous_state->channel].bias = (ehs_uint32)(inx_adc_read_continuous_state->bias * (ehs_float)(1 << 10));

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(adc_read_continuous)
{
	//inx_adc_read_continuous_state_type *inx_adc_read_continuous_state = (inx_adc_read_continuous_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION enable MACRO START -- DO NOT ALTER
/**
 * Definition of adc_read_continuous_enable.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(adc_read_continuous_enable)
{
	inx_adc_read_continuous_state_type* inx_adc_read_continuous_state = (inx_adc_read_continuous_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	if (g_ehs_adc_continuous_enabled_bitmask)
		EHS_SET_BIT_N(g_ehs_adc_continuous_enabled_bitmask[inx_adc_read_continuous_state->device_id], inx_adc_read_continuous_state->channel);
	EHS_FB_FINISH(INX_adc_read_continuous_ARG_enable_en_done);
}//ICB FUNCTION enable MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION disable MACRO START -- DO NOT ALTER
/**
 * Definition of adc_read_continuous_disable.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(adc_read_continuous_disable)
{
	inx_adc_read_continuous_state_type* inx_adc_read_continuous_state = (inx_adc_read_continuous_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	if (g_ehs_adc_continuous_enabled_bitmask)
		EHS_CLEAR_BIT_N(g_ehs_adc_continuous_enabled_bitmask[inx_adc_read_continuous_state->device_id], inx_adc_read_continuous_state->channel);
	EHS_FB_FINISH(INX_adc_read_continuous_ARG_disable_dis_done);
}//ICB FUNCTION disable MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION convert MACRO START -- DO NOT ALTER
/**
 * Definition of adc_read_continuous_convert.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_MEMORY_ATTRIB EHS_FB_RUN_FUNCTION(adc_read_continuous_convert)
{
	inx_adc_read_continuous_state_type* inx_adc_read_continuous_state = (inx_adc_read_continuous_state_type*)EHS_FB_RUN_CONTEXT;
	/***** Error guarding & handling *****/
	// This unit is not initialised
	if (g_ehs_adc_configs[inx_adc_read_continuous_state->device_id].unit_config.init == 0)
	{
		EHS_FB_FINISH(INX_adc_read_continuous_ARG_convert_error);
		return;
	}
	// This ADC is not in continuous conversion mode
	if (g_ehs_adc_configs[inx_adc_read_continuous_state->device_id].unit_config.mode != 1)
	{
		EHS_FB_FINISH(INX_adc_read_continuous_ARG_convert_error);
		return;
	}
	// This ADC channel of the unit is not enabled
	if (g_ehs_adc_configs[inx_adc_read_continuous_state->device_id].unit_config.channel[inx_adc_read_continuous_state->channel] == -1)
	{
		EHS_FB_FINISH(INX_adc_read_continuous_ARG_convert_error);
		return;
	}

	// Your code here
	// Internal trigger event to output the value when ready (should be called from ISR)
	if (EHS_FB_OUT_CONNECTED_API2(INX_adc_read_continuous_ARG_convert_mean))
		EHS_FB_OUT_I_API2(INX_adc_read_continuous_ARG_convert_mean) = gAdcReadContinuousResultMean[inx_adc_read_continuous_state->device_id][inx_adc_read_continuous_state->channel];
	if (EHS_FB_OUT_CONNECTED_API2(INX_adc_read_continuous_ARG_convert_mean_sqared))
		EHS_FB_OUT_I_API2(INX_adc_read_continuous_ARG_convert_mean_sqared) = gAdcReadContinuousResultMeanSquared[inx_adc_read_continuous_state->device_id][inx_adc_read_continuous_state->channel];
	if (EHS_FB_OUT_CONNECTED_API2(INX_adc_read_continuous_ARG_convert_variance))
		EHS_FB_OUT_I_API2(INX_adc_read_continuous_ARG_convert_variance) = gAdcReadContinuousResultVariance[inx_adc_read_continuous_state->device_id][inx_adc_read_continuous_state->channel];
	EHS_FB_FINISH(INX_adc_read_continuous_ARG_convert_done);
	
}//ICB FUNCTION convert MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION set_bias MACRO START -- DO NOT ALTER
/**
 * Definition of adc_read_continuous_set_bias.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(adc_read_continuous_set_bias)
{
	inx_adc_read_continuous_state_type* inx_adc_read_continuous_state = (inx_adc_read_continuous_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_adc_read_continuous_ARG_set_bias_bias))
		g_ehs_adc_configs[inx_adc_read_continuous_state->device_id].channel_configs[inx_adc_read_continuous_state->channel].bias = EHS_FB_IN_I_API2(INX_adc_read_continuous_ARG_set_bias_bias) ;
	EHS_FB_FINISH(INX_adc_read_continuous_ARG_set_bias_setOK);
}//ICB FUNCTION set_bias MACRO END -- DO NOT ALTER THIS LINE

EHS_MEMORY_ATTRIB void EhsTHAdcChannelContinuousConvertCB(ehs_uint8 unit, ehs_uint8 channel, ehs_sint32 mean, ehs_uint32 variance, ehs_uint32 mean_squared)
{
	gAdcReadContinuousResultMean[unit][channel] = mean;
	gAdcReadContinuousResultVariance[unit][channel] = variance;
	gAdcReadContinuousResultMeanSquared[unit][channel] = mean_squared;
	EhsCallbackQueue_execute(&xAdcReadContinuousCallbackQueue[unit][channel]);
}