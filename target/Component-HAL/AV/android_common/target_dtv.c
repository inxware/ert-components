/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/*
 * Definitions for target-specific dtv-related functions.
 *
 * @author: inx limited
 *
 */

/**
 * @page GStreamer media URI player supper for EHS
 * @section A/V
 * @todo there are some blocking functions here that should be threaded
 * This file contained no derogations to the MISRA standard.
 */


#define EHS_TARGET_CODE

/*****************************************************************************/
/* Included files */
#include "hal_dtv.h"
#include "hal_string.h"
#include "messages.h"
#include "ehs_types.h"
#include "hal_viewport.h"
#include "widget.h"
#include "widget.h"
#include "hal-api.h"
#include "dtv_pvr_play2.h"
#include "target_os_dtv.h" //this contains definitions of functions that call os specific code
#include <sys/types.h>
//#include <sys/dir.h> // this was only required when file stuff was in here ??
#include <sys/param.h>

/**
 * Initialise the DTV playback structure
 * @param[in] pPlayback Playback structure
 * @return true if successful
 */
ehs_bool EhsTDPlayback_reset(EhsTDPlaybackClass* pPlayback)   //@todo not implemented - this should stop the media player if it is running
{
    return EHS_TRUE;
}


ehs_bool Ehs_TVInstallstopCallBack(EhsFunctionInstanceDataType* pFIdata,ehs_bool Install_Remove)
{
    return EHS_TRUE;
}

#define EHS_MEDIAPLAYERPOLEINTERVAL_MS 10
#define EHS_MEDIAPLAYERTIMEOUT_MS 1000
/** Blocking function to stop and unload the media and remove any windows created
 *  @todoimplement flag to actually release the playbin object (This is currently permenant throughout an application session).
 *
 * */
ehs_bool EhsTDPlayback_UnLoadMedia(EhsTDPlaybackClass* pPlayback,ehs_bool remove_player_too)
{
    EHSH_LOG_INFO("Unloading Media");
    if (pPlayback != NULL)
    {
        if (remove_player_too)
        {
            EhsTDPlayback_JNI_AV_Command(pPlayback->id, "unload", "remove_player");
        }
        else
        {
            EhsTDPlayback_JNI_AV_Command(pPlayback->id, "unload", "");
        }
        return EHS_TRUE;
    }
    return EHS_FALSE;
}

/**
 * Load the file to start playing. The specified file must exist in the current folder.
 * @param[in] pPlayback Playback structure
 * @param[in] pFiles Specifies folder where file is to be loaded from
 * @param[in] szFilename Name of file to play (including extension)
 * @return true if successful
 *
 * @todo we need to avoid memory leeks here..
 */

ehs_bool EhsTDPlayback_loadFile(EhsFunctionInstanceDataType* pFIdata, const ehs_char* szFilename)   //@'todo remove the pFiles referene here.
{
    EhsFbPvrPlayClass* pPVR = (EhsFbPvrPlayClass*) EHS_FB_RUN_CONTEXT;
    EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*) pPVR->pTDPlayback;

    float fv = (float) pPlayback->nVolume;
    float fp = (float) 100;
    float fnv = fv / fp;
    //set the pointer to the function instance data, needed so that we can fire end of stream call backs
    pPlayback->pFIdata=pFIdata;
    ehs_bool success=EHS_TRUE;
    if (pPlayback != NULL)
    {
        /* we have something so stop it and load new media */
        //EHSH_LOG_INFO("Loading new media %s",szFilename);
        EhsTDPlayback_UnLoadMedia(pPlayback,EHS_FALSE);
        EhsStrcpy(pPlayback->LoadedFilename, szFilename);

        EhsTDPlayback_JNI_AV_Command(pPlayback->id, "load", szFilename);

        return EHS_TRUE;
    }
    else EHS_FALSE;
}

/* The following set of functions look after the interface state machine and basically just connect to the same functions below.
 *
 *This layer should be reoved and combined with the ine below to pass on the player handle.
 */

/**
 * Set the playback position for the currently loaded file.
 * @param[in] pPlayback playback structure
 * @param[in] nPos index into the file
 * @return true if the index has been set successfully
 */
ehs_bool EhsTDPlayback_setPos(EhsTDPlaybackClass* pPlayback, ehs_sint32 nPos)
{
    //use the duration to calculate a seek time in seconds then send that through
    ehs_sint32 length;
    ehs_bool ret=EhsTDPlayback_getLength(pPlayback,&length);
    if(ret==EHS_FALSE)
    {
        return EHS_FALSE;
    }
    //turn everything in to floats to force the correct arithmetic
    float fl=(float)length;
    float fp=(float)100;
    float fpp=(float)nPos;
    float ft=(fl/fp)*fpp;
    ehs_sint32 time=(ehs_sint32)ft;
    ret=EhsTDPlayback_setTime(pPlayback,time);
    if(ret==EHS_FALSE)
    {
        return EHS_FALSE;
    }
    return EHS_TRUE;
}

ehs_bool EhsTDPlayback_setTime(EhsTDPlaybackClass* pPlayback, ehs_sint32 nTime)
{
    if (pPlayback != NULL)
    {
        ehs_char time[16];
        sprintf(time, "%d", nTime);
        EhsTDPlayback_JNI_AV_Command(pPlayback->id, "time", time);
        return EHS_TRUE;
    }
    return EHS_FALSE;
}

/**
 * Get the playback position for the currently loaded file.
 * @param[in] pPlayback playback structure
 * @param[out] pnPos The current position of playback. 0 if no file is loaded, or playback hasn't started
 * @return true if the index has been read successfully
 */
ehs_bool EhsTDPlayback_getPos(EhsTDPlaybackClass* pPlayback, ehs_sint32* pnPos)
{
    ehs_sint32 length=0;
    ehs_sint32 time=0;
    ehs_bool ret;
    //get the length
    ret=EhsTDPlayback_getLength(pPlayback,&length);
    if(ret==EHS_FALSE)
    {
        //don't carry on since we can't get the length of the media
        return EHS_FALSE;
    }
    //get elapsed time
    ret=EhsTDPlayback_getTime(pPlayback,&time);
    if(ret==EHS_FALSE)
    {
        //don't carry on
        return EHS_FALSE;
    }
    //cast everything to floats otherwise the arithmetic doesn't work
    float tf=(float)time;
    float tl=(float)length;
    float tp=(float)100;
    //position is (elapsed/length)*100
    *pnPos=(ehs_sint32)((tf/tl)*tp);
    return EHS_TRUE;
}

ehs_bool EhsTDPlayback_getTime(EhsTDPlaybackClass* pPlayback,ehs_sint32* pnTime)
{
    //use *pnTime=1 to set the length (help for pointers because I hate them too)
    ehs_bool ret=EHS_TRUE;
    *pnTime=EhsTDPlayback_JNI_AV_GetIntAttribute(pPlayback->id,"time");
    if(*pnTime < 0)
    {
        ret=EHS_FALSE;
        *pnTime=0;
    }
    return ret;
}

ehs_bool EhsTDPlayback_getLength(EhsTDPlaybackClass* pPlayback,ehs_sint32* pnLength)
{
    //use *pnLength=1 to set the length (help for pointers because I hate them too)
    ehs_bool ret=EHS_TRUE;
    *pnLength=EhsTDPlayback_JNI_AV_GetIntAttribute(pPlayback->id,"length");
    if(*pnLength < 0)
    {
        ret=EHS_FALSE;
        *pnLength=0;
    }
    return ret;
}


/**
 * Get the playback meta data from the media player
 * @todo needs to be implemented for gstreamer
 */
#undef EHS_USE_REAL_MEDIA_DATA
ehs_bool EhsTDPlayback_getInput(EhsTDPlaybackClass* pPlayback,EhsDataflowStringType pnInput)
{
    if ((pPlayback->xPlaybackState != EHS_FB_STATE_NOT_LOADED) && pnInput)
    {
#ifdef EHS_USE_REAL_MEDIA_DATA
//		EhsTV_getInput(pPlayback->nMediaPlayer,pPlayback->nMedia, (char*)pnInput); //@todo checkout this pnInout data type
#else
        if (pPlayback->LoadedFilename)
        {
            EhsStrcpy(pnInput, pPlayback->LoadedFilename);
        }
#endif
    }
    else
    {
        pnInput[0] = '\0';
    }
    return EHS_TRUE;
}

/**
 * Get the playback speed for the currently loaded file.
 * @param[in] pPlayback playback structure
 * @param[out] pnSpeed The current speed of playback (as a percentage of normal speed). 0 means that the file isn't playing.
 * 100 is playing at normal speed, negative values indicate playing backwards
 * @return true if the index has been read successfully
 *
 * @todo needs to be completed
 */
ehs_bool EhsTDPlayback_getSpeed(EhsTDPlaybackClass* pPlayback,ehs_sint32* pnSpeed)
{
    /*
    	if (pPlayback->xPlaybackState != EHS_FB_STATE_NOT_LOADED) {
    		*pnSpeed = (ehs_sint32) (EhsTV_getSpeed(pPlayback->nMediaPlayer)
    				* 100.0);
    	} else {
    		*pnSpeed = 0;
    	}
    */
    return EHS_TRUE;
}

/**
 * Set the playback speed for the currently loaded file.
 * @param[in] pPlayback playback structure
 * @param[in] nSpeed The current speed of playback (as a percentage of normal speed). 0 pause the file
 * 100 is playing at normal speed, negative values indicate playing backwards
 * @return true if the speed has been set successfully
 *
 * @todo needs to be completed
 */
ehs_bool EhsTDPlayback_setSpeed(EhsTDPlaybackClass* pPlayback, ehs_sint32 nSpeed)
{

    if (pPlayback->xPlaybackState != EHS_FB_STATE_NOT_LOADED)
    {
        ehs_char speed[16];
        sprintf(speed, "%d", nSpeed);
        EhsTDPlayback_JNI_AV_Command(pPlayback->id, "set_speed", speed);
        return EHS_TRUE;
    }
    else
    {
        return EHS_FALSE;
    }
}/* Some appropriate error stuff for trying to play when already playing*/

ehs_bool EhsTDPlayback_pause(EhsTDPlaybackClass* pPlayback)
{
    if (pPlayback != NULL)
    {
        EhsTDPlayback_JNI_AV_Command(pPlayback->id, "pause", "");
        return EHS_TRUE;//@todo what are we supposed to return?
    }
    return EHS_FALSE;
}

/**
 * Start video playback at the specified speed/change playback to the specified speed.
 * This function is called with speed 0 to pause the playback.
 * @param[in] pPlayback playback structure
 * @param[in] nSpeed The speed to play the file.
 * @return true if the video playback has started running at that speed
 * @todo the nSpeed is now defunct should remove this
 */
ehs_bool EhsTDPlayback_play(EhsFunctionInstanceDataType* pFIdata,ehs_sint32 nSpeed) /* @TODO: These arguments do nothing and need to be removed at a later date. 20081231*/
{
    EhsFbPvrPlayClass* pPVR = (EhsFbPvrPlayClass*) EHS_FB_RUN_CONTEXT;
    if (pPVR->pTDPlayback)
    {
        EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*) pPVR->pTDPlayback; /* @todo Do want this not to play if we have already loaded? Do we always want another load? */
        if (pPlayback != NULL)
        {
            EhsTDPlayback_JNI_AV_Command(pPlayback->id, "play", "");
        }
    }
    return EHS_TRUE; //@todo what are we actually supposed to return here?
}

/**
 * Terminate video playback. This causes the screen to clear
 * @param[in] pPlayback The item to stop
 * @return true if playback structure was valid, and video was stopped
 */
ehs_bool EhsTDPlayback_stop(EhsFbPvrPlayClass* pPVR)
{
    EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*) pPVR->pTDPlayback; /* @todo This crashes on restarts */
    /* can call this as mush as we like if it still exists/lingers - just in case!*/
    if (pPVR->MediaPlayerState !=  EHS_FB_STATE_NOT_LOADED/*EHS_FB_STATE_PLAY_ENDED*/)
    {
        if (pPlayback != NULL)
        {
            EhsTDPlayback_JNI_AV_Command(pPlayback->id, "stop", "");
        }
        /* This may not be nedded bnut is doesn't trust the client so we will try it */
        EhsTDPlayback_closeWindow(pPVR);
    }
    return EHS_TRUE; //@todo what are we actually supposed to return?
}

/* Set the devices volume using specific gubbins
 * */
void EhsTDSetVol(EhsTDPlaybackClass* pPlayback, ehs_sint32 nVol)
{
    float fv = (float) nVol;
    float fp = (float) 100;
    float fnv = fv / fp;
    if (nVol > 100)
    {
        nVol = 100; /*The most vlc will take without breaking something.*/
    }
    else if (nVol < 0)
    {
        nVol = 0;
    }
    pPlayback->nVolume = nVol; /* media player will set this each time new media is loaded */
    /* Only set the following when we have a gstreamer pipeline */
    if (pPlayback != NULL)
    {
        ehs_char volume[16];
        sprintf(volume, "%d", nVol);
        EhsTDPlayback_JNI_AV_Command(pPlayback->id, "volume", volume);
    }
    return;
}

/********************************************************************************
 *  Begin the TV functions  - not sure why these are TV not TD...
 * The following functions are trusted client.
 * If these are called wrongly then the media player may brake.
 *
 * @todo the layer above should really be abstracted
 * @todo call all the following just be removed?
 * into the common code area
 *
 */

void EhsTV_hideVideoPort(EhsTDPlaybackClass* pPlayback)
{
    //gdk_window_hide(pPlayback->pVLCWindow);
}

void EhsTV_showVideoPort(EhsTDPlaybackClass* pPlayback)
{
    //gdk_window_show(pPlayback->pVLCWindow);
}

//@todo rename all the other functions in here with the TD Prefix
void EhsTDDrawVideoPort(EhsTDPlaybackClass* pPlayback, EhsWidgetClass* pWidget)
{
}

#define CALLBACK_BUFFER_SIZE 64

/**
 * Used for the playback object request
 */
EhsTDPlaybackClass* EhsTDPlayback_get_playback_object(const ehs_char* dataPtr)
{
    void* ptr;
    sscanf(dataPtr, "%p", &ptr);
    EhsFbPvrPlayClass* pPvrPlay = (EhsFbPvrPlayClass*)ptr;
    if(pPvrPlay != NULL)
    {
        return pPvrPlay->pTDPlayback;
    }
    return NULL;
}

/**
 * Used for registartion of the android devices playback callbacks
 */
void EhsTDPlayback_android_register (const ehs_char* id, void* pPvrPlay)
{
    EHSH_LOG_INFO("register android callback.");
    if(id != NULL && pPvrPlay != NULL)
    {
        // register on stop callback
        ehs_char dataPtr[20];
        sprintf(dataPtr, "%p", pPvrPlay);
        EhsTDPlayback_JNI_AV_RegisterCallback(id, "finished", dataPtr);
    }
    else
    {
        EHSH_LOG_INFO("Failed to register callbacks. Both id(%s) and play object must not be NULL.", id);
    }
}

/**
 * Android native media AV callback
 */
JNIEXPORT void JNICALL Java_com_inx_ehs_media_av_EhsMediaHandler_jniCallback(JNIEnv *env, jobject obj, jstring j_id, jstring j_event, jstring j_dataPtr)
{
    if(j_id == NULL || j_event == NULL)
    {
        EHSH_LOG_INFO("Failed to handle JNI callback. id and event strings must not be null.");
        return;
    }
    jboolean iscopy;
    ehs_char event[CALLBACK_BUFFER_SIZE];
    EhsStrcpy(event,(*env)->GetStringUTFChars(env, j_event, &iscopy));
    // handle media callbacks
    if(strcmp(event, "finished") == 0)  /*Gets called after playing is completed*/
    {
        ehs_char dataPtr[CALLBACK_BUFFER_SIZE];
        EhsStrcpy(dataPtr,(*env)->GetStringUTFChars(env, j_dataPtr, &iscopy));
        EhsTDPlaybackClass* pPlayback=EhsTDPlayback_get_playback_object(dataPtr);
        if(pPlayback != NULL && pPlayback->pFIdata != NULL)
        {
            EHS_FB_RUN_NAME(DtvPvrPlay2_end)(pPlayback->pFIdata); /* Sets state EHS_FB_STATE_PLAY_ENDED */
            /* todo these should actually be run in a thread :: */
            //EhsTDPlayback_stop(pPvrPlay);//
            //EhsTDPlayback_closeWindow(pPvrPlay); // this makes it crash @todo - what gst tisy up can we do? Can't do it in this call back probably, but could handle in play/stop calls that crash
        }
        else
        {
            EHSH_LOG_ERROR("function instance data is null, can't fire end of play event\n");
        }
    }// else if(***) handle other callbacks
    //EHSH_LOG_INFO("JNI CALLBACK =======> id=%s, event=%s, data=%s", id, event, (*env)->GetStringUTFChars(env, j_data, &iscopy));
}
