/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
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


#include "globals.h"
/** @todo check which includes are required here */
//#include "types.h"
#include "setCompletes.h"
#include "hal-api.h" /* Required for logging */
#include "arraystring1.h"


EHS_FB_FUNCTIONS_START(ArrayString1)

EHS_FB_FUNCTION_ENTRY("Run_ArrayStringClear", 0x01, ArrayStringClear1)

EHS_FB_FUNCTION_ENTRY("Run_ArrayStringWrite", 0x02, ArrayStringWrite1)

EHS_FB_FUNCTION_ENTRY("Run_ArrayStringRead", 0x03, ArrayStringRead1)

EHS_FB_FUNCTIONS_END

/**
 * Structure used only in this function block. Thus scope is limited to arraystring.c
 */
typedef struct
{
    char** arrayData;
    int arraySize;
    char *clear_value;
} EHS_structArray;

/**
 * Define the identify function
 */
EHS_FB_IDENTIFY_FUNCTION(ArrayString1)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(EHS_structArray);
}

/**
 * Initialise the memory required by the array
 *
 * @todo remove malloc from this function
 */
EHS_FB_INIT_FUNCTION(ArrayString1)
{
    EHS_structArray *arrayString;
    int i;

    arrayString = (EHS_structArray*)EHS_FB_INIT_CONTEXT;

    arrayString->clear_value =(char *)EhsHMem_writeableAlloc(EHS_STRING_LENGTH_MAX * sizeof(char*)); // OK so it's a bit bigger than it needs to be..
    if (!arrayString->clear_value)
    {
        EHSH_LOG_ERROR("Could not allocate default value memory");
        return EHS_FALSE;
    }

    EhsStrcpy(arrayString->clear_value,"");
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d%s",&(arrayString->arraySize),arrayString->clear_value);
    if(EhsStrcmp(arrayString->clear_value,"NULL")==0) arrayString->clear_value[0]='\0';


    arrayString->arrayData = (char **)EhsHMem_writeableAlloc(sizeof(char*)*(arrayString->arraySize));

    if (!arrayString->arrayData)
    {
        EHSH_LOG_ERROR("Could not allocate array memory");
        return EHS_FALSE;
    }
    else
    {
        for (i = 0; i < arrayString->arraySize; i++)
        {
            arrayString->arrayData[i] = (char *)EhsHMem_writeableAlloc(EHS_STRING_LENGTH_MAX);
            if (!arrayString->arrayData[i])
            {
                EHSH_LOG_ERROR("Could not allocate element Memory");
                return EHS_FALSE;
                break;
            }
            if (arrayString->arrayData[i]) EhsStrcpy(arrayString->arrayData[i],arrayString->clear_value);
        }
    }
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Read from the array
 * @todo provide range checking on array read/write.
 */
EHS_FB_RUN_FUNCTION(ArrayStringClear1)
{
    EHS_structArray *arrayString = (EHS_structArray*)EHS_FB_RUN_CONTEXT;
    int i;
    if (!arrayString->arrayData) return; // we've already logged an error

    for (i = 0; i < arrayString->arraySize; i++)
    {
        if (!arrayString->arrayData[i])
        {
            EHSH_LOG_ERROR("Element memory not allocated");
        }
        if (arrayString->clear_value)
            EhsStrcpy(arrayString->arrayData[i],arrayString->clear_value);
        else
            EhsStrcpy(arrayString->arrayData[i],"");
    }

    SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);  //  rd
}

/**
 * Read from the array
 */
EHS_FB_RUN_FUNCTION(ArrayStringRead1)
{
    EHS_structArray *arrayString = (EHS_structArray*)EHS_FB_RUN_CONTEXT;
    int index;
    // Output Assignment
    index = NCAPSA_nIn(0);
    if (!arrayString->arrayData)
    {
        // we've already logged an error
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    else if ((index < 0 ) || index >= arrayString->arraySize )
    {
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    else
    {
        if (arrayString->arrayData[index])
        {
            EhsStrcpy(NCAPSA_szOut(0),arrayString->arrayData[index]);
        }
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);  //  rd
    }
    return;
}

/**
 * Write to the array
 */
EHS_FB_RUN_FUNCTION(ArrayStringWrite1)
{
    EHS_structArray *arrayString = (EHS_structArray*)EHS_FB_RUN_CONTEXT;
    int index;

    index = NCAPSA_nIn(1);
    if (!arrayString->arrayData)
    {
        // we've already logged an error
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    else if ((index < 0 ) || (index >= arrayString->arraySize) )
    {
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT); /* @todo put a proper overflow function in here */
    }
    else
    {
        // Input Assignment
        if (arrayString->arrayData[index]) strcpy(arrayString->arrayData[index], NCAPSA_szIn(0));
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    return;
}


/* Bufer for Integers */

EHS_FB_FUNCTIONS_START(ArrayInt1)

EHS_FB_FUNCTION_ENTRY("Run_ArrayIntClear", 0x01, ArrayIntClear1)

EHS_FB_FUNCTION_ENTRY("Run_ArrayIntWrite", 0x02, ArrayIntWrite1)

EHS_FB_FUNCTION_ENTRY("Run_ArrayIntRead", 0x03, ArrayIntRead1)

EHS_FB_FUNCTIONS_END

/**
 * Structure used only in this function block. Thus scope is limited to ArrayInt.c
 */
typedef struct
{
    ehs_sint32* arrayData;
    ehs_sint32 arraySize;
    ehs_sint32 clearval;
} EHS_intstructArray;

/**
 * Define the identify function
 */
EHS_FB_IDENTIFY_FUNCTION(ArrayInt1)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(EHS_intstructArray);
}

/**
 * Initialise the memory required by the array
 *
 * @todo remove malloc from this function
 */
EHS_FB_INIT_FUNCTION(ArrayInt1)
{
    EHS_intstructArray *ArrayInt;
    int i;

    ArrayInt = (EHS_intstructArray*)EHS_FB_INIT_CONTEXT;

    ArrayInt->clearval=0;
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d%d",&(ArrayInt->arraySize),&(ArrayInt->clearval));
//	ArrayInt->arraySize = atoi(EHS_FB_INIT_PARAMETERS);
    ArrayInt->arrayData = (ehs_sint32*)EhsHMem_writeableAlloc(sizeof(ehs_sint32)*(ArrayInt->arraySize));
    if (!ArrayInt->arrayData)
    {
        EHSH_LOG_ERROR("Could Not allocate memory");
        return EHS_FALSE;
    }
    else
    {
        for (i = 0; i < ArrayInt->arraySize; i++)
        {
            ArrayInt->arrayData[i]=ArrayInt->clearval;
        }
    }
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Read from the array
 * @todo provide range checking on array read/write.
 * @todo
 */
EHS_FB_RUN_FUNCTION(ArrayIntClear1)
{
    EHS_intstructArray *ArrayInt = (EHS_intstructArray*)EHS_FB_RUN_CONTEXT;
    int i;
    for (i = 0; i < ArrayInt->arraySize; i++)
    {
        ArrayInt->arrayData[i]=ArrayInt->clearval;
    }
    SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);  //  rd
}

/**
 * Read from the array
 * @todo provide range checking on array read/write.
 */
EHS_FB_RUN_FUNCTION(ArrayIntRead1)
{
    EHS_intstructArray *ArrayInt = (EHS_intstructArray*)EHS_FB_RUN_CONTEXT;
    int index;
    // Output Assignment
    index = NCAPSA_nIn(0);
    if ((index < 0 ) || index >= ArrayInt->arraySize )
    {
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    else
    {
        NCAPSA_nOut(0)=ArrayInt->arrayData[index];
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);  //  rd
    }
    return;
}

/**
 * Write to the array
 * @todo provide range checking.
 */
EHS_FB_RUN_FUNCTION(ArrayIntWrite1)
{
    EHS_intstructArray *ArrayInt = (EHS_intstructArray*)EHS_FB_RUN_CONTEXT;
    int index;
    index = NCAPSA_nIn(1);
    if ((index < 0 ) || (index >= ArrayInt->arraySize) )
    {
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    else
    {
        // Input Assignment
        ArrayInt->arrayData[index]=NCAPSA_nIn(0);
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);  //  wr
    }
    return;

}


/* and bools */

EHS_FB_FUNCTIONS_START(ArrayBool1)

EHS_FB_FUNCTION_ENTRY("Run_ArrayBoolClear", 0x01, ArrayBoolClear1)

EHS_FB_FUNCTION_ENTRY("Run_ArrayBoolWrite", 0x02, ArrayBoolWrite1)

EHS_FB_FUNCTION_ENTRY("Run_ArrayBoolRead", 0x03, ArrayBoolRead1)

EHS_FB_FUNCTIONS_END

/**
 * Structure used only in this function block. Thus scope is limited to ArrayBool.c
 */
typedef struct
{
    ehs_bool* arrayData;
    ehs_sint32 arraySize;
    ehs_bool clearval;
} EHS_boolstructArray;

/**
 * Define the identify function
 */
EHS_FB_IDENTIFY_FUNCTION(ArrayBool1)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(EHS_boolstructArray);
}

/**
 * Initialise the memory required by the array
 *
 * @todo remove malloc from this function
 */
EHS_FB_INIT_FUNCTION(ArrayBool1)
{
    EHS_boolstructArray *ArrayBool;
    int i;
    ehs_uint32 tempByte=0;

    ArrayBool = (EHS_boolstructArray*)EHS_FB_INIT_CONTEXT;

    ArrayBool->clearval=0;
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d%d",&(ArrayBool->arraySize),&tempByte);
//	ArrayBool->arraySize = atoi(EHS_FB_INIT_PARAMETERS);
    ArrayBool->arrayData = (ehs_bool*)EhsHMem_writeableAlloc(sizeof(ehs_bool)*(ArrayBool->arraySize));


    if (!ArrayBool->arrayData)
    {
        EHSH_LOG_ERROR("Could Not allocate memory");
        return EHS_FALSE;
    }
    else
    {
        ArrayBool->clearval=(ehs_bool) tempByte;
        for (i = 0; i < ArrayBool->arraySize; i++)
        {
            ArrayBool->arrayData[i]=ArrayBool->clearval;
        }
    }
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Read from the array
 * @todo provide range checking on array read/write.
 * @todo
 */
EHS_FB_RUN_FUNCTION(ArrayBoolClear1)
{
    EHS_boolstructArray *ArrayBool = (EHS_boolstructArray*)EHS_FB_RUN_CONTEXT;
    int i;
    for (i = 0; i < ArrayBool->arraySize; i++)
    {
        ArrayBool->arrayData[i]=ArrayBool->clearval;
    }
    SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);  //  rd
}

/**
 * Read from the array
 * @todo provide range checking on array read/write.
 */
EHS_FB_RUN_FUNCTION(ArrayBoolRead1)
{
    EHS_boolstructArray *ArrayBool = (EHS_boolstructArray*)EHS_FB_RUN_CONTEXT;
    int index;
    // Output Assignment
    index = NCAPSA_nIn(0);
    if ((index < 0 ) || index >= ArrayBool->arraySize )
    {
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    else
    {
        NCAPSA_bOut(0)=ArrayBool->arrayData[index];
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);  //  rd
    }
    return;
}

/**
 * Write to the array
 * @todo provide range checking.
 */
EHS_FB_RUN_FUNCTION(ArrayBoolWrite1)
{
    EHS_boolstructArray *ArrayBool = (EHS_boolstructArray*)EHS_FB_RUN_CONTEXT;
    int index;
    index = NCAPSA_nIn(1);
    if ((index < 0 ) || (index >= ArrayBool->arraySize) )
    {
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    else
    {
        // Input Assignment
        ArrayBool->arrayData[index]=NCAPSA_bIn(0);
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);  //  wr
    }
    return;

}

#ifdef EHS_TARGET_FP_SUPPORT

/* and Reals */

EHS_FB_FUNCTIONS_START(ArrayReal1)

EHS_FB_FUNCTION_ENTRY("Run_ArrayRealClear", 0x01, ArrayRealClear1)

EHS_FB_FUNCTION_ENTRY("Run_ArrayRealWrite", 0x02, ArrayRealWrite1)

EHS_FB_FUNCTION_ENTRY("Run_ArrayRealRead", 0x03, ArrayRealRead1)

EHS_FB_FUNCTIONS_END

/**
 * Structure used only in this function block. Thus scope is limited to ArrayReal.c
 */
typedef struct
{
    ehs_float* arrayData;
    ehs_sint32 arraySize;
    ehs_float clearval;
} EHS_realstructArray;

/**
 * Define the identify function
 */
EHS_FB_IDENTIFY_FUNCTION(ArrayReal1)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(EHS_realstructArray);
}

/**
 * Initialise the memory required by the array
 *
 * @todo remove malloc from this function
 */
EHS_FB_INIT_FUNCTION(ArrayReal1)
{
    EHS_realstructArray *ArrayReal;
    int i;
    float temp=0; //@todo we should have single and double floats defined in ehs_ verbage.

    ArrayReal = (EHS_realstructArray*)EHS_FB_INIT_CONTEXT;

    ArrayReal->clearval=0.0f;
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d%f",&(ArrayReal->arraySize),&temp);
    ArrayReal->clearval=(ehs_float) temp;
//	ArrayReal->arraySize = atoi(EHS_FB_INIT_PARAMETERS);
    ArrayReal->arrayData = (ehs_float*)EhsHMem_writeableAlloc(sizeof(ehs_float)*(ArrayReal->arraySize));
    if (!ArrayReal->arrayData)
    {
        EHSH_LOG_ERROR("Could Not allocate memory");
        return EHS_FALSE;
    }
    else
    {
        for (i = 0; i < ArrayReal->arraySize; i++)
        {
            ArrayReal->arrayData[i]=ArrayReal->clearval;
        }
    }
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Read from the array
 * @todo provide range checking on array read/write.
 * @todo
 */
EHS_FB_RUN_FUNCTION(ArrayRealClear1)
{
    EHS_realstructArray *ArrayReal = (EHS_realstructArray*)EHS_FB_RUN_CONTEXT;
    int i;
    for (i = 0; i < ArrayReal->arraySize; i++)
    {
        ArrayReal->arrayData[i]=ArrayReal->clearval;
    }
    SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);  //  rd
}

/**
 * Read from the array
 * @todo provide range checking on array read/write.
 */
EHS_FB_RUN_FUNCTION(ArrayRealRead1)
{
    EHS_realstructArray *ArrayReal = (EHS_realstructArray*)EHS_FB_RUN_CONTEXT;
    int index;
    // Output Assignment
    index = NCAPSA_nIn(0);
    if ((index < 0 ) || index >= ArrayReal->arraySize )
    {
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    else
    {
        NCAPSA_dOut(0)=ArrayReal->arrayData[index];
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);  //  rd
    }
    return;
}

/**
 * Write to the array
 * @todo provide range checking.
 */
EHS_FB_RUN_FUNCTION(ArrayRealWrite1)
{
    EHS_realstructArray *ArrayReal = (EHS_realstructArray*)EHS_FB_RUN_CONTEXT;
    int index;
    index = NCAPSA_nIn(1);
    if ((index < 0 ) || (index >= ArrayReal->arraySize) )
    {
        SetCompletes2((structFuncArg*)&EHS_FB_RUN_CONTEXT);
    }
    else
    {
        // Input Assignment
        ArrayReal->arrayData[index]=NCAPSA_dIn(0);
        SetCompletes1((structFuncArg*)&EHS_FB_RUN_CONTEXT);  //  wr
    }
    return;

}
#endif



