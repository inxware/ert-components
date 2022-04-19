/**
 * @file guitextbox.h
 * This declares the guitextbox function blocks
 *
 * @author: inx limited
 * @version: $Revision: 958 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 *
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef _EHS_GUIVIEWPORT_H
#define _EHS_GUIVIEWPORT_H

#include "ehs_fb_types.h"

/*****************************************************************************/
/* patch */


#define EHS_FB_GUI_VIEWPORT_NAME "gui_viewport"

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(gui_viewport);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(gui_viewport);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_viewport_create);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_viewport_destroy);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_viewport_show);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_viewport_hide);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_viewport_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_viewport)

#endif /* _EHS_GUIVIEWPORT_H */

