/* inx (c) 2024 
   This is inx additional use if the jmns library for general key value pair storage.

   TODO2024 - This is 100% common code (I thin) and should be in the ./Common/HAL/ directory along with the json,xmls things 
*/

#include "jsmn_utils.h"
#ifndef JSMN_HEADER
#define JSMN_HEADER
#endif
#include "jsmn.h"
#include "inx-parameters.h"
#include "inx-component.h"
#include "hal_mem.h"
#include "hal.h"
#include <limits.h>

jsmntok_t *parseKeyValuePair(ehs_char *content, ehs_sint32 *n_elements, ehs_uint8 *ret_code)
{
	ehs_uint8 ret = 0;
	if (content == NULL)
	{
		ret = 1;
		goto end;
	}
	jsmn_parser parser;
	jsmntok_t *array = NULL;
	ehs_sint32 parser_result = 0;
	// Start Parsing
	jsmn_init(&parser);
	parser_result = jsmn_parse(&parser, content, EhsStrlen(content), NULL, INT_MAX);
	if (parser_result < 0)
	{
		ret = 2;
		goto end;
	}
	if (parser_result == 0)
	{
		array = (jsmntok_t *) EhsHMem_tempAlloc(sizeof(jsmntok_t));
		goto end;
	}
	array = (jsmntok_t *) EhsHMem_tempAlloc(parser_result * sizeof(jsmntok_t));
	if (array == NULL)
	{
		ret = 3;
		goto end;
	}
	jsmn_init(&parser);
	parser_result = jsmn_parse(&parser, content, EhsStrlen(content), array, parser_result);
	if (parser_result < 0)
	{
		ret = 4;
		goto end;
	}
end:
	*n_elements = parser_result;
	*ret_code = ret;
	return array;
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
 * `json` is the output string. It needs to be pre-allocated
 * Return the error code:
 *  0 if Success
 *  1 if array input invalid
 *  2 if key/value input invalid
 *  3 if json string not pre-allocated
 */
ehs_uint8 upsertKeyValuePair(
	ehs_char *orig, jsmntok_t *array, ehs_sint32 key_index, ehs_sint32 value_index, ehs_char *key, ehs_char *value, 
	ehs_char *json
)
{
	//
	if (orig == NULL || array == NULL) return 1;
	if (key == NULL || value == NULL) return 2;
	if (json == NULL) return 3;
	ehs_bool bInsert = key_index <= 0 || value_index <= 0 ? EHS_TRUE : EHS_FALSE;
	int split_start = 0;
	int split_end = 0;

	if (bInsert)
	{
		EhsSprintf(json, "%.*s,\"%s\":\"%s\"}", array[0].end - array[0].start - 1, &orig[array[0].start], key, value);
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
		EhsSprintf(json, "%.*s\"%s\":\"%s\"%.*s", 
			split_start - array[0].start, &orig[array[0].start], 
			key, value,
			array[0].end - split_end + 1, &orig[split_end - 1]);
	}

	return 0;
}