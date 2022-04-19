/**
 * @file playManager.h
 * This declares the xml functions.
 *
 * @author: Patrick Beaumont
 * @version: $Revision: 1650 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 *
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef _EHS_MEDIA_PLAYMANAGER_H
#define _EHS_MEDIA_PLAYMANAGER_H

#include "ehs_fb_types.h"


#define EHS_FB_MEDIA_PLAYMANAGER_NAME "PlayManager"
#define EHS_FB_MEDIA_PLAYMANAGER_NEXT_PLAYLIST_NAME "Next_Playlist"
#define EHS_FB_MEDIA_PLAYMANAGER_TICK_NAME "Tick"
#define EHS_FB_MEDIA_PLAYMANAGER_PLAYED_NAME "Played"
#define EHS_FB_MEDIA_PLAYMANAGER_STARTED_NAME "Started"
#define EHS_FB_MEDIA_PLAYMANAGER_NEXT_URL_NAME "Next_URL"
#define EHS_FB_MEDIA_PLAYMANAGER_GOT_NAME "Got"
#define EHS_FB_MEDIA_PLAYMANAGER_SET_PATHS_NAME "Set_Paths"
#define EHS_FB_MEDIA_PLAYMANAGER_DAY_SECONDS 86400
/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(PlayManager);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(PlayManager);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PlayManager_Next_Playlist);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PlayManager_Tick);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PlayManager_Played);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PlayManager_Started);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PlayManager_Next_URL);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PlayManager_Got);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PlayManager_Set_Paths);
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(PlayManager)

#endif /* _EHS_MEDIA_PLAYMANAGER_H */

