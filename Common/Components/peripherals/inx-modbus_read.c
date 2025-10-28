/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-modbus_read.h"
#include "portcallbacks.h"
#include "mb_m.h"
//#include "ehs_main.h" // we run th main from here!
//ICB HEADER MACRO END -- DO NOT ALTER

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_modbus_read_state
{
    EhsFunctionInstanceDataType* pFIdata;
    ehs_sint32 device_id;
    ehs_sint32 reg_type;
    ehs_sint32 reg_addr;
    // Data storage
    ehs_sint32 data;
    struct inx_modbus_read_state* pNext;
    struct inx_modbus_read_state* pPrev;
} inx_modbus_read_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
static inx_modbus_read_state_type* gpFirstWidget=NULL;

static inx_modbus_read_state_type* inxModbusReadGetLastWidget()
{
    inx_modbus_read_state_type* widget=gpFirstWidget;
    while(widget!=NULL && widget->pNext!=NULL)
    {
        widget=widget->pNext;
        if(widget==widget->pNext)
        {
            EHSH_LOG_ERROR("inxModbusReadGetLastWidget infinite loop found");
            widget->pNext=NULL;
        }
    }
    return widget;
}

static void inxModbusReadRegisterWidget(inx_modbus_read_state_type* pState)
{
    if (pState == NULL)
    {
        return;
    }
    if(gpFirstWidget==NULL)
    {
        gpFirstWidget=pState;
        gpFirstWidget->pPrev = NULL;
        gpFirstWidget->pNext = NULL;
        return;
    }

    inx_modbus_read_state_type* lastWidget=inxModbusReadGetLastWidget();
    if(lastWidget==NULL)
    {
        gpFirstWidget=pState;
    }
    else
    {
        lastWidget->pNext=pState;
        pState->pPrev=lastWidget;
        pState->pNext = NULL;
    }
}

/*
 * Get the context which matches the given parameters. If nothing found, it returns NULL
 */
static inx_modbus_read_state_type* inxModbusReadSearch(ehs_sint32 device_id, ehs_sint32 register_type, ehs_sint32 address)
{
    inx_modbus_read_state_type* widget = gpFirstWidget;
    while (widget != NULL)
    {
        if (widget->device_id == device_id && widget->reg_type == register_type && widget->reg_addr == address) return widget;
        widget=widget->pNext;
        if (widget == NULL) return NULL;
        if(widget==widget->pNext)
        {
            EHSH_LOG_ERROR("inxModbusSlaveRegisterSearch infinite loop found");
            widget->pNext=NULL;
        }
    }
    return NULL;
}

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(modbus_read)

EHS_FB_FUNCTION_ENTRY("read", 0x01, modbus_read_read)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_modbus_read_ARG_read_device 1
#define INX_modbus_read_ARG_read_type 2
#define INX_modbus_read_ARG_read_register 3
#define INX_modbus_read_ARG_read_value 1
#define INX_modbus_read_ARG_read_error_code 2
#define INX_modbus_read_ARG_read_finishread 1
#define INX_modbus_read_ARG_read_error 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_modbus_read_device_id 0
#define INX_FB_modbus_read_reg_type 
#define INX_FB_modbus_read_reg_addr 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(modbus_read)
{
    /* Uncomment the following if you need to parse the parameters to calculate memory required */
    /*
    ehs_sint32 device_id;
    ehs_sint32 reg_type;
    ehs_sint32 reg_addr;
    EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d %d %d",&device_id,&reg_type,&reg_addr); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_modbus_read_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(modbus_read)
{
    ehs_bool bRet = EHS_TRUE; /* assume success */

    //this is the reference to the object data for this instance of the function block
    inx_modbus_read_state_type* inx_modbus_read_state = (inx_modbus_read_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d %d %d",&inx_modbus_read_state->device_id,&inx_modbus_read_state->reg_type,&inx_modbus_read_state->reg_addr);
    inx_modbus_read_state->pFIdata=NULL;
    inx_modbus_read_state->pNext=NULL;
    inx_modbus_read_state->pPrev=NULL;
    inx_modbus_read_state->data = 0;

    /* Add any further intialisation code here */
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    inxModbusReadRegisterWidget(inx_modbus_read_state);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);

    eMBMasterControlThreadStart();

    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(modbus_read)
{
    /*
    inx_modbus_read_state_type *inx_modbus_read_state = (inx_modbus_read_state_type*)EHS_FB_DESTROY_CONTEXT;
    */

    eMBMasterControlThreadKill();
    
    //Your code below here
    gpFirstWidget=NULL;
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION read MACRO START -- DO NOT ALTER
/**
 * Definition of modbus_read_read.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(modbus_read_read)
{
    inx_modbus_read_state_type* inx_modbus_read_state = (inx_modbus_read_state_type*)EHS_FB_RUN_CONTEXT;

    // MODBUS is not in master mode
    if (!gbEhsMBMasterFlag)
    {
        if (EHS_FB_OUT_CONNECTED_API2(INX_modbus_read_ARG_read_error_code))
            EHS_FB_OUT_I_API2(INX_modbus_read_ARG_read_error_code) = -503;
        EHS_FB_FINISH(INX_modbus_read_ARG_read_error);
        return;
    }

    // Your code here
    inx_modbus_read_state->pFIdata = EHS_FB_RUN_CONTEXT_REF;
    
    if (EHS_FB_IN_CONNECTED_API2(INX_modbus_read_ARG_read_device) &&
        EHS_FB_IN_CONNECTED_API2(INX_modbus_read_ARG_read_type) &&
        EHS_FB_IN_CONNECTED_API2(INX_modbus_read_ARG_read_register))
    {
        inx_modbus_read_state->device_id = EHS_FB_IN_I_API2(INX_modbus_read_ARG_read_device) ;
        inx_modbus_read_state->reg_type = EHS_FB_IN_I_API2(INX_modbus_read_ARG_read_type) ;
        inx_modbus_read_state->reg_addr = EHS_FB_IN_I_API2(INX_modbus_read_ARG_read_register) ;
    }
    //printf("read (REQ) val id=%d reg=%d addr=%d \n", inx_modbus_read_state->device_id, (mbSHORT)inx_modbus_read_state->reg_type, (mbSHORT)inx_modbus_read_state->reg_addr);
    eMBMasterRead((mbUCHAR)inx_modbus_read_state->device_id, (mbSHORT)inx_modbus_read_state->reg_type, (mbSHORT)inx_modbus_read_state->reg_addr);
    
    /*
    if (EHS_FB_OUT_CONNECTED_API2(INX_modbus_read_ARG_read_value))
        EHS_FB_OUT_I_API2(INX_modbus_read_ARG_read_value) ;
    EHS_FB_FINISH(INX_modbus_read_ARG_read_finishread);
    */
}//ICB FUNCTION read MACRO END -- DO NOT ALTER THIS LINE

#ifdef EHS_MINGW
#define EHS_MODBUS_READ_EXPORT __declspec(dllexport)
#else
#define EHS_MODBUS_READ_EXPORT // nothing
#endif

EHS_MODBUS_READ_EXPORT ehs_sint32 EhsHMBMasterReadSetValue(ehs_sint32 device_id, eMBRegisterType register_type, ehs_sint32 address, ehs_sint32 value)
{
    //printf("read val id=%d reg=%d addr=%d \n",device_id, register_type, address);
    inx_modbus_read_state_type *widget = inxModbusReadSearch(device_id, register_type, address);
    if (widget == NULL) return -1;
    widget->data = value;
    EhsFunctionInstanceDataType* pFIdata=widget->pFIdata;
    if (pFIdata == NULL) return -2;
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    if (EHS_FB_OUT_CONNECTED_API2(INX_modbus_read_ARG_read_value))
        EHS_FB_OUT_I_API2(INX_modbus_read_ARG_read_value) = widget->data ;
    if (EHS_FB_OUT_CONNECTED_API2(INX_modbus_read_ARG_read_error_code))
        EHS_FB_OUT_I_API2(INX_modbus_read_ARG_read_error_code) = 0;
    EHS_FB_FINISH(INX_modbus_read_ARG_read_finishread);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    return 0;
}

EHS_MODBUS_READ_EXPORT ehs_sint32 EhsHMBMasterReadError(ehs_sint32 device_id, eMBRegisterType register_type, ehs_sint32 address, ehs_sint32 error)
{
    //printf("read ERR val id=%d reg=%d addr=%d \n",device_id, register_type, address);
    inx_modbus_read_state_type *widget = inxModbusReadSearch(device_id, register_type, address);
    if (widget == NULL) return -1;
    EhsFunctionInstanceDataType* pFIdata=widget->pFIdata;
    if (pFIdata == NULL) return -2;
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    eMBMasterReqErrCode mb_error = (eMBMasterReqErrCode)error;
    if (mb_error != MB_MRE_NO_ERR)
    {
        if (EHS_FB_OUT_CONNECTED_API2(INX_modbus_read_ARG_read_error_code))
            EHS_FB_OUT_I_API2(INX_modbus_read_ARG_read_error_code) = mb_error;
        EHS_FB_FINISH(INX_modbus_read_ARG_read_error);
    }
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    return 0;
}

/*
get rid of the BSS buffers that emodbus looks at
instead have it iterate our modbus blocks
if that gets a match on the channel then return it as the correct type
static void inxModbusReadSetValues(const uint32_t channel,const EhsDataflowIntType value)
{
    inx_modbus_read_state_type* widget=gpFirstWidget;
    while(widget!=NULL)
    {
        //create pFIData variable so we can use the APIs
        EhsFunctionInstanceDataType* pFIdata=widget->pFIdata;
        if(pFIdata==NULL)
        {

        }
        else
        {
            if(EHS_FB_IN_CONNECTED_API2(INX_modbus_read_ARG_read_channel) && EHS_FB_IN_I_API2(INX_modbus_read_ARG_read_channel)==channel)
            {
                EHS_FB_OUT_I_API2(INX_modbus_read_ARG_read_value)=value;
                EHS_FB_FINISH(INX_modbus_read_ARG_read_finishread);
            }
        }
        widget=widget->pNext;
        if(widget==widget->pNext)
        {
            EHSH_LOG_ERROR("inxModbusReadSetValues infinite loop found");
            widget->pNext=NULL;
        }
    }
}

EHS_MODBUS_READ_EXPORT ehs_bool EhsModbusReadEvent(const uint32_t channel,const EhsDataflowIntType value)
{
    inxModbusReadSetValues(channel,value);
    return EHS_TRUE;
}
*/
