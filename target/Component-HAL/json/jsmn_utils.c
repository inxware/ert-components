/* inx (c) 2024 
   This is inx additional use if the jmns library for general key value pair storage.

   TODO2025 - This is 100% common code (I think) and should be in the ./Common/HAL/ directory along with the json,xmls things 
*/

#include "jsmn_utils.h"
#ifndef JSMN_PARENT_LINKS
#define JSMN_PARENT_LINKS
#endif//JSMN_PARENT_LINKS
#ifndef JSMN_HEADER
#define JSMN_HEADER
#endif
#include "jsmn.h"
#include "inx-parameters.h"
#include "inx-component.h"
#include "hal_mem.h"
#include "hal.h"
#include <limits.h>

//jsmntok_t *parseKeyValuePair(ehs_char *content, ehs_sint32 *n_elements, ehs_uint8 *ret_code)
ehs_sint32 parseKeyValuePair(ehs_char *content, jsmntok_t *output_buffer, ehs_uint32 buffer_size, ehs_sint32 *n_elements, ehs_uint8 *ret_code)
{
	ehs_uint8 ret = 0;
	if (content == NULL)
	{
		ret = 1;
		goto end;
	}
	if (output_buffer == NULL || buffer_size == 0)
	{
		ret = 3;
		goto end;
	}
	jsmn_parser parser;
	ehs_sint32 parser_result = 0;
	// First pass to get the total number of tokens
	jsmn_init(&parser);
	parser_result = jsmn_parse(&parser, content, EhsStrlen(content), NULL, INT_MAX);
	if (parser_result < 0)
	{
		ret = 2;
		goto end;
	}
	if (parser_result == 0)
	{
		goto end;
	}
	// Start Parsing
	jsmn_init(&parser);
	jsmn_parse(&parser, content, EhsStrlen(content), &output_buffer[EHS_JSMN_PARENT_LAYER_LIMIT], buffer_size);
end:
	if (n_elements != NULL) *n_elements = parser_result <= buffer_size ? parser_result : buffer_size;
	if (ret_code != NULL) *ret_code = ret;
	if (parser_result <= buffer_size) return parser_result < 0 ? parser_result : 0;
	else return parser_result - buffer_size;
}

/**
 * @brief Organise the broken key-value pair within the origin JSON string context
 * 
 * @param parents Array of cached parents. This will be organised within the origin JSON string pointer. The first element would must be the JSON root and the remaining elements would record the remaining. The size must be EHS_JSMN_PARENT_LAYER_LIMIT
 * @param tokens Array of JSMN tokens. This will be organised within the origin JSON string pointer. The size must be EHS_JSMN_JSON_ARRAY_BUFFER_SIZE+EHS_JSMN_PARENT_LAYER_LIMIT. The first EHS_JSMN_PARENT_LAYER_LIMIT elements must be unpopulated.
 * @param tokens_size Size of parsed tokens in the array `tokens`. It has to be smaller than EHS_JSMN_JSON_ARRAY_BUFFER_SIZE.
 * @param str_offset The offset from the origin JSON string
 * @param parent_added (OUTPUT) Whether the non-root parent token is added
 * @param n_added_tokens (OUTPUT) The number of added tokens (JSON root and potential missing key parent)
 * @return ehs_jsmn_error_t 
 */
ehs_jsmn_error_t organiseKeyValuePair(
	ehs_jsmn_parent_t *parents, jsmntok_t *tokens, ehs_uint32 tokens_size, ehs_uint32 str_offset, 
	ehs_bool *parent_added, ehs_uint8 *n_added_tokens
)
{
	if (parents == NULL || tokens == NULL || parent_added == NULL || n_added_tokens == NULL) return EHS_JSMN_EINVALID_INPUT;
	*parent_added = EHS_FALSE;
	*n_added_tokens = 0;
	if ((tokens[EHS_JSMN_PARENT_LAYER_LIMIT].parent == -1 && tokens[EHS_JSMN_PARENT_LAYER_LIMIT+1].parent == -1) ||
		(tokens_size == 1 && str_offset != 0))
	{
		tokens[EHS_JSMN_PARENT_LAYER_LIMIT-1] = parents[1].token;
		tokens[EHS_JSMN_PARENT_LAYER_LIMIT-1].start += parents[1].offset;
		tokens[EHS_JSMN_PARENT_LAYER_LIMIT-1].end += parents[1].offset;
		tokens[EHS_JSMN_PARENT_LAYER_LIMIT-1].parent = 0;
		*parent_added = EHS_TRUE;
		*n_added_tokens += 1;
	}
	if (str_offset != 0)
	{
		tokens[EHS_JSMN_PARENT_LAYER_LIMIT - (*parent_added ? 2 : 1)] = parents[0].token;
		*n_added_tokens += 1;
	}
	if (str_offset != 0) tokens[EHS_JSMN_PARENT_LAYER_LIMIT].parent = *parent_added ? 1 : 0;
	tokens[EHS_JSMN_PARENT_LAYER_LIMIT].start += str_offset;
	tokens[EHS_JSMN_PARENT_LAYER_LIMIT].end += str_offset;
	ehs_uint16 token_index = EHS_JSMN_PARENT_LAYER_LIMIT + 1;
	for (; str_offset != 0 && token_index < EHS_JSMN_PARENT_LAYER_LIMIT + tokens_size ; token_index++)
	{
		if (tokens[token_index].parent != -1) tokens[token_index].parent += *n_added_tokens;
		if (tokens[token_index].parent == -1) tokens[token_index].parent = 0;
		tokens[token_index].start += str_offset;
		tokens[token_index].end += str_offset;
	}

	// Cache tokens to parent array
	if (tokens[EHS_JSMN_PARENT_LAYER_LIMIT + tokens_size - 1].parent == 0)
	{
		parents[1].offset = 0;
		parents[1].token = tokens[EHS_JSMN_PARENT_LAYER_LIMIT + tokens_size - 1];
	}
	// JSON root parent is always at position 0
	if (str_offset == 0) {
		parents[0].token = tokens[EHS_JSMN_PARENT_LAYER_LIMIT];
		parents[0].offset = 0;
	}

	return EHS_JSMN_EOK;
}

/*
 * Fill the {key index, value index} from the array input in the `result` buffer
 * Return 1, 2 and 3 means in/out pointer NULL error
 * Return 4 means empty JSON
 * Return 5 means key not found
 */
ehs_uint8 findKeyValue(
	jsmntok_t *array, ehs_sint32 size, ehs_char *json, const ehs_char *key, 
	ehs_sint32 *key_index, ehs_sint32 *value_index, ehs_char **value
)
{
	if (json == NULL) return 1;
	if (key == NULL) return 2;
	if (key_index == NULL || value_index == NULL) return 3;
	if (size == 0 || size == 1) return 4;
	ehs_sint32 index = 0;
	ehs_sint32 i_key = 0;
	ehs_sint32 i_value = 0;
	ehs_char *token = NULL;
	ehs_bool found = EHS_FALSE;
	for (index = 1 ; index < size ; index++)
	{
		//
		if (value != NULL)
		{
			if (token != NULL)
			{
				EhsHMem_tempFree(token);
				token = NULL;
			}
			token = (ehs_char *) EhsHMem_tempAlloc(array[index].end - array[index].start + 2);
			EhsSprintf(token, "%.*s", array[index].end - array[index].start, &json[array[index].start]);
		}
		if (array[index].parent == 0)
		{
			if (EhsStrncmp(&json[array[index].start], key, EhsStrlen(key)) == 0)
			{
				i_key = index;
			}
		}
		if (i_key > 0 && array[index].parent == i_key)
		{
			i_value = index;
			found = EHS_TRUE;
			break;
		}
	}
	if (found != EHS_TRUE)
	{
		if (token != NULL)
		{
			EhsHMem_tempFree(token);
			token = NULL;
		}
		return 5;
	}
	*key_index = i_key;
	*value_index = i_value;
	if (value != NULL) *value = token;

	return 0;
}

/*
 * If key_index <= 0 or value_index <= 0, insert the key-value pair
 * Or it will update the value according to the index input
 * The input key and value are the actual string
 * The following two parameters are outputs. They cannot be NULL at the same time but one of them can be NULL to disable the output
 * `json_file` is the file pointer to write to
 * `json` is the output string. It needs to be pre-allocated
 * Return the error code:
 *  0 if Success
 *  1 if array input invalid
 *  2 if key/value input invalid
 *  3 if json string not pre-allocated
 */
ehs_uint8 upsertKeyValuePair(
	ehs_char *orig, jsmntok_t *array, ehs_sint32 key_index, ehs_sint32 value_index, ehs_char *key, ehs_char *value, 
	ehs_char *json, ehs_FILE *json_file
)
{
	//
	if (orig == NULL || array == NULL) return 1;
	if (key == NULL || value == NULL) return 2;
	if (json == NULL && json_file == NULL) return 3;
	ehs_bool bInsert = key_index <= 0 || value_index <= 0 ? EHS_TRUE : EHS_FALSE;
	int split_start = 0;
	int split_end = 0;

	if (bInsert)
	{
		if (json != NULL) EhsSprintf(json, "%.*s,\"%s\":\"%s\"}", array[0].end - array[0].start - 1, &orig[array[0].start], key, value);
		if (json_file != NULL) EhsFprintf(json_file, "%.*s,\"%s\":\"%s\"}", array[0].end - array[0].start - 1, &orig[array[0].start], key, value);
	}
	else
	{
		switch (array[key_index].type)
		{
			case JSMN_STRING:
				split_start = array[key_index].start - 1;
				break;
			default:
				split_start = array[key_index].start;
				break;
		}
		switch (array[value_index].type)
		{
			case JSMN_STRING:
				split_end = array[value_index].end + 2;
				break;
			default:
				split_end = array[value_index].end + 1;
				break;
		}
		if (json_file != NULL) EhsFprintf(json_file,
			"%.*s\"%s\":\"%s\"%.*s", 
			split_start - array[0].start, &orig[array[0].start], 
			key, value,
			array[0].end - split_end + 1, &orig[split_end - 1]);
		if (json != NULL) EhsSprintf(json,
			"%.*s\"%s\":\"%s\"%.*s", 
			split_start - array[0].start, &orig[array[0].start], 
			key, value,
			array[0].end - split_end + 1, &orig[split_end - 1]);
	}

	return 0;
}