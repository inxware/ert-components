/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
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

#define EHS_FB_NAME_ArrayString "ArrayString"
#define EHS_FB_ID_ArrayString 0xDBBB

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ArrayString);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ArrayString);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArrayStringRead);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArrayStringWrite);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ArrayString)
#endif

