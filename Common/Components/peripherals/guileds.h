/**
 * @file guitextbox.h
 * This declares the guitextbox function blocks
 *
 * @author: inx limited
 * @version: $Revision: 1248 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 *
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef _EHS_GUILEDS_H
#define _EHS_GUILEDS_H

#include "ehs_fb_types.h"

/*****************************************************************************/
/* String textbox */

#define EHS_FB_GUI_LEDS_NAME "gui_leds"

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(gui_leds);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(gui_leds);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_leds_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_leds)

#endif /* _EHS_GUITEXTDISPLAY_H */
