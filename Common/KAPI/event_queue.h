/** @file event_queue.h
 * In this file, the event queue type and the associated functions to
 * operate upon it are defined.
 *
 * @sa console_queue.h
 * 
 * @author: inx limited
 * @version: $Revision: 780 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

#ifndef EHS_EVENT_QUEUE_H
#define EHS_EVENT_QUEUE_H

#include "ehs_fb_types.h" // needed for FID (Function Instance) structur references
#include "target_config.h"

/**
 * Event queue type. This is implemented using the Mascot3 approach to queues
 * to ensure preemption safety without the need for semaphores/locks.
 *
 * Input and output indexes are uint16s to ensure that updating them is an atomic
 * action. We might implement EHS on a 16 bit architecture. If we did that, we
 * couldn't guarantee that writing to a 32 bit value takes place within a single
 * instruction.
 */
typedef struct {
	ehs_uint16 uInIdx; /**< Pointer to the next place to write the input. Ranges from 0..2*EHS_MAX_EVENT_QUEUE_SIZE */
	ehs_uint16 uOutIdx; /**< Pointer to the next place to read the output. Ranges from 0..2*EHS_MAX_EVENT_QUEUE_SIZE */
	EhsFunctionInstanceIndexType xQueue[EHS_MAX_EVENT_QUEUE_SIZE]; /**< Contents of the event queue */
} EhsEventQueueType;

/**
 * Map uInIdx and uOutIdx to xQueue entries
 */
#define EHS_EVENT_QUEUE_INDEX(x) ((x) & (EHS_MAX_EVENT_QUEUE_SIZE-1))


/**
 * Add a new event to the event queue.
 * @param queue Queue to add event to.
 * @param event Event to add to queue.
 * @return false if queue is full before the push takes place.
 */
EHS_GLOBAL ehs_bool EhsEventQueue_push(EhsEventQueueType* queue, EhsFunctionInstanceIndexType eventId);
/**
 * Remove an event from the event queue
 * @param queue Queue to remove event from.
 * @param pEventId pointer to the event id that we've just retrieved
 * @return false if the queue is empty prior to this call
 */
EHS_GLOBAL ehs_bool EhsEventQueue_pop(EhsEventQueueType* queue, EhsFunctionInstanceIndexType* pEventId);

/**
 * Test whether the queue is empty
 * @param queue Queue to test
 * @return true if queue is empty
 */
EHS_GLOBAL ehs_bool EhsEventQueue_isEmpty(const EhsEventQueueType* queue);

/**
 * Test whether the queue is full
 * @param queue Queue to test
 * @return true if queue is full
 */
EHS_GLOBAL ehs_bool EhsEventQueue_isFull(const EhsEventQueueType* queue);

/**
 * Determine how many elements are in the event queue
 * @param queue Queue to test
 * @return number of elements currently in the queue
 */
EHS_GLOBAL ehs_uint16 EhsEventQueue_length(const EhsEventQueueType* queue);

/**
 * Reset the queue - empty all items from the queue
 * @param queue Queue to reset
 */
EHS_GLOBAL void EhsEventQueue_reset(EhsEventQueueType* queue);

#endif /* EHS_EVENT_QUEUE_H */

