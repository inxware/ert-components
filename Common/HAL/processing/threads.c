/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

#include "ehs_types.h" // needed for function data struct types
#include "hal_process.h" //needed for EhsHThread_execute()

/*
 * Function instance specific version of the thread creator
 * This is use for Function block facing HAL API.
 * It simply calls the target specific function EhsHThread_execute()
 * */

EHS_GLOBAL EhsTPThread EhsTPThread_execute(EhsThreadFuncType pfRun, struct EhsFunctionInstanceDataStruct* context,ehs_sint16 priority)
{
    return EhsHThread_execute((EhsGeneralThreadFuncType)pfRun, (void *) context,priority) ;
}


