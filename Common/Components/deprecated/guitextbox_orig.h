/** 
 * @file guitextbox.h
 * This declares the guitextbox function blocks
 *
 * @author: inx limited
 * @version: $Revision: 959 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef _EHS_GUITEXTBOX_H
#define _EHS_GUITEXTBOX_H

#include "ehs_fb_types.h"

/*****************************************************************************/
/* String textbox */

#define EHS_FB_GUITEXTBOX_STRING_NAME "GUI_OutputTextBoxString"

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(GUI_Textbox);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(GUI_Textbox);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_Textbox_create);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_Textbox_destroy);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_TextboxString_write);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(GUI_TextboxString)

/*****************************************************************************/
/* "Real" textbox */

#ifdef EHS_TARGET_FP_SUPPORT
#define EHS_FB_GUITEXTBOX_FLOAT_NAME "GUI_OutputTextBoxReal"

/* declare functions provided by this function block */
/* identify function, create function and destroy functions are common
   to all textbox functions */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_TextboxFloat_write);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(GUI_TextboxFloat)
#endif

/*****************************************************************************/
/* Integer textbox */

#define EHS_FB_GUITEXTBOX_INT_NAME "GUI_OutputTextBoxInt"

/* declare functions provided by this function block */
/* identify function, create function and destroy functions are common
   to all textbox functions */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_TextboxInt_write);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(GUI_TextboxInt)

/*****************************************************************************/
/* Boolean textbox */

#define EHS_FB_GUITEXTBOX_BOOL_NAME "GUI_OutputTextBoxBool"

/* declare functions provided by this function block */
/* identify function, create function and destroy functions are common
   to all textbox functions */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_TextboxBool_write);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(GUI_TextboxBool)

#endif /* _EHS_GUITEXTBOX_H */

