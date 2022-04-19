/**
 * @file keypress.h
 * This declares the keypress function block
 *
 * @author: inx limited
 * @version: $Revision: 1250 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 *
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef _EHS_KEYPRESS_H
#define _EHS_KEYPRESS_H

#include "ehs_fb_types.h"
#include "callback_queue.h"

#define EHS_FB_KEYPRESS_NAME "GUI_keypress"

/*
 * Provides a callback chain to call whenever a key gets pressed.
 *
 * @todo ensure that this gets reset every time SODL is reloaded.
 */
EHS_GLOBAL EhsCallbackQueueType EhsGUIKeypressCallback;

EHS_GLOBAL void EhsGUIKbClearCBs(void ); // used by some functions that share this info @todo this shojuld go in a HAL layer.
EHS_GLOBAL void EhsGUIKbHit(ehs_uint32 nCharacter);

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(GUI_keypress);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(GUI_keypress);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_keypress_hit);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_keypress_Enable);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_keypress_Disable);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(GUI_keypress);

#endif /* _EHS_KEYPRESS_H */

