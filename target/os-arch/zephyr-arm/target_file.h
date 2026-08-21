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
 * Zephyr ARM target - file I/O goes through the POSIX fd layer
 * (open/read/write/lseek/close/fsync), NOT picolibc stdio. On Zephyr with
 * picolibc a FILE* is effectively console-only: fopen() on a real littlefs
 * file returns non-NULL but fgetc() reads nothing (immediate EOF), so the
 * SODL parser rejects a valid t.sdl as "Wrong SODL format". ehs_FILE is
 * therefore an fd-backed handle and every EhsF* op is a real function.
 * stdio is still pulled in for console printf / sscanf (string ops).
 *
 * @author: inx limited
 */

#ifndef EHS_TARGET_FILE_H
#define EHS_TARGET_FILE_H

/*****************************************************************************/
/* Included files */
#include <stdio.h>  /* printf / sscanf / vsnprintf / EOF only - NOT file FILE* */
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
/* Opaque fd-backed file handle. Callers only ever pass ehs_FILE* around
 * (EhsFopen -> ehs_FILE*, then EhsFgetc/EhsFclose/... take it); the struct
 * body lives in target_file.c. */
typedef struct EhsZephyrFileHandle ehs_FILE;

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

/* Console / string ops stay on picolibc (these work fine on Zephyr). */
#define Ehsprintf printf
#define EhsSscanf sscanf

/* File ops are fd-backed functions implemented in target_file.c.
 * EhsFopen/EhsFclose/EhsFgetc/EhsFgets/EhsFread/EhsFwrite/EhsFprintf/EhsFscanf/
 * EhsFflush are declared by hal_file.h (their #ifndef guards fire because no
 * macro is defined here) with the correct prototypes - do not re-declare them.
 *
 * EhsFseek/EhsFtell/EhsFrewind must be macro-routed here: hal_file.h's
 * fallback prototypes for these take ehs_FILE *by value* with 8-bit
 * offsets, which would truncate the (long) SODL file offsets the parser
 * uses. Route them to properly-typed functions instead. */
#define EhsFseek(f, off, whence) EhsZephyrFseek((f), (off), (whence))
#define EhsFtell(f) EhsZephyrFtell((f))
#define EhsFrewind(f) EhsZephyrFrewind((f))

ehs_sint32 EhsZephyrFseek(ehs_FILE *f, long offset, ehs_sint32 whence);
long EhsZephyrFtell(ehs_FILE *f);
ehs_sint32 EhsZephyrFrewind(ehs_FILE *f);

/* Not declared by hal_file.h and not used by common/kernel today, but keep a
 * correctly-typed fd-backed entry point in case a target component calls it. */
#define EhsFputc(c, f) EhsZephyrFputc((c), (f))
ehs_sint16 EhsZephyrFputc(ehs_sint16 c, ehs_FILE *f);

/* feof() needs a stdio FILE*; route to the fd-handle implementation. */
#define EhsFeof(f) EhsZephyrFeof((f))
ehs_bool EhsZephyrFeof(ehs_FILE *f);

ehs_bool EhsTgtFilesystem_Init(void);

#endif /* EHS_TARGET_FILE_H */
