/** 
 * @file guiimage.h
 * This declares the guiimage (bitmap) function block
 *
 * @author: inx limited
 * @version: $Revision: 1248 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef _EHS_GUIIMAGE1_H
#define _EHS_GUIIMAGE1_H

#include "ehs_fb_types.h"

#define EHS_FB_GUIIMAGE1_NAME "GUI_Image1"

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(GUI_Image1);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(GUI_Image1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_Image1_create);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_Image1_destroy);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_Image1_show);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_Image1_hide);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_Image1_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(GUI_Image1)
#endif /* _EHS_GUIIMAGE1_H */

