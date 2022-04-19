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

#define EHS_FB_ARRAYSTRING_NAME "ArrayString1"

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ArrayString1);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ArrayString1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArrayStringRead1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArrayStringWrite1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArrayStringClear1);

EHS_FB_FUNCTIONS(ArrayString1)

#define EHS_FB_ARRAYINT_NAME "ArrayInt1"

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ArrayInt1);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ArrayInt1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArrayIntRead1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArrayIntWrite1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArrayIntClear1);

EHS_FB_FUNCTIONS(ArrayInt1)

#define EHS_FB_ARRAYBOOL_NAME "ArrayBool1"

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ArrayBool1);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ArrayBool1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArrayBoolRead1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArrayBoolWrite1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArrayBoolClear1);

EHS_FB_FUNCTIONS(ArrayBool1)


#ifdef EHS_TARGET_FP_SUPPORT

#define EHS_FB_ARRAYREAL_NAME "ArrayReal1"

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ArrayReal1);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ArrayReal1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArrayRealRead1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArrayRealWrite1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArrayRealClear1);

EHS_FB_FUNCTIONS(ArrayReal1)

#endif
/* declare the table for use by the blockref table */
#endif

