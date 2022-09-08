/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file group_table.h
 * Declares the types and functions required for accessing the EHS group type
 *
 * @author: inx limited
 *
 */

#ifndef EHS_GROUP_H
#define EHS_GROUP_H

#include "event_queue.h"
#include "globals.h"
#include "hal_time.h"

/*********************************************************************************************/
/* Group data types */

/**
 * Contains all of the information required by a single group.
 */
typedef struct
{
    EhsTickType nPeriod; /**< Defines group period in target ticks. See EhsusToTick. */
    EhsTickType nAllocation; /**< Defines group allocation in target ticks. */
    EhsTickType nRemainingAllocation; /**< How much of the current allocation remains */
    EhsTickType nNextScheduled; /**< When do we next get the allocation renewed */
    EhsTickType nFunctionWCET; /**< What is the WCET for functions in this group? */
    EhsEventQueueType xEventQueue; /**< The event queue for this group */
    ehs_bool bBreakpointSet; /**< Is a breakpoint set for any functions within this group */
    ehs_uint16 nId; /**< group Id */
} EhsKEGroupClass;

#define EHS_INVALID_GROUP_ID 0xffff /**< No group should have this Id */

/**
 * This function is used to initialise the members of the group
 */
EHS_GLOBAL void EhsKEGroup_init(EhsKEGroupClass* group);

/**
 * This function is executed just before the group starts running
 *
 * @param pGroup group to start up.
 */
EHS_GLOBAL void EhsKEGroup_start(EhsKEGroupClass *pGroup);

/**
 * This function is executed at the start of every period. It refreshes the CPU
 * allocation for the group, and recalculates the next scheduled time.
 *
 * @param group Group to refresh.
 */
EHS_GLOBAL void EhsKEGroup_restart(EhsKEGroupClass* group);

/**
 * This function returns the latest time that this group can start and still
 * use up all of the remaining allocation before the allocation gets refreshed
 *
 * @param group Group to evaluate
 * @return latest (absolute) time when this group must start executing
 */
EHS_GLOBAL EhsTickType EhsKEGroup_latestStartTime(EhsKEGroupClass* group);

/**
 * Check whether the group is ready for execution. Are there any events
 * in its queue?
 *
 * @param group pGroup to test
 * @return true if group has events in its queue
 */
EHS_GLOBAL ehs_bool EhsKEGroup_ready(EhsKEGroupClass* pGroup);

/**
 * Execute the functions within this group for a specified period of time
 *
 * @param group Group to execute
 * @param deadline Time to finish execution by
 */
EHS_GLOBAL ehs_bool EhsKEGroup_execute(EhsKEGroupClass* group, EhsTickType deadline);

#endif /* EHS_GROUP_H */

