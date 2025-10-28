/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

//ICB HEADER MACRO START -- DO NOT ALTER
//#include "inx_uart.h"
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-uart_config.h"
#include "target_uart.h"
//#include "ehs_main.h" // we run th main from here!
//ICB HEADER MACRO END -- DO NOT ALTER

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_uart_config_state
{
    ehs_sint32 Port;
    ehs_sint32 Baud;
    ehs_sint32 Data_Length;
    ehs_sint32 Parity;
    ehs_sint32 Stop_bits;
    ehs_bool Hardware_Control;
} inx_uart_config_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
typedef enum {
    eEhsUartConfigErrorOk = 0,
    eEhsUartConfigErrorInvalidPort
} eEhsUartConfigError_t;
#ifndef EHS_TARGET_UART_COUNT 
#define EHS_TARGET_UART_COUNT 1
#endif//EHS_TARGET_UART_COUNT 
ehs_uint32 gUARTBaudRate[EHS_TARGET_UART_COUNT];
ehs_uint8 gUARTStopBits[EHS_TARGET_UART_COUNT];
ehs_uint8 gUARTParity[EHS_TARGET_UART_COUNT];
ehs_uint8 gUARTHWCTRL[EHS_TARGET_UART_COUNT];
ehs_uint8 gUARTLength[EHS_TARGET_UART_COUNT];
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(uart_config)
EHS_FB_FUNCTION_ENTRY("enable", 0x01, uart_config_enable)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_uart_config_ARG_enable_baudrate 1
#define INX_uart_config_ARG_enable_stopbits 2
#define INX_uart_config_ARG_enable_parity 3
#define INX_uart_config_ARG_enable_hw_ctrl 4
#define INX_uart_config_ARG_enable_length 5
#define INX_uart_config_ARG_enable_Errno 1
#define INX_uart_config_ARG_enable_finishenable 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_uart_config_Port 0
#define INX_FB_uart_config_Baud 115200
#define INX_FB_uart_config_Data_Length 8
#define INX_FB_uart_config_Parity 0
#define INX_FB_uart_config_Stop_bits 0
#define INX_FB_uart_config_Hardware_Control 0
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
    ehs_sint32 Port;
    ehs_sint32 Baud;
    ehs_sint32 Data_Length;
    ehs_sint32 Parity;
    ehs_sint32 Stop_bits;
    ehs_bool Hardware_Control;
    EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d %d %d %d %d %d",&Port,&Baud,&Data_Length,&Parity,&Stop_bits,&Hardware_Control); */
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
    ehs_sint32 Port;
    ehs_sint32 Baud;
    ehs_sint32 Data_Length;
    ehs_sint32 Parity;
    ehs_sint32 Stop_bits;
    ehs_bool Hardware_Control;
    ehs_bool bRet = EHS_TRUE; /* assume success */

    //this is the reference to the object data for this instance of the function block
    inx_uart_config_state_type* inx_uart_config_state = (inx_uart_config_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d %d %d %d %d %d",&Port,&Baud,&Data_Length,&Parity,&Stop_bits,&Hardware_Control);
    inx_uart_config_state->Port = Port;
    inx_uart_config_state->Baud = Baud;
    inx_uart_config_state->Data_Length = Data_Length;
    inx_uart_config_state->Parity = Parity;
    inx_uart_config_state->Stop_bits = Stop_bits;
    inx_uart_config_state->Hardware_Control = Hardware_Control;

    if (Port < EHS_TARGET_UART_COUNT)
    {
        gUARTBaudRate[Port] = Baud;
        gUARTParity[Port] = Parity;
        gUARTStopBits[Port] = Stop_bits;
        gUARTHWCTRL[Port] = Hardware_Control;
        gUARTLength[Port] = Data_Length;
    }

    /* Add any further intialisation code here */
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(uart_config)
{
    inx_uart_config_state_type *inx_uart_config_state = (inx_uart_config_state_type*)EHS_FB_DESTROY_CONTEXT;
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
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
    inx_uart_config_state_type* inx_uart_config_state = (inx_uart_config_state_type*)EHS_FB_RUN_CONTEXT;

    eEhsUartConfigError_t Errno = eEhsUartConfigErrorOk;

    if (inx_uart_config_state->Port >= EHS_TARGET_UART_COUNT)
    {
        Errno = eEhsUartConfigErrorInvalidPort;
        if (EHS_FB_OUT_CONNECTED_API2(INX_uart_config_ARG_enable_Errno))
            EHS_FB_OUT_I_API2(INX_uart_config_ARG_enable_Errno) = Errno;
        EHS_FB_FINISH(INX_uart_config_ARG_enable_finishenable);
        return;
    }

    // Your code here
    if (EHS_FB_IN_CONNECTED_API2(INX_uart_config_ARG_enable_baudrate))
        gUARTBaudRate[inx_uart_config_state->Port]=EHS_FB_IN_I_API2(INX_uart_config_ARG_enable_baudrate);
    if (EHS_FB_IN_CONNECTED_API2(INX_uart_config_ARG_enable_stopbits))
        gUARTStopBits[inx_uart_config_state->Port]=EHS_FB_IN_I_API2(INX_uart_config_ARG_enable_stopbits);
    if (EHS_FB_IN_CONNECTED_API2(INX_uart_config_ARG_enable_parity))
        gUARTParity[inx_uart_config_state->Port]=EHS_FB_IN_I_API2(INX_uart_config_ARG_enable_parity);
    if (EHS_FB_IN_CONNECTED_API2(INX_uart_config_ARG_enable_hw_ctrl))
        gUARTHWCTRL[inx_uart_config_state->Port]=EHS_FB_IN_B_API2(INX_uart_config_ARG_enable_hw_ctrl) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_uart_config_ARG_enable_length))
		gUARTLength[inx_uart_config_state->Port]=EHS_FB_IN_I_API2(INX_uart_config_ARG_enable_length) ;

    // inxUARTEnable();
    /*
    */
    if (EHS_FB_OUT_CONNECTED_API2(INX_uart_config_ARG_enable_Errno))
        EHS_FB_OUT_I_API2(INX_uart_config_ARG_enable_Errno) = Errno;
    EHS_FB_FINISH(INX_uart_config_ARG_enable_finishenable);

}//ICB FUNCTION enable MACRO END -- DO NOT ALTER THIS LINE
