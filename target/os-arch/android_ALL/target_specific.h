/** @file target.h
 * In this file, all of the hardware abstraction layer functions provided for EHS are given.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_SPECIFIC_H
#define EHS_TARGET_SPECIFIC_H

#include <stdlib.h> /* required for malloc, exit */

#ifndef EHS_GLOBALS_H
#error "This file should only be included by globals.h" 
#endif


#include "globals.h"
#include "target_logger.h"

/**
 * Forces the declared variable to be aligned in the way specified by EHS_TGT_MEMORY_ALLOC_ALIGN
 */
#define EHS_TGT_MEMORY_ALIGNED(type,name) type name

#define EhsStdioPrintf(x,y,z,...)  __android_log_print(ANDROID_LOG_INFO,z,x,y,z,__VA_ARGS__) /* @TODO - review this function */
#define EhsStdioSimplePrintf(...) __android_log_print(ANDROID_LOG_INFO,/*TAG=*/"EHS",__VA_ARGS__)

/* math functions not implemented in Windows */

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
