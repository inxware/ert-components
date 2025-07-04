/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/
#ifndef __target_os_dtv_h
#define __target_os_dtv_h

#include "hal_dtv.h"
#include "android_native_app_glue.h"

#define EHS_ANDROID_AV_ID_STR_SIZE 6

struct EhsTDPlaybackStruct
{
    EhsFBMediaPlayerStateEnum xPlaybackState; //= EHS_FB_STATE_NOT_LOADED; /**< What state is the playback in? */
    ehs_sint32 nPlaySpeed; //= 0; /**< What speed are we playing at the current time? */
    ehs_char LoadedFilename[EHS_TD_FILES_MAX_PATH];
    ehs_bool mediaFormat;
    int nVolume;
    ehs_char id[EHS_ANDROID_AV_ID_STR_SIZE];
    EhsFunctionInstanceDataType *pFIdata; //needed for call backs on gstreamer bus events
} EhsTDPlaybackStructClass;

EHS_GLOBAL ehs_bool EhsTDPlayback_initJNI(struct android_app* state);

EHS_GLOBAL void* EhsTDPlayback_init(EhsFbPvrPlayClass* pPvrPlay);

EHS_GLOBAL ehs_bool EhsTDPlayback_JNI_AV_Command(const ehs_char* jniID, const ehs_char* jniCmdName, const ehs_char* jniData);

EHS_GLOBAL int EhsTDPlayback_JNI_AV_GetIntAttribute(const ehs_char* jniID, const ehs_char* jniAttribName);
#endif