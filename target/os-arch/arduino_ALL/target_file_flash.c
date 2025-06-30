/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file target_file.c
 * Functions supporting file usage on the SH4 platform.
 *
 * @author: inx limited
 *
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


//#define EHSL_MODULE_ID (EHSH_LOG_MODULE_HAL_FILE)

#include "messages.h"
#include "hal-api.h"

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */


/**
 * Contains the properties held about a directory
 */
struct EhsTDFilesStruct
{
    ehs_char szPath[EHS_TD_FILES_MAX_PATH];	/* the full "current path". Always begins with EHS_TD_FILES_ROOT */
    void* pDirectoryInfo;
    //EhsTDFileFlagEnum eSearch;					/* search types currently in force */
};


/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */

/*****************************************************************************/
/* Variables defined with global-scope */

/*****************************************************************************/
/* Function definitions */

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/**
 * Specifies where the root of the playback media folder can be found
 * NOTE: no trailing /
 */

/* @todo This file has a very inconcistent API with the generic player function block TD / TV functiontypes are not consistent. */



/**
 * Check for any errors and report them if appropriate
 */
#define EHS_TD_REPORT_ERRORS(err,func) if ((err) != ST_NO_ERROR) { EHSH_LOG_ERROR(EHS_MSG_TGT_STAPI(func,GetErrorText(err))); }

/**
 * Indicates that the search for files isn't currently taking place
 */
#define EHS_TD_FILES_SEARCH_INVALID -1

ehs_bool EhsTgtFilesystem_Init(void)
{
    
    return EHS_TRUE;
}

ehs_uint8 EhsTF_exists(const ehs_char* fname)
{
   
    return 0;  // Default case for other errors
}


/** Raw OS cd function */

ehs_bool EhsTF_cd(const ehs_char * dir)
{
    // @TODO - not supported create a fake root
    return EHS_FALSE;
}

ehs_bool EhsTF_mkdir(const char * szParameterFilePath)
{
    return EHS_FALSE;
}

/* return true if succesful */
ehs_bool EhsTF_rmdir(ehs_char* szPath)
{
    return EHS_FALSE;
}

/**
 * Initialise the EhsTDFilesClass structure. Sets the path to the root of the
 * media "filesystem" (i.e. EhsTDFiles_getDir returns "empty"). Resets any
 * searches.  @todo make this work for vlc
 */
void EhsTDFiles_init(struct EhsTDFilesStruct** pFiles)
{
    *pFiles = EhsHMem_tempAlloc(sizeof(struct EhsTDFilesStruct )); //this is cleared at the end of the app run by the garbage collector
    if (*pFiles)
    {
        (*pFiles)->pDirectoryInfo = NULL;
        //(*pFiles)->eSearch = EHS_TD_FILES_SEARCH_INVALID;
    }
}

/**
 * Cleanup - close the current directory
 * @param[in] pFiles Context for the file operation
 */
void EhsTDFiles_cleanup(EhsTDFilesClass* pFiles)
{
    // todo
}

/**
 * Return the directory we are curently looking at.
 * @param[in] pFiles Context for the file operation
 * @param[out] szDir Text containing the current path. Caller must pass in an array of length EHS_TD_FILES_MAX_PATH
 * @return true if successful
 */
ehs_bool EhsTDFiles_getDir(struct EhsTDFilesStruct* pFiles, ehs_char* szDir)
{
    return EHS_FALSE; // todo
}

/**
 * Change the directory we will use for file listing operations
 * @param[in] pFiles Context for the file operation
 * @param[in] szDir absolute Directory path
 * @return true if successful.
 */
ehs_bool EhsTDFiles_setDir(EhsTDFilesClass* pFiles, const ehs_char* szDir)
{
    return EHS_FALSE; // todo
}

/**
 * Start retrieving files from the current directory
 * @param[in] pFiles Context for the file operation
 * @param[out] szName Text containing the name of the retrieved object. Caller must pass in an array
 * of length EHS_TD_FILES_MAX_FILENAME
 * @return true if a file name has been obtained
 */
ehs_uint8 EhsTDFiles_listFirst(EhsTDFilesClass* pFiles, ehs_char* szName)
{
    return EHS_FALSE; // todo
}

/**
 * Continue retrieving files from the current directory. Keep calling this function until
 * it returns false.
 * @param[in] pFiles Context for the file operation
 * @param[out] szName Text containing the name of the retrieved object. Caller must pass in an array
 * of length EHS_TD_FILES_MAX_FILENAME
 * @return 0 for nothing, 1 for a file and 2 for a directory
 */
ehs_uint8 EhsTDFiles_listNext(EhsTDFilesClass* pFiles, ehs_char* szName)
{
    return EHS_FALSE; // todo
}


ehs_bool EhsTF_stat(ehs_char* szPath,struct stat *statbuf)
{
    return EHS_TRUE;
}

ehs_bool EhsTF_utime(ehs_char* szPath,struct utimbuf *new_times)
{
    //return (utime(szPath, new_times) == 0);
    return EHS_TRUE;
}
