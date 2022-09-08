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
#include "inx-adc_read.h"
#include "target_adcdac.h"
//#include "ehs_main.h" // we run th main from here!
//ICB HEADER MACRO END -- DO NOT ALTER

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_adc_read_state
{
    ehs_uint8 channel;
    ehs_uint8 unit;
    ehs_uint8 configuration;
    ehs_bool enable_continuous;
    ehs_float clock_rate_hz;
    ehs_sint32 average;
    ehs_float value;
    EhsFunctionInstanceDataType* pFIdata;
    struct inx_adc_read_state* pNext;
    struct inx_adc_read_state* pPrev;
} inx_adc_read_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
static inx_adc_read_state_type* gpFirstWidget=NULL;

static inx_adc_read_state_type* inxADCReadGetLastWidget()
{
    inx_adc_read_state_type* widget=gpFirstWidget;
    while(widget!=NULL && widget->pNext!=NULL)
    {
        widget=widget->pNext;
        if(widget==widget->pNext)
        {
            EHSH_LOG_ERROR("ADC infinite loop found");
            widget->pNext=NULL;
        }
    }
    return widget;
}

static void inxADCReadRegisterWidget(inx_adc_read_state_type* pState)
{
    if(gpFirstWidget==NULL)
    {
        gpFirstWidget=pState;
        return;
    }

    inx_adc_read_state_type* lastWidget=inxADCReadGetLastWidget();
    if(lastWidget==NULL)
    {
        gpFirstWidget=pState;
    }
    else
    {
        lastWidget->pNext=pState;
        pState->pPrev=lastWidget;
    }
}

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(adc_read)
EHS_FB_FUNCTION_ENTRY("read", 0x00, adc_read_read)
EHS_FB_FUNCTION_ENTRY("sample", 0x01, adc_read_sample)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_adc_read_ARG_read_finishread 1
#define INX_adc_read_ARG_read_channel 1
#define INX_adc_read_ARG_read_value 1
#define INX_adc_read_ARG_read_finishread 1
#define INX_adc_read_ARG_sample_value 1
#define INX_adc_read_ARG_sample_finishread 1
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
EHS_FB_IDENTIFY_FUNCTION(adc_read)
{
    /* Uncomment the following if you need to parse the parameters to calculate memory required */


    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_adc_read_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(adc_read)
{
    ehs_bool bRet = EHS_TRUE; /* assume success */

    //this is the reference to the object data for this instance of the function block
    inx_adc_read_state_type* inx_adc_read_state = (inx_adc_read_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%hhd %hhd %hhd %hhd %lf %d",&inx_adc_read_state->channel,&inx_adc_read_state->unit,&inx_adc_read_state->configuration,&inx_adc_read_state->enable_continuous,&inx_adc_read_state->clock_rate_hz,&inx_adc_read_state->average);

    // configure the adc type
    configure_adc(inx_adc_read_state->channel,inx_adc_read_state->unit,inx_adc_read_state->configuration);

    // e.g. send in the pratmers and let th hardware layer try and set things up as requested:
    // ehs_bool ok = target_adc_config(inx_adc_read_state);
    /* Add any further intialisation code here */
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    // note: the following could be a fair bot more complex if we allow each block to have different ADC config paramters, but is still a shared resource with constraints.
    // .. but we'll percevere here and decide of this can be done or we need a separate ADC config block for ADC groups.
    // only works in callback version: inxADCReadRegisterWidget(inx_adc_read_state);
    /* todo2022 implement this -> target_read_adc_init(...); */
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(adc_read)
{
    /*
    inx_adc_read_state_type *inx_adc_read_state = (inx_adc_read_state_type*)EHS_FB_DESTROY_CONTEXT;
    */
    //Your code below here
    gpFirstWidget=NULL;
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION read MACRO START -- DO NOT ALTER
/**
 * Definition of adc_read_read.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */

// todo 2022 - this function can  be a simple read raw adc function for now it doesn't need to be DMA based .
// but if you have the DMA based sampling working then this function can read and average N-samples from the DMA buffer, which would be useful.
// if it is not DMA based then the target_adcdac.c need not support averaging.
EHS_FB_RUN_FUNCTION(adc_read_read)
{
    inx_adc_read_state_type* inx_adc_read_state = (inx_adc_read_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    inx_adc_read_state->pFIdata = EHS_FB_RUN_CONTEXT_REF;

// e.g. float value = 0.0;
//       ehs_bool OK = target_adc_get_sample(&value,inx_adc_read_state.average);
    /*
    if (EHS_FB_IN_CONNECTED_API2(INX_adc_read_ARG_read_channel))
    	EHS_FB_IN_I_API2(INX_adc_read_ARG_read_channel) ;
    if (EHS_FB_OUT_CONNECTED_API2(INX_adc_read_ARG_read_value))
    	EHS_FB_OUT_F_API2(INX_adc_read_ARG_read_value) ;
    EHS_FB_FINISH(INX_adc_read_ARG_read_finishread);
    */
}//ICB FUNCTION read MACRO END -- DO NOT ALTER THIS LINE

//*ICB FUNCTION sample MACRO START -- DO NOT ALTER
/**
 * Definition of adc_read_sample.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(adc_read_sample)
{
    inx_adc_read_state_type* inx_adc_read_state = (inx_adc_read_state_type*)EHS_FB_RUN_CONTEXT;
    // Your code here
    ehs_float value = 0.0f;
    target_read_adc_sample(inx_adc_read_state->channel,&value,inx_adc_read_state->configuration);
    if (EHS_FB_OUT_CONNECTED_API2(INX_adc_read_ARG_sample_value))
        EHS_FB_OUT_F_API2(INX_adc_read_ARG_sample_value) = value;
    EHS_FB_FINISH(INX_adc_read_ARG_sample_finishread);
}//ICB FUNCTION sample MACRO END -- DO NOT ALTER THIS LINE

// we do we have a mingw dependency here here? another hack for something to do with different compilers perhaps

#ifdef EHS_MINGW
#define EHS_ADC_READ_EXPORT __declspec(dllexport)
#else
#define EHS_ADC_READ_EXPORT // nothing
#endif

static void inxADCReadSetValues(const uint8_t channel,const ehs_float value)
{
    inx_adc_read_state_type* widget=gpFirstWidget;
    while(widget!=NULL)
    {
        //create pFIData variable so we can use the APIs
        EhsFunctionInstanceDataType* pFIdata=widget->pFIdata;
        if(pFIdata==NULL)
        {

        }
        else
        {
            if(EHS_FB_IN_CONNECTED_API2(INX_adc_read_ARG_read_channel))
            {
                widget->channel=EHS_FB_IN_I_API2(INX_adc_read_ARG_read_channel);
            }
            if(widget->channel==channel)
            {
                widget->value=value;
                EHS_FB_OUT_F_API2(INX_adc_read_ARG_read_value)=widget->value;
                EHS_FB_FINISH(INX_adc_read_ARG_read_finishread);
            }
        }
        widget=widget->pNext;
        if(widget==widget->pNext)
        {
            EHSH_LOG_ERROR("inxADCReadSetValues infinite loop found");
            widget->pNext=NULL;
        }
    }
}
/* THis is for the callback version */

EHS_ADC_READ_EXPORT ehs_bool EhsADCReadEvent(const uint8_t channel,const ehs_float value)
{
    EHSH_LOG_INFO("EhsADCReadEvent");
    inxADCReadSetValues(channel,value);
    return EHS_TRUE;
}
