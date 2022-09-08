/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file fidt.h
 * Declarations of EhsFunctionInstanceDataTableType
 *
 * @author: inx limited
 *
 */

#ifndef EHS_FIDT_H
#define EHS_FIDT_H

#include "target_config.h"
#include "ehs_types.h"
#include "globals.h"
#include "fid.h"

typedef struct
{
    EhsFunctionInstanceDataType* pFunctions; /**< Contains the actual function instances */
    ehs_uint32 nSize; /**< Defines the size of the function instance data table for the specific application - populated at SODL parse time*/
    ehs_uint32 nMemorySize; //Size of memory allocated at init time of this structure.
} EhsFunctionInstanceDataTableType;

/**
 * Contains the function instances used in the current application
 */
EHS_GLOBAL EhsFunctionInstanceDataTableType EhsFunctionInstanceDataTable;

/**
 * Contains the set of function instances for callbacks for the current application
 */
EHS_GLOBAL EhsFunctionInstanceDataTableType EhsCallbackInstanceDataTable;

/**
 * Sets the specified function instance table to empty.
 * @param pFIDT data table to reset.
 */
EHS_GLOBAL void EhsFunctionInstanceDataTable_reset(EhsFunctionInstanceDataTableType* pFIDT);

/**
 * Allocate memory for the function instance table
 * @param[in] pFIDT data table to initialize
 * @param[in] nSize number of entries to allocate to the table
 * @return true if allocations was successful
 */
EHS_GLOBAL ehs_bool EhsFunctionInstanceDataTable_init(EhsFunctionInstanceDataTableType* pFIDT, ehs_uint32 nSize);

/**
 * Fire the initial event for an application
 */
EHS_GLOBAL void EhsFunctionInstanceDataTable_triggerInitialEvent();

/**
 * Reset the debug monitor flags for the application. This should be called whenever entering debug mode
 */
EHS_GLOBAL void EhsFunctionInstanceDataTable_resetMonitorFlags(void);

/**
 * Set the monitor flag for a specific data line of a specific data type
 *
 * @param nLine Line number to set
 * @param eType data type of the line
 * @param bSet set or not
 * @return true if nLine,eType is a valid combination
 */
EHS_GLOBAL ehs_bool EhsFunctionInstanceDataTable_monitorDataLine(ehs_uint32 nLine, EhsDataTypeEnum eType, ehs_bool bSet);

/**
 * Set the monitor flag for a specific event trigger
 *
 * @param nLine Line number to set
 * @param monitorType type of debug monitoring for this function
 * @param bClearBreakpoint clear the breakpoint
 * @return true if nLine is a valid event number
 */
EHS_GLOBAL ehs_bool EhsFunctionInstanceDataTable_monitorEventLine(ehs_uint32 nLine, debug_type_byte monitorType, ehs_bool bClearBreakpoint);

#endif /* EHS_FIDT_H */
