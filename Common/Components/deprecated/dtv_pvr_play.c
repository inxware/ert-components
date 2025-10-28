/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file dtv_pvr_play.c
 *
 * Implementation of the PVR playback functions for the DTV toolkit
 *
 * @author: inx limited
 *
 */

/**
 * @page FunctionReference Lucid Function Block Reference Guide
 * @section DTV
 * @anchor DtvPvrPlay1
 * @subsection appliesTo Applies To:
 * DTV &gt; PVR &gt; Play
 * @subsection properties Properties:
 * None
 * @subsection description Description:
 * This function block implements playback from a specified video file.
 *
 * Before playback can commence, a file name and a position in the file needs to be provided.
 * This is done with inputs "pos" and "name", and using the "set" event. The "set" event can also
 * be used to change the current position of playback after the video has been loaded (for example,
 * while the video is playing).
 *
 * To start the playback, the "play" event is triggered. Triggering the play event when the
 * video is already playing will have no effect. If video is playing at any speed other than 100%
 * (i.e. normal playback speed), the "play" event changes playback speed to 100%. The "play" event will
 * also cancel the effects of the "rev" event.
 *
 * The "pause" event can be used to pause a playing video (causing a still image to be shown on the screen).
 * A subsequent "play" event will resume video playback. Multiple pause events toggle between play and pause. A pause event
 * when video is not playing will have no effect.
 *
 * The "rev" event causes the video playback to operate in reverse. If the video is currently playing (at any speed),
 * "rev" simply changes the playback direction, but maintains the current speed. If paused or stopped, "rev" causes
 * playback to start at normal speed (in reverse).
 *
 * The "faster" and "slower" events cause the playback (forward or reverse) to change by some target-specific step.
 * If the playback is going at the fastest (or slowest) speed possible the "faster" (or "slower") event has
 * no effect. If the playback is operating in reverse, "faster" and "slower" keep the direction the same, but
 * change the speed appropriately.
 *
 * The "get" event causes the current position to be reported on the "pos" output and the current playback speed
 * to be reported on the "speed" output. Speed is represented as a percentage. Zero is paused, negative values
 * are backward, positive values forward.
 *
 * The "end" event is asserted if the playback reaches an end point (either in forward direction or backward
 * direction). When an end point is reached, behaviour is otherwise the same as if the "stop" event has been asserted
 *
 * If any operation causes an error, the "err" event is asserted and an error number is passed to the errNo port.
 * The meaning of this value can be found with the "play error" block.
 */

#include "globals.h"
#include "dtv_pvr_play.h"
#include "setCompletes.h"
#include "callback_queue.h"
#include "graphics.h"

#include "hal-api.h" /* Needed for logging */
#include "hal_dtv.h"

EHS_FB_FUNCTIONS_START(DtvPvrPlay1)
EHS_FB_FUNCTION_ENTRY("set", 0x01, DtvPvrPlay1_set)
EHS_FB_FUNCTION_ENTRY("get", 0x02, DtvPvrPlay1_get)
EHS_FB_FUNCTION_ENTRY("play", 0x03, DtvPvrPlay1_play)
EHS_FB_FUNCTION_ENTRY("pause", 0x04, DtvPvrPlay1_pause)
EHS_FB_FUNCTION_ENTRY("reverse", 0x05, DtvPvrPlay1_rev)
EHS_FB_FUNCTION_ENTRY("faster", 0x06, DtvPvrPlay1_faster)
EHS_FB_FUNCTION_ENTRY("slower", 0x07, DtvPvrPlay1_slower)
EHS_FB_FUNCTION_ENTRY("stop", 0x08, DtvPvrPlay1_stop)
EHS_FB_FUNCTION_ENTRY("end", 0x09, DtvPvrPlay1_end)
EHS_FB_FUNCTIONS_END

/* port identifiers */
#define PORT_IN_SET_NAME 0
#define PORT_IN_SET_POS 1
#define PORT_OUT_SET_ERRNO 1
#define PORT_END_SET 1
#define PORT_END_SET_ERR 2
#define PORT_OUT_GET_ERRNO 0
#define PORT_OUT_GET_POS 1
#define PORT_OUT_GET_SPEED 2
#define PORT_END_GET 1
#define PORT_END_GET_ERR 2
#define PORT_END_PLAY 1
#define PORT_END_PLAY_ERR 2
#define PORT_OUT_PLAY_ERRNO 0
#define PORT_END_PAUSE 1
#define PORT_END_PAUSE_ERR 2
#define PORT_OUT_PAUSE_ERRNO 0
#define PORT_END_REV 1
#define PORT_END_REV_ERR 2
#define PORT_OUT_REV_ERRNO 0
#define PORT_END_FASTER 1
#define PORT_END_FASTER_ERR 2
#define PORT_OUT_FASTER_ERRNO 0
#define PORT_END_SLOWER 1
#define PORT_END_SLOWER_ERR 2
#define PORT_OUT_SLOWER_ERRNO 0
#define PORT_END_STOP 1
#define PORT_END_STOP_ERR 2
#define PORT_OUT_STOP_ERRNO 0
#define PORT_CALLBACK_END -1
#define PORT_END_END 1


struct EhsFbDtvPvrPlayStruct
{
    ehs_char szFilename[EHS_TD_FILES_MAX_FILENAME];	/**< contains the filename of the current DTV playback */
    EhsCallbackQueueEntryType xCallbackEnd;					/**< used to callback end when video play is complete */
    ehs_bool bPaused;										/**< Is the current playback paused? */
};
/**
 * Define the identify function.
 */
EHS_FB_IDENTIFY_FUNCTION(DtvPvrPlay1)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(struct EhsFbDtvPvrPlayStruct);
}

/**
 * Initialise play back.
 *
 */
EHS_FB_INIT_FUNCTION(DtvPvrPlay1)
{
    /*Dimensions for video port*/
    EhsGraphicsRectangleClass rect;
    struct EhsFbDtvPvrPlayStruct* pPvrPlay = (struct EhsFbDtvPvrPlayStruct *)EHS_FB_INIT_CONTEXT;
    ehs_char szRoot[2];
    szRoot[0] = EHS_TD_FILES_SEPARATOR;
    szRoot[1] = '\0';
    rect.nLeft = 300; //@todo these should be parameters and be used (& Removed from here).
    rect.nTop = 200;
    rect.nWidth = 640;
    rect.nHeight = 480;

    EhsMemset(pPvrPlay->szFilename,0,EHS_TD_FILES_MAX_FILENAME);
    //EhsTDFiles_setDir(EhsTDFilesRef, szRoot); This makes things crash - there's no EhsTDFilesRef here!
    /* put this item into the end playback callback queue */
    EhsCallbackQueue_register(&EhsTDPlayEndCallback,
                              EHS_FB_RUN_NAME(DtvPvrPlay1_end),
                              EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(PORT_CALLBACK_END),
                              &pPvrPlay->xCallbackEnd);

    /*Initialise VLC stuff - this shouldn't be here it is target specific*/
    //EhsTV_initVlc(&rect);
    return EhsTDPlayback_reset(EhsTDPlaybackRef); /* initialisation was successful */
}

/**
 * Set filename and position. At this time, position is ignored.
 */
EHS_FB_RUN_FUNCTION(DtvPvrPlay1_set)
{
    struct EhsFbDtvPvrPlayStruct* pPvrPlay = (struct EhsFbDtvPvrPlayStruct *)EHS_FB_RUN_CONTEXT;
    ehs_char filename[EHS_TD_FILES_MAX_FILENAME];
    ehs_char *pIn, *pOut;

    EhsTPMutex_lock(EhsTPMutex_fbIO);
#ifndef RESTRICT_NAME_TO_FILENAMES_ONLY_NOT_PATHS
    EhsStrcpy(filename,EHS_FB_IN_S(PORT_IN_SET_NAME));
#else
    /* copy only characters A-Z,a-z,0-9 and . */
    for (pIn = EHS_FB_IN_S(PORT_IN_SET_NAME), pOut = filename; *pIn != '\0'; pIn++)
    {
        if (((*pIn >= 'A') && (*pIn <= 'Z')) ||
                ((*pIn >= 'a') && (*pIn <= 'z')) ||
                ((*pIn >= '0') && (*pIn <= '9')) ||
                ((*pIn == '.')))
        {
            *pOut = *pIn;
            pOut++;
        }
    }
    *pOut = '\0';
#endif
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    if (!EhsTDPlayback_loadFile(EhsTDPlaybackRef,filename))
    {
        EHS_FB_FINISH(PORT_END_SET_ERR); /* error */
    }
    else
    {
        /* it's valid - update the name in the PvrPlay structure */
        EhsStrcpy(pPvrPlay->szFilename,filename);
    }
    EHS_FB_FINISH(PORT_END_SET);
}

/**
 * Get playback speed and position.
 */
EHS_FB_RUN_FUNCTION(DtvPvrPlay1_get)
{
    EhsDataflowIntType nPosition, nSpeed;
    if (!EhsTDPlayback_getSpeed(EhsTDPlaybackRef,&nSpeed))
    {
        EHS_FB_FINISH(PORT_END_GET_ERR);
        nSpeed = 0;
    }
    else
    {
        EHS_FB_OUT_I(PORT_OUT_GET_SPEED) = nSpeed;
    }
    if (!EhsTDPlayback_getPos(EhsTDPlaybackRef,&nPosition))
    {
        EHS_FB_FINISH(PORT_END_GET_ERR);
        nPosition = 0;
    }
    else
    {
        EHS_FB_OUT_I(PORT_OUT_GET_POS) = nPosition;
    }
    EHS_FB_FINISH(PORT_END_GET);
}

/**
 * Play current file from current position
 */
EHS_FB_RUN_FUNCTION(DtvPvrPlay1_play)
{
    struct EhsFbDtvPvrPlayStruct* pPvrPlay = (struct EhsFbDtvPvrPlayStruct *)EHS_FB_RUN_CONTEXT;

    if (!EhsTDPlayback_play(EhsTDPlaybackRef,100 /* percent of normal speed */))
    {
        EHS_FB_FINISH(PORT_END_PLAY_ERR);
    }
    else
    {
        pPvrPlay->bPaused = EHS_FALSE;
    }
    EHS_FB_FINISH(PORT_END_PLAY);

}

/**
 * Pause playback
 */
EHS_FB_RUN_FUNCTION(DtvPvrPlay1_pause)
{
    struct EhsFbDtvPvrPlayStruct* pPvrPlay = (struct EhsFbDtvPvrPlayStruct *)EHS_FB_RUN_CONTEXT;
    ehs_sint32 nSpeed;

    if (pPvrPlay->bPaused)
    {
        nSpeed = 100; /* normal speed */
    }
    else
    {
        nSpeed = 0; /* paused */
    }

    if (!EhsTDPlayback_play(EhsTDPlaybackRef,nSpeed))
    {
        EHS_FB_FINISH(PORT_END_PAUSE_ERR);
    }
    else
    {
        pPvrPlay->bPaused = !pPvrPlay->bPaused;
    }
    EHS_FB_FINISH(PORT_END_PAUSE);
}

/**
 * Reverse direction of playback
 */
EHS_FB_RUN_FUNCTION(DtvPvrPlay1_rev)
{
    EhsDataflowIntType nSpeed;

    if (EhsTDPlayback_getSpeed(EhsTDPlaybackRef,&nSpeed))
    {
        if (nSpeed > 0)
        {
            /* only if we're going forward */
            nSpeed *= -1;
            if (!EhsTDPlayback_setSpeed(EhsTDPlaybackRef,nSpeed))
            {
                EHS_FB_FINISH(PORT_END_REV_ERR);
            }
        }
    }
    else
    {
        EHS_FB_FINISH(PORT_END_REV_ERR);
    }
    EHS_FB_FINISH(PORT_END_REV);

}

/**
 * Increase playback speed in current direction (forward/backward)
 */
EHS_FB_RUN_FUNCTION(DtvPvrPlay1_faster)
{
    EhsDataflowIntType nSpeed;

    if (EhsTDPlayback_getSpeed(EhsTDPlaybackRef,&nSpeed))
    {
        if (nSpeed != 0)
        {
            /* only if we're playing */
            nSpeed *= 2;
            if (!EhsTDPlayback_setSpeed(EhsTDPlaybackRef,nSpeed))
            {
                EHS_FB_FINISH(PORT_END_FASTER_ERR);
            }
        }
    }
    else
    {
        EHS_FB_FINISH(PORT_END_FASTER_ERR);
    }
    EHS_FB_FINISH(PORT_END_FASTER);
}

/**
 * Decrease playback speed in current direction (forward/backward)
 */
EHS_FB_RUN_FUNCTION(DtvPvrPlay1_slower)
{
    EhsDataflowIntType nSpeed;

    if (EhsTDPlayback_getSpeed(EhsTDPlaybackRef,&nSpeed))
    {
        if ((nSpeed > 25) || (nSpeed < -25))
        {
            /* only if we're playing and we can halve without rounding errors */
            nSpeed /= 2;
            if (!EhsTDPlayback_setSpeed(EhsTDPlaybackRef,nSpeed))
            {
                EHS_FB_FINISH(PORT_END_SLOWER_ERR);
            }
        }
    }
    else
    {
        EHS_FB_FINISH(PORT_END_SLOWER_ERR);
    }
    EHS_FB_FINISH(PORT_END_SLOWER);
}

/**
 * Stop playback
 */
EHS_FB_RUN_FUNCTION(DtvPvrPlay1_stop)
{
    if (!EhsTDPlayback_stop(EhsTDPlaybackRef))
    {
        EHS_FB_FINISH(PORT_END_STOP_ERR);
    }
    EHS_FB_FINISH(PORT_END_STOP);
}

/**
 * Callback to signify that playback has finished
 */
EHS_FB_RUN_FUNCTION(DtvPvrPlay1_end)
{
    EHS_FB_FINISH(PORT_END_END);
}
