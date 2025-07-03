/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file tgttest_string.c
 * Target tests for string operations
 *
 * @author: inx limited
 */

/**
 * @todo add tests for: EhsIsAlpha, EhsIsAlNum, EhsGetUint32FromString, EhsGetSint32FromString,
 * EhsGetUint16FromString, EhsGetUint8FromString, EhsGetEol
 */
#include "hal_mem.h"
#include "tgttest.h"

/**
 * Defines the global state for EHS.
 */
EHS_GLOBAL EhsKEStateType EhsKEState;

EHS_GLOBAL ehs_uint16 TestsTried;
EHS_GLOBAL ehs_uint16 TestsPassed;

const ehs_char* TgtTestMemory_poolSwitch(void);
const ehs_char* TgtTestMemory_tempAlloc(void);

struct TgtTestSuiteStruct TgtTestSetMemory = {"Memory", 2,
    {
        { "pool switch", TgtTestMemory_poolSwitch},
        { "temp alloc", TgtTestMemory_tempAlloc}
    }
};

/**
 * @page Test_Memory pool_switch
 * @section pool_switch
 * @anchor pool_switch
 * @subsection purpose Purpose:
 * Tests the pool switching behaviour in the memory module
 *
 * @subsection initial Initial conditions:
 * None defined.
 * @subsection data Test data:
 * No specific test data is required
 * @subsection steps Test steps:
 * -# Init memory
 * -# Check used = 0
 * -# Allocate N bytes
 * -# Check used = N
 * -# Switch pool to AppSodl
 * -# Allocate M bytes
 * -# Check used = M+N
 * -# Allocate L bytes
 * -# Check used = M+N+L
 * -# Switch pool to AppData
 * -# Allocate K bytes
 * -# Check used = K+L+M+N
 * -# Switch pool to AppData
 * -# Check used = L+M+M
 * -# Switch pool to AppSodl
 * -# Check used = N
 * @subsection results Expected results:
 * -# Overwrites are detected in each case.
 */
const ehs_char* TgtTestMemory_poolSwitch()
{
    const ehs_char* pRet = NULL;
    void* pMem;
    ehs_uint32 nDelta; /* amount of additional memory per allocation */

    EhsHMem_init();
    if (EhsHMem_used() != 0u)
    {
        pRet = "1. Memory used <> 0";
    }
    if (!pRet)
    {
        pMem = EhsHMem_writeableAlloc(2); /* N =  2 */
        if (!pMem)
        {
            pRet = "2. Malloc failed";
        }
        else if (EhsHMem_used() == 0)
        {
            pRet = "3. Memory used <> N";
        }
        else
        {
            nDelta = EhsHMem_used() - 2;
        }
    }
    if (!pRet)
    {
        EhsHMem_switchPool(EHSHM_POOL_APP_SODL);
        pMem = EhsHMem_writeableAlloc(3); /* M = 3 */
        if (!pMem)
        {
            pRet = "4. Alloc failed";
        }
        else if (EhsHMem_used() != (5+2*nDelta))
        {
            pRet = "5. Memory used <> M + N";
        }
    }
    if (!pRet)
    {
        pMem = EhsHMem_writeableAlloc(5); /* L = 5 */
        if (!pMem)
        {
            pRet = "6. Alloc failed";
        }
        else if (EhsHMem_used() != (10+3*nDelta))
        {
            pRet = "7. Memory used <> L + M + N";
        }
    }
    if (!pRet)
    {
        EhsHMem_switchPool(EHSHM_POOL_APP_DATA);
        pMem = EhsHMem_writeableAlloc(7); /* K = 7 */
        if (!pMem)
        {
            pRet = "8. Alloc failed";
        }
        else if (EhsHMem_used() != (17+4*nDelta))
        {
            pRet = "9. Memory used <> K + L + M + N";
        }
    }
    if (!pRet)
    {
        EhsHMem_switchPool(EHSHM_POOL_APP_DATA);
        if (EhsHMem_used() != (10+3*nDelta))
        {
            pRet = "10. Memory used <> L + M + N";
        }
    }
    if (!pRet)
    {
        EhsHMem_switchPool(EHSHM_POOL_APP_SODL);
        if (EhsHMem_used() != (2+nDelta))
        {
            pRet = "10. Memory used <> N";
        }
    }

    return pRet;
}

/**
 * @page Test_Memory temp_alloc
 * @section temp_alloc
 * @anchor temp_alloc
 * @subsection purpose Purpose:
 * Tests that temporary allocation/deallocation works correctly.
 *
 * @subsection initial Initial conditions:
 * None defined.
 * @subsection data Test data:
 * No specific test data is required
 * @subsection steps Test steps:
 * -# Init memory
 * -# Allocate N temp bytes
 * -# Check used > 0 (also determine delta as used - N)
 * -# Allocate M temp bytes
 * -# Check used = N+M+2delta
 * -# Allocate L temp bytes
 * -# Check used = L+M+N+3delta
 * -# Allocate K temp bytes
 * -# Check used =K+L+M+N+4delta
 * -# deallocate M (middle item)
 * -# Check used = K+L+N+3delta
 * -# deallocate N (first allocated)
 * -# check used = K+L+2delta
 * -# deallocate K (last allocated)
 * -# check used = L+delta
 * -# allocate J temp bytes
 * -# check used = J+L+2delta
 * -# Switch pool Appdata
 * -# Check used = 0
 * @subsection results Expected results:
 * -# Test performs as specified above.
 */
const ehs_char* TgtTestMemory_tempAlloc()
{
    const ehs_char* pRet = NULL;
    void* J,*K,*L,*M,*N;
    ehs_uint32 nDelta; /* amount of additional memory per allocation */

    EhsHMem_init();
    if (EhsHMem_used() != 0u)
    {
        pRet = "1. Memory used <> 0";
    }
    if (!pRet)
    {
        N = EhsHMem_tempAlloc(2); /* N =  2 */
        if (!N)
        {
            pRet = "2. Malloc failed";
        }
        else if (EhsHMem_used() == 0)
        {
            pRet = "3. Memory used <> N";
        }
        else
        {
            nDelta = EhsHMem_used() - 2;
        }
    }

    // * -# Allocate M temp bytes
    // * -# Check used = N+M+2delta
    if (!pRet)
    {
        M = EhsHMem_tempAlloc(3); /* M =  3 */
        if (!M)
        {
            pRet = "4. Malloc failed";
        }
        else if (EhsHMem_used() != (5+2*nDelta))
        {
            pRet = "5. Memory used <> M + N";
        }
    }
    // * -# Allocate L temp bytes
    // * -# Check used = L+M+N+3delta
    if (!pRet)
    {
        L = EhsHMem_tempAlloc(5); /* L = 5 */
        if (!L)
        {
            pRet = "6. Malloc failed";
        }
        else if (EhsHMem_used() != (10+3*nDelta))
        {
            pRet = "7. Memory used <> L + M + N";
        }
    }
    // * -# Allocate K temp bytes
    // * -# Check used =K+L+M+N+4delta
    if (!pRet)
    {
        K = EhsHMem_tempAlloc(7); /* K = 7 */
        if (!K)
        {
            pRet = "8. Malloc failed";
        }
        else if (EhsHMem_used() != (17+4*nDelta))
        {
            pRet = "9. Memory used <> K + L + M + N";
        }
    }
    // * -# deallocate M (middle item)
    // * -# Check used = K+L+N+3delta
    if (!pRet)
    {
        EhsHMem_tempFree(M);
        if (EhsHMem_used() != 14+3*nDelta)
        {
            pRet = "10. Memory used <> K + L + N";
        }
    }
    // * -# deallocate N (first allocated)
    // * -# check used = K+L+2delta
    if (!pRet)
    {
        EhsHMem_tempFree(N);
        if (EhsHMem_used() != 12+2*nDelta)
        {
            pRet = "11. Memory used <> K + L";
        }
    }
    // * -# deallocate K (last allocated)
    // * -# check used = L+delta
    if (!pRet)
    {
        EhsHMem_tempFree(K);
        if (EhsHMem_used() != 5+nDelta)
        {
            pRet = "12. Memory used <>  L";
        }
    }
    // * -# allocate J temp bytes
    // * -# check used = J+L+2delta
    if (!pRet)
    {
        K = EhsHMem_tempAlloc(11); /* J = 11 */
        if (!K)
        {
            pRet = "13. Malloc failed";
        }
        else if (EhsHMem_used() != (16+2*nDelta))
        {
            pRet = "14. Memory used <> J + L";
        }
    }
    // * -# Switch pool Appdata
    // * -# Check used = 0
    if (!pRet)
    {
        EhsHMem_switchPool(EHSHM_POOL_APP_SODL);
        if (EhsHMem_used() != 0)
        {
            pRet = "15. Memory used <> 0";
        }
    }

    return pRet;
}
