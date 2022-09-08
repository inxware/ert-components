/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file group_table.h
 * Declares the types and functions required for accessing the EHS group table type
 *
 * @author: inx limited
 *
 */

#ifndef EHS_GROUP_TABLE_H
#define EHS_GROUP_TABLE_H

#include "group.h"
#include "event_queue.h"
#include "globals.h"

/*********************************************************************************************/
/* Group table data types */

/**
 * Places all EHS Group information into a single table
 *
 * @todo split this structure into readonly and read/write elements
 *
 */
typedef struct
{
    EhsKEGroupClass* pGroup; /**< table of groups */
    EhsKEGroupClass* xCurrentGroup; /**< The currently executing group */
    ehs_uint16 nNumGroups; /**< number of currently defined groups */
} EhsKEGroupTableClass;

/**
 * Search the EhsKEGroupTable for a group with a specified ID.
 *
 * @param nGroupId Group ID to search table for
 * @return NULL if name not found, or pointer to group if it is found.
 */
EHS_GLOBAL EhsKEGroupClass* EhsKEGroupTable_findGroup(ehs_uint16 nGroupId);

/**
 * Continually schedule the most appropriate group from the current list of groups
 * until there is input from the console.
 *
 * This may return running state change requests
 */
EHS_GLOBAL Ehs_ConsoleCommand_Type EhsKEGroupTable_schedule(Ehs_ConsoleCommand_Type(*target_loop_iteration)(void *),void * target_env_blob);
//EHS_GLOBAL void EhsKEGroupTable_schedule((*target_loop_iteration)(void *),void * target_env_blob);

/**
 * Allocate memory to store our table of groups
 * @param[in] pGroupTable group table to allocate memory for
 * @param[in] nGroups maximum number of groups to allocate memory for
 * @return true if operation is successful
 */
EHS_GLOBAL ehs_bool EhsKEGroupTable_init(EhsKEGroupTableClass* pGroupTable, ehs_uint16 nGroups);

#endif /* EHS_GROUP_TABLE_H */

