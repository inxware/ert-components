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
 * @section DTV
 * @anchor DtvPvrList
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
 */

#include "globals.h"
#include "dtv_pvr_list.h"
#include "hal_dtv.h"
#include "hal_string.h"
#include "setCompletes.h"
#include "hal-api.h" /* Needed for logging */
EHS_FB_FUNCTIONS_START(DtvPvrList)
//EHS_FB_FUNCTION_ENTRY("run", 0x01, DtvPvrList)
EHS_FB_FUNCTIONS_END

/**
 * Structure for maintaining information about the DTV PVR List
 */
struct DtvPvrListStruct
{
    ehs_char szLastFile[EHS_TD_FILES_MAX_FILENAME];	/**< The name of the last file read */
    ehs_bool bFileValid;									/**< True if szLastFile contains a filename */
};

/**
 * Define the identify function. No memory is required by this structure.
 */
EHS_FB_IDENTIFY_FUNCTION(DtvPvrList)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(struct DtvPvrListStruct);
}

/**
 * Point to the first programme on our list
 */
EHS_FB_INIT_FUNCTION(DtvPvrList)
{
    struct DtvPvrListStruct* pListInfo = (struct DtvPvrListStruct*)EHS_FB_INIT_CONTEXT;

    EhsTDFiles_init(EhsTDFilesRef);
    pListInfo->bFileValid = EHS_FALSE;

    return EHS_TRUE; /* initialisation succeeded */
}



/**
 * Get an entry from the catalogue of pre-recorded programmes.
 */
EHS_FB_RUN_FUNCTION(DtvPvrList)
{
    struct DtvPvrListStruct* pListInfo = (struct DtvPvrListStruct*)EHS_FB_RUN_CONTEXT;

    if (!pListInfo->bFileValid)
    {
        //pListInfo->bFileValid = EhsTDFiles_listFirst(EhsTDFilesRef,EHS_TD_FILEFLAG_FILE,pListInfo->szLastFile);
    }
    else
    {
        pListInfo->bFileValid = EhsTDFiles_listNext(EhsTDFilesRef, pListInfo->szLastFile);
    }

    if (pListInfo->bFileValid)
    {
        EHS_FB_OUT_S_SET(1, pListInfo->szLastFile);
    }
    EHS_FB_OUT_B(0) = pListInfo->bFileValid;

    EHS_FB_FINISH(1);
}


