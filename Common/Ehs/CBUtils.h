/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/** @file CBUtils.h
 * This file provides declarations of utilities for setting up the call back function table.
 *
 * @author: inx limited
 *
 */


#error "Is this header being read"

#ifndef  _CBUTILS_H
#define  _CBUTILS_H

#include "functable.h"
#include "types.h"

//todo - This is currently not used anywhere, but probablyy should be.
// Seem we should change it to lnked list (rather an array that I'm nit sure is actually allocated anywhere.
// would also be bet to change the index to be an ID value so we don't have to worry about managing the list.
// linkedlist would be managed by destry functions or just use the app-scope memory allocator.

ptrfRunFunc GetCallbackPointers(callbackObjData* objectData,int nCallBackNumber, structFuncArg** functionArgData);
BOOL SetObjectCallbackIndex(void * objectData,int nCallBackObjNumber,int nCallBackTableIndex );

#endif
