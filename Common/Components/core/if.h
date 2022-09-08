/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/**
 * if.h
 *
 * function prototypes for if.c
 *
 * @author: inx limited
 *
 */

#ifndef EHS_IF_H
#define EHS_IF_H
#include "ehs_fb_types.h"

/******************************************************************************/
/* Declare if */
#define EHS_FB_NAME_if "if"
#define EHS_FB_ID_if 0x7AAE

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(If);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(If);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(If);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(If)

#endif /* EHS_IF_H */

