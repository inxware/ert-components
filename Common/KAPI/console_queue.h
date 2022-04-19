/** @file console_queue.h
 * In this file, the console queue type is defined and the associated functions to
 * operate upon it are declared. This file is derived from event_queue.h
 * Please ensure changes are reflected there too.
 *
 * @sa event_queue.h
 * 
 * @author: inx limited
 * @version: $Revision: 780 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

#ifndef EHS_CONSOLE_QUEUE_H
#define EHS_CONSOLE_QUEUE_H
#include "ehs_types.h"
#include "target_config.h"

/**
 * Console queue type. This is implemented using the Mascot3 approach to queues
 * to ensure preemption safety without the need for semaphores/locks.
 *
 * Unlike the event queue, indices are uint32. We assume that updates are atomic
 * (i.e. writing to them is not store low 16-bits, store high 16-bits (or vice-versa)
 * Otherwise an interrupt between one instruction and the other could cause havoc
 * if the interrupt results in another queue operation.
 */
typedef struct {
	ehs_uint32 uInIdx; /**< Pointer to the next place to write the input. Ranges from 0..2*EHS_MAX_EVENT_QUEUE_SIZE */
	ehs_uint32 uOutIdx; /**< Pointer to the next place to read the output. Ranges from 0..2*EHS_MAX_EVENT_QUEUE_SIZE */
	//ehs_bool EhsConsole_buffer_empty; // default is true.Note we could optiimise this for memory by checking for uInIdx == uOutIdx instead.
	ehs_uint8 xQueue[EHS_MAX_CONSOLE_QUEUE_SIZE]; /**< Contents of the event queue */
} EhsConsoleQueueType;

/**
 * Map uInIdx and uOutIdx to xQueue entries
 */
#define EHS_CONSOLE_QUEUE_INDEX(x) ((x) & (EHS_MAX_CONSOLE_QUEUE_SIZE-1))


/**
 * Add new data to the console queue.
 * @param xQueue Queue to add data to.
 * @param pData Pointer to the data to add to queue.
 * @param nSize amount of data to add to the queue.
 * @return Amount of data that was added to the queue (0 = unsuccessful)
 */
EHS_GLOBAL ehs_uint32 EhsConsoleQueue_push(EhsConsoleQueueType* xQueue, ehs_uint8* pData, ehs_uint32 nSize);

/**
 * Remove data from the console queue
 * @param xQueue Queue to remove data from.
 * @param pData pointer to the data that we want to retrieve
 * @param nSize amount of data to retrieve
 * @return amount of data that has been read during this pop
 */
EHS_GLOBAL ehs_uint32 EhsConsoleQueue_pop(EhsConsoleQueueType* xQueue, ehs_uint8* pData, ehs_uint32 nSize);

/**
 * Test whether the queue is empty
 * @param queue Queue to test
 * @return true if queue is empty
 */
EHS_GLOBAL ehs_bool EhsConsoleQueue_isEmpty(const EhsConsoleQueueType* queue);

/**
 * Test whether the queue is full
 * @param queue Queue to test
 * @return true if queue is full
 */
EHS_GLOBAL ehs_bool EhsConsoleQueue_isFull(const EhsConsoleQueueType* queue);

/**
 * Determine how many elements are in the queue
 * @param queue Queue to test
 * @return number of elements currently in the queue
 */
EHS_GLOBAL ehs_uint32 EhsConsoleQueue_length(const EhsConsoleQueueType* queue);

/**
 * Determine how many elements can be written into the queue
 * @param queue Queue to test
 * @return number of elements that can be pushed
 */
EHS_GLOBAL ehs_uint32 EhsConsoleQueue_space(const EhsConsoleQueueType* queue);


/**
 * Non-destructively lookahead into the queue to see if a specified character
 * can be found there
 *
 * @param xQueue Queue to search
 * @param nChar Character to search for
 * @return how many characters need to be read to obtain the specified character
 * (0 if the character doesn't exist)
 */
EHS_GLOBAL ehs_uint32 EhsConsoleQueue_peek(const EhsConsoleQueueType* xQueue, ehs_uint8 nChar);

/**
 * Reset the queue - empty all items from the queue
 * @param queue Queue to reset
 */
EHS_GLOBAL void EhsConsoleQueue_reset(EhsConsoleQueueType* queue);

#endif /* EHS_CONSOLE_QUEUE_H */

