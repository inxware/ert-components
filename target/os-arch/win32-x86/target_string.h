/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_string.h
 * The target-specific declarations required to support the HAL for string services
 * are defined here. This file should only be included by hal_string.h
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_STRING_H
#define EHS_TARGET_STRING_H

#ifndef EHS_HAL_STRING_H
#error "This file should only be included by hal_string.h"
#endif

/*****************************************************************************/
/* Included files */

#include <string.h>
#include <ctype.h>

/*****************************************************************************/
/* Define macros  */

/* These macros provide a mapping to the string.h functions.
 * If the target cannot use the string.h version, the #define is removed from target_string,
 * and the declaration provided in hal_string is used instead
 */

#define EhsSprintf sprintf

/* Every other target maps this in its own target_string.h; without it here the
 * hal_string.h fallback declaration is used, and that one takes no size
 * argument - so a bounded EhsSnprintf(dst, cap, fmt, ...) call would silently
 * treat cap as the format string. */
#define EhsSnprintf snprintf

#define EhsSscanf sscanf

#define EhsVsnprintf vsnprintf

#define EhsStricmp _stricmp

#define EhsStrnicmp _strnicmp

#define EhsStrncmp strncmp

#define EhsStrcmp strcmp

#define EhsStrcpy strcpy

#define EhsStrcat strcat

#define EhsStrncat strncat

#define EhsStrcat_s strcat_s

#define EhsStrchr strchr

#define EhsStrcpy_s strcpy_s

//#define EhsStrncpy_s strncpy_s
#define EhsStrncpy_s(dst,n,src,len) strncpy(dst,src,len) //@todo check this is OK
#define EhsStrstr strstr

#define EhsStrlen strlen

#define EhsMemset memset

#define EhsMemcpy memcpy

#define EhsMemchr memchr

#define EhsStrIsSpace(c) (ehs_bool)isspace((ehs_uint8)(c))

/*****************************************************************************/
/* Define types */

/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */


#endif /* EHS_TARGET_STRING_H */
