/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

#include "globals.h"
#include "hal_process.h" //needed for EhsHThread_execute()

/*
 * Function instance specific version of the thread creator
 * This is use for Function block facing HAL API.
 * It simply calls the target specific function EhsHThread_execute()
 * */

ehs_bool EhsTPThread_execute(EhsThreadFuncType pfRun, struct EhsFunctionInstanceDataStruct* context, ehs_sint16 priority, ehs_sint32 stackSize, ehs_char * _szThreadname)
{
    return EhsHThread_execute((EhsGeneralThreadFuncType)pfRun, (void *) context, priority, stackSize,_szThreadname) ;
}




