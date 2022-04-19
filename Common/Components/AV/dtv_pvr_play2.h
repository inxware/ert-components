/**
 * @file dtv_pvr_play.h
 * This declares the PVR playback functions for the DTV toolkit.
 *
 * @author: inx limited
 * @version: $Revision: 3991 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 *
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef _EHS_DTV_PVR_PLAY_H
#define _EHS_DTV_PVR_PLAY_H

#include "ehs_fb_types.h"


#define EHS_AV_URI_PREFIX_FILE "file:///"

#define EHS_FB_DTV_PVR_PLAY_NAME "DtvPvrPlay2"

#define EHS_FB_DTV_PVR_PLAY_GET_NAME "get"
#define EHS_FB_DTV_PVR_PLAY_SET_INPUT_NAME "setinput"
#define EHS_FB_DTV_PVR_PLAY_SET_POS_NAME "setpos"
#define EHS_FB_DTV_PVR_PLAY_SET_TIME_NAME "settime"
#define EHS_FB_DTV_PVR_PLAY_PLAY_NAME "play"
#define EHS_FB_DTV_PVR_PLAY_PAUSE_NAME "pause"
#define EHS_FB_DTV_PVR_PLAY_REV_NAME "reverse"
#define EHS_FB_DTV_PVR_PLAY_FASTER_NAME "faster"
#define EHS_FB_DTV_PVR_PLAY_SLOWER_NAME "slower"
#define EHS_FB_DTV_PVR_PLAY_STOP_NAME "stop"
#define EHS_FB_DTV_PVR_PLAY_END_NAME "end"
#define EHS_FB_DTV_PVR_PLAY_ERR_NAME "error"
#define EHS_FB_DTV_PVR_PLAY_SET_WINDOW_NAME "setWindow"
#define EHS_FB_DTV_PVR_PLAY_SET_INDEX_NAME "playIndex"
#define EHS_FB_DTV_PVR_PLAY_TO_FRONT "toFront"
#define EHS_FB_DTV_PVR_PLAY_TO_BACK "toBack"

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DtvPvrPlay2);
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(DtvPvrPlay2);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DtvPvrPlay2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrPlay2_set_input);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrPlay2_set_pos);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrPlay2_set_time);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrPlay2_get);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrPlay2_play);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrPlay2_pause);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrPlay2_rev);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrPlay2_faster);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrPlay2_slower);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrPlay2_stop);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrPlay2_end);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrPlay2_set_volume);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrPlay2_set_window);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrPlay2_to_front);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrPlay2_to_back);
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DtvPvrPlay2)

#endif /* _EHS_DTV_PVR_H */

