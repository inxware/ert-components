//ICB HEADER MACRO START -- DO NOT ALTER
#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-PID.h"
#include "hal_pid.h"
#include "hal_logger.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_PID_state
{
	EhsCallbackQueueType xQueue;
	EhsCallbackQueueEntryType xEntry;
	ehs_pid_ctrl_type pid_ctrl;
} inx_PID_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(PID)
EHS_FB_FUNCTION_ENTRY("set_value", 0x01, PID_set_value)
EHS_FB_FUNCTION_ENTRY("set_config", 0x02, PID_set_config)
EHS_FB_FUNCTION_ENTRY("control_cb", 0x03, PID_control_cb)
EHS_FB_FUNCTION_ENTRY("get_value", 0x04, PID_get_value)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_PID_ARG_set_value_setpoint 1
#define INX_PID_ARG_set_value_measurement 2
#define INX_PID_ARG_set_value_valueSetOK 1
#define INX_PID_ARG_set_config_configP 1
#define INX_PID_ARG_set_config_configI 2
#define INX_PID_ARG_set_config_configD 3
#define INX_PID_ARG_set_config_ChADC 4
#define INX_PID_ARG_set_config_EnRelayCheck 5
#define INX_PID_ARG_set_config_MaxGlobalValue 6
#define INX_PID_ARG_set_config_ChGPIO1 7
#define INX_PID_ARG_set_config_ChGPIO2 8
#define INX_PID_ARG_set_config_ChGPIO3 9
#define INX_PID_ARG_set_config_OutputMode 10
#define INX_PID_ARG_set_config_EnableControl 11
#define INX_PID_ARG_set_config_maxi 12
#define INX_PID_ARG_set_config_ConfigSetOK 1
#define INX_PID_ARG_control_cb_outInPercent 1
#define INX_PID_ARG_control_cb_errorValue 2
#define INX_PID_ARG_control_cb_PWM2Out 3
#define INX_PID_ARG_control_cb_PWM1Out 4
#define INX_PID_ARG_control_cb_PWM3Out 5
#define INX_PID_ARG_control_cb_control 1
#define INX_PID_ARG_get_value_adc_value 1
#define INX_PID_ARG_get_value_got_val 1
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
EHS_FB_IDENTIFY_FUNCTION(PID)
{
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_PID_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(PID)
{
	unsigned int PIDNo = 0;
	unsigned int ISR_Mode = 0;
	float P_Default = 0.0f;
	float I_Default = 0.0f;
	float D_Default = 0.0f;
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_PID_state_type* inx_PID_state = (inx_PID_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d %d %f %f %f", &PIDNo, &ISR_Mode, &P_Default, &I_Default, &D_Default);

	inx_PID_state->pid_ctrl.pid_no = (ehs_sint32)PIDNo;
	inx_PID_state->pid_ctrl.ISR_mode = (ehs_bool)ISR_Mode;
	inx_PID_state->pid_ctrl.target_data = NULL;
	inx_PID_state->pid_ctrl.nSetValue = 0;
	inx_PID_state->pid_ctrl.nMeasuredValue = 0;
	inx_PID_state->pid_ctrl.P = P_Default;
	inx_PID_state->pid_ctrl.I = I_Default;
	inx_PID_state->pid_ctrl.D = D_Default;
	inx_PID_state->pid_ctrl.bEnableRelayCheck = EHS_FALSE;
	inx_PID_state->pid_ctrl.nMaxGlobalValue = 4096; // default is a max value of the adc 12-bit (2^12) adc
	inx_PID_state->pid_ctrl.nMaxIValue = 200;

	// todo - should these have specific default values
	inx_PID_state->pid_ctrl.nChannelADC = -1;
    inx_PID_state->pid_ctrl.nChannelGPIOOut1 = 0;
    inx_PID_state->pid_ctrl.nChannelGPIOOut2 = 0;
    inx_PID_state->pid_ctrl.nChannelGPIOOut3 = 0;
	
	inx_PID_state->pid_ctrl.nOutputPercent = 0;
    inx_PID_state->pid_ctrl.bPWMOut1 = EHS_FALSE;
    inx_PID_state->pid_ctrl.bPWMOut2 = EHS_FALSE;
    inx_PID_state->pid_ctrl.bPWMOut3 = EHS_FALSE;
    inx_PID_state->pid_ctrl.nError = PID_CTRL_NO_ERROR;

	/* set up the callback queue to call the callback function */
	inx_PID_state->xQueue = NULL;
	EhsCallbackQueue_register(&(inx_PID_state->xQueue),
								EHS_FB_RUN_NAME(PID_control_cb),
								EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(-1),
								&(inx_PID_state->xEntry));

	/* tell the timer what to callback upon expiry */
	inx_PID_state->pid_ctrl.pCallback = &(inx_PID_state->xQueue);

	if( EhsInitPIDCtrl(&inx_PID_state->pid_ctrl) == EHS_FALSE ){
		EHSH_LOG_ERROR("Failed to init PID controller (error=%d)\n", (ehs_sint32)inx_PID_state->pid_ctrl.nError);
	}

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(PID)
{
	inx_PID_state_type *inx_PID_state = (inx_PID_state_type*)EHS_FB_DESTROY_CONTEXT;
	
	EhsDestroyPIDCtrl(&inx_PID_state->pid_ctrl);
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION set_value MACRO START -- DO NOT ALTER
/**
 * Definition of PID_set_value.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PID_set_value)
{
	inx_PID_state_type* inx_PID_state = (inx_PID_state_type*)EHS_FB_RUN_CONTEXT;

	ehs_bool bUpdate = EHS_FALSE;

	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_value_setpoint)){
		inx_PID_state->pid_ctrl.nSetValue = EHS_FB_IN_I_API2(INX_PID_ARG_set_value_setpoint);
		bUpdate = EHS_TRUE;
	}
	if(inx_PID_state->pid_ctrl.ISR_mode == EHS_FALSE){
		if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_value_measurement)) {
			inx_PID_state->pid_ctrl.nMeasuredValue = EHS_FB_IN_I_API2(INX_PID_ARG_set_value_measurement);
			bUpdate = EHS_TRUE;
		}
	}
	if(bUpdate == EHS_TRUE){
		ehs_bool ret = EhsSetMeasuredValuePIDCtrl(&inx_PID_state->pid_ctrl);
		if(ret == EHS_FALSE){
			// todo - output error
		}
	}
	
	EHS_FB_FINISH(INX_PID_ARG_set_value_valueSetOK);
}//ICB FUNCTION set_value MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION set_config MACRO START -- DO NOT ALTER
/**
 * Definition of PID_set_config.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PID_set_config)
{
	inx_PID_state_type* inx_PID_state = (inx_PID_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_config_configP)){
		inx_PID_state->pid_ctrl.P = EHS_FB_IN_F_API2(INX_PID_ARG_set_config_configP);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_config_configI)){
		inx_PID_state->pid_ctrl.I = EHS_FB_IN_F_API2(INX_PID_ARG_set_config_configI);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_config_configD)){
		inx_PID_state->pid_ctrl.D = EHS_FB_IN_F_API2(INX_PID_ARG_set_config_configD);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_config_ChADC)){
		inx_PID_state->pid_ctrl.nChannelADC = EHS_FB_IN_I_API2(INX_PID_ARG_set_config_ChADC);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_config_EnRelayCheck)){
		inx_PID_state->pid_ctrl.bEnableRelayCheck = EHS_FB_IN_B_API2(INX_PID_ARG_set_config_EnRelayCheck);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_config_MaxGlobalValue)){
		inx_PID_state->pid_ctrl.nMaxGlobalValue = EHS_FB_IN_I_API2(INX_PID_ARG_set_config_MaxGlobalValue);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_config_ChGPIO1)){
		inx_PID_state->pid_ctrl.nChannelGPIOOut1 = EHS_FB_IN_I_API2(INX_PID_ARG_set_config_ChGPIO1);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_config_ChGPIO2)){
		inx_PID_state->pid_ctrl.nChannelGPIOOut2 = EHS_FB_IN_I_API2(INX_PID_ARG_set_config_ChGPIO2);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_config_ChGPIO3)){
		inx_PID_state->pid_ctrl.nChannelGPIOOut3 = EHS_FB_IN_I_API2(INX_PID_ARG_set_config_ChGPIO3);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_config_OutputMode)){
		inx_PID_state->pid_ctrl.nOutputMode = EHS_FB_IN_I_API2(INX_PID_ARG_set_config_OutputMode);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_config_EnableControl)){
		inx_PID_state->pid_ctrl.bEnableControl = EHS_FB_IN_B_API2(INX_PID_ARG_set_config_EnableControl);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_config_maxi)){
		inx_PID_state->pid_ctrl.nMaxIValue = EHS_FB_IN_I_API2(INX_PID_ARG_set_config_maxi);
	}
	ehs_bool ret = EhsConfigurePIDCtrl(&inx_PID_state->pid_ctrl);
	if(ret == EHS_FALSE){
		// todo - output error
	}
	
	EHS_FB_FINISH(INX_PID_ARG_set_config_ConfigSetOK);
}//ICB FUNCTION set_config MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION control_cb MACRO START -- DO NOT ALTER
/**
 * Definition of PID_control_cb.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PID_control_cb)
{
	inx_PID_state_type* inx_PID_state = (inx_PID_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	if (EHS_FB_OUT_CONNECTED_API2(INX_PID_ARG_control_cb_outInPercent))
		EHS_FB_OUT_I_API2(INX_PID_ARG_control_cb_outInPercent) = inx_PID_state->pid_ctrl.nOutputPercent;
	if (EHS_FB_OUT_CONNECTED_API2(INX_PID_ARG_control_cb_errorValue))
		EHS_FB_OUT_I_API2(INX_PID_ARG_control_cb_errorValue) = (ehs_sint32)inx_PID_state->pid_ctrl.nError;
	if (EHS_FB_OUT_CONNECTED_API2(INX_PID_ARG_control_cb_PWM1Out))
		EHS_FB_OUT_B_API2(INX_PID_ARG_control_cb_PWM1Out) = inx_PID_state->pid_ctrl.bPWMOut1;
	if (EHS_FB_OUT_CONNECTED_API2(INX_PID_ARG_control_cb_PWM2Out))
		EHS_FB_OUT_B_API2(INX_PID_ARG_control_cb_PWM2Out) = inx_PID_state->pid_ctrl.bPWMOut2;
	if (EHS_FB_OUT_CONNECTED_API2(INX_PID_ARG_control_cb_PWM3Out))
		EHS_FB_OUT_B_API2(INX_PID_ARG_control_cb_PWM3Out) = inx_PID_state->pid_ctrl.bPWMOut3;
	EHS_FB_FINISH(INX_PID_ARG_control_cb_control);
}//ICB FUNCTION control_cb MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION get_value MACRO START -- DO NOT ALTER
/**
 * Definition of PID_get_value.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PID_get_value)
{
	inx_PID_state_type* inx_PID_state = (inx_PID_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	if (EHS_FB_OUT_CONNECTED_API2(INX_PID_ARG_get_value_adc_value)){
		EHS_FB_OUT_I_API2(INX_PID_ARG_get_value_adc_value) = EhsGetAdcValuePIDCtrl(&(inx_PID_state->pid_ctrl));
	}
	EHS_FB_FINISH(INX_PID_ARG_get_value_got_val);
}//ICB FUNCTION get_value MACRO END -- DO NOT ALTER THIS LINE
