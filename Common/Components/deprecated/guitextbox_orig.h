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

#ifndef _EHS_GUITEXTBOX_H
#define _EHS_GUITEXTBOX_H

#include "ehs_fb_types.h"

/*****************************************************************************/
/* String textbox */

#define EHS_FB_NAME_GUI_OutputTextBoxString "GUI_OutputTextBoxString"
#define EHS_FB_ID_GUI_OutputTextBoxString 0xF01B


/* declare functions provided by this function block */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(GUI_Textbox);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(GUI_Textbox);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(GUI_Textbox_create);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(GUI_Textbox_destroy);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(GUI_TextboxString_write);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(GUI_TextboxString)

/*****************************************************************************/
/* "Real" textbox */

#ifdef EHS_TARGET_FP_SUPPORT
#define EHS_FB_NAME_GUI_OutputTextBoxReal "GUI_OutputTextBoxReal"
#define EHS_FB_ID_GUI_OutputTextBoxReal 0xADD5

/* declare functions provided by this function block */
/* identify function, create function and destroy functions are common
   to all textbox functions */
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(GUI_TextboxFloat_write);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(GUI_TextboxFloat)
#endif

/*****************************************************************************/
/* Integer textbox */

#define EHS_FB_NAME_GUI_OutputTextBoxInt "GUI_OutputTextBoxInt"
#define EHS_FB_ID_GUI_OutputTextBoxInt 0xAC48


/* declare functions provided by this function block */
/* identify function, create function and destroy functions are common
   to all textbox functions */
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(GUI_TextboxInt_write);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(GUI_TextboxInt)

/*****************************************************************************/
/* Boolean textbox */

#define EHS_FB_NAME_GUI_OutputTextBoxBool "GUI_OutputTextBoxBool"
#define EHS_FB_ID_GUI_OutputTextBoxBool 0x0FF5

/* declare functions provided by this function block */
/* identify function, create function and destroy functions are common
   to all textbox functions */
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(GUI_TextboxBool_write);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(GUI_TextboxBool)

#endif /* _EHS_GUITEXTBOX_H */

