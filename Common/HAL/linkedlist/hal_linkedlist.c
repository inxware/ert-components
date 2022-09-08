/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

#include <stdlib.h>
#include "hal_linkedlist.h"

EhsLinkedList* ehs_create_list()
{
    EhsLinkedList* list = (EhsLinkedList*)malloc(sizeof(EhsLinkedList));
    list->front = NULL;
    list->back = NULL;
    list->count = 0;
    return list;
}

void ehs_free_list(EhsLinkedList* list)
{
    ehs_clear_list(list);
    free(list);
}

void ehs_free_node(EhsNode* node)
{
    if(node)
    {
        free(node->data);
        free(node);
    }
}

void ehs_push_front(EhsLinkedList* list, void* data, ehs_uint32 data_size)
{
    // create a new node
    EhsNode* node = (EhsNode*)malloc(sizeof(EhsNode));
    node->data = malloc(data_size);
    node->data_size = data_size;
    node->next = NULL;
    // copy data
    for (ehs_uint32 i = 0; i < data_size; i++)
    {
        *(ehs_char *)(node->data + i) = *(ehs_char *)(data + i);
    }
    // update previous element
    if(!list->front)
    {
        list->back = node; // first element so back=front
    }
    else
    {
        list->front->next = node;
        if(!list->back->next)
        {
            list->back->next = node;
        }
    }
    // update latest data
    list->front = node;
    list->count++;
}

EhsNode* ehs_pop_back(EhsLinkedList* list)
{
    EhsNode* node = list->back;
    if(node)
    {
        list->back = node->next;
        node->next = NULL;
        if(!list->back)
        {
            list->front = NULL;
        }
        list->count--;
    }
    return node;
}

void ehs_clear_list(EhsLinkedList* list)
{
    EhsNode* next = NULL;
    while( (next = ehs_pop_back(list)) )
    {
        ehs_free_node(next);
    }
}

ehs_bool ehs_is_empty(EhsLinkedList* list)
{
    return list->count == 0;
}
