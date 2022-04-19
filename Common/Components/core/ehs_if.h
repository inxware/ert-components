/**
 * if.h
 *
 * function prototypes for if.c 
 *
 * @author: MDD
 * @version: $Revision: 278 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef EHS_IF_H
#define EHS_IF_H
#include "ehs_fb_types.h"

/******************************************************************************/
/* Declare if */
#define EHS_FB_IF_NAME "if"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(If);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(If);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(If);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(If)

#endif /* EHS_IF_H */

