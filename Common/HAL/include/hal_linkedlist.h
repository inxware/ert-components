/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/


#ifndef EHS_LINKED_LIST_H
#define EHS_LINKED_LIST_H

#include <stddef.h>
#include "ehs_types.h"

typedef struct Node
{
    void* data;
    ehs_uint32 data_size;
    struct Node* next;
} EhsNode;

typedef struct
{
    EhsNode* front;
    EhsNode* back;
    ehs_uint32 count;
} EhsLinkedList;


EhsLinkedList* ehs_create_list();
void ehs_free_list(EhsLinkedList* list);
void ehs_free_node(EhsNode* node);

void ehs_push_front(EhsLinkedList* list, void* data, ehs_uint32 data_size);
// pop_back - you must call free_node on the node pointer once you done with it,
// or just call free if you don't want to free the data it points to.
EhsNode* ehs_pop_back(EhsLinkedList* list);
ehs_bool ehs_is_empty(EhsLinkedList* list);
void ehs_clear_list(EhsLinkedList* list);

#endif // EHS_LINKED_LIST_H
