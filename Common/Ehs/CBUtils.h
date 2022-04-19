/** @file CBUtils.h
 * This file provides declarations of utilities for setting up the call back function table.
 * 
 * @author: Lucid V0.9 team
 * @version: $Revision: 20 $
 * @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
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
