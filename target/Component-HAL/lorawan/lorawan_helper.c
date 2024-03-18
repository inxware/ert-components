#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define _LORAWAN_HELPER_C_DEBUG_

/*
 * alloc_str function allocates memory and copy substring
 * to allocated memory.
 */

static char * alloc_str (const char * start, const char * end) {
    if (!start || !end || (start >= end)) {
        return NULL;
    }

    char * tmp = (char *)malloc (end - start + 1);
#ifdef _LORAWAN_HELPER_C_DEBUG_
    printf("alloc_str - length: [%d]\n", end - start + 1);
#endif
    if (tmp != NULL) {
        memcpy (tmp, start, end - start);
        tmp[end - start] = '\0';
    } else {
        fprintf (stderr, "Failed to allocate memory\n");
        exit (EXIT_FAILURE);
    }

    return tmp;
}

/*
 * str_split function returns the next token which is sequences of contiguous
 * characters separated by any of the characters that are part of delimiters.
 *
 * Parameters: 
 * p_str : Address of pointer to the string that you want to split.
 * sep : A set of characters that delimit the pieces in the string.
 *
 * Behaviour is undefined if sep is not a pointer to a null-terminated string. 
 *
 * Return :
 * Returns the pointer to dynamically allocated memory where the token is copied.
 * If p_str is NULL or empty string, NULL is returned.
 */

static char * str_split (char ** p_str, const char * sep) {
    char * token = NULL;

    if (*p_str && **p_str) {
        char * p_end;

        // skip separator
        *p_str += strspn(*p_str, sep);

        p_end = *p_str;

        // find separator
        p_end = strpbrk (p_end, sep);

        // strpbrk() returns null pointer if no such character
        // exists in the input string which is part of sep argument.
        if (p_end != NULL) {
            p_end = *p_str + strlen (*p_str);
        }

        token = alloc_str (*p_str, p_end);
        *p_str = p_end;
    }

    return token;
}

/*==================================================*/
/*==================================================*/

/*
 * Split the string into tokens.
 *
 * Parameters:
 * in_str : The string to be splitted
 * sep : A set of characters that delimit the pieces in the string.
 * token_count : The total number of splitted tokens
 *
 * Return:
 * Returns the array of splitted tokens in order. Note that the
 *  output array needs to be freed, and the token is not ended with
 *  NULL character.
 */
char ** token_split (char * in_str, const char * delim, int * token_count) {
    char * ptr = in_str;
    char * token = NULL;
    const int size_interval = 5;
    int max_size = 5;
    int idx = 0;
    char ** output = (char **)malloc(sizeof(char *) * (max_size + 1));
    char ** temp;
    size_t token_length;
#ifdef _LORAWAN_HELPER_C_DEBUG_
    if (in_str != NULL) printf("token_split - in_str: [%s]\n", in_str);
#endif

    while ((token = str_split(&ptr, delim)) != NULL) {
        if (idx + 1 >= max_size) {
            max_size += size_interval;
            temp = (char **)realloc(output, sizeof(char *) * (max_size + 1));
            assert(temp != NULL);
            output = (char **)temp;
        }
        output[idx] = (char *)malloc((token_length = sizeof(char) * (strlen(token) + 1)));
        memmove(output[idx++], token, token_length);
        free(token);
        token = NULL;
    }
    *token_count = idx;
#ifdef _LORAWAN_HELPER_C_DEBUG_
    int i = 0;
    for (i = 0; i < *token_count; i++) printf("token_split - output[%d]: [%s]\n", i, output[i]);
#endif
    return output;
}
