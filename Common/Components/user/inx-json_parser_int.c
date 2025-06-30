//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-json_parser_int.h"
#include "jsmn_utils.h"
#ifndef JSMN_HEADER
#define JSMN_HEADER
#endif
#include "jsmn.h"
#include <limits.h>
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_json_parser_int_state
{
    ehs_char *file;
    ehs_char *keys[16];
} inx_json_parser_int_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(json_parser_int)
EHS_FB_FUNCTION_ENTRY("parse", 0x01, json_parser_int_parse)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_json_parser_int_ARG_parse_input_str 1
#define INX_json_parser_int_ARG_parse_key1 2
#define INX_json_parser_int_ARG_parse_key2 3
#define INX_json_parser_int_ARG_parse_key3 4
#define INX_json_parser_int_ARG_parse_key4 5
#define INX_json_parser_int_ARG_parse_key5 6
#define INX_json_parser_int_ARG_parse_key6 7
#define INX_json_parser_int_ARG_parse_key7 8
#define INX_json_parser_int_ARG_parse_key8 9
#define INX_json_parser_int_ARG_parse_key9 10
#define INX_json_parser_int_ARG_parse_key10 11
#define INX_json_parser_int_ARG_parse_key11 12
#define INX_json_parser_int_ARG_parse_key12 13
#define INX_json_parser_int_ARG_parse_key13 14
#define INX_json_parser_int_ARG_parse_key14 15
#define INX_json_parser_int_ARG_parse_key15 16
#define INX_json_parser_int_ARG_parse_key16 17
#define INX_json_parser_int_ARG_parse_err_no 1
#define INX_json_parser_int_ARG_parse_output1 2
#define INX_json_parser_int_ARG_parse_output2 3
#define INX_json_parser_int_ARG_parse_output3 4
#define INX_json_parser_int_ARG_parse_output4 5
#define INX_json_parser_int_ARG_parse_output5 6
#define INX_json_parser_int_ARG_parse_output6 7
#define INX_json_parser_int_ARG_parse_output7 8
#define INX_json_parser_int_ARG_parse_output8 9
#define INX_json_parser_int_ARG_parse_output9 10
#define INX_json_parser_int_ARG_parse_output10 11
#define INX_json_parser_int_ARG_parse_output11 12
#define INX_json_parser_int_ARG_parse_output12 13
#define INX_json_parser_int_ARG_parse_output13 14
#define INX_json_parser_int_ARG_parse_output14 15
#define INX_json_parser_int_ARG_parse_output15 16
#define INX_json_parser_int_ARG_parse_output16 17
#define INX_json_parser_int_ARG_parse_done 1
#define INX_json_parser_int_ARG_parse_error 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_json_parser_int_file  ""
#define INX_FB_json_parser_int_k1  ""
#define INX_FB_json_parser_int_k2  ""
#define INX_FB_json_parser_int_k3  ""
#define INX_FB_json_parser_int_k4  ""
#define INX_FB_json_parser_int_k5  ""
#define INX_FB_json_parser_int_k6  ""
#define INX_FB_json_parser_int_k7  ""
#define INX_FB_json_parser_int_k8  ""
#define INX_FB_json_parser_int_k9  ""
#define INX_FB_json_parser_int_k10 ""
#define INX_FB_json_parser_int_k11 ""
#define INX_FB_json_parser_int_k12 ""
#define INX_FB_json_parser_int_k13 ""
#define INX_FB_json_parser_int_k14 ""
#define INX_FB_json_parser_int_k15 ""
#define INX_FB_json_parser_int_k16 ""
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(json_parser_int)
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
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_json_parser_int_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(json_parser_int)
{
    ehs_char *pFbInitParam = (ehs_char *) EHS_FB_INIT_PARAMETERS;
    ehs_uint8 i = 0;
    ehs_char *param_array[17] = {0};
    ehs_bool bRet = EHS_TRUE; /* assume success */
    //this is the reference to the object data for this instance of the function block
    inx_json_parser_int_state_type* inx_json_parser_int_state = (inx_json_parser_int_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
    i = EhsGetWordsFromString(param_array, pFbInitParam, 17);
    // Init failed if no parameter is parsed
    if (i == 0) return EHS_FALSE;
    inx_json_parser_int_state->file = (ehs_char *) param_array[0];
    for (i = 1 ; i < 17 ; i++) inx_json_parser_int_state->keys[i - 1] = (ehs_char *) param_array[i];
    /* Add any further intialisation code here */
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(json_parser_int)
{
    inx_json_parser_int_state_type *inx_json_parser_int_state = (inx_json_parser_int_state_type*)EHS_FB_DESTROY_CONTEXT;
    //Your code below here
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION parse MACRO START -- DO NOT ALTER
/**
 * Definition of json_parser_int_parse.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(json_parser_int_parse)
{
    inx_json_parser_int_state_type* inx_json_parser_int_state = (inx_json_parser_int_state_type*)EHS_FB_RUN_CONTEXT;
    ehs_uint8 count = 0;
    ehs_uint8 i = 0;
    ehs_sint32 n_elements = 0;
    ehs_uint8 ret_code = 0;
    ehs_sint32 key_index = 0;
    ehs_sint32 value_index = 0;
    EhsDataflowStringType json_string = NULL; //TPDPURGENT THIS WAS A double pointer - looks like it should be a single POINTER. What do we want.
    EhsDataflowIntType err_no = 0;
    ehs_char *int_end = NULL;
    long integer_long = 0;
    ehs_FILE *fp = NULL;
    ehs_bool output_ok[16] = { EHS_FALSE };
    EhsDataflowIntType output[16] = { 0 };
    ehs_char *temp_char = NULL;
    jsmntok_t *json_array = NULL;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_int_ARG_parse_input_str))
        json_string = EHS_FB_IN_S_API2(INX_json_parser_int_ARG_parse_input_str) ;
    if (inx_json_parser_int_state->file == NULL && json_string == NULL)
    {
        err_no = -1;
        goto function_end;
    }
    //TODO Read the file if json_string is NULL?
    if (json_string == NULL)
    {
        //TODO
        fp = Ehs_UserFopen(inx_json_parser_int_state->file, "r");
        // File open failed
        if (fp == NULL)
        {
            err_no = -2;
            goto function_end;
        }
        json_string = EhsFreadDynamic(fp, &ret_code); // Do we really want to do this? We should avoiding mallocs as the main reason for using this methof key values pairs is to avoid mallocs.
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
    EhsDataflowStringType session[16] = {//TODOURGENT REMOVED * from all the following type converters.
        (EhsDataflowStringType ) inx_json_parser_int_state->keys[0],
        (EhsDataflowStringType ) inx_json_parser_int_state->keys[1],
        (EhsDataflowStringType ) inx_json_parser_int_state->keys[2],
        (EhsDataflowStringType ) inx_json_parser_int_state->keys[3],
        (EhsDataflowStringType ) inx_json_parser_int_state->keys[4],
        (EhsDataflowStringType ) inx_json_parser_int_state->keys[5],
        (EhsDataflowStringType ) inx_json_parser_int_state->keys[6],
        (EhsDataflowStringType ) inx_json_parser_int_state->keys[7],
        (EhsDataflowStringType ) inx_json_parser_int_state->keys[8],
        (EhsDataflowStringType ) inx_json_parser_int_state->keys[9],
        (EhsDataflowStringType ) inx_json_parser_int_state->keys[10],
        (EhsDataflowStringType ) inx_json_parser_int_state->keys[11],
        (EhsDataflowStringType ) inx_json_parser_int_state->keys[12],
        (EhsDataflowStringType ) inx_json_parser_int_state->keys[13],
        (EhsDataflowStringType ) inx_json_parser_int_state->keys[14],
        (EhsDataflowStringType ) inx_json_parser_int_state->keys[15]
    };

    // Assign the key inputs
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_int_ARG_parse_key1))
        session[0] = EHS_FB_IN_S_API2(INX_json_parser_int_ARG_parse_key1) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_int_ARG_parse_key2))
        session[1] = EHS_FB_IN_S_API2(INX_json_parser_int_ARG_parse_key2) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_int_ARG_parse_key3))
        session[2] = EHS_FB_IN_S_API2(INX_json_parser_int_ARG_parse_key3) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_int_ARG_parse_key4))
        session[3] = EHS_FB_IN_S_API2(INX_json_parser_int_ARG_parse_key4) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_int_ARG_parse_key5))
        session[4] = EHS_FB_IN_S_API2(INX_json_parser_int_ARG_parse_key5) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_int_ARG_parse_key6))
        session[5] = EHS_FB_IN_S_API2(INX_json_parser_int_ARG_parse_key6) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_int_ARG_parse_key7))
        session[6] = EHS_FB_IN_S_API2(INX_json_parser_int_ARG_parse_key7) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_int_ARG_parse_key8))
        session[7] = EHS_FB_IN_S_API2(INX_json_parser_int_ARG_parse_key8) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_int_ARG_parse_key9))
        session[8] = EHS_FB_IN_S_API2(INX_json_parser_int_ARG_parse_key9) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_int_ARG_parse_key10))
        session[9] = EHS_FB_IN_S_API2(INX_json_parser_int_ARG_parse_key10) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_int_ARG_parse_key11))
        session[10] = EHS_FB_IN_S_API2(INX_json_parser_int_ARG_parse_key11) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_int_ARG_parse_key12))
        session[11] = EHS_FB_IN_S_API2(INX_json_parser_int_ARG_parse_key12) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_int_ARG_parse_key13))
        session[12] = EHS_FB_IN_S_API2(INX_json_parser_int_ARG_parse_key13) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_int_ARG_parse_key14))
        session[13] = EHS_FB_IN_S_API2(INX_json_parser_int_ARG_parse_key14) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_int_ARG_parse_key15))
        session[14] = EHS_FB_IN_S_API2(INX_json_parser_int_ARG_parse_key15) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_int_ARG_parse_key16))
        session[15] = EHS_FB_IN_S_API2(INX_json_parser_int_ARG_parse_key16) ;

    // Parse the json string into the jsmn array
    json_array = parseKeyValuePair(json_string, &n_elements, &ret_code);
    if (ret_code != 0)
    {
        err_no = -5; //todo2025: let's have some MACROs or an ENUM to describe what this error means
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
            err_no = -100 - (i + 1); // todo2025 - would be good to have an enum (why - 100 for these errors? Hopefully in the HTML)
            goto function_end; //todo2025 - why do we want to bail here? is because the key doesn't exist, or there is some other error. I think we probably just need to set an error flag and asseert an error number (as suggested in the helpfile)
        }
        switch (json_array[value_index].type)
        {
            case JSMN_PRIMITIVE:
            {
                // It could be integer, float or boolean (true/false)
                // Determine whether it's float
                if (EhsStrchr(temp_char, '.') != NULL)
                {
                    if (temp_char != NULL)
                    {
                        EhsHMem_tempFree(temp_char);
                        temp_char = NULL;
                    }
                    err_no = i + 1;
                    goto function_end;
                }
                // Determine whether it's boolean or something else
                switch (temp_char[0])
                {
                    case 't':
                    case 'f':
                    case 'T':
                    case 'F':
                    case '\0':
                    case ' ':
                    case '\f':
                    case '\n':
                    case '\r':
                    case '\v':
                    case '\t':
                    {
                        if (temp_char != NULL)
                        {
                            EhsHMem_tempFree(temp_char);
                            temp_char = NULL;
                        }
                        err_no = i + 1;
                        goto function_end;
                    }
                    default:
                    break;
                }
                // printf("%s %d-%d %s: %s\n", json_string, json_array[value_index].start, json_array[value_index].end, session[i], temp_char);
                // Convert the string to integer
                integer_long = strtol(temp_char, &int_end, 10);
                if (temp_char != NULL)
                {
                    EhsHMem_tempFree(temp_char);
                    temp_char = NULL;
                }
                if (integer_long > INT_MAX || integer_long< INT_MIN || int_end[0] != '\0')
                {
                    err_no = i + 1;
                    goto function_end;
                }
                output[i] =  integer_long;
                output_ok[i] = EHS_TRUE;
                break;
            }
            default:
            {
                // Not definitely not the integer value. Abort parsing and return error
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
    if (!EHS_FB_IN_CONNECTED_API2(INX_json_parser_int_ARG_parse_input_str) && json_string != NULL)
    {
        EhsHMem_tempFree(json_string);
        json_string = NULL;
    }
    if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_int_ARG_parse_output1) && output_ok[0])
        EHS_FB_OUT_I_API2(INX_json_parser_int_ARG_parse_output1) = output[0];
    if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_int_ARG_parse_output2) && output_ok[1])
        EHS_FB_OUT_I_API2(INX_json_parser_int_ARG_parse_output2) = output[1];
    if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_int_ARG_parse_output3) && output_ok[2])
        EHS_FB_OUT_I_API2(INX_json_parser_int_ARG_parse_output3) = output[2];
    if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_int_ARG_parse_output4) && output_ok[3])
        EHS_FB_OUT_I_API2(INX_json_parser_int_ARG_parse_output4) = output[3];
    if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_int_ARG_parse_output5) && output_ok[4])
        EHS_FB_OUT_I_API2(INX_json_parser_int_ARG_parse_output5) = output[4];
    if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_int_ARG_parse_output6) && output_ok[5])
        EHS_FB_OUT_I_API2(INX_json_parser_int_ARG_parse_output6) = output[5];
    if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_int_ARG_parse_output7) && output_ok[6])
        EHS_FB_OUT_I_API2(INX_json_parser_int_ARG_parse_output7) = output[6];
    if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_int_ARG_parse_output8) && output_ok[7])
        EHS_FB_OUT_I_API2(INX_json_parser_int_ARG_parse_output8) = output[7];
    if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_int_ARG_parse_output9) && output_ok[8])
        EHS_FB_OUT_I_API2(INX_json_parser_int_ARG_parse_output9) = output[8];
    if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_int_ARG_parse_output10) && output_ok[9])
        EHS_FB_OUT_I_API2(INX_json_parser_int_ARG_parse_output10) = output[9];
    if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_int_ARG_parse_output11) && output_ok[10])
        EHS_FB_OUT_I_API2(INX_json_parser_int_ARG_parse_output11) = output[10];
    if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_int_ARG_parse_output12) && output_ok[11])
        EHS_FB_OUT_I_API2(INX_json_parser_int_ARG_parse_output12) = output[11];
    if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_int_ARG_parse_output13) && output_ok[12])
        EHS_FB_OUT_I_API2(INX_json_parser_int_ARG_parse_output13) = output[12];
    if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_int_ARG_parse_output14) && output_ok[13])
        EHS_FB_OUT_I_API2(INX_json_parser_int_ARG_parse_output14) = output[13];
    if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_int_ARG_parse_output15) && output_ok[14])
        EHS_FB_OUT_I_API2(INX_json_parser_int_ARG_parse_output15) = output[14];
    if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_int_ARG_parse_output16) && output_ok[15])
        EHS_FB_OUT_I_API2(INX_json_parser_int_ARG_parse_output16) = output[15];
    EHS_FB_FINISH(INX_json_parser_int_ARG_parse_done);
    if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_int_ARG_parse_err_no))
        EHS_FB_OUT_I_API2(INX_json_parser_int_ARG_parse_err_no) = err_no;
    if (err_no != 0) EHS_FB_FINISH(INX_json_parser_int_ARG_parse_error);
}//ICB FUNCTION parse MACRO END -- DO NOT ALTER THIS LINE
