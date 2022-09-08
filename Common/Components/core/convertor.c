/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/* convertor.c
 *
 * src file for convertor functions.  Convertor functions convert boolean data
 * table values (such as error conditions output by other objects) into triggers
 * that may be used to trigger other objects.  Any function appearing in
 * this file must also be listed in objRefTable.c for it to be included in the
 * list of functions available to the EHS.
 *
 * For definition of arguments in Ncapsa functions (Identify_, Init_ and Run_)
 * please see types.h.
 *
*/

#include <string.h> //PPP: This should probably be an OS specific include for HAL.

#include "target.h"
#include "convertor.h"
#include "setCompletes.h"
#include "globals.h"

#include "hal-api.h" /* Required for logging */
/******************************************************************************/
/* Define ConvertorBoolToTrigger function block */

EHS_FB_FUNCTIONS_START(ConvertorBoolToTrigger)

EHS_FB_FUNCTION_ENTRY("Run_ConvertorBoolToTrigger", 0x00, ConvertorBoolToTrigger)
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
EHS_FB_IDENTIFY_FUNCTION(ConvertorBoolToTrigger)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ConvertorBoolToTrigger)
{
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ConvertorBoolToTrigger)
{
    ehs_bool bIn;
    /* converts a single boolean data value to a trigger...*/
    bIn = NCAPSA_bIn(0);
    /* if this trigger is fired and the boolean input is true...*/
    if (bIn)
    {
        SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    return;
}

/******************************************************************************/
/* Define ConvertorIntToBool function block */

EHS_FB_FUNCTIONS_START(ConvertorIntToBool)

EHS_FB_FUNCTION_ENTRY("Run_ConvertorIntToBool", 0x00, ConvertorIntToBool)
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
EHS_FB_IDENTIFY_FUNCTION(ConvertorIntToBool)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ConvertorIntToBool)
{
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ConvertorIntToBool)
{
    NCAPSA_bOut(0) = (ehs_bool)NCAPSA_nIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}


/******************************************************************************/
/* Define ConvertorStringToBool function block */

EHS_FB_FUNCTIONS_START(ConvertorStringToBool)

EHS_FB_FUNCTION_ENTRY("Run_ConvertorStringToBool", 0x00, ConvertorStringToBool)
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
EHS_FB_IDENTIFY_FUNCTION(ConvertorStringToBool)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ConvertorStringToBool)
{
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ConvertorStringToBool)
{
    int cmp1, cmp2;
    char szString[EHS_STRING_LENGTH_MAX] = {'\0'};
    char* pString = &szString[0];
    pString = strcpy(pString, NCAPSA_szIn(0));
//	pString = strupr(pString);
    cmp1 = strcmp(pString, "0");
    cmp2 = EhsStricmp(pString, "EHS_FALSE");
    NCAPSA_bOut(0) = (ehs_bool)((cmp1 != 0) && (cmp2 != 0));

    //NCAPSA_bOut(0) = (ehs_bool)NCAPSA_szIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define ConvertorStringToInt function block */

EHS_FB_FUNCTIONS_START(ConvertorStringToInt)

EHS_FB_FUNCTION_ENTRY("Run_ConvertorStringToInt", 0x00, ConvertorStringToInt)
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
EHS_FB_IDENTIFY_FUNCTION(ConvertorStringToInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ConvertorStringToInt)
{
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ConvertorStringToInt)
{
    char szString[EHS_STRING_LENGTH_MAX] = {'\0'};
    char* pString = &szString[0];
    int nInt;
    /* converts a stirng data value to an int data value...*/
    pString = strcpy(pString, NCAPSA_szIn(0));
    nInt = atoi(pString);
    NCAPSA_nOut(0) = nInt;

    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define ConvertorBoolToInt function block */

EHS_FB_FUNCTIONS_START(ConvertorBoolToInt)

EHS_FB_FUNCTION_ENTRY("Run_ConvertorBoolToInt", 0x00, ConvertorBoolToInt)
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
EHS_FB_IDENTIFY_FUNCTION(ConvertorBoolToInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ConvertorBoolToInt)
{
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ConvertorBoolToInt)
{
    NCAPSA_nOut(0) = (int)NCAPSA_bIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define ConvertorBoolToString function block */

EHS_FB_FUNCTIONS_START(ConvertorBoolToString)

EHS_FB_FUNCTION_ENTRY("Run_ConvertorBoolToString", 0x00, ConvertorBoolToString)
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
EHS_FB_IDENTIFY_FUNCTION(ConvertorBoolToString)
{
}
/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ConvertorBoolToString)
{
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ConvertorBoolToString)
{

    if (NCAPSA_bIn(0))
    {
        strcpy(NCAPSA_szOut(0), "1");
    }
    else
    {
        strcpy(NCAPSA_szOut(0), "0");
    }
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define ConvertorIntToString function block */

EHS_FB_FUNCTIONS_START(ConvertorIntToString)

EHS_FB_FUNCTION_ENTRY("Run_ConvertorIntToString", 0x00, ConvertorIntToString)
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
EHS_FB_IDENTIFY_FUNCTION(ConvertorIntToString)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ConvertorIntToString)
{
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ConvertorIntToString)
{
    EhsSprintf(NCAPSA_szOut(0), "%d", NCAPSA_nIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

#ifdef EHS_TARGET_FP_SUPPORT

/******************************************************************************/
/* Define Identify_ConvertorRealToBool function block */

EHS_FB_FUNCTIONS_START(ConvertorRealToBool)
EHS_FB_FUNCTION_ENTRY("Run_ConvertorRealToBool", 0x00, ConvertorRealToBool)
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
EHS_FB_IDENTIFY_FUNCTION(ConvertorRealToBool)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ConvertorRealToBool)
{
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ConvertorRealToBool)
{
    NCAPSA_bOut(0) = (ehs_bool)NCAPSA_dIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define ConvertorRealToInt function block */

EHS_FB_FUNCTIONS_START(ConvertorRealToInt)

EHS_FB_FUNCTION_ENTRY("Run_ConvertorRealToInt", 0x00, ConvertorRealToInt)
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
EHS_FB_IDENTIFY_FUNCTION(ConvertorRealToInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ConvertorRealToInt)
{
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ConvertorRealToInt)
{
    NCAPSA_nOut(0) = (int)NCAPSA_dIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define ConvertorBoolToReal function block */

EHS_FB_FUNCTIONS_START(ConvertorBoolToReal)

EHS_FB_FUNCTION_ENTRY("Run_ConvertorBoolToReal", 0x00, ConvertorBoolToReal)
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
EHS_FB_IDENTIFY_FUNCTION(ConvertorBoolToReal)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ConvertorBoolToReal)
{
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ConvertorBoolToReal)
{
    /*	if (NCAPSA_bIn(0) == EHS_TRUE)
    	{
    		NCAPSA_dOut(0) = 1.0;
    	}
    	else
    	{
    		NCAPSA_dOut(0) = 0.0;
    	}*/
    NCAPSA_dOut(0) = (double)NCAPSA_bIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define ConvertorIntToReal function block */

EHS_FB_FUNCTIONS_START(ConvertorIntToReal)
EHS_FB_FUNCTION_ENTRY("Run", 0x00, ConvertorIntToReal)
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
EHS_FB_IDENTIFY_FUNCTION(ConvertorIntToReal)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ConvertorIntToReal)
{
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ConvertorIntToReal)
{
    NCAPSA_dOut(0) = (double)NCAPSA_nIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define ConvertorStringToReal function block */

EHS_FB_FUNCTIONS_START(ConvertorStringToReal)

EHS_FB_FUNCTION_ENTRY("Run_ConvertorStringToReal", 0x00, ConvertorStringToReal)
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
EHS_FB_IDENTIFY_FUNCTION(ConvertorStringToReal)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ConvertorStringToReal)
{
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ConvertorStringToReal)
{
    char szString[EHS_STRING_LENGTH_MAX] = {'\0'};
    char* pString = &szString[0];

    pString = strcpy(pString, NCAPSA_szIn(0));
    NCAPSA_dOut(0) = atof(pString);

    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define ConvertorRealToString function block */

EHS_FB_FUNCTIONS_START(ConvertorRealToString)

EHS_FB_FUNCTION_ENTRY("Run_ConvertorRealToString", 0x00, ConvertorRealToString)
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
EHS_FB_IDENTIFY_FUNCTION(ConvertorRealToString)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ConvertorRealToString)
{
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ConvertorRealToString)
{
    if(EHS_FB_OUT_CONNECTED(0) && EHS_FB_IN_CONNECTED(0))
    {
        EhsSprintf(NCAPSA_szOut(0), "%.3f", NCAPSA_dIn(0));
    }
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif /* EHS_TARGET_FP_SUPPORT */
