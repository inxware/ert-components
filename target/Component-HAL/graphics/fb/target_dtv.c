/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file target_dtv.c
 * Definitions for target-specific dtv-related functions.
 *
 * @author: inx limited
 *
 */


/**
 * @page Verification Verification report
 * @section target_timer
 * @anchor target_timer
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on
 * Last modified on $Date:$
 *
 * This file contained no derogations to the MISRA standard.
 */


/**
 * Provides access to the target-specific declarations of header files
 */
#define EHS_TARGET_CODE

/*****************************************************************************/
/* Included files */
#include "hal_dtv.h"
#include "hal_string.h"
#include "hal_file.h"
#include "messages.h"

#include <dirent.h>
#include <stddefs.h>

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/**
 * Specifies where the root of the playback media folder can be found
 * NOTE: no trailing /
 */
#define EHS_TD_FILES_ROOT "/opt/Lucid/media"

/**
 * Path separator for this target
 */

/**
 * Check for any errors and report them if appropriate
 */
#define EHS_TD_REPORT_ERRORS(err,func) if ((err) != ST_NO_ERROR) { EhsError(EHS_MSG_TGT_STAPI(func,GetErrorText(err))); }

/**
 * Indicates the state that the playback is currently in
 */

/**
 * Indicates the properties contained within the target viewport type
 */
struct EhsTDPlaybackStruct
{
    EhsTDStateEnum xPlaybackState;		/**< What state is the playback in? */
    ehs_sint32 nPlaySpeed;					/**< What speed are we playing at the current time? */
};

/**
 * Contains the properties held about the video playback filesystem
 */
struct EhsTDFilesStruct
{
    ehs_char szPath[EHS_TD_FILES_MAX_PATH];	/* the full "current path". Always begins with EHS_TD_FILES_ROOT */
    DIR* pDirectoryInfo;
    EhsTDFileFlagEnum eSearch;					/* search types currently in force */
};

/*****************************************************************************/
/* Declare prototypes of local functions */
/*****************************************************************************/
/* Variables defined with file-scope */

//EHS_LOCAL EhsTDPlaybackClass EhsTDPlayback;
//EHS_LOCAL EhsTDFilesClass EhsTDFiles;

/*****************************************************************************/
/* Variables defined with global-scope */

/**
 * Main playback structure
 * @todo Handle initialization in a funciton */
//EhsTDPlaybackClass* EhsTDPlaybackRef = &EhsTDPlayback;

//EhsTDFilesClass* EhsTDFilesRef = &EhsTDFiles;

/**
 * Callback structure to indicate when playback finishes
 * @todo initialise this correctly
 */
EhsCallbackQueueType EhsTDPlayEndCallback;

/*****************************************************************************/
/* Function definitions */

/**
 * Initialise the DTV playback structure
 * @param[in] pPlayback Playback structure
 * @return true if successful
 */
ehs_bool EhsTDPlayback_init(EhsTDPlaybackClass* pPlayback)
{
    pPlayback->xPlaybackState = EHS_FB_STATESTATE_NOT_LOADED;
    pPlayback->nPlaySpeed = 0;
    return EHS_TRUE;
}

/**
 * Load the file to start playing. The specified file must exist in the current folder.
 * @param[in] pPlayback Playback structure
 * @param[in] pFiles Specifies folder where file is to be loaded from
 * @param[in] szFilename Name of file to play (including extension)
 * @return true if successful
 */
ehs_bool EhsTDPlayback_loadFile(EhsTDPlaybackClass* pPlayback, EhsTDFilesClass* pFiles, const ehs_char* szFilename)
{
    ehs_char szCmd[12+EHS_TD_FILES_MAX_FILENAME];
    ST_ErrorCode_t ErrCode = ST_NO_ERROR;
    ehs_bool bRet;
    /* @todo check for current xPlaybackState */
    if (ErrCode == ST_NO_ERROR)
    {
        EhsSprintf(szCmd,"vid_load \"%s%c%s\"", EHS_TD_FILES_ROOT,EHS_TD_FILES_SEPARATOR,szFilename);
        ErrCode = main_shared_exec(szCmd);
        EHS_TD_REPORT_ERRORS(ErrCode, szCmd);
    }

    if (ErrCode == ST_NO_ERROR)
    {
        pPlayback->xPlaybackState = EHS_FB_STATE_LOADED;
        pPlayback->nPlaySpeed = 0;
        bRet = EHS_TRUE;
    }
    else
    {
        pPlayback->xPlaybackState = EHS_FB_STATE_NOT_LOADED;
        pPlayback->nPlaySpeed = 0;
        exit(1);
        bRet = EHS_FALSE;
    }

    return bRet;
}

/**
 * Set the playback position for the currently loaded file.
 * @param[in] pPlayback playback structure
 * @param[in] nPos index into the file
 * @return true if the index has been set successfully
 */
ehs_bool EhsTDPlayback_setPos(EhsTDPlaybackClass* pPlayback, ehs_uint32 nPos)
{
    EhsError(EHS_MSG_ERROR_NOT_YET_IMPLEMENTED("EhsTDPlayback_setPos"));
    return EHS_FALSE;
}

/**
 * Get the playback position for the currently loaded file.
 * @param[in] pPlayback playback structure
 * @param[out] pnPos The current position of playback. 0 if no file is loaded, or playback hasn't started
 * @return true if the index has been read successfully
 */
ehs_bool EhsTDPlayback_getPos(EhsTDPlaybackClass* pPlayback, ehs_uint32* pnPos)
{
    EhsError(EHS_MSG_ERROR_NOT_YET_IMPLEMENTED("EhsTDPlayback_setPos"));
    return EHS_FALSE;
}

/**
 * Get the playback speed for the currently loaded file.
 * @param[in] pPlayback playback structure
 * @param[out] pnSpeed The current speed of playback (as a percentage of normal speed). 0 means that the file isn't playing.
 * 100 is playing at normal speed, negative values indicate playing backwards
 * @return true if the index has been read successfully
 */
ehs_bool EhsTDPlayback_getSpeed(EhsTDPlaybackClass* pPlayback, ehs_sint32* pnSpeed)
{
    *pnSpeed = pPlayback->nPlaySpeed;
    return EHS_TRUE;
}

/**
 * Set the playback speed for the currently loaded file.
 * @param[in] pPlayback playback structure
 * @param[in] nSpeed The current speed of playback (as a percentage of normal speed). 0 pause the file
 * 100 is playing at normal speed, negative values indicate playing backwards
 * @return true if the speed has been set successfully
 */
ehs_bool EhsTDPlayback_setSpeed(EhsTDPlaybackClass* pPlayback, ehs_sint32 nSpeed)
{
    return EHS_FALSE;
}

/**
 * Start video playback at the specified speed/change playback to the specified speed.
 * This function is called with speed 0 to pause the playback.
 * @param[in] pPlayback playback structure
 * @param[in] nSpeed The speed to play the file.
 * @return true if the video playback has started running at that speed
 */
ehs_bool EhsTDPlayback_play(EhsTDPlaybackClass* pPlayback, ehs_sint32 nSpeed)
{
    ST_ErrorCode_t ErrCode,	/* reports errors on the main function */
                   ErrCodeRecover;	/* reports errors that occur as we're trying to recover from previous errors */
    ehs_bool bRet = EHS_TRUE; /* assume success */

    if (pPlayback->xPlaybackState == EHS_FB_STATE_LOADED)
    {
        /* inject the loaded stream into the video driver. argument is number of times to play video + 1 */
        ErrCode = main_shared_exec("vid_inj -1");
        if (ErrCode != ST_NO_ERROR)
        {
            EhsError(EHS_MSG_TGT_STAPI("main_shared_exec(vid_inj -1)",GetErrorText(ErrCode)));
            pPlayback->xPlaybackState = EHS_FB_STATE_NOT_LOADED;
            pPlayback->nPlaySpeed = 0;
            bRet = EHS_FALSE;
        }
        else
        {
            /* start video decode and display */
            ErrCode = main_shared_exec("vid_start");
            if (ErrCode != ST_NO_ERROR)
            {
                EhsError(EHS_MSG_TGT_STAPI("main_shared_exec(vid_start)",GetErrorText(ErrCode)));
                /* couldn't start the video decode, kill the injection */
                ErrCodeRecover = main_shared_exec("vid_kill");
                EHS_TD_REPORT_ERRORS(ErrCodeRecover,"recovery - main_shared_exec(\"vid_kill\")")
                pPlayback->xPlaybackState = EHS_FB_STATE_NOT_LOADED;
                pPlayback->nPlaySpeed = 0;
                bRet = EHS_FALSE;
            }
            else
            {
                /* success video is now playing back at normal speed */
                pPlayback->nPlaySpeed = 100;
                pPlayback->xPlaybackState = EHS_FB_STATE_PLAYING;
            }
        }
    }
    else if (pPlayback->xPlaybackState == EHS_FB_STATE_PLAYING)
    {
        /* video is currently playing - ignore this */
    }
    else
    {
        /* must be an error condition */
        bRet = EHS_FALSE;
        exit(1);
    }

    return bRet;
}

/**
 * Terminate video playback. This causes the screen to clear
 * @param[in] pPlayback The item to stop
 * @return true if playback structure was valid, and video was stopped
 */
ehs_bool EhsTDPlayback_stop(EhsTDPlaybackClass* pPlayback)
{
    ST_ErrorCode_t ErrCode1, ErrCode2;	/* reports errors on the main function */
    ehs_bool bRet = EHS_FALSE; /* assume stop failed */

    if (pPlayback->xPlaybackState == EHS_FB_STATE_PLAYING)
    {
        /* stop the video playback */
        ErrCode1 = main_shared_exec("vid_stop");
        EHS_TD_REPORT_ERRORS(ErrCode1,"main_shared_exec(\"vid_stop\")");
        /* kill the injection of the video stream - note we do this even if vid_stop failed */
        ErrCode2 = main_shared_exec("vid_kill");
        EHS_TD_REPORT_ERRORS(ErrCode1,"main_shared_exec(\"vid_kill\")");

        pPlayback->nPlaySpeed = 0;
        if ((ErrCode1 == ST_NO_ERROR)&&(ErrCode2 == ST_NO_ERROR))
        {
            pPlayback->xPlaybackState = EHS_FB_STATE_LOADED;
            bRet = EHS_TRUE;
        }
        else
        {
            pPlayback->xPlaybackState = EHS_FB_STATE_NOT_LOADED;
        }
    } /* else no effect */

    return bRet;
}

/**
 * Initialise the EhsTDFilesClass structure
 */
void EhsTDFiles_init(EhsTDFilesClass* pFiles)
{
    EhsStrcpy(pFiles->szPath,EHS_TD_FILES_ROOT);
    pFiles->pDirectoryInfo = NULL;
}

/**
 * Return the directory we are curently looking at.
 * @param[in] pFiles Context for the file operation
 * @param[out] szDir Text containing the current path. Caller must pass in an array of length EHS_TD_FILES_MAX_PATH
 * @return true if successful
 */
ehs_bool EhsTDFiles_getDir(EhsTDFilesClass* pFiles, ehs_char* szDir)
{
    ehs_bool bRet = EHS_FALSE; /* assume failure */

    if (0 == EhsStrncmp(pFiles->szPath,EHS_TD_FILES_ROOT,EhsStrlen(EHS_TD_FILES_ROOT)))
    {
        /* if the path begins with EHS_TD_FILES_ROOT, it must be valid! */
        EhsStrcpy(szDir,&pFiles->szPath[EhsStrlen(EHS_TD_FILES_ROOT)]);
        bRet = EHS_TRUE;
    }
    else
    {
        /* invalid path - reset the pFiles structure, and return and empty string */
        *szDir = '\0';
        EhsTDFiles_init(pFiles);
    }

    return bRet;
}

/**
 * Change the directory we are currently looking at
 * @param[in] pFiles Context for the file operation
 * @param[in] szDir Directory to change to. "/" represents the root of the DTV file system (which might
 * not be the root of the underlying OS file system). Absolute and relative paths are supported.
 * @return true if successful.
 */
ehs_bool EhsTDFiles_setDir(EhsTDFilesClass* pFiles, const ehs_char* szDir)
{
    DIR* pNewDir;
    ehs_char szNewPath[EHS_TD_FILES_MAX_PATH];
    ehs_bool bRet = EHS_FALSE; /* assume this fails */

    /* determine full version of new path */
    if (*szDir == EHS_TD_FILES_SEPARATOR)
    {
        /* need to prefix root */
        EhsStrcpy(szNewPath,EHS_TD_FILES_ROOT);
        EhsStrcpy(&(szNewPath[EhsStrlen(EHS_TD_FILES_ROOT)+1]),szDir);
    }
    else
    {
        /* path is relative to current directory */
        EhsError(EHS_MSG_ERROR_NOT_YET_IMPLEMENTED("EhsTDFiles_setDir"));
        EhsStrcpy(szNewPath,EHS_TD_FILES_ROOT);
    }

    pNewDir = opendir(szNewPath);
    if (pNewDir)
    {
        /* directory really exists */
        EhsStrcpy(pFiles->szPath,szNewPath);
        bRet = EHS_TRUE;
        closedir(pNewDir);
    }

    return bRet;
}

/**
 * Start retrieving files from the current directory
 * @param[in] pFiles Context for the file operation
 * @param[in] eFlags Indicates which types of things to report files, directories or both
 * @param[out] szName Text containing the name of the retrieved object. Caller must pass in an array
 * of length EHS_TD_FILES_MAX_FILENAME
 * @return true if a file name has been obtained
 */
ehs_bool EhsTDFiles_listFirst(EhsTDFilesClass* pFiles, EhsTDFileFlagEnum eFlags, ehs_char* szName)
{
    struct dirent* pCurrent;
    struct dirent xDirent;
    ehs_bool bRet = EHS_FALSE;

    if (pFiles->pDirectoryInfo)
    {
        closedir(pFiles->pDirectoryInfo);
    }

    pFiles->pDirectoryInfo = opendir(pFiles->szPath);
    pFiles->eSearch = eFlags;

    return EhsTDFiles_listNext(pFiles, szName);
}

/**
 * Continue retrieving files from the current directory. Keep calling this function until
 * it returns false.
 * @param[in] pFiles Context for the file operation
 * @param[out] szName Text containing the name of the retrieved object. Caller must pass in an array
 * of length EHS_TD_FILES_MAX_FILENAME
 * @return true if a file name has been obtained
 */
ehs_bool EhsTDFiles_listNext(EhsTDFilesClass* pFiles, ehs_char* szName)
{
    struct dirent* pCurrent;
    struct dirent xDirent;
    ehs_bool bRet = EHS_FALSE;

    if (pFiles->pDirectoryInfo)
    {
        while ((!bRet) && (0 == readdir_r(pFiles->pDirectoryInfo, &xDirent, &pCurrent)) && pCurrent)
        {
            switch (xDirent.d_type)
            {
            case DT_DIR:
                if ((pFiles->eSearch & EHS_TD_FILEFLAG_FOLDER) == EHS_TD_FILEFLAG_FOLDER)
                {
                    EhsStrcpy(szName,xDirent.d_name);
                    bRet = EHS_TRUE;
                }
                /* else get next file */
                break;
            case DT_REG:
                if ((pFiles->eSearch & EHS_TD_FILEFLAG_FILE) == EHS_TD_FILEFLAG_FILE)
                {
                    EhsStrcpy(szName,xDirent.d_name);
                    bRet = EHS_TRUE;
                }
                /* else get next file */
                break;
            default:
                break;
            }
        }
    }

    if (!bRet)
    {
        closedir(pFiles->pDirectoryInfo);
        pFiles->pDirectoryInfo = NULL;
    }
    return bRet;

}

