/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file arraystring.h
 * This declares the arraystring function block
 *
 * @author: inx limited
 *
 */

#ifndef _ARRAY_H
#define _ARRAY_H

#include "ehs_fb_types.h"

#define EHS_FB_NAME_ArrayString1 "ArrayString1"
#define EHS_FB_ID_ArrayString1 0xA75A


/* declare functions provided by this function block */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ArrayString1);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ArrayString1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ArrayStringRead1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ArrayStringWrite1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ArrayStringClear1);

EHS_FB_FUNCTIONS(ArrayString1)

#define EHS_FB_NAME_ArrayInt1 "ArrayInt1"
#define EHS_FB_ID_ArrayInt1 0x47EC


EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ArrayInt1);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ArrayInt1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ArrayIntRead1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ArrayIntWrite1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ArrayIntClear1);

EHS_FB_FUNCTIONS(ArrayInt1)


#define EHS_FB_NAME_ArrayBool1 "ArrayBool1"
#define EHS_FB_ID_ArrayBool1 0xF36B


EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ArrayBool1);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ArrayBool1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ArrayBoolRead1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ArrayBoolWrite1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ArrayBoolClear1);

EHS_FB_FUNCTIONS(ArrayBool1)


#ifdef EHS_TARGET_FP_SUPPORT
#define EHS_FB_NAME_ArrayReal1 "ArrayReal1"
#define EHS_FB_ID_ArrayReal1 0x2BC8

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ArrayReal1);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ArrayReal1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ArrayRealRead1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ArrayRealWrite1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ArrayRealClear1);

EHS_FB_FUNCTIONS(ArrayReal1)

#endif
/* declare the table for use by the blockref table */
#endif

