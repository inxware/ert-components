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

#include "ehs_types.h"
#include "callback_queue.h"

#ifndef EHRT1
#define EHS_FB_KEYPRESS_NAME "gtk_keypress"
#else
#define EHS_FB_KEYPRESS_NAME 0x38B4
#endif

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

