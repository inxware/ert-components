/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/**
 * @page GStreamer media URI player supper for EHS
 * @section target_dtv
 * @todo there are some blocking functions here that should be threaded
 * This file contained no derogations to the MISRA standard.
 * Definitions for target-specific dtv-related functions.
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
#include <gst/gst.h>
#ifdef EHS_GST_010
#ifdef EHS_GUI_SUPPORT
#include <gst/interfaces/xoverlay.h>
#endif
#endif /* EHS_GUI_SUPPORT */

//handle bus messages from the playbin
GstBusSyncReply EhsTDPlayback_bus_sync_handler (GstBus * bus, GstMessage * message, gpointer user_data)
{
    EhsFbPvrPlayClass *pPvrPlay=(EhsFbPvrPlayClass*)user_data;
    EhsTDPlaybackClass* pPlayback=(EhsTDPlaybackClass*)pPvrPlay->pTDPlayback;
    int percent = 0;
    switch(GST_MESSAGE_TYPE(message))
    {
    case GST_MESSAGE_ELEMENT:
#ifdef EHS_GUI_SUPPORT
        if (gst_structure_has_name (message->structure, "prepare-xwindow-id"))
        {
            if(pPlayback->window==0)
            {
                pPlayback->window=EhsTDPlayback_createWindow(pPvrPlay);
            }
            if (pPlayback->window != 0)
            {
                GstXOverlay *xoverlay;
                // GST_MESSAGE_SRC (message) will be the video sink element
                xoverlay = GST_X_OVERLAY (GST_MESSAGE_SRC (message));
                if(xoverlay==NULL)
                {
                    return GST_BUS_PASS;
                }
                //gst_x_overlay_set_window_handle (xoverlay, video_window_xid);
                gst_x_overlay_set_xwindow_id(xoverlay,(gulong)pPlayback->window);
            }
            else
            {
                g_warning ("Should have obtained video_window_xid by now!");
            }
            gst_message_unref (message);
            return GST_BUS_DROP;
        }
#endif
        break;
    case GST_MESSAGE_EOS:
        if(pPlayback->pFIdata!=NULL)
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
        break;
    case GST_MESSAGE_BUFFERING:

        gst_message_parse_buffering(message,&percent);
        if (percent == 100 && pPlayback->xPlaybackState == EHS_FB_STATE_PLAYING)
        {
            gst_element_set_state(GST_ELEMENT(pPlayback->playbin), GST_STATE_PLAYING);
        }
        else
        {
            gst_element_set_state(GST_ELEMENT(pPlayback->playbin), GST_STATE_PAUSED);
        }
        break;
    default:
        break;
    }
    return GST_BUS_PASS;
}

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
    GstElement *playbin=pPlayback->playbin;
    GstStateChangeReturn state;
    ehs_uint32 i;
    //EHSH_LOG_INFO("Unloading Media");
    if (playbin)
    {
        state = gst_element_set_state(GST_ELEMENT(playbin), GST_STATE_NULL); /*  always returns synch */
        //EHSH_LOG_INFO("Done Unloading Media");
    }
    else
    {
        EHSH_LOG_WARNING("Could not Unloading Media");
        return EHS_FALSE; /* No playbin */
    }
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
    GstElement *playbin=pPlayback->playbin;
    if (playbin)
    {
        /* we have something so stop it and load new media */
        //EHSH_LOG_INFO("Loading new media %s",szFilename);
        EhsTDPlayback_UnLoadMedia(pPlayback,EHS_FALSE);
        EhsStrcpy(pPlayback->LoadedFilename, szFilename);
        g_object_set(G_OBJECT(playbin), "uri", szFilename, NULL);
        /* Set the playback volume */
        g_object_set(G_OBJECT(pPlayback->playbin), "volume", (gdouble)(fnv), NULL);
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
    ehs_bool ret;
    GstFormat format = GST_FORMAT_TIME;
    GstSeekFlags flags=GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT;
    ret=gst_element_seek_simple(pPlayback->playbin,format,flags,GST_SECOND*nTime);
    if(ret==EHS_FALSE)
    {
        return EHS_FALSE;
    }
    return EHS_TRUE;
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
    ehs_bool ret=EHS_FALSE;
    GstFormat fmt = GST_FORMAT_TIME;
    gint64 time;
    /* extra check! */
    if (pPlayback->playbin)
    {
        ret = (ehs_bool) gst_element_query_position(
                  GST_ELEMENT(pPlayback->playbin), &fmt, &time);
        if (ret == EHS_FALSE)
        {
            EHSH_LOG_WARNING("Could not get time from gstreamer playbin.\n");
            return EHS_FALSE;
        }
        //if we are here then getting the position worked
        *pnTime = (ehs_sint32) (time / 1000000000);
    }
    else *pnTime=0;
    return EHS_TRUE;
}

ehs_bool EhsTDPlayback_getLength(EhsTDPlaybackClass* pPlayback,ehs_sint32* pnLength)
{
    //use *pnLength=1 to set the length (help for pointers because I hate them too)
    ehs_bool ret=EHS_FALSE;
    GstFormat fmt = GST_FORMAT_TIME;
    gint64 len;
    ret=(ehs_bool)gst_element_query_duration(GST_ELEMENT(pPlayback->playbin),&fmt,&len);
    if(ret==EHS_FALSE)
    {
        EHSH_LOG_WARNING("Could not get duration from gstreamer playbin.\n");
        return ret;
    }
    //if we are here then getting the duration worked
    *pnLength=(ehs_sint32)(len/1000000000);
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
ehs_bool EhsTDPlayback_setSpeed(EhsTDPlaybackClass* pPlayback,
                                ehs_sint32 nSpeed)
{

    if (pPlayback->xPlaybackState != EHS_FB_STATE_NOT_LOADED)
    {
//		EhsTV_setSpeed(pPlayback->nMediaPlayer, ((float) nSpeed) / 100.0);
        return EHS_TRUE;
    }
    else
        return EHS_FALSE;
}/* Some appropriate error stuff for trying to play when already playing*/

ehs_bool EhsTDPlayback_pause(EhsTDPlaybackClass* pPlayback)
{
    gst_element_set_state(GST_ELEMENT(pPlayback->playbin),GST_STATE_PAUSED);
    return EHS_TRUE;//@todo what are we supposed to return?
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
        gst_element_set_state(GST_ELEMENT(pPlayback->playbin), GST_STATE_PLAYING);
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
    if (pPlayback->playbin) gst_element_set_state(GST_ELEMENT(pPlayback->playbin),GST_STATE_NULL);
    if (pPVR->MediaPlayerState !=  EHS_FB_STATE_NOT_LOADED/*EHS_FB_STATE_PLAY_ENDED*/)
    {
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
    if (pPlayback->playbin)
    {
        g_object_set(G_OBJECT(pPlayback->playbin), "volume", (gdouble)(fnv), NULL);
    }
    /* Only set the following when we have a gstreamer pipeline */
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
