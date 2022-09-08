/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file libvlc_dll.c
 * Imports all of the libvlc functions from the libvlc DLL
 *
 * @author: inx limited
 *
 */


/**
 * @page Verification Verification report
 * @section hal_graphics_font
 * @anchor hal_graphics_font
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on
 * Last modified on $Date:$
 *
 * This file contained no derogations to the MISRA standard.
 */

/*****************************************************************************/
/* Included files */
#include "messages.h"
#include "hal_string.h"
#include "hal_time.h"
#include "libvlc_dll.h"
#include "widget.h"
#include "graphics.h"
#include "hal_process.h"



/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/**
 * Helper macro for defining DLL functions.
 */
#define DefineLibFunction(returnType, name, params) \
	returnType (*DLL##name) params; \
	typedef returnType (*name##Type) params;

/**
 * Helper macro for assigning an address to the DLL functions
 */
#define AssignVLCLibFunction(name) \
	DLL##name = (name##Type)GetProcAddress(hLib,#name);\
	if (!DLL##name){int i = 1; i /= (i);}

/**
 * Helper macro for assigning an address to the DLL functions
 */
#define AssignVLCControlLibFunction(name) \
	DLL##name = (name##Type)GetProcAddress(hLibControl,#name);\
	if (!DLL##name){int i = 1; i /= (i);}

/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */

/**
 * Video subpicture used for displaying EHS graphics
 */
EHS_LOCAL subpicture_t* EhsL_pSubpic;

/**
 * Video output thread
 */
EHS_LOCAL vout_thread_t* EhsL_pVout;

/*****************************************************************************/
/* Variables defined with global-scope */

/**
 * Reference to the main window handle
 */
HANDLE EhsTgtMainWindowHandle;

DefineLibFunction(vout_thread_t*,		__vlc_object_find, (vlc_object_t* p_this, int i_type, int i_mode))
DefineLibFunction(void, 				__vlc_object_release, (vlc_object_t*))
DefineLibFunction(void,				__vout_OSDMessage,(vlc_object_t *, int, char *, ...))
DefineLibFunction(void, 				libvlc_destroy,( libvlc_instance_t *p_instance ))
DefineLibFunction(const char *,		libvlc_exception_get_message,( const libvlc_exception_t *p_exception ))
DefineLibFunction(int, 				libvlc_exception_raised,( const libvlc_exception_t *p_exception ))
DefineLibFunction(void,				libvlc_exception_init,( libvlc_exception_t *p_exception ))
DefineLibFunction(float,			libvlc_input_get_position, ( libvlc_input_t *p_input, libvlc_exception_t *p_e ))

DefineLibFunction(libvlc_instance_t *,	libvlc_new,( int, const char *const *, libvlc_exception_t *));
DefineLibFunction(void,				libvlc_video_set_parent,(  	libvlc_instance_t *   	,
                  libvlc_drawable_t  	,
                  libvlc_exception_t *
                                                   ))
DefineLibFunction(int,					libvlc_playlist_add,(libvlc_instance_t*, const char*, const char*, libvlc_exception_t*))
DefineLibFunction(libvlc_input_t*,	libvlc_playlist_get_input,(libvlc_instance_t*, libvlc_exception_t* p_e))
DefineLibFunction(void,				libvlc_playlist_pause, (libvlc_instance_t *, libvlc_exception_t*))
DefineLibFunction(void,				libvlc_playlist_play,(libvlc_instance_t *, int, int, char**,libvlc_exception_t*))
/** This doesn't do fullscreen as I'd hope - it intercepts/ignores windows events (eg keypresses) */
DefineLibFunction(void,				libvlc_set_fullscreen, (libvlc_input_t*, int, libvlc_exception_t*))
DefineLibFunction(void, 				libvlc_video_set_size,( libvlc_instance_t *, int, int, libvlc_exception_t * ))
DefineLibFunction(mtime_t, 			mdate, (void))
DefineLibFunction(subpicture_t*, 		spu_CreateSubpicture,(spu_t* p_spu))
DefineLibFunction(void, 				spu_DisplaySubpicture, (spu_t* p_spu, subpicture_t* p_subpic))
DefineLibFunction(void,				vout_OSDIcon,(vlc_object_t *, int i_channel, short i_type))
DefineLibFunction(int,				VLC_LengthGet,(int i_object))
DefineLibFunction(float,			VLC_PositionGet,(int i_object))
DefineLibFunction(char const*,			VLC_Version,(void ))

/*
 * The following functions are not exported by the libvlc DLL.
 */
DefineLibFunction(void,libvlc_media_release,( libvlc_media_t *p_meta_desc ))

DefineLibFunction(libvlc_media_t *,libvlc_media_new,(
                      libvlc_instance_t *p_instance,
                      const char * psz_mrl,
                      libvlc_exception_t *p_e ))

DefineLibFunction(libvlc_media_player_t *, libvlc_media_player_new_from_media,( libvlc_media_t *, libvlc_exception_t * ))
DefineLibFunction(void,libvlc_media_player_set_drawable, ( libvlc_media_player_t *, libvlc_drawable_t, libvlc_exception_t * ))
DefineLibFunction(void,libvlc_media_player_release,( libvlc_media_player_t * ))
DefineLibFunction(void,libvlc_media_player_stop,( libvlc_media_player_t *, libvlc_exception_t * ))
DefineLibFunction(void,libvlc_media_player_play,( libvlc_media_player_t *, libvlc_exception_t * ))
DefineLibFunction(void,libvlc_release,( libvlc_instance_t * ))

DefineLibFunction(libvlc_media_player_t *, libvlc_media_player_new,( libvlc_instance_t *, libvlc_exception_t * ))
DefineLibFunction(libvlc_media_list_player_t *, libvlc_media_list_player_new, ( libvlc_instance_t * p_instance, libvlc_exception_t * p_e ))
DefineLibFunction(float, libvlc_media_player_get_position, ( libvlc_media_player_t *, libvlc_exception_t *))
DefineLibFunction(void, libvlc_media_player_set_position, ( libvlc_media_player_t *, float, libvlc_exception_t *))
DefineLibFunction(libvlc_time_t, libvlc_media_player_get_time, ( libvlc_media_player_t *, libvlc_exception_t *))
DefineLibFunction(void, libvlc_media_player_set_time, ( libvlc_media_player_t *, libvlc_time_t, libvlc_exception_t *))
DefineLibFunction(libvlc_time_t, libvlc_media_player_get_length, ( libvlc_media_player_t *, libvlc_exception_t *))
DefineLibFunction(float, libvlc_media_player_get_rate, ( libvlc_media_player_t *, libvlc_exception_t *))
DefineLibFunction(void, libvlc_media_player_set_rate, ( libvlc_media_player_t *, float, libvlc_exception_t *))
DefineLibFunction(void, libvlc_media_player_pause, ( libvlc_media_player_t *, libvlc_exception_t * ))
DefineLibFunction(char *, libvlc_media_get_mrl, ( libvlc_media_t * p_md, libvlc_exception_t * p_e ))
DefineLibFunction(libvlc_media_t *, libvlc_media_player_get_media, ( libvlc_media_player_t *, libvlc_exception_t * ))
DefineLibFunction(void, libvlc_media_player_set_media, ( libvlc_media_player_t *, libvlc_media_t *, libvlc_exception_t * ))
DefineLibFunction(int, libvlc_media_get_id, (libvlc_media_player_t *, libvlc_exception_t *))
DefineLibFunction(libvlc_media_player_t *, libvlc_playlist_get_media_player, (libvlc_instance_t *, libvlc_exception_t *))
DefineLibFunction(int, libvlc_playlist_isplaying, (libvlc_media_player_t *, libvlc_exception_t *))

/**
 * Reference of the libvlc instance
 */
libvlc_instance_t * EhsTV_pLibvlc;
libvlc_media_list_player_t* pMediaListPlayer;
libvlc_media_list_t* pMediaList;
libvlc_media_player_t* pMediaPlayer;
static ehs_bool vlcInited = EHS_FALSE;
/**
 * Exception handler for libvlc functions
 */
libvlc_exception_t EhsTV_excp;

/*****************************************************************************/
/* Global Function definitions */

/**
 * Initialise VLC.
 * - Load DLL definitions
 * - set up exception handler
 * - call libvlc_new
 */
void EhsTV_initVlc(const EhsGraphicsRectangleClass* pBounds)
{
    /* @TODO: make use of pBounds*/
    ehs_bool bRet = EHS_TRUE;

    /* Initialise VLC library */
    //HINSTANCE hLib = LoadLibraryA(EHSTV_LIBVLC_PATH);
    HINSTANCE hLib;
    HINSTANCE hLibControl;

    hLib = LoadLibraryA(".\\libvlc.dll");
    hLibControl = LoadLibraryA(".\\libvlc-control.dll");

    if (hLib)
    {
        //	AssignLibFunction(__vlc_object_find)
        //	AssignLibFunction( __vlc_object_release)
        //	AssignLibFunction(__vout_OSDMessage);
        AssignVLCLibFunction(VLC_LengthGet);
        AssignVLCLibFunction(VLC_PositionGet);
        //	AssignVLCControlLibFunction(VLC_Version);
    }
    else
    {
        DWORD err = GetLastError();

        EhsError("Couldn't find libvlc-control.dll");
        bRet = EHS_FALSE;
    }
    if (hLibControl)
    {


        //	AssignVLCControlLibFunction( libvlc_destroy);
        AssignVLCControlLibFunction(libvlc_exception_get_message);
        AssignVLCControlLibFunction( libvlc_exception_raised);
        AssignVLCControlLibFunction(libvlc_exception_init);
        //	AssignVLCControlLibFunction(libvlc_input_get_position);
        AssignVLCControlLibFunction(libvlc_new);
        AssignVLCControlLibFunction(libvlc_media_new);
        AssignVLCControlLibFunction(libvlc_media_list_player_new);
        AssignVLCControlLibFunction(libvlc_media_player_new);
        AssignVLCControlLibFunction(libvlc_video_set_parent);
        AssignVLCControlLibFunction(libvlc_media_player_get_position);
        AssignVLCControlLibFunction(libvlc_media_player_set_position);
        AssignVLCControlLibFunction(libvlc_media_player_get_time);
        AssignVLCControlLibFunction(libvlc_media_player_set_time);
        AssignVLCControlLibFunction(libvlc_media_player_get_length);
        AssignVLCControlLibFunction(libvlc_media_player_get_rate);
        AssignVLCControlLibFunction(libvlc_media_player_set_rate);
        AssignVLCControlLibFunction(libvlc_media_player_pause);
        AssignVLCControlLibFunction(libvlc_media_get_mrl);
        AssignVLCControlLibFunction(libvlc_media_player_get_media);
        AssignVLCControlLibFunction(libvlc_media_player_set_drawable);
        AssignVLCControlLibFunction(libvlc_media_player_set_media);
        AssignVLCControlLibFunction(libvlc_media_player_set_media);
        AssignVLCControlLibFunction(libvlc_media_player_release);
        AssignVLCControlLibFunction(libvlc_media_player_stop);
        AssignVLCControlLibFunction(libvlc_media_player_play);
        AssignVLCControlLibFunction(libvlc_release);
        AssignVLCControlLibFunction(libvlc_media_get_id);
        AssignVLCControlLibFunction(libvlc_playlist_get_media_player);
        AssignVLCControlLibFunction(libvlc_playlist_isplaying);

        AssignVLCControlLibFunction(libvlc_playlist_add);
        //	AssignVLCControlLibFunction(libvlc_playlist_get_input);
        AssignVLCControlLibFunction(libvlc_playlist_play);
        AssignVLCControlLibFunction(libvlc_playlist_pause);
        //	AssignVLCControlLibFunction(libvlc_set_fullscreen);
        AssignVLCControlLibFunction(libvlc_video_set_size);
        //	AssignVLCControlLibFunction(mdate);
        //	AssignVLCControlLibFunction(spu_CreateSubpicture);
        //	AssignVLCControlLibFunction(spu_DisplaySubpicture);
        //	AssignVLCControlLibFunction(vout_OSDIcon);

    }
    else
    {
        DWORD err = GetLastError();

        EhsError("Couldn't find libvlc.dll");
        bRet = EHS_FALSE;
    }

    if (vlcInited)
    {
        /* @TODO move vlc window to new pBounds */
        SetWindowPos(EhsTgtMainWindowHandle,HWND_TOP, pBounds->nLeft,pBounds->nTop,pBounds->nWidth,pBounds->nHeight,SWP_SHOWWINDOW);

    }
    else
    {
        const char * const vlc_args[] = { "--vout=wingdi", "--no-video-title-show", "--no-osd",  "--plugin-path=../bin/modules",NULL}; //"-I", "dummy", "--no-dummy-quiet", NULL};

        SetWindowPos(EhsTgtMainWindowHandle,HWND_TOP, pBounds->nLeft,pBounds->nTop,pBounds->nWidth,pBounds->nHeight,SWP_SHOWWINDOW);

        /* set VLC exception handler */
        DLLlibvlc_exception_init(&EhsTV_excp);

        /* init vlc modules, should be done only once */

        EhsTV_pLibvlc = DLLlibvlc_new( 4, vlc_args, &EhsTV_excp ); /* @TODO: number of input args is wrong */
        //EhsTV_pLibvlc = libvlc_new( 4, vlc_args, &EhsTV_excp );
        EhsTV_quit_on_exception( &EhsTV_excp );

        /*Initialise media list player*/
        pMediaListPlayer = DLLlibvlc_media_list_player_new(EhsTV_pLibvlc, &EhsTV_excp);
        EhsTV_quit_on_exception( &EhsTV_excp );

        /*Initialise media player*/
        pMediaPlayer = DLLlibvlc_media_player_new(EhsTV_pLibvlc, &EhsTV_excp);
        EhsTV_quit_on_exception( &EhsTV_excp );

        /* Get our media instance to use our window */
        DLLlibvlc_media_player_set_drawable(pMediaPlayer, (libvlc_drawable_t)EhsTgtMainWindowHandle, &EhsTV_excp);
        EhsTV_quit_on_exception( &EhsTV_excp );

        /* put the libvlc display on our main window */
        //libvlc_video_set_parent(EhsTV_pLibvlc,(libvlc_drawable_t)GDK_WINDOW_XID(vlcWindow),&EhsTV_excp);
        //EhsTV_quit_on_exception(& EhsTV_excp);
        vlcInited = EHS_TRUE;

        /* put the libvlc display on our main window */
        DLLlibvlc_video_set_parent(EhsTV_pLibvlc,(libvlc_drawable_t)EhsTgtMainWindowHandle,&EhsTV_excp);
        EhsTV_quit_on_exception( &EhsTV_excp );
        /*
        DLLlibvlc_video_set_size(EhsTV_pLibvlc, EHS_TV_RES_X, EHS_TV_RES_Y, &EhsTV_excp);
        EhsTV_quit_on_exception(& EhsTV_excp);*/

        /* start a background video playing */
        /*
        EhsTV_backgroundId = DLLlibvlc_playlist_add(EhsTV_pLibvlc, "C:\\VideoOutput\\PeterPan.mpg", NULL, &EhsTV_excp);
        EhsTV_quit_on_exception( &EhsTV_excp );

        DLLlibvlc_playlist_play(EhsTV_pLibvlc, EhsTV_backgroundId, 0, NULL, &EhsTV_excp);
        EhsTV_quit_on_exception( &EhsTV_excp );*/

        /* set our display thread to 'not currently used' */
        EhsL_pVout = NULL;

    }
}

/**
 * Get pointers to the Y,U,V,A planes and the width/height of the main display. This isn't used
 * because the OSD is performed on a transparent window "above" the video window.
 */
void EhsTV_getSurface(ehs_uint16* pnWid, ehs_uint16* pnHt, ehs_uint8** ppY, ehs_uint8** ppU, ehs_uint8** ppV, ehs_uint8** ppA)
{

    int i_channel = 1;
    ehs_bool bRet = EHS_TRUE;
    picture_heap_t* p_render;
    spu_t* pSpu = NULL;

    if (EhsL_pVout)
    {
        EhsError("**Error: Output thread already initialised\n");
        bRet = EHS_FALSE;
    }

    if (bRet)
    {
        /* get the display thread */
        EhsL_pVout = DLL__vlc_object_find((vlc_object_t*)EhsTV_pLibvlc->p_vlc, VLC_OBJECT_VOUT, FIND_ANYWHERE);
        if (!EhsL_pVout)
        {
            EhsError("**Error: __vlc_object_find failed\n");
            bRet = EHS_FALSE;
        }
    }

    if (bRet)
    {
        /* determine screen width/height.
         * Nasty hack here. Can't get vout_thread_t to match the
         * structure generated by gcc build of libvlc. This adds a magic offset to access the render
         * structure correctly */
        p_render = (picture_heap_t*)((uint8_t*)(EhsL_pVout)+572);
        if (!p_render)
        {
            EhsError("**Error: EhsTV_getSurface: p_render is null");
            bRet = EHS_FALSE;
        }
    }

    if (bRet)
    {
        /* set up the subpicture */
        /* Nasty hack: I haven't got the vout_thread to match the details of the structure used by the gcc
         * build of libvlc, so this adds a magic offset to EhsL_pSpu, to allow us to access it correctly
         */
        pSpu = *(spu_t**)((uint8_t*)&(EhsL_pVout->p_spu) + 72);
        if (!pSpu)
        {
            EhsError("**Error: EhsTV_getSurface: pSpu is null");
            bRet = EHS_FALSE;
        }
    }

    if (bRet)
    {
        EhsL_pSubpic = DLLspu_CreateSubpicture(pSpu);
        if (!EhsL_pSubpic)
        {
            EhsError("**Error: spu_CreateSubpicture failed");
            bRet = EHS_FALSE;
        }
    }

    if (bRet)
    {
        video_format_t fmt;

        /* set up properties of EhsL_pSubpic */
        EhsL_pSubpic->i_channel = i_channel;
        EhsL_pSubpic->i_start = DLLmdate();
        EhsL_pSubpic->i_stop = 0;		/* item stays on display forever */
        EhsL_pSubpic->b_ephemer = EHS_TRUE;
        EhsL_pSubpic->b_fade = EHS_TRUE;
        EhsL_pSubpic->b_absolute = EHS_TRUE;
        EhsL_pSubpic->i_original_picture_width = EHS_TV_RES_X;
        EhsL_pSubpic->i_original_picture_height = EHS_TV_RES_Y;

        /* set properties of video_format_t */
        memset(&fmt,0,sizeof(video_format_t));
        fmt.i_chroma = VLC_FOURCC('Y','U','V','A');
        fmt.i_aspect = 0;
        fmt.i_width = fmt.i_visible_width = EHS_TV_RES_X; //p_render->i_width;
        fmt.i_height = fmt.i_visible_height = EHS_TV_RES_Y; //p_render->i_height;
        fmt.i_x_offset = fmt.i_y_offset = 0;
        EhsL_pSubpic->p_region = EhsL_pSubpic->pf_create_region((vlc_object_t*)(pSpu), &fmt );

        EhsL_pSubpic->p_region->i_x = 0; /* LHS of region */
        EhsL_pSubpic->p_region->i_y = 0; /* Top of region */

        *ppY = EhsL_pSubpic->p_region->picture.Y_PIXELS;
        *ppU = EhsL_pSubpic->p_region->picture.U_PIXELS;
        *ppV = EhsL_pSubpic->p_region->picture.V_PIXELS;
        *ppA = EhsL_pSubpic->p_region->picture.A_PIXELS;

        *pnWid = EHS_TV_RES_X; //EhsL_pSubpic->p_region->fmt.i_width;
        *pnHt = EHS_TV_RES_Y; //EhsL_pSubpic->p_region->fmt.i_height;
    }
}

/**
 * Tell VLC that we're ready for it to display this surface
 */
void EhsTV_displayAndReleaseSurface(void)
{
    /**
     * Subpicture unit used for displaying EHS graphics.
     */
    spu_t* pSpu;

    if (EhsL_pVout)
    {
        /* Nasty hack: I haven't got the vout_thread to match the details of the structure used by the gcc
         * build of libvlc, so this adds a magic offset to EhsL_pSpu, to allow us to access it correctly
         */
        pSpu = *(spu_t**)((uint8_t*)&(EhsL_pVout->p_spu) + 72);
        DLLspu_DisplaySubpicture(pSpu, EhsL_pSubpic);

        DLL__vlc_object_release((vlc_object_t*)EhsL_pVout);
        EhsL_pVout = NULL;
    } /* else nothing to display */
}


