/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file base_types.h
 * In this file, all of the base-specifc type definitions required by EHS are given.
 * This file is normally used by being included by ehs_types.h.
 * This applies to the win_x86 target.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_BASE_FULL_TYPES_H
#define EHS_BASE_FULL_TYPES_H
/*lint -e(829) Project permitted derogation from the standard */
#include <stdio.h>

#include "globals.h"
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
#if EHS_FLOAT_AS_FLOAT_TYPE == 1
typedef float			ehs_float; /**< Standard EHS floating point type */
#define EHS_FL_FMT "%f" /**< scanf format string for ehs_float */
#else
typedef double			ehs_float; /**< Standard EHS floating point type */
#define EHS_FL_FMT "%lf" /**< scanf format string for ehs_float */
#endif

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

#ifndef EHS_MEMORY_ATTRIB
#define EHS_MEMORY_ATTRIB
#endif

#ifndef EHS_DATA_MEMORY_ATTRIB
#define EHS_DATA_MEMORY_ATTRIB
#endif//EHS_DATA_MEMORY_ATTRIB

#endif /* EHS_TARGET_TYPES_H */
