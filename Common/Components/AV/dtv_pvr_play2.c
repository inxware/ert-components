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
 */

/**
 * @page FunctionReference Lucid Function Block Reference Guide
 * @section DTV
 * @anchor DtvPvrPlay2
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

#include "ehs_fb_types.h" /* for some elements referred to in fid.h */

#include "globals.h"
#include "target_os_dtv.h"
#include "dtv_pvr_play2.h" //@todo what is this for?
#include "setCompletes.h"
#include "callback_queue.h"
#include "app_data.h"
#include "graphics.h"
#include "guiparams.h" //needed for parsing the widget parameters if not in graphics.h
#include "hal-api.h"
#include "hal_dtv.h" //This is not in the hal.h it needs to be included specifically

EHS_FB_FUNCTIONS_START(DtvPvrPlay2)
EHS_FB_FUNCTION_ENTRY("setinput", 0x01, DtvPvrPlay2_set_input)
EHS_FB_FUNCTION_ENTRY("setpos", 0x02, DtvPvrPlay2_set_pos)
EHS_FB_FUNCTION_ENTRY("settime", 0x03, DtvPvrPlay2_set_time)
EHS_FB_FUNCTION_ENTRY("get", 0x04, DtvPvrPlay2_get)
EHS_FB_FUNCTION_ENTRY("play", 0x05, DtvPvrPlay2_play)
EHS_FB_FUNCTION_ENTRY("pause", 0x06, DtvPvrPlay2_pause)
EHS_FB_FUNCTION_ENTRY("reverse", 0x07, DtvPvrPlay2_rev)
EHS_FB_FUNCTION_ENTRY("faster", 0x08, DtvPvrPlay2_faster)
EHS_FB_FUNCTION_ENTRY("slower", 0x09, DtvPvrPlay2_slower)
EHS_FB_FUNCTION_ENTRY("stop", 0x0A, DtvPvrPlay2_stop)
EHS_FB_FUNCTION_ENTRY("vol", 0x0B, DtvPvrPlay2_set_volume)
EHS_FB_FUNCTION_ENTRY("setWindow", 0x0C, DtvPvrPlay2_set_window)
EHS_FB_FUNCTION_ENTRY("toFront", 0x0D,DtvPvrPlay2_to_front)
EHS_FB_FUNCTION_ENTRY("toBack", 0x0E,DtvPvrPlay2_to_back)
EHS_FB_FUNCTIONS_END

/* port identifiers - @todo These are badly named*/
#define PORT_IN_INPUT 0
#define PORT_IN_POS 0
#define PORT_IN_TIME 0
#define PORT_IN_INDEX 0

#define PORT_OUT_INPUT 0
#define PORT_OUT_POS 1
#define PORT_OUT_TIME 2
#define PORT_OUT_LENGTH 3
#define PORT_OUT_SPEED 4
#define PORT_OUT_INDEX 5
#define PORT_OUT_ERR 0 /* @TODO: implement error functionality for DTV_play function block. */

#define PORT_FINISH_INPUT 1
#define PORT_FINISH_SETINPUT 2
#define PORT_FINISH_POS 1
#define PORT_FINISH_TIME 1
#define PORT_FINISH_GET 1
#define PORT_FINISH_PLAY 1
#define PORT_FINISH_PAUSE 1
#define PORT_FINISH_REV 1
#define PORT_FINISH_FASTER 1
#define PORT_FINISH_SLOWER 1
#define PORT_FINISH_STOP 1
#define PORT_FINISH_ERR 1
#define PORT_FINISH_END 1
#define PORT_FINISH_INDEX 1

#define PORT_CALLBACK_END -1

/**
 * Define the identify function.
 */
EHS_FB_IDENTIFY_FUNCTION(DtvPvrPlay2)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsFbPvrPlayClass);
}

/**
 * Initialise play back.
 *
 */
EHS_FB_INIT_FUNCTION(DtvPvrPlay2)
{
    EhsGuiParamsType xParams;
    EhsFbPvrPlayClass* pPvrPlay = (EhsFbPvrPlayClass*)EHS_FB_INIT_CONTEXT;
    // EhsMemset(pPvrPlay->szFilename,0,EHS_TD_FILES_MAX_PATH);
    //@todo something like the following may need to go into the destructor :if (pPvrPlay->pTDPlayback) EhsHMem_tempFree(pPvrPlay->pTDPlayback)
    char guiParams[MAX_PARAM_STR_LEN];
    const char* pParams;
    pPvrPlay->MediaPlayerState=EHS_FB_STATE_NOT_LOADED;
    EHS_TRACE_FUNCTION(EHS_FB_INIT_NAME(GUI_ImageFile));
    pParams = ReadParmFile(&EHS_FB_INIT_PARAMETERS[4], guiParams);
    if (guiParams) {
        EhsParseGuiParameters(guiParams,&xParams);  // We will be reading data is if we are a patch type (LGB Does this - it should be a generic render type rather than for a specific widget type)
        //copy the frame data into struct and ignore the rest
        pPvrPlay->nXpos_adjusted=pPvrPlay->nXpos=xParams.xRect.nLeft;
        pPvrPlay->nYpos_adjusted=pPvrPlay->nYpos=xParams.xRect.nTop;
        pPvrPlay->nWidth_adjusted=pPvrPlay->nWidth=xParams.xRect.nWidth;
        pPvrPlay->nHeight_adjusted=pPvrPlay->nHeight=xParams.xRect.nHeight;
        pPvrPlay->video_on_top=(xParams.nZorder!=0x80000000);// 0 in LAB is converted to 0x80000000 just to be anoying.
        pPvrPlay->pTDPlayback = EhsTDPlayback_init(pPvrPlay);
        if (! pPvrPlay->pTDPlayback)
        {
            EHSH_LOG_ERROR("Could not initialise Media Player");
            //return EHS_FALSE;
        }
    }
    else {
        // slready reported: EHSH_LOG_ERROR("Could not parse paramter file");
        pPvrPlay->pTDPlayback = NULL;
    }

    //todo 2023 fail silently. We should check and log error, but carry on anyway
    EhsTDPlayback_reset((EhsTDPlaybackClass*)pPvrPlay->pTDPlayback); /* initialisation was successful */
    return EHS_TRUE;
}

EHS_FB_DESTROY_FUNCTION(DtvPvrPlay2)
{
    EhsFbPvrPlayClass* pPvrPlay = (EhsFbPvrPlayClass*)EHS_FB_DESTROY_CONTEXT;
    if (pPvrPlay->MediaPlayerState != EHS_FB_STATE_NOT_LOADED)
    {
        EhsTDPlayback_stop(pPvrPlay);
    }
    // not needed -> else {EhsTDPlayback_closeWindow(pPvrPlay);
    pPvrPlay->MediaPlayerState=EHS_FB_STATE_NOT_LOADED; /* Not needed realy as we are about to be destroyed */
}

/**
 * Set filename and position. At this time, position is ignored.
 */
EHS_FB_RUN_FUNCTION(DtvPvrPlay2_set_input)
{
    EhsFbPvrPlayClass* pPVR = (EhsFbPvrPlayClass*)EHS_FB_RUN_CONTEXT;
    ehs_bool URIOK = EHS_FALSE;
    ehs_bool UriIsUrl = EHS_FALSE;
    ehs_char filename[EHS_STRING_LENGTH_MAX];
    ehs_sint32 pos,i,filenamelength;
    ehs_char *pIn, *pOut;
    ehs_char *postfix = NULL;
    const ehs_char* fileprefix=EHS_AV_URI_PREFIX_FILE; //"file:///";
    int prefixlen = 0;
    int pathoffset = 0;

    EhsTPMutex_lock(EhsTPMutex_fbIO);  // todo this fbio mutexing hoould be much finer grained in this function - may need to copy some strings locally though
    EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*)pPVR->pTDPlayback;
    ehs_char *inFileName = EHS_FB_IN_S(PORT_IN_INPUT);
    filenamelength = EhsStrlen(inFileName);
    EhsStrcpy(filename,"");
    if (filenamelength < EHS_STRING_LENGTH_MAX)
    {
        /* find the prefix (last dot) */

        for (i=filenamelength-1; i >= 0; i--)
        {
            if (inFileName[i] == '.')
            {
                postfix = &inFileName[i+1];
                break;
            }
        }
    }
    else
    {
        EHSH_LOG_ERROR("Filename is too long. Not playing");
    }
    /* sort out the URI, adding any prefixes and check it a valid file type */
    /* THis function does not support playing media paths otside of the user data directory */
    if (    (postfix &&
             EhsStrcmp(postfix,"jpg")!=0 &&
             EhsStrcmp(postfix,"png")!=0 &&
             EhsStrcmp(postfix,"gif")!=0 &&
             EhsStrcmp(postfix,"txt")!=0) ||
            EhsStrncmp(inFileName,"http:",EhsStrlen("http:"))==0
       )
    {
        if (EhsStrlen(inFileName) > 6 )   // max length of any uri prefix like file://
        {
            if (EhsStrncmp(inFileName,"http",EhsStrlen("http"))==0)   /* Check if we are a URL */
            {
                EhsStrcpy(filename,inFileName);
                URIOK=EHS_TRUE;// Pass the URL in unckecked
                UriIsUrl = EHS_TRUE;
            }
            else
            {
                if (EhsStrncmp(inFileName,EHS_AV_URI_PREFIX_FILE,EhsStrlen(EHS_AV_URI_PREFIX_FILE))==0)   /* Check if we are a file URI */
                {
                    pathoffset = EhsStrlen(EHS_AV_URI_PREFIX_FILE); // todo why do we have offset and length for the same prefix
                }
                else
                {
                    // leave path offset at 0
                    pathoffset = 0;
                }
            }
        }
        else   // short file name - hope for the best! ...
        {
            // URIOK = EHS_FALSE; //default left
        }
        /* Create th actual file path in user area and check the file exists in the user area */
        if (URIOK == EHS_FALSE /*only checked for http so far, check file URIs only*/)
        {
            prefixlen = EhsStrlen(EHS_AV_URI_PREFIX_FILE);
            if ( EhsTF_tryCanonicPath(&filename[prefixlen], EHS_RUNTIME_USERDATA_DIR,&inFileName[pathoffset],EHS_FALSE ) == EHS_TRUE)
            {
                for (i=0; i<EhsStrlen(EHS_AV_URI_PREFIX_FILE); i++) filename[i]=fileprefix[i]; /* add prefix in-place don't use strcpy here to avoid terminating null*/
                URIOK=EHS_TRUE;
            }
            else
            {
                //EhsStrcpy(filename,"");
                URIOK=EHS_FALSE;
                EHSH_LOG_ERROR("Could not open path %s",inFileName);
            }
        }
    }
    else
    {
        EHSH_LOG_ERROR("Canot open files wihout a file type extension: %s",filename);
        URIOK=EHS_FALSE;
    }

    if (URIOK == EHS_TRUE && !UriIsUrl)
    {
        if (EhsTF_exists(&filename[prefixlen]) != 1 )
        {
            URIOK = EHS_FALSE;
            EHSH_LOG_ERROR("Media file [%s] not found",&filename[prefixlen]);
        }
    }
    if (URIOK)
    {

        if ( !EhsTDPlayback_loadFile(EHS_FB_RUN_FUNCTION_INSTANCE, filename))
        {
            EHS_FB_FINISH(PORT_FINISH_SETINPUT);
            pPVR->MediaPlayerState = EHS_FB_STATE_NOT_LOADED;
            EHSH_LOG_WARNING("Could not Load the media file %s",filename);
        }
        else
        {
            /* it's valid - update the name in the PvrPlay structure */
            //****EhsStrcpy((ehs_char*) pPVR->szFilename, filename);
            pPVR->MediaPlayerState = EHS_FB_STATE_LOADED;
        }

    }
    else
    {
        EHSH_LOG_ERROR("Will not set media player input [%s] to [%s]",inFileName,filename);
        EHS_FB_FINISH(PORT_FINISH_SETINPUT); // todo this is an error port that is floating way below the icon and doesn't get used.
        pPVR->MediaPlayerState = EHS_FB_STATE_NOT_LOADED;
    }
    EHS_FB_FINISH(PORT_FINISH_INPUT);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);

}


EHS_FB_RUN_FUNCTION(DtvPvrPlay2_set_pos)
{
    EhsFbPvrPlayClass* pPVR = (EhsFbPvrPlayClass*) EHS_FB_RUN_CONTEXT;
    if (pPVR->MediaPlayerState != EHS_FB_STATE_NOT_LOADED)
    {
        if (EHS_FB_IN_CONNECTED(PORT_IN_POS))
        {
            EhsTDPlayback_setPos((EhsTDPlaybackClass*) pPVR->pTDPlayback,
                                 EHS_FB_IN_I(PORT_IN_POS));
        }
    }//@todo need an error out here
    EHS_FB_FINISH(PORT_FINISH_POS);
}

EHS_FB_RUN_FUNCTION(DtvPvrPlay2_set_time)
{
    EhsFbPvrPlayClass* pPVR = (EhsFbPvrPlayClass*)EHS_FB_RUN_CONTEXT;
    if (pPVR->MediaPlayerState != EHS_FB_STATE_NOT_LOADED)
    {
        if (EHS_FB_IN_CONNECTED(PORT_IN_TIME))
        {
            EhsTDPlayback_setTime((EhsTDPlaybackClass*) pPVR->pTDPlayback,
                                  EHS_FB_IN_I(PORT_IN_TIME));
        }
    } //@todo need an error out here
    EHS_FB_FINISH(PORT_FINISH_TIME);
}


/**
 *  information from the media player.
 */
EHS_FB_RUN_FUNCTION(DtvPvrPlay2_get)
{
    EhsDataflowIntType nTime = 0;
    EhsDataflowIntType nSpeed = 0;
    EhsDataflowIntType nPos = 0;
    EhsDataflowIntType nLength = 0;
    EhsDataflowIntType nIndex = 0;
    EhsDataflowStringType nInput[EHS_STRING_LENGTH_MAX];
    EhsFbPvrPlayClass* pPVR = (EhsFbPvrPlayClass*)EHS_FB_RUN_CONTEXT;
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*)pPVR->pTDPlayback;
    /*Get position information*/
    if (pPVR->MediaPlayerState != EHS_FB_STATE_NOT_LOADED && pPVR->MediaPlayerState != EHS_FB_STATE_PLAY_ENDED)
    {
        if (!EhsTDPlayback_getPos(pPlayback, &nPos))
        {
            //EHS_FB_FINISH(PORT_FINISH_GET_ERR);
        }
        else
        {
            EHS_FB_OUT_I(PORT_OUT_POS) = nPos;
        }

        /*Get speed information*/
        if (!EhsTDPlayback_getSpeed(pPlayback, &nSpeed))
        {
            //EHS_FB_FINISH(PORT_FINISH_GET_ERR);
        }
        else
        {
            EHS_FB_OUT_I(PORT_OUT_SPEED) = nSpeed;
        }

        /*Get time information*/
        if (!EhsTDPlayback_getTime(pPlayback, &nTime))
        {
            //EHS_FB_FINISH(PORT_FINISH_GET_ERR);
        }
        else
        {
            EHS_FB_OUT_I(PORT_OUT_TIME) = nTime;
        }
        /*Get length information*/
        if (!EhsTDPlayback_getLength(pPlayback, &nLength))
        {
            /*Some error thingy we haven't decided upon yet*/
        }
        else
        {
            EHS_FB_OUT_I(PORT_OUT_LENGTH) = nLength;
        }
        /*Get input string*/
        if (!EhsTDPlayback_getInput(pPlayback, (ehs_char*) nInput)) //@todo typo here was a ref to ref
        {
            /*Some error thingy we haven't decided upon yet*/
        }
        else
        {
            EhsStrcpy(EHS_FB_OUT_S(PORT_OUT_INPUT), (ehs_char*) nInput);
        }
    } //@todo should post error here
    else
    {
        if (pPVR->MediaPlayerState != EHS_FB_STATE_PLAY_ENDED)
        {
            EHS_FB_OUT_I(PORT_OUT_SPEED) = 0;
            EHS_FB_OUT_I(PORT_OUT_POS) = 0;
            EHS_FB_OUT_I(PORT_OUT_TIME) = 0;
            EHS_FB_OUT_I(PORT_OUT_LENGTH) = 0;
            EhsStrcpy(EHS_FB_OUT_S(PORT_OUT_INPUT), "");
        }//else leave all as is
    }
    EHS_FB_FINISH(PORT_FINISH_GET);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
}

/**
 * Play current file from current position
 */
EHS_FB_RUN_FUNCTION(DtvPvrPlay2_play)
{
    EhsFbPvrPlayClass* pPvrPlay = (EhsFbPvrPlayClass*)EHS_FB_RUN_CONTEXT;
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*)pPvrPlay->pTDPlayback; //This interface is very crudy! the setweindow should not need this should be done lower down
    if (pPvrPlay->MediaPlayerState == EHS_FB_STATE_NOT_LOADED) goto error;
    if (pPvrPlay->MediaPlayerState == EHS_FB_STATE_PLAY_ENDED)
    {
        EhsTDPlayback_stop(pPvrPlay); // run this to reset any ended GST player states that wont play otherwise
    }

    if ( EhsTDPlayback_play(EHS_FB_RUN_CONTEXT_REF,100))
    {
        pPvrPlay->MediaPlayerState = EHS_FB_STATE_PLAYING;
        //	EHS_FB_FINISH(PORT_FINISH_ERR);
    }
    else
    {
        pPvrPlay->bPaused = EHS_FALSE;//@todo - do we need this? - should use MediaPlayerState
        pPvrPlay->MediaPlayerState = EHS_FB_STATE_PAUSED;
    }
error://@todo we should assert an error event here really
    EHS_FB_FINISH(PORT_FINISH_PLAY); //This is just the ack that a play request has been made!
    EhsTPMutex_unlock(EhsTPMutex_fbIO);

}

/**
 * Pause playback
 */
EHS_FB_RUN_FUNCTION(DtvPvrPlay2_pause)
{
    EhsFbPvrPlayClass* pPvrPlay = (EhsFbPvrPlayClass*)EHS_FB_RUN_CONTEXT;
    ehs_sint32 nSpeed;
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    if (pPvrPlay->MediaPlayerState != EHS_FB_STATE_NOT_LOADED)
    {
        if (!EhsTDPlayback_pause((EhsTDPlaybackClass*)pPvrPlay->pTDPlayback))
        {
            //EHS_FB_FINISH(PORT_FINISH_ERR);
        }
        else
        {
            pPvrPlay->bPaused = !pPvrPlay->bPaused;
            if (pPvrPlay->bPaused) pPvrPlay->MediaPlayerState = EHS_FB_STATE_PAUSED;
            else pPvrPlay->MediaPlayerState = EHS_FB_STATE_PLAYING;
        }
    } //@todo error handler?
    EHS_FB_FINISH(PORT_FINISH_PAUSE);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
}

/**
 * Reverse direction of playback
 */
EHS_FB_RUN_FUNCTION(DtvPvrPlay2_rev)
{
    /*@TODO: this function has been commented out due to causing problems and not really being needed 20081231*/
    EhsDataflowIntType nSpeed;
    EhsFbPvrPlayClass* pPvrPlay = (EhsFbPvrPlayClass*)EHS_FB_RUN_CONTEXT;
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*)pPvrPlay->pTDPlayback;

    if (pPvrPlay->MediaPlayerState != EHS_FB_STATE_NOT_LOADED)
    {
        if (EhsTDPlayback_getSpeed(pPlayback,&nSpeed))
        {
            if (nSpeed > 0)
            {
                /* only if we're going forward */
                nSpeed *= -1;
                if (!EhsTDPlayback_setSpeed(pPlayback,nSpeed))
                {
                    EHS_FB_FINISH(PORT_FINISH_ERR);
                }
            }
        }
        else
        {
            EHS_FB_FINISH(PORT_FINISH_ERR);
        }
    } //@todo error handler
    EHS_FB_FINISH(PORT_FINISH_REV);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
}

/**
 * Increase playback speed in current direction (forward/backward)
 */
EHS_FB_RUN_FUNCTION(DtvPvrPlay2_faster)
{
    EhsDataflowIntType nSpeed;
    EhsFbPvrPlayClass* pPvrPlay = (EhsFbPvrPlayClass*)EHS_FB_RUN_CONTEXT;
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    if (pPvrPlay->MediaPlayerState != EHS_FB_STATE_NOT_LOADED)
    {
        EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*)pPvrPlay->pTDPlayback;
        if (EhsTDPlayback_getSpeed(pPlayback,&nSpeed))
        {
            if ((nSpeed != 0) && (nSpeed < 10000))
            {
                /* only if we're playing */
                nSpeed *= 2;
                if (!EhsTDPlayback_setSpeed(pPlayback,nSpeed))
                {
                    EHS_FB_FINISH(PORT_FINISH_ERR);
                }
            }
        }
        else
        {
            //EHS_FB_FINISH(PORT_FINISH_ERR); // todo we hould use this if is actualy a real port
        }
    }
    EHS_FB_FINISH(PORT_FINISH_FASTER);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
}

/**
 * Decrease playback speed in current direction (forward/backward)
 */
EHS_FB_RUN_FUNCTION(DtvPvrPlay2_slower)
{
    EhsDataflowIntType nSpeed;
    EhsFbPvrPlayClass* pPvrPlay = (EhsFbPvrPlayClass*)EHS_FB_RUN_CONTEXT;
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    if (pPvrPlay->MediaPlayerState != EHS_FB_STATE_NOT_LOADED)
    {
        EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*)pPvrPlay->pTDPlayback;
        if (EhsTDPlayback_getSpeed(pPlayback,&nSpeed))
        {
            if ((nSpeed > 25) || (nSpeed < -25))
            {
                /* only if we're playing and we can halve without rounding errors */
                nSpeed /= 2;
                if (!EhsTDPlayback_setSpeed(pPlayback,nSpeed))
                {
                    EHS_FB_FINISH(PORT_FINISH_ERR);
                }
            }
        }
        else
        {
            //EHS_FB_FINISH(PORT_FINISH_ERR);
        }
    }
    EHS_FB_FINISH(PORT_FINISH_SLOWER);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
}

/**
 * Stop playback
 */
EHS_FB_RUN_FUNCTION(DtvPvrPlay2_stop)
{

    EhsFbPvrPlayClass* pPvrPlay = (EhsFbPvrPlayClass*)EHS_FB_RUN_CONTEXT;
    //EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*)pPvrPlay->pTDPlayback;
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    if (pPvrPlay->MediaPlayerState != EHS_FB_STATE_NOT_LOADED)
    {
        if (!EhsTDPlayback_stop(pPvrPlay))
        {
            //EHS_FB_FINISH(PORT_FINISH_ERR);
        }
        pPvrPlay->MediaPlayerState = EHS_FB_STATE_STOPPED; /* This would need to be re loaded */
    }
    EHS_FB_FINISH(PORT_FINISH_STOP);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
}

/**
 * Callback to signify that playback has finished
 */
EHS_FB_RUN_FUNCTION(DtvPvrPlay2_end)
{
    EhsFbPvrPlayClass* pPvrPlay = (EhsFbPvrPlayClass*)EHS_FB_RUN_CONTEXT;
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    /* Only assert an end from the taret player if we haven't requested a stop */
    if (pPvrPlay->MediaPlayerState != EHS_FB_STATE_STOPPED)
    {
        pPvrPlay->MediaPlayerState = EHS_FB_STATE_PLAY_ENDED;
        EHS_FB_FINISH(2);
    }
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
}

EHS_FB_RUN_FUNCTION(DtvPvrPlay2_set_volume)
{
    EhsFbPvrPlayClass* pPvrPlay = (EhsFbPvrPlayClass*)EHS_FB_RUN_CONTEXT;
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    // We don't need to check state as the target code checks for existance and also remembers state that can be set at any time */
    // if (pPvrPlay->MediaPlayerState != EHS_FB_STATE_NOT_LOADED) {
    EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*)pPvrPlay->pTDPlayback;
    if(EHS_FB_IN_CONNECTED(0))
    {
        EhsTDSetVol(pPlayback,EHS_FB_IN_I(0));
    }
//	}
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
}

/**
 * Initializes and resizes the window for VLC's video
 */
EHS_FB_RUN_FUNCTION(DtvPvrPlay2_set_window)
{
    EhsFbPvrPlayClass* pPvrPlay = (EhsFbPvrPlayClass*)EHS_FB_RUN_CONTEXT;
    EhsTPMutex_lock(EhsTPMutex_fbIO);

    EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*)pPvrPlay->pTDPlayback;
    /*
    EhsDataflowIntType windowX = 0;
    EhsDataflowIntType windowY = 0;
    EhsDataflowIntType windowW = 0;
    EhsDataflowIntType windowH = 0;

    */
    ehs_uint32 x,y,w,h;
    x=pPvrPlay->nXpos;
    y=pPvrPlay->nYpos;
    w=pPvrPlay->nWidth;
    h=pPvrPlay->nHeight;

    //read in the dimensions of the window
    if(EHS_FB_IN_CONNECTED(0))
    {
        x+= EHS_FB_IN_I(0);
    }
    if(EHS_FB_IN_CONNECTED(1))
    {
        y+= EHS_FB_IN_I(1);
    }
    if(EHS_FB_IN_CONNECTED(2))
    {
        w+= EHS_FB_IN_I(2);
    }
    if(EHS_FB_IN_CONNECTED(3))
    {
        h += EHS_FB_IN_I(3);
    }

    //use this information in our target specific code to set the window
    // We should only call this if the window is in play

    pPvrPlay->nXpos_adjusted = x;
    pPvrPlay->nYpos_adjusted = y;
    pPvrPlay->nWidth_adjusted = w;
    pPvrPlay->nHeight_adjusted = h;
    if (pPlayback)
    {
        EhsTDPlayback_setWindow(pPvrPlay);
        //set the output ports
        EHS_FB_OUT_I(0) = pPvrPlay->nXpos_adjusted;
        EHS_FB_OUT_I(1) = pPvrPlay->nYpos_adjusted;
        EHS_FB_OUT_I(2) = pPvrPlay->nWidth_adjusted;
        EHS_FB_OUT_I(3) = pPvrPlay->nHeight_adjusted;
    }
    else
    {
        EHSH_LOG_ERROR("Coldn't set window with uninitialised pPlayback");
    }
    //fire the finish port
    EHS_FB_FINISH(1);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
}

EHS_FB_RUN_FUNCTION(DtvPvrPlay2_to_back)
{
    EhsFbPvrPlayClass *pPVR = (EhsFbPvrPlayClass*)EHS_FB_RUN_CONTEXT;
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    EhsTDPlaybackClass *pPlayback = (EhsTDPlaybackClass*)pPVR->pTDPlayback;
    pPVR->video_on_top=EHS_FALSE;
    EhsTDPlayback_updateZorder(pPVR);
    EHS_FB_FINISH(1);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
}

EHS_FB_RUN_FUNCTION(DtvPvrPlay2_to_front)
{
    EhsFbPvrPlayClass *pPVR = (EhsFbPvrPlayClass*)EHS_FB_RUN_CONTEXT;
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    EhsTDPlaybackClass *pPlayback = (EhsTDPlaybackClass*)pPVR->pTDPlayback;
    pPVR->video_on_top=EHS_TRUE;
    EhsTDPlayback_updateZorder(pPVR);
    EHS_FB_FINISH(1);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
}
