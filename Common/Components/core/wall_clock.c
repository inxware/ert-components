/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * wall_clock.c
 *
 * function declarations for wall_clock
 *
 * @author: inx limited
 *
 */


//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "ehs_fb_types.h"
#include "wall_clock.h"
#include "app_data.h"

#ifdef EHS_RTC_SUPPORT
#include "target_hal_rtc.h"
#endif//EHS_RTC_SUPPORT

#include "hal-api.h" /* Required for logging */

#ifdef EHS_NXP_SUPPORT
    #include "inx_utils.h"
#endif //#ifdef EHS_NXP_SUPPORT
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_wall_clock_state
{
    ehs_sint32 format;
} inx_wall_clock_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/******************************************************************************/
/* Define state function block */

EHS_FB_FUNCTIONS_START(wall_clock)
EHS_FB_FUNCTION_ENTRY("get", 0x01, wall_clock_get)
EHS_FB_FUNCTION_ENTRY("set", 0x02, wall_clock_set)
EHS_FB_FUNCTIONS_END

/**
 * Context for a timer block. Comprises the timer, the callback queue and the callback queue entry
 */

/*typedef struct {
@todo
} EhsTimeClockType;
*/ 

#define EHS_FB_WALLCLOCK_GET_DATA 0
#define EHS_FB_WALLCLOCK_GET_MINS 1
#define EHS_FB_WALLCLOCK_GET_SECS 2
#define EHS_FB_WALLCLOCK_GET_DONE 1
#define EHS_FB_WALLCLOCK_SET_DATA 0
#define EHS_FB_WALLCLOCK_SET_DONE 1

//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_wall_clock_ARG_get_Date_String 1
#define INX_wall_clock_ARG_get_Mins_1970 2
#define INX_wall_clock_ARG_get___secs 3
#define INX_wall_clock_ARG_get_unix_time_out 4
#define INX_wall_clock_ARG_get_Year 5
#define INX_wall_clock_ARG_get_MDay 6
#define INX_wall_clock_ARG_get_WDay 7
#define INX_wall_clock_ARG_get_Hour 8
#define INX_wall_clock_ARG_get_Minute 9
#define INX_wall_clock_ARG_get_Second 10
#define INX_wall_clock_ARG_get_Month 11
#define INX_wall_clock_ARG_get_local_diff 12
#define INX_wall_clock_ARG_get_______ 1
#define INX_wall_clock_ARG_set_time 1
#define INX_wall_clock_ARG_set_unix_time 2
#define INX_wall_clock_ARG_set_timezone 3
#define INX_wall_clock_ARG_set______ 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_wall_clock_format 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(wall_clock)
{
    /**
    ehs_sint32 format;
    EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d",&format); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_wall_clock_state_type);
    EHS_TRACE_FUNCTION(EHS_FB_IDENTIFY_NAME(wall_clock));
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(wall_clock)
{
    ehs_sint32 format;
    ehs_bool bRet = EHS_TRUE; /* assume success */
    //this is the reference to the object data for this instance of the function block
    inx_wall_clock_state_type* inx_wall_clock_state = (inx_wall_clock_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d",&format);
    inx_wall_clock_state->format = format;

    /* Add any further intialisation code here */
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(wall_clock)
{
    // inx_wall_clock_state_type *inx_wall_clock_state = (inx_wall_clock_state_type*)EHS_FB_DESTROY_CONTEXT;
    //Your code below here
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION get MACRO START -- DO NOT ALTER
/**
 * Run the function start. This starts (or restarts) a timer.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(wall_clock_get)
{
    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(wall_clock_));
    inx_wall_clock_state_type* inx_wall_clock_state = (inx_wall_clock_state_type*)EHS_FB_RUN_CONTEXT;
    ehs_sint64 time_value;
    if (EHS_FB_OUT_CONNECTED_API2(INX_wall_clock_ARG_get_Date_String))
        time_value = EhsHGetdateTime(EHS_FB_OUT_S_API2(INX_wall_clock_ARG_get_Date_String), EHS_STRING_LENGTH_MAX, EHS_TRUE, inx_wall_clock_state->format);
    else 
        time_value = EhsHGetdateTime(NULL, 0, EHS_TRUE, inx_wall_clock_state->format);

    // Your code here
    if (EHS_FB_OUT_CONNECTED_API2(INX_wall_clock_ARG_get_Mins_1970))
        EHS_FB_OUT_I_API2(INX_wall_clock_ARG_get_Mins_1970) = time_value / 60; // Minutes since 1970
    if (EHS_FB_OUT_CONNECTED_API2(INX_wall_clock_ARG_get___secs))
        EHS_FB_OUT_I_API2(INX_wall_clock_ARG_get___secs) = time_value % 60; // and the residual seconds
    if (EHS_FB_OUT_CONNECTED_API2(INX_wall_clock_ARG_get_unix_time_out))
        EHS_FB_OUT_I_API2(INX_wall_clock_ARG_get_unix_time_out) = time_value;
    if (EHS_FB_OUT_CONNECTED_API2(INX_wall_clock_ARG_get_local_diff))
        EHS_FB_OUT_I_API2(INX_wall_clock_ARG_get_local_diff) = EhsHDiffLocalUnix(&time_value);
    EhsHDateTimeBreakdown(time_value, EHS_TRUE,
                            (ehs_uint32*)&EHS_FB_OUT_I_API2(INX_wall_clock_ARG_get_Year),
                            (ehs_uint32*)&EHS_FB_OUT_I_API2(INX_wall_clock_ARG_get_Month),
                            (ehs_uint32*)&EHS_FB_OUT_I_API2(INX_wall_clock_ARG_get_MDay),
                            (ehs_uint32*)&EHS_FB_OUT_I_API2(INX_wall_clock_ARG_get_WDay),
                            (ehs_uint32*)&EHS_FB_OUT_I_API2(INX_wall_clock_ARG_get_Hour),
                            (ehs_uint32*)&EHS_FB_OUT_I_API2(INX_wall_clock_ARG_get_Minute),
                            (ehs_uint32*)&EHS_FB_OUT_I_API2(INX_wall_clock_ARG_get_Second));
    EHS_FB_FINISH(INX_wall_clock_ARG_get_______);
    return;
}//ICB FUNCTION get MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION set MACRO START -- DO NOT ALTER

/**
 * Run the function 'stop'. This halts a currently running timer.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(wall_clock_set)
{
    //inx_wall_clock_state_type* inx_wall_clock_state = (inx_wall_clock_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here

    //EhsTimeClockType* pTimeClock = (EhsTimeClockType*)EHS_FB_RUN_CONTEXT;
    #ifdef EHS_NXP_SUPPORT
        if(EHS_FB_IN_CONNECTED(EHS_FB_WALLCLOCK_SET_DATA)){
            inxNXPEHSSetDate(EHS_FB_IN_S(EHS_FB_WALLCLOCK_SET_DATA));
        }
    #else
    int unixTime = 0;
    char* strTime = NULL;
    char* timezone = NULL;
    ehs_uint32 time_sec = 0;
    if(EHS_FB_IN_CONNECTED_API2(INX_wall_clock_ARG_set_time)){
        strTime = EHS_FB_IN_S_API2(INX_wall_clock_ARG_set_time);
    }
    if(EHS_FB_IN_CONNECTED_API2(INX_wall_clock_ARG_set_unix_time)){
        unixTime = EHS_FB_IN_I_API2(INX_wall_clock_ARG_set_unix_time);
    }
    if(EHS_FB_IN_CONNECTED_API2(INX_wall_clock_ARG_set_timezone)){
        timezone = EHS_FB_IN_S_API2(INX_wall_clock_ARG_set_timezone);
    }
    EhsHSetDateTime(strTime, unixTime, timezone, &time_sec);
    #ifdef  EHS_RTC_SUPPORT
    ehs_uint32 time_get = EhsTGetRtcSecond();
    ehs_uint32 time_diff = time_get > time_sec ? time_get - time_sec : time_sec - time_get;
    if (time_diff > 10) EhsTSetRtcSecond(time_sec);
    #endif//EHS_RTC_SUPPORT

    #endif //#ifdef EHS_NXP_SUPPORT
    EHS_FB_FINISH(INX_wall_clock_ARG_set______);
    return;
}//ICB FUNCTION set MACRO END -- DO NOT ALTER THIS LINE

