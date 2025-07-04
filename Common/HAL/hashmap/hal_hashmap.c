/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

#include <stdlib.h>
#include "hal-api.h"
#include "hal_hashmap.h"
#include "hal_logger.h"

// Grow the map by allocating a new larger block of memory
void EhsHashMapGrow(EhsHashMap* map) {
    ehs_uint32 new_capacity = map->capacity + 2;
    
    // Allocate new memory blocks
    ehs_sint32* new_keys = malloc(new_capacity * sizeof(ehs_sint32));
    void** new_values = malloc(new_capacity * sizeof(void*));
    
    // Copy existing data
    memcpy(new_keys, map->keys, map->size * sizeof(ehs_sint32));
    memcpy(new_values, map->values, map->size * sizeof(void*));
    
    // Free old memory
    free(map->keys);
    free(map->values);
    
    // Update map
    map->keys = new_keys;
    map->values = new_values;
    map->capacity = new_capacity;
}

// Create a new hash table
EhsHashMap* EhsCreateHashMap(ehs_uint32 initial_capacity) {

    EhsHashMap* map = (EhsHashMap*)malloc(sizeof(EhsHashMap));
    if (!map) {
        EHSH_LOG_ERROR("Memory allocation failed for hash map");
        return NULL;
    }

    map->keys = malloc(initial_capacity * sizeof(ehs_sint32));
    map->values = malloc(initial_capacity * sizeof(void*));
    map->size = 0;
    map->capacity = initial_capacity;

    return map;
}

ehs_uint32 EhsHashMapSize(EhsHashMap* map)
{
    return (map != NULL) ? map->size : 0;
}

// Add a key-value pair to the map
void EhsHashMapAppend(EhsHashMap* map, ehs_sint32 key, void* value) {
    // If map is full, grow it
    if (map->size >= map->capacity) {
        EhsHashMapGrow(map);
    }
    
    // Add new key-value pair
    map->keys[map->size] = key;
    map->values[map->size] = value;
    map->size++;
}

// Retrieve a value by key
void* EhsHashMapGetValue(EhsHashMap* map, ehs_sint32 key) {
    if(map == NULL){
        return NULL;
    }
    for (ehs_uint32 i = 0; i < map->size; i++) {
        if (map->keys[i] == key) {
            return map->values[i];
        }
    }
    return NULL;
}

// Retrieve a value by map index
void* EhsHashMapGetValueByIndex(EhsHashMap* map, ehs_uint32 idx)
{
    return (map != NULL && idx < map->size) ? map->values[idx] : NULL;
}

// Free the map
void EhsHashMapFree(EhsHashMap* map) {
    if(map != NULL){
        free(map->keys);
        free(map->values);
        free(map);
    }
}