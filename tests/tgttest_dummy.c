/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/


/** @file tgttest_dummy.c
 * This contains dummy function and data definitions necessary to perform EHS target tests
 *
 * @author: inx limited
 */

/*****************************************************************************/
/* From dtv_components.c */
#include "blockref_table.h"

EhsBlockRefType EhsBlockRefTable_Dtv[] =
{
    {0}
};

EhsBlockRefType EhsBlockRefTable_windows[] =
{
    {0}
};

/*****************************************************************************/
/* From blockref_table.c */

EhsToolkitTableType EhsToolkitTable;

ehs_bool EhsToolkitTable_addTable(EhsBlockRefType* pTable)
{
    return EHS_TRUE;
}

/*****************************************************************************/
/* From widget.c */
#include "widget.h"

EhsWidgetTableClass EhsWidgetTable;


/*****************************************************************************/
/* From callback_queue.c */

#include "callback_queue.h"

void EhsCallbackQueue_register(EhsCallbackQueueType* pQueue,
                               EhsRunFuncType fpRunFunc,
                               EhsFunctionInstanceDataType* pFuncInst,
                               EhsCallbackQueueEntryType *pEntry)
{

}

void EhsCallbackQueue_execute(EhsCallbackQueueType* pQueue)
{
}

/*****************************************************************************/
/* From fid.c */

#include "fid.h"

/**
 * set the completed triggers of a given function instance
 *
 * @param pByte pointer to the functions main table entry
 *
 * @return int nNumComps - number of completes set
 */
void EhsFunctionInstanceData_triggerEvent(EhsFunctionInstanceDataType* pFuncInst, ehs_uint32 x)
{
}

/*****************************************************************************/
/* From globals.c */


void EhsShutdown()
{
    EhsExit(0);
}

/**
 * Defines the global state for EHS.
 */
EhsKEStateType EhsKEState;


/*****************************************************************************/
/* From console.c */

ehs_bool EhsProcessCommands(void)
{
}

void	EhsTopfieldRemotePress(ehs_sint32 key)
{
}

EhsCallbackQueueType EhsTopfieldRemoteCallback;


/*****************************************************************************/
/* From calc_i.c */
#include "calc_i.h"
EhsICalcTokenInfoType *EhsICalcTokenInfo[];
