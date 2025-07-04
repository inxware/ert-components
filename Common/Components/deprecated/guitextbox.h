/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * @file guitextbox.h
 * This declares the guitextbox function blocks
 *
 * @author: inx limited
 *
 */

#ifndef _EHS_GUITEXTBOXA_H
#define _EHS_GUITEXTBOXA_H

#include "ehs_fb_types.h"

/*****************************************************************************/
/* String textbox */

#define EHS_FB_NAME_gui_text_string "gui_text_string"
#define EHS_FB_ID_gui_text_string 0x4B01

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

#define EHS_FB_NAME_gui_text_real "gui_text_real"
#define EHS_FB_ID_gui_text_real 0x09F1


/* declare functions provided by this function block */
/* identify function, create function and destroy functions are common
   to all textbox functions */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_float_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_float)
#endif

/*****************************************************************************/
/* Integer textbox */

#define EHS_FB_NAME_gui_text_int "gui_text_int"
#define EHS_FB_ID_gui_text_int 0xC9C7


/* declare functions provided by this function block */
/* identify function, create function and destroy functions are common
   to all textbox functions */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_int_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_int)

/*****************************************************************************/
/* Boolean textbox */

#define EHS_FB_NAME_gui_text_bool "gui_text_bool"
#define EHS_FB_ID_gui_text_bool 0xABD1


/* declare functions provided by this function block */
/* identify function, create function and destroy functions are common
   to all textbox functions */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_bool_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_bool)

#endif /* _EHS_GUITEXTBOXA_H */

