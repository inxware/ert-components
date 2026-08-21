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

#include "globals.h"

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
 * Check whether input x is connected or not.
 * Bounds-checked against nNumInputs (the count the SODL actually parsed for
 * this function instance) — a FB rebuilt against a newer CDF can probe a
 * higher argument than an older exported app ever declared; without this,
 * that reads past the pIn array instead of reporting "not connected".
 */
#define EHS_FB_IN_CONNECTED_API2(x)                                               \
    ((EHS_FB_RUN_FUNCTION_INSTANCE->pIn) &&                                      \
     ((x) <= EHS_FB_RUN_FUNCTION_INSTANCE->nNumInputs) &&                        \
     (EHS_FB_IN_API2(x) != EhsDataConnectionTable.xDummyIn))

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
 * Check whether output x is connected or not.
 * Bounds-checked against nNumOutputs — see EHS_FB_IN_CONNECTED_API2.
 */
#define EHS_FB_OUT_CONNECTED_API2(x)                                              \
    ((EHS_FB_RUN_FUNCTION_INSTANCE->pOut) &&                                     \
     ((x) <= EHS_FB_RUN_FUNCTION_INSTANCE->nNumOutputs) &&                       \
     (EHS_FB_OUT_API2(x) != EhsDataConnectionTable.xDummy))

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

/*
 * Bounded access to string ports - see the equivalent block in ehs_fb_types.h.
 * String connections are variable length, so neither EHS_STRING_LENGTH_MAX nor
 * EHS_DATA_TABLE_STRING_DEFAULT_LENGTH is a safe bound for a write.
 */

/** Capacity of string output x in bytes including the NUL, 0 if not a string port */
#define EHS_FB_OUT_S_CAP_API2(x) EhsDataString_capacity(EHS_FB_OUT_S_API2(x))

/** Longest string that fits in output x, excluding the NUL */
#define EHS_FB_OUT_S_MAXLEN_API2(x) EhsDataString_maxLen(EHS_FB_OUT_S_API2(x))

/** Length of the string currently in output x, bounded by its capacity */
#define EHS_FB_OUT_S_LEN_API2(x) EhsDataString_len(EHS_FB_OUT_S_API2(x))

/** Write src to string output x, truncating to fit. Always NUL terminates. */
#define EHS_FB_OUT_S_SET_API2(x,src) EhsDataString_set(EHS_FB_OUT_S_API2(x),(src))

/** As EHS_FB_OUT_S_SET_API2 but takes at most n characters from src */
#define EHS_FB_OUT_S_SETN_API2(x,src,n) EhsDataString_setN(EHS_FB_OUT_S_API2(x),(src),(n))

/** Append src to string output x, truncating to fit */
#define EHS_FB_OUT_S_CAT_API2(x,src) EhsDataString_append(EHS_FB_OUT_S_API2(x),(src))

/** Capacity of string input x in bytes including the NUL */
#define EHS_FB_IN_S_CAP_API2(x) EhsDataString_capacity(EHS_FB_IN_S_API2(x))

/** Length of the string on input x, bounded by its capacity */
#define EHS_FB_IN_S_LEN_API2(x) EhsDataString_len(EHS_FB_IN_S_API2(x))

/**
 * Assert event x
 */
#define EHS_FB_FINISH_API2(x) EhsFunctionInstanceData_triggerEvent(EHS_FB_RUN_CONTEXT_REF,(x))

/**
 * Check whether finish port x is connected (i.e. has a trigger registered for
 * it in the SODL graph). Note that EHS_FB_FINISH / EHS_FB_FINISH_API2 already
 * silently handle unconnected ports inside the kernel — this macro is for FBs
 * that want to skip the call (and any expensive work that precedes it) when
 * the port is not wired up.
 */
#define EHS_FB_FINISH_CONNECTED_API2(x)                                            \
    ((EHS_FB_RUN_FUNCTION_INSTANCE->pFinishPort) &&                                \
     ((x) <= EHS_FB_RUN_FUNCTION_INSTANCE->nFinishPorts) &&                        \
     (EHS_FB_RUN_FUNCTION_INSTANCE->pFinishPort[(x)-1] != NULL))

/*********************************************************************************************/


/* @todo Add all the paramter parsing functions as consistent Macros in here */
#endif /* EHS_COMP_API2_H */

