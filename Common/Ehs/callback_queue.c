/** @file callback_queue.c
 * In this file, the functions to operate upon the console queue type are defined 
 * This file is derived from event_queue.c
 * Please ensure changes are reflected there too.
 *
 * @sa event_queue.c
 * 
 * @author: inx limited
 * @version: $Revision: 2884 $
 * @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006), $
 * 
 * Copyright (c), inx limited, 2007. All rights reserved.
 */

/* @todo - mark this for deletion - we don't need this really this is so that a list of FB functions can be
 * called sequentially from a linked list. This is not a useful utility and shouldn't be
 * needed as only one CB is usually associated with an internal or io-input event.
 */


#include "callback_queue.h"
#include "globals.h"

/* THis resets the linked list to null.
 * It does not free memory of pointers as these might be static entries
 *
 */
void EhsCallbackQueue_clear(EhsCallbackQueueType* pQueue)
{
	*pQueue=NULL;
}

/**
 * Add a new function to the callback queue.
 * @param pQueue Pointer to a start of the queue
 * @param fpFunc Pointer to the function to run when the callback is called
 * @param pFuncInst Pointer to the data to use when the callback is called.
 * @param pEntry Pointer to the struct to hold this instances data (presumably
 * held in the function instance data type)
 */

void EhsCallbackQueue_register(EhsCallbackQueueType* pQueue, 
										  EhsRunFuncType fpRunFunc, 
										  EhsFunctionInstanceDataType* pFuncInst,
										  EhsCallbackQueueEntryType *pEntry)
{
	pEntry->fpRunFunc = fpRunFunc;
	pEntry->pFuncInst = pFuncInst;
	pEntry->pNext = *pQueue;

	*pQueue = pEntry;
}

/**
 * Execute the contents of the callback queue.
 * @param pQueue Pointer to the start of the queue
 */
void EhsCallbackQueue_execute(EhsCallbackQueueType* pQueue)
{
	EhsCallbackQueueEntryType* pEntry;

	if (pQueue && (EhsKEState == EHSKE_STATE_RUNNING))
	{

		pEntry = *pQueue;
		while (pEntry)
		{
			pEntry->fpRunFunc(pEntry->pFuncInst);
			pEntry = pEntry->pNext;
		}
	}
}



