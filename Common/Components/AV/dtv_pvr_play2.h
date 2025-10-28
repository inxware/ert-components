/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * @file dtv_pvr_play.h
 * This declares the PVR playback functions for the DTV toolkit.
 *
 * @author: inx limited
 */

#ifndef _EHS_DTV_PVR_PLAY_H
#define _EHS_DTV_PVR_PLAY_H

#include "ehs_fb_types.h"


#define EHS_AV_URI_PREFIX_FILE "file:///"

#define EHS_FB_NAME_DtvPvrPlay2 "DtvPvrPlay2"
#define EHS_FB_ID_DtvPvrPlay2 0x837B




#define EHS_FB_NAME_DTV_PVR_PLAY_ERR_NAME "error"
#define EHS_FB_ID_DTV_PVR_PLAY_ERR_NAME 0xBE5A


#define EHS_FB_NAME_DTV_PVR_PLAY_SET_INDEX_NAME "playIndex"
#define EHS_FB_ID_DTV_PVR_PLAY_SET_INDEX_NAME 0x5BB3


/* declare functions provided by this function block */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(DtvPvrPlay2);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(DtvPvrPlay2);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(DtvPvrPlay2);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DtvPvrPlay2_set_input);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DtvPvrPlay2_set_pos);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DtvPvrPlay2_set_time);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DtvPvrPlay2_get);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DtvPvrPlay2_play);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DtvPvrPlay2_pause);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DtvPvrPlay2_rev);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DtvPvrPlay2_faster);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DtvPvrPlay2_slower);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DtvPvrPlay2_stop);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DtvPvrPlay2_end);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DtvPvrPlay2_set_volume);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DtvPvrPlay2_set_window);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DtvPvrPlay2_to_front);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DtvPvrPlay2_to_back);
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DtvPvrPlay2)

#endif /* _EHS_DTV_PVR_H */

