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
 * @section target_dtv
 * @anchor target_dtv
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
#include "messages.h"
#include "ehs_types.h"
#include "libvlc_dll.h"
#include "hal_viewport.h"
#include "widget.h"
#include "widget.h"

#include "hal-api.h"

#include "dtv_pvr_play2.h"
#include <sys/types.h>
//#include <sys/dir.h> // this was only required when file stuff was in here ??
#include <sys/param.h>

#include "target_os_dtv.h"

#ifdef EHS_GUI_SUPPORT

#ifndef EHS_MINGW
#include <X11/Xlib.h>
#include <gdk/gdkx.h>
#else

//#include <windows.h>
#include <gdk/gdkwin32.h>
#include <gdk/gdk.h>
#include <windows.h>

#endif
#include <gtk/gtk.h>

#endif /* EHS_GUI_SUPPORT */

//#include "SDL/SDL.h"


/**
 * Indicates the state that the playback is currently in
 *
 * Note: this is now duplicated in the common FB code
 *
 */


/**
 * Indicates the properties contained within the target viewport type
 */
struct EhsTDPlaybackStruct
{
    EhsFBMediaPlayerStateEnum xPlaybackState; //= EHS_FB_STATE_NOT_LOADED; /**< What state is the playback in? */
    ehs_sint32 nPlaySpeed; //= 0; /**< What speed are we playing at the current time? */
    libvlc_media_t* nMedia; //=  NULL; /**< Video currently loaded for playback */
    libvlc_media_player_t* nMediaPlayer; //= NULL; /*@todo this is the one that actually needs to be used */
    libvlc_event_manager_t * nMPEventManager; //Event manager used to handle stop events (etc).
    ehs_char LoadedFilename[EHS_TD_FILES_MAX_PATH];
    libvlc_instance_t* pVLCInstance;
    EhsFBMediaType mediaFormat;
    int nVolume;



#ifdef EHS_GUI_SUPPORT
    GtkWidget* pGTKWindow; //Pointer to the GDK handle for the VLC window. Used for rezing.

#ifndef EHS_MINGW
    Window window; //X handle of the GDK window so that we can pass it to the vlc media player
    Display *pDisplay; //Pointer to the GDK handle for the VLC window. Used for rezing.
#else
    HWND window;
#endif

#endif /*EHS_GUI_SUPPORT*/
};
//libvlc_media_player_t* BADGLOBAL_HANDLE=NULL;
//EhsTDStateEnum xPlaybackState=EHS_FB_STATE_NOT_LOADED;
/*****************************************************************************/
/* Declare prototypes of local functions */
/*****************************************************************************/
/* Variables defined with file-scope */
/**
 * Reference to the main window handle
 */
//HANDLE EhsTgtMainWindowHandle;
/*****************************************************************************/
/* Variables defined with global-scope */
//EhsCallbackQueueType EhsTDPlayEndCallback;
//EhsTDPlaybackClass* EhsTDPlaybackRef;
/**
 * Main playback structure
 * @todo Handle initialization in a funciton */
//@todo must get rid of Andy's crap EHS call back queue stuff and bring back the original method - We don't need it and it forces the use of gloval variables.. this will be a pain for multi-instance issue.
/**
 * Callback structure to indicate when playback finishes
 * @todo initialise this correctly
 */
/*****************************************************************************/
/* Function definitions */
//EHS_GLOBAL ehs_bool EhsTgtMainSetWorkingDirectory(const ehs_char* szChildDir);

/*******************************************************************************
 * Internal Prototypes
 */

float EhsTV_getPosition(libvlc_media_player_t* pMP);
void EhsTV_setPosition(libvlc_media_player_t* pMP, float nPos);
ehs_sint32 EhsTV_getIndex(libvlc_media_player_t* pMP);
void EhsTV_setIndex(libvlc_media_player_t* pMP, ehs_sint32 nIndex);
libvlc_time_t EhsTV_getTime(libvlc_media_player_t* pMP);
void EhsTV_setTime(libvlc_media_player_t* pMP, libvlc_time_t nTime);
libvlc_time_t EhsTV_getLength(libvlc_media_player_t* pMP);
float EhsTV_getSpeed(libvlc_media_player_t* pMP);
void EhsTV_setSpeed(libvlc_media_player_t* pMP, float nSpeed);
int EhsTV_pause(libvlc_media_player_t* pMP);
void EhsTV_getInput(libvlc_media_player_t* pMP, libvlc_media_t* pM,
                    char * input);
void EhsTV_stop(libvlc_media_player_t* pMP);

/*******************************************************************************
 *
 Some VLC functions that will be useful

 libvlc_set_fullscreen libvlc_media_player_t p_mi, int b_fullscreen)

 float libvlc_media_player_get_fps(libvlc_media_player_t p_mi)
 libvlc_state_t libvlc_media_player_get_state(libvlc_media_player_t p_mi)
 void libvlc_media_player_set_time(libvlc_media_player_t p_mi,long i_time)
 libvlc_media_player_get_hwnd(libvlc_media_player_t p_mi)

 void libvlc_media_player_set_hwnd(libvlc_media_player_t p_mi,HWND)

 int libvlc_media_player_get_xwindow(libvlc_media_player_t p_mi)
 int libvlc_media_get_tracks_info(libvlc_media_t media,com.sun.jna.Pointer tracks) ES data?
 int libvlc_media_get_stats(libvlc_media_t p_md,libvlc_media_stats_t p_stats)
 int libvlc_media_get_state(libvlc_media_t p_meta_desc)


 */

/**
 * Create a target specific AV decoder instance
 * @todo rename as create, rather than init
 */
void* EhsTDPlayback_init(EhsFbPvrPlayClass* pPvrPlay)
{
    //Initialise the structure for our target dependent dtv
    ehs_char full_plugin_path[EHS_SYS_MAXPATHLENGTH];
    struct EhsTDPlaybackStruct
        * pPlayback =
        (struct EhsTDPlaybackStruct*) EhsHMem_tempAlloc(sizeof(struct EhsTDPlaybackStruct));
    if (!pPlayback)
        return NULL;
    pPlayback->xPlaybackState = EHS_FB_STATE_NOT_LOADED; //@todo move to the holding class
    pPlayback->nPlaySpeed = 0; //@todo move to the holding class
    pPlayback->nMedia = NULL;
    pPlayback->nMediaPlayer = NULL;
    pPlayback->nMPEventManager = NULL;
    pPlayback->mediaFormat=EHS_UNKNOWN_MEDIA_TYPE;
    pPlayback->nVolume = 50; //@todo move to the holding class //set a defailt value. persistent values are set by the player manager
#ifdef EHS_GUI_SUPPORT
#ifdef EHS_MINGW
#warning "MINGW & LIBVLC VIDEO Enabled"
    //pPlayback->pDisplay=NULL;
    //pPlayback->window = (Window) NULL; // * This is the native window handle */
#else
    pPlayback->pGTKWindow = (GtkWidget*)NULL;
#endif
#endif /*EHS_TARGET_GUI_SUPPIORT*/
    //Initialise the vlc instance
    //,"--no-video-deco"
#ifndef EHS_NO_MEDIAPLAYER_VIDEO
    /* init threads */
    if (!g_thread_supported())
    {
        g_thread_init(NULL);
    }
    gdk_threads_init();
#endif
    /*
     #define VLC_PARAMETERS "--plugin-path=../bin/csdir/vlc", "--reset-plugins-cache", "--ignore-config", "--vout=x11", "--no-osd", "--no-video-title-show", "--aout=alsa","--audio", "--no-loop","--no-repeat","--http-caching=20000","--http-reconnect","--http-user-agent=inxplayer-2.0"
     #define NO_VLC_PARAMETERS 13 // , "--no-plugins-cache", "--list"
     */

#ifndef EHS_MINGW
    /* "--plugin-path=../bin/csdir/vlc/plugins", */
#define VLC_PARAMETERS "--reset-plugins-cache", "--ignore-config", "--vout=x11", "--no-osd", "--no-video-title-show","--audio", "--no-loop","--no-repeat","--http-caching=20000","--http-reconnect","--http-user-agent=inxplayer-2.0","--no-video-title-show","--no-overlay","--no-video-deco"
#define NO_VLC_PARAMETERS 14 // , "--no-plugins-cache", "--list"
#else // We start with a small size because in windows we need vlc to create the window and then resize.
    /* "--plugin-path=..\\bin\\csdir\\vlc\\plugins",  */
#define VLC_PARAMETERS "--reset-plugins-cache", "--ignore-config", "--no-osd", "--no-video-title-show","--audio", "--no-loop","--no-repeat","--http-caching=20000","--http-reconnect","--http-user-agent=inxplayer-2.0","--no-video-title-show","--no-overlay","--no-video-deco", "--width=25", "--height=25"
#define NO_VLC_PARAMETERS 15 // , "--no-plugins-cache", "--list"
#endif

    /* setup the absolute path to the VLC plugins */
#ifndef EHS_MINGW
    EhsStrcpy(full_plugin_path,"--plugin-path=");
    EhsStrcat(full_plugin_path,EhsHMetaGetInstPath());
    EhsStrcat(full_plugin_path,"/bin/csdir/vlc/plugins");
#else
    EhsStrcpy(full_plugin_path,"--plugin-path=");
    EhsStrcat(full_plugin_path,EhsHMetaGetInstPath());
    EhsStrcat(full_plugin_path,"\\bin\\csdir\\vlc\\plugins");
    EHSH_LOG_INFO("Full_plugin_path=%s",full_plugin_path);
#endif

//#define EHS_XDEBUG
    if (pPvrPlay->video_on_top)
    {
#ifdef EHS_XDEBUG//&& defined(EHS_DEBUG_AV)
        const char * const vlc_args[] = {VLC_PARAMETERS,"--video-on-top", "-vvv", full_plugin_path,NULL};
        pPlayback->pVLCInstance = libvlc_new(NO_VLC_PARAMETERS+3, vlc_args);
#else
        const char * const vlc_args[] = { VLC_PARAMETERS,"--video-on-top", full_plugin_path,NULL };
        pPlayback->pVLCInstance = libvlc_new(NO_VLC_PARAMETERS+2, vlc_args);
#endif
    }
    else
    {
#ifdef EHS_XDEBUG//&& defined(EHS_DEBUG_AV)
        const char * const vlc_args[] = {VLC_PARAMETERS,"--no-video-on-top", "-vvv", full_plugin_path,NULL};
        pPlayback->pVLCInstance = libvlc_new(NO_VLC_PARAMETERS+3, vlc_args);
#else
        const char * const vlc_args[] = { VLC_PARAMETERS, "--no-video-on-top",full_plugin_path,NULL };
        pPlayback->pVLCInstance = libvlc_new(NO_VLC_PARAMETERS+2, vlc_args);
#endif
    }


    if (!pPlayback->pVLCInstance)
    {
        EHSH_LOG_ERROR("Could Not Initialise VLC lib");
    }
    if (NULL == pPlayback->pVLCInstance)
    {
        EhsHMem_tempFree(pPlayback);
        return NULL;
    }
    else
    {
        return (void*) pPlayback;
    }
}

/**
 * Initialise the DTV playback structure
 * @param[in] pPlayback Playback structure
 * @return true if successful
 */
ehs_bool EhsTDPlayback_reset(EhsTDPlaybackClass* pPlayback)   //@todo not implemented - this should stop vlc and clear all
{
    if (pPlayback->xPlaybackState != EHS_FB_STATE_PLAYING) /*If a video is already playing then don't ruin state data */
    {
        //pPlayback->xPlaybackState = EHS_FB_STATE_NOT_LOADED; //@todo we only need to initialise at EHS start up and not for each new app so we can leave media playing while apps are updated.
        //pPlayback->nPlaySpeed = 0;
        //pPlayback->nMedia = NULL;
        //pPlayback->nMediaPlayer = NULL;
    }
    pPlayback->nPlaySpeed = 0;
    pPlayback->window=0;//NULL;// @todo We may need to review this if we let video play over app updates?
    return EHS_TRUE;
}
ehs_bool EhsTDPlayback_UnLoadMedia(EhsTDPlaybackClass* pPlayback,ehs_bool);

void * end_function_callback_vlc_eventid_remover(
    const libvlc_event_t * libvlcevent, void * data);

void * EhsVlcMediaInstanceStatusPlaying (const libvlc_event_t * libvlcevent, void * data) ;

ehs_bool Ehs_TVInstallstopCallBack(EhsFunctionInstanceDataType* pFIdata,
                                   ehs_bool Install_Remove)
{
    EhsFbPvrPlayClass* pPVR = (EhsFbPvrPlayClass*) EHS_FB_RUN_CONTEXT;
    EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*) pPVR->pTDPlayback;

    if (!pPlayback->nMediaPlayer) return EHS_FALSE;

    pPlayback->nMPEventManager = libvlc_media_player_event_manager(	pPlayback->nMediaPlayer);
    if (Install_Remove)

    {
        libvlc_event_attach(pPlayback->nMPEventManager,
                            libvlc_MediaPlayerEndReached,
                            (libvlc_callback_t) end_function_callback_vlc_eventid_remover,
                            (void*) pFIdata //@we will keep the istance data as this is instance specific.
                           );

        /* were going to add a playing one also *
         */
#ifdef EHS_MINGW
        libvlc_event_attach(pPlayback->nMPEventManager,
                            libvlc_MediaPlayerTimeChanged,
                            (libvlc_callback_t) EhsVlcMediaInstanceStatusPlaying,(void*) pFIdata);
#endif

    }
    else
    {

        libvlc_event_detach(pPlayback->nMPEventManager,
                            libvlc_MediaPlayerEndReached,
                            (libvlc_callback_t) end_function_callback_vlc_eventid_remover,
                            (void*) pFIdata);

#ifdef EHS_MINGW

        libvlc_event_detach(pPlayback->nMPEventManager,
                            libvlc_MediaPlayerTimeChanged,
                            (libvlc_callback_t) EhsVlcMediaInstanceStatusPlaying,
                            (void*) pFIdata);
#endif
        //pPlayback->nMPEventManager = NULL; // Guessing this is already done but redo here in case not.
    }
    return EHS_TRUE;
}

/* This function is only really needed for win32
 * It is designed to position the window as soon as it appears using the weird setwindow function
 */

void * EhsVlcMediaInstanceStatusPlaying (const libvlc_event_t * libvlcevent, void * data)
{
#ifndef EHS_NO_MEDIAPLAYER_VIDEO
#ifdef EHS_MINGW //We shouldn't be called but just in case...

    //Cast our instance data into something usable
    EhsFunctionInstanceDataType* pFIdata = (EhsFunctionInstanceDataType*) data;
    EhsFbPvrPlayClass* pPVR = (EhsFbPvrPlayClass*) EHS_FB_RUN_CONTEXT;
    EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*) pPVR->pTDPlayback;
    //@todo more refactoring of the EhsTDPlaybackRef to be in the object data references in the function instance data rather than using the global
    if (pPlayback->mediaFormat == EHS_VIDEO)
    {
        EhsTDPlayback_setWindow(pPVR); // set the window to the coords
    }
//Only do this once per load.
    if (pPlayback->nMPEventManager)
    {
        libvlc_event_detach(pPlayback->nMPEventManager,
                            libvlc_MediaPlayerTimeChanged,
                            (libvlc_callback_t) EhsVlcMediaInstanceStatusPlaying,
                            (void*) data);
    }
#endif
#endif
}

/*
 * This function is called by VLC's event handler on the end of a media playback with arguements related to the VLC internal event and also the EHS function instance data.
 * It is interepted here to reformat the callbak to the standard EHS call back structure.
 * Also we run the player stop here so that we don't get the strange effect of the media not playing of played again (even though we don't usually do this).
 */

void * end_function_callback_vlc_eventid_remover(
    const libvlc_event_t * libvlcevent, void * data)
{

    //Cast our instance data into something usable
    EhsFunctionInstanceDataType* pFIdata = (EhsFunctionInstanceDataType*) data;
    EhsFbPvrPlayClass* pPVR = (EhsFbPvrPlayClass*) EHS_FB_RUN_CONTEXT;
    EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*) pPVR->pTDPlayback;
    //@todo more refactoring of the EhsTDPlaybackRef to be in the object data references in the function instance data rather than using the global
    libvlc_state_t playing;

    // - Need to deal with why the same event won't play once it is ended. Reload File seems to work, but nit ideal - try removing with libvlc1.1
    if (pPlayback->nMPEventManager)
    {
        Ehs_TVInstallstopCallBack(data, EHS_FALSE); // removes all the call backs associated with the media
    }
    EhsTDPlayback_UnLoadMedia(pPlayback,EHS_FALSE);
    /*
     playing = libvlc_media_player_get_state(EhsTDPlaybackRef->nMediaPlayer, &EhsTV_excp);
     EhsTV_CheckException( &EhsTV_excp);
     */
    pPlayback->xPlaybackState = EHS_FB_STATE_PLAY_ENDED;
    //EhsTDPlayback_loadFile(EhsTDPlaybackRef,EhsTDPlaybackRef->LoadedFilename); //This crashes here - so do this on replay instead if in ended state.
    EHS_FB_RUN_NAME(DtvPvrPlay2_end)(data); // this is asserted events and stuff..
    //EhsFunctionInstanceData_triggerEvent(pFIdata,2);
}



ehs_bool EhsTDPlayback_UnLoadMedia(EhsTDPlaybackClass* pPlayback,ehs_bool remove_player_too)
{
    EhsTV_stop(pPlayback->nMediaPlayer);

    /* This is the reverse order they were created */
    if (pPlayback->nMediaPlayer && remove_player_too)
    {
        libvlc_media_player_release(pPlayback->nMediaPlayer); //@todo check there aren't any memory leaks - is this the only place the media descriptor is not needed.
        pPlayback->nMediaPlayer = NULL;
    } //else
    if (pPlayback->nMedia)
    {
        libvlc_media_release(pPlayback->nMedia);
        pPlayback->nMedia = NULL;
    } //else

    //make sure it;s flag has gone;
#ifdef EHS_MINGW
    if (pPlayback->window)
    {
        DestroyWindow(pPlayback->window);
        pPlayback->window=(HWND)NULL;
    }
#else
    if (pPlayback->pGTKWindow)
    {
        gtk_widget_destroy(pPlayback->pGTKWindow);
        pPlayback->pGTKWindow=NULL;
        pPlayback->window=(Window) NULL;
    }


#endif
    //
}

/**
 * Load the file to start playing. The specified file must exist in the current folder.
 * @param[in] pPlayback Playback structure
 * @param[in] pFiles Specifies folder where file is to be loaded from
 * @param[in] szFilename Name of file to play (including extension)
 * @return true if successful
 */

//@todo this should go somewhere
extern GtkWidget * EhsTMain_GTK_Window_Handle;

ehs_bool EhsTDPlayback_loadFile(EhsFunctionInstanceDataType* pFIdata,
                                const ehs_char* szFilename)   //@'todo remove the pFiles referene here.
{
    EhsFbPvrPlayClass* pPVR = (EhsFbPvrPlayClass*) EHS_FB_RUN_CONTEXT;
    EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*) pPVR->pTDPlayback;
    ehs_bool success=EHS_TRUE;
    /* We are passed a URI rather than path so  check if this is a file and chop if so */
    if (	EhsStrlen(szFilename) > EhsStrlen(EHS_AV_URI_PREFIX_FILE) &&
            EhsStrncmp(szFilename,EHS_AV_URI_PREFIX_FILE, EhsStrlen(EHS_AV_URI_PREFIX_FILE)) == 0)
    {
        EhsStrcpy(pPlayback->LoadedFilename, &szFilename[EhsStrlen(EHS_AV_URI_PREFIX_FILE)]);
    }
    else EhsStrcpy(pPlayback->LoadedFilename, szFilename);
    // attempt not to show window if we are audio
    if (EhsStrstr(szFilename,".mp3")
            || EhsStrstr(szFilename,".ogg")
            || EhsStrstr(szFilename,".oga")
            || EhsStrstr(szFilename,".wav")
            || EhsStrstr(szFilename,".aac")
            || EhsStrstr(szFilename,".m4a")
       )
    {
        pPlayback->mediaFormat=EHS_AUDIO;
    }
    else if (EhsStrstr(szFilename,".mov") || EhsStrstr(szFilename,".mp4") || EhsStrstr(szFilename,".avi") || EhsStrstr(szFilename,".wmv"))
    {
        pPlayback->mediaFormat=EHS_VIDEO;
    }
    else if (EhsStrncmp(szFilename,"http",4) == 0 || EhsStrncmp(szFilename,"HTTP",4) == 0)
    {
        pPlayback->mediaFormat=EHS_HTTPSTREAM;
    }
    if (pPlayback->mediaFormat != EHS_UNKNOWN_MEDIA_TYPE)
    {
        if (pPlayback->pVLCInstance)
        {

            if (!EhsStrncmp("http://", pPlayback->LoadedFilename, 7) || EhsTF_exists(pPlayback->LoadedFilename))   //@todo otherwise fire a can't play  event
            {
                EHSH_LOG_INFO("Loading %s (using LibVLC)",pPlayback->LoadedFilename);
                //if ((pPlayback->xPlaybackState == EHS_FB_STATE_PLAYING)||(pPlayback->xPlaybackState == EHS_FB_STATE_PAUSED ) ||(pPlayback->xPlaybackState ==  EHS_FB_STATE_PLAY_ENDED) ) {/*Stop playlist*/
                if (pPlayback->nMPEventManager)
                {
                    Ehs_TVInstallstopCallBack(pFIdata, EHS_FALSE);//remove call back handler if there is one (only not on the first run of this function) @todo check the validity of this.
                }
                if (pPlayback->xPlaybackState != EHS_FB_STATE_LOADED)
                {
                    //EhsTDPlayback_UnLoadMedia(pPVR->pTDPlayback,EHS_TRUE);
                    pPlayback->xPlaybackState = EHS_FB_STATE_LOADED;
                }

                // The stop callback function is here to avoid memory leaks if reloading without getting to the end of the current file.
                pPlayback->xPlaybackState = EHS_FB_STATE_NOT_LOADED;
                /* Media player instance is associated with the media file - need to remove the old one*/
                EhsTDPlayback_UnLoadMedia(pPVR->pTDPlayback,EHS_TRUE); // release anything if we have it.
                /* Need to load media to create a libvlc_media_handle */
                if (pPlayback->nMedia = libvlc_media_new_path(
                                            pPlayback->pVLCInstance, pPlayback->LoadedFilename))
                {
                    // do this anyway if we have a handle or abandon media? //[1]
                    pPlayback->nMediaPlayer = libvlc_media_player_new_from_media(
                                                  pPlayback->nMedia);
                    if (pPlayback->nMediaPlayer != NULL)
                    {
                        pPlayback->xPlaybackState = EHS_FB_STATE_LOADED; //important this here to notify setwindow to create the window


#ifdef EHS_GUI_SUPPORT
#ifndef EHS_MINGW
                        if (pPlayback->mediaFormat == EHS_VIDEO)
                        {
                            //Set the window it supposed to use
                            //X11 behaves properly and lets you give it windows
                            EhsTDPlayback_setWindow(pPVR); // set the window to the coords
                            if (pPlayback->window)
                            {
                                libvlc_media_player_set_xwindow(
                                    pPlayback->nMediaPlayer, pPlayback->window);

                            }
                            else
                            {
                                EHSH_LOG_ERROR(
                                    "ERROR****EhsTDPlayback_setWindow returned no window handle (Not MINGW)\n");

                            }
                        }
#else
                        //windows-mingw doesn't - so we need t ge the window it has generated instead after it has started playing.
                        /*			{
                        WNDCLASSEX wc;
                        HWND hwnd;
                        MSG Msg;
                        // HRGN hrgn = CreateRectRgn( 0, 0, 200, 200 );
                        HMODULE HIn=GetModuleHandle(NULL);

                        //Step 1: Registering the Window Class
                        wc.cbSize        = sizeof(WNDCLASSEX);
                        wc.style         = CS_HREDRAW|CS_VREDRAW;
                        wc.lpfnWndProc   = (WNDPROC)WndProc;
                        wc.cbClsExtra    = 0;
                        wc.cbWndExtra    = 0;
                        wc.hInstance     = HIn;
                        wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
                        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
                        wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
                        wc.lpszMenuName  = NULL;
                        wc.lpszClassName = TEXT("inxware");
                        wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

                         */
                        //window=  GDK_WINDOW_HWND(GTK_WINDOW(pViewport->pMainWindow));
                        //pPlayback->window= (HWND)(GDK_WINDOW_HWND(EhsTMain_GTK_Window_Handle->window));//GDK_WINDOW_HWND(GTK_WINDOW(pViewport->pMainWindow));GDK_WINDOW_HWND(GTK_WINDOW(EhsTMain_GTK_Window_Handle));	 //GetDesktopWindow();//CreateWindowEx(WS_EX_TOPMOST,TEXT("inxware"),TEXT("ixnware"),WS_OVERLAPPEDWINDOW/*WS_POPUP*/,0,0,200,200,NULL, NULL ,HIn,NULL);
                        //EhsTDPlayback_setWindow(pPVR); // set the window to the coords
                        //if (pPlayback->window) {
                        //ShowWindow(pPlayback->window,SW_SHOW);
                        //UpdateWindow(pPlayback->window); SetFocus(pPlayback->window);
                        //SetWindowRgn( pPlayback->window, hrgn, TRUE );
                        //SetWindowPos(pPlayback->window,HWND_TOP, 400,300,400,200,SWP_SHOWWINDOW);
                        //libvlc_media_player_set_hwnd(pPlayback->nMediaPlayer,pPlayback->window);
                        //}
                        //else {
                        //}

                        //libvlc_media_player_set_drawable(pPlayback->nMediaPlayer, (libvlc_drawable_t)EhsTgtMainWindowHandle, &EhsTV_excp);
#endif
#endif /*EHS_GUI_SUPPORT*/
                        libvlc_audio_set_volume(pPlayback->nMediaPlayer,
                                                pPlayback->nVolume); // need to set the volume to the player state value
                        pPlayback->xPlaybackState = EHS_FB_STATE_LOADED; //[1]
                    }
                    else
                    {
                        pPlayback->xPlaybackState = EHS_FB_STATE_NOT_LOADED;
                        success=EHS_FALSE;
                    }
                }
                else
                {
                    pPlayback->xPlaybackState = EHS_FB_STATE_NOT_LOADED;
                    success=EHS_FALSE;
                }
                /***************** NEED to add the event handler for the ended event *******************/
                Ehs_TVInstallstopCallBack(pFIdata, EHS_TRUE); //@todo this should be conditional on all being well
                //@todo and for error handling
                //@todo and for error handling
            }
            else
            {
                EHSH_LOG_INFO("VLC File [%s] Doesn't Exist and is not a URL",pPlayback->LoadedFilename);
                pPlayback->xPlaybackState = EHS_FB_STATE_NOT_LOADED;
                success=EHS_FALSE;
            }
        }
        else
        {
            EHSH_LOG_ERROR("VLC WAS NOT INITIALISED, Could not load media resource");
            pPlayback->xPlaybackState = EHS_FB_STATE_NOT_LOADED;
            success=EHS_FALSE;
        }
    }
    else
    {
        EHSH_LOG_ERROR("Media Type (extension) is unknown to vlc");
        pPlayback->xPlaybackState = EHS_FB_STATE_NOT_LOADED;
        success=EHS_FALSE;
    }
    return success;
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
    if (pPlayback->xPlaybackState != EHS_FB_STATE_NOT_LOADED)
    {
        //  if ( libvlc_playlist_isplaying(EhsTV_pLibvlc, &EhsTV_excp) ) ..
        EhsTV_setPosition(pPlayback->nMediaPlayer, ((float) nPos) / 100); /* @TODO: convert this to accept a float straight in, function block needs to be changed first though*/
        return EHS_TRUE;
    }
    else
        return EHS_FALSE;
}

ehs_bool EhsTDPlayback_setIndex(EhsTDPlaybackClass* pPlayback,
                                ehs_sint32 nIndex)
{
    if (pPlayback->xPlaybackState != EHS_FB_STATE_NOT_LOADED)
    {
        EhsTV_setIndex(pPlayback->nMediaPlayer, nIndex);
        return EHS_TRUE;
    }
    else
        return EHS_FALSE;
}

ehs_bool EhsTDPlayback_setTime(EhsTDPlaybackClass* pPlayback, ehs_sint32 nTime)
{
    libvlc_time_t millisecs; /*64 bit unsigned int defined in vlc headers*/
    if (pPlayback->xPlaybackState != EHS_FB_STATE_NOT_LOADED)
    {
        millisecs = nTime * 1000; /*Time from Lucid app in seconds so convert to ms*/
        EhsTV_setTime(pPlayback->nMediaPlayer, millisecs);
        return EHS_TRUE;
    }
    else
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
    if ((pPlayback->xPlaybackState != EHS_FB_STATE_NOT_LOADED) )
    {
        if (pPlayback->nMediaPlayer) *pnPos = (ehs_sint32) (EhsTV_getPosition(pPlayback->nMediaPlayer)* 100.0);
        else *pnPos = 0; // in case the state machine is not solid
        return EHS_TRUE;
    }
    else
    {
        *pnPos = 0;
        return EHS_TRUE; //@todo should we care and send back false?
    }
}

ehs_bool EhsTDPlayback_getTime(EhsTDPlaybackClass* pPlayback,
                               ehs_sint32* pnTime)
{
    if ((pPlayback->xPlaybackState == EHS_FB_STATE_PAUSED)
            || (pPlayback->xPlaybackState == EHS_FB_STATE_PLAYING))
    {
        //	ehs_bool bRet = EHS_TRUE;
        *pnTime
            = (ehs_sint32) (EhsTV_getTime(pPlayback->nMediaPlayer) / 1000.0);

    }
    else
        *pnTime = 0;

    return EHS_TRUE; //@todo should we care and send back false?
}

ehs_bool EhsTDPlayback_getLength(EhsTDPlaybackClass* pPlayback,
                                 ehs_sint32* pnLength)
{
    if (pPlayback->xPlaybackState != EHS_FB_STATE_NOT_LOADED)
    {
        *pnLength = (ehs_sint32) (EhsTV_getLength(pPlayback->nMediaPlayer)
                                  / 1000.0);
    }
    else
    {
        *pnLength = -1;
    }
    return EHS_TRUE;
}

#undef EHS_USE_REAL_MEDIA_DATA
ehs_bool EhsTDPlayback_getInput(EhsTDPlaybackClass* pPlayback,
                                EhsDataflowStringType pnInput)
{
    if ((pPlayback->xPlaybackState != EHS_FB_STATE_NOT_LOADED) && pnInput)
    {
#ifdef EHS_USE_REAL_MEDIA_DATA
        EhsTV_getInput(pPlayback->nMediaPlayer,pPlayback->nMedia, (char*)pnInput); //@todo checkout this pnInout data type
#else
        if (pPlayback->LoadedFilename)
            EhsStrcpy(pnInput, pPlayback->LoadedFilename);
#endif
    }
    else
    {
        pnInput[0] = '\0';
    }
    return EHS_TRUE;

}

/* what does this do - is this the vlc format index?? */
// @todo the following is obsolete (we don't use player's playlist features) and the function block ports should be removed
ehs_bool EhsTDPlayback_getIndex(EhsTDPlaybackClass* pPlayback,
                                ehs_sint32* pnIndex)
{
    if (pPlayback->xPlaybackState != EHS_FB_STATE_NOT_LOADED)
    {
        *pnIndex = EhsTV_getIndex(pPlayback->nMediaPlayer);
    }
    else
    {
        *pnIndex = 0;
    }
    return EHS_TRUE;
}

/**
 * Get the playback speed for the currently loaded file.
 * @param[in] pPlayback playback structure
 * @param[out] pnSpeed The current speed of playback (as a percentage of normal speed). 0 means that the file isn't playing.
 * 100 is playing at normal speed, negative values indicate playing backwards
 * @return true if the index has been read successfully
 */
ehs_bool EhsTDPlayback_getSpeed(EhsTDPlaybackClass* pPlayback,
                                ehs_sint32* pnSpeed)
{

    if (pPlayback->xPlaybackState != EHS_FB_STATE_NOT_LOADED)
    {
        *pnSpeed = (ehs_sint32) (EhsTV_getSpeed(pPlayback->nMediaPlayer)
                                 * 100.0);
    }
    else
    {
        *pnSpeed = 0;
    }

    return EHS_TRUE;
}

/**
 * Set the playback speed for the currently loaded file.
 * @param[in] pPlayback playback structure
 * @param[in] nSpeed The current speed of playback (as a percentage of normal speed). 0 pause the file
 * 100 is playing at normal speed, negative values indicate playing backwards
 * @return true if the speed has been set successfully
 */
ehs_bool EhsTDPlayback_setSpeed(EhsTDPlaybackClass* pPlayback,
                                ehs_sint32 nSpeed)
{

    if (pPlayback->xPlaybackState != EHS_FB_STATE_NOT_LOADED)
    {
        EhsTV_setSpeed(pPlayback->nMediaPlayer, ((float) nSpeed) / 100.0);
        return EHS_TRUE;
    }
    else
        return EHS_FALSE;
}/* Some appropriate error stuff for trying to play when already playing*/

ehs_bool EhsTDPlayback_pause(EhsTDPlaybackClass* pPlayback)
{
    if (pPlayback->xPlaybackState != EHS_FB_STATE_NOT_LOADED)  // ||(pPlayback->xPlaybackState == EHS_FB_STATE_PAUSED)) {//@todo do we want this in all states?
    {
        if (EhsTV_pause(pPlayback->nMediaPlayer))
        {
            pPlayback->xPlaybackState = EHS_FB_STATE_PAUSED;
        }
        else
        {
            pPlayback->xPlaybackState = EHS_FB_STATE_PLAYING;
        }
        return EHS_TRUE;
    }
    else
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
ehs_bool EhsTDPlayback_play(EhsFunctionInstanceDataType* pFIdata,
                            ehs_sint32 nSpeed) /* @TODO: These arguments do nothing and need to be removed at a later date. 20081231*/
{
    EhsFbPvrPlayClass* pPVR = (EhsFbPvrPlayClass*) EHS_FB_RUN_CONTEXT;
    EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*) pPVR->pTDPlayback; /* @todo Do want this not to play if we have already loaded? Do we always want another load? */
    ehs_bool bRet = EHS_TRUE;
    libvlc_state_t playing;
    int isPlaying = 0;
    if (!pPlayback)
    {
        EHSH_LOG_WARNING("Media Player started before initialisation");
        return EHS_FALSE;
    }
    if (pPlayback->xPlaybackState == EHS_FB_STATE_NOT_LOADED)
    {
        return EHS_FALSE;
    }
    if (pPlayback->xPlaybackState == EHS_FB_STATE_PLAY_ENDED)
    {
        //EhsTDPlayback_UnLoadMedia(pPVR->pTDPlayback);
        EhsTDPlayback_loadFile(pFIdata, pPlayback->LoadedFilename); // not found anything else that works here.. This should release all memory @todo check this
    }
    else     // Stop the player (may not be necessary - but does make i start from the beginning each time)
    {
        if (pPlayback->xPlaybackState == EHS_FB_STATE_PLAYING)   //we should ignore this.
        {
            //EhsTV_stop(pPlayback->nMediaPlayer);
            return EHS_FALSE;
        }
        //pPlayback->xPlaybackState = EHS_FB_STATE_NOT_LOADED;
    }
    playing = libvlc_media_player_get_state(pPlayback->nMediaPlayer);
    if ((playing == libvlc_Playing) || playing == libvlc_Paused || playing
            == libvlc_Ended)
    {
        EhsTDPlayback_UnLoadMedia(pPVR->pTDPlayback,EHS_TRUE);
    }
    if (pPlayback->nMedia && pPlayback->nMediaPlayer) /*If item equals -1 then no media has been set yet*/
    {

        if (libvlc_media_player_play(pPlayback->nMediaPlayer) != 0)
        {
            pPlayback->xPlaybackState = EHS_FB_STATE_LOADED; //@todo this should really check why and release all and set the state to NOT_LOADED?
        }
        else     /* We are playing! */
        {
            pPlayback->xPlaybackState = EHS_FB_STATE_PLAYING;
            //Sleep(10000);
// call this in-case something has changed..
#ifndef EHS_MINGW /* np point for windows where the window is created dynamically */
            // Possibility that the windfow is created here so lets try!
            if (pPlayback->mediaFormat == EHS_VIDEO)
                EhsTDPlayback_setWindow(pPVR); // set the window to the coords
#endif
            //
        }
        pPlayback->xPlaybackState = EHS_FB_STATE_PLAYING; //@todo  the above check for the exception needs to be fixed to return 0 if there is no exception (the libvlc function doesn't seem to work).
    }
    else
    {
        EHSH_LOG_ERROR("no media/player handles");
        pPlayback->xPlaybackState = EHS_FB_STATE_NOT_LOADED; //@todo we shouldn't really be changing the higher level states here - should just return the error
    }

    return bRet;
}

/**
 * Terminate video playback. This causes the screen to clear
 * @param[in] pPlayback The item to stop
 * @return true if playback structure was valid, and video was stopped
 */
ehs_bool EhsTDPlayback_stop(EhsFbPvrPlayClass* pPVR)
{
    //EhsFbPvrPlayClass* pPVR = (EhsFbPvrPlayClass*) EHS_FB_RUN_CONTEXT;
    EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*) pPVR->pTDPlayback; /* @todo This crashes on restarts */
    if (pPlayback)
    {
        if (pPlayback->xPlaybackState != EHS_FB_STATE_NOT_LOADED)
        {
            EhsTDPlayback_UnLoadMedia(pPlayback, EHS_TRUE);
            pPlayback->xPlaybackState = EHS_FB_STATE_NOT_LOADED;
        }
    }
    else
    {
        EHSH_LOG_WARNING("Stopped a media player before initialisation");
    }
}

/* Set the devices volume using specific gubbins
 * */
void EhsTDSetVol(EhsTDPlaybackClass* pPlayback, ehs_sint32 nVol)
{
    if (nVol > 100)
    {
        nVol = 100; /*The most vlc will take without breaking something.*/
    }
    else if (nVol < 0)
    {
        nVol = 0;
    }
    pPlayback->nVolume = nVol/2; /* VLC is louder than gstreamer - and distorts at 100% on fully encoded mp3s */
    //EhsTVSetVol(pPlayback->nMediaPlayer, nVol) ;
    if (pPlayback->nMediaPlayer && (pPlayback->xPlaybackState
                                    != EHS_FB_STATE_NOT_LOADED))
    {
        libvlc_audio_set_volume(pPlayback->nMediaPlayer, pPlayback->nVolume); //set the state now.
        //nVol = libvlc_audio_get_volume(pPlayback->nMediaPlayer);
    }
    return;
}

/********************************************************************************
 *  Begin the TV functions  - not sure why these are TV not TD...
 * The following functions are trused client.
 * If these are called wrongly then VLC will brake. @todo the layer above should really be abstracted
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
    /*gdk_window_move_resize(pPlayback->pVLCWindow, pWidget->xCurRect.nLeft,
     pWidget->xCurRect.nTop, pWidget->xCurRect.nWidth,
     pWidget->xCurRect.nHeight);*/
}

/****************************************************************************************************************
 * LIBVLC Specific Functions - trusted client from here on
 * ************************************************************************************************************
 */

float EhsTV_getPosition(libvlc_media_player_t* pMP)
{

    float position = 0;
    libvlc_state_t playing = libvlc_media_player_get_state(pMP);
    if ((playing == libvlc_Playing) || playing == libvlc_Paused)
    {
        if (pMP)
        {
            position = libvlc_media_player_get_position(pMP);
        }
        else
        {
        }
    }
    else
    {
    }
    return position;
}
/*
 if (!libvlc_exception_raised(EhsTV_excp)) {
 libvlc_exception_clear(&EhsTV_excp);
 */

void EhsTV_setPosition(libvlc_media_player_t* pMP, float nPos)
{

    libvlc_state_t playing = libvlc_media_player_get_state(pMP);
    if ((playing == libvlc_Playing) || playing == libvlc_Paused)
    {
        if (pMP)
        {
            libvlc_media_player_set_position(pMP, nPos);
            //libvlc_video_set_screen_position
            //libvlc_media_player_release(pMP);
        }
    }
}

/* This function writes the strange 'vlc_item' number of the current file and return the index offset from the beginning of the playlist */

ehs_sint32 EhsTV_getIndex(libvlc_media_player_t* pMP)
{
    /*
     int index = 0;
     static int calc_item = 0;
     libvlc_state_t playing = libvlc_media_player_get_state(pMP, &EhsTV_excp);
     EhsTV_CheckException( &EhsTV_excp);
     if ((playing == libvlc_Playing ) || playing == libvlc_Paused) {
     index = 0; //@todo remove this: libvlc_playlist_get_current_index(EhsTV_pLibvlc, &EhsTV_excp);
     if (index >= 0) {
     calc_item = ((index - 7) / 2) + 5 - vlc_offset; //This is how you convert from a useless index value to a useful item id 20081231
     //vlc_item = calc_item;
     } else {
     calc_item = -1;
     //vlc_item = vlc_offset; //@TODO is this a valid index number if there is nothing playing?
     }
     }
     */
    return (1); //@todo check that this returns a sensible number if not playing.
}

/* nIndex is a @todo 1-indexed value for the song in the playlist */

void EhsTV_setIndex(libvlc_media_player_t* pMP, ehs_sint32 nIndex)
{
    int numItems = 0;
    int playing = 0;
    int isPlaying = 0;
    int timeout = 0;
    /*We must make sure we don't try to jump to an index that does not exist otherwise we will upset vlc*/
    /*Get number of items on playlist*/
    /*
     *
     while ((numItems <= 1) && (timeout < 10)) {
     numItems = libvlc_playlist_items_count(EhsTV_pLibvlc, &EhsTV_excp);
     EhsSleep(EHS_TIME_ms(100));
     timeout++;
     }

     //We should only  need to do an index jump if there is more than one item in the playlist. Doing a jump with only 1 item in the playlist can upset VLC
     if ((numItems > 1) && (nIndex <= numItems)) { // don't do anything if a set list has not been set
     playing = libvlc_playlist_isplaying(EhsTV_pLibvlc, &EhsTV_excp);
     if ((nIndex <= 0) || (nIndex > numItems)) {
     //New index does not exist so do nothing
     } else {
     //if (playing) EhsTV_stop();
     //Then update global id number with new id number
     vlc_item = nIndex + vlc_offset;
     //Set vlc playing again if it was //
     isPlaying = libvlc_playlist_isplaying(EhsTV_pLibvlc, &EhsTV_excp); //@todo global var still
     if (isPlaying) {
     EhsTV_stop(pMP);
     }
     //{
     if (vlc_item != -1) //If item equals -1 then no media has been set yet/
     {
     libvlc_playlist_play(EhsTV_pLibvlc, vlc_item, 0, NULL,
     &EhsTV_excp); //@todo more globals
     }

     }
     }
     */
}

libvlc_time_t EhsTV_getTime(libvlc_media_player_t* pMP)
{
    libvlc_time_t time = 0;

    libvlc_state_t playing = libvlc_media_player_get_state(pMP);
    if ((playing == libvlc_Playing) || playing == libvlc_Paused)
    {
        //pMP = BADGLOBAL_HANDLE;//libvlc_playlist_get_media_player(EhsTV_pLibvlc, &EhsTV_excp);
        if (pMP)
        {
            time = libvlc_media_player_get_time(pMP);
        } 
    } 
    return time;
}

void EhsTV_setTime(libvlc_media_player_t* pMP, libvlc_time_t nTime)
{

    libvlc_state_t playing = libvlc_media_player_get_state(pMP);
    if ((playing == libvlc_Playing) || playing == libvlc_Paused)
    {
        if (pMP)
        {
            libvlc_media_player_set_time(pMP, nTime);
        }
    }
}

libvlc_time_t EhsTV_getLength(libvlc_media_player_t* pMP)
{
    libvlc_time_t length = 0;
    if (pMP)
    {
        length = libvlc_media_player_get_length(pMP);
    }
    return length;
}

float EhsTV_getSpeed(libvlc_media_player_t* pMP)
{

    float speed = 0;
    libvlc_state_t playing = libvlc_media_player_get_state(pMP);
    if ((playing == libvlc_Playing) || playing == libvlc_Paused)
    {
        if (pMP)
        {
            speed = libvlc_media_player_get_rate(pMP);
        }
        else
            EHSH_LOG_WARNING("No pMp in  get Speed");
    }// else don't care

    return speed;
}

void EhsTV_setSpeed(libvlc_media_player_t* pMP, float nSpeed)
{

    libvlc_state_t playing = libvlc_media_player_get_state(pMP);
    //if ((playing == libvlc_Playing ) || playing == libvlc_Paused) {
    if (pMP)
    {
        libvlc_media_player_set_rate(pMP, nSpeed);
    }
    //}
}

int EhsTV_pause(libvlc_media_player_t* pMP)   //return 0 when moving from paused to unpaused and for moving paused from playing.
{
    /*If pause is called with no input set then vlc gets upset*/
    int pause_state = 0; //default unpaused.
    libvlc_state_t playing = libvlc_media_player_get_state(pMP);
    if ((playing == libvlc_Playing) || playing == libvlc_Paused)
    {
        libvlc_media_player_pause(pMP);
    }
    if (playing == libvlc_Playing)
        pause_state = 1; //we are now paused
    else
        pause_state = 0;// or we are not
    return pause_state;
}

//@todo this function is works OK without memory leaks - it maybe best however not to used by higher layers to avoid unecessary memory re-allocation etc.
void EhsTV_getInput(libvlc_media_player_t* pMP, libvlc_media_t* pM,
                    char * input)
{
    char * input_temp;

    libvlc_state_t playing = libvlc_media_player_get_state(pMP);
    if ((playing == libvlc_Playing) || playing == libvlc_Paused)
    {

        if (pMP)
        {
            //pM = libvlc_media_player_get_media(pMP, &EhsTV_excp);
            if (pM)
            {
                input_temp = libvlc_media_get_mrl(pM);
                if (input_temp)
                {
                    EhsStrcpy(input, input_temp);
                    free(input_temp);
                }

            }
            else
            {
                EHSH_LOG_ERROR("GetInput: no media");
                EhsStrcpy(input, "NO media");
            }
        }
        else
        {
            EHSH_LOG_ERROR("GetInput: no player");
            EhsStrcpy(input, "NO media");
        }

    }

}

/* This just stops play but does not unload anything */

void EhsTV_stop(libvlc_media_player_t* pMP)
{
    int i;
    if (pMP)
    {
        libvlc_state_t playing  = libvlc_Playing; // stalls?  libvlc_media_player_get_state(pMP);
        if ((playing == libvlc_Playing) || (playing == libvlc_Paused ) )  //libvlc_Stopped ) && (playing != libvlc_Ended)) { //perhaps we need to add libvlc ==ended here too?
        {
            libvlc_media_player_stop(pMP);
            /* We'll continue to test of this is completing - but hope no iterations are required as we are blocking longer than EHS should allow. */
            playing = libvlc_media_player_get_state(pMP);
            for (i = 0; (i < 5) && ((playing != libvlc_Stopped) && (playing
                                    != libvlc_Ended)); i++)
            {
                EhsSleep(EHS_TIME_ms(10)); // @todo should This blocks so this should be a threaded function.
                libvlc_state_t playing = libvlc_media_player_get_state(pMP);
                EHSH_LOG_WARNING("Need to retry LIBVLC play stop (20ms)");
            }
            if ((playing != libvlc_Stopped) && (playing != libvlc_Ended))   //@todo should add a flag to allow some kind of forced stop of VLC next operation rather than leek the memory
            {
                EHSH_LOG_ERROR("Coouldn't stop VLC - memory leek likely playing = %d\n", playing);
            }
        } // end of were we already stopped test.


    }
    /* We don't want to release descriptors as the media can still be used.
     }
     /* We will only exit when it has stopped @todo thread this function  but we give up if this takes too @todo without an exception. */
    /*
     * @todo do we need this or does the media player function block? Note we don't like blocking for a long time in these functions
     * If we need to make the state change accurate in themedia player we should wait for call-backs from VLC to change the state.
     * Currently we won't wait as we are happy to consider this as an interface state machine that doesn't cause problems for VLC's itnernal state machine
     for (i = 0; (i < 100) && (libvlc_playlist_isplaying(EhsTV_pLibvlc,
     &EhsTV_excp)); i++) {
     EhsSleep(EHS_TIME_ms(100)); // @todo should This blocks so this should be a threaded function.
     }
     */
}

EHS_GLOBAL void EhsTDPlayback_closeWindow(EhsFbPvrPlayClass* pPvrPlay)
{
    EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*) pPvrPlay->pTDPlayback;
#ifdef EHS_GUI_SUPPORT
    ehs_bool ret;
#ifdef EHS_MINGW
    ret=CloseWindow(pPlayback->window);
#else
    ret=EHS_TRUE;
    //XCloseDisplay(pPlayback->pDisplay);
#endif
    if(ret==EHS_FALSE)
    {
        EHSH_LOG_ERROR("Failed to close window.");
    }
#endif /* EHS_GUI_SUPPORT */
}


#ifdef TRY_AND_CREATE_HWND_THAT_VLC_WILL_USE_INSTEAD_OF_CREATING_ITS_OWN//EHS_MINGW
LRESULT CALLBACK WndProc(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
    return DefWindowProc (hwnd, Message, wParam, lParam);
}
WNDCLASSEX wc;
HWND hwnd;
MSG Msg;
HMODULE HIn;
#endif

/* The following should be in the OS specific part of the code, b ut we'll ignore as VLC is depricated */

EHS_GLOBAL void EhsTDPlayback_updateZorder(EhsFbPvrPlayClass* pPvr)
{
    EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*) pPvr->pTDPlayback;
#ifdef EHS_GUI_SUPPORT
    /*	if(pPvr->video_on_top==EHS_TRUE) {
    		XRaiseWindow(pPlayback->pDisplay,pPlayback->window);
    	} else {
    		XLowerWindow(pPlayback->pDisplay,pPlayback->window);
    	}
    	XSync(pPlayback->pDisplay,FALSE);
    	*/
#endif //EHS_GUI_SUPPORT
}

//extern GtkWidget *GUIViewPortWidget; // need this pass lybfocus back..

/* Don't use g_thread_enter() stuff here we are always called in this context if we run gtk in any case */
/*EHS_GLOBAL void EhsTDPlayback_setWindow(EhsDataflowIntType windowX,
		EhsDataflowIntType windowY, EhsDataflowIntType windowW,
		EhsDataflowIntType windowH, ehs_uint8 zorder, EhsTDPlaybackClass* pPlayback) {
*/
EHS_GLOBAL void EhsTDPlayback_setWindow(EhsFbPvrPlayClass* pPvrPlay )
{

    EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*)pPvrPlay->pTDPlayback;
    GdkColor 	bg = {0x50, 0x50, 0x50, 0x50};
    //GdkScreen *screen; /*Used for getting colour map of screen*/
    //GdkColormap *colormap;/*Used for getting colour map of screen*/
#ifndef EHS_NO_MEDIAPLAYER_VIDEO
    if (pPlayback->mediaFormat == EHS_VIDEO)
    {
        if ((pPlayback->xPlaybackState == EHS_FB_STATE_LOADED) || 
		(pPlayback->xPlaybackState == EHS_FB_STATE_PLAYING) || 
		(pPlayback->xPlaybackState == EHS_FB_STATE_PAUSED))   // ignore if we have no immediate prospect of playing anything.
        {
#ifdef EHS_GUI_SUPPORT
            //only do window stuff if size is above the minimum of 21 (based on vlc1.1.2 testing) 
            //using a window size smaller crashes VLC.
            if (( pPvrPlay->nWidth_adjusted > 21) && ( pPvrPlay->nHeight_adjusted > 21))
            {
                //Create the window VLC will use to render video

#ifndef EHS_MINGW //This only works X11/linux as GTK threading does not work on win32.
		if (!gtk_init_check (NULL, NULL))
		{
			EHSH_LOG_ERROR("Could not initialise GTK - continuing anyway in case something else has already!");
		}
		//create the window if it has not already been created
		if (pPlayback->window == (Window) NULL)
		{
                	pPlayback->pGTKWindow = gtk_window_new(GTK_WINDOW_POPUP);
                	if (pPlayback->pGTKWindow)
                	{
                    		gtk_widget_set_app_paintable(pPlayback->pGTKWindow, TRUE);
                    		gtk_container_set_border_width(GTK_CONTAINER (pPlayback->pGTKWindow), 0);
                    		gtk_window_set_decorated(GTK_WINDOW(pPlayback->pGTKWindow),
                                             FALSE);
                    		gtk_window_set_resizable(GTK_WINDOW(pPlayback->pGTKWindow),
                                             TRUE);
                    /* Setup default colours and transparency */
                    //screen = gdk_screen_get_default();
                    //colormap = gdk_screen_get_rgba_colormap(screen);
                    //gtk_widget_set_default_colormap(colormap);
                    //gdk_window_set_opacity (screen,0.5); //set a default value
                    //viewColour
                    //gdk_window_clear (screen);
                   		 gtk_window_set_gravity(GTK_WINDOW(pPlayback->pGTKWindow),GDK_GRAVITY_STATIC); // want to align with GUI window without toolbars deco

                    		gtk_window_set_default_size(GTK_WINDOW(pPlayback->pGTKWindow), pPvrPlay->nWidth_adjusted, pPvrPlay->nHeight_adjusted);
                    		gtk_window_move(GTK_WINDOW(pPlayback->pGTKWindow), pPvrPlay->nXpos_adjusted, pPvrPlay->nYpos_adjusted);
                    		if (pPvrPlay->video_on_top)
                    		{
                        		gtk_window_set_keep_above(GTK_WINDOW(pPlayback->pGTKWindow),EHS_TRUE); // This doesn't go ontaop in win32 in any case.
                        		gtk_window_set_keep_below(GTK_WINDOW(pPlayback->pGTKWindow),EHS_FALSE); // This doesn't go ontaop in win32 in any case.
                    		}
                    		else
                    		{
                        		gtk_window_set_keep_above(GTK_WINDOW(pPlayback->pGTKWindow),EHS_FALSE); // This doesn't go ontaop in win32 in any case.
                        		gtk_window_set_keep_below(GTK_WINDOW(pPlayback->pGTKWindow),EHS_TRUE); // This doesn't go ontaop in win32 in any case.

                    		}
                    		//@todo make this "switchable"

                    		//gtk_window_move(GTK_WINDOW(pPlayback->pGTKWindow), windowX, windowY);
                    		if (pPlayback->pGTKWindow) gtk_widget_show(GTK_WIDGET(pPlayback->pGTKWindow));

                	}
                	else
                	{
                    		EHSH_LOG_ERROR("Could not set the video port window - using the desktop for rendering");
                	}
                	if (pPlayback->pGTKWindow->window)
                	{
                    		pPlayback->window = GDK_WINDOW_XID(gtk_widget_get_window(pPlayback->pGTKWindow));
                    		//pPlayback->window = GDK_WINDOW_HWND(pPlayback->pGTKWindow->window);
                    		gtk_widget_modify_bg(pPlayback->pGTKWindow, GTK_STATE_NORMAL,&bg);

                	}

            	}
           	 else     // We are just moving one that exists already
            	{

                	if (pPlayback->pGTKWindow)
                	{
                    		//move and resize according to our passed parameters
                    		gtk_window_move(GTK_WINDOW(pPlayback->pGTKWindow),pPvrPlay->nXpos_adjusted,pPvrPlay->nYpos_adjusted);
                    		gtk_window_resize(GTK_WINDOW(pPlayback->pGTKWindow),pPvrPlay->nWidth_adjusted,pPvrPlay->nHeight_adjusted);
                	}
            	}
#else //Create the window with native win32 code.

#ifdef TRY_AND_CREATE_HWND_THAT_VLC_WILL_USE_INSTEAD_OF_CREATING_ITS_OWN
// This didn't work very well for some reason when using the GTK viewport stuff..
if( pPlayback->window== (Window)NULL)
{

                HMODULE HIn=GetModuleHandle(NULL);
                //Step 1: Registering the Window Class
                wc.cbSize = sizeof(WNDCLASSEX);
                wc.style = 0;
                wc.lpfnWndProc = WndProc;
                wc.cbClsExtra = 0;
                wc.cbWndExtra = 0;
                wc.hInstance =HIn;
                wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
                wc.hCursor = LoadCursor(NULL, IDC_ARROW);
                wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
                wc.lpszMenuName = NULL;
                wc.lpszClassName = "inxware";
                wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

                if(!RegisterClassEx(&wc))
                {
                }

                pPlayback->window = CreateWindowEx(WS_EX_TOPMOST,TEXT("inxware"),TEXT("inxware"),WS_OVERLAPPEDWINDOW| WS_VISIBLE/*WS_POPUP*/,windowX,windowY,windowW,windowH,NULL, NULL,HIn,NULL);
                if (!pPlayback->window)
                {
                    pPlayback->window=GetDesktopWindow();
                }
                else
                {
                }
                ShowWindow(pPlayback->window,SW_SHOW);
            }
#else// NEED to wait for VLC to create its own HWND (for some reason) for direct3d to work - OK so its crap!
if( pPlayback->window==NULL)   //
{
//HWND blob=-1;
HWND window_found=FindWindowEx(NULL, NULL, NULL, "VLC (Direct3D output)");
/*
 *
 if (pPlayback->nMediaPlayer) {
	blob=libvlc_media_player_get_hwnd(pPlayback->nMediaPlayer);
}
 */
                if (window_found)
                {
                    SetWindowText(window_found,pPlayback->LoadedFilename);
                    pPlayback->window=window_found;//HWNDs are pointers so this is Good?
                } // if we've got a handle then use it = might even me the right one...
            }
            if (pPlayback->window)
            {
                //if (pPlayback)  {
                //BringWindowToTop(window_found);
                //}
                if (pPvrPlay->video_on_top)
                {
                    SetWindowPos(pPlayback->window,HWND_TOPMOST,pPvrPlay->nXpos_adjusted,pPvrPlay->nYpos_adjusted,pPvrPlay->nWidth_adjusted,pPvrPlay->nHeight_adjusted,SWP_NOACTIVATE);//SWP_NOSENDCHANGING|SWP_NOACTIVATE|SWP_ASYNCWINDOWPOS|SWP_NOREDRAW); //SWP_SHOWWINDOW
                }
                else
                {
                    SetWindowPos(pPlayback->window,HWND_BOTTOM,pPvrPlay->nXpos_adjusted,pPvrPlay->nYpos_adjusted,pPvrPlay->nWidth_adjusted,pPvrPlay->nHeight_adjusted,SWP_NOACTIVATE);//This doesn't work!.HWND_BOTTOM); // this doesn't seem to make any difference..
                }
                {
                    //	GtkWidget* blob = (GtkWidget*)(EhsTV.pMainWindow);
                    //SetFocus(GDK_WINDOW_HWND(GUIViewPortWidget->window));
                    //SetParent(window_found,GDK_WINDOW_HWND(GUIViewPortWidget->window));
                    //SendMessage(window_found, WM_KILLFOCUS, GDK_WINDOW_HWND(GUIViewPortWidget->window), NULL);
                    //SendMessage(window_found,0x0127 /*WM_CHANGEUISTATE*/,3 /*UIS_INITIALIZE*/,NULL);
                    //SendMessage(GDK_WINDOW_HWND(GUIViewPortWidget->window),0x0127 /* WM_CHANGEUISTATE*/,3 /*UIS_INITIALIZE*/,NULL);
                    //}
                    //SendMessage(window_found, WM_KILLFOCUS, NULL, NULL);
                    /*
                    SetWindowLong(hwnd, GWL_EXSTYLE,
                    			        GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED|WS_EX_TOPMOST);
                    */
                    //EnableWindow(window_found,EHS_FALSE); // we want keybpard Focus to go to the widget viewport

                }
            }
#endif
#endif

        }
        else
        {

        }


#endif /* EHS_GUI_SUPPORT */
    }
    else
    {
    }
} // end of check we have video content
#endif
}
