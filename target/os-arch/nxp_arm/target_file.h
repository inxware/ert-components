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
#include "stdio.h"
#include "target_types.h"
#include "target_config.h"
/*****************************************************************************/
/* Define macros  */
#define EHS_EOF 0
#define EHS_TD_FILES_SEPARATOR '/'
#define EHS_TD_FILES_SEPARATOR_STR "/"
#define EHS_TD_FILES_MAX_FILENAME 128

#define EhsFprintf fprintf /**< Print to file */

#define EhsFscanf fscanf /**< Read variables from file */

#define EhsFflush(x) fflush(x) 

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
typedef FILE ehs_FILE;
struct stat {
    ehs_char a;
};
struct utimbuf {
    ehs_char a;
};

/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */

ehs_FILE* EhsFopen(const char* fname,const char* mode);
ehs_sint16 EhsFgetc(ehs_FILE* stream);
//ehs_char* EhsFgets(ehs_char* dst,ehs_sint16 max,ehs_FILE* fp);

#endif /* EHS_TARGET_FILE_H */
