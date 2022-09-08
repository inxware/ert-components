/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file target_string-deleteme.h
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

/* These macros provide a mapping to the string.h file functions.
 * If the target cannot use the string.h version, the #define is removed from target_file,
 * and the declaration provided in hal_file is used instead
 */

#define EhsSprintf sprintf

#define EhsSscanf sscanf

#define EhsStrcpy strcpy

#define EhsStrcpy_s(dst,n,src) strcpy(dst,src)

#define EhsStrncpy_s(dst,n,src,len) strncpy(dst,src,len)

#define EhsStricmp stricmp

#define EhsStrnicmp strnicmp

#define EhsStrncmp strncmp

#define EhsStrcmp	strcmp

#define EhsStrlen strlen

#define EhsStrchr strchr

#define EhsStrstr strstr

#define EhsStrcat_s(dest,n,src) strcat(dest,src)

#define EhsMemset memset

#define EhsMemcpy memcpy

#define EhsStrIsSpace(x) isspace(x)

/*****************************************************************************/
/* Define types */

/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */


#endif /* EHS_TARGET_STRING_H */
