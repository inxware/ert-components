/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file fid.h
 * Declarations for the EhsFunctionInstanceDataType and related functions
 *
 * @author: inx limited
 *
 */

#ifndef _FUNCTION_INSTANCE_DATA_H
#define _FUNCTION_INSTANCE_DATA_H

#include "globals.h"
#include "ehs_fb_types.h"

/* debug masks */
#define DEBUG_MASK_MONITOR			0x01	// 0000 0001
#define DEBUG_MASK_BREAKPOINT		0x02	// 0000 0010

/* debug type masks */
/* note - not all combinations of masks are valid */
#define DEBUG_SET_MASK_NONE				0x00											// 0000 0000
#define DEBUG_SET_MASK_MONITOR			(DEBUG_MASK_MONITOR)							// 0000 0001
#define DEBUG_SET_MASK_BREAKPOINT		(DEBUG_MASK_MONITOR | DEBUG_MASK_BREAKPOINT)	// 0000 0011


/* This structure defines the function block's paramters and is defined 
    There is one of these for each function block.
*/ 

typedef struct EhsFunctionInstanceDataStruct
{
    void* pObjData;
    EhsRunFuncType fpRunFunc; /**< Pointer to the function to run when this function is invoked */
    EhsTriggerType** pFinishPort; /**< Provides access to the set of start ports that are connected to each finish port */
    void** pIn;
    void** pOut;
    ehs_char* cDataOutType; /**< The type of each output data port */
    debug_type_byte dMonitorType; /* bitmask for monitor type for this function - see DEBUG_SET_MASK_ above */
    ehs_uint8 nNumInputs; /**< Number of data inputs to this function block, it is populated when parsing SODL as the number if inout connections supported by the FB.  */
    ehs_uint8 nNumOutputs; /**< Number of data outputs to this function block (also populated when parsing SODL).*/
    ehs_uint8 nFinishPorts; /**< The number of finish ports that can be accessed by this function (also populated when parsing SODL) */
} EhsFunctionInstanceDataType;

/* TODO BLOCK - remove this or ractify this for debug */
// @pierre
#if 0
#include "stdio.h"
static inline void EhsHDebugPrint_pFIdata(EhsFunctionInstanceDataType *a)
{
    printf(
        "{\n%p\n"
        "pObjData %p \n"
        "fpRunFunc %p\n"
        "pFinishPort %p\n"
        "pIn: %p ; pOut: %p\n"
        "cDataOutType %p\n"
        "nNumInputs: %d\n"
        "nNumOutputs: %d\n"
        "nFinishPorts: %d\n"
        "}\n",
        a,
        a->pObjData,
        a->fpRunFunc,
        a->pFinishPort,
        a->pIn, a->pOut,
        a->cDataOutType,
        a->nNumInputs,
        a->nNumOutputs,
        a->nFinishPorts
    );
}
#endif
/* TODO BLOCK */

typedef struct EhsFunctionInstanceClearEventsStruct
{
    EhsFunctionInstanceDataType* pFuncInstCaller; /* function instance which calls EhsFunctionInstanceData_clearEvents */
    EhsFunctionInstanceDataType* pFuncInst; /* function instance of which event will be cleared if onClearEventCondition returns EHS_TRUE */
    ehs_bool (*onClearEventCondition)(void* self);  /* callback used for deciding if event queue elemnet should be removed. Return EHS_TRUE to clear event. */
} EhsFunctionInstanceClearEventsType;

/**
 * Set the monitor flag of the specified function instance
 */
#define EhsFunctionInstanceData_monitorFunction(pFunc,monType) (pFunc)->dMonitorType = (monType)

#define EhsFunctionInstanceData_triggerEventReversedParams(x,pByte) EhsFunctionInstanceData_triggerEvent(pByte,x)

/* Legacy API support */
//@todo depricate this out
#define SetCompletes(p) EHS_FB_FINISH(1)
#define SetCompletes1(p) EHS_FB_FINISH(1)
#define SetCompletes2(p) EHS_FB_FINISH(2)
#define SetCompletes3(p) EHS_FB_FINISH(3)
#define SetCompletes4(p) EHS_FB_FINISH(4)
#define SetCompletes5(p) EHS_FB_FINISH(5)
#define SetCompletes6(p) EHS_FB_FINISH(6)
#define SetCompletes7(p) EHS_FB_FINISH(7)
#define SetCompletes8(p) EHS_FB_FINISH(8)
/* default is to only reset one event - no multiple event */

EHS_MEMORY_ATTRIB void EhsFunctionInstanceData_triggerEvent(EhsFunctionInstanceDataType* pByte, ehs_uint32 x);

void EhsFunctionInstanceData_clearEvents(EhsFunctionInstanceClearEventsType* pClearEvents);

#endif /* _FUNCTION_INSTANCE_DATA_H */

