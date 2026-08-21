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

#include "globals.h"

#include "console_queue.h"
//#include "debug.h"
#include "hal_process.h"
#include "hal_string.h"

#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_CONSOLE
#include "hal_logger.h"

#ifdef EHS_CONSOLE_QUEUE_STATS
#ifndef EHS_RUNTIME_LOGGER_ENABLED
#error "EHS_CONSOLE_QUEUE_STATS requires EHS_RUNTIME_LOGGER_ENABLED - without it, EHSH_LOG_INFO compiles to nothing (see hal_logger.h) and every CQ push/pushRecord line below is silently dropped at compile time, not just filtered at runtime. Fix the target's config.mk instead of relying on this file compiling quietly."
#endif
#endif

/**
 * Add new data to the console queue. This function can be called from separate threads.
 * It uses a mutex to guard against concurrent access of data structures.
 * 
 * @param xQueue Queue to add data to.
 * @param pData Pointer to the data to add to queue.
 * @param nSize amount of data to add to the queue.
 * @return Amount of data that was added to the queue (0 = unsuccessful)
 */
ehs_sint32 EhsConsoleQueue_push(EhsConsoleQueueType* xQueue, ehs_uint8* pData, ehs_uint32 nSize)
{
	if(xQueue==NULL || xQueue->xQueue==NULL){
		return 0;
	}
    const ehs_uint32 consoleQueueMaxSize = EhsConsoleQueue_maxSize();
    ehs_sint32 nCopy = 0; /* amount of data to copy */
    ehs_uint32* pnIdx = &(xQueue->uInIdx); /* pointer to the queue input index */
	ehs_uint32 nBytesTillWrap; /* number of bytes that can be written before the wrap-around takes place */
	void* pIn = NULL;
    
	EhsTPMutex_lock(EhsTPMutex_consoleQueue);
	//if (nSize>0) xQueue->EhsConsole_buffer_empty = EHS_FALSE;
	pIn = &(xQueue->xQueue[EHS_CONSOLE_QUEUE_INDEX(*pnIdx)]); /* point to start writing data */
	/* determine how much data we can write */
	nCopy = consoleQueueMaxSize - EhsConsoleQueue_length(xQueue);
	if (nCopy > nSize)
	{
		nCopy = nSize;
	}
	else {
		EHSH_LOG_ERROR("QUEUE PUSH queue=%p - Couldn't write all data: requested=%u left=%d used=%u/%u",
		               (void*)xQueue, nSize, nCopy, EhsConsoleQueue_length(xQueue), consoleQueueMaxSize);
		//nCopy = -1;
	}
   /* write the data */
	if (pnIdx && ((EHS_CONSOLE_QUEUE_INDEX(*pnIdx) + nCopy) > consoleQueueMaxSize))
	{
		/* writing data wraps around the input buffer */
        
		nBytesTillWrap = consoleQueueMaxSize - EHS_CONSOLE_QUEUE_INDEX(*pnIdx);
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
	*pnIdx = (*pnIdx + nCopy) & ((consoleQueueMaxSize*2)-1);
	EhsTPMutex_unlock(EhsTPMutex_consoleQueue);
#ifdef EHS_CONSOLE_QUEUE_STATS
	EHSH_LOG_INFO("CQ push queue=%p +%d bytes: used=%u/%u left=%u", (void*)xQueue, nCopy,
	              EhsConsoleQueue_length(xQueue), consoleQueueMaxSize,
	              consoleQueueMaxSize - EhsConsoleQueue_length(xQueue));
#endif
    return nCopy;
}

/**
 * Add a whole record to the console queue, or nothing at all. See console_queue.h.
 */
ehs_bool EhsConsoleQueue_pushRecord(EhsConsoleQueueType* xQueue, const ehs_uint8* pData,
                                    ehs_uint32 nSize, ehs_uint32 nKeepFree)
{
	ehs_bool bRet = EHS_FALSE;

	if (xQueue == NULL || xQueue->xQueue == NULL || pData == NULL || nSize == 0u)
	{
		return EHS_FALSE;
	}

	EhsTPMutex_lock(EhsTPMutex_consoleQueue);
	{
		const ehs_uint32 consoleQueueMaxSize = EhsConsoleQueue_maxSize();
		ehs_uint32* pnIdx = &(xQueue->uInIdx);
		const ehs_uint32 nSpace = consoleQueueMaxSize - EhsConsoleQueue_length(xQueue);

		/* All-or-nothing: only commit if the whole record fits and the reserve survives */
		if (nSpace >= (nSize + nKeepFree))
		{
			void* pIn = &(xQueue->xQueue[EHS_CONSOLE_QUEUE_INDEX(*pnIdx)]);

			if ((EHS_CONSOLE_QUEUE_INDEX(*pnIdx) + nSize) > consoleQueueMaxSize)
			{
				/* record wraps the end of the ring */
				const ehs_uint32 nBytesTillWrap = consoleQueueMaxSize - EHS_CONSOLE_QUEUE_INDEX(*pnIdx);
				EhsMemcpy(pIn,(void*)pData,nBytesTillWrap);
				EhsMemcpy(&(xQueue->xQueue[0]),(void*)(pData + nBytesTillWrap),nSize - nBytesTillWrap);
			}
			else
			{
				EhsMemcpy(pIn,(void*)pData,nSize);
			}
			/* reader can't see the record until the in pointer moves */
			*pnIdx = (*pnIdx + nSize) & ((consoleQueueMaxSize*2)-1);
			bRet = EHS_TRUE;
		}
#ifdef EHS_CONSOLE_QUEUE_STATS
		if (bRet)
		{
			EHSH_LOG_INFO("CQ pushRecord queue=%p +%u bytes: used=%u/%u left=%u",
			              (void*)xQueue, nSize, EhsConsoleQueue_length(xQueue), consoleQueueMaxSize,
			              consoleQueueMaxSize - EhsConsoleQueue_length(xQueue));
		}
		else
		{
			/* nSpace is the space that was actually available at the point of rejection -
			 * requested/keepFree/used/left/capacity together are everything needed to tell
			 * whether this was a near-miss or the queue was already essentially full. */
			EHSH_LOG_INFO("CQ pushRecord queue=%p REJECTED: requested=%u keepFree=%u used=%u/%u left=%u",
			              (void*)xQueue, nSize, nKeepFree, EhsConsoleQueue_length(xQueue), consoleQueueMaxSize, nSpace);
		}
#endif
	}
	EhsTPMutex_unlock(EhsTPMutex_consoleQueue);
	return bRet;
}

/**
 * Remove data from the console queue.
 *
 * Note: the Mascot single-producer/single-consumer scheme this was written for needs no
 * mutex, but the queue now has more than one potential writer, so push and pop both take
 * EhsTPMutex_consoleQueue. EhsConsoleQueue_length/_space do NOT, so they are only safe as
 * hints - never as the basis of a decision. Use EhsConsoleQueue_pushRecord for that.
 * @param xQueue Queue to remove data from.
 * @param pData pointer to the data that we want to retrieve
 * @param nSize amount of data to retrieve
 * @return amount of data that has been read during this pop
 */
ehs_uint32 EhsConsoleQueue_pop(EhsConsoleQueueType* xQueue, ehs_uint8* pData, ehs_uint32 nSize)
{
	if(xQueue==NULL || xQueue->xQueue==NULL){
		return 0;
	}
	const ehs_uint32 consoleQueueMaxSize=EhsConsoleQueue_maxSize();
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

	if ((EHS_CONSOLE_QUEUE_INDEX(*pnIdx) + nCopy) > consoleQueueMaxSize)
	{
		/* reading data wraps around the input buffer */
		nBytesTillWrap = consoleQueueMaxSize - EHS_CONSOLE_QUEUE_INDEX(*pnIdx);
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
	*pnIdx = (*pnIdx + nCopy) & ((consoleQueueMaxSize*2)-1);
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
 * NOTE: EHS_DEBUG_CONSOLE_BUFFER_SIZE is not used in the kernel- it is only set in ert-components
 * 
 * Test whether the queue is full. This occurs if in-pointer ==
 * (out-pointer + EHS_DEBUG_CONSOLE_BUFFER_SIZE) % EHS_DEBUG_CONSOLE_BUFFER_SIZE*2
 * 
 * A neat optimization here is to use out-pointer XOR EHS_DEBUG_CONSOLE_BUFFER_SIZE
 * Examples to prove this works:
 * outpointer(o) = 18 (0x12), EHS_DEBUG_CONSOLE_BUFFER_SIZE(M) = 64 (0x40)
 * (o + M) % M*2 = (18 + 64) % 128 = 82
 * (o ^ M*2) = 0x12 ^ 0x40 = 0x52 = 82
 * outpointer(o) = 77 (0x4d), EHS_DEBUG_CONSOLE_BUFFER_SIZE(M) = 64 (0x40)
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
	return (queue->uInIdx == (queue->uOutIdx ^ EhsConsoleQueue_maxSize()));
}

/**
 * Determine how many elements are in the event queue.
 * Obviously in-out works where out < in.
 * Where in < out, unsigned integer arithmetic works fine
 * provided that we mask result with EHS_DEBUG_CONSOLE_BUFFER_SIZE*2 -1
 * Assertion: this will always result in a value 0..EHS_DEBUG_CONSOLE_BUFFER_SIZE
 * If in < out and no overflow has occured, the following facts obtain:
 * -# in <= EHS_DEBUG_CONSOLE_BUFFER_SIZE
 * -# out >= EHS_DEBUG_CONSOLE_BUFFER_SIZE
 * -# out-in >= EHS_DEBUG_CONSOLE_BUFFER_SIZE
 * @param queue Queue to test
 * @return number of elements currently in the queue
 */
ehs_uint32 EhsConsoleQueue_length(const EhsConsoleQueueType* queue)
{
	return ((queue->uInIdx - queue->uOutIdx)&((EhsConsoleQueue_maxSize()*2)-1));
}

/* Check how much space is left */
ehs_uint32 EhsConsoleQueue_space(const EhsConsoleQueueType* queue) {
	return EhsConsoleQueue_maxSize() - EhsConsoleQueue_length(queue);
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
	if(xQueue==NULL || xQueue->xQueue==NULL){
		return 0;
	}
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

