/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_file.h
 * The target-specific declarations required to support the HAL for file
 * services are defined here. This file should only be included by hal_file.h
 *
 * Zephyr ARM target - uses POSIX-compatible file API when Zephyr's
 * CONFIG_POSIX_API / CONFIG_FILE_SYSTEM is enabled.
 *
 * @author: inx limited
 */

#ifndef EHS_TARGET_FILE_H
#define EHS_TARGET_FILE_H

/*****************************************************************************/
/* Included files */
#include <stdio.h>
#include "globals.h"

/*****************************************************************************/
/* Define macros  */

/* Skip stat-based operations (not all Zephyr FS backends support stat) */
#define EHS_TARGET_FILE_SKIP_STAT yes

/**
 * Path separator for this target
 */
#define EHS_TD_FILES_SEPARATOR '/'
#define EHS_TD_FILES_SEPARATOR_STR "/"

/*****************************************************************************/
/* Define types */
typedef FILE ehs_FILE; /**< File type used for file handling */

/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */

#ifndef EHS_TD_FILES_MAX_PATH
#define EHS_TD_FILES_MAX_PATH 128
#endif

#ifndef EHS_TD_FILES_MAX_FILENAME
#define EHS_TD_FILES_MAX_FILENAME 32
#endif

#define EHS_EOF EOF

#define EhsFopen(fname,fmode) fopen(fname,fmode)
#define EhsFclose(f) fclose(f)
#define EhsFgetc(f) fgetc(f)
#define EhsFprintf fprintf
#define Ehsprintf printf
#define EhsSscanf sscanf
#define EhsFscanf fscanf
#define EhsFgets fgets
#define EhsFflush(x) fflush(x)
#define EhsFseek(f, x, y) fseek(f, x, y)
#define EhsFrewind(f) rewind(f)
#define EhsFtell(f) ftell(f)
#define EhsFwrite(ptr, size, num, stream) fwrite(ptr, size, num, stream)
#define EhsFread(ptr, size, num, stream)                                       \
  (ehs_sint16) fread((void *)ptr, (size_t)size, (size_t)num, stream)
#define EhsFputc fputc

ehs_bool EhsTgtFilesystem_Init(void);

#endif /* EHS_TARGET_FILE_H */
