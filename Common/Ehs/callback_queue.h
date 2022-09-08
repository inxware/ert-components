/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/** @file callback_queue.h
 * In this file, the callback queue type is defined and the associated functions to
 * operate upon it are declared.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_CALLBACK_QUEUE_H
#define EHS_CALLBACK_QUEUE_H

#include "ehs_fb_types.h"
#include "fid.h"

/**
 * Callback queue entry type. This is a single element of a linked list
 *
 * Unlike the event queue, indices are uint32. We assume that updates are atomic
 * (i.e. writing to them is not store low 16-bits, store high 16-bits (or vice-versa)
 * Otherwise an interrupt between one instruction and the other could cause havoc
 * if the interrupt results in another queue operation.
 */
typedef struct EhsCallbackQueueEntryStruct
{
    EhsRunFuncType fpRunFunc;				/**< Pointer to function to run when callback is called */
    EhsFunctionInstanceDataType* pFuncInst;	/**< Pointer to function context to use when callback is called */
    struct EhsCallbackQueueEntryStruct* pNext;		/**< Pointer to next callback to process when callback is called */
} EhsCallbackQueueEntryType;

/**
 * Callback queue type. This contains a pointer to the first callback queue
 * entry element.
 */
typedef EhsCallbackQueueEntryType *EhsCallbackQueueType;

/**
 * Add a new function to the callback queue.
 * @param pQueue Pointer assigned as the pEntry pointer once it is populated
 * @param fpFunc Pointer to the function to run when the callback is called
 * @param pFuncInst Pointer to the data to use when the callback is called.
 * @param pEntry Pointer to the struct to hold this instances data (presumably
 * held in the function instance data type)
 */
EHS_GLOBAL void EhsCallbackQueue_register(EhsCallbackQueueType* pQueue,
        EhsRunFuncType fpRunFunc,
        EhsFunctionInstanceDataType* pFuncInst,
        EhsCallbackQueueEntryType *pEntry);

/**
 * Execute the contents of the callback queue.
 * @param pQueue Pointer to the start of the queue
 */
EHS_GLOBAL void EhsCallbackQueue_execute(EhsCallbackQueueType* pQueue);


/* THis resets the linked list to null.
 * It does not free memory of pointers as these might be static entries
 * @param pQueue Pointer to the start of the queue
 */
EHS_GLOBAL void EhsCallbackQueue_clear(EhsCallbackQueueType* pQueue);


#endif /* EHS_CALLBACK_QUEUE_H */

