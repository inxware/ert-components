/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file CBUtils.h
 * This file provides declarations of utilities for setting up the call back function table.
 *
 * @author: inx limited
 *
 */

#warning "This should be moved to the KAPI directory X" 


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
