/** @file target_string.h
 * The target-specific declarations required to support the HAL for string services
 * are defined here. This file should only be included by hal_string.h
 * 
 * @author: inx limited
 * @version: $Revision: 1547 $
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

/* These macros provide a mapping to the string.h functions.
 * If the target cannot use the string.h version, the #define is removed from target_string,
 * and the declaration provided in hal_string is used instead
 */
 
#define EhsSprintf sprintf

#define EhsSscanf sscanf

#define EhsStricmp _stricmp

#define EhsStrnicmp _strnicmp

#define EhsStrncmp strncmp

#define EhsStrcmp strcmp

#define EhsStrcpy strcpy

#define EhsStrcat strcat

#define EhsStrcat_s strcat_s

#define EhsStrchr strchr

#define EhsStrcpy_s strcpy_s

//#define EhsStrncpy_s strncpy_s 
#define EhsStrncpy_s(dst,n,src,len) strncpy(dst,src,len) //@todo check this is OK
#define EhsStrstr strstr

#define EhsStrlen strlen

#define EhsMemset memset

#define EhsMemcpy memcpy

#define EhsStrIsSpace(c) (ehs_bool)isspace((ehs_uint8)(c))
 
/*****************************************************************************/
/* Define types */

/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */


#endif /* EHS_TARGET_STRING_H */
