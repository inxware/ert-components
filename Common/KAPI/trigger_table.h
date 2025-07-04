/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file group_table.c
 * Contains the declarations of functions and types required for accessing the EHS trigger table
 *
 * @author: inx limited
 *
 */


#ifndef EHS_TRIGGER_TABLE_H
#define EHS_TRIGGER_TABLE_H
#include "target_types.h"
#include "ehs_fb_types.h"
/**
 * A trigger table is an array of EhsTriggerType with a size parameter
 */
typedef struct
{
    ehs_uint32 nSize; /**< Number of entries currently defined */
    EhsTriggerType* pTrigger; /**< Table containing entries */
} EhsTriggerTableType;

/**
 * Initialise the trigger table
 * @param[in] nTableSize Number of entries to allocate to table.
 * @return true if allocation was successful
 */
EHS_GLOBAL ehs_bool EhsTriggerTable_init(ehs_uint32 nTableSize);

/**
 * Search the EhsTriggerTable for a group with a specified trigger ID.
 *
 * @param nTrigId trigger ID to search table for
 * @return NULL if trigger not found, or pointer to trigger if it is found.
 */
EHS_GLOBAL EhsTriggerType* EhsTriggerTable_findTriggerId(EhsTriggerIdType nTrigId);

/**
 * Add a new entry to the trigger table
 * @precondition Assumes that trigger ID isn't in the table already.
 *
 * @param nTrigId trigger ID to add to the table
 * @return pointer to created trigger if successful, NULL otherwise
 */
EHS_GLOBAL EhsTriggerType* EhsTriggerTable_addTrigger(EhsTriggerIdType nTrigId);

/**
 * Fire a single trigger
 *
 * @param xTrigger The trigger to fire
 */
EHS_GLOBAL EHS_MEMORY_ATTRIB void EhsTrigger_fire(EhsTriggerType* xTrigger);

/**
 * Change the monitor state for all functions driven by this trigger
 *
 * @param xTrigger The trigger that fires the functions
 * @param monitorType type of debug monitoring for this function.
 */
EHS_GLOBAL void EhsTrigger_monitorTriggeredFunctions(EhsTriggerType* xTrigger, debug_type_byte monitorType);

/**
 * clears all monitors in trigger table
 * clears trigger and function monitors
 */
EHS_GLOBAL void EhsTriggerTable_clearAllMonitors(void);

/**
 * clears specific events in trigger table, based on what's specified in
 * void* pConditionData
 */
EHS_GLOBAL void EhsTriggerTable_clearEvents(void* pConditionData);


#endif /* EHS_TRIGGER_TABLE_H */

