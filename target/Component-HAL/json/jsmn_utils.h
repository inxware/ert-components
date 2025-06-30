#ifndef _JSMN_UTILS_H_
#define _JSMN_UTILS_H_

#include "hal.h"

/* The JSMN is a single header library. If we just want type defs and public prototypes we need to set JSM_HEADERto not include the implementations*/
#ifndef JSMN_HEADER
#define JSMN_HEADER
#endif
#include "jsmn.h"

// don't need this it's in the orginal struct typedef struct jsmntok jsmntok_t;

/**
 * @brief 
 * 
 * @param content (Input) The json string
 * @param n_elements (Output) The number of elements
 * @param ret_code (Output) Return code
 * @return jsmntok_t* JSMN array
 * 
 * @code {.C}
 * ehs_uint8 ret_code = 0;
 * ehs_sint32 n_elements = 0;
 * ehs_char content[] = "{"Hello": 1}";
 * jsmntok_t *array = parseKeyValuePair(content, &n_elements, &ret_code);
 * @endcode
 * 
 */
jsmntok_t *parseKeyValuePair(ehs_char *content, ehs_sint32 *n_elements, ehs_uint8 *ret_code);

/**
 * @brief Fill the {key index, value index} from the array input in the `result` buffer
 * 
 * @param array (Input) The JSMN array
 * @param size (Input) The size of the JSMN array input (i.e. `n_elements` in `parseKeyValuePair`)
 * @param json (Input) The JSON string
 * @param key (Input) The JSON key to find
 * @param key_index (Output) The index of key in the JSMN array 
 * @param value_index (Output) The index of found value in the JSMN array
 * @param value (Output) The found value
 * @return ehs_uint8 Return code
 * Return 1, 2 and 3 means in/out pointer NULL error
 * Return 4 means empty JSON
 * Return 5 means key not found
 */
ehs_uint8 findKeyValue(
	jsmntok_t *array, ehs_sint32 size, ehs_char *json, const ehs_char *key, 
	ehs_sint32 *key_index, ehs_sint32 *value_index, ehs_char **value
);

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
);

#endif//_JSMN_UTILS_H_