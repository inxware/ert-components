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
 * @author: inx limited
 *
 */

/**
 * @page Verification Verification report
 * @section hal_file_h
 * @anchor hal_file_h
 * @subsection misra MISRA compliance:
 * Demonstrated MISRA compliant on
 * Last modified on $Date$
 *
 * This file contained the following derogations to the MISRA standard.
 * - Rule 20.9 "Stdio.h shall not be used in production code". Acceptable to use
 * stdio.h on this platform
 */

#ifndef EHS_TARGET_FILE_H
#define EHS_TARGET_FILE_H

/*****************************************************************************/
/* Included files */
//#include "hal.h" // Note - target_file.h must be included specifically - not part of hal.h
#include <stdio.h>
#include "globals.h"
//#include "target_config.h"
//#include "globals.h"
/*****************************************************************************/
/* Define macros  */

#ifndef EHS_TARGET_FILE_SKIP_STAT
#define EHS_TARGET_FILE_SKIP_STAT yes
#endif


/* If we want to use the super small flash file system durectory free files
 * #define this: */
#undef EHS_USE_SIMPLE_FILESYSTEM
#ifdef EHS_USE_SIMPLE_FILESYSTEM

#define EHS_EOF EOF
#define EHS_TD_FILES_SEPARATOR '/'
#define EHS_TD_FILES_SEPARATOR_STR "/"

#define CONFIG_FLASH_SECTOR_SIZE_BYTES (4096)
#define CONFIG_EHS_MAX_SODL_SECTORS 2

/*****************************************************************************/
/* Define types */
struct stat
{
    ehs_char a;
};
struct utimbuf
{
    ehs_char a;
};

typedef struct
{
    char *filename;
    uint16_t sizeBytes;
} inxEHSFile_t;

/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */

ehs_FILE *EhsFopen(const char *fname, const char *mode);
ehs_sint16 EhsFgetc(ehs_FILE *stream);
// ehs_char* EhsFgets(ehs_char* dst,ehs_sint16 max,ehs_FILE* fp);

#else
/* Use normal libc file system (note this could be a #include of the gnu target.h)*/
/**
 * Path separator for this target
 */
#ifndef EHS_MINGW
#define EHS_TD_FILES_SEPARATOR '/'
#define EHS_TD_FILES_SEPARATOR_STR "/"
#else
#define EHS_TD_FILES_SEPARATOR '\\'
#define EHS_TD_FILES_SEPARATOR_STR "\\"
#endif/*****************************************************************************/

/* Define types */
typedef FILE ehs_FILE; /**< File type used for file handling */
//#define ehs_FILE FILE

/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */

// assume we have a standard lib for these
#ifndef EHS_TD_FILES_MAX_PATH 
#define EHS_TD_FILES_MAX_PATH 128
#endif

/**
 * Longest possible filename
 */
#ifndef EHS_TD_FILES_MAX_FILENAME
#define EHS_TD_FILES_MAX_FILENAME 128
#endif

/* These macros provide a mapping to the stdio file functions.
 * If the target cannot use the stdio version, the #define is removed from target_file,
 * and the declaration provided in hal_file is used instead
 */
#define EHS_EOF EOF /**< Defines end of file */

#define EhsFopen(fname,fmode) fopen(fname,fmode) /**< Open file. */

#define EhsFclose(f) fclose(f) /**< Close file. */

#define EhsFgetc(f) fgetc(f) /**< Get character from file. */

#define EhsFprintf fprintf /**< Print to file */

#define Ehsprintf printf /**< Print to file */

#define EhsSscanf sscanf /**< Read variables from string */

#define EhsFscanf fscanf /**< Read variables from file */

#define EhsFgets fgets /**< Read string from file */

#define EhsFflush(x) fflush(x)

#define EhsFseek(f, x, y) fseek(f, x, y)

#define EhsFrewind(f) rewind(f)

#define EhsFtell(f) ftell(f)
/**
 * Write block of data to file
 */
#define EhsFwrite(ptr, size, num, stream) fwrite(ptr, size, num, stream)

/**
 * Read block of data from file
 */
#define EhsFread(ptr, size, num, stream)                                       \
  (ehs_sint16) fread((void *)ptr, (size_t)size, (size_t)num, stream)

/**
 * Write character to file
 */
#define EhsFputc fputc
#endif

ehs_bool EhsTgtFilesystem_Init(void);
#endif /* EHS_TARGET_FILE_H */
