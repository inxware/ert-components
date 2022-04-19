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

#ifndef _EHS_GUITEXTBOXA_H
#define _EHS_GUITEXTBOXA_H

#include "ehs_fb_types.h"

/*****************************************************************************/
/* String textbox */

#define EHS_FB_GUI_TEXT_STRING_NAME "gui_text_string"

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(gui_text);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(gui_text);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_create);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_destroy);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_show);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_hide);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_string_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_string)

/*****************************************************************************/
/* "Real" textbox */

#ifdef EHS_TARGET_FP_SUPPORT
#define EHS_FB_GUI_TEXT_FLOAT_NAME "gui_text_real"

/* declare functions provided by this function block */
/* identify function, create function and destroy functions are common
   to all textbox functions */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_float_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_float)
#endif

/*****************************************************************************/
/* Integer textbox */

#define EHS_FB_GUI_TEXT_INT_NAME "gui_text_int"

/* declare functions provided by this function block */
/* identify function, create function and destroy functions are common
   to all textbox functions */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_int_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_int)

/*****************************************************************************/
/* Boolean textbox */

#define EHS_FB_GUI_TEXT_BOOL_NAME "gui_text_bool"

/* declare functions provided by this function block */
/* identify function, create function and destroy functions are common
   to all textbox functions */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_bool_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_bool)

#endif /* _EHS_GUITEXTBOXA_H */

