#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define _LORAWAN_HELPER_C_DEBUG_

// /*
//  * alloc_str function allocates memory and copy substring
//  * to allocated memory.
//  */
// 
// static char * alloc_str (const char * start, const char * end) {
//     if (!start || !end || (start >= end)) {
//         return NULL;
//     }
// 
//     char * tmp = (char *)malloc (end - start + 1);
// #ifdef _LORAWAN_HELPER_C_DEBUG_
//     printf("alloc_str - length: [%d]\n", end - start + 1);
// #endif
//     if (tmp != NULL) {
//         memcpy (tmp, start, end - start);
//         tmp[end - start] = '\0';
//     } else {
//         fprintf (stderr, "Failed to allocate memory\n");
//         exit (EXIT_FAILURE);
//     }
// 
//     return tmp;
// }
// 
// /*
//  * str_split function returns the next token which is sequences of contiguous
//  * characters separated by any of the characters that are part of delimiters.
//  *
//  * Parameters: 
//  * p_str : Address of pointer to the string that you want to split.
//  * sep : A set of characters that delimit the pieces in the string.
//  *
//  * Behaviour is undefined if sep is not a pointer to a null-terminated string. 
//  *
//  * Return :
//  * Returns the pointer to dynamically allocated memory where the token is copied.
//  * If p_str is NULL or empty string, NULL is returned.
//  */
// 
// static char * str_split (char ** p_str, const char * sep) {
//     char * token = NULL;
// 
//     if (*p_str && **p_str) {
//         char * p_end;
// 
//         // skip separator
//         *p_str += strspn(*p_str, sep);
// 
//         p_end = *p_str;
// 
//         // find separator
//         p_end = strpbrk (p_end, sep);
// 
//         // strpbrk() returns null pointer if no such character
//         // exists in the input string which is part of sep argument.
//         if (p_end != NULL) {
//             p_end = *p_str + strlen (*p_str);
//         }
// 
//         token = alloc_str (*p_str, p_end);
//         *p_str = p_end;
//     }
// 
//     return token;
// }
// 
// /*==================================================*/
// /*==================================================*/
// 
// /*
//  * Split the string into tokens.
//  *
//  * Parameters:
//  * in_str : The string to be splitted
//  * sep : A set of characters that delimit the pieces in the string.
//  * token_count : The total number of splitted tokens
//  *
//  * Return:
//  * Returns the array of splitted tokens in order. Note that the
//  *  output array needs to be freed, and the token is not ended with
//  *  NULL character.
//  */
// char ** token_split (char * in_str, const char * delim, int * token_count) {
//     char * ptr = in_str;
//     char * token = NULL;
//     const int size_interval = 5;
//     int max_size = 5;
//     int idx = 0;
//     char ** output = (char **)malloc(sizeof(char *) * (max_size + 1));
//     char ** temp;
//     size_t token_length;
// #ifdef _LORAWAN_HELPER_C_DEBUG_
//     if (in_str != NULL) printf("token_split - in_str: [%s]\n", in_str);
// #endif
// 
//     while ((token = str_split(&ptr, delim)) != NULL) {
//         if (idx + 1 >= max_size) {
//             max_size += size_interval;
//             temp = (char **)realloc(output, sizeof(char *) * (max_size + 1));
//             assert(temp != NULL);
//             output = (char **)temp;
//         }
//         output[idx] = (char *)malloc((token_length = sizeof(char) * (strlen(token) + 1)));
//         memmove(output[idx++], token, token_length);
//         free(token);
//         token = NULL;
//     }
//     *token_count = idx;
// #ifdef _LORAWAN_HELPER_C_DEBUG_
//     int i = 0;
//     for (i = 0; i < *token_count; i++) printf("token_split - output[%d]: [%s]\n", i, output[i]);
// #endif
//     return output;
// }

/***
 * Split multi-line output into string arrays. The delimiter is '\r\n'
 *
 * Parameters:
 * in_str : The input string to be splited. 
 * in_strlen : The max string length of the in_str to be used
 * array_index_split_start : The output array of string index numbers where to start spliting the `in_str`. Length is defined by out_len.
 * array_index_split_len : The output array of string length of the splitted `in_str` excluding the splitter. Length is defined by out_len.
 * out_len : The maximum length of the array_index_split. It must be greater than 0
 * 
 * Return:
 * Error number : Natural number represents the number of splited elements. -1 if in_str or array_index_split_* is NULL or out_len is 0.
 * 
 * Note:
 * -1 in array_index_split_* means invalid. Only natural number is valid.
 */
int16_t TEhsLoraWioSplitLine(char *in_str, uint16_t in_strlen, int16_t *array_index_split_start, int16_t *array_index_split_len, uint16_t out_len)
{
    // Initialisation
    int16_t _count = 0;
    uint16_t i = 0;
    bool pre_break = false;

    // Invalid input
    if (in_str == NULL || array_index_split_start == NULL || array_index_split_len == NULL || out_len == 0) return -1;

    // Set invalid values to the whole array
    memset(array_index_split_start, -1, out_len * sizeof(int16_t));
    memset(array_index_split_len, -1, out_len * sizeof(int16_t));
    if (in_strlen < 2 || (in_strlen == 2 && strcmp(in_str, "\r\n") != 0)) return 0;

    // Set the start value for the array
    array_index_split_start[_count] = 0;
    array_index_split_len[_count] = 0;
    if (in_strlen == 2 && strcmp(in_str, "\r\n") == 0) return 1;

    // Split the string into index arrays
    for (i = 0 ; i < in_strlen ; i++) {
        if (in_str[i] == '\r')
        {
            if (pre_break) array_index_split_len[_count]++;
            pre_break = true;
        }
        else if (in_str[i] == '\n' && pre_break)
        {
            // Finish if the next next count is not supported
            if (_count + 2 > out_len) break;
            array_index_split_start[++_count] = i + 1;
            array_index_split_len[_count] = 0;
            pre_break = false;
        }
        else
        {
            if (pre_break) array_index_split_len[_count] += 2;
            else array_index_split_len[_count]++;
            pre_break = false;
        }
    }

    // The length should be plus 1 from the index
    return _count + 1;
}

/***
 * This will remove all occurance of a character from a string. It will modify the string in place
 * 
 * Parameters:
 * str : Input string
 * ch : Character to be removed
 */
void TEhsLoraRemoveAllOccurance(char *str, char ch)
{
	int i, j, len;
	
	len = strlen(str);
	
	for(i = 0; i < len; i++)
	{
		if(str[i] == ch)
		{
			for(j = i; j < len; j++)
			{
				str[j] = str[j + 1];
			}
			len--;
			i--;	
		} 
	}	
}