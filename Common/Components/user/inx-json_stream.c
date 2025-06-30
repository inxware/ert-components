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
#include "inx-json_stream.h"

#include "yajl_parse.h"
#include "yajl_gen.h"
//ICB HEADER MACRO END -- DO NOT ALTER
typedef struct inx_json_stream_parser_chunk_struct
{
    ehs_char chunk[EHS_STRING_LENGTH_MAX]; //TODO:STRINGLENGTH!
    struct inx_json_stream_parser_chunk_struct* pNext;
} inx_json_stream_parser_chunk_type;
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct
{
    yajl_handle hand;
    EhsFunctionInstanceDataType* pReadFid;
    EhsFunctionInstanceDataType* pParseFid;
    inx_json_stream_parser_chunk_type* input;
    size_t read;
} inx_json_stream_parser_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(json_stream_parser)

EHS_FB_FUNCTION_ENTRY("read", 0x01, json_stream_parser_read)

EHS_FB_FUNCTION_ENTRY("parse", 0x02, json_stream_parser_parse)
EHS_FB_FUNCTION_ENTRY("eos", 0x03, json_stream_parser_eos)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_json_stream_parser_ARG_read_data 1
#define INX_json_stream_parser_ARG_read_readDataOut 1
#define INX_json_stream_parser_ARG_read_read_finish 1
#define INX_json_stream_parser_ARG_read_Error 2
#define INX_json_stream_parser_ARG_read_ErrorNum 2
#define INX_json_stream_parser_ARG_parse_nextFinish 1
#define INX_json_stream_parser_ARG_parse_startMap 2
#define INX_json_stream_parser_ARG_eos_eosFinish 1
#define INX_json_stream_parser_ARG_parse_endMap 3
#define INX_json_stream_parser_ARG_parse_mapKey 4
#define INX_json_stream_parser_ARG_parse_string 5
#define INX_json_stream_parser_ARG_eos_eosFinish 1
#define INX_json_stream_parser_ARG_parse_DataType 1
#define INX_json_stream_parser_ARG_parse_parseDataOut 2
#define INX_json_stream_parser_ARG_parse_Key 3
#define INX_json_stream_parser_ARG_parse_Parent 4
#define INX_json_stream_parser_ARG_parse_errorNum 5
#define INX_json_stream_parser_ARG_parse_nextFinish 1
#define INX_json_stream_parser_ARG_parse_startMap 2
#define INX_json_stream_parser_ARG_parse_endMap 3
#define INX_json_stream_parser_ARG_parse_mapKey 4
#define INX_json_stream_parser_ARG_parse_startArray 5
#define INX_json_stream_parser_ARG_parse_endArray 6
#define INX_json_stream_parser_ARG_parse_value 7
#define INX_json_stream_parser_ARG_parse_Error 8
#define INX_json_stream_parser_ARG_eos_eosFinish 1

#define INX_json_stream_parser_memory_alloc_error 1001
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
EHS_FB_IDENTIFY_FUNCTION(json_stream_parser)
{
    /* Uncomment the following if you need to parse the parameters to calculate memory required */
    /*
    	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_json_stream_parser_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
static int reformat_null(void * ctx)
{
    inx_json_stream_parser_state_type* state = (inx_json_stream_parser_state_type*)ctx;
    EhsFunctionInstanceDataType* pFIdata=state->pParseFid;
    if(EHS_FB_OUT_CONNECTED_API2(INX_json_stream_parser_ARG_parse_parseDataOut))
    {
        strcpy(EHS_FB_OUT_S_API2(INX_json_stream_parser_ARG_parse_parseDataOut),"null");
        ((ehs_char*)EHS_FB_OUT_S_API2(INX_json_stream_parser_ARG_parse_parseDataOut))[4]='\0';
    }
    if(EHS_FB_OUT_CONNECTED_API2(INX_json_stream_parser_ARG_parse_DataType))
    {
        EHS_FB_OUT_I_API2(INX_json_stream_parser_ARG_parse_DataType)=3;
    }
    EHS_FB_FINISH(INX_json_stream_parser_ARG_parse_value);
    return 0;
}

static int reformat_boolean(void * ctx, int boolean)
{
    inx_json_stream_parser_state_type* state = (inx_json_stream_parser_state_type*)ctx;
    EhsFunctionInstanceDataType* pFIdata=state->pParseFid;
    if(EHS_FB_OUT_CONNECTED_API2(INX_json_stream_parser_ARG_parse_parseDataOut))
    {
        if(boolean==0)
        {
            strcpy(EHS_FB_OUT_S_API2(INX_json_stream_parser_ARG_parse_parseDataOut),"0");
        }
        else
        {
            strcpy(EHS_FB_OUT_S_API2(INX_json_stream_parser_ARG_parse_parseDataOut),"1");
        }
        ((ehs_char*)EHS_FB_OUT_S_API2(INX_json_stream_parser_ARG_parse_parseDataOut))[1]='\0';
    }
    if(EHS_FB_OUT_CONNECTED_API2(INX_json_stream_parser_ARG_parse_DataType))
    {
        EHS_FB_OUT_I_API2(INX_json_stream_parser_ARG_parse_DataType)=2;
    }
    EHS_FB_FINISH(INX_json_stream_parser_ARG_parse_value);
    return 0;
}

static int reformat_number(void * ctx, const char * s, size_t l)
{
    inx_json_stream_parser_state_type* state = (inx_json_stream_parser_state_type*)ctx;
    EhsFunctionInstanceDataType* pFIdata=state->pParseFid;
    if(EHS_FB_OUT_CONNECTED_API2(INX_json_stream_parser_ARG_parse_parseDataOut))
    {
        strncpy(EHS_FB_OUT_S_API2(INX_json_stream_parser_ARG_parse_parseDataOut),s,l);
        ((ehs_char*)EHS_FB_OUT_S_API2(INX_json_stream_parser_ARG_parse_parseDataOut))[l]='\0';
    }
    if(EHS_FB_OUT_CONNECTED_API2(INX_json_stream_parser_ARG_parse_DataType))
    {
        EHS_FB_OUT_I_API2(INX_json_stream_parser_ARG_parse_DataType)=1;
    }
    EHS_FB_FINISH(INX_json_stream_parser_ARG_parse_value);
    return 0;
}

static int reformat_string(void * ctx, const char * stringVal,
                           size_t stringLen)
{
    inx_json_stream_parser_state_type* state = (inx_json_stream_parser_state_type*)ctx;
    EhsFunctionInstanceDataType* pFIdata=state->pParseFid;
    //copy stringVal to output
    if(EHS_FB_OUT_CONNECTED_API2(INX_json_stream_parser_ARG_parse_parseDataOut))
    {
        strncpy(EHS_FB_OUT_S_API2(INX_json_stream_parser_ARG_parse_parseDataOut),stringVal,stringLen);
        ((ehs_char*)EHS_FB_OUT_S_API2(INX_json_stream_parser_ARG_parse_parseDataOut))[stringLen]='\0';
    }
    if(EHS_FB_OUT_CONNECTED_API2(INX_json_stream_parser_ARG_parse_DataType))
    {
        EHS_FB_OUT_I_API2(INX_json_stream_parser_ARG_parse_DataType)=0;
    }
    EHS_FB_FINISH(INX_json_stream_parser_ARG_parse_value);
    return 0;
}

static int reformat_map_key(void * ctx, const char * stringVal,
                            size_t stringLen)
{
    inx_json_stream_parser_state_type* state = (inx_json_stream_parser_state_type*)ctx;
    EhsFunctionInstanceDataType* pFIdata=state->pParseFid;
    //copy stringVal to output
    if (EHS_FB_OUT_CONNECTED_API2(INX_json_stream_parser_ARG_parse_Key))
    {
        strncpy(EHS_FB_OUT_S_API2(INX_json_stream_parser_ARG_parse_Key),stringVal,stringLen);
        ((ehs_char*)EHS_FB_OUT_S_API2(INX_json_stream_parser_ARG_parse_Key))[stringLen]='\0';
    }
    EHS_FB_FINISH(INX_json_stream_parser_ARG_parse_mapKey);
    return 0;
}

static int reformat_start_map(void * ctx)
{
    inx_json_stream_parser_state_type* state = (inx_json_stream_parser_state_type*)ctx;
    EhsFunctionInstanceDataType* pFIdata=state->pParseFid;
    EHS_FB_FINISH(INX_json_stream_parser_ARG_parse_startMap);
    return 0;
}

static int reformat_end_map(void * ctx)
{
    inx_json_stream_parser_state_type* state = (inx_json_stream_parser_state_type*)ctx;
    EhsFunctionInstanceDataType* pFIdata=state->pParseFid;
    EHS_FB_FINISH(INX_json_stream_parser_ARG_parse_endMap);
    return 0;
}

static int reformat_start_array(void * ctx)
{
    inx_json_stream_parser_state_type* state = (inx_json_stream_parser_state_type*)ctx;
    EhsFunctionInstanceDataType* pFIdata=state->pParseFid;
    EHS_FB_FINISH(INX_json_stream_parser_ARG_parse_startArray);
    return 0;
}

static int reformat_end_array(void * ctx)
{
    inx_json_stream_parser_state_type* state = (inx_json_stream_parser_state_type*)ctx;
    EhsFunctionInstanceDataType* pFIdata=state->pParseFid;
    EHS_FB_FINISH(INX_json_stream_parser_ARG_parse_endArray);
    return 0;
}

static yajl_callbacks callbacks =
{
    reformat_null,
    reformat_boolean,
    NULL,
    NULL,
    reformat_number,
    reformat_string,
    reformat_start_map,
    reformat_map_key,
    reformat_end_map,
    reformat_start_array,
    reformat_end_array
};

//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(json_stream_parser)
{
    ehs_bool bRet = EHS_TRUE; /* assume success */
    //this is the reference to the object data for this instance of the function block
    inx_json_stream_parser_state_type* inx_json_stream_parser_state = (inx_json_stream_parser_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
    //EhsSscanf(EHS_FB_INIT_PARAMETERS,"");

    /* Add any further intialisation code here */
    inx_json_stream_parser_state->input=NULL;
    inx_json_stream_parser_state->hand=NULL;
    inx_json_stream_parser_state->pReadFid=NULL;
    inx_json_stream_parser_state->pParseFid=NULL;
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER

//ICB FUNCTION read MACRO START -- DO NOT ALTER
/**
 * Definition of json_stream_parser_read.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(json_stream_parser_read)
{
    inx_json_stream_parser_state_type* inx_json_stream_parser_state = (inx_json_stream_parser_state_type*)EHS_FB_RUN_CONTEXT;
    inx_json_stream_parser_chunk_type* input=inx_json_stream_parser_state->input;
    inx_json_stream_parser_chunk_type* pNew=NULL;
    inx_json_stream_parser_chunk_type* pCurrent=NULL;
    ehs_char* chunk=NULL;
    yajl_handle hand=inx_json_stream_parser_state->hand;
    inx_json_stream_parser_state->pReadFid=pFIdata;

    // Your code here
    // Your code here
    if (EHS_FB_IN_CONNECTED_API2(INX_json_stream_parser_ARG_read_data))
    {
        //initialise handle
        if(hand==NULL)
        {
            inx_json_stream_parser_state->hand = yajl_alloc(&callbacks, NULL, (void *)inx_json_stream_parser_state);
            inx_json_stream_parser_state->read=0;
            inx_json_stream_parser_state->input=NULL;
            input=NULL;
            hand=inx_json_stream_parser_state->hand;
        }
        //what we are being presented with should be held in a linked list
        if(input==NULL)
        {
            //no existing input so this is the first
            input=(inx_json_stream_parser_chunk_type*)EhsHMem_tempAlloc(sizeof(inx_json_stream_parser_chunk_type));
            
            if(input==NULL) // check if we run out of memory
            {
                if(EHS_FB_OUT_CONNECTED_API2(INX_json_stream_parser_ARG_read_ErrorNum))
                {
                    EHS_FB_OUT_I_API2(INX_json_stream_parser_ARG_read_ErrorNum)=INX_json_stream_parser_memory_alloc_error;
                }
                EHS_FB_FINISH(INX_json_stream_parser_ARG_read_Error);
                return;
            }

            input->pNext=NULL;
            chunk=input->chunk;
            inx_json_stream_parser_state->input=input;
        }
        else
        {
            //input is already there so use the pNext
            //find the next free slot
            pCurrent=input;
            while(pCurrent->pNext!=NULL)
            {
                pCurrent=pCurrent->pNext;
            }
            pCurrent->pNext=(inx_json_stream_parser_chunk_type*)EhsHMem_tempAlloc(sizeof(inx_json_stream_parser_chunk_type));
            if(pCurrent->pNext==NULL) // check if we run out of memory
            {
                if(EHS_FB_OUT_CONNECTED_API2(INX_json_stream_parser_ARG_read_ErrorNum))
                {
                    EHS_FB_OUT_I_API2(INX_json_stream_parser_ARG_read_ErrorNum)=INX_json_stream_parser_memory_alloc_error;
                }
                EHS_FB_FINISH(INX_json_stream_parser_ARG_read_Error);
                return;
            }
            pCurrent->pNext->pNext=NULL;
            chunk=pCurrent->pNext->chunk;
        }
        //copy our new chunk of data in
        strcpy(chunk,EHS_FB_IN_S_API2(INX_json_stream_parser_ARG_read_data));
        if (EHS_FB_OUT_CONNECTED_API2(INX_json_stream_parser_ARG_read_readDataOut))
        {
            strcpy(EHS_FB_OUT_S_API2(INX_json_stream_parser_ARG_read_readDataOut),chunk);
        }
    }
    EHS_FB_FINISH(INX_json_stream_parser_ARG_read_read_finish);
    return;
}//ICB FUNCTION read MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION parse MACRO START -- DO NOT ALTER
/**
 * Definition of json_stream_parser_parse.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(json_stream_parser_parse)
{
    inx_json_stream_parser_state_type* inx_json_stream_parser_state = (inx_json_stream_parser_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    inx_json_stream_parser_state->pParseFid=pFIdata;
    yajl_handle hand=inx_json_stream_parser_state->hand;
    yajl_status status;
    size_t size;
    //try to parse
    inx_json_stream_parser_chunk_type* input=inx_json_stream_parser_state->input;
    inx_json_stream_parser_chunk_type* pOld=NULL;
    ehs_char* chunk=NULL;
    //if we don't have any input then do nothing
    if(input==NULL||hand==NULL)
    {
        EHSH_LOG_ERROR("parse:No input or handle");
        return;
    }
    while(input!=NULL)
    {
        chunk=input->chunk;
        size=EhsStrlen(chunk)-inx_json_stream_parser_state->read;
        status=yajl_parse(hand,(const unsigned char *)&chunk[inx_json_stream_parser_state->read],size);
        inx_json_stream_parser_state->read=inx_json_stream_parser_state->read+yajl_get_bytes_consumed(hand);
        if (status == yajl_status_error)
        {
            unsigned char * str = yajl_get_error(hand,1,(const unsigned char *)&chunk[inx_json_stream_parser_state->read],size);
            //fprintf(stderr,"%s\n",(const char *)str);
            yajl_free_error(hand,str);
            if (EHS_FB_OUT_CONNECTED_API2(INX_json_stream_parser_ARG_parse_errorNum))
            {
                EHS_FB_OUT_I_API2(INX_json_stream_parser_ARG_parse_errorNum)=status;
            }
            /* Auto generated but not configured
            			if (EHS_FB_OUT_CONNECTED_API2(INX_json_stream_parser_ARG_parse_DataType))
            				EHS_FB_OUT_I_API2(INX_json_stream_parser_ARG_parse_DataType) ;
            			if (EHS_FB_OUT_CONNECTED_API2(INX_json_stream_parser_ARG_parse_parseDataOut))
            				EHS_FB_OUT_S_API2(INX_json_stream_parser_ARG_parse_parseDataOut) ;
            			if (EHS_FB_OUT_CONNECTED_API2(INX_json_stream_parser_ARG_parse_Key))
            				EHS_FB_OUT_S_API2(INX_json_stream_parser_ARG_parse_Key) ;
            			if (EHS_FB_OUT_CONNECTED_API2(INX_json_stream_parser_ARG_parse_Parent))
            				EHS_FB_OUT_S_API2(INX_json_stream_parser_ARG_parse_Parent) ;
            		*/
            EHS_FB_FINISH(INX_json_stream_parser_ARG_parse_Error);
            break;
        }
        else if(status==yajl_status_client_canceled)
        {
            //cancelled due to event
            break;
        }
        else
        {
            //status equaled 0 so end of file hit?
            //is there a next chunk?
            if(input->pNext!=NULL)
            {
                //there is a next chunk so use that
                pOld=input;
                input=pOld->pNext;
                EhsHMem_tempFree(pOld);
                pOld=NULL;
                inx_json_stream_parser_state->read=0;
                inx_json_stream_parser_state->input=input;
            }
            else
            {
                //no next chunk so just free the input
                EhsHMem_tempFree(input);
                input=NULL;
                inx_json_stream_parser_state->input=NULL;
                inx_json_stream_parser_state->read=0;
            }
        }

    }
    EHS_FB_FINISH(INX_json_stream_parser_ARG_parse_nextFinish);
    //EHS_FB_FINISH(INX_json_stream_parser_ARG_parse_startMap);
    //EHS_FB_FINISH(INX_json_stream_parser_ARG_parse_endMap);
    //EHS_FB_FINISH(INX_json_stream_parser_ARG_parse_mapKey);
    //EHS_FB_FINISH(INX_json_stream_parser_ARG_parse_string);
    //EHS_FB_FINISH(INX_json_stream_parser_ARG_parse_startArray);
    //EHS_FB_FINISH(INX_json_stream_parser_ARG_parse_endArray);
    //EHS_FB_FINISH(INX_json_stream_parser_ARG_parse_value);*/
    //EHS_FB_FINISH(INX_json_stream_parser_ARG_parse_Error);
}//ICB FUNCTION parse MACRO END -- DO NOT ALTER THIS LINE


static void inx_json_stream_parser_cleanup(inx_json_stream_parser_state_type* state)
{
    // Your code here
    yajl_handle hand=state->hand;
    inx_json_stream_parser_chunk_type* current;
    inx_json_stream_parser_chunk_type* parent;
    if(hand!=NULL)
    {
        yajl_free(hand);
        state->hand=NULL;
    }
    //free up the linked list
    current=state->input;
    if(current!=NULL)
    {
        do{
            parent=current;
            current=current->pNext;
            EhsHMem_tempFree(parent);
            parent=NULL;
        }while(current!=NULL && current->pNext!=NULL);
    }
    state->input=NULL;
    state->read=0;
}
//ICB FUNCTION eos MACRO START -- DO NOT ALTER
/**
 * Definition of json_stream_parser_eos.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(json_stream_parser_eos)
{
    inx_json_stream_parser_state_type* state = (inx_json_stream_parser_state_type*)EHS_FB_RUN_CONTEXT;
    inx_json_stream_parser_state_type* inx_json_stream_parser_state = (inx_json_stream_parser_state_type*)EHS_FB_RUN_CONTEXT;


    inx_json_stream_parser_cleanup(state);
    // Your code here
    EHS_FB_FINISH(INX_json_stream_parser_ARG_eos_eosFinish);
}//ICB FUNCTION eos MACRO END -- DO NOT ALTER THIS LINE

//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(json_stream_parser)
{
    inx_json_stream_parser_state_type *inx_json_stream_parser_state = (inx_json_stream_parser_state_type*)EHS_FB_DESTROY_CONTEXT;
    //Your code below here
    inx_json_stream_parser_state_type* state = (inx_json_stream_parser_state_type*)EHS_FB_DESTROY_CONTEXT;
    inx_json_stream_parser_cleanup(state);
    return EHS_FALSE;
}//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
