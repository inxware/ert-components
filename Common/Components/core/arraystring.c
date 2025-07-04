/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * @file arraystring.c
 *
 * source file for array functions.
 *
 *
 * @author: inx limited
 *
 */

#include "arraystring.h"
#include "globals.h"
//#include "target.h"
/** @todo check which includes are required here */
//#include "types.h"
#include "setCompletes.h"

#include "hal-api.h" /* Required for logging */

EHS_FB_FUNCTIONS_START(ArrayString)

EHS_FB_FUNCTION_ENTRY("Run_ArrayStringWrite", 0x01, ArrayStringWrite)

EHS_FB_FUNCTION_ENTRY("Run_ArrayStringRead", 0x02, ArrayStringRead)

EHS_FB_FUNCTIONS_END

/**
 * Structure used only in this function block. Thus scope is limited to arraystring.c
 */
typedef struct
{
    char** arrayData;
    int arraySize;
} EHS_structArray;

/**
 * Define the identify function
 */
EHS_FB_IDENTIFY_FUNCTION(ArrayString)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(EHS_structArray);
}

/**
 * Initialise the memory required by the array
 *
 * @todo remove malloc from this function
 */
EHS_FB_INIT_FUNCTION(ArrayString)
{
    EHS_structArray *arrayString;
    int i;

    arrayString = (EHS_structArray*)EHS_FB_INIT_CONTEXT;
    arrayString->arraySize = atoi(EHS_FB_INIT_PARAMETERS);
    arrayString->arrayData = (char **)EhsHMem_writeableAlloc(sizeof(char*)*(arrayString->arraySize));

    if (!arrayString->arrayData)
    {
        return EHS_FALSE;
    }
    for (i = 0; i < arrayString->arraySize; i++)
    {
        arrayString->arrayData[i] = (char *)EhsHMem_writeableAlloc(EHS_STRING_LENGTH_MAX);//TODO:STRINGLENGTH! allocate in run function when we know connected port sizes?
        arrayString->arrayData[i][0] = '\0'; /* make them all empty */
        if (!arrayString->arrayData[i])
        {
            return EHS_FALSE;
        }
        EhsSprintf(arrayString->arrayData[i],"Unallocated data in cell %d",i);
    }
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Read from the array
 * @todo provide range checking on array read/write.
 */
EHS_FB_RUN_FUNCTION(ArrayStringRead)
{
    EHS_structArray *arrayString = (EHS_structArray*)EHS_FB_RUN_CONTEXT;
    int index;
    // Output Assignment
    index = NCAPSA_nIn(0);
    if ((index < 0 ) || index > arrayString->arraySize )
    {
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT); /* @todo put a proper overflow function in here */
    }
    else
    {
        EhsStrcpy(NCAPSA_szOut(0),arrayString->arrayData[index] );
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);  //  rd
    }
    return;
}

/**
 * Write to the array
 * @todo provide range checking.
 */
EHS_FB_RUN_FUNCTION(ArrayStringWrite)
{
    EHS_structArray *arrayString = (EHS_structArray*)EHS_FB_RUN_CONTEXT;
    int index;
    index = NCAPSA_nIn(1);
    if ((index < 0 ) || (index >= arrayString->arraySize) )
    {
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT); /* @todo put a proper overflow function in here */
    }
    else
    {
        // Input Assignment
        strcpy(arrayString->arrayData[index], NCAPSA_szIn(0));
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);  //  wr
    }
    return;
}

