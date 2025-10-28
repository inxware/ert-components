/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_file.h
 * The target-specific declarations required to support the HAL for file services
 * are defined here. This file should only be included by hal_file.h
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_FILE_H
#define EHS_TARGET_FILE_H

/*****************************************************************************/
/* Included files */
#include <stdio.h> /* required for malloc, exit */
#include <sys/stat.h>
#include "globals.h"

/*****************************************************************************/
/* Define macros  */

/* These macros provide a mapping to the stdio file functions.
 * If the target cannot use the stdio version, the #define is removed from target_file,
 * and the declaration provided in hal_file is used instead
 */
#define EHS_EOF EOF /**< Defines end of file */

#define EhsFopen(fname,fmode) fopen(fname,fmode) /**< Open file. */

#define EhsFclose(f) fclose(f) /**< Close file. */

#define EhsFgetc(f) fgetc(f) /**< Get character from file. */

#define EhsFprintf fprintf /**< Print to file */

#define EhsFscanf fscanf /**< Read variables from file */

#define EhsFgets fgets /**< Read string from file */

#define EhsFflush(x) fflush(x)

/**
 * Write block of data to file
 */
#define EhsFwrite(ptr,size,num,stream) (ehs_sint16)fwrite((const void*)ptr, (size_t)size, (size_t)num, stream)

/**
 * Read block of data from file
 */
#define EhsFread(ptr,size,num,stream) (ehs_sint16)fread((void*)ptr, (size_t)size, (size_t)num, stream)

/**
 * Write character to file
 */
#define EhsFputc fputc

/*****************************************************************************/
/* Define types */

typedef FILE ehs_FILE; /**< File type used for file handling */

/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/


ehs_bool EhsTgtFilesystem_Init(void);

#endif /* EHS_TARGET_FILE_H */
