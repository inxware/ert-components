/** 
 * @file dtv_pvr_play.h
 * This declares the PVR playback functions for the DTV toolkit. 
 *
 * @author: inx limited
 * @version: $Revision: 1650 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef _EHS_DTV_PVR_PLAY_H
#define _EHS_DTV_PVR_PLAY_H

#include "ehs_fb_types.h"


#define EHS_FB_DTV_PVR_PLAY_NAME "DtvPvrPlay1"
#define EHS_FB_DTV_PVR_PLAY_GET_NAME "get"
#define EHS_FB_DTV_PVR_PLAY_SET_NAME "set"
#define EHS_FB_DTV_PVR_PLAY_PLAY_NAME "play"
#define EHS_FB_DTV_PVR_PLAY_PAUSE_NAME "pause"
#define EHS_FB_DTV_PVR_PLAY_REV_NAME "reverse"
#define EHS_FB_DTV_PVR_PLAY_FASTER_NAME "faster"
#define EHS_FB_DTV_PVR_PLAY_SLOWER_NAME "slower"
#define EHS_FB_DTV_PVR_PLAY_STOP_NAME "stop"
#define EHS_FB_DTV_PVR_PLAY_END_NAME "end"

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DtvPvrPlay1);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DtvPvrPlay1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrPlay1_set);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrPlay1_get);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrPlay1_play);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrPlay1_pause);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrPlay1_rev);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrPlay1_faster);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrPlay1_slower);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrPlay1_stop);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrPlay1_end);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DtvPvrPlay1)

#endif /* _EHS_DTV_PVR_H */

