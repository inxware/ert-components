#ifndef EHS_JSON_LIST_H
#define EHS_JSON_LIST_H

#include "hal_linkedlist.h"

typedef struct JsonList {
    EhsLinkedList* list;
    ehs_uint32 length;
} EhsJsonList;

typedef struct JsonArrayString {
    ehs_char* data;
    ehs_uint32 size;
} EhsJsonArrayString;

EhsJsonList* ehs_create_json_list();
void ehs_free_json_list(EhsJsonList* json_list);
void ehs_free_json_array_string(EhsJsonArrayString* json_array_string);
// push json object, which can be eventually poped as a json array string
void ehs_push_json(EhsJsonList* json_list, ehs_char* json, ehs_uint32 data_size);
// pops all elements and convert it to json array string. Note that you 
// have to free the memory of "ehs_char*" once you done with it.
EhsJsonArrayString* ehs_pop_json_array_string(EhsJsonList* json_list);

ehs_bool ehs_is_json_array(ehs_char* json_array_string);
ehs_uint32 ehs_push_json_array_string(EhsLinkedList* list, ehs_char* json_array_string, ehs_uint32 length);

#endif // EHS_JSON_LIST_H
