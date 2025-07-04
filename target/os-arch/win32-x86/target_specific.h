/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_specific.h
 * In this file, all of the hardware abstraction layer functions provided for EHS are given.
 * Applies to win_x86 target.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_SPECIFIC_H
#define EHS_TARGET_SPECIFIC_H

#include <stdlib.h> /* required for malloc, exit */

/**
 * Whenever a pointer is accessed from a ROM data table, it should
 * be handled through this function
 */
#define EHS_TGT_ROM_PTR(type,x) (type)(x)
/**
 * Forces the declared variable to be aligned in the way specified by EHS_TGT_MEMORY_ALLOC_ALIGN
 */
#define EHS_TGT_MEMORY_ALIGNED(type,name) type name

#define EhsStdioPrintf(x) printf(#x)
#define EhsStdioSimplePrintf(x) printf(#x)

#define EhsTargetExit exit

/**
 * Performs sine function using integers.
 *
 * @param nMult multiplier for output
 * @param nAngle angle to find (0 to 360)
 * @return nMult * sin(nAngle % 360)
 */
EHS_GLOBAL ehs_sint32 EhsTgtInt_sin(ehs_sint32 nMult, ehs_sint32 nAngle);
/**
 * Performs cosine function using integers.
 *
 * @param nMult multiplier for output
 * @param nAngle angle to find (0 to 360)
 * @return nMult * cos(nAngle % 360)
 */
EHS_GLOBAL ehs_sint32 EhsTgtInt_cos(ehs_sint32 nMult, ehs_sint32 nAngle);

/* The following functions are also onlu used by function blocks and should be seperated into another target support header
 * designed for FB development
 */

/* math functions not implemented in Windows */
EHS_GLOBAL ehs_float asinh(ehs_float x);
EHS_GLOBAL ehs_float acosh(ehs_float x);
EHS_GLOBAL ehs_float atanh(ehs_float x);

/*****************************************************************************/
/* Internal implementation-specific functions. These functions are only used
 * by other functions within this target, and aren't part of the global scope
 * of EHS
 */

/**
 * Main function handling TCP/IP for this implementation
 */
EHS_GLOBAL ehs_uint32 EhsTgtTcp(void);


#endif /* EHS_TARGET_SPECIFIC_H */
