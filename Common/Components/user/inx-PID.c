//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-PID.h"
//ICB HEADER MACRO END -- DO NOT ALTER
#include "hal_pid.h"
#include "hal_logger.h"

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(PID)
EHS_FB_FUNCTION_ENTRY("init", 0x01, PID_init)
EHS_FB_FUNCTION_ENTRY("new_measure", 0x02, PID_new_measure)
EHS_FB_FUNCTION_ENTRY("set_point_value", 0x03, PID_set_point_value)
EHS_FB_FUNCTION_ENTRY("set_pid_config", 0x04, PID_set_pid_config)
EHS_FB_FUNCTION_ENTRY("set_io_config", 0x05, PID_set_io_config)
EHS_FB_FUNCTION_ENTRY("get_isr_value", 0x06, PID_get_isr_value)
EHS_FB_FUNCTION_ENTRY("get_ctrl_value", 0x07, PID_get_ctrl_value)
EHS_FB_FUNCTION_ENTRY("disable_ctrl", 0x08, PID_disable_ctrl)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_PID_ARG_init_isr_mode 1
#define INX_PID_ARG_init_calib 2
#define INX_PID_ARG_init_init_done 1
#define INX_PID_ARG_new_measure_new_measured_val 1
#define INX_PID_ARG_new_measure_measuerd_val 1
#define INX_PID_ARG_new_measure_connected 2
#define INX_PID_ARG_new_measure_new_measuerd_done 1
#define INX_PID_ARG_new_measure_measured_done 2
#define INX_PID_ARG_set_point_value_set_point_val 1
#define INX_PID_ARG_set_point_value_set_point_done 1
#define INX_PID_ARG_set_pid_config_p_coeff 1
#define INX_PID_ARG_set_pid_config_i_coeff 2
#define INX_PID_ARG_set_pid_config_d_coeff 3
#define INX_PID_ARG_set_pid_config_max_i_val 4
#define INX_PID_ARG_set_pid_config_max_global_val 5
#define INX_PID_ARG_set_pid_config_pid_config_done 1
#define INX_PID_ARG_set_io_config_sensor_id 1
#define INX_PID_ARG_set_io_config_output_mode 2
#define INX_PID_ARG_set_io_config_output1 3
#define INX_PID_ARG_set_io_config_output2 4
#define INX_PID_ARG_set_io_config_output3 5
#define INX_PID_ARG_set_io_config_relay_check 6
#define INX_PID_ARG_set_io_config_io_config_done 1
#define INX_PID_ARG_get_isr_value_measuerd_val 1
#define INX_PID_ARG_get_isr_value_connected 2
#define INX_PID_ARG_get_isr_value_measured_done 1
#define INX_PID_ARG_get_ctrl_value_perc_out_val 1
#define INX_PID_ARG_get_ctrl_value_pwm1 2
#define INX_PID_ARG_get_ctrl_value_pwm2 3
#define INX_PID_ARG_get_ctrl_value_pwm3 4
#define INX_PID_ARG_get_ctrl_value_ctrl_done 1
#define INX_PID_ARG_disable_ctrl_disable_state 1
#define INX_PID_ARG_disable_ctrl_disable_done 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER

//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(PID)
{
	EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_pid_ctrl_type);
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
	ehs_bool bRet = EHS_TRUE; /* assume success */
	
	//this is the reference to the object data for this instance of the function block
	ehs_pid_ctrl_type* pid_ctrl = (ehs_pid_ctrl_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
	const char* pParams = EHS_FB_INIT_PARAMETERS;
	pParams = EhsGetSint32FromString(&(pid_ctrl->pid_no), pParams);
	pParams = EhsGetUint8FromString(&(pid_ctrl->ISR_mode), pParams);
	pParams = EhsGetSint32FromString(&(pid_ctrl->nSensorID), pParams);
	pParams = EhsGetDoubleFromString(&(pid_ctrl->P), pParams);
	pParams = EhsGetDoubleFromString(&(pid_ctrl->I), pParams);
	pParams = EhsGetDoubleFromString(&(pid_ctrl->D), pParams);
	pParams = EhsGetSint32FromString(&(pid_ctrl->nOutputMode), pParams);
	pParams = EhsGetSint32FromString(&(pid_ctrl->nChannelOutput1), pParams);
	pParams = EhsGetSint32FromString(&(pid_ctrl->nChannelOutput2), pParams);
	pParams = EhsGetSint32FromString(&(pid_ctrl->nChannelOutput3), pParams);
	pParams = EhsGetUint8FromString(&(pid_ctrl->bDisableControl), pParams);
	pParams = EhsGetUint8FromString(&(pid_ctrl->calib_mode), pParams);

	pid_ctrl->setpointValue = 0;
	pid_ctrl->measuredValue = 0;
	pid_ctrl->bEnableRelayCheck = EHS_FALSE;
	pid_ctrl->maxGlobalValue = 1000;
	pid_ctrl->maxIValue = 20;
	pid_ctrl->nOutputPercent = 0;
	pid_ctrl->bPWMOut1 = EHS_FALSE;
	pid_ctrl->bPWMOut2 = EHS_FALSE;
	pid_ctrl->bPWMOut3 = EHS_FALSE;
	pid_ctrl->nError = PID_CTRL_NO_ERROR;

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(PID)
{
	ehs_pid_ctrl_type* pid_ctrl = (ehs_pid_ctrl_type*)EHS_FB_DESTROY_CONTEXT;
	
	EhsPIDCtrlDestroy(pid_ctrl);
	return EHS_TRUE; /* destroy functions return ehs_bool */
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION init MACRO START -- DO NOT ALTER
/**
 * Definition of PID_init.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PID_init)
{
	ehs_pid_ctrl_type* pid_ctrl = (ehs_pid_ctrl_type*)EHS_FB_RUN_CONTEXT;

	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_init_isr_mode)){
		pid_ctrl->ISR_mode = EHS_FB_IN_B_API2(INX_PID_ARG_init_isr_mode);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_init_calib)){
		pid_ctrl->calib_mode = EHS_FB_IN_B_API2(INX_PID_ARG_init_calib);
	}
	if( EhsPIDCtrlInit(pid_ctrl) == EHS_TRUE ){
		EHS_FB_FINISH(INX_PID_ARG_init_init_done);
	}else{
		EHSH_LOG_ERROR("Failed to init PID controller (error=%d)", (ehs_sint32)pid_ctrl->nError);
	}
}//ICB FUNCTION init MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION new_measure MACRO START -- DO NOT ALTER
/**
 * Definition of PID_new_measure.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PID_new_measure)
{
	ehs_pid_ctrl_type* pid_ctrl = (ehs_pid_ctrl_type*)EHS_FB_RUN_CONTEXT;

	if(pid_ctrl->ISR_mode == EHS_TRUE){
		EHSH_LOG_ERROR("Setting new measuered value in ISR mode is not supported.");
		return;
	}

	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_new_measure_new_measured_val)){
		pid_ctrl->measuredValue = EHS_FB_IN_F_API2(INX_PID_ARG_new_measure_new_measured_val);
		if(EhsPIDCtrlSetMeasuredValue(pid_ctrl) == EHS_TRUE){
			EHS_FB_FINISH(INX_PID_ARG_new_measure_new_measuerd_done);
			if (EHS_FB_OUT_CONNECTED_API2(INX_PID_ARG_new_measure_measuerd_val)){
				ehs_bool isConnected = EHS_FALSE;
				EHS_FB_OUT_F_API2(INX_PID_ARG_new_measure_measuerd_val) = EhsPIDCtrlGetSensorValue(pid_ctrl, &isConnected);
				EHS_FB_OUT_B_API2(INX_PID_ARG_new_measure_connected) = isConnected;
				EHS_FB_FINISH(INX_PID_ARG_new_measure_measured_done);
			}
		}
	}
}//ICB FUNCTION new_measure MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION set_point_value MACRO START -- DO NOT ALTER
/**
 * Definition of PID_set_point_value.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PID_set_point_value)
{
	ehs_pid_ctrl_type* pid_ctrl = (ehs_pid_ctrl_type*)EHS_FB_RUN_CONTEXT;

	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_point_value_set_point_val)){
		pid_ctrl->setpointValue = EHS_FB_IN_F_API2(INX_PID_ARG_set_point_value_set_point_val);
		if(EhsPIDCtrlSetSetpointValue(pid_ctrl) == EHS_TRUE){
			EHS_FB_FINISH(INX_PID_ARG_set_point_value_set_point_done);
		}else{
			EHSH_LOG_ERROR("EhsPIDCtrlSetSetpointValue configure FAILED ! ");
		}
	}
}//ICB FUNCTION set_point_value MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION set_pid_config MACRO START -- DO NOT ALTER
/**
 * Definition of PID_set_pid_config.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PID_set_pid_config)
{
	ehs_pid_ctrl_type* pid_ctrl = (ehs_pid_ctrl_type*)EHS_FB_RUN_CONTEXT;

	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_pid_config_p_coeff)){
		pid_ctrl->P = EHS_FB_IN_F_API2(INX_PID_ARG_set_pid_config_p_coeff);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_pid_config_i_coeff)){
		pid_ctrl->I = EHS_FB_IN_F_API2(INX_PID_ARG_set_pid_config_i_coeff);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_pid_config_d_coeff)){
		pid_ctrl->D = EHS_FB_IN_F_API2(INX_PID_ARG_set_pid_config_d_coeff);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_pid_config_max_i_val)){
		pid_ctrl->maxIValue = EHS_FB_IN_F_API2(INX_PID_ARG_set_pid_config_max_i_val);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_pid_config_max_global_val)){
		pid_ctrl->maxGlobalValue = EHS_FB_IN_F_API2(INX_PID_ARG_set_pid_config_max_global_val);
	}
	if(EhsPIDCtrlConfigurePID(pid_ctrl) == EHS_TRUE){
		EHS_FB_FINISH(INX_PID_ARG_set_pid_config_pid_config_done);
	}else{
		EHSH_LOG_ERROR("EhsPIDCtrlConfigurePID configure FAILED ! ");
	}
}//ICB FUNCTION set_pid_config MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION set_io_config MACRO START -- DO NOT ALTER
/**
 * Definition of PID_set_io_config.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PID_set_io_config)
{
	ehs_pid_ctrl_type* pid_ctrl = (ehs_pid_ctrl_type*)EHS_FB_RUN_CONTEXT;

	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_io_config_sensor_id)){
		pid_ctrl->nSensorID = EHS_FB_IN_I_API2(INX_PID_ARG_set_io_config_sensor_id);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_io_config_output_mode)){
		pid_ctrl->nOutputMode = EHS_FB_IN_I_API2(INX_PID_ARG_set_io_config_output_mode);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_io_config_output1)){
		pid_ctrl->nChannelOutput1 = EHS_FB_IN_I_API2(INX_PID_ARG_set_io_config_output1);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_io_config_output2)){
		pid_ctrl->nChannelOutput2 = EHS_FB_IN_I_API2(INX_PID_ARG_set_io_config_output2);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_io_config_output3)){
		pid_ctrl->nChannelOutput3 = EHS_FB_IN_I_API2(INX_PID_ARG_set_io_config_output3);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_set_io_config_relay_check)){
		pid_ctrl->bEnableRelayCheck = EHS_FB_IN_B_API2(INX_PID_ARG_set_io_config_relay_check) ;
	}
	if(EhsPIDCtrlConfigureIO(pid_ctrl) == EHS_TRUE){
		EHS_FB_FINISH(INX_PID_ARG_set_io_config_io_config_done);
	}else{
		EHSH_LOG_ERROR("EhsPIDCtrlConfigureIO configure FAILED ! ");
	}
}//ICB FUNCTION set_io_config MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION get_isr_value MACRO START -- DO NOT ALTER
/**
 * Definition of PID_get_isr_value.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PID_get_isr_value)
{
	ehs_pid_ctrl_type* pid_ctrl = (ehs_pid_ctrl_type*)EHS_FB_RUN_CONTEXT;

	if(pid_ctrl->ISR_mode == EHS_FALSE){
		EHSH_LOG_ERROR("Getting measuered value in non-ISR mode is not supported.");
		return;
	}

	if (EHS_FB_OUT_CONNECTED_API2(INX_PID_ARG_get_isr_value_measuerd_val)){
		ehs_bool isConnected = EHS_FALSE;
		EHS_FB_OUT_F_API2(INX_PID_ARG_get_isr_value_measuerd_val) = EhsPIDCtrlGetSensorValue(pid_ctrl, &isConnected);
		EHS_FB_OUT_B_API2(INX_PID_ARG_get_isr_value_connected) = isConnected;
		EHS_FB_FINISH(INX_PID_ARG_get_isr_value_measured_done);
	}
}//ICB FUNCTION get_isr_value MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION get_ctrl_value MACRO START -- DO NOT ALTER
/**
 * Definition of PID_get_ctrl_value.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PID_get_ctrl_value)
{
	ehs_pid_ctrl_type* pid_ctrl = (ehs_pid_ctrl_type*)EHS_FB_RUN_CONTEXT;
	ehs_bool bUpdated = EHS_FALSE;

	if(EhsPIDCtrlUpdateOutputs(pid_ctrl) == EHS_FALSE){
		EHSH_LOG_ERROR("Failed to updated PID (%d) output data.", pid_ctrl->pid_no);
		return;
	}

	if (EHS_FB_OUT_CONNECTED_API2(INX_PID_ARG_get_ctrl_value_perc_out_val)){
		EHS_FB_OUT_F_API2(INX_PID_ARG_get_ctrl_value_perc_out_val) = (ehs_float) pid_ctrl->nOutputPercent;
		bUpdated = EHS_TRUE;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_PID_ARG_get_ctrl_value_pwm1)){
		EHS_FB_OUT_B_API2(INX_PID_ARG_get_ctrl_value_pwm1) = pid_ctrl->bPWMOut1;
		bUpdated = EHS_TRUE;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_PID_ARG_get_ctrl_value_pwm2)){
		EHS_FB_OUT_B_API2(INX_PID_ARG_get_ctrl_value_pwm2) = pid_ctrl->bPWMOut2;
		bUpdated = EHS_TRUE;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_PID_ARG_get_ctrl_value_pwm3)){
		EHS_FB_OUT_B_API2(INX_PID_ARG_get_ctrl_value_pwm3) = pid_ctrl->bPWMOut3;
		bUpdated = EHS_TRUE;
	}
	if(bUpdated == EHS_TRUE){
		EHS_FB_FINISH(INX_PID_ARG_get_ctrl_value_ctrl_done);
	}
}//ICB FUNCTION get_ctrl_value MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION disable_ctrl MACRO START -- DO NOT ALTER
/**
 * Definition of PID_disable_ctrl.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PID_disable_ctrl)
{
	ehs_pid_ctrl_type* pid_ctrl = (ehs_pid_ctrl_type*)EHS_FB_RUN_CONTEXT;

	if (EHS_FB_IN_CONNECTED_API2(INX_PID_ARG_disable_ctrl_disable_state)){
		pid_ctrl->bDisableControl = EHS_FB_IN_B_API2(INX_PID_ARG_disable_ctrl_disable_state);
		EhsPIDCtrlDisable(pid_ctrl);
	}
	EHS_FB_FINISH(INX_PID_ARG_disable_ctrl_disable_done);
}//ICB FUNCTION disable_ctrl MACRO END -- DO NOT ALTER THIS LINE
