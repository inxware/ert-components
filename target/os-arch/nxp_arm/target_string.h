/** @file target_string.h
 * The target-specific declarations required to support the HAL for string services
 * are defined here. This file should only be included by hal_string.h
 * 
 * @author: inx limited
 * @version: $Revision: 1815 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
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

#define EhsSnprintf snprintf

#define EhsSscanf sscanf

#define EhsStrcpy strcpy


#define EhsStrncpy strncpy

#define EhsStrcpy_s(dst,n,src) strcpy(dst,src)

#define EhsStrncpy_s(dst,n,src,len) strncpy(dst,src,len)

#define EhsStricmp strcasecmp

#define EhsStrnicmp strncasecmp

#define EhsStrncmp strncmp

#define EhsStrcmp	strcmp

#define EhsStrcat strcat

#define EhsStrlen strlen

#define EhsStrchr strchr

#define EhsStrstr strstr

#define EhsStrchr strchr

#define EhsStrnstr strstr

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
