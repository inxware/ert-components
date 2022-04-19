/** @file target_types.h
 * In this file, all of the target-specifc type definitions required by EHS are given.
 * This file is normally used by being included by ehs_types.h.
 * This applies to the win_x86 target.
 * 
 * @author: inx limited
 * @version: $Revision: 1261 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

#ifndef EHS_BASE_FULL_TYPES_H
#define EHS_BASE_FULL_TYPES_H
/*lint -e(829) Project permitted derogation from the standard */
#include <stdio.h>
/*line +e(829) */

/* standard types */
typedef unsigned long long		ehs_uint64; /**< standard 32-bit unsigned type */
typedef long long			ehs_sint64; /**< standard 32-bit signed type */
typedef unsigned int	ehs_uint32; /**< standard 32-bit unsigned type */
typedef int			ehs_sint32; /**< standard 32-bit signed type */
typedef unsigned short	ehs_uint16; /**< standard 16-bit unsigned type */
typedef short			ehs_sint16; /**< standard 16-bit signed type */
typedef unsigned char	ehs_uint8; /**< standard 8-bit unsigned type */
typedef char			ehs_sint8; /**< standard 8-bit signed type */
typedef unsigned char	ehs_bool; /**< Standard boolean type */
typedef char			ehs_char; /**< Standard char type */
typedef double			ehs_float; /**< Standard EHS floating point type */

typedef int			EhsThreadFuncReturnType; /**< Type to return from a thread function */


#ifdef EHS_TARGET_CODE
/*
 * Provide a redeclaration of int type to allow casting to target-specific int type
 * without breaking MISRA 2004 rule 6.3: Use of type 'int' outside a typedef
 */
#ifndef EhsTargetIntType
typedef int EhsTargetIntType;
#endif

#endif /* EHS_TARGET_CODE */


#define EHS_TRUE	1	/**< allows target-specific boolean types to be used */
#define EHS_FALSE	0	/**< allows target-specific boolean types to be used */

#ifdef __cplusplus
#define EHS_GLOBAL extern "C" /**< allows target-specific definition of global (i.e extern) types */
#else
#define EHS_GLOBAL extern
#endif

#ifdef EHS_UNIT_TEST
#ifndef EHS_LOCAL
#define EHS_LOCAL
#endif
#else
#ifndef EHS_LOCAL
#define EHS_LOCAL static /**< allows target-specific definition of local (i.e. static) types */
#endif
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

#endif /* EHS_TARGET_TYPES_H */
