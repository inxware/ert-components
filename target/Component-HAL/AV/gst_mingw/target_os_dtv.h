/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/
#ifndef __target_os_dtv_h
#define __target_os_dtv_h

#include <gst/gst.h>
#include "hal_dtv.h"

typedef struct EhsTDPlaybackStruct
{
    EhsFBMediaPlayerStateEnum xPlaybackState; //= EHS_FB_STATE_NOT_LOADED; /**< What state is the playback in? */
    ehs_sint32 nPlaySpeed; //= 0; /**< What speed are we playing at the current time? */
    GstElement *playbin;//the target specific object that we interact with for playback
    ehs_char LoadedFilename[EHS_TD_FILES_MAX_PATH];
    ehs_bool mediaFormat;
    int nVolume;
    EhsFunctionInstanceDataType *pFIdata; //needed for call backs on gstreamer bus events
    EhsFbPvrPlayClass *pPvrPlay;
#ifdef EHS_GUI_SUPPORT
    HWND window;
#endif /*EHS_GUI_SUPPORT*/
} EhsTDPlaybackStructClass;


//EHS_GLOBAL Window EhsTDPlayback_createWindow(EhsFbPvrPlayClass* pPvrPlay);


EHS_GLOBAL void* EhsTDPlayback_init(EhsFbPvrPlayClass* pPvrPlay);

EHS_GLOBAL void EhsTDPlayback_setWindow(EhsFbPvrPlayClass* pPvrPlay);

EHS_GLOBAL void EhsTDPlayback_updateZorder(EhsFbPvrPlayClass* pPvr);

#endif