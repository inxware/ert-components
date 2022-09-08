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
#include "inx-rtc.h"
#include "heatrod_config.h"
#include "ehs_main.h" // we run th main from here!
//ICB HEADER MACRO END -- DO NOT ALTER

#define INX_MQTT_PUBLISH_FIFO_SIZE 1

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_rtc_state
{
} inx_rtc_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(rtc)

EHS_FB_FUNCTION_ENTRY("set", 0x00, rtc_set)

EHS_FB_FUNCTION_ENTRY("read", 0x01, rtc_read)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_rtc_ARG_set_finishset 1
#define INX_rtc_ARG_read_tz 1
#define INX_rtc_ARG_read_time 1
#define INX_rtc_ARG_read_year 2
#define INX_rtc_ARG_read_month 3
#define INX_rtc_ARG_read_dom 4
#define INX_rtc_ARG_read_dow 5
#define INX_rtc_ARG_read_hour 6
#define INX_rtc_ARG_read_minute 7
#define INX_rtc_ARG_read_second 8
#define INX_rtc_ARG_read_finishread 1
#define INX_rtc_ARG_set_time 1
#define INX_rtc_ARG_set_finishset 1
#define INX_rtc_ARG_read_tz 1
#define INX_rtc_ARG_read_time 1
#define INX_rtc_ARG_read_year 2
#define INX_rtc_ARG_read_month 3
#define INX_rtc_ARG_read_dom 4
#define INX_rtc_ARG_read_dow 5
#define INX_rtc_ARG_read_hour 6
#define INX_rtc_ARG_read_minute 7
#define INX_rtc_ARG_read_second 8
#define INX_rtc_ARG_read_finishread 1
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
EHS_FB_IDENTIFY_FUNCTION(rtc)
{
    /* Uncomment the following if you need to parse the parameters to calculate memory required */
    /*
    	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_rtc_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(rtc)
{
    ehs_bool bRet = EHS_TRUE; /* assume success */

    //this is the reference to the object data for this instance of the function block
    /*
    inx_rtc_state_type* inx_rtc_state = (inx_rtc_state_type*)EHS_FB_INIT_CONTEXT;
    */
    /* read the initialisation parameters */
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"");

    /* Add any further intialisation code here */
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(rtc)
{
    /*
    inx_rtc_state_type *inx_rtc_state = (inx_rtc_state_type*)EHS_FB_DESTROY_CONTEXT;
    */
    //Your code below here
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION set MACRO START -- DO NOT ALTER
/**
 * Definition of rtc_set.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(rtc_set)
{
    /*
    inx_rtc_state_type* inx_rtc_state = (inx_rtc_state_type*)EHS_FB_RUN_CONTEXT;
    */

    // Your code here
    if(EHS_FB_IN_CONNECTED_API2(INX_rtc_ARG_set_time))
    {
        inxRTCWriteSeconds(EHS_FB_IN_I_API2(INX_rtc_ARG_set_time));
    }
    /*
    if (EHS_FB_IN_CONNECTED_API2(INX_rtc_ARG_set_time))
    */
    EHS_FB_FINISH(INX_rtc_ARG_set_finishset);
}//ICB FUNCTION set MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION read MACRO START -- DO NOT ALTER
/**
 * Definition of rtc_read.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(rtc_read)
{
    /*
    inx_rtc_state_type* inx_rtc_state = (inx_rtc_state_type*)EHS_FB_RUN_CONTEXT;
    */

    // Your code here
    EHS_FB_OUT_I_API2(INX_rtc_ARG_read_time)=inxRTCGetSeconds();
    ehs_uint16 year=0;
    ehs_uint8 month=0;
    ehs_uint8 dom=0;
    ehs_uint8 dow=0;
    ehs_uint8 hour=0;
    ehs_uint8 minute=0;
    ehs_uint8 second=0;
    inxRTCSecondsToDate(EHS_FB_IN_S_API2(INX_rtc_ARG_read_tz),EHS_FB_OUT_I_API2(INX_rtc_ARG_read_time),&year,&month,&dom,&dow,&hour,&minute,&second);
    EHS_FB_OUT_I_API2(INX_rtc_ARG_read_year)=(EhsDataflowIntType)year;
    EHS_FB_OUT_I_API2(INX_rtc_ARG_read_month)=(EhsDataflowIntType)month;
    EHS_FB_OUT_I_API2(INX_rtc_ARG_read_dom)=(EhsDataflowIntType)dom;
    EHS_FB_OUT_I_API2(INX_rtc_ARG_read_dow)=(EhsDataflowIntType)dow;
    EHS_FB_OUT_I_API2(INX_rtc_ARG_read_hour)=(EhsDataflowIntType)hour;
    EHS_FB_OUT_I_API2(INX_rtc_ARG_read_minute)=(EhsDataflowIntType)minute;
    EHS_FB_OUT_I_API2(INX_rtc_ARG_read_second)=(EhsDataflowIntType)second;
    /*
    if (EHS_FB_IN_CONNECTED_API2(INX_rtc_ARG_read_tz))
    if (EHS_FB_OUT_CONNECTED_API2(INX_rtc_ARG_read_time))
    if (EHS_FB_OUT_CONNECTED_API2(INX_rtc_ARG_read_year))
    if (EHS_FB_OUT_CONNECTED_API2(INX_rtc_ARG_read_month))
    if (EHS_FB_OUT_CONNECTED_API2(INX_rtc_ARG_read_dom))
    if (EHS_FB_OUT_CONNECTED_API2(INX_rtc_ARG_read_dow))
    if (EHS_FB_OUT_CONNECTED_API2(INX_rtc_ARG_read_hour))
    if (EHS_FB_OUT_CONNECTED_API2(INX_rtc_ARG_read_minute))
    if (EHS_FB_OUT_CONNECTED_API2(INX_rtc_ARG_read_second))
    */
    EHS_FB_FINISH(INX_rtc_ARG_read_finishread);
}//ICB FUNCTION read MACRO END -- DO NOT ALTER THIS LINE
