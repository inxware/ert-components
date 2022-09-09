/** @file console_queue.c
 * In this file, the functions to operate upon the console queue type are defined 
 * This file is derived from event_queue.c
 * Please ensure changes are reflected there too.
 *
 * @sa event_queue.c
 * 
 * @author: inx limited
 * @version: $Revision: 1248 $
 * @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006), $
 * 
 * Copyright (c), inx limited, 2007. All rights reserved.
 */

#include "ehs_types.h"

#include "console_queue.h"
//#include "debug.h"
#include "hal_process.h"
#include "hal_string.h"

#include "hal_logger.h"

/**
 * Add new data to the console queue. This function can be called from separate threads.
 * It uses a mutex to guard against concurrent access of data structures.
 * 
 * @param xQueue Queue to add data to.
 * @param pData Pointer to the data to add to queue.
 * @param nSize amount of data to add to the queue.
 * @return Amount of data that was added to the queue (0 = unsuccessful)
 */
ehs_uint32 EhsConsoleQueue_push(EhsConsoleQueueType* xQueue, ehs_uint8* pData, ehs_uint32 nSize)
{
    
    ehs_uint32 nCopy = 0; /* amount of data to copy */
    ehs_uint32* pnIdx = &(xQueue->uInIdx); /* pointer to the queue input index */
	ehs_uint32 nBytesTillWrap; /* number of bytes that can be written before the wrap-around takes place */
	void* pIn = NULL;
    
	EhsTPMutex_lock(EhsTPMutex_consoleQueue);
	//if (nSize>0) xQueue->EhsConsole_buffer_empty = EHS_FALSE;
	pIn = &(xQueue->xQueue[EHS_CONSOLE_QUEUE_INDEX(*pnIdx)]); /* point to start writing data */
	/* determine how much data we can write */
	nCopy = EHS_MAX_CONSOLE_QUEUE_SIZE - EhsConsoleQueue_length(xQueue);
	if (nCopy > nSize)
	{
		nCopy = nSize;
	}
	else {
		EHSH_LOG_ERROR("QUEUE PUSH - Couldn't right all data!!! %d > %d\n",nCopy , nSize);

	}
   /* write the data */
	if (pnIdx && ((EHS_CONSOLE_QUEUE_INDEX(*pnIdx) + nCopy) > EHS_MAX_CONSOLE_QUEUE_SIZE))
	{
		/* writing data wraps around the input buffer */
        
		nBytesTillWrap = EHS_MAX_CONSOLE_QUEUE_SIZE - EHS_CONSOLE_QUEUE_INDEX(*pnIdx);
			EhsMemcpy(pIn,pData,nBytesTillWrap);
			pIn = &(xQueue->xQueue[0]);
			EhsMemcpy(pIn,pData+nBytesTillWrap,nCopy-nBytesTillWrap);
	}
	else
	{
		EhsMemcpy(pIn,pData,nCopy);
	}

	/* update the in pointer */
	/* reader can't start reading this new data until the in pointer has been updated */
	*pnIdx = (*pnIdx + nCopy) & ((EHS_MAX_CONSOLE_QUEUE_SIZE*2)-1);
	EhsTPMutex_unlock(EhsTPMutex_consoleQueue);
    return nCopy;
}

/**
 * Remove data from the console queue. This is accessed by only one
 * thread, and is therefore threadsafe. The protocol for communicating between
 * push and pop follows the Mascot queue scheme, and is safe without using mutexes.
 * @param xQueue Queue to remove data from.
 * @param pData pointer to the data that we want to retrieve
 * @param nSize amount of data to retrieve
 * @return amount of data that has been read during this pop
 */
ehs_uint32 EhsConsoleQueue_pop(EhsConsoleQueueType* xQueue, ehs_uint8* pData, ehs_uint32 nSize)
{
	ehs_uint32 nBytesTillWrap; /* number of bytes that can be read before the wrap-around takes place */
	ehs_uint32* pnIdx = &(xQueue->uOutIdx); /* pointer to the queue output index */
	ehs_uint32 nCopy; /* amount of data to copy from the queue */
	EhsTPMutex_lock(EhsTPMutex_consoleQueue);
	//printf("->Q\n");
	void* pOut = &(xQueue->xQueue[EHS_CONSOLE_QUEUE_INDEX(*pnIdx)]); /* point to start reading data */

	/* determine how much data to read from the queue */
	nCopy = EhsConsoleQueue_length(xQueue);
	if (nCopy > nSize)
	{
		nCopy = nSize;
		//printf("QPOP-Reducing the size to required %d\n",nSize);
	}

	if ((EHS_CONSOLE_QUEUE_INDEX(*pnIdx) + nCopy) > EHS_MAX_CONSOLE_QUEUE_SIZE)
	{
		/* reading data wraps around the input buffer */
		nBytesTillWrap = EHS_MAX_CONSOLE_QUEUE_SIZE - EHS_CONSOLE_QUEUE_INDEX(*pnIdx);
		EhsMemcpy(pData,pOut,nBytesTillWrap);
		pOut = &(xQueue->xQueue[0]);
		EhsMemcpy(pData+nBytesTillWrap,pOut,nCopy-nBytesTillWrap);
	}
	else
	{
		EhsMemcpy(pData,pOut,nCopy);
	}

	/* this is an atomic update to the output pointer index - if preemption
	 * occurs around this point queue->uOutIdx is either value before pop
	 * or value after pop, but not some intermediate broken value
	 */
	*pnIdx = (*pnIdx + nCopy) & ((EHS_MAX_CONSOLE_QUEUE_SIZE*2)-1);
	//printf("Q->\n");
	EhsTPMutex_unlock(EhsTPMutex_consoleQueue);

	return nCopy;
}

/**
 * Test whether the queue is empty. This occurs if the in-pointer == the
 * out-pointer. Because we can count on updates to the in-pointer and 
 * out-pointer to be atomic, this test is "preemption-safe".
 * @pre queue must point to something
 * @param queue Queue to test
 * @return true if queue is empty
 */
ehs_bool EhsConsoleQueue_isEmpty(const EhsConsoleQueueType* queue)
{
	/* We assume queue is valid for speed */
	//printf ("Q in = %d, out = %d\n",queue->uInIdx ,queue->uOutIdx);
	if (queue) return (queue->uInIdx == queue->uOutIdx); // in case this is not a debug build
	else return EHS_TRUE;
}

/**
 * Test whether the queue is full. This occurs if in-pointer ==
 * (out-pointer + EHS_MAX_CONSOLE_QUEUE_SIZE) % EHS_MAX_CONSOLE_QUEUE_SIZE*2
 * 
 * A neat optimization here is to use out-pointer XOR EHS_MAX_CONSOLE_QUEUE_SIZE
 * Examples to prove this works:
 * outpointer(o) = 18 (0x12), EHS_MAX_CONSOLE_QUEUE_SIZE(M) = 64 (0x40)
 * (o + M) % M*2 = (18 + 64) % 128 = 82
 * (o ^ M*2) = 0x12 ^ 0x40 = 0x52 = 82
 * outpointer(o) = 77 (0x4d), EHS_MAX_CONSOLE_QUEUE_SIZE(M) = 64 (0x40)
 * (o + M) % M*2 = (77 + 64) % 128 = 141%128 = 13
 * (o ^ M*2) = 0x4d ^ 0x40 = 0x0d = 13
 *
 * @pre queue must point to a valid queue.
 * @param queue Queue to test
 * @return true if queue is full
 */
ehs_bool EhsConsoleQueue_isFull(const EhsConsoleQueueType* queue)
{
	/* we assume queue points to a valid queue for speed */
	return (queue->uInIdx == (queue->uOutIdx ^ EHS_MAX_CONSOLE_QUEUE_SIZE));
}

/**
 * Determine how many elements are in the event queue.
 * Obviously in-out works where out < in.
 * Where in < out, unsigned integer arithmetic works fine
 * provided that we mask result with EHS_MAX_CONSOLE_QUEUE_SIZE*2 -1
 * Assertion: this will always result in a value 0..EHS_MAX_CONSOLE_QUEUE_SIZE
 * If in < out and no overflow has occured, the following facts obtain:
 * -# in <= EHS_MAX_CONSOLE_QUEUE_SIZE
 * -# out >= EHS_MAX_CONSOLE_QUEUE_SIZE
 * -# out-in >= EHS_MAX_CONSOLE_QUEUE_SIZE
 * @param queue Queue to test
 * @return number of elements currently in the queue
 */
ehs_uint32 EhsConsoleQueue_length(const EhsConsoleQueueType* queue)
{
	return ((queue->uInIdx - queue->uOutIdx)&((EHS_MAX_CONSOLE_QUEUE_SIZE*2)-1));
}

/* Check how much space is left */
ehs_uint32 EhsConsoleQueue_space(const EhsConsoleQueueType* queue) {
	return EHS_MAX_CONSOLE_QUEUE_SIZE - EhsConsoleQueue_length(queue);

}


/**
 * Non-destructively lookahead into the queue to see if a specified character
 * can be found there
 *
 * @param xQueue Queue to search
 * @param nChar Character to search for
 * @return how many characters need to be read to obtain the specified character
 * (0 if the character doesn't exist)
 */
ehs_uint32 EhsConsoleQueue_peek(const EhsConsoleQueueType* xQueue, ehs_uint8 nChar)
{
	ehs_uint32 nLookahead = 0;
	ehs_uint32 nRet = 0;
	EhsTPMutex_lock(EhsTPMutex_consoleQueue);
	while (nLookahead < EhsConsoleQueue_length(xQueue))
	{
		if (nChar == xQueue->xQueue[EHS_CONSOLE_QUEUE_INDEX(xQueue->uOutIdx + nLookahead)])
		{
			nRet = nLookahead + 1;
			break;
		}
		else
		{
			nLookahead++;
		}
	}
	EhsTPMutex_unlock(EhsTPMutex_consoleQueue);
	return nRet;
}

/**
 * Reset the queue - empty all items from the queue
 * @pre queue must point to something
 * @param queue Queue to reset
 */
void EhsConsoleQueue_reset(EhsConsoleQueueType* queue)
{
	/* initialise both input and output values to safe, known values */
	EhsTPMutex_lock(EhsTPMutex_consoleQueue);
	queue->uInIdx = 0;
	queue->uOutIdx = 0;
	EhsTPMutex_unlock(EhsTPMutex_consoleQueue);
}

