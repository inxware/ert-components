/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_string.h
 * Target-specific string function mappings for XMOS xcore.ai FreeRTOS targets.
 * Should only be included by hal_string.h.
 *
 * @author: inx limited
 */

#ifndef EHS_TARGET_STRING_H
#define EHS_TARGET_STRING_H

#ifndef EHS_HAL_STRING_H
#error "This file should only be included by hal_string.h"
#endif

#include <ctype.h>
#include <string.h>
/* strings.h not provided by XMOS xcore libc; strcasecmp/strncasecmp are
 * available in <string.h> on this target. */

#define EhsSprintf        sprintf
#define EhsSnprintf       snprintf
#define EhsSscanf         sscanf
#define EhsVsnprintf      vsnprintf
#define EhsStrcpy         strcpy
#define EhsStrncpy        strncpy
#define EhsStrcpy_s(dst, n, src)          strcpy(dst, src)
#define EhsStrncpy_s(dst, n, src, len)    strncpy(dst, src, len)
#define EhsStricmp        strcasecmp
#define EhsStrnicmp       strncasecmp
#define EhsStrncmp        strncmp
#define EhsStrcmp         strcmp
#define EhsStrcat         strcat
#define EhsStrncat        strncat
#define EhsStrlen         strlen
#define EhsStrchr         strchr
#define EhsStrstr         strstr
#define EhsStrnstr        strstr
#define EhsStrcat_s(dest, n, src)         strcat(dest, src)
#define EhsMemset         memset
#define EhsMemcpy         memcpy
#define EhsMemchr         memchr
#define EhsStrIsSpace(x)  isspace(x)

#endif /* EHS_TARGET_STRING_H */
