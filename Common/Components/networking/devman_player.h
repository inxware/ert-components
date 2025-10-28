/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/
/**
 * @file devman_player.h
 *
 * function prototypes for devman_player.c (devman player function block)
 *
 * @author: inx limited
 */

#ifndef _EHS_DEVMAN_PLAYER_H
#define _EHS_DEVMAN_PLAYER_H

/******************************************************************************/
/* Declare IDF references - depricated!!! just use the names or do want to decouple? */

#define EHS_FB_NAME_DevmanPlayer "DevmanPlayer"
#define EHS_FB_ID_DevmanPlayer 0xB0BC

/* declare functions provided by this function block */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(DevmanPlayer);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(DevmanPlayer);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(DevmanPlayer);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DevmanPlayer);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DevmanPlayer_start_checking);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DevmanPlayer_stop_checking);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DevmanPlayer_change_url);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DevmanPlayer_reconfigure);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DevmanPlayer_track_changed);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DevmanPlayer_pass_thru_get_next);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DevmanPlayer_pass_thru_send_next);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DevmanPlayer_reset);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DevmanPlayer_out);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DevmanPlayer_info); //a function executed by a start port
//EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DevmanPlayer_StartFromDisk);
//EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DevmanPlayer_clearPlaylist);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DevmanPlayer)

#endif /* _EHS_DEVMAN_PLAYER_H */
