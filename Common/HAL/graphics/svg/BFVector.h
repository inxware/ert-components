/*
 * Filename : BFVector.h
 *
 *  Version : 6.4.0_2271s
 *     Date : 20080508-20.49.08
 *
 *
 * (c) COPYRIGHT 2002- 2008 Open Text Corp. - BitFlash Division. ALL RIGHTS RESERVED
 *
 *
 * This software is the property and confidential information of Open Text and is furnished under
 * license by BitFlash Inc.  This software may be used only in accordance with the terms of said license.
 *
 * This software may not be copied, transmitted, provided to or otherwise made available to any other person,
 * company, corporation or other entity except as specified in the terms of said license.
 *
 * This copyright notice may not be removed or altered without the prior written permission of Open Text.
 *
 *
 */
/**
 * \file BFVector.h
 *
 * This file defines a Vector class.
 *
 * Copyright ©2000-2002 BitFlash Graphics Inc. All rights reserved.
 */


#ifndef BITFLASH_VECTOR_H_
#define BITFLASH_VECTOR_H_

#include "BFBasicTypes.h"
#include "BFGlobalHeap.h"

#ifdef __cplusplus
BF_BEGIN_EXTERN_C
#endif

/* Define the type for a vector index.
   Also define an invalid index which, if used, has a better chance of being detected
   as a memory violation than if -1 was used.  Using -1 as an array index is more
   likely to provoke heap corruption which is harder to debug.
*/
typedef BFUInteger BFVectorIndex;
#define BF_VECTOR_INVALID_INDEX ((BFVectorIndex)0x80000000)

typedef struct
{
    void            *m_pVector;         /* Vector data */
    BFHeapContext   *m_pMemCtx;         /* Memory context */
    BFVectorIndex   m_uCapacity;        /* Element capacity (allocated) */
    BFVectorIndex   m_uNumberOfItems;   /* Elements stored */
    BFUInteger      m_uElementSize;     /* Size of each element in bytes */
}
BFVector;

/* Allocate a vector and attempt to allocate space for the specified number of elements.

   If both the vector and the element space can be allocated, the vector capacity will be >= the
   initial size, the number of elements will be set to the initial size and the vector pointer
   is returned.
   If either the vector or the element space cannot be allocated when initial size != 0, any allocations
   will be freed and 0 will be returned.
*/
BF_PLAYER_IMPORT BFVector* BFVectorAlloc( BFHeapContext *pMemCtx, BF_UINT16 uElementSize, BFVectorIndex InitialSize );
#define BF_VECTOR_ALLOC(pMemCtx,elementSize) \
    BFVectorAlloc( (pMemCtx), (BF_UINT16)(elementSize), 0 )

#define BF_VECTOR_ALLOC_SIZE(pMemCtx,elementSize,initialSize) \
    BFVectorAlloc( (pMemCtx), (BF_UINT16)(elementSize), (initialSize) )

/* Initialize a statically allocated vector */
BF_PLAYER_IMPORT void BFVectorInitialize( BFHeapContext *pMemCtx, BFVector *vector, BF_UINT16 elementSize );
#define BF_VECTOR_INITIALIZE(pMemCtx, vector, elementSize) \
    BFVectorInitialize( (pMemCtx), (vector), (BF_UINT16)(elementSize) );

/* Destroy a vector */
#define BF_VECTOR_DESTROY(vector) \
    BFmemFree((vector)->m_pMemCtx,(vector)->m_pVector)

/* Free a vector obtained via BF_VECTOR_ALLOC */
BF_PLAYER_IMPORT void BFVectorFree( BFVector *pVector );
#define BF_VECTOR_FREE(vector) \
    BFVectorFree( vector )

/* Empty the vector but do not free memory */
#define BF_VECTOR_CLEAR(vector) (vector)->m_uNumberOfItems = 0;

/* Empty the vector , free memory, zero pointers and counters */
#define BF_VECTOR_EMPTY(vector) \
    do { \
    if ( (vector)->m_pMemCtx ) \
        BFmemFree((vector)->m_pMemCtx,(vector)->m_pVector);\
    (vector)->m_uNumberOfItems = 0;\
    (vector)->m_pVector = 0;\
    (vector)->m_uCapacity = 0; } while(0)


/* Empty the vector but do not free memory */
#define BF_VECTOR_GET_COUNT(vector) \
    ((vector)->m_uNumberOfItems)

/* Return the object at the given index, index assumed to be valid */
#define BF_VECTOR_GET(vector,Index) \
    ((BF_UINT8*)(vector)->m_pVector + (BFVectorIndex)(Index) * (vector)->m_uElementSize)

/* Replace the object at the given index, index assumed to be valid */
#define BF_VECTOR_REPLACE(vector,Index,pObject) \
    do { BFmemmove( \
        (BF_UINT8*)(vector)->m_pVector + (BFVectorIndex)(Index) * (vector)->m_uElementSize, \
        (pObject), (vector)->m_uElementSize ); } while(0)

/* Removes the object at the given index. If the index
   is out of bounds, nothing happens. */
BF_PLAYER_IMPORT void BFVectorRemoveItem( BFVector *pVector, BFVectorIndex Index );
#define BF_VECTOR_REMOVE( vector,Index ) \
    if ( (BFVectorIndex)(Index) == (vector)->m_uNumberOfItems-1 )   \
        (vector)->m_uNumberOfItems--;   \
    else \
        BFVectorRemoveItem( (vector), (Index) )

/* Ensure the vector can hold the specified number of elements.
   Return BF_TRUE if vector has been resized to hold at least the specified
   number of elements.  Return BF_FALSE if not enough capacity. */
BF_PLAYER_IMPORT BF_BOOL BFVectorEnsureCapacity( BFVector *pVector, BFVectorIndex Size );
#define BF_VECTOR_CAPACITY( vector, size ) \
    BFVectorEnsureCapacity( (vector), (size) )

/* Add an object to the end of the vector and return the index of the new object */
BF_PLAYER_IMPORT BFVectorIndex BFVectorAppendItem( BFVector *pVector, void const *pObject );
#define BF_VECTOR_APPEND( vector, object ) \
    BFVectorAppendItem( (vector), (object) )

/* appends an array of uCount objects to the vector */
BF_PLAYER_IMPORT BFVectorIndex BFVectorAppendItems( BFVector *pVector, void const *pObject, BFVectorIndex uCount );
#define BF_VECTOR_APPEND_ITEMS( vector, object, count ) \
    BFVectorAppendItems( (vector), (object), (count) )


/* Inserts the object at the given index. If the index is equal to zero,
   the point will be prepended to the list. If the index is greater
   than the length of the list, the point will be appended to the list.
   Returns the index of the point. */
BF_PLAYER_IMPORT BFVectorIndex BFVectorInsertItemBefore( BFVector *pVector, BFVectorIndex Index, void const *pObject );
#define BF_VECTOR_INSERT( vector, Index, object ) \
    BFVectorInsertItemBefore( (vector), (Index), (object) )



#ifdef __cplusplus
BF_END_EXTERN_C
#endif

#endif

