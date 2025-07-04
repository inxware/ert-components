/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

#ifndef EHS_COMP_API2_H
#define EHS_COMP_API2_H

#include "ehs_types.h"

/** @file ehs_comp_api2.h
 * A redinition of some macros in ehs_fb_types.h
 *
 */



/*********************************************************************************************/
/* Function block definitions */

/* todo2023 - we need merge _API2 references from ehs_fb_types.h into this header*/
/* We can (probably) move the proviate bits of this into the above types e.g. EHS_FB_IN_API2(x) probably wont be used publicly in FBs */


/*********************************************************************************************/
/* Connection macros used by components
 *  * -----> index Starts at 1 !!!!!!!!
 *********************************************************************************************/

/**
 * Get input x - generic version - needs to be casted before use
 */
#define EHS_FB_IN_API2(x) EHS_FB_RUN_FUNCTION_INSTANCE->pIn[x-1]

/**
 * Check whether input x is connected or not
 */
#define EHS_FB_IN_CONNECTED_API2(x) ((EHS_FB_RUN_FUNCTION_INSTANCE->pIn) && (EHS_FB_IN_API2(x) != EhsDataConnectionTable.xDummyIn))

/**
 * Get input x as an integer
 */
#define EHS_FB_IN_I_API2(x) *(EhsDataflowIntType*)EHS_FB_IN_API2(x)

/**
 * Get input x as a float
 */
#define EHS_FB_IN_F_API2(x) *(EhsDataflowFloatType*)EHS_FB_IN_API2(x)

/**
 * Get input x as a bool
 */
#define EHS_FB_IN_B_API2(x) *(ehs_bool*)EHS_FB_IN_API2(x)

/**
 * Get input x as a string
 */
#define EHS_FB_IN_S_API2(x) (char*)EHS_FB_IN_API2(x)

/**
 * Check whether output x is connected or not
 */
#define EHS_FB_OUT_CONNECTED_API2(x) ((EHS_FB_RUN_FUNCTION_INSTANCE->pOut) && (EHS_FB_OUT_API2(x) != EhsDataConnectionTable.xDummy))

/**
 * Get output x - generic version
 */
#define EHS_FB_OUT_API2(x) (EHS_FB_RUN_FUNCTION_INSTANCE->pOut[x-1])

/**
 * Get output x as a bool
 */
#define EHS_FB_OUT_B_API2(x) *(ehs_bool*)EHS_FB_OUT_API2(x)

/**
 *  Get output x as an integer
 */
#define EHS_FB_OUT_I_API2(x) *(EhsDataflowIntType*)EHS_FB_OUT_API2(x)

/**
 * Get output x as a float
 */
#define EHS_FB_OUT_F_API2(x) *(EhsDataflowFloatType*)EHS_FB_OUT_API2(x)

/**
 * Get output x as a string
 */
#define EHS_FB_OUT_S_API2(x) (char*)EHS_FB_OUT_API2(x)

/**
 * Assert event x
 */
#define EHS_FB_FINISH_API2(x) EhsFunctionInstanceData_triggerEvent(EHS_FB_RUN_CONTEXT_REF,(x))

/*********************************************************************************************/


/* @todo Add all the paramter parsing functions as consistent Macros in here */
#endif /* EHS_COMP_API2_H */

