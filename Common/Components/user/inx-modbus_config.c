/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

//ICB HEADER MACRO START -- DO NOT ALTER
//#include "inx_uart.h"
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-modbus_config.h"
//#include "ehs_main.h" // we run th main from here!
#include "target_uart.h"
#include "mb.h"
#include "mb_m.h"
#include "portcallbacks.h"
//ICB HEADER MACRO END -- DO NOT ALTER

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_modbus_config_state
{
    ehs_sint32 bus_id;
    ehs_bool master;
    ehs_sint32 data_mode;
    ehs_sint32 port_id;
    ehs_char ip[17];
    ehs_char cert[50];// Path to the cert
} inx_modbus_config_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER

extern unsigned char ucMBAddress;
ehs_bool gbEhsMBMasterFlag = EHS_FALSE;
ehs_bool gbEhsMBInitialised = EHS_FALSE;
ehs_bool gbEhsMBTCPIPFlag = EHS_FALSE;

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(modbus_config)
EHS_FB_FUNCTION_ENTRY("enable", 0x01, modbus_config_enable)
EHS_FB_FUNCTION_ENTRY("disable", 0x02, modbus_config_disable)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_modbus_config_ARG_enable_ip 1
#define INX_modbus_config_ARG_enable_port 2
#define INX_modbus_config_ARG_enable_cert 3
#define INX_modbus_config_ARG_enable_Bus_ID 4
#define INX_modbus_config_ARG_enable_Errno 1
#define INX_modbus_config_ARG_enable_finishenable 1
#define INX_modbus_config_ARG_disable_finishdisable 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_modbus_config_bus_id 0
#define INX_FB_modbus_config_master 0
#define INX_FB_modbus_config_data_mode 0
#define INX_FB_modbus_config_port_id 0
#define INX_FB_modbus_config_ip "127.0.0.1"
#define INX_FB_modbus_config_cert 
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(modbus_config)
{
    /* Uncomment the following if you need to parse the parameters to calculate memory required */
    /*
    ehs_sint32 bus_id;
    ehs_bool master;
    ehs_sint32 data_mode;
    ehs_sint32 port_id;
    ehs_char* ip;
    ehs_char* cert;
    EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d %d %d %d %s %s",&bus_id,&master,&data_mode,port_id,ip,&cert); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_modbus_config_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(modbus_config)
{
    ehs_sint32 bus_id;
    ehs_bool master;
    ehs_sint32 data_mode;
    ehs_sint32 port_id;
    ehs_bool bRet = EHS_TRUE; /* assume success */

    //this is the reference to the object data for this instance of the function block
    inx_modbus_config_state_type* inx_modbus_config_state = (inx_modbus_config_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
    // Directly allocate to the structure data
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d %d %d %d %s %s",&bus_id,&master,&data_mode,&port_id,inx_modbus_config_state->ip,inx_modbus_config_state->cert);

    inx_modbus_config_state->bus_id = bus_id;
    inx_modbus_config_state->master = master;
    gbEhsMBMasterFlag = master;
    inx_modbus_config_state->data_mode = data_mode;
    inx_modbus_config_state->port_id = port_id;

    /* Add any further intialisation code here */
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(modbus_config)
{
    inx_modbus_config_state_type *inx_modbus_config_state = (inx_modbus_config_state_type*)EHS_FB_DESTROY_CONTEXT;
    //Your code below here
    switch (inx_modbus_config_state->master)
    {
        case 0:
            eMBDisable();
            eMBClose();
            break;
        case 1:
            eMBMasterDisable();
            eMBMasterClose();
            break;
        default:
            break;
    }
    gbEhsMBInitialised = EHS_FALSE;
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION enable MACRO START -- DO NOT ALTER
/**
 * Definition of modbus_config_enable.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(modbus_config_enable)
{
    inx_modbus_config_state_type* inx_modbus_config_state = (inx_modbus_config_state_type*)EHS_FB_RUN_CONTEXT;

    ehs_uint32 enable_port = inx_modbus_config_state->port_id;
    ehs_uint32 slave_id = inx_modbus_config_state->bus_id;
    if (EHS_FB_IN_CONNECTED_API2(INX_modbus_config_ARG_enable_port))
        enable_port = EHS_FB_IN_I_API2(INX_modbus_config_ARG_enable_port) ;
    if (enable_port >= EHS_TARGET_UART_COUNT)
    {
        if (EHS_FB_OUT_CONNECTED_API2(INX_modbus_config_ARG_enable_Errno))
            EHS_FB_OUT_I_API2(INX_modbus_config_ARG_enable_Errno) = 1;
        EHS_FB_FINISH(INX_modbus_config_ARG_enable_finishenable);
        return;
    }
	if (EHS_FB_IN_CONNECTED_API2(INX_modbus_config_ARG_enable_Bus_ID))
		slave_id = EHS_FB_IN_I_API2(INX_modbus_config_ARG_enable_Bus_ID) ;

    switch (inx_modbus_config_state->master)
    {
        case 0: // Slave
        switch (inx_modbus_config_state->data_mode)
        {
            case 0:case 1:
            gbEhsMBTCPIPFlag = EHS_FALSE;
            eMBInit(
                inx_modbus_config_state->data_mode, 
                slave_id, 
                enable_port, 
                gUARTBaudRate[enable_port], 
                gUARTParity[enable_port]);
            gbEhsMBInitialised = EHS_TRUE;
            break;
            case 2:
            gbEhsMBTCPIPFlag = EHS_TRUE;
            eMBTCPInit(slave_id, enable_port);
            gbEhsMBInitialised = EHS_TRUE;
            break;
            default:
            break;
        }
            printf("data_mode: %d\nslave_id: %d\nport: %d\nbaudrate: %d\nparity: %d\nrx: %d\ntx: %d\nrtx: %d\ncts: %d\n",
                inx_modbus_config_state->data_mode,
                slave_id,
                enable_port, gUARTBaudRate[enable_port], gUARTParity[enable_port],
                gTargetUartPinRx[enable_port],
                gTargetUartPinTx[enable_port],
                gTargetUartPinRts[enable_port],
                gTargetUartPinCts[enable_port]);
            eMBEnable();
            break;
        case 1: // Master
            if (inx_modbus_config_state->data_mode < 2)
            {
                gbEhsMBTCPIPFlag = EHS_FALSE;
                eMBMasterSerialInit(
                    inx_modbus_config_state->data_mode,
                    enable_port, 
                    gUARTBaudRate[enable_port], 
                    gUARTParity[enable_port]);
            }
            else
            {
                gbEhsMBTCPIPFlag = EHS_TRUE;
                eMBMasterTCPInit(enable_port);
            }
            eMBMasterEnable();
            gbEhsMBInitialised = EHS_TRUE;
            break;
        default:
            break;
    }
    if (EHS_FB_OUT_CONNECTED_API2(INX_modbus_config_ARG_enable_Errno))
        EHS_FB_OUT_I_API2(INX_modbus_config_ARG_enable_Errno) = 
            gbEhsMBInitialised == EHS_TRUE ? 0 : 1;
    EHS_FB_FINISH(INX_modbus_config_ARG_enable_finishenable);

    return;

    // Your code here
    // if (EHS_FB_IN_CONNECTED_API2(INX_modbus_config_ARG_enable_ip))
    // 	EHS_FB_IN_S_API2(INX_modbus_config_ARG_enable_ip) ;
    // if (EHS_FB_IN_CONNECTED_API2(INX_modbus_config_ARG_enable_cert))
    // 	EHS_FB_IN_S_API2(INX_modbus_config_ARG_enable_cert) ;
    // EHS_FB_FINISH(INX_modbus_config_ARG_enable_finishenable);
    // ucMBAddress=(unsigned char)EHS_FB_IN_I_API2(INX_modbus_config_ARG_enable_id);
    // inxUARTModbusEnable();
    // if (EHS_FB_IN_CONNECTED_API2(INX_modbus_config_ARG_enable_id))
    //     EHS_FB_FINISH(INX_modbus_config_ARG_enable_finishenable);
}//ICB FUNCTION enable MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION disable MACRO START -- DO NOT ALTER
/**
 * Definition of modbus_config_disable.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(modbus_config_disable)
{
    inx_modbus_config_state_type* inx_modbus_config_state = (inx_modbus_config_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    // inxUARTModbusDisable();
    switch (inx_modbus_config_state->master)
    {
        case 0:
            eMBDisable();
            eMBClose();
            break;
        case 1:
            eMBMasterDisable();
            eMBMasterClose();
            break;
        default:
            break;
    }
    gbEhsMBInitialised = EHS_FALSE;
    EHS_FB_FINISH(INX_modbus_config_ARG_disable_finishdisable);
}//ICB FUNCTION disable MACRO END -- DO NOT ALTER THIS LINE