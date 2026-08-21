//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-pid_relay_config.h"
//ICB HEADER MACRO END -- DO NOT ALTER
#include "hal_pid.h"
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_pid_relay_config_state
{
	ehs_sint32 type_id;
} inx_pid_relay_config_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(pid_relay_config)
EHS_FB_FUNCTION_ENTRY("configure", 0x01, pid_relay_config_configure)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_pid_relay_config_ARG_configure_rel_1 1
#define INX_pid_relay_config_ARG_configure_rel_2 2
#define INX_pid_relay_config_ARG_configure_cyc_len_1 3
#define INX_pid_relay_config_ARG_configure_cyc_len_2 4
#define INX_pid_relay_config_ARG_configure_rel_3 5
#define INX_pid_relay_config_ARG_configure_cyc_len_3 6
#define INX_pid_relay_config_ARG_configure_config_done 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_pid_relay_config_type_id 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(pid_relay_config)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_sint32 type_id;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d",&type id); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_pid_relay_config_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(pid_relay_config)
{
	ehs_sint32 type_id;
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_pid_relay_config_state_type* inx_pid_relay_config_state = (inx_pid_relay_config_state_type*)EHS_FB_INIT_CONTEXT;

	/* read the initialisation parameters */
	const char* pParams = EHS_FB_INIT_PARAMETERS;
	pParams = EhsGetSint32FromString(&(inx_pid_relay_config_state->type_id), pParams);

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(pid_relay_config)
{
	inx_pid_relay_config_state_type *inx_pid_relay_config_state = (inx_pid_relay_config_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
	return EHS_TRUE; /* destroy functions return ehs_bool */
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION configure MACRO START -- DO NOT ALTER
/**
 * Definition of pid_relay_config_configure.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(pid_relay_config_configure)
{
	inx_pid_relay_config_state_type* inx_pid_relay_config_state = (inx_pid_relay_config_state_type*)EHS_FB_RUN_CONTEXT;
	ehs_sint32 type_id = inx_pid_relay_config_state->type_id;
	if(type_id > PID_CTRL_RELAY_TYPE_UNKOWN && type_id < PID_CTRL_RELAY_TYPE_COUNT){
		// make sure all relay data ports are connected and a correct relay type id is specified
		ehs_bool success = EHS_FALSE;
		if (EHS_FB_IN_CONNECTED_API2(INX_pid_relay_config_ARG_configure_rel_1) && EHS_FB_IN_CONNECTED_API2(INX_pid_relay_config_ARG_configure_cyc_len_1)){
			success = EhsPIDCtrlConfigureRelays((ehs_pid_ctrl_relay_type_id)type_id, 0, 
												EHS_FB_IN_I_API2(INX_pid_relay_config_ARG_configure_rel_1), 
												EHS_FB_IN_F_API2(INX_pid_relay_config_ARG_configure_cyc_len_1));
			if(success == EHS_FALSE) return;
								
		}
		if (EHS_FB_IN_CONNECTED_API2(INX_pid_relay_config_ARG_configure_rel_2) && EHS_FB_IN_CONNECTED_API2(INX_pid_relay_config_ARG_configure_cyc_len_2)){
			success = EhsPIDCtrlConfigureRelays((ehs_pid_ctrl_relay_type_id)type_id, 1, 
												EHS_FB_IN_I_API2(INX_pid_relay_config_ARG_configure_rel_2), 
												EHS_FB_IN_F_API2(INX_pid_relay_config_ARG_configure_cyc_len_2));
			if(success == EHS_FALSE) return;
		}
		if (EHS_FB_IN_CONNECTED_API2(INX_pid_relay_config_ARG_configure_rel_3) && EHS_FB_IN_CONNECTED_API2(INX_pid_relay_config_ARG_configure_cyc_len_3)){
			success = EhsPIDCtrlConfigureRelays((ehs_pid_ctrl_relay_type_id)type_id, 2, 
												EHS_FB_IN_I_API2(INX_pid_relay_config_ARG_configure_rel_3), 
												EHS_FB_IN_F_API2(INX_pid_relay_config_ARG_configure_cyc_len_3));
			if(success == EHS_FALSE) return;
		}
		if(success == EHS_TRUE){
			EHS_FB_FINISH(INX_pid_relay_config_ARG_configure_config_done);
		}
	}	
}//ICB FUNCTION configure MACRO END -- DO NOT ALTER THIS LINE
