//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-key_value_bool.h"
#define JSMN_HEADER
#include "jsmn.h"
#include "jsmn_utils.h"
#include "limits.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_key_value_bool_state
{
    ehs_char* domain;
    ehs_char* key;
    ehs_bool default_value;
} inx_key_value_bool_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(key_value_bool)
EHS_FB_FUNCTION_ENTRY("upsert", 0x01, key_value_bool_upsert)
EHS_FB_FUNCTION_ENTRY("query", 0x02, key_value_bool_query)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_key_value_bool_ARG_upsert_value_in 1
#define INX_key_value_bool_ARG_upsert_err_no 1
#define INX_key_value_bool_ARG_upsert_set_done 1
#define INX_key_value_bool_ARG_query_value_out 1
#define INX_key_value_bool_ARG_query_query_errno 2
#define INX_key_value_bool_ARG_query_get_done 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_key_value_bool_domain "default"
#define INX_FB_key_value_bool_key "default"
#define INX_FB_key_value_bool_default_value 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
#define INX_FB_key_value_bool_domain_ext ".js"
#define INX_FB_key_value_bool_buf_size_unit 32
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(key_value_bool)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
    ehs_char* domain;
    ehs_char* key;
    ehs_bool default_value;
    EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%s %s %d",&domain,&key,&default_value); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_key_value_bool_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(key_value_bool)
{
    //
    ehs_uint8 i = 0;
    //TODO place it back to be a pointer once it's fixed
    // ehs_char *in_temp = NULL;
    ehs_char in_temp[EHS_STRING_LENGTH_MAX]; //TODO:STRINGLENGTH! mallof on FTDI when port sizes are known.
    ehs_char *pFbInitParam = (ehs_char *) EHS_FB_INIT_PARAMETERS;
    ehs_uint16 str_count = 0;
    ehs_char *temp_param[2] = { 0 };
    ehs_uint8 temp_default = INX_FB_key_value_bool_default_value;
    ehs_bool bRet = EHS_TRUE; /* assume success */
    //this is the reference to the object data for this instance of the function block
    inx_key_value_bool_state_type* inx_key_value_bool_state = (inx_key_value_bool_state_type*)EHS_FB_INIT_CONTEXT;
    /* Allocate memory to read the parameter */
    //TODO place it back to EhsHMem_tempAlloc once it's fixed
    // in_temp = (ehs_char *) EhsHMem_permAlloc(EhsStrlen(EHS_FB_INIT_PARAMETERS) + 1);
    // Init error if there is no enough memory left to be allocated
    if (in_temp == NULL) return EHS_FALSE;
    /* read the initialisation parameters */
    for (i = 0 ; i < 2 && pFbInitParam ; i++)
    {
        EhsMemset(in_temp, '\0', str_count + 1);
        pFbInitParam = EhsStrTrimL(pFbInitParam);
        if (pFbInitParam[0] != '"')
        {
            /* When it's not a string containing spaces */
            pFbInitParam = EhsGetWordFromString(in_temp, pFbInitParam);
            str_count = EhsStrlen(in_temp);
            if (str_count == 4 && EhsStrncmp(in_temp, "NULL", 4) == 0)
            {
                temp_param[i] = NULL;
                continue;
            }
            else
            {
                goto jump_point;
            }
        }
        else
        {
            /* When it's a string containing spaces */
            pFbInitParam = EhsGetQuoteDelimFromString(in_temp, pFbInitParam, EHS_STRING_LENGTH_MAX);
            str_count = EhsStrlen(in_temp);
            goto jump_point;
        }
jump_point:
        if (str_count > 0)
        {
            // The memory allocated by `EhsHMem_writableAlloc` is freed after each app run
            temp_param[i] = EhsHMem_writeableAlloc(i != 0 ? str_count + 1 : str_count + 1 + EhsStrlen(INX_FB_key_value_bool_domain_ext));
            if (temp_param[i] != NULL)
            {
                EhsStrcpy(temp_param[i], in_temp);
                if (i == 0)
                {
                    EhsStrcat(temp_param[i], INX_FB_key_value_bool_domain_ext);
                    str_count += EhsStrlen(INX_FB_key_value_bool_domain_ext);
                }
                temp_param[i][str_count] = '\0';
            }
        }
        else temp_param[i] = NULL;
    }

    /* Assign the pointer to the parameters */
    inx_key_value_bool_state->domain = (ehs_char *) temp_param[0];
    inx_key_value_bool_state->key = (ehs_char *) temp_param[1];
    pFbInitParam = EhsGetUint8FromString(&temp_default, pFbInitParam);
    inx_key_value_bool_state->default_value = temp_default;


    /* Add any further intialisation code here */
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(key_value_bool)
{
    inx_key_value_bool_state_type *inx_key_value_bool_state = (inx_key_value_bool_state_type*)EHS_FB_DESTROY_CONTEXT;
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
static ehs_FILE *openKeyValuePairFile(const ehs_char *filename)
{
    ehs_FILE *fp = Ehs_UserFopen(filename, "r+");
    if (fp == NULL) fp = Ehs_UserFopen(filename, "w+");
    return fp;
}
//ICB FUNCTION upsert MACRO START -- DO NOT ALTER
/**
 * Definition of key_value_bool_upsert.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(key_value_bool_upsert)
{
    inx_key_value_bool_state_type* inx_key_value_bool_state = (inx_key_value_bool_state_type*)EHS_FB_RUN_CONTEXT;
    ehs_sint32 err_no = 0;
    ehs_char *domain = (ehs_char *) inx_key_value_bool_state->domain;
    ehs_char *key = (ehs_char *) inx_key_value_bool_state->key;
    ehs_bool value_temp = inx_key_value_bool_state->default_value;
    // Min value is "0". Max value is "1". String length with NULL terminator is max 2.
    ehs_char value[2];
    ehs_FILE *fp = NULL;
    ehs_char *content = NULL;
    EhsDataflowStringType json_slice_string = NULL;
    jsmntok_t array[EHS_JSMN_JSON_ARRAY_BUFFER_SIZE + EHS_JSMN_PARENT_LAYER_LIMIT];
    ehs_sint32 n_token_left = 0;
    ehs_jsmn_parent_t parents[EHS_JSMN_PARENT_LAYER_LIMIT] = { 0 };
    ehs_uint32 str_offset = 0;
    ehs_bool parent_added = EHS_FALSE;
    ehs_uint8 n_added_tokens = 0;
    ehs_sint32 item_size = 0;
    ehs_uint8 _err_code = 0;
    ehs_sint32 key_index = 0;
    ehs_sint32 value_index = 0;

    // Your code here
    // Open the config file within domain
    if (EHS_FB_IN_CONNECTED_API2(INX_key_value_bool_ARG_upsert_value_in))
        value_temp = EHS_FB_IN_B_API2(INX_key_value_bool_ARG_upsert_value_in) ;
    EhsSprintf(value, "%d", value_temp);
    if (domain == NULL)
        fp = openKeyValuePairFile(INX_FB_key_value_bool_domain INX_FB_key_value_bool_domain_ext);
    else
    {
        fp = openKeyValuePairFile(domain);
    }
    if (fp == NULL)
    {
        err_no = 1;
        EHSH_LOG_ERROR("Failed to open key-value pair file!\n");
        goto function_end;
    }

    /* Main code START */

    // Read from file
    // Note that we do the dynamic memory allocation because the upsert operation might update the file in place,
    //  which would either write to a second file, delete the original file and rename the second file to the original file name,
    //  or rewrite the original file in place with additional buffers to store the data after the updated key-value pair.
    // So dynamic memory allocation is the tradeoff between memory usage and meomry fragmentation.
    // The dynamic memory allocation is done in EhsFreadDynamic function.
    content = EhsFreadDynamic(fp, &_err_code);
    if (_err_code != 0)
    {
        EHSH_LOG_ERROR("Failed to read key-value pair file! Error (%d)\n", _err_code);
        err_no = 2;
        goto function_end;
    }
    if (fp == NULL)
    {
        EHSH_LOG_ERROR("File object pointer is NULL!\n");
        err_no = 6;
        goto function_end;
    }
    EhsFclose(fp);
    fp = NULL;

    // Re-open the file for writing
    if (domain == NULL)
        fp = Ehs_UserFopen(INX_FB_key_value_bool_domain INX_FB_key_value_bool_domain_ext, "w");
    else
    {
        fp = Ehs_UserFopen(domain, "w");
    }
    if (fp == NULL)
    {
        EHSH_LOG_ERROR("Failed to re-open key-value pair file!\n");
        err_no = 7;
        goto function_end;
    }

    // Parse the string
    for (
        n_token_left = parseKeyValuePair(content, array, EHS_JSMN_JSON_ARRAY_BUFFER_SIZE, &item_size, &_err_code);
        n_token_left >= 0;
        n_token_left = parseKeyValuePair(&content[str_offset], array, EHS_JSMN_JSON_ARRAY_BUFFER_SIZE, &item_size, &_err_code)
    )
    {
        if (_err_code != 0)
        {
            EHSH_LOG_ERROR("Failed to parse key-value pair! Error (%d)\n", _err_code);
            err_no = 3;
            goto function_end;
        }

        // Organise the key-value pairs in the json_array and cache parents
        organiseKeyValuePair(parents, array, item_size, str_offset, &parent_added, &n_added_tokens);
        
        // Find the key
        _err_code = findKeyValue(
            array + EHS_JSMN_PARENT_LAYER_LIMIT - n_added_tokens, 
            item_size + n_added_tokens, 
            content, key == NULL ? INX_FB_key_value_bool_key : key, 
            &key_index, &value_index, NULL);

        // Upsert the key value
        switch (_err_code)
        {
            case 0:
            {
                EHS_TRACE_MESSAGE("Key found! Key (%d) Value (%d)\n", key_index, value_index);
                upsertKeyValuePair(
                    content, array + EHS_JSMN_PARENT_LAYER_LIMIT - n_added_tokens, key_index, value_index, 
                    key != NULL ? key : INX_FB_key_value_bool_key, 
                    value, 
                    NULL, fp
                );
                goto function_end;
                break;
            }
            case 1:case 2:case 3:
            {
                // Treat as error only if there is no key left
                if (n_token_left > 0) break;
                EHSH_LOG_ERROR("Find Key pointer error! Error (%d)\n", _err_code);
                err_no = 4;
                goto function_end;
                break;
            }
            case 4:
            {
                // If it's an empty JSON, this loop only run once
                EHSH_LOG_WARNING("Empty JSON! Error (%d)\n", _err_code);
                if (item_size == 1)
                {
                    upsertKeyValuePair(
                        content, array + EHS_JSMN_PARENT_LAYER_LIMIT - n_added_tokens, key_index, value_index, 
                        key != NULL ? key : INX_FB_key_value_bool_key, 
                        value, 
                        NULL, fp
                    );
                    goto function_end;
                    break;
                }
                if (item_size == 0)
                {
                    EhsFprintf(fp,
                        "{\"%s\":\"%s\"}",
                        key == NULL ? INX_FB_key_value_bool_key : key,
                        value
                    );
                    goto function_end;
                    break;
                }
                EHSH_LOG_ERROR("Empty JSON input error!");
                goto function_end;
                break;
            }
            case 5:
            {
                // Treat as error only if there is no key left
                if (n_token_left > 0) break;
                EHSH_LOG_WARNING("Key not found! Error (%d)\n", _err_code);
                // Insert the new key-value pair
                upsertKeyValuePair(
                    content, array + EHS_JSMN_PARENT_LAYER_LIMIT - n_added_tokens, -1, -1, 
                    key != NULL ? key : INX_FB_key_value_bool_key, 
                    value, 
                    NULL, fp
                );
                break;
            }
            default:
            {
                // Treat as error only if there is no key left
                if (n_token_left > 0) break;
                EHSH_LOG_ERROR("Find Key unknown error! Error (%d)\n", _err_code);
                err_no = 5;
                goto function_end;
                break;
            }
        }

        json_slice_string = (EhsDataflowStringType)(content + array[value_index + EHS_JSMN_PARENT_LAYER_LIMIT - n_added_tokens].start);
        str_offset = array[EHS_JSMN_PARENT_LAYER_LIMIT+item_size-1].end+1;
        if (n_token_left == 0) break;
    }

    if (n_token_left < 0)
    {
        err_no = 8;
        goto function_end;
    }

    /* Main code ENDED */

function_end:
    if (fp != NULL)
    {
        EhsFclose(fp);
        fp = NULL;
    }
    if (content != NULL)
    {
        EhsHMem_tempFree(content);
        content = NULL;
    }
    if (EHS_FB_OUT_CONNECTED_API2(INX_key_value_bool_ARG_upsert_err_no))
        EHS_FB_OUT_I_API2(INX_key_value_bool_ARG_upsert_err_no) = err_no;
    EHS_FB_FINISH(INX_key_value_bool_ARG_upsert_set_done);
}//ICB FUNCTION upsert MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION query MACRO START -- DO NOT ALTER
/**
 * Definition of key_value_bool_query.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(key_value_bool_query)
{
    inx_key_value_bool_state_type* inx_key_value_bool_state = (inx_key_value_bool_state_type*)EHS_FB_RUN_CONTEXT;
    ehs_char *key = (ehs_char *) inx_key_value_bool_state->key;
    ehs_FILE *fp = NULL;
    ehs_char *content = NULL;
    EhsDataflowStringType json_slice_string = NULL;
    ehs_char *int_end = NULL;
    ehs_uint8 _err_code = 0;
    ehs_sint32 err_no = 0;
    ehs_sint32 temp_output = 0;
    jsmntok_t array[EHS_JSMN_JSON_ARRAY_BUFFER_SIZE + EHS_JSMN_PARENT_LAYER_LIMIT];
    ehs_sint32 n_token_left = 0;
    ehs_jsmn_parent_t parents[EHS_JSMN_PARENT_LAYER_LIMIT] = { 0 };
    ehs_uint32 str_offset = 0;
    ehs_bool parent_added = EHS_FALSE;
    ehs_uint8 n_added_tokens = 0;
    long integer_long = 0;

    ehs_sint32 item_size = 0;
    ehs_sint32 key_index = 0;
    ehs_sint32 value_index = 0;

    // Prepare the output with default value
    ehs_bool value_output = inx_key_value_bool_state->default_value;

    // Your code here
    if (inx_key_value_bool_state->domain == NULL) fp = Ehs_UserFopen(INX_FB_key_value_bool_domain INX_FB_key_value_bool_domain_ext, "r");
    else fp = Ehs_UserFopen(inx_key_value_bool_state->domain, "r");
    
    if (fp == NULL)
    {
        // Return default value if no file exists
        EHSH_LOG_WARNING("Key-value pair file not found!\n");
        err_no = -1;
        goto function_end;
    }

    content = EhsFreadDynamic(fp, &_err_code);
    if (_err_code != 0)
    {
        EHSH_LOG_ERROR("Failed to read key-value pair file! Error (%d)\n", _err_code);
        err_no = -2;
        goto function_end;
    }

    // Parse the string
    for (
        n_token_left = parseKeyValuePair(content, array, EHS_JSMN_JSON_ARRAY_BUFFER_SIZE, &item_size, &_err_code);
        n_token_left >= 0;
        n_token_left = parseKeyValuePair(&content[str_offset], array, EHS_JSMN_JSON_ARRAY_BUFFER_SIZE, &item_size, &_err_code)
    )
    {
        if (_err_code != 0)
        {
            EHSH_LOG_ERROR("Failed to parse key-value pair! Error (%d)\n", _err_code);
            err_no = -3;
            goto function_end;
        }
        _err_code = findKeyValue(
            array + EHS_JSMN_PARENT_LAYER_LIMIT - n_added_tokens, 
            item_size + n_added_tokens, 
            content, key == NULL ? INX_FB_key_value_bool_key : key, &key_index, &value_index, NULL);

        // Organise the key-value pairs in the json_array and cache parents
        organiseKeyValuePair(parents, array, item_size, str_offset, &parent_added, &n_added_tokens);

        // Find the key
        if (_err_code != 0)
        {
            EHSH_LOG_ERROR("Failed to find key-value pair! Error (%d)\n", _err_code);
            goto for_end;
        }
        value_index += EHS_JSMN_PARENT_LAYER_LIMIT - n_added_tokens;
        json_slice_string = (EhsDataflowStringType)(content + array[value_index].start);
        switch (array[value_index].type)
        {
            // All Key-Value pairs are stored as string
            case JSMN_STRING:
            {
                // It could be integer, float or boolean (true/false)
                // Determine whether it's float
                if (EhsMemchr(json_slice_string, '.', array[value_index].end - array[value_index].start) != NULL)
                {
                    goto for_end; // It is definitely not an integer value, continue to the next key
                    break;
                }
                // Determine whether it's boolean or something else
                switch (json_slice_string[0])
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
                        goto for_end; // It is definitely not an integer value, continue to the next key
                        break;
                    }
                    default:
                    break;
                }
                // Convert the string to integer
                integer_long = strtol(json_slice_string, &int_end, 10);
                if (integer_long > INT_MAX || integer_long< INT_MIN || 
                    (int)(int_end - json_slice_string) != (int)(array[value_index].end - array[value_index].start))
                {
                    break;
                }
                value_output = integer_long == 0 ? EHS_FALSE : EHS_TRUE;
                EHSH_LOG_INFO("The bool value found: %d\n", value_output);
                err_no = 0;
                // The first matching key found, no need to continue
                goto function_end;
            }
            default:
            {
                // Not definitely not the integer value. Continue to the next key
                break;
            }
        }
for_end:
        str_offset = array[EHS_JSMN_PARENT_LAYER_LIMIT+item_size-1].end+1;
        if (n_token_left == 0) break;
    }

    if (n_token_left < 0)
    {
        EHSH_LOG_ERROR("No key-value pair found!\n");
        err_no = -4;
        goto function_end;
    }

function_end:
    if (fp != NULL)
    {
        EhsFclose(fp);
        fp = NULL;
    }
    if (content != NULL)
    {
        EhsHMem_tempFree(content);
        content = NULL;
    }
    if (EHS_FB_OUT_CONNECTED_API2(INX_key_value_bool_ARG_query_value_out))
        EHS_FB_OUT_B_API2(INX_key_value_bool_ARG_query_value_out) = value_output;
    if (EHS_FB_OUT_CONNECTED_API2(INX_key_value_bool_ARG_query_query_errno)){
        EHS_FB_OUT_I_API2(INX_key_value_bool_ARG_query_query_errno) = err_no;
    }
    EHS_FB_FINISH(INX_key_value_bool_ARG_query_get_done);
}//ICB FUNCTION query MACRO END -- DO NOT ALTER THIS LINE
