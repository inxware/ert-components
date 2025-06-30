/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-pwm.h"

#ifdef EHS_NXP_SUPPORT
// @TODO - do not use any target specific implemenation here
// fix how NXP target is structured ! 
#include "inxware_hrx_config.h"
#include "ehs_main.h" // we run th main from here!
#else
#include "hal_pwm_legacy.h"
#endif
//ICB HEADER MACRO END -- DO NOT ALTER

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_pwm_state
{
    ehs_uint8 pin;
    ehs_bool enabled;
    ehs_uint32 periodHz;
    ehs_uint8 powerPercent;
} inx_pwm_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(pwm)

EHS_FB_FUNCTION_ENTRY("config", 0x01, pwm_config)

EHS_FB_FUNCTION_ENTRY("enable", 0x02, pwm_enable)

EHS_FB_FUNCTION_ENTRY("duty", 0x03, pwm_duty)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_pwm_ARG_config_hz 1
#define INX_pwm_ARG_config_pin 2
#define INX_pwm_ARG_config_finishconfig 1
#define INX_pwm_ARG_enable_enable 1
#define INX_pwm_ARG_enable_finishenable 1
#define INX_pwm_ARG_duty_percent 1
#define INX_pwm_ARG_duty_finishduty 1
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
EHS_FB_IDENTIFY_FUNCTION(pwm)
{
    /* Uncomment the following if you need to parse the parameters to calculate memory required */
    /*
    	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_pwm_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(pwm)
{
    ehs_bool bRet = EHS_TRUE; /* assume success */

    //this is the reference to the object data for this instance of the function block

    inx_pwm_state_type* inx_pwm_state = (inx_pwm_state_type*)EHS_FB_INIT_CONTEXT;

    /* read the initialisation parameters */
    const char* pParams = EHS_FB_INIT_PARAMETERS;
    pParams = (pParams) ? EhsGetUint8FromString(&inx_pwm_state->pin, pParams) : pParams;
    pParams = (pParams) ? EhsGetUint8FromString(&inx_pwm_state->enabled, pParams) : pParams;
    pParams = (pParams) ? EhsGetUint32FromString(&inx_pwm_state->periodHz, pParams) : pParams;
    pParams = (pParams) ? EhsGetUint8FromString(&inx_pwm_state->powerPercent, pParams) : pParams;

    /* Add any further intialisation code here */
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(pwm)
{
    inx_pwm_state_type* inx_pwm_state = (inx_pwm_state_type*)EHS_FB_DESTROY_CONTEXT;
    EhsPWMEnable(inx_pwm_state->pin, EHS_FALSE);
    return EHS_TRUE;
}
//Your code below here
//}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION config MACRO START -- DO NOT ALTER
/**
 * Definition of pwm_config.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(pwm_config)
{
    inx_pwm_state_type* inx_pwm_state = (inx_pwm_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    if(EHS_FB_IN_CONNECTED_API2(INX_pwm_ARG_config_pin))
    {
        inx_pwm_state->pin=EHS_FB_IN_I_API2(INX_pwm_ARG_config_pin);
    }
    if(EHS_FB_IN_CONNECTED_API2(INX_pwm_ARG_config_hz)){
        inx_pwm_state->periodHz = EHS_FB_IN_I_API2(INX_pwm_ARG_config_hz);
    }
#ifdef EHS_NXP_SUPPORT
    // @TODO - do not use any target specific implemenation here
    // fix how NXP target is structured ! 
    inxPWMConfig(inx_pwm_state->periodHz,inx_pwm_state->pin);
    EHS_FB_FINISH(INX_pwm_ARG_config_finishconfig);
#else
    if(EhsPWMConfig(inx_pwm_state->pin, inx_pwm_state->periodHz) == EHS_TRUE){
        if(inx_pwm_state->enabled){
            if(EhsPWMEnable(inx_pwm_state->pin, inx_pwm_state->enabled)){
                EhsPWMDuty(inx_pwm_state->pin, inx_pwm_state->powerPercent);
            }
        }
        EHS_FB_FINISH(INX_pwm_ARG_config_finishconfig);
    }
#endif
}//ICB FUNCTION config MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION enable MACRO START -- DO NOT ALTER
/**
 * Definition of pwm_enable.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(pwm_enable)
{
#ifdef EHS_NXP_SUPPORT
    // @TODO - do not use any target specific implemenation here
    // fix how NXP target is structured ! 
    if(EHS_FB_IN_CONNECTED_API2(INX_pwm_ARG_enable_enable))
    {
        inxPWMEnable(EHS_FB_IN_B_API2(INX_pwm_ARG_enable_enable));
        EHS_FB_FINISH(INX_pwm_ARG_enable_finishenable);
    }
#else
    inx_pwm_state_type* inx_pwm_state = (inx_pwm_state_type*)EHS_FB_RUN_CONTEXT;
    if(EHS_FB_IN_CONNECTED_API2(INX_pwm_ARG_enable_enable))
    {
        inx_pwm_state->enabled = EHS_FB_IN_B_API2(INX_pwm_ARG_enable_enable);
        if(EhsPWMEnable(inx_pwm_state->pin, inx_pwm_state->enabled) == EHS_TRUE){
            EHS_FB_FINISH(INX_pwm_ARG_enable_finishenable);
        }
    }
#endif

}//ICB FUNCTION enable MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION duty MACRO START -- DO NOT ALTER
/**
 * Definition of pwm_duty.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(pwm_duty)
{
    inx_pwm_state_type* inx_pwm_state = (inx_pwm_state_type*)EHS_FB_RUN_CONTEXT;
#ifdef EHS_NXP_SUPPORT
    // @TODO - do not use any target specific implemenation here
    // fix how NXP target is structured ! 
    // Your code here
    if(EHS_FB_IN_CONNECTED_API2(INX_pwm_ARG_duty_percent))
    {
        inxPWMDuty(inx_pwm_state->pin,EHS_FB_IN_B_API2(INX_pwm_ARG_duty_percent));
    }
    EHS_FB_FINISH(INX_pwm_ARG_duty_finishduty);
#else
    if(EHS_FB_IN_CONNECTED_API2(INX_pwm_ARG_duty_percent))
    {
        inx_pwm_state->powerPercent = EHS_FB_IN_B_API2(INX_pwm_ARG_duty_percent);
        if(EhsPWMDuty(inx_pwm_state->pin, inx_pwm_state->powerPercent) == EHS_TRUE){
            EHS_FB_FINISH(INX_pwm_ARG_duty_finishduty);
        }
    }
#endif
}//ICB FUNCTION duty MACRO END -- DO NOT ALTER THIS LINE
