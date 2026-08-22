/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_file.h
 * Target-specific declarations for the file services HAL.
 * This file should only be included by hal_file.h
 *
 * XMOS xcore.ai / FreeRTOS target — uses the POSIX file API provided
 * by fwk_rtos.  All standard stdio/POSIX calls are available once
 * EhsTgtFilesystem_Init() returns EHS_TRUE.
 */

#ifndef EHS_TARGET_FILE_H
#define EHS_TARGET_FILE_H

/* The XMOS libc / fwk_rtos POSIX layer does not provide <utime.h>, so the
 * stat/utime functions in hal_file.[ch] would be declared with `struct utimbuf*`
 * and defined the same way — but `struct utimbuf` is never declared, leading
 * to "conflicting types for 'Ehs_UserUtime'" at hal_file.c:735. Match the
 * esp32 / zephyr precedent and switch hal_file to the void* stub branch. */
#define EHS_TARGET_FILE_SKIP_STAT yes

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ehs_types.h>

/**
 * Path separator for this target
 */
#define EHS_TD_FILES_SEPARATOR     '/'
#define EHS_TD_FILES_SEPARATOR_STR "/"

/**
 * Longest possible path name (excluding filename)
 */
#ifndef EHS_TD_FILES_MAX_PATH
#define EHS_TD_FILES_MAX_PATH 256
#endif

/**
 * Longest possible filename
 */
#ifndef EHS_TD_FILES_MAX_FILENAME
#define EHS_TD_FILES_MAX_FILENAME 64
#endif

typedef FILE ehs_FILE;

#define EHS_EOF EOF

#define EhsFopen(fname, fmode)           fopen(fname, fmode)
#define EhsFclose(f)                     fclose(f)
#define EhsFgetc(f)                      fgetc(f)
#define EhsFprintf                       fprintf
#define Ehsprintf                        printf
#define EhsSscanf                        sscanf
#define EhsFscanf                        fscanf
#define EhsFgets                         fgets
#define EhsFflush(x)                     fflush(x)
#define EhsFseek(f, x, y)               fseek(f, x, y)
#define EhsFtell(f)                      ftell(f)
#define EhsFrewind(f)                    rewind(f)
#define EhsFwrite(ptr, size, num, stream) fwrite((const void *)(ptr), (size_t)(size), (size_t)(num), stream)
#define EhsFread                         fread
#define EhsFputc                         fputc
#define EhsFsync                         sync
#define EhsFfsync                        fsync

ehs_bool EhsTgtFilesystem_Init(void);

#endif /* EHS_TARGET_FILE_H */
