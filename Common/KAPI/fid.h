/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file fid.h
 * Declarations for the EhsFunctionInstanceDataType and related functions
 *
 * @author: inx limited
 *
 */

#ifndef _FUNCTION_INSTANCE_DATA_H
#define _FUNCTION_INSTANCE_DATA_H

#include "ehs_defs.h"
#include "target_types.h"
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
    ehs_uint8 nNumInputs; /**< Number of data inputs to this function block, it is populated when parsing SODL as the number if inout connections supported by the FB.  */
    ehs_uint8 nNumOutputs; /**< Number of data outputs to this function block (also populated when parsing SODL).*/
    ehs_uint8 nFinishPorts; /**< The number of finish ports that can be accessed by this function (also populated when parsing SODL) */
    EhsTriggerType** pFinishPort; /**< Provides access to the set of start ports that are connected to each finish port */
    void** pIn;
    void** pOut;
    ehs_char* cDataOutType; /**< The type of each output data port */
    debug_type_byte dMonitorType; /* bitmask for monitor type for this function - see DEBUG_SET_MASK_ above */
} EhsFunctionInstanceDataType;

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

EHS_GLOBAL void EhsFunctionInstanceData_triggerEvent(EhsFunctionInstanceDataType* pByte, ehs_uint32 x);


#endif /* _FUNCTION_INSTANCE_DATA_H */

