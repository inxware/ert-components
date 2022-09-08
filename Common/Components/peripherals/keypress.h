/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/**
 * @file keypress.h
 * This declares the keypress function block
 *
 * @author: inx limited
 *
 */

#ifndef _EHS_KEYPRESS_H
#define _EHS_KEYPRESS_H

#include "ehs_fb_types.h"
#include "callback_queue.h"

#define EHS_FB_NAME_GUI_keypress "GUI_keypress"
#define EHS_FB_ID_GUI_keypress 0x7CDC

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

