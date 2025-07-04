/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file keypress.h
 * This declares the keypress function block
 *
 * @author: inx limited
 *
 */

#ifndef _EHS_KEYPRESS_H
#define _EHS_KEYPRESS_H

#include "ehs_types.h"
#include "callback_queue.h"

#define EHS_FB_NAME_gtk_keypress "gtk_keypress"
#define EHS_FB_ID_gtk_keypress 0x5F52


/*
 * Provides a callback chain to call whenever a key gets pressed.
 *
 * @todo ensure that this gets reset every time SODL is reloaded.
 */
EHS_GLOBAL EhsCallbackQueueType EhsGtkKeypressCallback;


/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(gtk_keypress);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(gtk_keypress);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gtk_keypress_hit);

/* declare the table for use by the blockref table */
EHS_GLOBAL void EHS_FB_FUNCTIONS(gtk_keypress);
EHS_GLOBAL void EhsGUIKbHit(ehs_uint32 nCharacter);

#endif /* _EHS_KEYPRESS_H */

