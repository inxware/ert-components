//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-json_parser_str.h"
#include "jsmn_utils.h"
#ifndef JSMN_HEADER
#define JSMN_HEADER
#endif
#include "jsmn.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_json_parser_str_state
{
    ehs_char* file;
    ehs_char *keys[16];
} inx_json_parser_str_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(json_parser_str)
EHS_FB_FUNCTION_ENTRY("parse", 0x01, json_parser_str_parse)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_json_parser_str_ARG_parse_input_str 1
#define INX_json_parser_str_ARG_parse_key1 2
#define INX_json_parser_str_ARG_parse_key2 3
#define INX_json_parser_str_ARG_parse_key3 4
#define INX_json_parser_str_ARG_parse_key4 5
#define INX_json_parser_str_ARG_parse_key5 6
#define INX_json_parser_str_ARG_parse_key6 7
#define INX_json_parser_str_ARG_parse_key7 8
#define INX_json_parser_str_ARG_parse_key8 9
#define INX_json_parser_str_ARG_parse_key9 10
#define INX_json_parser_str_ARG_parse_key10 11
#define INX_json_parser_str_ARG_parse_key11 12
#define INX_json_parser_str_ARG_parse_key12 13
#define INX_json_parser_str_ARG_parse_key13 14
#define INX_json_parser_str_ARG_parse_key14 15
#define INX_json_parser_str_ARG_parse_key15 16
#define INX_json_parser_str_ARG_parse_key16 17
#define INX_json_parser_str_ARG_parse_err_no 1
#define INX_json_parser_str_ARG_parse_output1 2
#define INX_json_parser_str_ARG_parse_output2 3
#define INX_json_parser_str_ARG_parse_output3 4
#define INX_json_parser_str_ARG_parse_output4 5
#define INX_json_parser_str_ARG_parse_output5 6
#define INX_json_parser_str_ARG_parse_output6 7
#define INX_json_parser_str_ARG_parse_output7 8
#define INX_json_parser_str_ARG_parse_output8 9
#define INX_json_parser_str_ARG_parse_output9 10
#define INX_json_parser_str_ARG_parse_output10 11
#define INX_json_parser_str_ARG_parse_output11 12
#define INX_json_parser_str_ARG_parse_output12 13
#define INX_json_parser_str_ARG_parse_output13 14
#define INX_json_parser_str_ARG_parse_output14 15
#define INX_json_parser_str_ARG_parse_output15 16
#define INX_json_parser_str_ARG_parse_output16 17
#define INX_json_parser_str_ARG_parse_done 1
#define INX_json_parser_str_ARG_parse_error 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_json_parser_str_file default.js
#define INX_FB_json_parser_str_k1 
#define INX_FB_json_parser_str_k2 
#define INX_FB_json_parser_str_k3 
#define INX_FB_json_parser_str_k4 
#define INX_FB_json_parser_str_k5 
#define INX_FB_json_parser_str_k6 
#define INX_FB_json_parser_str_k7 
#define INX_FB_json_parser_str_k8 
#define INX_FB_json_parser_str_k9 
#define INX_FB_json_parser_str_k10 
#define INX_FB_json_parser_str_k11 
#define INX_FB_json_parser_str_k12 
#define INX_FB_json_parser_str_k13 
#define INX_FB_json_parser_str_k14 
#define INX_FB_json_parser_str_k15 
#define INX_FB_json_parser_str_k16 
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(json_parser_str)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
    ehs_char* file;
    ehs_char* k1;
    ehs_char* k2;
    ehs_char* k3;
    ehs_char* k4;
    ehs_char* k5;
    ehs_char* k6;
    ehs_char* k7;
    ehs_char* k8;
    ehs_char* k9;
    ehs_char* k10;
    ehs_char* k11;
    ehs_char* k12;
    ehs_char* k13;
    ehs_char* k14;
    ehs_char* k15;
    ehs_char* k16;
    EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",&file,&k1,&k2,&k3,&k4,&k5,&k6,&k7,&k8,&k9,&k10,&k11,&k12,&k13,&k14,&k15,&k16); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_json_parser_str_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(json_parser_str)
{
    ehs_char *pFbInitParam = (ehs_char *) EHS_FB_INIT_PARAMETERS;
    ehs_uint8 i = 0;
    ehs_char *param_array[17] = {0};
    ehs_bool bRet = EHS_TRUE; /* assume success */
    //this is the reference to the object data for this instance of the function block
    inx_json_parser_str_state_type* inx_json_parser_str_state = (inx_json_parser_str_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
    i = EhsGetWordsFromString(param_array, pFbInitParam, 17);
    // Init failed if no parameter is parsed
    if (i == 0) return EHS_FALSE;
    inx_json_parser_str_state->file = (ehs_char *) param_array[0];
    for (i = 1 ; i < 17 ; i++) inx_json_parser_str_state->keys[i - 1] = (ehs_char *) param_array[i];

    /* Add any further intialisation code here */
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(json_parser_str)
{
    inx_json_parser_str_state_type *inx_json_parser_str_state = (inx_json_parser_str_state_type*)EHS_FB_DESTROY_CONTEXT;
    //Your code below here
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION parse MACRO START -- DO NOT ALTER
/**
 * Definition of json_parser_str_parse.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(json_parser_str_parse)
{
    inx_json_parser_str_state_type* inx_json_parser_str_state = (inx_json_parser_str_state_type*)EHS_FB_RUN_CONTEXT;
    ehs_uint8 count = 0;
    ehs_uint8 i = 0;
    ehs_sint32 n_elements = 0;
    ehs_uint8 ret_code = 0;
    ehs_sint32 key_index = 0;
    ehs_sint32 value_index = 0;
    EhsDataflowStringType json_string = NULL;
    EhsDataflowIntType err_no = 0;
    ehs_FILE *fp = NULL;
    ehs_uint8 output[16] = {
        INX_json_parser_str_ARG_parse_output1, INX_json_parser_str_ARG_parse_output2,
        INX_json_parser_str_ARG_parse_output3, INX_json_parser_str_ARG_parse_output4,
        INX_json_parser_str_ARG_parse_output5, INX_json_parser_str_ARG_parse_output6,
        INX_json_parser_str_ARG_parse_output7, INX_json_parser_str_ARG_parse_output8,
        INX_json_parser_str_ARG_parse_output9, INX_json_parser_str_ARG_parse_output10,
        INX_json_parser_str_ARG_parse_output11, INX_json_parser_str_ARG_parse_output12,
        INX_json_parser_str_ARG_parse_output13, INX_json_parser_str_ARG_parse_output14,
        INX_json_parser_str_ARG_parse_output15, INX_json_parser_str_ARG_parse_output16
    };
    ehs_char *temp_char = NULL;
    jsmntok_t *json_array = NULL;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_str_ARG_parse_input_str))
        json_string = EHS_FB_IN_S_API2(INX_json_parser_str_ARG_parse_input_str) ;
    if (inx_json_parser_str_state->file == NULL && json_string == NULL)
    {
        err_no = -1;
        goto function_end;
    }
    // Read the file if json_string is NULL
    if (json_string == NULL)
    {
        fp = Ehs_UserFopen(inx_json_parser_str_state->file, "r");
        // File open failed
        if (fp == NULL)
        {
            err_no = -2;
            goto function_end;
        }
        json_string = EhsFreadDynamic(fp, &ret_code);
        EhsFclose(fp);
        switch (ret_code)
        {
            case 1:
            {
                err_no = -3;
                goto function_end;
            }
            case 2:
            {
                err_no = -4;
                goto function_end;
            }
            default:
            break;
        }
    }
    EhsDataflowStringType session[16] = {
        (EhsDataflowStringType ) inx_json_parser_str_state->keys[0],
        (EhsDataflowStringType ) inx_json_parser_str_state->keys[1],
        (EhsDataflowStringType ) inx_json_parser_str_state->keys[2],
        (EhsDataflowStringType ) inx_json_parser_str_state->keys[3],
        (EhsDataflowStringType ) inx_json_parser_str_state->keys[4],
        (EhsDataflowStringType ) inx_json_parser_str_state->keys[5],
        (EhsDataflowStringType ) inx_json_parser_str_state->keys[6],
        (EhsDataflowStringType ) inx_json_parser_str_state->keys[7],
        (EhsDataflowStringType ) inx_json_parser_str_state->keys[8],
        (EhsDataflowStringType ) inx_json_parser_str_state->keys[9],
        (EhsDataflowStringType ) inx_json_parser_str_state->keys[10],
        (EhsDataflowStringType ) inx_json_parser_str_state->keys[11],
        (EhsDataflowStringType ) inx_json_parser_str_state->keys[12],
        (EhsDataflowStringType ) inx_json_parser_str_state->keys[13],
        (EhsDataflowStringType ) inx_json_parser_str_state->keys[14],
        (EhsDataflowStringType ) inx_json_parser_str_state->keys[15]
    };

    // Your code here
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_str_ARG_parse_key1))
        session[0] = EHS_FB_IN_S_API2(INX_json_parser_str_ARG_parse_key1) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_str_ARG_parse_key2))
        session[1] = EHS_FB_IN_S_API2(INX_json_parser_str_ARG_parse_key2) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_str_ARG_parse_key3))
        session[2] = EHS_FB_IN_S_API2(INX_json_parser_str_ARG_parse_key3) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_str_ARG_parse_key4))
        session[3] = EHS_FB_IN_S_API2(INX_json_parser_str_ARG_parse_key4) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_str_ARG_parse_key5))
        session[4] = EHS_FB_IN_S_API2(INX_json_parser_str_ARG_parse_key5) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_str_ARG_parse_key6))
        session[5] = EHS_FB_IN_S_API2(INX_json_parser_str_ARG_parse_key6) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_str_ARG_parse_key7))
        session[6] = EHS_FB_IN_S_API2(INX_json_parser_str_ARG_parse_key7) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_str_ARG_parse_key8))
        session[7] = EHS_FB_IN_S_API2(INX_json_parser_str_ARG_parse_key8) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_str_ARG_parse_key9))
        session[8] = EHS_FB_IN_S_API2(INX_json_parser_str_ARG_parse_key9) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_str_ARG_parse_key10))
        session[9] = EHS_FB_IN_S_API2(INX_json_parser_str_ARG_parse_key10) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_str_ARG_parse_key11))
        session[10] = EHS_FB_IN_S_API2(INX_json_parser_str_ARG_parse_key11) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_str_ARG_parse_key12))
        session[11] = EHS_FB_IN_S_API2(INX_json_parser_str_ARG_parse_key12) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_str_ARG_parse_key13))
        session[12] = EHS_FB_IN_S_API2(INX_json_parser_str_ARG_parse_key13) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_str_ARG_parse_key14))
        session[13] = EHS_FB_IN_S_API2(INX_json_parser_str_ARG_parse_key14) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_str_ARG_parse_key15))
        session[14] = EHS_FB_IN_S_API2(INX_json_parser_str_ARG_parse_key15) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_str_ARG_parse_key16))
        session[15] = EHS_FB_IN_S_API2(INX_json_parser_str_ARG_parse_key16) ;

    // Parse the json string into the jsmn array
    json_array = parseKeyValuePair(json_string, &n_elements, &ret_code);
    if (ret_code != 0)
    {
        err_no = -5;
        goto function_end;
    }

    // Find the value from the key
    for (i = 0 ; i < 16 ; i++)
    {
        if (session[i] == NULL)
        {
            continue;
        }
        ret_code = findKeyValue(json_array, n_elements, json_string, session[i], &key_index, &value_index, &temp_char);
        if (ret_code != 0)
        {
            err_no = -100 - (i + 1);
            goto function_end;
        }
        // printf("%s %d-%d %s: %s\n", json_string, json_array[value_index].start, json_array[value_index].end, session[i], temp_char);
        switch (json_array[value_index].type)
        {
            case JSMN_STRING:
            {
                // Copy the string to the output buffer
                if (EHS_FB_OUT_CONNECTED_API2(output[i]))
                {
                    EhsStrcpy(EHS_FB_OUT_S_API2(output[i]), temp_char);
                }
                if (temp_char != NULL)
                {
                    EhsHMem_tempFree(temp_char);
                    temp_char = NULL;
                }
                break;
            }
            default:
            {
                // Not definitely not the string value. Abort parsing and return error
                err_no = i + 1;
                goto function_end;
            }
        }
    }

    // Final output and cleanup
function_end:
    if (json_array != NULL)
    {
        EhsHMem_tempFree(json_array);
        json_array = NULL;
    }
    // If there is no input string and the json string is not NULL, then it should be deallocated
    if (!EHS_FB_IN_CONNECTED_API2(INX_json_parser_str_ARG_parse_input_str) && json_string != NULL)
    {
        EhsHMem_tempFree(json_string);
        json_string = NULL;
    }

    EHS_FB_FINISH(INX_json_parser_str_ARG_parse_done);
    if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_str_ARG_parse_err_no))
        EHS_FB_OUT_I_API2(INX_json_parser_str_ARG_parse_err_no) = err_no;
    if (err_no != 0) EHS_FB_FINISH(INX_json_parser_str_ARG_parse_error);
}//ICB FUNCTION parse MACRO END -- DO NOT ALTER THIS LINE
