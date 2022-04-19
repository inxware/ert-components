/**
 * @file devman_player.h
 *
 * function prototypes for devman_player.c (devman player function block)
 *
 * @author: Sam Gardner
 * @version: $Revision: 1248 $
 * @date: $Date: $
 * 
 * Copyright (c) inx limited, 2008. All rights reserved.
 */

#ifndef _EHS_DEVMAN_PLAYER_H
#define _EHS_DEVMAN_PLAYER_H

/******************************************************************************/
/* Declare IDF references - depricated!!! just use the names or do want to decouple? */
#define EHS_FB_DEVMAN_PLAYER_NAME "DevmanPlayer"
#define EHS_FB_DEVMAN_PLAYER_START_CHECKING "startchecking"
#define EHS_FB_DEVMAN_PLAYER_STOP_CHECKING "stopchecking"
#define EHS_FB_DEVMAN_PLAYER_CHANGE_URL "changeurl"
#define EHS_FB_DEVMAN_PLAYER_RECONFIGURE "reconfigure"
#define EHS_FB_DEVMAN_PLAYER_TRACK_CHANGED "trackchanged"
#define EHS_FB_DEVMAN_PLAYER_PASS_THRU_NEXT "passthru_next"
#define EHS_FB_DEVMAN_PLAYER_PASS_THRU_SEND "passthru_send"
#define EHS_FB_DEVMAN_PLAYER_RESET "reset"
#define EHS_FB_DEVMAN_PLAYER_OUT "out"
//#define EHS_FB_DEVMAN_PLAYER_START_FROM_DISK "startfromdisk"
//#define EHS_FB_DEVMAN_PLAYER_CLEAR_PLAYLIST "clearplaylist"

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DevmanPlayer);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DevmanPlayer);
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(DevmanPlayer);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanPlayer);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanPlayer_start_checking);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanPlayer_stop_checking);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanPlayer_change_url);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanPlayer_reconfigure);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanPlayer_track_changed);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanPlayer_pass_thru_get_next);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanPlayer_pass_thru_send_next);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanPlayer_reset);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanPlayer_out);
//EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanPlayer_StartFromDisk);
//EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanPlayer_clearPlaylist);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DevmanPlayer)

#endif /* _EHS_DEVMAN_PLAYER_H */
