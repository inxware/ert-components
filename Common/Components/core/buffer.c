/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/* buffer.c
 *
 * src file for fifo functions. Any function appearing in
 * this file must also be listed in objRefTable.c for it to be included in the
 * list of functions available to the EHS.
 *
 * For definition of arguments in Ncapsa functions (Identify_, Init_ and Run_)
 * please see types.h.
 *
*/
#include "globals.h"
#include "buffer.h"
#include "hal_mem.h"

#include "setCompletes.h"

#include "hal-api.h" /* Required for logging */




typedef struct			/* state data for integer FIFO buffer...*/
{
    int nPushIndex;   	/* index of next push point*/
    int nPopIndex;    	/* index of next pop point */
    int nCurrentUsage;	/* current usage / size    */
    int nSize;		  	/* actual size             */
    int* pContents;   	/* pointer to contents     */
} structFIFO_Int;

typedef struct			/* state data for real FIFO buffer...*/
{
    int nPushIndex;   	/* index of next push point*/
    int nPopIndex;    	/* index of next pop point */
    int nCurrentUsage;	/* current usage / size    */
    int nSize;		  	/* actual size             */
    ehs_float* pContents;  /* pointer to contents     */
} structFIFO_Real;

typedef struct			/* state data for bool FIFO buffer...*/
{
    int nPushIndex;   	/* index of next push point*/
    int nPopIndex;    	/* index of next pop point */
    int nCurrentUsage;	/* current usage / size    */
    int nSize;		  	/* actual size             */
    int* pContents;     /* pointer to contents     */
} structFIFO_Bool;

typedef struct			/* state data for string FIFO buffer...*/
{
    int nPushIndex;   	/* index of next push point*/
    int nPopIndex;    	/* index of next pop point */
    int nCurrentUsage;	/* current usage / size    */
    int nSize;		  	/* actual size             */
    char** pContents;   /* pointer to contents     */
} structFIFO_String;

typedef struct			/* state data for integer LIFO buffer...*/
{
    int nIndex;			/* index of next push point*/
    int nCurrentUsage;	/* current usage / size    */
    int nSize;		  	/* actual size             */
    int* pContents;   	/* pointer to contents     */
} structLIFO_Int;

typedef struct			/* state data for real LIFO buffer...*/
{
    int nIndex;			/* index of next push point*/
    int nCurrentUsage;	/* current usage / size    */
    int nSize;		  	/* actual size             */
    ehs_float* pContents;   	/* pointer to contents     */
} structLIFO_Real;

typedef struct			/* state data for boolean LIFO buffer...*/
{
    int nIndex;			/* index of next push point*/
    int nCurrentUsage;	/* current usage / size    */
    int nSize;		  	/* actual size             */
    int* pContents;   	/* pointer to contents     */
} structLIFO_Bool;

typedef struct			/* state data for string LIFO buffer...*/
{
    int nIndex;   	 	/* index of next push point */
    int nCurrentUsage;	/* current usage / size    */
    int nSize;		  	/* actual size             */
    char** pContents;   /* pointer to contents     */
} structLIFO_String;




/******************************************************************************/
/* Define FIFO Integer function block */

EHS_FB_FUNCTIONS_START(FIFOInt)

EHS_FB_FUNCTION_ENTRY("Run_PushFIFOInt", 0x01, PushFIFOInt)

EHS_FB_FUNCTION_ENTRY("Run_PopFIFOInt", 0x02, PopFIFOInt)

EHS_FB_FUNCTION_ENTRY("Run_FlushFIFOInt", 0x03, FlushAnyBuffer)
EHS_FB_FUNCTIONS_END

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(FIFOInt)
{
    /* basically we need to malloc the size of the FIFO struct * number required...*/
    EHS_FB_IDENTIFY_MEMORY = sizeof(structFIFO_Int);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * @todo remove exit from this function
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(FIFOInt)
{
    int nSize;
    int* pContents = NULL;
    structFIFO_Int* pFIFO_Int = NULL;

    nSize = atoi(EHS_FB_INIT_PARAMETERS);
    /* malloc some space for the buffer from the pParams description...*/
    pContents = (int*)EhsHMem_writeableAlloc(nSize*sizeof(int));
    if( !pContents )
    {
        return EHS_TRUE;
    }

    pFIFO_Int = (structFIFO_Int*)EHS_FB_INIT_CONTEXT;

    pFIFO_Int->nPushIndex = 0;
    pFIFO_Int->nPopIndex = 0;
    pFIFO_Int->nCurrentUsage = 0;
    pFIFO_Int->nSize = nSize;
    pFIFO_Int->pContents = pContents;

    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FlushAnyBuffer)
{
    //@todo - note that we are using FIFO Int struct for all the buffers - should create one generic struct for all types and extend it for different data types
    structFIFO_Int* pFIFO_Int = (structFIFO_Int*)EHS_FB_RUN_CONTEXT;
    pFIFO_Int->nPushIndex = 0;
    pFIFO_Int->nPopIndex = 0;
    pFIFO_Int->nCurrentUsage = 0;
    SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    return;
}


/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PushFIFOInt)
{
    structFIFO_Int* pFIFO_Int = NULL;

    int nPushIndex;
    int nCurrentUsage;
    int nSize;
    int* pContents = NULL;

    /* the dereference of pByte gives the location of the state data...*/
    pFIFO_Int = (structFIFO_Int*)EHS_FB_RUN_CONTEXT;
    /* copy the required parts to local storage...*/
    nPushIndex = pFIFO_Int->nPushIndex;
    nCurrentUsage = pFIFO_Int->nCurrentUsage;
    nSize = pFIFO_Int->nSize;
    pContents = (int*)pFIFO_Int->pContents;

    /* check we haven't walked over the top of the buffer...*/
    if (nCurrentUsage < nSize)
    {
        /* push input on to the buffer...*/
        pContents[nPushIndex] = NCAPSA_nIn(0);
        /* increment the push and current indicies...*/
        nPushIndex++;
        if (nPushIndex == nSize)
        {
            nPushIndex = 0;
        }
        nCurrentUsage++;
        /* reset any error condition...*/
//		NCAPSA_nOut(0) = ERR_NO_ERROR;
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    else
    {
        /* buffer is full, set buffer overrun error...*/
//		NCAPSA_nOut(0) = ERR_PUSH_ERROR;
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    /* copy state data back over...*/
    pFIFO_Int->nCurrentUsage = nCurrentUsage;
    pFIFO_Int->nPushIndex = nPushIndex;

    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PopFIFOInt)
{
    structFIFO_Int* pFIFO_Int = NULL;

    int nPopIndex;
    int nCurrentUsage;
    int nSize;
    int* pContents = NULL;

//	ehs_uint8* pB = NULL;

    /* the dereference of EHS_FB_RUN_CONTEXT gives the location of the state data...*/
    pFIFO_Int = (structFIFO_Int*)EHS_FB_RUN_CONTEXT;

    nPopIndex = pFIFO_Int->nPopIndex;
    nCurrentUsage = pFIFO_Int->nCurrentUsage;
    nSize = pFIFO_Int->nSize;
    pContents = (int*)pFIFO_Int->pContents;

    if (nCurrentUsage == 0)
    {
        /* we can't pop an empty buffer, set underrun error ...*/
//		NCAPSA_nOut(1) = ERR_POP_ERROR;
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    else
    {
        //pB = NCAPSA_pOut(0);
        /* pop the buffer to the required output location...*/
        NCAPSA_nOut(0) = pContents[nPopIndex];
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
        /* increment pop index for next pop...*/
        nPopIndex++;
        if (nPopIndex == nSize)
        {
            /* if we've gone over the edge - restart at the beginning...*/
            nPopIndex = 0;
        }
        /* decrement usage indicator...*/
        nCurrentUsage--;
        /* reset any error condition...*/
//		NCAPSA_nOut(1) = ERR_NO_ERROR;
    }
    /* copy the data back into the object state memory...*/
    pFIFO_Int->nPopIndex = nPopIndex;
    pFIFO_Int->nCurrentUsage = nCurrentUsage;

    return;
}

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define FIFO Real function block */

EHS_FB_FUNCTIONS_START(FIFOReal)

EHS_FB_FUNCTION_ENTRY("Run_PushFIFOReal", 0x01, PushFIFOReal)

EHS_FB_FUNCTION_ENTRY("Run_PopFIFOReal", 0x02, PopFIFOReal)

EHS_FB_FUNCTION_ENTRY("Run_FlushFIFOReal", 0x03, FlushAnyBuffer)
EHS_FB_FUNCTIONS_END

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(FIFOReal)
{
    /* basically we need to malloc the size of the FIFO struct * number required...*/
    EHS_FB_IDENTIFY_MEMORY = sizeof(structFIFO_Real);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * @todo remove malloc from this function
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(FIFOReal)
{
    int nSize;
    ehs_float* pContents = NULL;
    structFIFO_Real* pFIFO_Real = NULL;

    nSize = atoi(EHS_FB_INIT_PARAMETERS);
    /* Allocate some space for the buffer from the pParams description...*/
    pContents = (ehs_float*)EhsHMem_writeableAlloc(nSize*sizeof(ehs_float));
    if( !pContents )
    {

        return EHS_TRUE;
    }

    pFIFO_Real = (structFIFO_Real*)EHS_FB_INIT_CONTEXT;

    pFIFO_Real->nPushIndex = 0;
    pFIFO_Real->nPopIndex = 0;
    pFIFO_Real->nCurrentUsage = 0;
    pFIFO_Real->nSize = nSize;
    pFIFO_Real->pContents = pContents;

    return EHS_TRUE; /* initialisation always succeeds */;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
//Use the commong flusher

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PushFIFOReal)
{
    structFIFO_Real* pFIFO_Real = NULL;

    int nPushIndex;
    int nCurrentUsage;
    int nSize;
    ehs_float* pContents = NULL;

    /* the dereference of pByte gives the location of the state data...*/
    pFIFO_Real = (structFIFO_Real*)EHS_FB_RUN_CONTEXT;
    /* copy the required parts to local storage...*/
    nPushIndex = pFIFO_Real->nPushIndex;
    nCurrentUsage = pFIFO_Real->nCurrentUsage;
    nSize = pFIFO_Real->nSize;
    pContents = (ehs_float*)pFIFO_Real->pContents;

    /* check we haven't walked over the top of the buffer...*/
    if (nCurrentUsage < nSize)
    {
        /* push input on to the buffer...*/
        pContents[nPushIndex] = NCAPSA_dIn(0);
        /* increment the push and current indicies...*/
        nPushIndex++;
        if (nPushIndex == nSize)
        {
            nPushIndex = 0;
        }
        nCurrentUsage++;
        /* reset any error condition...*/
//		NCAPSA_nOut(0) = ERR_NO_ERROR;
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    else
    {
        /* buffer is full, set buffer overrun error...*/
//		NCAPSA_nOut(0) = ERR_PUSH_ERROR;
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    /* copy state data back over...*/
    pFIFO_Real->nCurrentUsage = nCurrentUsage;
    pFIFO_Real->nPushIndex = nPushIndex;


    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PopFIFOReal)
{
    structFIFO_Real* pFIFO_Real = NULL;

    int nPopIndex;
    int nCurrentUsage;
    int nSize;
    ehs_float* pContents = NULL;

    //ehs_uint8* pB = NULL;

    /* the dereference of pByte gives the location of the state data...*/
    pFIFO_Real = (structFIFO_Real*)EHS_FB_RUN_CONTEXT;

    nPopIndex = pFIFO_Real->nPopIndex;
    nCurrentUsage = pFIFO_Real->nCurrentUsage;
    nSize = pFIFO_Real->nSize;
    pContents = (ehs_float*)pFIFO_Real->pContents;

    if (nCurrentUsage == 0)
    {
        /* we can't pop an empty buffer, set underrun error ...*/
//		NCAPSA_nOut(1) = ERR_POP_ERROR;
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    else
    {
        //pB = NCAPSA_pOut(0);
        /* pop the buffer to the required output location...*/
        NCAPSA_dOut(0) = pContents[nPopIndex];
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
        /* increment pop index for next pop...*/
        nPopIndex++;
        if (nPopIndex == nSize)
        {
            /* if we've gone over the edge - restart at the beginning...*/
            nPopIndex = 0;
        }
        /* decrement usage indicator...*/
        nCurrentUsage--;
        /* reset any error condition...*/
//		NCAPSA_nOut(1) = ERR_NO_ERROR;
    }
    /* copy the data back into the object state memory...*/
    pFIFO_Real->nPopIndex = nPopIndex;
    pFIFO_Real->nCurrentUsage = nCurrentUsage;

    return;
}

#endif

/******************************************************************************/
/* Define FIFO Boolean function block */

EHS_FB_FUNCTIONS_START(FIFOBool)

EHS_FB_FUNCTION_ENTRY("Run_PushFIFOBool", 0x01, PushFIFOBool)

EHS_FB_FUNCTION_ENTRY("Run_PopFIFOBool", 0x02, PopFIFOBool)

EHS_FB_FUNCTION_ENTRY("Run_FlushFIFOBool", 0x03, FlushAnyBuffer)
EHS_FB_FUNCTIONS_END

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(FIFOBool)
{
    /* basically we need to malloc the size of the FIFO struct * number required...*/
    EHS_FB_IDENTIFY_MEMORY = sizeof(structFIFO_Bool);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * @todo remove malloc from this function
 * @todo Remove EhsExit
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(FIFOBool)
{
    int nSize;
    int* pContents = NULL;
    structFIFO_Bool* pFIFO_Bool = NULL;

    nSize = atoi(EHS_FB_INIT_PARAMETERS);
    /* malloc some space for the buffer from the pParams description...*/
    pContents = (int*)EhsHMem_writeableAlloc(nSize*sizeof(int));
    if( !pContents )
    {
        return EHS_TRUE;
    }


    pFIFO_Bool = (structFIFO_Bool*)EHS_FB_INIT_CONTEXT;

    pFIFO_Bool->nPushIndex = 0;
    pFIFO_Bool->nPopIndex = 0;
    pFIFO_Bool->nCurrentUsage = 0;
    pFIFO_Bool->nSize = nSize;
    pFIFO_Bool->pContents = pContents;

    return EHS_TRUE; /* initialisation always succeeds */
}



/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PushFIFOBool)
{
    structFIFO_Bool* pFIFO_Bool = NULL;

    int nPushIndex;
    int nCurrentUsage;
    int nSize;
    int* pContents = NULL;

    /* the dereference of pByte gives the location of the state data...*/
    pFIFO_Bool = (structFIFO_Bool*)EHS_FB_RUN_CONTEXT;
    /* copy the required parts to local storage...*/
    nPushIndex = pFIFO_Bool->nPushIndex;
    nCurrentUsage = pFIFO_Bool->nCurrentUsage;
    nSize = pFIFO_Bool->nSize;
    pContents = (int*)pFIFO_Bool->pContents;

    /* check we haven't walked over the top of the buffer...*/
    if (nCurrentUsage < nSize)
    {
        /* push input on to the buffer...*/
        pContents[nPushIndex] = NCAPSA_bIn(0);
        /* increment the push and current indicies...*/
        nPushIndex++;
        if (nPushIndex == nSize)
        {
            nPushIndex = 0;
        }
        nCurrentUsage++;
        /* reset any error condition...*/
//		NCAPSA_nOut(0) = ERR_NO_ERROR;
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    else
    {
        /* buffer is full, set buffer overrun error...*/
//		NCAPSA_nOut(0) = ERR_PUSH_ERROR;
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    /* copy state data back over...*/
    pFIFO_Bool->nCurrentUsage = nCurrentUsage;
    pFIFO_Bool->nPushIndex = nPushIndex;


    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PopFIFOBool)
{
    structFIFO_Bool* pFIFO_Bool = NULL;

    int nPopIndex;
    int nCurrentUsage;
    int nSize;
    int* pContents = NULL;

    //ehs_uint8* pB = NULL;

    /* the dereference of pByte gives the location of the state data...*/
    pFIFO_Bool = (structFIFO_Bool*)EHS_FB_RUN_CONTEXT;

    nPopIndex = pFIFO_Bool->nPopIndex;
    nCurrentUsage = pFIFO_Bool->nCurrentUsage;
    nSize = pFIFO_Bool->nSize;
    pContents = (int*)pFIFO_Bool->pContents;

    if (nCurrentUsage == 0)
    {
        /* we can't pop an empty buffer, set underrun error ...*/
//		NCAPSA_nOut(1) = ERR_POP_ERROR;
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    else
    {
        //pB = NCAPSA_pOut(0);
        /* pop the buffer to the required output location...*/
        NCAPSA_bOut(0) = (ehs_bool)(0 != pContents[nPopIndex]);
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
        /* increment pop index for next pop...*/
        nPopIndex++;
        if (nPopIndex == nSize)
        {
            /* if we've gone over the edge - restart at the beginning...*/
            nPopIndex = 0;
        }
        /* decrement usage indicator...*/
        nCurrentUsage--;
        /* reset any error condition...*/
//		NCAPSA_nOut(1) = ERR_NO_ERROR;
    }
    /* copy the data back into the object state memory...*/
    pFIFO_Bool->nPopIndex = nPopIndex;
    pFIFO_Bool->nCurrentUsage = nCurrentUsage;

    return;
}


/******************************************************************************/
/* Define FIFO String function block */

EHS_FB_FUNCTIONS_START(FIFOString)

EHS_FB_FUNCTION_ENTRY("Run_PushFIFOString", 0x01, PushFIFOString)

EHS_FB_FUNCTION_ENTRY("Run_PopFIFOString", 0x02, PopFIFOString)

EHS_FB_FUNCTION_ENTRY("Run_FlushFIFOString", 0x03, FlushAnyBuffer)
EHS_FB_FUNCTIONS_END

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(FIFOString)
{
    /* basically we need to malloc the size of the FIFO struct * number required...*/
    EHS_FB_IDENTIFY_MEMORY = sizeof(structFIFO_String);
    return;
}




/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * @todo remove malloc from this function
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(FIFOString)
{
    int nSize;
    int i;
    char** pContents = NULL;
    structFIFO_String* pFIFO_String = NULL;

    nSize = atoi(EHS_FB_INIT_PARAMETERS);
    /* malloc some space for the buffer from the pParams description...*/
    pContents = (char**)EhsHMem_writeableAlloc(nSize*sizeof(char**));
    if( !(pContents) )
    {
        return EHS_TRUE;
    }

    for (i=0; i<nSize; i++)
    {
        pContents[i] = (char*)EhsHMem_writeableAlloc(EHS_STRING_LENGTH_MAX); //TODO:STRINGLENGTH!
        if( !(pContents[i]) )
        {
            return EHS_TRUE;
        }

    }

    pFIFO_String = (structFIFO_String*)EHS_FB_INIT_CONTEXT;

    pFIFO_String->nPushIndex = 0;
    pFIFO_String->nPopIndex = 0;
    pFIFO_String->nCurrentUsage = 0;
    pFIFO_String->nSize = nSize;
    pFIFO_String->pContents = pContents;

    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PushFIFOString)
{
    structFIFO_String* pFIFO_String = NULL;

    int nPushIndex;
    int nCurrentUsage;
    int nSize;
    char** pContents = NULL;

    /* the dereference of pByte gives the location of the state data...*/
    pFIFO_String = (structFIFO_String*)EHS_FB_RUN_CONTEXT;
    /* copy the required parts to local storage...*/
    nPushIndex = pFIFO_String->nPushIndex;
    nCurrentUsage = pFIFO_String->nCurrentUsage;
    nSize = pFIFO_String->nSize;
    pContents = (char**)pFIFO_String->pContents;

    /* check we haven't walked over the top of the buffer...*/
    if (nCurrentUsage < nSize)
    {
        /* push input on to the buffer...*/
        EhsStrcpy(pContents[nPushIndex],NCAPSA_szIn(0));
        /* increment the push and current indicies...*/
        nPushIndex++;
        if (nPushIndex == nSize)
        {
            nPushIndex = 0;
        }
        nCurrentUsage++;
        /* reset any error condition...*/
//		NCAPSA_nOut(0) = ERR_NO_ERROR;
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    else
    {
        /* buffer is full, set buffer overrun error...*/
//		NCAPSA_nOut(0) = ERR_PUSH_ERROR;
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    /* copy state data back over...*/
    pFIFO_String->nCurrentUsage = nCurrentUsage;
    pFIFO_String->nPushIndex = nPushIndex;

    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PopFIFOString)
{
    structFIFO_String* pFIFO_String = NULL;

    int nPopIndex;
    int nCurrentUsage;
    int nSize;
    char** pContents = NULL;

    //ehs_uint8* pB = NULL;

    /* the dereference of pByte gives the location of the state data...*/
    pFIFO_String = (structFIFO_String*)EHS_FB_RUN_CONTEXT;

    nPopIndex = pFIFO_String->nPopIndex;
    nCurrentUsage = pFIFO_String->nCurrentUsage;
    nSize = pFIFO_String->nSize;
    pContents = (char**)pFIFO_String->pContents;

    if (nCurrentUsage == 0)
    {
        /* we can't pop an empty buffer, set underrun error ...*/
//		NCAPSA_nOut(1) = ERR_POP_ERROR;
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    else
    {
        //pB = NCAPSA_pOut(0);
        /* pop the buffer to the required output location...*/
        strcpy(NCAPSA_szOut(0),pContents[nPopIndex]);
        /* increment pop index for next pop...*/
        nPopIndex++;
        if (nPopIndex == nSize)
        {
            /* if we've gone over the edge - restart at the beginning...*/
            nPopIndex = 0;
        }
        /* decrement usage indicator...*/
        nCurrentUsage--;
        /* reset any error condition...*/
//		NCAPSA_nOut(1) = ERR_NO_ERROR;
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    /* copy the data back into the object state memory...*/
    pFIFO_String->nPopIndex = nPopIndex;
    pFIFO_String->nCurrentUsage = nCurrentUsage;
    return;
}


/******************************************************************************/
/* Define LIFO Integer function block */

EHS_FB_FUNCTIONS_START(LIFOInt)

EHS_FB_FUNCTION_ENTRY("Run_PushLIFOInt", 0x01, PushLIFOInt)

EHS_FB_FUNCTION_ENTRY("Run_PopLIFOInt", 0x02, PopLIFOInt)

EHS_FB_FUNCTION_ENTRY("Run_FlushLIFOInt", 0x03, FlushAnyLIFOBuffer)
EHS_FB_FUNCTIONS_END

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(LIFOInt)
{
    /* basically we need to malloc the size of the LIFO struct * number required...*/
    EHS_FB_IDENTIFY_MEMORY = sizeof(structLIFO_Int);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(LIFOInt)
{
    int nSize;
    int* pContents = NULL;
    structLIFO_Int* pLIFO_Int = NULL;

    nSize = atoi(EHS_FB_INIT_PARAMETERS);
    /* malloc some space for the buffer from the pParams description...*/
    pContents = (int*)EhsHMem_writeableAlloc(nSize*sizeof(int));
    if( !(pContents) )
    {
        return EHS_TRUE;
    }

    pLIFO_Int = (structLIFO_Int*)EHS_FB_INIT_CONTEXT;

    pLIFO_Int->nIndex = 0;
    pLIFO_Int->nCurrentUsage = 0;
    pLIFO_Int->nSize = nSize;
    pLIFO_Int->pContents = pContents;

    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FlushAnyLIFOBuffer)
{
    //@todo - note that we are using LIFO Int struct for all the buffers - should create one generic struct for all types and extend it for different data types
    structLIFO_Int* pLIFO_Int = (structLIFO_Int*)EHS_FB_RUN_CONTEXT;
    pLIFO_Int->nIndex = 0;
    pLIFO_Int->nCurrentUsage = 0;
    SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PushLIFOInt)
{
    structLIFO_Int* pLIFO_Int = NULL;

    int nIndex;
    int nCurrentUsage;
    int nSize;
    int* pContents = NULL;

    /* the dereference of pByte gives the location of the state data...*/
    pLIFO_Int = (structLIFO_Int*)EHS_FB_RUN_CONTEXT;
    /* copy the required parts to local storage...*/
    nIndex = pLIFO_Int->nIndex;
    nCurrentUsage = pLIFO_Int->nCurrentUsage;
    nSize = pLIFO_Int->nSize;
    pContents = (int*)pLIFO_Int->pContents;

    /* check we haven't walked over the top of the buffer...*/
    if (nCurrentUsage < nSize)
    {
        /* push input on to the buffer...*/
        pContents[nIndex] = NCAPSA_nIn(0);
        /* increment the push and current indicies...*/
        nIndex++;
        nCurrentUsage++;
        /* reset any error condition...*/
//		NCAPSA_nOut(0) = ERR_NO_ERROR;
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    else
    {
        /* buffer is full, set buffer overrun error...*/
//		NCAPSA_nOut(0) = ERR_PUSH_ERROR;
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    /* copy state data back over...*/
    pLIFO_Int->nCurrentUsage = nCurrentUsage;
    pLIFO_Int->nIndex = nIndex;


    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PopLIFOInt)
{
    structLIFO_Int* pLIFO_Int = NULL;

    int nIndex;
    int nCurrentUsage;
    //int nSize;
    int* pContents = NULL;

    //ehs_uint8* pB = NULL;

    /* the dereference of pByte gives the location of the state data...*/
    pLIFO_Int = (structLIFO_Int*)EHS_FB_RUN_CONTEXT;

    nIndex = pLIFO_Int->nIndex;
    nCurrentUsage = pLIFO_Int->nCurrentUsage;
    //nSize = pLIFO_Int->nSize;
    pContents = (int*)pLIFO_Int->pContents;

    if (nCurrentUsage == 0)
    {
        /* we can't pop an empty buffer, set underrun error ...*/
//		NCAPSA_nOut(1) = ERR_POP_ERROR;
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    else
    {
        //	pB = NCAPSA_pOut(0);
        /* pop the buffer to the required output location...*/
        NCAPSA_nOut(0) = pContents[nIndex - 1];
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
        /* decrement index for next pop...*/
        nIndex--;
        /* decrement usage indicator...*/
        nCurrentUsage--;
        /* reset any error condition...*/
//		NCAPSA_nOut(1) = ERR_NO_ERROR;
    }
    /* copy the data back into the object state memory...*/
    pLIFO_Int->nIndex = nIndex;
    pLIFO_Int->nCurrentUsage = nCurrentUsage;

    return;
}

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define LIFO Float function block */

EHS_FB_FUNCTIONS_START(LIFOReal)

EHS_FB_FUNCTION_ENTRY("Run_PushLIFOReal", 0x01, PushLIFOReal)

EHS_FB_FUNCTION_ENTRY("Run_PopLIFOReal", 0x02, PopLIFOReal)

EHS_FB_FUNCTION_ENTRY("Run_FlushLIFOReal", 0x03, FlushAnyLIFOBuffer)
EHS_FB_FUNCTIONS_END

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(LIFOReal)
{
    /* basically we need to malloc the size of the LIFO struct * number required...*/
    EHS_FB_IDENTIFY_MEMORY = sizeof(structLIFO_Real);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(LIFOReal)
{
    int nSize;
    ehs_float* pContents = NULL;
    structLIFO_Real* pLIFO_Real = NULL;

    nSize = atoi(EHS_FB_INIT_PARAMETERS);
    /* malloc some space for the buffer from the pParams description...*/
    pContents = (ehs_float*)EhsHMem_writeableAlloc(nSize*sizeof(ehs_float));
    if( !(pContents) )
    {
        return EHS_TRUE;
    }
    pLIFO_Real = (structLIFO_Real*)EHS_FB_INIT_CONTEXT;

    pLIFO_Real->nIndex = 0;
    pLIFO_Real->nCurrentUsage = 0;
    pLIFO_Real->nSize = nSize;
    pLIFO_Real->pContents = pContents;

    return EHS_TRUE; /* initialisation always succeeds */;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PushLIFOReal)
{
    structLIFO_Real* pLIFO_Real = NULL;

    int nIndex;
    int nCurrentUsage;
    int nSize;
    ehs_float* pContents = NULL;

    /* the dereference of &EHS_FB_RUN_CONTEXT gives the location of the state data...*/
    pLIFO_Real = (structLIFO_Real*)EHS_FB_RUN_CONTEXT;
    /* copy the required parts to local storage...*/
    nIndex = pLIFO_Real->nIndex;
    nCurrentUsage = pLIFO_Real->nCurrentUsage;
    nSize = pLIFO_Real->nSize;
    pContents = (ehs_float*)pLIFO_Real->pContents;

    /* check we haven't walked over the top of the buffer...*/
    if (nCurrentUsage < nSize)
    {
        /* push input on to the buffer...*/
        pContents[nIndex] = NCAPSA_dIn(0);
        /* increment the push and current indicies...*/
        nIndex++;
        nCurrentUsage++;
        /* reset any error condition...*/
//		NCAPSA_nOut(0) = ERR_NO_ERROR;
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    else
    {
        /* buffer is full, set buffer overrun error...*/
//		NCAPSA_nOut(0) = ERR_PUSH_ERROR;
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    /* copy state data back over...*/
    pLIFO_Real->nCurrentUsage = nCurrentUsage;
    pLIFO_Real->nIndex = nIndex;


    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PopLIFOReal)
{
    structLIFO_Real* pLIFO_Real = NULL;

    int nIndex;
    int nCurrentUsage;
    //int nSize;
    ehs_float* pContents = NULL;

    //ehs_uint8* pB = NULL;

    /* the dereference of pByte gives the location of the state data...*/
    pLIFO_Real = (structLIFO_Real*)EHS_FB_RUN_CONTEXT;

    nIndex = pLIFO_Real->nIndex;
    nCurrentUsage = pLIFO_Real->nCurrentUsage;
    //nSize = pLIFO_Real->nSize;
    pContents = (ehs_float*)pLIFO_Real->pContents;

    if (nCurrentUsage == 0)
    {
        /* we can't pop an empty buffer, set underrun error ...*/
//		NCAPSA_nOut(1) = ERR_POP_ERROR;
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    else
    {
        //	pB = NCAPSA_pOut(0);
        /* pop the buffer to the required output location...*/
        NCAPSA_dOut(0) = pContents[nIndex - 1];
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
        /* decrement index for next pop...*/
        nIndex--;
        /* decrement usage indicator...*/
        nCurrentUsage--;
        /* reset any error condition...*/
//		NCAPSA_nOut(1) = ERR_NO_ERROR;
    }
    /* copy the data back into the object state memory...*/
    pLIFO_Real->nIndex = nIndex;
    pLIFO_Real->nCurrentUsage = nCurrentUsage;

    return;
}
#endif /* EHS_TARGET_FP_SUPPORT */
/******************************************************************************/
/* Define FIFO Integer function block */

EHS_FB_FUNCTIONS_START(LIFOBool)

EHS_FB_FUNCTION_ENTRY("Run_PushLIFOBool", 0x01, PushLIFOBool)

EHS_FB_FUNCTION_ENTRY("Run_PopLIFOBool", 0x02, PopLIFOBool)

EHS_FB_FUNCTION_ENTRY("Run_FlushLIFOBool", 0x03, FlushAnyLIFOBuffer)
EHS_FB_FUNCTIONS_END

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(LIFOBool)
{
    /* basically we need to malloc the size of the LIFO struct * number required...*/
    EHS_FB_IDENTIFY_MEMORY = sizeof(structLIFO_Bool);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(LIFOBool)
{
    int nSize;
    int* pContents = NULL;
    structLIFO_Bool* pLIFO_Bool = NULL;

    nSize = atoi(EHS_FB_INIT_PARAMETERS);
    /* malloc some space for the buffer from the pParams description...*/
    pContents = (int*)EhsHMem_writeableAlloc(nSize*sizeof(int));
    if( !(pContents) )
    {
        return EHS_TRUE;
    }

    pLIFO_Bool = (structLIFO_Bool*)EHS_FB_INIT_CONTEXT;

    pLIFO_Bool->nIndex = 0;
    pLIFO_Bool->nCurrentUsage = 0;
    pLIFO_Bool->nSize = nSize;
    pLIFO_Bool->pContents = pContents;

    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PushLIFOBool)
{
    structLIFO_Bool* pLIFO_Bool = NULL;

    int nIndex;
    int nCurrentUsage;
    int nSize;
    int* pContents = NULL;

    /* the dereference of pByte gives the location of the state data...*/
    pLIFO_Bool = (structLIFO_Bool*)EHS_FB_RUN_CONTEXT;
    /* copy the required parts to local storage...*/
    nIndex = pLIFO_Bool->nIndex;
    nCurrentUsage = pLIFO_Bool->nCurrentUsage;
    nSize = pLIFO_Bool->nSize;
    pContents = (int*)pLIFO_Bool->pContents;

    /* check we haven't walked over the top of the buffer...*/
    if (nCurrentUsage < nSize)
    {
        /* push input on to the buffer...*/
        pContents[nIndex] = NCAPSA_bIn(0);
        /* increment the push and current indicies...*/
        nIndex++;
        nCurrentUsage++;
        /* reset any error condition...*/
//		NCAPSA_nOut(0) = ERR_NO_ERROR;
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    else
    {
        /* buffer is full, set buffer overrun error...*/
//		NCAPSA_nOut(0) = ERR_PUSH_ERROR;
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    /* copy state data back over...*/
    pLIFO_Bool->nCurrentUsage = nCurrentUsage;
    pLIFO_Bool->nIndex = nIndex;


    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PopLIFOBool)
{
    structLIFO_Bool* pLIFO_Bool = NULL;

    int nIndex;
    int nCurrentUsage;
    //int nSize;
    int* pContents = NULL;

    //ehs_uint8* pB = NULL;

    /* the dereference of pByte gives the location of the state data...*/
    pLIFO_Bool = (structLIFO_Bool*)EHS_FB_RUN_CONTEXT;

    nIndex = pLIFO_Bool->nIndex;
    nCurrentUsage = pLIFO_Bool->nCurrentUsage;
    //nSize = pLIFO_Bool->nSize;
    pContents = (int*)pLIFO_Bool->pContents;

    if (nCurrentUsage == 0)
    {
        /* we can't pop an empty buffer, set underrun error ...*/
//		NCAPSA_nOut(1) = ERR_POP_ERROR;
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    else
    {
        //	pB = NCAPSA_pOut(0);
        /* pop the buffer to the required output location...*/
        NCAPSA_bOut(0) = (ehs_bool)(0 != pContents[nIndex - 1]);
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
        /* decrement index for next pop...*/
        nIndex--;
        /* decrement usage indicator...*/
        nCurrentUsage--;
        /* reset any error condition...*/
//		NCAPSA_nOut(1) = ERR_NO_ERROR;
    }
    /* copy the data back into the object state memory...*/
    pLIFO_Bool->nIndex = nIndex;
    pLIFO_Bool->nCurrentUsage = nCurrentUsage;

    return;
}

/******************************************************************************/
/* Define FIFO Integer function block */

EHS_FB_FUNCTIONS_START(LIFOString)

EHS_FB_FUNCTION_ENTRY("Run_PushLIFOString", 0x01, PushLIFOString)

EHS_FB_FUNCTION_ENTRY("Run_PopLIFOString", 0x02, PopLIFOString)

EHS_FB_FUNCTION_ENTRY("Run_FlushLIFOString", 0x03, FlushAnyLIFOBuffer)
EHS_FB_FUNCTIONS_END

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(LIFOString)
{
    /* basically we need to malloc the size of the LIFO struct * number required...*/
    EHS_FB_IDENTIFY_MEMORY = sizeof(structLIFO_String);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(LIFOString)
{
    int nSize;
    int i;
    char** pContents = NULL;
    structLIFO_String* pLIFO_String = NULL;

    nSize = atoi(EHS_FB_INIT_PARAMETERS);
    /* malloc some space for the buffer from the pParams description...*/
    pContents = (char**)EhsHMem_writeableAlloc(nSize*sizeof(char**));
    if( !(pContents) )
    {
        return EHS_TRUE;
    }

    for (i=0; i<nSize; i++)
    {
        pContents[i] = (char*)EhsHMem_writeableAlloc(EHS_STRING_LENGTH_MAX); //TODO:STRINGLENGTH!
        if( !(pContents[i]) )
        {
            return EHS_TRUE;
        }
    }

    pLIFO_String = (structLIFO_String*)EHS_FB_INIT_CONTEXT;

    pLIFO_String->nIndex = 0;
    pLIFO_String->nCurrentUsage = 0;
    pLIFO_String->nSize = nSize;
    pLIFO_String->pContents = pContents;

    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PushLIFOString)
{
    structLIFO_String* pLIFO_String = NULL;

    int nIndex;
    int nCurrentUsage;
    int nSize;
    char** pContents = NULL;

    /* the dereference of pByte gives the location of the state data...*/
    pLIFO_String = (structLIFO_String*)EHS_FB_RUN_CONTEXT;
    /* copy the required parts to local storage...*/
    nIndex = pLIFO_String->nIndex;
    nCurrentUsage = pLIFO_String->nCurrentUsage;
    nSize = pLIFO_String->nSize;
    pContents = (char**)pLIFO_String->pContents;

    /* check we haven't walked over the top of the buffer...*/
    if (nCurrentUsage < nSize)
    {
        /* push input on to the buffer...*/
        strcpy(pContents[nIndex],NCAPSA_szIn(0));
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
        /* increment the push and current indicies...*/
        nIndex++;
        nCurrentUsage++;
        /* reset any error condition...*/
//		NCAPSA_nOut(0) = ERR_NO_ERROR;
    }
    else
    {
        /* buffer is full, set buffer overrun error...*/
//		NCAPSA_nOut(0) = ERR_PUSH_ERROR;
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    /* copy state data back over...*/
    pLIFO_String->nCurrentUsage = nCurrentUsage;
    pLIFO_String->nIndex = nIndex;

    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(PopLIFOString)
{
    structLIFO_String* pLIFO_String = NULL;

    int nIndex;
    int nCurrentUsage;
    //int nSize;
    char** pContents = NULL;

    //ehs_uint8* pB = NULL;

    /* the dereference of pByte gives the location of the state data...*/
    pLIFO_String = (structLIFO_String*)EHS_FB_RUN_CONTEXT;

    nIndex = pLIFO_String->nIndex;
    nCurrentUsage = pLIFO_String->nCurrentUsage;
    //nSize = pLIFO_String->nSize;
    pContents = (char**)pLIFO_String->pContents;

    if (nCurrentUsage == 0)
    {
        /* we can't pop an empty buffer, set underrun error ...*/
//		NCAPSA_nOut(1) = ERR_POP_ERROR;
        SetCompletes2((structFuncArg*)(structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    else
    {
        //	pB = NCAPSA_pOut(0);
        /* pop the buffer to the required output location...*/
        strcpy(NCAPSA_szOut(0),pContents[nIndex - 1]);
        SetCompletes1((structFuncArg*)(structFuncArg*)&EHS_FB_RUN_CONTEXT);
        /* decrement index for next pop...*/
        nIndex--;
        /* decrement usage indicator...*/
        nCurrentUsage--;
        /* reset any error condition...*/
//		NCAPSA_nOut(1) = ERR_NO_ERROR;
    }
    /* copy the data back into the object state memory...*/
    pLIFO_String->nIndex = nIndex;
    pLIFO_String->nCurrentUsage = nCurrentUsage;


    return;
}


/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
EHS_FB_RUN_FUNCTION(FlushLIFOString)
{
	structLIFO_String* pLIFO_String = (structLIFO_String*)EHS_FB_RUN_CONTEXT;
	pLIFO_String->nIndex = 0;
	pLIFO_String->nCurrentUsage = 0;
	SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
	return;
}
*/
