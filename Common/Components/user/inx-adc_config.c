//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-adc_config.h"
#include "target_adcdac.h"

#if 0
/* IF these are not set in the target then set defaults */
#ifndef EHS_TARGET_ADC_FREQ_MAX
#define EHS_TARGET_ADC_FREQ_MAX 1000000000
#endif//EHS_TARGET_ADC_FREQ_MAX
#ifndef EHS_TARGET_ADC_FREQ_MIN
#define EHS_TARGET_ADC_FREQ_MIN 0
#endif//EHS_TARGET_ADC_FREQ_MIN
#endif

//ICB HEADER MACRO END -- DO NOT ALTER
#if EHS_TARGET_ADC_CHANNEL_NUMBER > 16
#error The maximum number of ADC channel is 16!
#endif
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_adc_config_state
{
	ehs_uint8 device_id : 4;
	ehs_uint32 f_s_hz;
	ehs_bool mode : 1;
	ehs_sint16 channel_0  : 11;
	ehs_sint16 channel_1  : 11;
	ehs_sint16 channel_2  : 11;
	ehs_sint16 channel_3  : 11;
	ehs_sint16 channel_4  : 11;
	ehs_sint16 channel_5  : 11;
	ehs_sint16 channel_6  : 11;
	ehs_sint16 channel_7  : 11;
	ehs_sint16 channel_8  : 11;
	ehs_sint16 channel_9  : 11;
	ehs_sint16 channel_10 : 11;
	ehs_sint16 channel_11 : 11;
	ehs_sint16 channel_12 : 11;
	ehs_sint16 channel_13 : 11;
	ehs_sint16 channel_14 : 11;
	ehs_sint16 channel_15 : 11;
} inx_adc_config_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(adc_config)
EHS_FB_FUNCTION_ENTRY("config", 0x01, adc_config_config)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_adc_config_ARG_config_JSON 1
#define INX_adc_config_ARG_config_finish 1
#define INX_adc_config_ARG_config_error 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_adc_config_device_id 0
#define INX_FB_adc_config_f_s_hz 1000
#define INX_FB_adc_config_mode 0
#define INX_FB_adc_config_channel_0 -1
#define INX_FB_adc_config_channel_1 -1
#define INX_FB_adc_config_channel_2 -1
#define INX_FB_adc_config_channel_3 -1
#define INX_FB_adc_config_channel_4 -1
#define INX_FB_adc_config_channel_5 -1
#define INX_FB_adc_config_channel_6 -1
#define INX_FB_adc_config_channel_7 -1
#define INX_FB_adc_config_channel_8 -1
#define INX_FB_adc_config_channel_9 -1
#define INX_FB_adc_config_channel_10 -1
#define INX_FB_adc_config_channel_11 -1
#define INX_FB_adc_config_channel_12 -1
#define INX_FB_adc_config_channel_13 -1
#define INX_FB_adc_config_channel_14 -1
#define INX_FB_adc_config_channel_15 -1
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(adc_config)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_sint32 device_id;
	ehs_sint32 f_s_hz;
	ehs_bool mode;
	ehs_sint32 channel_0;
	ehs_sint32 channel_1;
	ehs_sint32 channel_2;
	ehs_sint32 channel_3;
	ehs_sint32 channel_4;
	ehs_sint32 channel_5;
	ehs_sint32 channel_6;
	ehs_sint32 channel_7;
	ehs_sint32 channel_8;
	ehs_sint32 channel_9;
	ehs_sint32 channel_10;
	ehs_sint32 channel_11;
	ehs_sint32 channel_12;
	ehs_sint32 channel_13;
	ehs_sint32 channel_14;
	ehs_sint32 channel_15;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",&device_id,&f_s_hz,&mode,&channel_0,&channel_1,&channel_2,&channel_3,&channel_4,&channel_5,&channel_6,&channel_7,&channel_8,&channel_9,&channel_10,&channel_11,&channel_12,&channel_13,&channel_14,&channel_15); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_adc_config_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(adc_config)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	ehs_uint16 _device_id = 0;
	ehs_uint16 mode;
	ehs_sint16 channel[16];
	//this is the reference to the object data for this instance of the function block
	inx_adc_config_state_type* inx_adc_config_state = (inx_adc_config_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"%hu %u %hu %hd %hd %hd %hd %hd %hd %hd %hd %hd %hd %hd %hd %hd %hd %hd %hd",
		&_device_id,
		&inx_adc_config_state->f_s_hz,
		&mode,
		&channel[0],
		&channel[1],
		&channel[2],
		&channel[3],
		&channel[4],
		&channel[5],
		&channel[6],
		&channel[7],
		&channel[8],
		&channel[9],
		&channel[10],
		&channel[11],
		&channel[12],
		&channel[13],
		&channel[14],
		&channel[15]);
	inx_adc_config_state->device_id = (ehs_uint8)_device_id;
	inx_adc_config_state->mode = (ehs_bool)mode;
	inx_adc_config_state->channel_0 = channel[0];
	inx_adc_config_state->channel_1 = channel[1];
	inx_adc_config_state->channel_2 = channel[2];
	inx_adc_config_state->channel_3 = channel[3];
	inx_adc_config_state->channel_4 = channel[4];
	inx_adc_config_state->channel_5 = channel[5];
	inx_adc_config_state->channel_6 = channel[6];
	inx_adc_config_state->channel_7 = channel[7];
	inx_adc_config_state->channel_8 = channel[8];
	inx_adc_config_state->channel_9 = channel[9];
	inx_adc_config_state->channel_10 = channel[10];
	inx_adc_config_state->channel_11 = channel[11];
	inx_adc_config_state->channel_12 = channel[12];
	inx_adc_config_state->channel_13 = channel[13];
	inx_adc_config_state->channel_14 = channel[14];
	inx_adc_config_state->channel_15 = channel[15];

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(adc_config)
{
	inx_adc_config_state_type *inx_adc_config_state = (inx_adc_config_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
	EhsTAdcUnitDestroy(inx_adc_config_state->device_id);
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION config MACRO START -- DO NOT ALTER
/**
 * Definition of adc_config_config.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(adc_config_config)
{
	inx_adc_config_state_type* inx_adc_config_state = (inx_adc_config_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	//TODO parse JSON and get the parameters
	if (EHS_FB_IN_CONNECTED_API2(INX_adc_config_ARG_config_JSON))
	{
		// EHS_FB_IN_S_API2(INX_adc_config_ARG_config_JSON) ;
		if (inx_adc_config_state->device_id >= EHS_TARGET_ADC_UNIT_NUMBER)
		{
			EHS_FB_FINISH(INX_adc_config_ARG_config_error);
			return;
		}
		if (g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.init != 0)
		{
			EHS_FB_FINISH(INX_adc_config_ARG_config_error);
			return;
		}
		if (inx_adc_config_state->f_s_hz > EHS_TARGET_ADC_FREQ_MAX || inx_adc_config_state->f_s_hz < EHS_TARGET_ADC_FREQ_MIN)
		{
			EHS_FB_FINISH(INX_adc_config_ARG_config_error);
			return;
		}
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.f_s_hz = inx_adc_config_state->f_s_hz;
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.mode = inx_adc_config_state->mode;
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[0] = inx_adc_config_state->channel_0;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 2
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[1] = inx_adc_config_state->channel_1;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 3
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[2] = inx_adc_config_state->channel_2;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 4
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[3] = inx_adc_config_state->channel_3;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 5
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[4] = inx_adc_config_state->channel_4;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 6
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[5] = inx_adc_config_state->channel_5;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 7
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[6] = inx_adc_config_state->channel_6;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 8
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[7] = inx_adc_config_state->channel_7;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 9
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[8] = inx_adc_config_state->channel_8;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 10
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[9] = inx_adc_config_state->channel_9;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 11
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[10] = inx_adc_config_state->channel_10;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 12
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[11] = inx_adc_config_state->channel_11;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 13
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[12] = inx_adc_config_state->channel_12;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 14
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[13] = inx_adc_config_state->channel_13;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 15
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[14] = inx_adc_config_state->channel_14;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER == 16
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[15] = inx_adc_config_state->channel_15;
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 16
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 15
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 14
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 13
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 12
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 11
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 10
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 9
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 8
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 7
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 6
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 5
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 4
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 3
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 2
	}
	else
	{
		if (inx_adc_config_state->device_id >= EHS_TARGET_ADC_UNIT_NUMBER)
		{
			EHS_FB_FINISH(INX_adc_config_ARG_config_error);
			return;
		}
		if (g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.init != 0)
		{
			EHS_FB_FINISH(INX_adc_config_ARG_config_error);
			return;
		}
		if (inx_adc_config_state->f_s_hz > EHS_TARGET_ADC_FREQ_MAX || inx_adc_config_state->f_s_hz < EHS_TARGET_ADC_FREQ_MIN)
		{
			EHS_FB_FINISH(INX_adc_config_ARG_config_error);
			return;
		}
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.f_s_hz = inx_adc_config_state->f_s_hz;
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.mode = inx_adc_config_state->mode;
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[0] = inx_adc_config_state->channel_0;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 2
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[1] = inx_adc_config_state->channel_1;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 3
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[2] = inx_adc_config_state->channel_2;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 4
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[3] = inx_adc_config_state->channel_3;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 5
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[4] = inx_adc_config_state->channel_4;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 6
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[5] = inx_adc_config_state->channel_5;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 7
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[6] = inx_adc_config_state->channel_6;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 8
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[7] = inx_adc_config_state->channel_7;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 9
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[8] = inx_adc_config_state->channel_8;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 10
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[9] = inx_adc_config_state->channel_9;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 11
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[10] = inx_adc_config_state->channel_10;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 12
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[11] = inx_adc_config_state->channel_11;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 13
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[12] = inx_adc_config_state->channel_12;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 14
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[13] = inx_adc_config_state->channel_13;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER >= 15
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[14] = inx_adc_config_state->channel_14;
		#if EHS_TARGET_ADC_CHANNEL_NUMBER == 16
		g_ehs_adc_configs[inx_adc_config_state->device_id].unit_config.channel[15] = inx_adc_config_state->channel_15;
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 16
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 15
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 14
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 13
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 12
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 11
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 10
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 9
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 8
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 7
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 6
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 5
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 4
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 3
		#endif//EHS_TARGET_ADC_CHANNEL_NUMBER >= 2
	}

	//TODO configure the ADC according to the configuration here
	EhsTAdcUnitConfigure(inx_adc_config_state->device_id);

	EHS_FB_FINISH(INX_adc_config_ARG_config_finish);
}//ICB FUNCTION config MACRO END -- DO NOT ALTER THIS LINE
