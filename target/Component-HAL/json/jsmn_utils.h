#ifndef _JSMN_UTILS_H_
#define _JSMN_UTILS_H_

#include "hal.h"
#include "hal_file.h"

/* The JSMN is a single header library. If we just want type defs and public prototypes we need to set JSM_HEADERto not include the implementations*/
#ifndef JSMN_PARENT_LINKS
#define JSMN_PARENT_LINKS
#endif//JSMN_PARENT_LINKS
#ifndef JSMN_HEADER
#define JSMN_HEADER
#endif
#include "jsmn.h"

typedef enum {
	EHS_JSMN_EOK = 0,
	EHS_JSMN_EINVALID_INPUT,
	EHS_JSMN_EINVALID_OUTPUT,
	EHS_JSMN_EPARSE_FAILED,
} ehs_jsmn_error_t;

// The size of JSON array buffer
#ifndef EHS_JSMN_JSON_ARRAY_BUFFER_SIZE
#define EHS_JSMN_JSON_ARRAY_BUFFER_SIZE 11
#endif//EHS_JSMN_JSON_ARRAY_BUFFER_SIZE
#if EHS_JSMN_JSON_ARRAY_BUFFER_SIZE < 3
#error EHS_JSMN_JSON_ARRAY_BUFFER_SIZE must be greater than 2!
#endif
// The limit of caching the parent object layer
#ifndef EHS_JSMN_PARENT_LAYER_LIMIT
#define EHS_JSMN_PARENT_LAYER_LIMIT 5
#endif//EHS_JSMN_PARENT_LAYER_LIMIT
#if EHS_JSMN_PARENT_LAYER_LIMIT < 3
#error EHS_JSMN_PARENT_LAYER_LIMIT must be greater than 2!
#endif

typedef struct {
	ehs_uint32 offset;
	jsmntok_t token;
} ehs_jsmn_parent_t;

/**
 * @brief 
 * 
 * @param content (Input) The json string
 * @param output_buffer (Output) The buffer to hold the JSON tokens. It must be pre-allocated with the size EHS_JSMN_JSON_ARRAY_BUFFER_SIZE + EHS_JSMN_PARENT_LAYER_LIMIT.
 * @param buffer_size (Input) The size of the output_buffer. This is always EHS_JSMN_JSON_ARRAY_BUFFER_SIZE
 * @param n_elements (Output) The number of parsed elements
 * @param ret_code (Output) Return code
 * @return Number of tokens left to be parsed. Negative value indicates ultimate error
 * 
 * @code {.C}
 * ehs_uint8 ret_code = 0;
 * ehs_sint32 n_elements = 0;
 * ehs_char content[] = "{\"Hello\": 1, \"World\": 2, \"Test\": 3, \"Test2\": 4, \"Test3\": 5, \"Test4\": 6}";
 * jsmntok_t array[EHS_JSMN_JSON_ARRAY_BUFFER_SIZE + EHS_JSMN_PARENT_LAYER_LIMIT];
 * ehs_sint32 n_tokens_left = parseKeyValuePair(content, array, EHS_JSMN_JSON_ARRAY_BUFFER_SIZE, &n_elements, &ret_code);
 * @endcode
 * 
 */
ehs_sint32 parseKeyValuePair(ehs_char *content, jsmntok_t *output_buffer, ehs_uint32 buffer_size, ehs_sint32 *n_elements, ehs_uint8 *ret_code);

/**
 * @brief Organise the broken key-value pair within the origin JSON string context
 * 
 * @param parents Array of cached parents. This will be organised within the origin JSON string pointer. The first element would must be the JSON root and the remaining elements would record the remaining. The size must be EHS_JSMN_PARENT_LAYER_LIMIT
 * @param tokens Array of JSMN tokens. This will be organised within the origin JSON string pointer. This must point to the beginning of the token array. The size must be EHS_JSMN_JSON_ARRAY_BUFFER_SIZE+EHS_JSMN_PARENT_LAYER_LIMIT. The first EHS_JSMN_PARENT_LAYER_LIMIT elements must be unpopulated.
 * @param tokens_size Size of parsed tokens in the array `tokens`. It has to be smaller than EHS_JSMN_JSON_ARRAY_BUFFER_SIZE.
 * @param str_offset The offset from the origin JSON string
 * @param parent_added (OUTPUT) Whether the non-root parent token is added
 * @param n_added_tokens (OUTPUT) The number of added tokens (JSON root and potential missing key parent)
 * @return ehs_jsmn_error_t 
 *
 * @code {.C}
 * ehs_uint8 ret_code = 0;
 * ehs_sint32 n_elements = 0;
 * ehs_char content[] = "{\"Hello\": 1, \"World\": 2, \"Test\": 3, \"Test2\": 4, \"Test3\": 5, \"Test4\": 6}";
 * jsmntok_t array[EHS_JSMN_JSON_ARRAY_BUFFER_SIZE + EHS_JSMN_PARENT_LAYER_LIMIT];
 * ehs_jsmn_parent_t parents[EHS_JSMN_PARENT_LAYER_LIMIT] = {0};
 * ehs_uint32 n_added_tokens = 0;
 * ehs_bool parent_added = EHS_FALSE;
 * ehs_sint32 n_tokens_left = parseKeyValuePair(content, array, EHS_JSMN_JSON_ARRAY_BUFFER_SIZE, &n_elements, &ret_code);
 * if (n_tokens_left < 0) {
 *     // Handle error
 * }
 * ehs_jsmn_error_t err = organiseKeyValuePair(parents, array, n_elements, 0, &parent_added, &n_added_tokens);
 * if (err != EHS_JSMN_EOK) {
 *     // Handle error
 * }
 * // The `parents` array will now contain the organised parent tokens, and `array` will have the
 * //  key-value pairs organised according to the original JSON string.
 *
 * if (n_tokens_left > 0) {
 *     // There are still tokens left to be parsed, you can continue parsing them by calling `parseKeyValuePair` again with the remaining string.
 *     ehs_sint32 str_offset = array[EHS_JSMN_JSON_ARRAY_BUFFER_SIZE + EHS_JSMN_PARENT_LAYER_LIMIT].end + 1; // Skip the comma or colon character
 *     // Continue parsing the remaining tokens
 *     n_tokens_left = parseKeyValuePair(&content[str_offset], array, EHS_JSMN_JSON_ARRAY_BUFFER_SIZE, &n_elements, &ret_code);
 *     if (n_tokens_left < 0) {
 *         // Handle error
 *     }
 *     err = organiseKeyValuePair(parents, array, n_elements, str_offset,
 *                                &parent_added, &n_added_tokens);
 *     if (err != EHS_JSMN_EOK) {
 *         // Handle error
 *     }
 * }
 * // Continue parsing the remaining tokens if n_tokens_left > 0 ...
 * @endcode
 * 
 */
ehs_jsmn_error_t organiseKeyValuePair(
	ehs_jsmn_parent_t *parents, jsmntok_t *tokens, ehs_uint32 tokens_size, ehs_uint32 str_offset, 
	ehs_bool *parent_added, ehs_uint8 *n_added_tokens
);

/**
 * @brief Fill the {key index, value index} from the array input in the `result` buffer
 * 
 * @param array (Input) The JSMN array
 * @param size (Input) The size of the JSMN array input (i.e. `n_elements` in `parseKeyValuePair`)
 * @param json (Input) The JSON string
 * @param key (Input) The JSON key to find
 * @param key_index (Output) The index of key in the JSMN array 
 * @param value_index (Output) The index of found value in the JSMN array
 * @param value (Output) The found value. If NULL, then it's not written and no memory is allocated
 * @return ehs_uint8 Return code
 * Return 1, 2 and 3 means in/out pointer NULL error
 * Return 4 means empty JSON
 * Return 5 means key not found
 *
 * @code {.C}
 * ehs_char content[] = "{\"Hello\": 1, \"World\": 2, \"Test\": 3, \"Test2\": 4, \"Test3\": 5, \"Test4\": 6}";
 * jsmntok_t array[EHS_JSMN_JSON_ARRAY_BUFFER_SIZE + EHS_JSMN_PARENT_LAYER_LIMIT];
 * ehs_sint32 n_added_tokens = 0;
 * ehs_sint32 n_elements = 0;
 * // ... after parsing the JSON string into JSMN tokens
 * ehs_sint32 key_index = 0;
 * ehs_sint32 value_index = 0;
 * ehs_uint8 ret_code = findKeyValue(
 * 							array + EHS_JSMN_PARENT_LAYER_LIMIT - n_added_tokens, 
 * 							n_elements + n_added_tokens, 
 * 							content, "Test4", 
 * 							&key_index, &value_index, NULL);
 * if (ret_code == 0) {
 *     // Key found, key_index and value_index are set
 *     // value points to the value string in the JSON
 *     printf("Key found at index %d, value index %d: %.*s\n", key_index, value_index, array[value_index].end-array[value_index].start, &content[array[value_index].start]);
 * } else if (ret_code == 5) {
 *     // Key not found
 *     printf("Key not found\n");
 * } else {
 *     // Handle other errors
 *     printf("Error code: %d\n", ret_code);
 * }
 * @endcode
 * 
 */
ehs_uint8 findKeyValue(
	jsmntok_t *array, ehs_sint32 size, ehs_char *json, const ehs_char *key, 
	ehs_sint32 *key_index, ehs_sint32 *value_index, ehs_char **value
);

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
);

#endif//_JSMN_UTILS_H_