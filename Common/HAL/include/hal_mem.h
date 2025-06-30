/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file memory.h
 * In this file, an interface for memory allocation is defined.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_MEMORY_H
#define EHS_MEMORY_H

/*****************************************************************************/
/* Included files */

#include "ehs_types.h"
#include "target_mem.h"

/*****************************************************************************/
/* Define macros  */

/* this is the default system memory allocation function
 * This does not use the System, SODL, App level pools and won't be freed
 * This should only be used by parts of the system and HAL
 * */
#define EhsHMem_permAlloc(x) EhsTMem_alloc(x)
#define EhsHMem_permFree(x) EhsTMem_free(x)


/**
 * These should be used by components only -this data is freed after each app run
 * Readonly memory currently uses the same memory pool as Writeable.
 * Separate call is provided for future compatibility.
 * @todo DEBUG version of these should be defined here where the file and line info is set to NULL, and the EhsHMem_Alloc()
 */
#define EhsHMem_writeableAlloc(x) EhsHMem_Alloc(x,__FILE__,__LINE__) //
#define EhsHMem_readonlyAlloc(x) EhsHMem_Alloc(x,__FILE__,__LINE__)
#define EhsHMem_writeNameAlloc(x,y) EhsHMem_Alloc(x,y,__LINE__) // what the hell is this?
#define EhsHMem_tempAlloc(x) EhsHMem_tAlloc(x,__FILE__,__LINE__) // why is this different?
/*****************************************************************************/
/* Define types */

/**
 * Indicates which memory we are currently writing to.
 */
typedef enum
{
    EHSHM_POOL_SYSTEM, EHSHM_POOL_APP_SODL, EHSHM_POOL_APP_DATA
} EhsHMPoolType;

/*****************************************************************************/
/* Declare global variables */


/*****************************************************************************/
/* Declare function prototypes  */

/**
 * Allocate memory to be used to hold information that is read/writeable
 * during the application execution.
 */
void* EhsHMem_Alloc(ehs_uint32 nSizeToAllocate,const char* fileName, ehs_uint32 lineNumber);

/**
 * Request temporary memory - that is memory that we can individually free,
 * or allow it to be released by temporaryFreeAll()
 */
EHS_GLOBAL void* EhsHMem_tAlloc(ehs_uint32 nSizeToAllocate, char* fileName, ehs_uint32 lineNumber);

/**
 * Free memory that's been allocated temporarily. More of an aspiration than an actual
 * request at the moment...
 */
EHS_GLOBAL void EhsHMem_tempFree(void*);

/**
 * Switch memory pool. - Of course nobody bothered to mention here that this freed memory as pools are changed:
 * EHSHM_POOL_SYSTEM - remove al SODL and app data when we get to here.
 * EHSHM_POOL_APP_SODL - switch to here and all pre application memory is freed
 * EHSHM_POOL_APP_DATA - switch to here and allocations are for application level
 *
 * Note this allocator should not be used for services that run in parallel with applications such as devman or any permenant features such as A/V systems
 */
EHS_GLOBAL void EhsHMem_switchPool(EhsHMPoolType xPool);

/**
 * Initialise memory system
 */
EHS_GLOBAL void EhsHMem_init(void);

/**
 * Return memory used so far
 * @return Total memory allocated
 */
EHS_GLOBAL ehs_uint32 EhsHMem_used(void);

#ifndef EhsTMem_alloc
/**
 * Target-specific memory allocation function. Note this is to be used only by
 * EhsHMem_* functions.
 * @param nSize memory to allocate
 * @return pointer to allocated memory, or null if unsuccessful.
 */
void* EhsTMem_alloc(ehs_uint32 nSize);
#endif

#ifndef EhsTMem_free
/**
 * Target-specific memory free function. Note this is to be used only by
 * EhsHMem_* functions.
 * @param pMem Memory to free
 */
void EhsTMem_free(void* pMem);
#endif


#endif /* EHS_MEMORY_H */
