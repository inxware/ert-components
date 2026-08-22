/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_specific.h
 * Target-specific declarations for XMOS xcore.ai FreeRTOS targets.
 * Included by globals.h.
 *
 * @author: inx limited
 */

#ifndef EHS_TARGET_SPECIFIC_H
#define EHS_TARGET_SPECIFIC_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef EHS_GLOBALS_H
#error "This file should only be included by globals.h"
#endif

#define EHS_TGT_MEMORY_ALIGNED(type, name) type name

#ifndef EhsStdioPrintf
#define EhsStdioPrintf(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define EhsStdioSimplePrintf(...) printf(__VA_ARGS__)
#endif

ehs_sint32 EhsTgtInt_sin(ehs_sint32 nMult, ehs_sint32 nAngle);
ehs_sint32 EhsTgtInt_cos(ehs_sint32 nMult, ehs_sint32 nAngle);
ehs_float EhsTgtFloat_log10(ehs_float);
ehs_float EhsTgtFloat_loge(ehs_float);

#ifdef EHS_TARGET_CODE
void EhsTargetInitSharedMemory(ehs_bool bUniqueCheck);
#endif

#endif /* EHS_TARGET_SPECIFIC_H */
