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

#ifndef _EHS_GUIPATCH_H
#define _EHS_GUIPATCH_H

#include "ehs_fb_types.h"

/*****************************************************************************/
/* patch */

#define EHS_FB_GUI_PATCH_NAME "gui_patch"

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(gui_patch);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(gui_patch);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_patch_create);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_patch_destroy);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_patch_show);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_patch_hide);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_patch_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_patch)

#endif /* _EHS_GUIPATCH_H */

