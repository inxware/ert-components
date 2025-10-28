/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file dtv_pvr_list.c
 *
 * Implementation of the PVR file list functions for the DTV toolkit
 *
 * @author: inx limited
 *
 */

/**
 * @page FunctionReference Lucid Function Block Reference Guide
 * @section File system
 * @anchor
 * @subsection appliesTo Applies To:
 * DTV &gt; PVR &gt; List
 * @subsection properties Properties:
 * None
 * @subsection description Description:
 * This function block allows the user to search the current list of recorded programmes.
 *
 * Each time the start action is triggered, the "valid" port is set to indicate if it was
 * possible to get a filename. If "valid" is true, the string output indicates the name of the
 * file. If valid is false, it indicates that the end of the collection has been reached - the
 * next time the start action is triggered, the collection will start reading from the beginning.
 * If the directory changes then the process starts from the first entry identified in the new directory.
 */


#include "globals.h"
#include "fs_dir_create_remove.h"

#include "setCompletes.h"
#include "app_data.h"
#include "hal-api.h"


EHS_FB_FUNCTIONS_START(FileSystemDirCreateRemove)
EHS_FB_FUNCTION_ENTRY("create", 0x01, FileSystemDirCreateRemoveCreate)
EHS_FB_FUNCTION_ENTRY("remove", 0x02, FileSystemDirCreateRemoveRemove)
EHS_FB_FUNCTIONS_END

/**
 * Structure for maintaining information about the DTV PVR List
 */
struct FSDirCreateRemoveStruct
{
    ehs_char szPath[EHS_TD_FILES_MAX_FILENAME];	/**< The name of the last file read */
    ehs_bool bOnlyContents;	 						/**< True if szLastFile contains a filename */
};

/**
 * Define the identify function. No memory is required by this structure.
 */
EHS_FB_IDENTIFY_FUNCTION(FileSystemDirCreateRemove)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(struct FSDirCreateRemoveStruct);
}

/**
 * Point to the first programme on our list
 */
EHS_FB_INIT_FUNCTION(FileSystemDirCreateRemove)
{
    struct FSDirCreateRemoveStruct* pInfo = (struct FSDirCreateRemoveStruct*)EHS_FB_INIT_CONTEXT;
    //@todo we should include the width modifers in the string reader but it's a macro EHS_TD_FILES_MAX_FILENAME forgotten how to do this!
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%s%hhd",pInfo->szPath, &(pInfo->bOnlyContents));
    //EhsTDFiles_setDir(pListInfo->EhsTDFiles, pInfo->szPath);//only have one paramter so being lazy!
    return EHS_TRUE; /* initialisation succeeded */
}

EHS_FB_RUN_FUNCTION(FileSystemDirCreateRemoveCreate)
{
    ehs_bool ret;
    struct FSDirCreateRemoveStruct* pInfo =
        (struct FSDirCreateRemoveStruct*) EHS_FB_INIT_CONTEXT;

    if (EHS_FB_IN_CONNECTED(0))
    {
        EhsStrcpy(pInfo->szPath,EHS_FB_IN_S(0));
    }
    /* don't allow creation of anything lower than /???/ */
    if (EhsStrlen(pInfo->szPath) > 15 && EhsStrncmp(pInfo->szPath, EHS_FILE_LOCALHOST_PREFIX, EhsStrlen(EHS_FILE_LOCALHOST_PREFIX)) == 0)   // allow abolute path to local host
    {
        ret = EhsTF_mkdir(&pInfo->szPath[10]);
    }
    else
    {
        ret = Ehs_UserMkdir(pInfo->szPath);
    }
    if (ret)
        EHS_FB_FINISH(1);
    else
    {
        EHS_FB_FINISH(2); //doesn't exist
        EHSH_LOG_WARNING("Couldn't create directory");
    }
}

EHS_FB_RUN_FUNCTION(FileSystemDirCreateRemoveRemove)
{
    ehs_bool ret;
    struct FSDirCreateRemoveStruct* pInfo = (struct FSDirCreateRemoveStruct*)EHS_FB_INIT_CONTEXT;
    if (EHS_FB_IN_CONNECTED(0))
    {
        EhsStrcpy(pInfo->szPath,EHS_FB_IN_S(0));
    }
    //@tod need to check bOnlyContents and send a flag to UserRmdir
    /* don't allow creation of anything lower than /???/ */
    if (EhsStrlen(pInfo->szPath) > 15 && EhsStrncmp(pInfo->szPath, EHS_FILE_LOCALHOST_PREFIX, EhsStrlen(EHS_FILE_LOCALHOST_PREFIX)) == 0)   // allow abolute path to local host
    {
        ret = EhsTF_rmdir(&pInfo->szPath[10]);
    }
    else
    {
        ret = Ehs_UserRmdir(pInfo->szPath);
    }
    if (ret)
    {
        EHS_FB_FINISH(1);
    }
    else
    {
        EHS_FB_FINISH(2); // perhaps doesn't exist or some permission problem possibly - it's not gone
    }
}




