//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include <stdio.h>
#include "inx-modbus_slave_register.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_modbus_slave_register_state
{
    EhsFunctionInstanceDataType* pFIdata;
    struct inx_modbus_slave_register_state* pNext;
    struct inx_modbus_slave_register_state* pPrev;
    ehs_sint32 register_type;
    ehs_sint32 address;
    ehs_sint32 data;
} inx_modbus_slave_register_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
static inx_modbus_slave_register_state_type* gpFirstWidget=NULL;

static inx_modbus_slave_register_state_type* inxModbusSlaveRegisterGetLastWidget( void )
{
    inx_modbus_slave_register_state_type* widget=gpFirstWidget;
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

static void inxModbusSlaveRegisterRegisterWidget(inx_modbus_slave_register_state_type* pState)
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

    inx_modbus_slave_register_state_type* lastWidget=inxModbusSlaveRegisterGetLastWidget();
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
 * Remove the given context from the linked list
 */
static void inxModbusSlaveRegisterRemoveWidget(inx_modbus_slave_register_state_type *pState)
{
    if (pState == NULL) return;
    if (pState->pPrev != NULL) pState->pPrev->pNext = pState->pNext;
    if (pState->pNext != NULL) pState->pNext->pPrev = pState->pPrev;
    pState->pPrev = NULL;
    pState->pNext = NULL;
    // Fix the linked list if possible
    inxModbusSlaveRegisterGetLastWidget();
}

/*
 * Get the context which matches the given parameters. If nothing found, it returns NULL
 */
static inx_modbus_slave_register_state_type* inxModbusSlaveRegisterSearch(ehs_sint32 register_type, ehs_sint32 address)
{
    inx_modbus_slave_register_state_type* widget = gpFirstWidget;
    while (widget != NULL)
    {
        if (widget->register_type == register_type && widget->address == address) return widget;
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
EHS_FB_FUNCTIONS_START(modbus_slave_register)
EHS_FB_FUNCTION_ENTRY("set", 0x01, modbus_slave_register_set)
EHS_FB_FUNCTION_ENTRY("get", 0x02, modbus_slave_register_get)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_modbus_slave_register_ARG_set_data 1
#define INX_modbus_slave_register_ARG_set_done 1
#define INX_modbus_slave_register_ARG_set_error 2
#define INX_modbus_slave_register_ARG_get_got_data 1
#define INX_modbus_slave_register_ARG_get_get_done 1
#define INX_modbus_slave_register_ARG_get_cb 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_modbus_slave_register_register_type 0
#define INX_FB_modbus_slave_register_address 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(modbus_slave_register)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
    ehs_sint32 register_type;
    ehs_sint32 address;
    EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d %d",&register_type,&address); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_modbus_slave_register_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(modbus_slave_register)
{
    ehs_bool bRet = EHS_TRUE; /* assume success */
    //this is the reference to the object data for this instance of the function block
    inx_modbus_slave_register_state_type* inx_modbus_slave_register_state = (inx_modbus_slave_register_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d %d",&inx_modbus_slave_register_state->register_type,&inx_modbus_slave_register_state->address);
    // Init the register data to be 0
    inx_modbus_slave_register_state->pFIdata = NULL;
    inx_modbus_slave_register_state->pNext = NULL;
    inx_modbus_slave_register_state->pPrev = NULL;
    inx_modbus_slave_register_state->data = 0;
    /* Register current function block context into the linked list */
    inxModbusSlaveRegisterRegisterWidget(inx_modbus_slave_register_state);

    /* Add any further intialisation code here */
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(modbus_slave_register)
{
    inx_modbus_slave_register_state_type *inx_modbus_slave_register_state = (inx_modbus_slave_register_state_type*)EHS_FB_DESTROY_CONTEXT;
    //Your code below here
    // inxModbusSlaveRegisterRemoveWidget(inx_modbus_slave_register_state);
    gpFirstWidget = NULL;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION set MACRO START -- DO NOT ALTER
/**
 * Definition of modbus_slave_register_set.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(modbus_slave_register_set)
{
    inx_modbus_slave_register_state_type* inx_modbus_slave_register_state = (inx_modbus_slave_register_state_type*)EHS_FB_RUN_CONTEXT;
    // Your code here
    //todo2025  - does this also need to check to see if the MODBUS system has been initialised?

    if (EHS_FB_IN_CONNECTED_API2(INX_modbus_slave_register_ARG_set_data))
    {
        inx_modbus_slave_register_state->data = EHS_FB_IN_I_API2(INX_modbus_slave_register_ARG_set_data) ;
        EHS_FB_FINISH(INX_modbus_slave_register_ARG_set_done);
    }
    else EHS_FB_FINISH(INX_modbus_slave_register_ARG_set_error);
}//ICB FUNCTION set MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION get MACRO START -- DO NOT ALTER
/**
 * Definition of modbus_slave_register_get.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(modbus_slave_register_get)
{
    inx_modbus_slave_register_state_type* inx_modbus_slave_register_state = (inx_modbus_slave_register_state_type*)EHS_FB_RUN_CONTEXT;
    inx_modbus_slave_register_state->pFIdata = EHS_FB_RUN_CONTEXT_REF;
     //todo2025  - does this also need to check to see if the MODBUS system has been initialised?
    // Your code here
    if (EHS_FB_OUT_CONNECTED_API2(INX_modbus_slave_register_ARG_get_got_data))
        EHS_FB_OUT_I_API2(INX_modbus_slave_register_ARG_get_got_data) = inx_modbus_slave_register_state->data;
    EHS_FB_FINISH(INX_modbus_slave_register_ARG_get_get_done);
}//ICB FUNCTION get MACRO END -- DO NOT ALTER THIS LINE

#ifdef EHS_MINGW
#define EHS_MODBUS_SLAVE_REGISTER_EXPORT __declspec(dllexport)
#else
#define EHS_MODBUS_SLAVE_REGISTER_EXPORT // nothing
#endif

/*
 * Get the pValue with the given register type and address.
 * If this is called from callback that needs to return quickly consider building a LUT of points to the linked list item rather than searching.
 * @return 0 if success
 * @return -1 if no value is found in the linked list
 */
EHS_MODBUS_SLAVE_REGISTER_EXPORT ehs_sint32 EhsHMBSlaveGetValue(ehs_sint32 register_type, ehs_sint32 address, ehs_sint32 *pValue)
{
    if (pValue == NULL) return -2;
    inx_modbus_slave_register_state_type *widget = inxModbusSlaveRegisterSearch(register_type, address);
    if (widget == NULL) return -1;
    *pValue = widget->data;
    return 0;
}

/*
 * Set the pValue with the given register type and address.
 * @return 0 if success
 * @return -1 if no value is found in the linked list
 */
EHS_MODBUS_SLAVE_REGISTER_EXPORT ehs_sint32 EhsHMBSlaveSetValue(ehs_sint32 register_type, ehs_sint32 address, ehs_sint32 value)
{
    inx_modbus_slave_register_state_type *widget = inxModbusSlaveRegisterSearch(register_type, address);
    if (widget == NULL) return -1;
    EhsFunctionInstanceDataType* pFIdata = widget->pFIdata;
    widget->data = value;
    if (pFIdata != NULL)
    {
        if (EHS_FB_OUT_CONNECTED_API2(INX_modbus_slave_register_ARG_get_got_data))
            EHS_FB_OUT_I_API2(INX_modbus_slave_register_ARG_get_got_data) = value;
        EHS_FB_FINISH(INX_modbus_slave_register_ARG_get_cb);
    }
    return 0;
}