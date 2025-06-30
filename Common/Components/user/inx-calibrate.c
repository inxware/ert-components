//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-calibrate.h"
//ICB HEADER MACRO END -- DO NOT ALTER
#include "hal_pid.h"
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_Calibrate_state
{
	ehs_sint32 id;
} inx_Calibrate_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(Calibrate)
EHS_FB_FUNCTION_ENTRY("calibrate", 0x01, Calibrate_calibrate)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_Calibrate_ARG_calibrate_scale 1
#define INX_Calibrate_ARG_calibrate_offset 2
#define INX_Calibrate_ARG_calibrate_errno 1
#define INX_Calibrate_ARG_calibrate_done 1
#define INX_Calibrate_ARG_calibrate_err 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_Calibrate_id -1
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(Calibrate)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_sint32 id;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d",&id); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_Calibrate_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(Calibrate)
{
	ehs_sint32 id;
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_Calibrate_state_type* inx_Calibrate_state = (inx_Calibrate_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d",&id);
	inx_Calibrate_state->id = id;
	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(Calibrate)
{
	//inx_Calibrate_state_type *inx_Calibrate_state = (inx_Calibrate_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION calibrate MACRO START -- DO NOT ALTER
/**
 * Definition of Calibrate_calibrate.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(Calibrate_calibrate)
{
	inx_Calibrate_state_type* inx_Calibrate_state = (inx_Calibrate_state_type*)EHS_FB_RUN_CONTEXT;

	ehs_sint32 nError = 0;
	float scale = 0.0f, offset = 0.0f;
	ehs_bool bScale = EHS_FALSE, bOffset = EHS_FALSE;

	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_Calibrate_ARG_calibrate_scale)){
		scale = EHS_FB_IN_F_API2(INX_Calibrate_ARG_calibrate_scale);
		bScale = EHS_TRUE;
	}
	
	if (EHS_FB_IN_CONNECTED_API2(INX_Calibrate_ARG_calibrate_offset)){
		offset = EHS_FB_IN_F_API2(INX_Calibrate_ARG_calibrate_offset);
		bOffset = EHS_TRUE;
	}

	nError = EhsPIDCtrlCalibrate(inx_Calibrate_state->id, bScale, scale, bOffset, offset);
	if(nError == 0){ // no error
		EHS_FB_FINISH(INX_Calibrate_ARG_calibrate_done);
	}else{
		if (EHS_FB_OUT_CONNECTED_API2(INX_Calibrate_ARG_calibrate_errno)){
			EHS_FB_OUT_I_API2(INX_Calibrate_ARG_calibrate_errno) = nError;
		}
		EHS_FB_FINISH(INX_Calibrate_ARG_calibrate_err);
	}
}//ICB FUNCTION calibrate MACRO END -- DO NOT ALTER THIS LINE
