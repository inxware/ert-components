/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file console_queue.h
 * In this file, the console queue type is defined and the associated functions to
 * operate upon it are declared. This file is derived from event_queue.h
 * Please ensure changes are reflected there too.
 *
 * @sa event_queue.h
 *
 * @author: inx limited
 *
 */

#ifndef EHS_CONSOLE_QUEUE_H
#define EHS_CONSOLE_QUEUE_H
#include "globals.h"

/**
 * Console queue type. This is implemented using the Mascot3 approach to queues
 * to ensure preemption safety without the need for semaphores/locks.
 *
 * Unlike the event queue, indices are uint32. We assume that updates are atomic
 * (i.e. writing to them is not store low 16-bits, store high 16-bits (or vice-versa)
 * Otherwise an interrupt between one instruction and the other could cause havoc
 * if the interrupt results in another queue operation.
 */
typedef struct
{
    ehs_uint32 uInIdx; /**< Pointer to the next place to write the input. Ranges from 0..2*EHS_EVENT_QUEUE_SIZE */
    ehs_uint32 uOutIdx; /**< Pointer to the next place to read the output. Ranges from 0..2*EHS_EVENT_QUEUE_SIZE */
    //ehs_bool EhsConsole_buffer_empty; // default is true.Note we could optiimise this for memory by checking for uInIdx == uOutIdx instead.
    ehs_uint8* xQueue; /**< Contents of the event queue */
} EhsConsoleQueueType;

/**
 * Map uInIdx and uOutIdx to xQueue entries
 */
#define EHS_CONSOLE_QUEUE_INDEX(x) ((x) & (EhsConsoleQueue_maxSize()-1))

ehs_uint32 EhsConsoleQueue_maxSize();

/**
 * Add new data to the console queue.
 * @param xQueue Queue to add data to.
 * @param pData Pointer to the data to add to queue.
 * @param nSize amount of data to add to the queue.
 * @return Amount of data that was added to the queue (0 = unsuccessful)
 */
ehs_sint32 EhsConsoleQueue_push(EhsConsoleQueueType* xQueue, ehs_uint8* pData, ehs_uint32 nSize);

/**
 * Add a whole record to the console queue, or nothing at all.
 *
 * The space test and the copy happen together under the queue lock, so the record can
 * never be split or interleaved with another writer's, and the answer is final on the
 * first call - callers must not retry or wait.
 *
 * @param xQueue    Queue to add the record to.
 * @param pData     Record to add.
 * @param nSize     Length of the record.
 * @param nKeepFree Bytes that must remain free afterwards. Normal records pass the
 *                  caller's reserve so that a failure notice can always be emitted;
 *                  the failure notice itself passes 0 to spend that reserve.
 * @return true if the whole record was queued, false if it was not queued at all.
 */
ehs_bool EhsConsoleQueue_pushRecord(EhsConsoleQueueType* xQueue, const ehs_uint8* pData,
                                    ehs_uint32 nSize, ehs_uint32 nKeepFree);

/**
 * Remove data from the console queue
 * @param xQueue Queue to remove data from.
 * @param pData pointer to the data that we want to retrieve
 * @param nSize amount of data to retrieve
 * @return amount of data that has been read during this pop
 */
ehs_uint32 EhsConsoleQueue_pop(EhsConsoleQueueType* xQueue, ehs_uint8* pData, ehs_uint32 nSize);

/**
 * Test whether the queue is empty
 * @param queue Queue to test
 * @return true if queue is empty
 */
ehs_bool EhsConsoleQueue_isEmpty(const EhsConsoleQueueType* queue);

/**
 * Test whether the queue is full
 * @param queue Queue to test
 * @return true if queue is full
 */
ehs_bool EhsConsoleQueue_isFull(const EhsConsoleQueueType* queue);

/**
 * Determine how many elements are in the queue
 * @param queue Queue to test
 * @return number of elements currently in the queue
 */
ehs_uint32 EhsConsoleQueue_length(const EhsConsoleQueueType* queue);

/**
 * Determine how many elements can be written into the queue
 * @param queue Queue to test
 * @return number of elements that can be pushed
 */
ehs_uint32 EhsConsoleQueue_space(const EhsConsoleQueueType* queue);


/**
 * Non-destructively lookahead into the queue to see if a specified character
 * can be found there
 *
 * @param xQueue Queue to search
 * @param nChar Character to search for
 * @return how many characters need to be read to obtain the specified character
 * (0 if the character doesn't exist)
 */
ehs_uint32 EhsConsoleQueue_peek(const EhsConsoleQueueType* xQueue, ehs_uint8 nChar);

/**
 * Reset the queue - empty all items from the queue
 * @param queue Queue to reset
 */
void EhsConsoleQueue_reset(EhsConsoleQueueType* queue);

#endif /* EHS_CONSOLE_QUEUE_H */

