/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/** @file target.c
 * In this file, the memory interface is defined.
 *
 * @author: inx limited
 *
 */

/**
 * @page Verification Verification report
 * @section hal_graphics_font
 * @anchor hal_graphics_font
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on
 * Last modified on $Date:$
 *
 * This file contained no derogations to the MISRA standard.
 */

/*****************************************************************************/
/* Included files */

#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_MEMORY

#include "hal_logger.h"
#include "hal_mem.h"
#include "hal_process.h"
#include "messages.h"
#include "globals.h"
#include "target_config.h"
#include "target.h"

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

#ifdef EHS_CONFIG_MEM_CHECK
#define EHSL_PRE_PATTERN 0x456789ab	/**< "unique" pattern used to check for memory corruption */
#define EHSL_POST_PATTERN0 0xfe	/**< 1st byte of "unique" pattern used to check for memory corruption */
#define EHSL_POST_PATTERN1 0xdc	/**< 1st byte of "unique" pattern used to check for memory corruption */
#define EHSL_POST_PATTERN2 0xba	/**< 1st byte of "unique" pattern used to check for memory corruption */
#define EHSL_POST_PATTERN3 0x98	/**< 1st byte of "unique" pattern used to check for memory corruption */
#endif

/**
 * Represents a single memory allocation. Uses a doubly linked list
 * to support fast item removal
 */
typedef struct EhsLQMemItemStruct
{
    struct EhsLQMemItemStruct* pPrev; /**< Pointer to the previously allocated item (root if NULL) */
    struct EhsLQMemItemStruct* pNext; /**< Pointer to the next allocated item */
#ifdef EHS_CONFIG_MEM_CHECK

    ehs_uint32 nLen;					/**< Length of the allocated memory */
    ehs_uint32 nPreBlock;				/**< Memory check pattern before data block */
#endif
    void * nMem;				 	/**< Start of user memory */
#ifdef EHS_CONFIG_MEM_CHECK
    ehs_uint32 nDummy;				/**< Memory check pattern after data block. Declaration ensures 4 bytes
	 								 * are reserved here */
    ehs_uint32 lineNumber;
    ehs_char fileName[EHS_STRING_LENGTH_MAX];
#endif
} EhsLMemQuickItemType;

/**
 * Represents a single memory allocation. Uses a singly linked list
 * to allow for efficient memory representation. Deletion of random elements is
 * slow (but deleting all elemens from the start is quick)
 */
typedef struct EhsLSMemItemStruct
{
    struct EhsLSMemItemStruct* pNext; /**< Pointer to the next allocated item */
#ifdef EHS_CONFIG_MEM_CHECK
    ehs_uint32 lineNumber;
    ehs_char fileName[EHS_STRING_LENGTH_MAX];
    ehs_uint32 nLen;					/**< Length of the allocated memory */
    ehs_uint32 nPreBlock;				/**< Memory check pattern before data block */
#endif
    void * nMem;				 	/**< Start of user memory */
#ifdef EHS_CONFIG_MEM_CHECK
    ehs_uint32 nDummy;				/**< Memory check pattern after data block.  Declaration ensures 4 bytes
	 								 * are reserved here */
#endif
} EhsLMemSmallItemType;

/*****************************************************************************/
/* Declare prototypes of local functions */

/**
 * Free all memory allocated with "small" allocations
 * @param[in] Pointer to the start of the chain of small items
 */
EHS_LOCAL void EhsL_freeAllSmall(EhsLMemSmallItemType** ppSmall);


/**
 * Free all memory allocated with "quick" allocations
 * @param[in] Pointer to the start of the chain of small items
 */
EHS_LOCAL void EhsL_freeAllQuick(EhsLMemQuickItemType** ppQuick);

#ifdef EHS_CONFIG_MEM_CHECK
/**
 * Check that the memory item hasn't overrun. Generates an error message
 * if the overrun occurs.
 * @param[in] Pointer to the memory item to check
 */
EHS_LOCAL void EhsL_checkSmall(const EhsLMemSmallItemType* pSmall);

/**
 * Check that the memory item hasn't overrun. Generates an error message
 * if the overrun occurs.
 * @param[in] Pointer to the memory item to check
 */
EHS_LOCAL void EhsL_checkQuick(const EhsLMemQuickItemType* pQuick);
/**
 * Set the memory pattern for a small item - used to check for
 * overruns/underruns
 * @param[in] pSmall Item to set the memory for
 */
EHS_LOCAL void EhsL_setPatternSmall(EhsLMemSmallItemType* pSmall);

/**
 * Set the memory pattern for a quick item - used to check for
 * overruns/underruns
 * @param[in] pQuick Item to set the memory for
 */
EHS_LOCAL void EhsL_setPatternQuick(EhsLMemQuickItemType* pQuick);
#else
#define EhsL_checkSmall(x)
#define EhsL_checkQuick(x)
#define EhsL_setPatternSmall(x)
#define EhsL_setPatternQuick(x)
#endif

/*****************************************************************************/
/* Variables defined with file-scope */


EHS_LOCAL struct
{
    EhsHMPoolType xCurrentPool;	/**< Indicates which pool to place memory requests into */
    EhsLMemSmallItemType* pSystem;	/**< Pointer to the system memory */
    EhsLMemSmallItemType* pAppSodl;	/**< Pointer to memory allocated for SODL */
    EhsLMemSmallItemType* pAppData;	/**< Pointer to memory allocated for data */
    EhsLMemQuickItemType* pTempSodl;		/**< Pointer to memory allocated for temporary use before the application starts
											    (memory is not reset until new application is loaded) */
    EhsLMemQuickItemType* pTempApp;	/**< Pointer to memory allocated for temporary use after the application starts
									     (memory is reset whenever application restarts) */
    ehs_uint8 nQuickOffset;			/**< Offset of the nMem array from the start of a EhsLMemQuickItemType */
    ehs_uint8 nSmallOffset;			/**< Offset of the nMem array from the start of a EhsLMemSmallItemType */
#ifdef EHS_CONFIG_MEM_CHECK
    ehs_uint32 nUsed;				/**< Reports amount of memory used so far */
#endif
} EhsLMem;

/*****************************************************************************/
/* Variables defined with global-scope */

/*****************************************************************************/
/* Function definitions */

/**
 * Allocate memory to be used to hold information that is read/writeable
 * during the application execution.
 * This is cleaned up after an application completes
 * @TODO this needs sorting desperately!
 */
EHS_GLOBAL void* EhsHMem_Alloc(ehs_uint32 nSizeToAllocate, const char* fileName, ehs_uint32 lineNumber)
{
    EhsLMemSmallItemType* pItem;	/* contains new memory allocated */
    void* pRet = NULL;				/* points to the user part of the memory */
#ifdef EHS_CONFIG_MEM_CHECK
    ehs_uint32 nAmount = nSizeToAllocate+sizeof(EhsLMemSmallItemType)-sizeof(ehs_uint32);		/* quantity of memory to allocates minus pattern??/ */
#else
    ehs_uint32 nAmount = nSizeToAllocate+sizeof(EhsLMemSmallItemType);//-sizeof(ehs_uint32);
#endif
    //EHSH_LOG_ENTER("EhsHMem_tempAlloc(%d)",nSizeToAllocate);
    EhsTPMutex_lock(EhsTPMutex_mem);
//@todo need to optimise all this with #ifdef EHS_CONFIG_MEM_CHECK
    pItem = EhsTMem_alloc(nAmount);
#ifdef EHS_CONFIG_MEM_CHECK
    strcpy(pItem->fileName,fileName);
    pItem->lineNumber = lineNumber;
#endif
    if (pItem)
    {
#ifdef EHS_CONFIG_MEM_CHECK
        EhsLMem.nUsed += nAmount;
        pItem->nLen = nSizeToAllocate;
#endif
        /* insert item into linked list */
        switch (EhsLMem.xCurrentPool)
        {
        case EHSHM_POOL_SYSTEM:
            pItem->pNext = EhsLMem.pSystem;
            EhsLMem.pSystem = pItem;
            break;
        case EHSHM_POOL_APP_SODL:
            pItem->pNext = EhsLMem.pAppSodl;
            EhsLMem.pAppSodl = pItem;
            break;
        case EHSHM_POOL_APP_DATA:
            pItem->pNext = EhsLMem.pAppData;
            EhsLMem.pAppData = pItem;
            break;
        }
        EhsL_setPatternSmall(pItem);
        pRet = &(pItem->nMem);
    }
    else
    {
        EHSH_LOG_ERROR(EHS_MSG_ERROR_MEMORY);
    }
    EhsTPMutex_unlock(EhsTPMutex_mem);
    //EHSH_LOG_EXIT("EhsHMem_tempAlloc() -> %x",pRet);
    return pRet;
}

/**
 * @brief  PRIVATE function that is called a macro to insert the code location
 *
 * Request temporary memory - that is memory that we can individually free,
 * or allow it to be released by temporaryFreeAll()
 */
void* EhsHMem_tAlloc(ehs_uint32 nSizeToAllocate, char* fileName, ehs_uint32 lineNumber)
{

    EhsLMemQuickItemType* pItem;	/* contains new memory allocated */
    void* pRet = NULL;				/* points to the user part of the memory */
    ehs_uint32 nAmount = nSizeToAllocate+sizeof(EhsLMemQuickItemType)-sizeof(ehs_uint32);
    /* quantity of memory to allocate */
    EhsLMemQuickItemType **ppPool;	/* pool to add memory to */
    //EHSH_LOG_ENTER("EhsHMem_tempAlloc(%d)",nSizeToAllocate);
    EhsTPMutex_lock(EhsTPMutex_mem);
    pItem = EhsTMem_alloc(nAmount);
    if (pItem)
    {
#ifdef EHS_CONFIG_MEM_CHECK
        strcpy(pItem->fileName,fileName);
        pItem->lineNumber = lineNumber;
        EhsLMem.nUsed += nAmount;
        pItem->nLen = nSizeToAllocate;
#endif
        if (EhsLMem.xCurrentPool == EHSHM_POOL_APP_DATA)
        {
            ppPool = &(EhsLMem.pTempApp);
        }
        else
        {
            ppPool = &(EhsLMem.pTempSodl);
        }
        /* insert item into linked list */
        pItem->pNext = *ppPool;
        pItem->pPrev = NULL;
        if (*ppPool)
        {
            (*ppPool)->pPrev = pItem;
        }
        *ppPool = pItem;
        EhsL_setPatternQuick(pItem);
        pRet = &(pItem->nMem);
    }
    else
    {
        EHSH_LOG_ERROR(EHS_MSG_ERROR_MEMORY);
    }
    EhsTPMutex_unlock(EhsTPMutex_mem);
    //EHSH_LOG_EXIT("EhsHMem_tempAlloc() -> %x",pRet);
    return pRet;
}

/**
 * Free memory that's been allocated temporarily. More of an aspiration than an actual
 * request at the moment...
 */
void EhsHMem_tempFree(void* pData)
{
    EhsLMemQuickItemType* pItem;
    //EHSH_LOG_ENTER("EhsHMem_tempFree(%x)",pData);
    EhsTPMutex_lock(EhsTPMutex_mem);

    pItem = (EhsLMemQuickItemType*)((ehs_uint8*)pData - EhsLMem.nQuickOffset);
    EhsL_checkQuick(pItem);

    /* unlink pItem from list */
    if (pItem->pPrev)
    {
        /* is there a previous item? */
        pItem->pPrev->pNext = pItem->pNext;
    }
    else
    {
        /* no, just update the EhsLMem entry */
        if (EhsLMem.pTempSodl == pItem)
        {
            EhsLMem.pTempSodl = pItem->pNext;
        }
        else if (EhsLMem.pTempApp == pItem)
        {
            EhsLMem.pTempApp = pItem->pNext;
        }
    }
    if (pItem->pNext)
    {
        /* if there is a next item, update its prev pointer */
        pItem->pNext->pPrev = pItem->pPrev;
    }
    /* free item */
#ifdef EHS_CONFIG_MEM_CHECK
    EhsLMem.nUsed -= (pItem->nLen + sizeof(EhsLMemQuickItemType)-sizeof(ehs_uint32));
#endif
    EhsTMem_free(pItem);
    EhsTPMutex_unlock(EhsTPMutex_mem);

    //EHSH_LOG_EXIT("EhsHMem_tempFree");
}

/**
 * Free all memory allocated with "small" allocations
 * @param[in] Pointer to the start of the chain of small items
 */
EHS_LOCAL void EhsL_freeAllSmall(EhsLMemSmallItemType** pSmall)
{
    EHSH_LOG_ENTER("EhsL_freeAllSmall(%x)",(unsigned int)pSmall);
    if (*pSmall)
    {
        EhsLMemSmallItemType* pNext; /* next item to free */
        EhsLMemSmallItemType* pCurrent; /* current item to free */

        pNext = (*pSmall);
        *pSmall = NULL;

        for (pCurrent = pNext; pCurrent; pCurrent = pNext)
        {
            EhsL_checkSmall(pCurrent);
            pNext = pCurrent->pNext;
            pCurrent->pNext = NULL;
#ifdef EHS_CONFIG_MEM_CHECK
            EhsLMem.nUsed -= (pCurrent->nLen + sizeof(EhsLMemSmallItemType)-sizeof(ehs_uint32));
#endif
            EhsTMem_free(pCurrent); //@todo Find a way that if an error is found this Free doesn't rune or doesn't crash
            //@todo this lionked list is very prone to crashing the system if any overruns on the heap occur.
            //@this is a pain in the neck ! Don't want crashes!
        }
    }
    EHSH_LOG_EXIT("EhsL_freeAllSmall()");
}

/**
 * Free all memory allocated with "quick" allocations
 */
EHS_LOCAL void EhsL_freeAllQuick(EhsLMemQuickItemType** ppQuick)
{
    EHSH_LOG_ENTER("EhsL_freeAllQuick(%x)",(unsigned int)ppQuick);
    if (*ppQuick)
    {
        EhsLMemQuickItemType* pNext; /* next item to free */
        EhsLMemQuickItemType* pCurrent; /* current item to free */

        pNext = (*ppQuick);
        *ppQuick = NULL;

        for (pCurrent = pNext; pCurrent; pCurrent = pNext)
        {
            EhsL_checkQuick(pCurrent);
            pNext = pCurrent->pNext;
            pCurrent->pNext = NULL;
            pCurrent->pPrev = NULL;
#ifdef EHS_CONFIG_MEM_CHECK
            EhsLMem.nUsed -= (pCurrent->nLen + sizeof(EhsLMemQuickItemType)-sizeof(ehs_uint32));
#endif
            EhsTMem_free(pCurrent);
        }
    }
    EHSH_LOG_EXIT("EhsL_freeAllQuick()");
}

#ifdef EHS_CONFIG_MEM_CHECK
/**
 * Check that the memory item hasn't overrun. Generates an error message
 * if the overrun occurs.
 * @param[in] Pointer to the memory item to check
 */
EHS_LOCAL void EhsL_checkSmall(const EhsLMemSmallItemType* pSmall)
{
    const ehs_uint8* pData = (const ehs_uint8*)&(pSmall->nMem);
    EHSH_LOG_ENTER("EhsL_checkSmall(%x)", (unsigned int)pSmall);
    pData += pSmall->nLen;
    if (pSmall->nPreBlock != EHSL_PRE_PATTERN)
    {
        EHSH_LOG_ERROR(EHS_MSG_ERR_HMPRE_CORRUPT(pSmall));
    }
    if (*(pData++) != EHSL_POST_PATTERN0)
    {
        pData = NULL;
    }
    else if (*(pData++) != EHSL_POST_PATTERN1)
    {
        pData = NULL;
    }
    else if (*(pData++) != EHSL_POST_PATTERN2)
    {
        pData = NULL;
    }
    else if (*(pData++) != EHSL_POST_PATTERN3)
    {
        pData = NULL;
    }
    if (!pData)
    {
        EHSH_LOG_ERROR(EHS_MSG_ERR_HMPOST_CORRUPT(pSmall));
    }
    EHSH_LOG_EXIT("EhsL_checkSmall()");
}

/**
 * Check that the memory item hasn't overrun. Generates an error message
 * if the overrun occurs.
 * @param[in] Pointer to the memory item to check
 */
EHS_LOCAL void EhsL_checkQuick(const EhsLMemQuickItemType* pQuick)
{
    const ehs_uint8* pData = (const ehs_uint8*)&(pQuick->nMem);
    EHSH_LOG_ENTER("EhsL_checkQuick(%x)",(unsigned int)pQuick);
    pData += pQuick->nLen;
    if (pQuick->nPreBlock != EHSL_PRE_PATTERN)
    {
        EHSH_LOG_ERROR(EHS_MSG_ERR_HMPRE_CORRUPT(pQuick));
    }
    if (*(pData++) != EHSL_POST_PATTERN0)
    {
        pData = NULL;
    }
    else if (*(pData++) != EHSL_POST_PATTERN1)
    {
        pData = NULL;
    }
    else if (*(pData++) != EHSL_POST_PATTERN2)
    {
        pData = NULL;
    }
    else if (*(pData++) != EHSL_POST_PATTERN3)
    {
        pData = NULL;
    }
    if (!pData)
    {
        EHSH_LOG_ERROR(EHS_MSG_ERR_HMPOST_CORRUPT(pQuick));
    }
    EHSH_LOG_EXIT("EhsL_checkQuick()");
}

/**
 * Set the memory pattern for a small item - used to check for
 * overruns/underruns
 * @param[in] pSmall Item to set the memory for
 */
EHS_LOCAL void EhsL_setPatternSmall(EhsLMemSmallItemType* pSmall)
{
    ehs_uint8* pData = (ehs_uint8*)&(pSmall->nMem);
    EHSH_LOG_ENTER("EhsL_setPatternSmall(%x)",pSmall);
    pData += pSmall->nLen;
    pSmall->nPreBlock = EHSL_PRE_PATTERN;
    *pData++ = EHSL_POST_PATTERN0;
    *pData++ = EHSL_POST_PATTERN1;
    *pData++ = EHSL_POST_PATTERN2;
    *pData++ = EHSL_POST_PATTERN3;
    EHSH_LOG_EXIT("EhsL_setPatternSmall()");
}

/**
 * Set the memory pattern for a quick item - used to check for
 * overruns/underruns
 * @param[in] pQuick Item to set the memory for
 */
EHS_LOCAL void EhsL_setPatternQuick(EhsLMemQuickItemType* pQuick)
{
    ehs_uint8* pData = (ehs_uint8*)&(pQuick->nMem);
    EHSH_LOG_ENTER("EhsL_setPatternQuick(%x)",pQuick);
    pData += pQuick->nLen;
    pQuick->nPreBlock = EHSL_PRE_PATTERN;
    *pData++ = EHSL_POST_PATTERN0;
    *pData++ = EHSL_POST_PATTERN1;
    *pData++ = EHSL_POST_PATTERN2;
    *pData++ = EHSL_POST_PATTERN3;
    EHSH_LOG_EXIT("EhsL_setPatternQuick()");
}
#endif /* EHS_CONFIG_MEM_CHECK */

/**
 * Switch memory pool. Memory pools are heirarchical in nature,
 * switching back to a "higher" one frees up all of the "lower" ones.
 *
 * WARNING this system is intended for single threaded implementation of the core EHS functions, where all memory permanent
 * memory is allocated at initialisation time before SODL and applications allocate further in the SODL and app (component) phases.
 *
 * devman for example allocated memory in a thread - this must be allocated outside of this state controlled allocator.
 */
void EhsHMem_switchPool(EhsHMPoolType xPool)
{
    EHSH_LOG_ENTER("EhsHMem_switchPool(%d)",xPool);
    EhsTPMutex_lock(EhsTPMutex_mem);
    if (EHSHM_POOL_SYSTEM == xPool)
    {
        EHSH_LOG_ERROR(EHS_MSG_ERR_HMPOOL);
    }
    else
    {
        switch (xPool)
        {
        case EHSHM_POOL_SYSTEM:
            EhsL_freeAllSmall(&EhsLMem.pSystem);
        /* fall through */
        case EHSHM_POOL_APP_SODL:
            EhsL_freeAllSmall(&(EhsLMem.pAppSodl));
            EhsL_freeAllQuick(&(EhsLMem.pTempSodl));
        /* fall through */
        case EHSHM_POOL_APP_DATA:
            EhsL_freeAllSmall(&(EhsLMem.pAppData));
            EhsL_freeAllQuick(&(EhsLMem.pTempApp));
            break;
        }

        EhsLMem.xCurrentPool = xPool;
    }
    EhsTPMutex_unlock(EhsTPMutex_mem);
    EHSH_LOG_EXIT("EhsHMem_switchPool");
}

/**
 * Initialise memory system. Note - can't use the logger at this stage.
 */
void EhsHMem_init(void)
{
    EhsLMemQuickItemType xQuick;
    EhsLMemSmallItemType xSmall;

    EhsLMem.xCurrentPool = EHSHM_POOL_SYSTEM;
    EhsLMem.pAppData = NULL;
    EhsLMem.pAppSodl = NULL;
    EhsLMem.pSystem = NULL;
    EhsLMem.pTempSodl = NULL;
    EhsLMem.pTempApp = NULL;
    // todo2022 check for overflows of uint8 type with this and fail to start
    // cppcheck-suppress comparePointers 
    EhsLMem.nQuickOffset = (ehs_uint8)((void*)&(xQuick.nMem) - (void*)&xQuick);
    
    // cppcheck-suppress comparePointers 
    EhsLMem.nSmallOffset = (ehs_uint8)((void*)&(xSmall.nMem) - (void*)&xSmall);
#ifdef EHS_CONFIG_MEM_CHECK
    EhsLMem.nUsed = 0u;
#endif
}

/**
 * Return memory used so far
 * @return Total memory allocated
 */
ehs_uint32 EhsHMem_used(void)
{
    ehs_uint32 nRet;

    EHSH_LOG_ENTER("EhsHMem_used()");
#ifdef EHS_CONFIG_MEM_CHECK
    nRet = EhsLMem.nUsed;
#else
    nRet = 0u;
#endif

    EHSH_LOG_EXIT("EhsHMem_used = %d",nRet);
    return nRet;
}
