/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file target_specific.h
 * In this file, all of the hardware abstraction layer functions provided for EHS are given.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_SPECIFIC_H
#define EHS_TARGET_SPECIFIC_H

#include <stdlib.h> /* required for malloc, exit */

#include "target_logger.h"

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

EHS_GLOBAL ehs_float EhsTgtFloat_log10(ehs_float);
EHS_GLOBAL ehs_float EhsTgtFloat_loge(ehs_float);

#ifdef EHS_TARGET_CODE
/**
 * Initialise shared memory. Called by the main process and the tcpip process.
 *
 * @param[in] bUniqueCheck If true, check to ensure that this is the only instance that currently
 * holds the shared memory.
 */
EHS_GLOBAL void EhsTargetInitSharedMemory(ehs_bool bUniqueCheck);

#endif /* EHS_TARGET_CODE */

#endif /* EHS_TARGET_SPECIFIC_H */
