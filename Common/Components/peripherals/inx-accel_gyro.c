//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-accel_gyro.h"
#include "hal_accel_gyro.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_accel_gyro_state
{
} inx_accel_gyro_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(accel_gyro)
EHS_FB_FUNCTION_ENTRY("EnableFunc", 0x01, accel_gyro_EnableFunc)
EHS_FB_FUNCTION_ENTRY("DisableFunc", 0x02, accel_gyro_DisableFunc)
EHS_FB_FUNCTION_ENTRY("ReadAccelFunc", 0x03, accel_gyro_ReadAccelFunc)
EHS_FB_FUNCTION_ENTRY("ReadGyroFunc", 0x04, accel_gyro_ReadGyroFunc)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_accel_gyro_ARG_EnableFunc_error_id 1
#define INX_accel_gyro_ARG_EnableFunc_enabled 1
#define INX_accel_gyro_ARG_DisableFunc_error_id 1
#define INX_accel_gyro_ARG_DisableFunc_disabled 1
#define INX_accel_gyro_ARG_ReadAccelFunc_Ax 1
#define INX_accel_gyro_ARG_ReadAccelFunc_Ay 2
#define INX_accel_gyro_ARG_ReadAccelFunc_Az 3
#define INX_accel_gyro_ARG_ReadAccelFunc_accel_done 1
#define INX_accel_gyro_ARG_ReadGyroFunc_Gx 1
#define INX_accel_gyro_ARG_ReadGyroFunc_Gy 2
#define INX_accel_gyro_ARG_ReadGyroFunc_Gz 3
#define INX_accel_gyro_ARG_ReadGyroFunc_gyro_done 1
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
EHS_FB_IDENTIFY_FUNCTION(accel_gyro)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_accel_gyro_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(accel_gyro)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	//inx_accel_gyro_state_type* inx_accel_gyro_state = (inx_accel_gyro_state_type*)EHS_FB_INIT_CONTEXT;

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(accel_gyro)
{
	//inx_accel_gyro_state_type *inx_accel_gyro_state = (inx_accel_gyro_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
	DisableAccelometerGyrosope();
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION EnableFunc MACRO START -- DO NOT ALTER
/**
 * Definition of accel_gyro_EnableFunc.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(accel_gyro_EnableFunc)
{
	//inx_accel_gyro_state_type* inx_accel_gyro_state = (inx_accel_gyro_state_type*)EHS_FB_RUN_CONTEXT;

	int error_id = 0;
	if(EnableAccelometerGyrosope() == EHS_FALSE){
		error_id = -1;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_accel_gyro_ARG_EnableFunc_error_id)){
		EHS_FB_OUT_I_API2(INX_accel_gyro_ARG_EnableFunc_error_id) = error_id;
	}
	if(error_id == 0){
		EHS_FB_FINISH(INX_accel_gyro_ARG_EnableFunc_enabled);
	}
}//ICB FUNCTION EnableFunc MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION DisableFunc MACRO START -- DO NOT ALTER
/**
 * Definition of accel_gyro_DisableFunc.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(accel_gyro_DisableFunc)
{
	//inx_accel_gyro_state_type* inx_accel_gyro_state = (inx_accel_gyro_state_type*)EHS_FB_RUN_CONTEXT;

	DisableAccelometerGyrosope();

	if (EHS_FB_OUT_CONNECTED_API2(INX_accel_gyro_ARG_DisableFunc_error_id)){
		EHS_FB_OUT_I_API2(INX_accel_gyro_ARG_DisableFunc_error_id) = 0;
	}
	EHS_FB_FINISH(INX_accel_gyro_ARG_DisableFunc_disabled);
}//ICB FUNCTION DisableFunc MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION ReadAccelFunc MACRO START -- DO NOT ALTER
/**
 * Definition of accel_gyro_ReadAccelFunc.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(accel_gyro_ReadAccelFunc)
{
	//inx_accel_gyro_state_type* inx_accel_gyro_state = (inx_accel_gyro_state_type*)EHS_FB_RUN_CONTEXT;
	ehs_float x, y, z;
	if(EhsGetAccelometerValues(&x, &y, &z) == EHS_TRUE){
		if (EHS_FB_OUT_CONNECTED_API2(INX_accel_gyro_ARG_ReadAccelFunc_Ax)){
			EHS_FB_OUT_F_API2(INX_accel_gyro_ARG_ReadAccelFunc_Ax) = x;
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_accel_gyro_ARG_ReadAccelFunc_Ay)){
			EHS_FB_OUT_F_API2(INX_accel_gyro_ARG_ReadAccelFunc_Ay) = y;
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_accel_gyro_ARG_ReadAccelFunc_Az)){
			EHS_FB_OUT_F_API2(INX_accel_gyro_ARG_ReadAccelFunc_Az) = z;
		}
		EHS_FB_FINISH(INX_accel_gyro_ARG_ReadAccelFunc_accel_done);
	}
}//ICB FUNCTION ReadAccelFunc MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION ReadGyroFunc MACRO START -- DO NOT ALTER
/**
 * Definition of accel_gyro_ReadGyroFunc.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(accel_gyro_ReadGyroFunc)
{
	//inx_accel_gyro_state_type* inx_accel_gyro_state = (inx_accel_gyro_state_type*)EHS_FB_RUN_CONTEXT;
	ehs_float x, y, z;

	if(EhsGetGyrosopeValues(&x, &y, &z) == EHS_TRUE){
		if (EHS_FB_OUT_CONNECTED_API2(INX_accel_gyro_ARG_ReadGyroFunc_Gx)){
			EHS_FB_OUT_F_API2(INX_accel_gyro_ARG_ReadGyroFunc_Gx) = x;
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_accel_gyro_ARG_ReadGyroFunc_Gy)){
			EHS_FB_OUT_F_API2(INX_accel_gyro_ARG_ReadGyroFunc_Gy) = y;
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_accel_gyro_ARG_ReadGyroFunc_Gz)){
			EHS_FB_OUT_F_API2(INX_accel_gyro_ARG_ReadGyroFunc_Gz) = z;
		}
		EHS_FB_FINISH(INX_accel_gyro_ARG_ReadGyroFunc_gyro_done);
	}
}//ICB FUNCTION ReadGyroFunc MACRO END -- DO NOT ALTER THIS LINE
