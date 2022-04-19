/** @file CBUtils.c
 * This file provides utilities for setting up the call back function table.
 * 
 * @author: Lucid V0.9 team
 * @version: $Revision: 20 $
 * @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
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
