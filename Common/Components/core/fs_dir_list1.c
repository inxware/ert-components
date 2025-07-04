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
 * @section File System
 * @anchor Listing
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
 *
 *
 * Add to docs
 * absolute paths using / are allowed ( & local:// secretly)
 */

#include "target.h"
#include "fs_dir_list1.h"
#include "globals.h"
#include "setCompletes.h"
#include "app_data.h"
#include "hal-api.h"


EHS_FB_FUNCTIONS_START(FileSystemDirList1)
EHS_FB_FUNCTION_ENTRY("set", 0x01, FileSystemDirSet1)
EHS_FB_FUNCTION_ENTRY("get", 0x02, FileSystemDirGet1)
EHS_FB_FUNCTION_ENTRY("rel", 0x03, FileSystemDirRelease1)
EHS_FB_FUNCTIONS_END

/**
 * Structure for maintaining information about the DTV PVR List
 */
struct FSDirListStruct
{
    ehs_char szLastFile[EHS_TD_FILES_MAX_FILENAME];	/**< The name of the last file read */
    ehs_bool bFileValid;	 						/**< True if szLastFile contains a filename */
    ehs_char azCurrentDir[EHS_TD_FILES_MAX_FILENAME]; /* This is the relateive path to be found */
    EhsTDFilesClass * EhsTDFiles;
    ehs_bool bAppDirectory;
    //ehs_bool bFindDirs; /* don't need this */
};

/**
 * Define the identify function. No memory is required by this structure.
 */
EHS_FB_IDENTIFY_FUNCTION(FileSystemDirList1)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(struct FSDirListStruct);
}

/**
 * Point to the first programme on our list
 */
EHS_FB_INIT_FUNCTION(FileSystemDirList1)
{
    struct FSDirListStruct* pListInfo = (struct FSDirListStruct*)EHS_FB_INIT_CONTEXT;
    EhsTDFiles_init(&(pListInfo->EhsTDFiles));
    pListInfo->bAppDirectory=EHS_FALSE;
    //ehs_uint8 exists=0;
    //EhsSscanf(EHS_FB_INIT_PARAMETERS,"%s%hhd%hhd",pListInfo->azCurrentDir, &pListInfo->bFindFiles, &pListInfo->bFindDirs);
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%s%hhd",pListInfo->azCurrentDir, &pListInfo->bAppDirectory);
    if (EhsStrcmp(pListInfo->azCurrentDir,"NULL")==0)
    {
        EhsStrcpy(pListInfo->azCurrentDir,"");
    }
    /* Don't try and look for files here, the path maybe dynamic */
    pListInfo->bFileValid = EHS_FALSE; /* this tells the get entry to set all up */
    return EHS_TRUE; /* initialisation succeeded */
}

/**
 * teardown
 */
EHS_FB_DESTROY_FUNCTION(FileSystemDirList1)
{
    struct FSDirListStruct* pListInfo = (struct FSDirListStruct*)EHS_FB_DESTROY_CONTEXT;
    EhsTDFiles_cleanup(pListInfo->EhsTDFiles);
    return EHS_TRUE;
}


EHS_FB_RUN_FUNCTION(FileSystemDirRelease1)
{
    struct FSDirListStruct* pListInfo = (struct FSDirListStruct*)EHS_FB_RUN_CONTEXT;
    EhsTDFiles_cleanup(pListInfo->EhsTDFiles);
}

/**
 * Get an entry from the catalogue of pre-recorded programmes.
 */
EHS_FB_RUN_FUNCTION(FileSystemDirGet1)
{
    struct FSDirListStruct* pListInfo = (struct FSDirListStruct*)EHS_FB_RUN_CONTEXT;
    ehs_char abs_path[EHS_TD_FILES_MAX_FILENAME]; /* Absolute path we build here */
    //ehs_char rel_canonical_path[EHS_TD_FILES_MAX_FILENAME];
    ehs_uint8 ret = 0;
    ehs_uint8 pathret = 0;
    ehs_bool bPathOK=EHS_TRUE;

    if (!pListInfo->bFileValid)   /* first time for this directory, so check the path now and intialise the interator */
    {
        /* Create the absolute path and check it */
        if (pListInfo->EhsTDFiles)   /* idiot check - should always be true here */
        {
            if (EhsStrncmp(pListInfo->azCurrentDir, EHS_FILE_LOCALHOST_PREFIX,EhsStrlen(EHS_FILE_LOCALHOST_PREFIX)) == 0)   // allow abolute path to local host
            {
                EhsStrcpy(abs_path,&pListInfo->azCurrentDir[EhsStrlen(EHS_FILE_LOCALHOST_PREFIX)]);
            }
            else     /* Look in a canonical directory */
            {
                /* pmake full path from application directory */
                if (pListInfo->bAppDirectory)
                {
                    EhsHMetagetCurrentAppDir(abs_path);
                    EhsStrcat(abs_path,EHS_TD_FILES_SEPARATOR_STR);
                    EhsStrcat(abs_path,pListInfo->azCurrentDir);
                    EhsStrcat(abs_path,EHS_TD_FILES_SEPARATOR_STR);
                }
                else   /* make the full path in user directory to check */
                {
                    EhsTF_tryCanonicPath(abs_path,EHS_RUNTIME_USERDATA_DIR, pListInfo->azCurrentDir,EHS_TRUE);
                }
            }
            /* Check the path is OK */
            pathret = EhsTF_exists(abs_path);
            if (pathret == 2)   /* This is a directory, not a file, or nothing at all */
            {
                EhsTDFiles_setDir(pListInfo->EhsTDFiles, abs_path);
                //bPathOK=EHS_TRUE; /*note: not needed we default to path OK */
            }
            else
            {
                bPathOK=EHS_FALSE;
                pListInfo->bFileValid = EHS_FALSE;
            }
        }
        else
        {
            EHSH_LOG_ERROR(" Directory handle not valid");
        }
        if (bPathOK)
        {
            ret= EhsTDFiles_listFirst(pListInfo->EhsTDFiles,pListInfo->szLastFile);
        }
        else
        {
            ret=0;
            pListInfo->bFileValid=EHS_FALSE; /* this will force a retry on current directory next time */
            EHSH_LOG_WARNING("Couldn't open %s",abs_path);
            //if(pListInfo->EhsTDFiles) {
            //}

        }
    }
    else
    {
        ret = EhsTDFiles_listNext(pListInfo->EhsTDFiles, pListInfo->szLastFile);
    }
    if (ret > 0)
        pListInfo->bFileValid = EHS_TRUE; /* file or directory is OK */

    if (pListInfo->bFileValid)
    {
        EhsStrcpy(EHS_FB_OUT_S(0), pListInfo->szLastFile);
    }
    if (bPathOK)
    {
        if (ret == 1)
        {
            EHS_FB_FINISH(1); //File Exists
        }
        else if (ret == 2)
        {
            EHS_FB_FINISH(2); //Directory Exists
        }
        else
        {
            EHS_FB_FINISH(3); //doesn't exist - no more
        }
    }
    else
    {
        EHS_FB_FINISH(4); //Path Error
    }
}

/** Sets the directory
 * This resets and starts again if the same path name is used
 *
 *
 * */

EHS_FB_RUN_FUNCTION(FileSystemDirSet1)
{
    struct FSDirListStruct* pListInfo = (struct FSDirListStruct*)EHS_FB_RUN_CONTEXT;
    //ehs_uint8 nRet;

    if (EHS_FB_IN_CONNECTED(0))
    {
        EhsStrcpy(pListInfo->azCurrentDir, EHS_FB_IN_S(0));//,EHS_TD_FILES_MAX_FILENAME);
        pListInfo->bFileValid = 0; //can't remember ehs false macro....
    }

    EHS_FB_FINISH(1); 	/* Always OK we don't check for errors here */
}



