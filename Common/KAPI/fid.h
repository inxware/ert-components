/** @file fid.h
 * Declarations for the EhsFunctionInstanceDataType and related functions
 * 
 * @author: inx limited
 * @version: $Revision: 4930 $
 * @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
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


typedef struct EhsFunctionInstanceDataStruct {
	void* pObjData;
	EhsRunFuncType fpRunFunc; /**< Pointer to the function to run when this function is invoked */
	ehs_uint8 nNumInputs; /**< Number of data inputs to this function block */
	ehs_uint8 nNumOutputs; /**< Number of data outputs to this function block */
	ehs_uint8 nFinishPorts; /**< The number of finish ports that can be accessed by this function */
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

