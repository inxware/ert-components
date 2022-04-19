/** 
 * @file guitextbox.h
 * This declares the guitextbox function blocks
 *
 * @author: inx limited
 * @version: $Revision: 2185 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef _EHS_GUITEXTBOX2_H
#define _EHS_GUITEXTBOX2_H

#include "ehs_fb_types.h"

/*****************************************************************************/
/* String textbox */

#define EHS_FB_GUI_TEXT_STRING_NAME "gui_text_string2"

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(gui_text2);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(gui_text2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text2_create);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text2_destroy);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text2_show);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text2_hide);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_string2_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_string2)

/*****************************************************************************/
/* "Real" textbox */

//#ifdef EHS_TARGET_FP_SUPPORT
#define EHS_FB_GUI_TEXT_FLOAT_NAME "gui_text_real2"

/* declare functions provided by this function block */
/* identify function, create function and destroy functions are common
   to all textbox functions */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_float2_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_float2)
//#endif

/*****************************************************************************/
/* Integer textbox */

#define EHS_FB_GUI_TEXT_INT_NAME "gui_text_int2"

/* declare functions provided by this function block */
/* identify function, create function and destroy functions are common
   to all textbox functions */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_int2_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_int2)

/*****************************************************************************/
/* Boolean textbox */

#define EHS_FB_GUI_TEXT_BOOL_NAME "gui_text_bool2"

/* declare functions provided by this function block */
/* identify function, create function and destroy functions are common
   to all textbox functions */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_bool2_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_bool2)

#endif /* _EHS_GUITEXTBOX2_H */

