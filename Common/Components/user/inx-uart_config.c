/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx_uart.h"
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-uart_config.h"
#include "ehs_main.h" // we run th main from here!
//ICB HEADER MACRO END -- DO NOT ALTER

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_uart_config_state
{
} inx_uart_config_state_type; //Reference this, maybe store your config parameters in here too.

//ICB STATE VAR MACRO END -- DO NOT ALTER
extern uint32_t gUARTBaudRate;
extern uint8_t gUARTStopBits;
extern uint8_t gUARTParity;
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER


/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(uart_config)
EHS_FB_FUNCTION_ENTRY("uart_config_enable", 0x01, uart_config_enable)
EHS_FB_FUNCTION_ENTRY("uart_config_disable", 0x02, uart_config_disable)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */

#define INX_uart_config_ARG_enable_baudrate 1
#define INX_uart_config_ARG_enable_stopbits 2
#define INX_uart_config_ARG_enable_parity 3
#define INX_uart_config_ARG_enable_finishenable 1
#define INX_uart_config_ARG_disable_finishdisable 1
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
EHS_FB_IDENTIFY_FUNCTION(uart_config)
{
    /* Uncomment the following if you need to parse the parameters to calculate memory required */
    /*
    	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_uart_config_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(uart_config)
{
    ehs_bool bRet = EHS_TRUE; /* assume success */

    //this is the reference to the object data for this instance of the function block
    /*
    inx_uart_config_state_type* inx_uart_config_state = (inx_uart_config_state_type*)EHS_FB_INIT_CONTEXT;
    */
    /* read the initialisation parameters */
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"");

    /* Add any further intialisation code here */
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(uart_config)
{
    /*
    inx_uart_config_state_type *inx_uart_config_state = (inx_uart_config_state_type*)EHS_FB_DESTROY_CONTEXT;
    */
    //Your code below here
    inxUARTDisable();
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION disable MACRO START -- DO NOT ALTER
/**
 * Definition of uart_config_disable.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(uart_config_disable)
{
    /*
    inx_modbus_config_state_type* inx_modbus_config_state = (inx_modbus_config_state_type*)EHS_FB_RUN_CONTEXT;
    */
    // Your code here
    inxUARTDisable();
    EHS_FB_FINISH(INX_uart_config_ARG_disable_finishdisable);
}//ICB FUNCTION disable MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION enable MACRO START -- DO NOT ALTER
/**
 * Definition of uart_config_enable.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(uart_config_enable)
{
    /*
    inx_uart_config_state_type* inx_uart_config_state = (inx_uart_config_state_type*)EHS_FB_RUN_CONTEXT;
    */


    // Your code here
    gUARTBaudRate=EHS_FB_IN_I_API2(INX_uart_config_ARG_enable_baudrate);
    gUARTStopBits=EHS_FB_IN_I_API2(INX_uart_config_ARG_enable_stopbits);
    gUARTParity=EHS_FB_IN_I_API2(INX_uart_config_ARG_enable_parity);
    inxUARTEnable();
    /*
    if (EHS_FB_IN_CONNECTED_API2(INX_uart_config_ARG_enable_baudrate))
    if (EHS_FB_IN_CONNECTED_API2(INX_uart_config_ARG_enable_stopbits))
    if (EHS_FB_IN_CONNECTED_API2(INX_uart_config_ARG_enable_parity))
    */
    EHS_FB_FINISH(INX_uart_config_ARG_enable_finishenable);
}//ICB FUNCTION enable MACRO END -- DO NOT ALTER THIS LINE
