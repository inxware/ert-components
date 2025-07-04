/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_dtv.h
 * In this file, all of the hardware abstraction layer functions relating to digital TV functions are given.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_HAL_DTV_H
#define EHS_HAL_DTV_H

/* only process this file if we provide video support */
//#ifdef EHS_AV_SUPPORT
//#ifdef EHS_GUI_SUPPORT

/*****************************************************************************/
/* Included files */
#include "ehs_types.h"
#include "target_dtv.h"
#include "callback_queue.h"
#include "hal-api.h"

/*****************************************************************************/
/* Define macros  */


/*****************************************************************************/
/* Define types */


typedef struct EhsTDPlaybackStruct EhsTDPlaybackClass;
//EHS_GLOBAL EhsTDPlaybackClass* EhsTDPlaybackRef;
/**
 * Callback structure to indicate when playback finishes
 */
EHS_GLOBAL EhsCallbackQueueType EhsTDPlayEndCallback;

/* This is typically set by the target specific codecs in their target specific structs*/
typedef enum
{
    EHS_AUDIO,
    EHS_VIDEO,
    EHS_HTTPSTREAM,
    EHS_UNKNOWN_MEDIA_TYPE
} EhsFBMediaType;

typedef enum
{
    EHS_FB_STATE_NOT_LOADED,
    EHS_FB_STATE_LOADED,
    EHS_FB_STATE_PLAYING,
    EHS_FB_STATE_PAUSED,
    EHS_FB_STATE_STOPPED,
    EHS_FB_STATE_PLAY_ENDED
} EhsFBMediaPlayerStateEnum;

/* delete me
typedef enum {
        EHS_TD_STATE_NOT_LOADED,
        EHS_TD_STATE_LOADED,
        EHS_TD_STATE_PLAYING,
        EHS_TD_STATE_PAUSED,
        EHS_TD_STATE_PLAY_ENDED
} EhsTDStateEnum;
*/

/* This is the generic AV component data */
//@todo should this be moved?
struct EhsFbDtvPvrPlay2Struct
{
    //ehs_char szFilename[EHS_TD_FILES_MAX_PATH];	/**< contains the filename of the current DTV playback */
    ehs_bool bPaused;
    EhsFBMediaPlayerStateEnum MediaPlayerState;
    //ehs_sint16 volume; // IN TARGET CODE! Persisted volume control so that this can be set when no media is being layed (i.. no object)
    ehs_bool video_on_top;// This is a static variable that identifies if a video is rendered in fron or behind the OSD.
    void* pTDPlayback; // this is the container for the target specific object.
    //@todo - there is alot of generic stuff in pTDPlayback that should be in this structure instead.
    ehs_uint16 nXpos; // these are the base positions
    ehs_uint16 nYpos;
    ehs_uint16 nWidth;
    ehs_uint16 nHeight;
    ehs_uint16 nXpos_adjusted; // these are the shifted positions
    ehs_uint16 nYpos_adjusted;
    ehs_uint16 nWidth_adjusted;
    ehs_uint16 nHeight_adjusted;

};
typedef struct EhsFbDtvPvrPlay2Struct EhsFbPvrPlayClass;

/*****************************************************************************/
/* Declare function prototypes  */


/**
 * Start up DTV subsystem
 */
EHS_GLOBAL void* EhsTDPlayback_init(EhsFbPvrPlayClass* pPvrPlay) ;

#ifndef EhsTDPlayback_reset
/**
 * Initialise the DTV playback structure
 * @param[in] pPlayback Playback structure
 * @return true if successful
 */
EHS_GLOBAL ehs_bool EhsTDPlayback_reset(EhsTDPlaybackClass* pPlayback);
#endif

#ifndef EhsTDPlayback_loadFile
/**
 * Load the file to start playing. The specified file must exist in the current folder.
 * @param[in] pPlayback Playback structure
 * @param[in] pFiles Specifies folder where file is to be loaded from
 * @param[in] szFilename Name of file to play (including extension)
 * @return true if successful
 */
EHS_GLOBAL ehs_bool EhsTDPlayback_loadFile(EhsFunctionInstanceDataType* pFIdata, const ehs_char* szFilename);
#endif


#ifndef EhsTDPlayback_setPos
/**
 * Set the playback position for the currently loaded file.
 * @param[in] pPlayback playback structure
 * @param[in] nPos index into the file
 * @return true if the index has been set successfully
 */
EHS_GLOBAL ehs_bool EhsTDPlayback_setPos(EhsTDPlaybackClass* pPlayback,ehs_sint32 nPos);
#endif

EHS_GLOBAL ehs_bool EhsTDPlayback_setTime(EhsTDPlaybackClass* pPlayback,ehs_sint32 nTime); /* @TODO: are all these ifndefs really needed? */

#ifndef EhsTDPlayback_setSpeed
/**
 * Set the playback speed for the currently loaded file.
 * @param[in] pPlayback playback structure
 * @param[in] nSpeed The current speed of playback (as a percentage of normal speed). 0 pause the file
 * 100 is playing at normal speed, negative values indicate playing backwards
 * @return true if the speed has been set successfully
 */
EHS_GLOBAL ehs_bool EhsTDPlayback_setSpeed(EhsTDPlaybackClass* pPlayback, ehs_sint32 nSpeed);
#endif

#ifndef EhsTDPlayback_getPos
/**
 * Get the playback position for the currently loaded file.
 * @param[in] pPlayback playback structure
 * @param[out] pnPos The current position of playback. 0 if no file is loaded, or playback hasn't started
 * @return true if the index has been read successfully
 */
EHS_GLOBAL ehs_bool EhsTDPlayback_getPos(EhsTDPlaybackClass* pPlayback, ehs_sint32* pnPos);
#endif
EHS_GLOBAL ehs_bool EhsTDPlayback_getLength(EhsTDPlaybackClass* pPlayback, ehs_sint32* pnLength);
EHS_GLOBAL ehs_bool EhsTDPlayback_getInput(EhsTDPlaybackClass* pPlayback, EhsDataflowStringType ppnInput);
EHS_GLOBAL ehs_bool EhsTDPlayback_getTime(EhsTDPlaybackClass* pPlayback, ehs_sint32* pnTime);
EHS_GLOBAL ehs_bool EhsTDPlayback_pause(EhsTDPlaybackClass* pPlayback);
#ifndef EhsTDPlayback_getSpeed
/**
 * Get the playback speed for the currently loaded file.
 * @param[in] pPlayback playback structure
 * @param[out] pnSpeed The current speed of playback (as a percentage of normal speed). 0 means that the file isn't playing.
 * 100 is playing at normal speed, negative values indicate playing backwards
 * @return true if the index has been read successfully
 */
EHS_GLOBAL ehs_bool EhsTDPlayback_getSpeed(EhsTDPlaybackClass* pPlayback, ehs_sint32* pnSpeed);
#endif

#ifndef EhsTDPlayback_play
/**
 * Start video playback at the specified speed/change playback to the specified speed.
 * This function is called with speed 0 to pause the playback.
 * @param[in] pPlayback playback structure
 * @param[in] nSpeed The speed to play the file.
 * @return true if the video playback has started running at that speed
 */
EHS_GLOBAL ehs_bool EhsTDPlayback_play(EhsFunctionInstanceDataType* pFIdata, ehs_sint32 nSpeed);
#endif

#ifndef EhsTDPlayback_stop
/**
 * Terminate video playback. This causes the screen to clear
 * @param[in] pPlayback The item to stop
 * @return true if playback structure was valid, and video was stopped
 */
EHS_GLOBAL ehs_bool EhsTDPlayback_stop(EhsFbPvrPlayClass* pPlayback);
#endif

#ifndef EhsTDPlayback_stop
EHS_GLOBAL void EhsTDSetVol(EhsTDPlaybackClass* pPlayback, ehs_sint32 nVol);
#endif

#ifndef EhsTDPlayback_closeWindow
//EHS_GLOBAL void EhsTDPlayback_setWindow(EhsDataflowIntType windowX, EhsDataflowIntType windowY, EhsDataflowIntType windowW, EhsDataflowIntType windowH, ehs_uint8 zorder,EhsTDPlaybackClass* pPlayback);
EHS_GLOBAL void EhsTDPlayback_closeWindow(EhsFbPvrPlayClass* pPvrPlay );
#endif

#ifndef EhsTDPlayback_setWindow
//EHS_GLOBAL void EhsTDPlayback_setWindow(EhsDataflowIntType windowX, EhsDataflowIntType windowY, EhsDataflowIntType windowW, EhsDataflowIntType windowH, ehs_uint8 zorder,EhsTDPlaybackClass* pPlayback);
EHS_GLOBAL void EhsTDPlayback_setWindow(EhsFbPvrPlayClass* pPvrPlay );
#endif


//#endif /* #ifdef EHS_AV_SUPPORT */

#endif /* EHS_HAL_DTV_H */
