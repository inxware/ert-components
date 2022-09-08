/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/** @file CBUtils.c
 * This file provides utilities for setting up the call back function table.
 *
 * @author: inx limited
 *
 */

#include "CBUtils.h"
#include "target.h"
extern ptrfRunFunc* sCallBackFunc ; /**< stores the object scope address of a call back function */
extern structFuncArg* sCallBackMain ; /**<stores the call back function's data. */

#error "Is this compiled?"

/**
 * Retrieve pointers to call back functions.
 *
 * THIS FUNCTION IS TYPICALLY CALLED BY A CREATE FUNCTION TO FIND THE CODE AND ASSOCIATED DATA FOR THE ACTUAL CALL BACK FUNCTION
 * the CB function prototype is the same as any other token table prototype, and also the argument data structure.
 * Note uses a global variable to reference the CB function data.
 * @param objectData
 * @param nCallBackNumber Index into the call back function table.
 * @param functionArgData
 * @return Pointer to call back function installer
 * @todo Describe function parameters better.
 */
ptrfRunFunc  GetCallbackPointers(callbackObjData* objectData,int nCallBackNumber, structFuncArg** functionArgData)
{
    ptrfRunFunc functionPointer;
    //if (nCallBackNumber<0) return FALSE; // feeble error checking necessary?
    //get the index (from the object data) for the call back function for the global call back function table.
    int nCallBackTableNumber;

    nCallBackTableNumber = objectData->callBackNumber[nCallBackNumber-1];

//	EHS_DebugInfo("callbacktablenumber = %d\n");
    functionPointer = sCallBackFunc[nCallBackTableNumber];
    *functionArgData = &sCallBackMain[nCallBackTableNumber];
//	EHS_DebugInfo("sCallBackFunc[0] = %x\nsCallBackFunc[1] = %x\nsCallBackFunc[2] = %x\nsCallBackFunc[3] = %x\n",
//		sCallBackFunc[0],sCallBackFunc[1],sCallBackFunc[2],sCallBackFunc[3]);
    return functionPointer;
}

/**
 * Set index for call back functions.
 *
 * @param objectData
 * @param nCallBackObjNumber
 * @param nCallBackTableIndex
 * @return TRUE
 */
BOOL SetObjectCallbackIndex(void * objectData,int nCallBackObjNumber,int nCallBackTableIndex )
{
    //if (nCallBackNumber<0) return FALSE; // feeble error checking necessary?
    //get the index (from the object data) for the call back function for the global call back function table.
    ((callbackObjData*)objectData)->callBackNumber[nCallBackObjNumber-1]=nCallBackTableIndex;
    //((callbackObjData*)objectData)->callBackNumber[nCallBackObjNumber]=nCallBackTableIndex;

    //functionPointer = (void*)sCallBackFunc[nCallBackTableNumber];
    //functionArgData = &sCallBackMain[nCallBackTableNumber];
    return TRUE;
}
