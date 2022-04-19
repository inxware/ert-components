/** @file target_file.h
 * The target-specific declarations required to support the HAL for file services
 * are defined here. This file should only be included by hal_file.h
 *
 * @author: inx limited
 * @version: $Revision: 2662 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 *
 * Copyright (c) inx limited, 2006. All rights reserved.
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
 * - Rule 20.9 "Stdio.h shall not be used in production code". Acceptable to use stdio.h on this platform
 */


#ifndef EHS_TARGET_FILE_H
#define EHS_TARGET_FILE_H

/*****************************************************************************/
/* Included files */
#include <stdio.h> /*lint !e829 required for malloc, exit */
#include <sys/stat.h>
#include <utime.h>
#include <fcntl.h>
#include <unistd.h>


/*****************************************************************************/
/* Define macros  */

struct EhsTDFilesStruct ;

/**
 * Longest possible path name (excluding filename, but including separators and
 * terminating zero).
 */
#define EHS_TD_FILES_MAX_PATH 1024

/**
 * Longest possible filename
 */
#define EHS_TD_FILES_MAX_FILENAME 512

/**
 * Path separator for this target
 */
#ifndef EHS_MINGW
#define EHS_TD_FILES_SEPARATOR '/'
#define EHS_TD_FILES_SEPARATOR_STR "/"
#else
#define EHS_TD_FILES_SEPARATOR '\\'
#define EHS_TD_FILES_SEPARATOR_STR "\\"
#endif

//#warning TODO FILE seperator needs to be made target specific.


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

/*Added by PB 23/07/2008 for reading SVG files. Possibly there is a better way of file reading that doesn't need these*/
#define EhsFseek(f,x,y) fseek(f,x,y)

#define EhsFtell(f) ftell(f)
/**
 * Write block of data to file
 */
#define EhsFwrite(ptr, size, num, stream) fwrite((const void*)ptr, (size_t)size, (size_t)num, stream)




/**
 * Read block of data from file
 */
#define EhsFread fread

/**
 * Write character to file
 */
#define EhsFputc fputc

/* Directory functions */

/* file system sync functions */


#ifndef EHS_MINGW
#define EhsFsync sync

#define EhsFfsync fsync
#else

#define EhsFsync (...)

#define EhsFfsync fsync(...)
#endif


/*****************************************************************************/
/* Define types */

//typedef FILE ehs_FILE; /**< File type used for file handling */
#define ehs_FILE FILE

/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */




#endif /* EHS_TARGET_FILE_H */
