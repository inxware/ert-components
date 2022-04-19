/** @file arraystring.h
 * This declares the arraystring function block
 *
 * @author: inx limited
 * @version: $Revision: 278 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef _ARRAY_H
#define _ARRAY_H

#include "ehs_fb_types.h"

#define EHS_FB_ARRAYSTRING_NAME "ArrayString"

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ArrayString);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ArrayString);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArrayStringRead);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArrayStringWrite);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ArrayString)
#endif

