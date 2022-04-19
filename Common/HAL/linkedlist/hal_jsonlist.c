
#include <stdlib.h>
#include <string.h>
#include "hal_jsonlist.h"

void open_bracket(EhsJsonList* json_list){
    ehs_push_front(json_list->list, "[", 1);
    json_list->length += 1;
}

void close_bracket(EhsJsonList* json_list){
    ehs_push_front(json_list->list, "]", 1);
    json_list->length += 1;
}

void add_comma(EhsJsonList* json_list){
    ehs_push_front(json_list->list, ",", 1);
    json_list->length += 1;
}

EhsJsonList* ehs_create_json_list()
{
    EhsJsonList* json_list = (EhsJsonList*)malloc(sizeof(EhsJsonList)); 
    json_list->list = ehs_create_list();
    json_list->length = 0;
    return json_list;
}

void ehs_free_json_list(EhsJsonList* json_list)
{
    ehs_free_list(json_list->list);
    free(json_list);
}

void ehs_free_json_array_string(EhsJsonArrayString* json_array_string)
{
    free(json_array_string->data);
    free(json_array_string);
}

void ehs_push_json(EhsJsonList* json_list, ehs_char* json, ehs_uint32 data_size)
{
    if(!json || !data_size){
        return;
    }
    EhsLinkedList* list = json_list->list;
    if(list->count == 0){
        open_bracket(json_list);
    }else{
        add_comma(json_list);
    }
    ehs_push_front(list, json, data_size);
    json_list->length += data_size;
}

EhsJsonArrayString* ehs_pop_json_array_string(EhsJsonList* json_list)
{
    EhsJsonArrayString* json_array_string = NULL;
    EhsLinkedList* list = json_list->list;
    if(list->count > 0){
        json_array_string = (EhsJsonArrayString*)malloc(sizeof(EhsJsonArrayString));
        close_bracket(json_list);
        json_array_string->size = sizeof(ehs_char)*json_list->length+1;
        json_array_string->data = (ehs_char*)malloc(json_array_string->size);
        EhsNode* next = NULL;
        ehs_char* copy_array = json_array_string->data;
        while( (next = ehs_pop_back(list)) ){
            memcpy(copy_array, next->data, next->data_size);
            copy_array += next->data_size;
            json_list->length -= next->data_size;
            ehs_free_node(next);    
        }
        json_array_string->data[json_array_string->size-1]='\0';
    }
    return json_array_string;
}

ehs_bool ehs_is_json_array(ehs_char* json_array_string)
{
    return (json_array_string != NULL) && (strlen(json_array_string) > 1) && 
           (json_array_string[0] == '[') && (json_array_string[strlen(json_array_string)-1] == ']');
}

ehs_uint32 ehs_push_json_array_string(EhsLinkedList* list, ehs_char* json_array_string, ehs_uint32 length)
{
    ehs_sint32 json_object_count = 0;
    if(list != NULL && ehs_is_json_array(json_array_string)){
        ehs_uint32 index = 1; // move forward from [ 
        ehs_uint32 last_char = length - 1; // move backward from ]
        ehs_uint32 bracket_count = 0, json_begin = 0;
        while(index < last_char){
            ehs_char json_char = json_array_string[index];
            if(json_char == '{'){ // open
                if(bracket_count == 0){
                    json_begin = index;
                }
                bracket_count++;
            }else if(json_char == '}'){ // close 
                bracket_count--;
                if(bracket_count == 0){
                    ehs_uint32 json_length = index - json_begin + 1;
                    ehs_push_front(list, (ehs_char*) (json_array_string + json_begin), json_length);
                    json_object_count++;
                }
            }
            index++;
        }
    }
    return json_object_count;
}
