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
 *
 */

#ifndef _EHS_DTV_PVR_PLAY_H
#define _EHS_DTV_PVR_PLAY_H

#include "ehs_fb_types.h"


#define EHS_FB_NAME_DtvPvrPlay1 "DtvPvrPlay1"
#define EHS_FB_ID_DtvPvrPlay1 0x823B


/* declare functions provided by this function block */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(DtvPvrPlay1);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(DtvPvrPlay1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DtvPvrPlay1_set);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DtvPvrPlay1_get);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DtvPvrPlay1_play);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DtvPvrPlay1_pause);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DtvPvrPlay1_rev);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DtvPvrPlay1_faster);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DtvPvrPlay1_slower);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DtvPvrPlay1_stop);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DtvPvrPlay1_end);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DtvPvrPlay1)

#endif /* _EHS_DTV_PVR_H */

