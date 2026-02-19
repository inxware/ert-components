/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_specific.h
 * Zephyr ARM target-specific declarations.
 * This file should only be included by globals.h
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_SPECIFIC_H
#define EHS_TARGET_SPECIFIC_H

#ifndef EHS_GLOBALS_H
#error "This file should only be included by globals.h"
#endif

#include <stdlib.h> /* required for malloc, exit */
#include <string.h>

/**
 * Forces the declared variable to be aligned in the way specified by
 * EHS_TGT_MEMORY_ALLOC_ALIGN
 */
#define EHS_TGT_MEMORY_ALIGNED(type, name) type name

#ifndef EHSStdioPrintf
    #define EhsStdioPrintf(x, ...) printf(x, __VA_ARGS__)
    #define EhsStdioSimplePrintf(...) printf(__VA_ARGS__)
#endif

/* math functions */

/**
 * Performs sine function using integers.
 *
 * @param nMult multiplier for output
 * @param nAngle angle to find (0 to 360)
 * @return nMult * sin(nAngle % 360)
 */
ehs_sint32 EhsTgtInt_sin(ehs_sint32 nMult, ehs_sint32 nAngle);
/**
 * Performs cosine function using integers.
 *
 * @param nMult multiplier for output
 * @param nAngle angle to find (0 to 360)
 * @return nMult * cos(nAngle % 360)
 */
ehs_sint32 EhsTgtInt_cos(ehs_sint32 nMult, ehs_sint32 nAngle);

ehs_float EhsTgtFloat_log10(ehs_float);
ehs_float EhsTgtFloat_loge(ehs_float);

#ifdef EHS_TARGET_CODE
/**
 * Initialise shared memory. Called by the main process and the tcpip process.
 * On Zephyr MCU targets this is typically a no-op (all memory is shared).
 *
 * @param[in] bUniqueCheck If true, check to ensure that this is the only
 * instance that currently holds the shared memory.
 */
void EhsTargetInitSharedMemory(ehs_bool bUniqueCheck);

#endif /* EHS_TARGET_CODE */

#endif /* EHS_TARGET_SPECIFIC_H */
