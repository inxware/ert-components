//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-pwm_gen.h"
#include "hal_pwm.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_pwm_gen_state
{
    ehs_sint32 channel;
    ehs_sint32 io_num;
    ehs_sint32 max_duty;
    ehs_sint32 frequency;
    inx_hw_pwm_t *pwm_obj;
} inx_pwm_gen_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(pwm_gen)
EHS_FB_FUNCTION_ENTRY("setup", 0x01, pwm_gen_setup)
EHS_FB_FUNCTION_ENTRY("enable", 0x02, pwm_gen_enable)
EHS_FB_FUNCTION_ENTRY("disable", 0x03, pwm_gen_disable)
EHS_FB_FUNCTION_ENTRY("set", 0x04, pwm_gen_set)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_pwm_gen_ARG_setup_frequency 1
#define INX_pwm_gen_ARG_setup_max_val 2
#define INX_pwm_gen_ARG_setup_setup_errno 1
#define INX_pwm_gen_ARG_setup_setup_done 1
#define INX_pwm_gen_ARG_setup_setup_error 2
#define INX_pwm_gen_ARG_enable_enabled 1
#define INX_pwm_gen_ARG_disable_disabled 1
#define INX_pwm_gen_ARG_set_duty_val 1
#define INX_pwm_gen_ARG_set_set_duty_errno 1
#define INX_pwm_gen_ARG_set_set_duty_done 1
#define INX_pwm_gen_ARG_set_set_duty_err 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_pwm_gen_channel 0
#define INX_FB_pwm_gen_io_num 0
#define INX_FB_pwm_gen_max_duty 100
#define INX_FB_pwm_gen_frequency 1000
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(pwm_gen)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
    ehs_sint32 channel;
    ehs_sint32 io_num;
    ehs_sint32 max_duty;
    ehs_sint32 frequency;
    EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d %d %d %d",&channel,&io_num,&max_duty,&frequency); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_pwm_gen_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(pwm_gen)
{
    ehs_sint32 channel;
    ehs_sint32 io_num;
    ehs_sint32 max_duty;
    ehs_sint32 frequency;
    ehs_bool bRet = EHS_TRUE; /* assume success */
    //this is the reference to the object data for this instance of the function block
    inx_pwm_gen_state_type* inx_pwm_gen_state = (inx_pwm_gen_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d %d %d %d",&inx_pwm_gen_state->channel,&inx_pwm_gen_state->io_num,&inx_pwm_gen_state->max_duty, &inx_pwm_gen_state->frequency);
    inx_pwm_gen_state->pwm_obj = NULL;

    /* Add any further intialisation code here */
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(pwm_gen)
{
    inx_pwm_gen_state_type *inx_pwm_gen_state = (inx_pwm_gen_state_type*)EHS_FB_DESTROY_CONTEXT;
    //Your code below here
    EhsTPwmDeinit(inx_pwm_gen_state->pwm_obj);
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION setup MACRO START -- DO NOT ALTER
/**
 * Definition of pwm_gen_setup.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(pwm_gen_setup)
{
    inx_pwm_gen_state_type* inx_pwm_gen_state = (inx_pwm_gen_state_type*)EHS_FB_RUN_CONTEXT;
    ehs_sint32 freq = inx_pwm_gen_state->frequency;
    ehs_sint32 channel = inx_pwm_gen_state->channel;
    ehs_sint32 io_num = inx_pwm_gen_state->io_num;
    ehs_sint32 max_duty = inx_pwm_gen_state->max_duty;
    ehs_sint32 err_no = 0;

    // Your code here
    if (EHS_FB_IN_CONNECTED_API2(INX_pwm_gen_ARG_setup_frequency))
        freq = EHS_FB_IN_I_API2(INX_pwm_gen_ARG_setup_frequency) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_pwm_gen_ARG_setup_max_val))
        max_duty = EHS_FB_IN_I_API2(INX_pwm_gen_ARG_setup_max_val) ;
	// Setup the PWM controller
	err_no = EhsTPwmSetup(channel, io_num, freq, max_duty, 1, &inx_pwm_gen_state->pwm_obj);
    if (EHS_FB_OUT_CONNECTED_API2(INX_pwm_gen_ARG_setup_setup_errno))
        EHS_FB_OUT_I_API2(INX_pwm_gen_ARG_setup_setup_errno) = err_no;
    EHS_FB_FINISH(INX_pwm_gen_ARG_setup_setup_done);
    if (err_no != 0) EHS_FB_FINISH(INX_pwm_gen_ARG_setup_setup_error);
}//ICB FUNCTION setup MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION enable MACRO START -- DO NOT ALTER
/**
 * Definition of pwm_gen_enable.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(pwm_gen_enable)
{
    inx_pwm_gen_state_type* inx_pwm_gen_state = (inx_pwm_gen_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
	EhsTPwmEnable(inx_pwm_gen_state->pwm_obj);
    EHS_FB_FINISH(INX_pwm_gen_ARG_enable_enabled);
}//ICB FUNCTION enable MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION disable MACRO START -- DO NOT ALTER
/**
 * Definition of pwm_gen_disable.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(pwm_gen_disable)
{
    inx_pwm_gen_state_type* inx_pwm_gen_state = (inx_pwm_gen_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
	EhsTPwmDisable(inx_pwm_gen_state->pwm_obj);
    EHS_FB_FINISH(INX_pwm_gen_ARG_disable_disabled);
}//ICB FUNCTION disable MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION set MACRO START -- DO NOT ALTER
/**
 * Definition of pwm_gen_set.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(pwm_gen_set)
{
    inx_pwm_gen_state_type* inx_pwm_gen_state = (inx_pwm_gen_state_type*)EHS_FB_RUN_CONTEXT;
	ehs_sint32 err_no = 0;
	ehs_sint32 duty = 0;

    // Your code here
    if (EHS_FB_IN_CONNECTED_API2(INX_pwm_gen_ARG_set_duty_val))
        duty = EHS_FB_IN_I_API2(INX_pwm_gen_ARG_set_duty_val) ;
	else
	{
		err_no = 1;
		goto function_end;
	}
	if (duty < 0)
	{
		err_no = 2;
		goto function_end;
	}
	err_no = EhsTPwmSetDuty(inx_pwm_gen_state->pwm_obj, (ehs_uint32) duty);
function_end:
    if (EHS_FB_OUT_CONNECTED_API2(INX_pwm_gen_ARG_set_set_duty_errno))
        EHS_FB_OUT_I_API2(INX_pwm_gen_ARG_set_set_duty_errno) = err_no;
    EHS_FB_FINISH(INX_pwm_gen_ARG_set_set_duty_done);
    if (err_no != 0) EHS_FB_FINISH(INX_pwm_gen_ARG_set_set_duty_err);
}//ICB FUNCTION set MACRO END -- DO NOT ALTER THIS LINE
