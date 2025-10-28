//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-led.h"
#include "hal_led.h"
//ICB HEADER MACRO END -- DO NOT ALTER

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_led_state
{
	ehs_uint8 id;
	ehs_bool state;
	ehs_uint8 brightness;
} inx_led_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(led)
EHS_FB_FUNCTION_ENTRY("EnableFunc", 0x01, led_EnableFunc)
EHS_FB_FUNCTION_ENTRY("DisableFunc", 0x02, led_DisableFunc)
EHS_FB_FUNCTION_ENTRY("SetFunc", 0x03, led_SetFunc)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_led_ARG_EnableFunc_error_id 1
#define INX_led_ARG_EnableFunc_enabled 1
#define INX_led_ARG_DisableFunc_error_id 1
#define INX_led_ARG_DisableFunc_disabled 1
#define INX_led_ARG_SetFunc_on_off 1
#define INX_led_ARG_SetFunc_brightness 2
#define INX_led_ARG_SetFunc_set_done 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER

//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(led)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_char* id;
	ehs_bool State;
	ehs_sint32 Brightness;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%s %d %d",&id,&State,&Brightness); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_led_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(led)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_led_state_type* inx_led_state = (inx_led_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
	const char* pParams = EHS_FB_INIT_PARAMETERS;
	pParams = (pParams) ? EhsGetUint8FromString(&inx_led_state->id, pParams) : pParams;
    pParams = (pParams) ? EhsGetUint8FromString(&inx_led_state->state, pParams) : pParams;
    pParams = (pParams) ? EhsGetUint8FromString(&inx_led_state->brightness, pParams) : pParams;

	/* Add any further intialisation code here */
	return EHS_TRUE; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(led)
{
	inx_led_state_type *inx_led_state = (inx_led_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
	EhsDisableLED(inx_led_state->id);
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION EnableFunc MACRO START -- DO NOT ALTER
/**
 * Definition of led_EnableFunc.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(led_EnableFunc)
{
	inx_led_state_type* inx_led_state = (inx_led_state_type*)EHS_FB_RUN_CONTEXT;
	ehs_sint32 err = 0;
	if(EhsEnableLED(inx_led_state->id) == EHS_TRUE){
		if(EhsSetLED(inx_led_state->id, inx_led_state->state, inx_led_state->brightness) == EHS_FALSE){
			err = -1;
		}
	}else{
		err = -1;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_led_ARG_EnableFunc_error_id)){
		EHS_FB_OUT_I_API2(INX_led_ARG_EnableFunc_error_id) = err;
	}
	EHS_FB_FINISH(INX_led_ARG_EnableFunc_enabled);
}//ICB FUNCTION EnableFunc MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION DisableFunc MACRO START -- DO NOT ALTER
/**
 * Definition of led_DisableFunc.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(led_DisableFunc)
{
	inx_led_state_type* inx_led_state = (inx_led_state_type*)EHS_FB_RUN_CONTEXT;

	ehs_sint32 err = 0;
	if(EhsDisableLED(inx_led_state->id) == EHS_FALSE){
		err = -1;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_led_ARG_DisableFunc_error_id)){
		EHS_FB_OUT_I_API2(INX_led_ARG_DisableFunc_error_id) = err;
	}
	EHS_FB_FINISH(INX_led_ARG_DisableFunc_disabled);
}//ICB FUNCTION DisableFunc MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION SetFunc MACRO START -- DO NOT ALTER
/**
 * Definition of led_SetFunc.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(led_SetFunc)
{
	inx_led_state_type* inx_led_state = (inx_led_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_led_ARG_SetFunc_on_off)){
		inx_led_state->state = EHS_FB_IN_B_API2(INX_led_ARG_SetFunc_on_off) ;
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_led_ARG_SetFunc_brightness)){
		inx_led_state->brightness = (ehs_uint8)EHS_FB_IN_I_API2(INX_led_ARG_SetFunc_brightness) ;
	}
	if(EhsSetLED(inx_led_state->id, inx_led_state->state, inx_led_state->brightness) == EHS_TRUE){
		EHS_FB_FINISH(INX_led_ARG_SetFunc_set_done);
	}
}//ICB FUNCTION SetFunc MACRO END -- DO NOT ALTER THIS LINE
