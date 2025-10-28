/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * @file playManager.h
 * This declares the xml functions.
 *
 * @author: inx limited
 *
 */

#ifndef _EHS_MEDIA_PLAYMANAGER_H
#define _EHS_MEDIA_PLAYMANAGER_H

#include "ehs_fb_types.h"


#define EHS_FB_NAME_PlayManager "PlayManager"
#define EHS_FB_ID_PlayManager 0xB6D9

#define EHS_FB_NAME_Next_Playlist "Next_Playlist"
#define EHS_FB_ID_Next_Playlist 0xB4CE

#define EHS_FB_NAME_Tick "Tick"
#define EHS_FB_ID_Tick 0x33A9

#define EHS_FB_NAME_Played "Played"
#define EHS_FB_ID_Played 0xC778

#define EHS_FB_NAME_Started "Started"
#define EHS_FB_ID_Started 0xA0A5

#define EHS_FB_NAME_Next_URL "Next_URL"
#define EHS_FB_ID_Next_URL 0x5834

#define EHS_FB_NAME_Got "Got"
#define EHS_FB_ID_Got 0xC29D

#define EHS_FB_NAME_Set_Paths "Set_Paths"
#define EHS_FB_ID_Set_Paths 0x83E7

#define EHS_FB_MEDIA_PLAYMANAGER_DAY_SECONDS 86400
/* declare functions provided by this function block */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(PlayManager);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(PlayManager);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(PlayManager_Next_Playlist);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(PlayManager_Tick);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(PlayManager_Played);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(PlayManager_Started);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(PlayManager_Next_URL);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(PlayManager_Got);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(PlayManager_Set_Paths);
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(PlayManager)

#endif /* _EHS_MEDIA_PLAYMANAGER_H */

