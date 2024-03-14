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
#include "inx-permanent_storage.h"
#include "ehs_main.h" // we run th main from here!
#include "inxware_hrx_config.h"
//ICB HEADER MACRO END -- DO NOT ALTER

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_permanent_storage_state
{
    struct inx_permanent_storage_state* pNext;
    struct inx_permanent_storage_state* pPrev;
    ehs_bool needWriting;
    ehs_uint16 id;
    ehs_uint16 valueSizeBytes;
} inx_permanent_storage_state_type; //Reference this, maybe store your config parameters in here too.

typedef struct inx_permanent_storage_state_int
{
    inx_permanent_storage_state_type base;
    EhsDataflowIntType value;
} inx_permanent_storage_state_int_type; //Reference this, maybe store your config parameters in here too.

typedef struct inx_permanent_storage_state_bool
{
    inx_permanent_storage_state_type base;
    EhsDataflowBoolType value;
} inx_permanent_storage_state_bool_type; //Reference this, maybe store your config parameters in here too.

typedef struct inx_permanent_storage_state_real
{
    inx_permanent_storage_state_type base;
    EhsDataflowFloatType value;
} inx_permanent_storage_state_real_type; //Reference this, maybe store your config parameters in here too.

typedef struct inx_permanent_storage_state_string
{
    inx_permanent_storage_state_type base;
    ehs_char value[EHS_STRING_LENGTH_MAX];
} inx_permanent_storage_state_string_type; //Reference this, maybe store your config parameters in here too.

//ICB STATE VAR MACRO END -- DO NOT ALTER
static inx_permanent_storage_state_type* gpFirstWidget=NULL;

static inx_permanent_storage_state_type* inxPermanentStorageGetLastWidget()
{
    inx_permanent_storage_state_type* widget=gpFirstWidget;
    while(widget!=NULL && widget->pNext!=NULL)
    {
        widget=widget->pNext;
        if(widget==widget->pNext)
        {
            widget->pNext=NULL;
        }
    }
    return widget;
}

static void inxPermanentStorageRegisterWidget(inx_permanent_storage_state_type* pState)
{
    if(gpFirstWidget==NULL)
    {
        gpFirstWidget=pState;
        return;
    }

    inx_permanent_storage_state_type* lastWidget=inxPermanentStorageGetLastWidget();
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

static ehs_bool inxPermanentStorageNeedWriting()
{
    inx_permanent_storage_state_type* widget=gpFirstWidget;
    while(widget!=NULL)
    {
        if(widget->needWriting)
        {
            return EHS_TRUE;
        }
        if(widget==widget->pNext)
        {
            widget->pNext=NULL;
        }
        widget=widget->pNext;
    }
    return EHS_FALSE;
}

static void inxPermanentStorageCopyToBuffer(ehs_uint8* pBuffer,const ehs_uint32 bufferMaxSizeBytes)
{
    inx_permanent_storage_state_type* widget=gpFirstWidget;
    ehs_uint32 bytesWritten=0;
    ehs_uint32 bytesToWrite=0;
    while(widget!=NULL)
    {
        bytesToWrite=sizeof(widget->id)+sizeof(widget->valueSizeBytes)+widget->valueSizeBytes;
        if((bytesWritten + bytesToWrite) <=bufferMaxSizeBytes)
        {
            void* pValue=(&widget->valueSizeBytes)+sizeof(widget->valueSizeBytes);
            //id
            memcpy(&pBuffer[bytesWritten],&widget->id,sizeof(widget->id));
            bytesWritten=bytesWritten+sizeof(widget->id);
            //length
            memcpy(&pBuffer[bytesWritten],&widget->valueSizeBytes,sizeof(widget->valueSizeBytes));
            bytesWritten=bytesWritten+sizeof(widget->valueSizeBytes);
            //value
            memcpy(&pBuffer[bytesWritten],pValue,widget->valueSizeBytes);
            bytesWritten=bytesWritten+widget->valueSizeBytes;
            widget->needWriting=EHS_FALSE;
            if(widget==widget->pNext)
            {
                widget->pNext=NULL;
            }
            widget=widget->pNext;
        }
        else
        {
            break;
        }
    }
}

#define INX_permanent_storage_ARG_write_finishwrite 1
#define INX_permanent_storage_ARG_write_id 1
#define INX_permanent_storage_ARG_write_value 2
#define INX_permanent_storage_ARG_read_id 1
#define INX_permanent_storage_ARG_read_finishread 1
#define INX_permanent_storage_ARG_read_finisherr 2
#define INX_permanent_storage_ARG_read_value 1

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(permanent_storage_int)

EHS_FB_FUNCTION_ENTRY("write", 0x01, permanent_storage_int_write)

EHS_FB_FUNCTION_ENTRY("read", 0x02, permanent_storage_int_read)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */

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
EHS_FB_IDENTIFY_FUNCTION(permanent_storage_int)
{
    /* Uncomment the following if you need to parse the parameters to calculate memory required */
    /*
    	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_permanent_storage_state_int_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(permanent_storage_int)
{
    ehs_bool bRet = EHS_TRUE; /* assume success */

    //this is the reference to the object data for this instance of the function block
    inx_permanent_storage_state_int_type* inx_permanent_storage_state = (inx_permanent_storage_state_int_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"");
    inx_permanent_storage_state->base.pNext=NULL;
    inx_permanent_storage_state->base.pPrev=NULL;
    inx_permanent_storage_state->base.needWriting=EHS_FALSE;
    /* Add any further intialisation code here */
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    inxPermanentStorageRegisterWidget(&inx_permanent_storage_state->base);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(permanent_storage_int)
{
    //inx_adc_read_state_type *inx_adc_read_state = (inx_adc_read_state_type*)EHS_FB_DESTROY_CONTEXT;
    //Your code below here
    gpFirstWidget=NULL;
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION write MACRO START -- DO NOT ALTER
/**
 * Definition of permanent_storage_write.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(permanent_storage_int_write)
{
    inx_permanent_storage_state_int_type* inx_permanent_storage_state = (inx_permanent_storage_state_int_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    inx_permanent_storage_state->base.id=EHS_FB_IN_I_API2(INX_permanent_storage_ARG_write_id);
    inx_permanent_storage_state->value=EHS_FB_IN_I_API2(INX_permanent_storage_ARG_write_value);
    inx_permanent_storage_state->base.valueSizeBytes=sizeof(inx_permanent_storage_state->value);
    inx_permanent_storage_state->base.needWriting=EHS_TRUE;
    EHS_FB_FINISH(INX_permanent_storage_ARG_write_finishwrite);
}//ICB FUNCTION write MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION read MACRO START -- DO NOT ALTER
/**
 * Definition of permanent_storage_read.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(permanent_storage_int_read)
{
    inx_permanent_storage_state_int_type* inx_permanent_storage_state = (inx_permanent_storage_state_int_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    inx_permanent_storage_state->base.id=EHS_FB_IN_I_API2(INX_permanent_storage_ARG_read_id);
    EhsDataflowIntType value=0;
    if(inxEHSPermanentStorageGetValue(inx_permanent_storage_state->base.id,&value))
    {
        EHS_FB_OUT_I_API2(INX_permanent_storage_ARG_read_value)=value;
        EHS_FB_FINISH(INX_permanent_storage_ARG_read_finishread);
    }
    else
    {
        EHS_FB_FINISH(INX_permanent_storage_ARG_read_finisherr);
    }
}//ICB FUNCTION write MACRO END -- DO NOT ALTER THIS LINE

#ifdef EHS_MINGW
#define EHS_permanent_storage_EXPORT __declspec(dllexport)
#else
#define EHS_permanent_storage_EXPORT // nothing
#endif

//when the NXP system wants to save
//iterate the linked list looking for any to have a flag indicating we need to write to flash
//only if at least one item
//memcpy each item in to our buffer for writing to flash
EHS_permanent_storage_EXPORT ehs_bool EhsPermanentStorageCopyToBuffer(uint8_t* pBufferStart,const uint32_t bufferMaxSizeBytes)
{
    ehs_bool needWriting=inxPermanentStorageNeedWriting();
    if(needWriting)
    {
        memset(pBufferStart,0,bufferMaxSizeBytes);
        inxPermanentStorageCopyToBuffer(pBufferStart,bufferMaxSizeBytes);
    }
    return needWriting;
}



//BOOL
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(permanent_storage_bool)

EHS_FB_FUNCTION_ENTRY("write", 0x01, permanent_storage_bool_write)

EHS_FB_FUNCTION_ENTRY("read", 0x02, permanent_storage_bool_read)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */

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
EHS_FB_IDENTIFY_FUNCTION(permanent_storage_bool)
{
    /* Uncomment the following if you need to parse the parameters to calculate memory required */
    /*
    	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_permanent_storage_state_bool_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(permanent_storage_bool)
{
    ehs_bool bRet = EHS_TRUE; /* assume success */

    //this is the reference to the object data for this instance of the function block
    inx_permanent_storage_state_bool_type* inx_permanent_storage_state = (inx_permanent_storage_state_bool_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"");
    inx_permanent_storage_state->base.pNext=NULL;
    inx_permanent_storage_state->base.pPrev=NULL;
    inx_permanent_storage_state->base.needWriting=EHS_FALSE;
    /* Add any further intialisation code here */
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    inxPermanentStorageRegisterWidget(&inx_permanent_storage_state->base);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(permanent_storage_bool)
{
    //inx_adc_read_state_type *inx_adc_read_state = (inx_adc_read_state_type*)EHS_FB_DESTROY_CONTEXT;
    //Your code below here
    gpFirstWidget=NULL;
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION write MACRO START -- DO NOT ALTER
/**
 * Definition of permanent_storage_write.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(permanent_storage_bool_write)
{
    inx_permanent_storage_state_bool_type* inx_permanent_storage_state = (inx_permanent_storage_state_bool_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    inx_permanent_storage_state->base.id=EHS_FB_IN_I_API2(INX_permanent_storage_ARG_write_id);
    inx_permanent_storage_state->value=EHS_FB_IN_B_API2(INX_permanent_storage_ARG_write_value);
    inx_permanent_storage_state->base.valueSizeBytes=sizeof(inx_permanent_storage_state->value);
    inx_permanent_storage_state->base.needWriting=EHS_TRUE;
    EHS_FB_FINISH(INX_permanent_storage_ARG_write_finishwrite);
}//ICB FUNCTION write MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION read MACRO START -- DO NOT ALTER
/**
 * Definition of permanent_storage_read.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(permanent_storage_bool_read)
{
    inx_permanent_storage_state_bool_type* inx_permanent_storage_state = (inx_permanent_storage_state_bool_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    inx_permanent_storage_state->base.id=EHS_FB_IN_I_API2(INX_permanent_storage_ARG_read_id);
    EhsDataflowBoolType value=0;
    if(inxEHSPermanentStorageGetValue(inx_permanent_storage_state->base.id,&value))
    {
        EHS_FB_OUT_B_API2(INX_permanent_storage_ARG_read_value)=value;
        EHS_FB_FINISH(INX_permanent_storage_ARG_read_finishread);
    }
    else
    {
        EHS_FB_FINISH(INX_permanent_storage_ARG_read_finisherr);
    }
}//ICB FUNCTION write MACRO END -- DO NOT ALTER THIS LINE




//REAL
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(permanent_storage_real)

EHS_FB_FUNCTION_ENTRY("write", 0x01, permanent_storage_real_write)

EHS_FB_FUNCTION_ENTRY("read", 0x02, permanent_storage_real_read)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */

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
EHS_FB_IDENTIFY_FUNCTION(permanent_storage_real)
{
    /* Uncomment the following if you need to parse the parameters to calculate memory required */
    /*
    	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_permanent_storage_state_real_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(permanent_storage_real)
{
    ehs_bool bRet = EHS_TRUE; /* assume success */

    //this is the reference to the object data for this instance of the function block
    inx_permanent_storage_state_real_type* inx_permanent_storage_state = (inx_permanent_storage_state_real_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"");
    inx_permanent_storage_state->base.pNext=NULL;
    inx_permanent_storage_state->base.pPrev=NULL;
    inx_permanent_storage_state->base.needWriting=EHS_FALSE;
    /* Add any further intialisation code here */
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    inxPermanentStorageRegisterWidget(&inx_permanent_storage_state->base);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(permanent_storage_real)
{
    //inx_adc_read_state_type *inx_adc_read_state = (inx_adc_read_state_type*)EHS_FB_DESTROY_CONTEXT;
    //Your code below here
    gpFirstWidget=NULL;
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION write MACRO START -- DO NOT ALTER
/**
 * Definition of permanent_storage_write.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(permanent_storage_real_write)
{
    inx_permanent_storage_state_real_type* inx_permanent_storage_state = (inx_permanent_storage_state_real_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    inx_permanent_storage_state->base.id=EHS_FB_IN_I_API2(INX_permanent_storage_ARG_write_id);
    inx_permanent_storage_state->value=EHS_FB_IN_F_API2(INX_permanent_storage_ARG_write_value);
    inx_permanent_storage_state->base.valueSizeBytes=sizeof(inx_permanent_storage_state->value);
    inx_permanent_storage_state->base.needWriting=EHS_TRUE;
    EHS_FB_FINISH(INX_permanent_storage_ARG_write_finishwrite);
}//ICB FUNCTION write MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION read MACRO START -- DO NOT ALTER
/**
 * Definition of permanent_storage_read.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(permanent_storage_real_read)
{
    inx_permanent_storage_state_real_type* inx_permanent_storage_state = (inx_permanent_storage_state_real_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    inx_permanent_storage_state->base.id=EHS_FB_IN_I_API2(INX_permanent_storage_ARG_read_id);
    EhsDataflowFloatType value=0;
    if(inxEHSPermanentStorageGetValue(inx_permanent_storage_state->base.id,&value))
    {
        EHS_FB_OUT_F_API2(INX_permanent_storage_ARG_read_value)=value;
        EHS_FB_FINISH(INX_permanent_storage_ARG_read_finishread);
    }
    else
    {
        EHS_FB_FINISH(INX_permanent_storage_ARG_read_finisherr);
    }
}//ICB FUNCTION write MACRO END -- DO NOT ALTER THIS LINE



//STRING
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(permanent_storage_string)

EHS_FB_FUNCTION_ENTRY("write", 0x01, permanent_storage_string_write)

EHS_FB_FUNCTION_ENTRY("read", 0x02, permanent_storage_string_read)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */

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
EHS_FB_IDENTIFY_FUNCTION(permanent_storage_string)
{
    /* Uncomment the following if you need to parse the parameters to calculate memory required */
    /*
    	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_permanent_storage_state_string_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(permanent_storage_string)
{
    ehs_bool bRet = EHS_TRUE; /* assume success */

    //this is the reference to the object data for this instance of the function block
    inx_permanent_storage_state_string_type* inx_permanent_storage_state = (inx_permanent_storage_state_string_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"");
    inx_permanent_storage_state->base.pNext=NULL;
    inx_permanent_storage_state->base.pPrev=NULL;
    inx_permanent_storage_state->base.needWriting=EHS_FALSE;
    /* Add any further intialisation code here */
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    inxPermanentStorageRegisterWidget(&inx_permanent_storage_state->base);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(permanent_storage_string)
{
    //inx_adc_read_state_type *inx_adc_read_state = (inx_adc_read_state_type*)EHS_FB_DESTROY_CONTEXT;
    //Your code below here
    gpFirstWidget=NULL;
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION write MACRO START -- DO NOT ALTER
/**
 * Definition of permanent_storage_write.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(permanent_storage_string_write)
{
    inx_permanent_storage_state_string_type* inx_permanent_storage_state = (inx_permanent_storage_state_string_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    inx_permanent_storage_state->base.id=EHS_FB_IN_I_API2(INX_permanent_storage_ARG_write_id);
    EhsStrcpy(inx_permanent_storage_state->value,EHS_FB_IN_S_API2(INX_permanent_storage_ARG_write_value));
    inx_permanent_storage_state->base.valueSizeBytes=EhsStrlen(inx_permanent_storage_state->value)+1;
    inx_permanent_storage_state->base.needWriting=EHS_TRUE;
    EHS_FB_FINISH(INX_permanent_storage_ARG_write_finishwrite);
}//ICB FUNCTION write MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION read MACRO START -- DO NOT ALTER
/**
 * Definition of permanent_storage_read.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(permanent_storage_string_read)
{
    inx_permanent_storage_state_string_type* inx_permanent_storage_state = (inx_permanent_storage_state_string_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    inx_permanent_storage_state->base.id=EHS_FB_IN_I_API2(INX_permanent_storage_ARG_read_id);
    if(inxEHSPermanentStorageGetValue(inx_permanent_storage_state->base.id,EHS_FB_OUT_S_API2(INX_permanent_storage_ARG_read_value)))
    {
        EHS_FB_FINISH(INX_permanent_storage_ARG_read_finishread);
    }
    else
    {
        EHS_FB_FINISH(INX_permanent_storage_ARG_read_finisherr);
    }
}//ICB FUNCTION write MACRO END -- DO NOT ALTER THIS LINE
