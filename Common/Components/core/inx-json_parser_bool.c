//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-json_parser_bool.h"
#include "jsmn_utils.h"

#ifndef JSMN_HEADER
#define JSMN_HEADER
#endif
#include "jsmn.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_json_parser_bool_state
{
    ehs_char *file;
    ehs_char *keys[16];
} inx_json_parser_bool_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(json_parser_bool)
EHS_FB_FUNCTION_ENTRY("parse", 0x01, json_parser_bool_parse)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_json_parser_bool_ARG_parse_input_str 1
#define INX_json_parser_bool_ARG_parse_key1 2
#define INX_json_parser_bool_ARG_parse_key2 3
#define INX_json_parser_bool_ARG_parse_key3 4
#define INX_json_parser_bool_ARG_parse_key4 5
#define INX_json_parser_bool_ARG_parse_key5 6
#define INX_json_parser_bool_ARG_parse_key6 7
#define INX_json_parser_bool_ARG_parse_key7 8
#define INX_json_parser_bool_ARG_parse_key8 9
#define INX_json_parser_bool_ARG_parse_key9 10
#define INX_json_parser_bool_ARG_parse_key10 11
#define INX_json_parser_bool_ARG_parse_key11 12
#define INX_json_parser_bool_ARG_parse_key12 13
#define INX_json_parser_bool_ARG_parse_key13 14
#define INX_json_parser_bool_ARG_parse_key14 15
#define INX_json_parser_bool_ARG_parse_key15 16
#define INX_json_parser_bool_ARG_parse_key16 17
#define INX_json_parser_bool_ARG_parse_err_no 1
#define INX_json_parser_bool_ARG_parse_output1 2
#define INX_json_parser_bool_ARG_parse_output2 3
#define INX_json_parser_bool_ARG_parse_output3 4
#define INX_json_parser_bool_ARG_parse_output4 5
#define INX_json_parser_bool_ARG_parse_output5 6
#define INX_json_parser_bool_ARG_parse_output6 7
#define INX_json_parser_bool_ARG_parse_output7 8
#define INX_json_parser_bool_ARG_parse_output8 9
#define INX_json_parser_bool_ARG_parse_output9 10
#define INX_json_parser_bool_ARG_parse_output10 11
#define INX_json_parser_bool_ARG_parse_output11 12
#define INX_json_parser_bool_ARG_parse_output12 13
#define INX_json_parser_bool_ARG_parse_output13 14
#define INX_json_parser_bool_ARG_parse_output14 15
#define INX_json_parser_bool_ARG_parse_output15 16
#define INX_json_parser_bool_ARG_parse_output16 17
#define INX_json_parser_bool_ARG_parse_done 1
#define INX_json_parser_bool_ARG_parse_error 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_json_parser_bool_file default.js
#define INX_FB_json_parser_bool_k1  0
#define INX_FB_json_parser_bool_k2  0
#define INX_FB_json_parser_bool_k3  0
#define INX_FB_json_parser_bool_k4  0
#define INX_FB_json_parser_bool_k5  0
#define INX_FB_json_parser_bool_k6  0
#define INX_FB_json_parser_bool_k7  0
#define INX_FB_json_parser_bool_k8  0
#define INX_FB_json_parser_bool_k9  0
#define INX_FB_json_parser_bool_k10 0
#define INX_FB_json_parser_bool_k11 0
#define INX_FB_json_parser_bool_k12 0
#define INX_FB_json_parser_bool_k13 0
#define INX_FB_json_parser_bool_k14 0
#define INX_FB_json_parser_bool_k15 0
#define INX_FB_json_parser_bool_k16 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(json_parser_bool)
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
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_json_parser_bool_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(json_parser_bool)
{
    ehs_char *pFbInitParam = (ehs_char *) EHS_FB_INIT_PARAMETERS;
    ehs_uint8 i = 0;
    ehs_char *param_array[17] = {0};
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_json_parser_bool_state_type* inx_json_parser_bool_state = (inx_json_parser_bool_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
    i = EhsGetWordsFromString(param_array, pFbInitParam, 17);
    // Init failed if no parameter is parsed
    if (i == 0) return EHS_FALSE;
    inx_json_parser_bool_state->file = (ehs_char *) param_array[0];
    for (i = 1 ; i < 17 ; i++) inx_json_parser_bool_state->keys[i - 1] = (ehs_char *) param_array[i];
	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(json_parser_bool)
{
	//inx_json_parser_bool_state_type *inx_json_parser_bool_state = (inx_json_parser_bool_state_type*)EHS_FB_DESTROY_CONTEXT;
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION parse MACRO START -- DO NOT ALTER
/**
 * Definition of json_parser_bool_parse.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(json_parser_bool_parse)
{
	inx_json_parser_bool_state_type* inx_json_parser_bool_state = (inx_json_parser_bool_state_type*)EHS_FB_RUN_CONTEXT;
    ehs_uint8 i = 0;
    ehs_sint32 n_elements = 0;
    ehs_uint8 ret_code = 0;
    ehs_sint32 key_index = 0;
    ehs_sint32 value_index = 0;
    EhsDataflowStringType json_string = NULL;
    EhsDataflowStringType json_slice_string = NULL; // Used to get the slice of JSON string pointer
    EhsDataflowIntType err_no = 0;
    ehs_FILE *fp = NULL;
    ehs_bool output_ok[16] = { EHS_FALSE };
    EhsDataflowBoolType output[16] = { 0 };
    // The extra size is to hold the parent tokens
    jsmntok_t json_array[EHS_JSMN_JSON_ARRAY_BUFFER_SIZE + EHS_JSMN_PARENT_LAYER_LIMIT];
    ehs_sint32 n_token_left = 0;
    ehs_jsmn_parent_t parents[EHS_JSMN_PARENT_LAYER_LIMIT] = { 0 };
    ehs_uint32 str_offset = 0;
    ehs_bool parent_added = EHS_FALSE;
    ehs_uint8 n_added_tokens = 0;
    if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_bool_ARG_parse_input_str))
        json_string = EHS_FB_IN_S_API2(INX_json_parser_bool_ARG_parse_input_str) ;
    if (inx_json_parser_bool_state->file == NULL && json_string == NULL)
    {
        err_no = -1;
        goto function_end;
    }
    //TODO Read the file if json_string is NULL
    if (json_string == NULL)
    {
        //TODO
        fp = Ehs_UserFopen(inx_json_parser_bool_state->file, "r");
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
        (EhsDataflowStringType ) inx_json_parser_bool_state->keys[0],
        (EhsDataflowStringType ) inx_json_parser_bool_state->keys[1],
        (EhsDataflowStringType ) inx_json_parser_bool_state->keys[2],
        (EhsDataflowStringType ) inx_json_parser_bool_state->keys[3],
        (EhsDataflowStringType ) inx_json_parser_bool_state->keys[4],
        (EhsDataflowStringType ) inx_json_parser_bool_state->keys[5],
        (EhsDataflowStringType ) inx_json_parser_bool_state->keys[6],
        (EhsDataflowStringType ) inx_json_parser_bool_state->keys[7],
        (EhsDataflowStringType ) inx_json_parser_bool_state->keys[8],
        (EhsDataflowStringType ) inx_json_parser_bool_state->keys[9],
        (EhsDataflowStringType ) inx_json_parser_bool_state->keys[10],
        (EhsDataflowStringType ) inx_json_parser_bool_state->keys[11],
        (EhsDataflowStringType ) inx_json_parser_bool_state->keys[12],
        (EhsDataflowStringType ) inx_json_parser_bool_state->keys[13],
        (EhsDataflowStringType ) inx_json_parser_bool_state->keys[14],
        (EhsDataflowStringType ) inx_json_parser_bool_state->keys[15]
    };

	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_bool_ARG_parse_key1))
		session[0] = EHS_FB_IN_S_API2(INX_json_parser_bool_ARG_parse_key1) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_bool_ARG_parse_key2))
		session[1] = EHS_FB_IN_S_API2(INX_json_parser_bool_ARG_parse_key2) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_bool_ARG_parse_key3))
		session[2] = EHS_FB_IN_S_API2(INX_json_parser_bool_ARG_parse_key3) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_bool_ARG_parse_key4))
		session[3] = EHS_FB_IN_S_API2(INX_json_parser_bool_ARG_parse_key4) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_bool_ARG_parse_key5))
		session[4] = EHS_FB_IN_S_API2(INX_json_parser_bool_ARG_parse_key5) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_bool_ARG_parse_key6))
		session[5] = EHS_FB_IN_S_API2(INX_json_parser_bool_ARG_parse_key6) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_bool_ARG_parse_key7))
		session[6] = EHS_FB_IN_S_API2(INX_json_parser_bool_ARG_parse_key7) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_bool_ARG_parse_key8))
		session[7] = EHS_FB_IN_S_API2(INX_json_parser_bool_ARG_parse_key8) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_bool_ARG_parse_key9))
		session[8] = EHS_FB_IN_S_API2(INX_json_parser_bool_ARG_parse_key9) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_bool_ARG_parse_key10))
		session[9] = EHS_FB_IN_S_API2(INX_json_parser_bool_ARG_parse_key10) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_bool_ARG_parse_key11))
		session[10] = EHS_FB_IN_S_API2(INX_json_parser_bool_ARG_parse_key11) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_bool_ARG_parse_key12))
		session[11] = EHS_FB_IN_S_API2(INX_json_parser_bool_ARG_parse_key12) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_bool_ARG_parse_key13))
		session[12] = EHS_FB_IN_S_API2(INX_json_parser_bool_ARG_parse_key13) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_bool_ARG_parse_key14))
		session[13] = EHS_FB_IN_S_API2(INX_json_parser_bool_ARG_parse_key14) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_bool_ARG_parse_key15))
		session[14] = EHS_FB_IN_S_API2(INX_json_parser_bool_ARG_parse_key15) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_json_parser_bool_ARG_parse_key16))
		session[15] = EHS_FB_IN_S_API2(INX_json_parser_bool_ARG_parse_key16) ;

    // Parse the json string into the jsmn array
    // Loop getting another buffer until no tokens left to be parsed
    for (
        n_token_left = parseKeyValuePair(json_string, json_array, EHS_JSMN_JSON_ARRAY_BUFFER_SIZE, &n_elements, &ret_code);
        n_token_left >= 0;
        n_token_left = parseKeyValuePair(&json_string[str_offset], json_array, EHS_JSMN_JSON_ARRAY_BUFFER_SIZE, &n_elements, &ret_code)
    )
    {
        if (ret_code != 0)
        {
            err_no = -5;
            goto function_end;
        }
        // Organise the key-value pairs in the json_array and cache parents
        organiseKeyValuePair(parents, json_array, n_elements, str_offset, &parent_added, &n_added_tokens);
        // Find the value from the key
        for (i = 0 ; i < 16 ; i++)
        {
            if (session[i] == NULL)
            {
                continue;
            }
            ret_code = findKeyValue(
                json_array + EHS_JSMN_PARENT_LAYER_LIMIT - n_added_tokens, 
                n_elements + n_added_tokens, 
                json_string, session[i], &key_index, &value_index, NULL);
            if (ret_code != 0)
            {
                continue;
            }
            value_index += EHS_JSMN_PARENT_LAYER_LIMIT - n_added_tokens;
            json_slice_string = (EhsDataflowStringType)(json_string + json_array[value_index].start);
            // printf("[%s] %s %d-%d %s: %.*s\n", __func__, json_string, json_array[value_index].start, json_array[value_index].end, session[i], json_array[value_index].end - json_array[value_index].start, &json_string[json_array[value_index].start]);
            switch (json_array[value_index].type)
            {
                case JSMN_PRIMITIVE:
                {
                    // It could be integer, float or boolean (true/false)
                    // Determine whether it's float
                    if (EhsMemchr(json_slice_string, '.', json_array[value_index].end - json_array[value_index].start) != NULL)
                    {
                        continue; // It is definitely not an integer value, continue to the next key
                    }
                    // Determine whether it's boolean or something else
                    switch (json_slice_string[0])
                    {
                        case 't':
                        case 'T':
                        {
                            // It's boolean true
                            output[i] = EHS_TRUE;
                            break;
                        }
                        case 'f':
                        case 'F':
                        {
                            // It's boolean false
                            output[i] = EHS_FALSE;
                            break;
                        }
                        case '\0':
                        case ' ':
                        case '\f':
                        case '\n':
                        case '\r':
                        case '\v':
                        case '\t':
                        default:
                        {
                            continue; // It is definitely not the boolean value, continue to the next key
                        }
                    }
                    output_ok[i] = EHS_TRUE;
                    break;
                }
                default:
                {
                    // Definitely not the integer value. continue to the next key
                    continue;
                }
            }
        }
        str_offset = json_array[EHS_JSMN_PARENT_LAYER_LIMIT+n_elements-1].end+1;
        if (n_token_left == 0) break;
    }
    if (n_token_left < 0)
    {
        err_no = -6;
        goto function_end;
    }

    // Final output and cleanup
function_end:
    // If there is no input string and the json string is not NULL, then it should be deallocated
    if (!EHS_FB_IN_CONNECTED_API2(INX_json_parser_bool_ARG_parse_input_str) && json_string != NULL)
    {
        EhsHMem_tempFree(json_string);
        json_string = NULL;
    }
	if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_bool_ARG_parse_err_no))
		EHS_FB_OUT_I_API2(INX_json_parser_bool_ARG_parse_err_no) = err_no;
	if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_bool_ARG_parse_output1) && output_ok[0])
		EHS_FB_OUT_B_API2(INX_json_parser_bool_ARG_parse_output1) = output[0];
	if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_bool_ARG_parse_output2) && output_ok[1])
		EHS_FB_OUT_B_API2(INX_json_parser_bool_ARG_parse_output2) = output[1];
	if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_bool_ARG_parse_output3) && output_ok[2])
		EHS_FB_OUT_B_API2(INX_json_parser_bool_ARG_parse_output3) = output[2];
	if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_bool_ARG_parse_output4) && output_ok[3])
		EHS_FB_OUT_B_API2(INX_json_parser_bool_ARG_parse_output4) = output[3];
	if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_bool_ARG_parse_output5) && output_ok[4])
		EHS_FB_OUT_B_API2(INX_json_parser_bool_ARG_parse_output5) = output[4];
	if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_bool_ARG_parse_output6) && output_ok[5])
		EHS_FB_OUT_B_API2(INX_json_parser_bool_ARG_parse_output6) = output[5];
	if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_bool_ARG_parse_output7) && output_ok[6])
		EHS_FB_OUT_B_API2(INX_json_parser_bool_ARG_parse_output7) = output[6];
	if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_bool_ARG_parse_output8) && output_ok[7])
		EHS_FB_OUT_B_API2(INX_json_parser_bool_ARG_parse_output8) = output[7];
	if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_bool_ARG_parse_output9) && output_ok[8])
		EHS_FB_OUT_B_API2(INX_json_parser_bool_ARG_parse_output9) = output[8];
	if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_bool_ARG_parse_output10) && output_ok[9])
		EHS_FB_OUT_B_API2(INX_json_parser_bool_ARG_parse_output10) = output[9];
	if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_bool_ARG_parse_output11) && output_ok[10])
		EHS_FB_OUT_B_API2(INX_json_parser_bool_ARG_parse_output11) = output[10];
	if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_bool_ARG_parse_output12) && output_ok[11])
		EHS_FB_OUT_B_API2(INX_json_parser_bool_ARG_parse_output12) = output[11];
	if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_bool_ARG_parse_output13) && output_ok[12])
		EHS_FB_OUT_B_API2(INX_json_parser_bool_ARG_parse_output13) = output[12];
	if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_bool_ARG_parse_output14) && output_ok[13])
		EHS_FB_OUT_B_API2(INX_json_parser_bool_ARG_parse_output14) = output[13];
	if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_bool_ARG_parse_output15) && output_ok[14])
		EHS_FB_OUT_B_API2(INX_json_parser_bool_ARG_parse_output15) = output[14];
	if (EHS_FB_OUT_CONNECTED_API2(INX_json_parser_bool_ARG_parse_output16) && output_ok[15])
		EHS_FB_OUT_B_API2(INX_json_parser_bool_ARG_parse_output16) = output[15];
	EHS_FB_FINISH(INX_json_parser_bool_ARG_parse_done);
	if (err_no != 0) EHS_FB_FINISH(INX_json_parser_bool_ARG_parse_error);
}//ICB FUNCTION parse MACRO END -- DO NOT ALTER THIS LINE
