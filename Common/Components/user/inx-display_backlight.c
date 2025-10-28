//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-display_backlight.h"
#include "target_display.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_Backlight_state
{
} inx_Backlight_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(Backlight)
EHS_FB_FUNCTION_ENTRY("set_intensity", 0x01, Backlight_set_intensity)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_Backlight_ARG_set_intensity_intensity 1
#define INX_Backlight_ARG_set_intensity_done 1
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
EHS_FB_IDENTIFY_FUNCTION(Backlight)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_Backlight_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(Backlight)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_Backlight_state_type* inx_Backlight_state = (inx_Backlight_state_type*)EHS_FB_INIT_CONTEXT;

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(Backlight)
{
	inx_Backlight_state_type *inx_Backlight_state = (inx_Backlight_state_type*)EHS_FB_DESTROY_CONTEXT;
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION set_intensity MACRO START -- DO NOT ALTER
/**
 * Definition of Backlight_set_intensity.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(Backlight_set_intensity)
{
	inx_Backlight_state_type* inx_Backlight_state = (inx_Backlight_state_type*)EHS_FB_RUN_CONTEXT;
	ehs_sint32 intensity = 0;

	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_Backlight_ARG_set_intensity_intensity))
	{
		intensity = EHS_FB_IN_I_API2(INX_Backlight_ARG_set_intensity_intensity);
		target_setBacklight(intensity > 100 ? 100 : intensity < 0 ? 0 : intensity) ;
	}
	EHS_FB_FINISH(INX_Backlight_ARG_set_intensity_done);
}//ICB FUNCTION set_intensity MACRO END -- DO NOT ALTER THIS LINE
