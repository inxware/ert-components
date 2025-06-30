/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file hal_file.h
 * In this file, all of the hardware abstraction layer declarations relating to file handling are given.
 *
 * @author: inx limited
 *
 */

/**
 * @page Verification Verification report
 * @section hal_file_h
 * @anchor hal_file_h
 * @subsection misra MISRA compliance:
 * Demonstrated MISRA compliant on
 * Last modified on $Date$
 *
 * This file contained the following derogations to the MISRA standard.
 * - Rule 16.1 Functions shall not have variable number of arguments. Required for target library functions.
 */

#ifndef EHS_HAL_HASH_MAP_H
#define EHS_HAL_HASH_MAP_H

/*****************************************************************************/
/* Included files */
#include "globals.h"


// Define the hash table structure
typedef struct {
    ehs_sint32* keys;
    void**      values;
    ehs_uint32  size;
    ehs_uint32  capacity;
} EhsHashMap;

// Create a new hash table
EhsHashMap* EhsCreateHashMap(ehs_uint32 initial_capacity);

// Get size of the map
ehs_uint32 EhsHashMapSize(EhsHashMap* map);

// Add a key-value pair to the map
void EhsHashMapAppend(EhsHashMap* map, ehs_sint32 key, void* value);

// Retrieve a value by key
void* EhsHashMapGetValue(EhsHashMap* map, ehs_sint32 key);

// Retrieve a value by map index
void* EhsHashMapGetValueByIndex(EhsHashMap* map, ehs_uint32 idx);

// Free the map
void EhsHashMapFree(EhsHashMap* map);

#endif // EHS_HAL_HASH_MAP_H