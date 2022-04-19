/*
 * Filename : BFGlobalHeap.h
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
 * \file BFGlobalHeap.h
 *
 * This file defines the interface to a global heap manager required
 * for use with the BitFlash Mobile SVG Player SDK.
 */
#ifndef BFGLOBALHEAP_h
#define BFGLOBALHEAP_h

#ifdef __cplusplus
BF_BEGIN_EXTERN_C
#endif

/**
 * Create a heap memory context
 * \param pMemory memory for initial context use
 * \param uInitialSize size of initial memory in bytes (must be at least 512 bytes in size)
 * \return pointer to heap memory context or 0
 */
BF_PLAYER_IMPORT BFHeapContext * BFmemCreateContext( void *pMemory, BF_UINT32 uInitialSize );

/**
 * Destroy a memory context manager
 * \param pMemCtx memory context
 */
BF_PLAYER_IMPORT  void       BFmemDestroyContext( BFHeapContext* pMemCtx );

/**
 * Allocate memory - identical to clib malloc()
 * \param pMemCtx memory context
 * \param uSize size of the memory to be allocated
 * \return pointer to memory
 */
BF_PLAYER_IMPORT  void       *BFmemAlloc( BFHeapContext* pMemCtx, BF_UINT32 uSize );

/**
 * Allocate memory which is initially cleared to 0 - identical to clib calloc()
 * \param pMemCtx memory context
 * \param uSize size of the memory to be allocated
 * \return pointer to memory
 */
BF_PLAYER_IMPORT  void       *BFmemCalloc( BFHeapContext* pMemCtx, BF_UINT32 uSize );

/**
 * Reallocate memory - identical to clib realloc()
 * \param pMemCtx memory context
 * \param pBuffer pointer to allocated buffer
 * \param uNewSize new size of the memory to be allocated
 * \return pointer to new memory
 */
BF_PLAYER_IMPORT  void       *BFmemRealloc( BFHeapContext* pMemCtx, void *pBuffer, BF_UINT32 uNewSize );

/**
 * free allocated memory - identical to free()
 * \param pMemCtx memory context
 * \param pBuffer pointer to allocated buffer
 */
BF_PLAYER_IMPORT  void       BFmemFree( BFHeapContext* pMemCtx, void *pBuffer );

/**
 * set memory to a given value - identical to memset()
 * \param pDest pointer to allocated buffer
 * \param uValue value to set
 * \param uSize the size of the memory set
 */
BF_PLAYER_IMPORT  void       BFmemset(void *pDest, BF_UINT8 uValue, BF_UINT32 uSize);

/**
 * move memory from one location to another- identical to memmove()
 * \param pvDst the destination of the move - a pointer to an allocated buffer
 * \param pvSrc the source of the move - a pointer to an allocated buffer
 * \param uSize the size of the memory move
 */
BF_PLAYER_IMPORT  void       BFmemmove( void *pvDst, const void *pvSrc, BF_UINT32 uSize );

/**
 * determine of a previous allocation has failed
 * \param pMemCtx memory context
 * \return BF_TRUE of the memory context has set out of memory on an allocation attempt
 */
BF_PLAYER_IMPORT  BF_BOOL    BFmemGetOutOfMemory( BFHeapContext* pMemCtx );

/**
 * Callback for adding to the memory pool.
 * \param pUserData application pointer, not used by memory context
 * \param uRequestSize bytes to be allocated
 * \return a pointer to allocated memory
 */
typedef void    *(*BFmemAddAreaCB)( BF_INT_VOID pUserData, BF_UINT32 uRequestSize );

/**
 * Callback for removing memory from the memory pool.
 * \param pUserData application pointer, not used by memory context
 * \param pMemory bytes a pointer to previously allocated memory
 */
typedef void    (*BFmemRemoveAreaCB)( BF_INT_VOID pUserData, void *pMemory );

/**
 * Set the memory pool callback functions.
 * \param pMemCtx memory context to add callbacks to
 * \param pUserData application pointer, not used by memory context
 * \param uExpansionIncr bytes to be allocated on each request
 (        (the expansion increment can be specified only once)
 * \param pfnAdd memory pool add request callback
 * \param pfnRemove memory pool remove request callback
 */
BF_PLAYER_IMPORT  void       BFmemSetCallbacks( BFHeapContext *pMemCtx, BF_INT_VOID pUserData, BF_UINT32 uExpansionIncr, BFmemAddAreaCB pfnAdd, BFmemRemoveAreaCB pfnRemove );


/**
 * Increment reference counter for memory manager. This count is simply stored in the BFHeapContext - it is up to 
 * caller to implement creation/destruction using reference counting, if desired.
 * \param pMemCtx memory context
 */
BF_PLAYER_IMPORT BF_INT32	BFincrementReferenceCounter( BFHeapContext *pMemCtx );

/**
 * Decrement reference counter for memory manager. This count is simply stored in the BFHeapContext - it is up to 
 * caller to implement creation/destruction using reference counting, if desired.
 * \param pMemCtx memory context
 */
BF_PLAYER_IMPORT BF_INT32	BFdecrementReferenceCounter( BFHeapContext *pMemCtx );

#ifdef __cplusplus
BF_END_EXTERN_C
#endif

#endif /* BFHeap */

