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

#ifndef _EHS_GUITEXTDISPLAY_H
#define _EHS_GUITEXTDISPLAY_H

#include "ehs_fb_types.h"

/*****************************************************************************/
/* String textbox */

#define EHS_FB_GUI_TEXTDISPLAY_NAME "gui_textdisplay"

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(gui_textdisplay);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(gui_textdisplay);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_textdisplay_update);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_textdisplay_clear);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_textdisplay_poll);
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_textdisplay)

#endif /* _EHS_GUITEXTDISPLAY_H */

