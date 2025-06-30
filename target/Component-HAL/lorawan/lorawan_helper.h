#ifndef _LORAWAN_HELPER_H_
#define _LORAWAN_HELPER_H_

#include <stdint.h>

// char ** token_split (char * in_str, const char * delim, int * token_count);
int16_t TEhsLoraWioSplitLine(char *in_str, uint16_t in_strlen, int16_t *array_index_split_start, int16_t *array_index_split_len, uint16_t out_len);
void TEhsLoraRemoveAllOccurance(char *str, char ch);

#endif
