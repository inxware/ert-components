/** @file target_file.c
 * Functions supporting file usage on the win32 platform.
 *
 * @author: inx limited
 * @version: $Revision: 1994 $
 * @date: $Date$
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */


/**
 * @page Verification Verification report
 * @section hal_graphics_font
 * @anchor hal_graphics_font
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on 
 * Last modified on $Date:$
 *
 * This file contained no derogations to the MISRA standard.
 */

/*****************************************************************************/
/* Included files */

#include <sys/stat.h>
#include <direct.h> /* This is posix for directory listings */
#include <errno.h>
#include "messages.h"
#include "hal_file.h"

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */


/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */

/*****************************************************************************/
/* Variables defined with global-scope */

/*****************************************************************************/
/* Function definitions */


ehs_sint32 EhsTDFiles_countDirFiles ( const char *path ){

    return -1;
}

/**
 * Tests whether a file exists
 * @param fname Name of the file to test
 * @return true if the file exists
 */

ehs_bool EhsTF_exists(const ehs_char* fname) {
	struct _stat xFileInfo;
	ehs_bool bRet = EHS_TRUE;
	if (( _stat(fname,&xFileInfo) == -1) && (errno == ENOENT)) {
		bRet = EHS_FALSE;
	}
	return bRet;
}

ehs_bool EhsTF_cd(const ehs_char * dir) {
	return (chdir(dir)==0);
}

ehs_bool EhsTF_mkdir(char * szParameterFilePath) {
	return (mkdir(szParameterFilePath)!= 0);
}

