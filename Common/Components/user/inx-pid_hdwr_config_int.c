//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-pid_hdwr_config_int.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_pid_hdwr_config_int_state
{
	ehs_sint32 id;
	ehs_sint32 value;
} inx_pid_hdwr_config_int_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(pid_hdwr_config_int)
EHS_FB_FUNCTION_ENTRY("configure", 0x01, pid_hdwr_config_int_configure)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_pid_hdwr_config_int_ARG_configure_id 1
#define INX_pid_hdwr_config_int_ARG_configure_value 2
#define INX_pid_hdwr_config_int_ARG_configure_errno 1
#define INX_pid_hdwr_config_int_ARG_configure_configured 1
#define INX_pid_hdwr_config_int_ARG_configure_err 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_pid_hdwr_config_int_id -1
#define INX_FB_pid_hdwr_config_int_value 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(pid_hdwr_config_int)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_sint32 id;
	ehs_sint32 value;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d %d",&id,&value); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_pid_hdwr_config_int_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(pid_hdwr_config_int)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_pid_hdwr_config_int_state_type* inx_pid_hdwr_config_int_state = (inx_pid_hdwr_config_int_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d %d",&(inx_pid_hdwr_config_int_state->id),&(inx_pid_hdwr_config_int_state->value));

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(pid_hdwr_config_int)
{
	inx_pid_hdwr_config_int_state_type *inx_pid_hdwr_config_int_state = (inx_pid_hdwr_config_int_state_type*)EHS_FB_DESTROY_CONTEXT;
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION configure MACRO START -- DO NOT ALTER
/**
 * Definition of pid_hdwr_config_int_configure.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(pid_hdwr_config_int_configure)
{
	inx_pid_hdwr_config_int_state_type* inx_pid_hdwr_config_int_state = (inx_pid_hdwr_config_int_state_type*)EHS_FB_RUN_CONTEXT;
	ehs_sint32 id = inx_pid_hdwr_config_int_state->id;
	ehs_sint32 value = inx_pid_hdwr_config_int_state->value;
	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_pid_hdwr_config_int_ARG_configure_id)){
		id = EHS_FB_IN_I_API2(INX_pid_hdwr_config_int_ARG_configure_id) ;
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_pid_hdwr_config_int_ARG_configure_value)){
		value = EHS_FB_IN_I_API2(INX_pid_hdwr_config_int_ARG_configure_value) ;
	}
	ehs_uint32 err = 0;
	if(EHS_FALSE == EhsPIDCtrlConfigureHardware(id, value, &err)){
		// failed
		if (EHS_FB_OUT_CONNECTED_API2(INX_pid_hdwr_config_int_ARG_configure_errno)){
			EHS_FB_OUT_I_API2(INX_pid_hdwr_config_int_ARG_configure_errno) = err;
		}
		EHS_FB_FINISH(INX_pid_hdwr_config_int_ARG_configure_err);
	}else{
		EHS_FB_FINISH(INX_pid_hdwr_config_int_ARG_configure_configured);
	}
}//ICB FUNCTION configure MACRO END -- DO NOT ALTER THIS LINE
