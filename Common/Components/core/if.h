/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
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

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(If);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(If);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(If);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(If)

#endif /* EHS_IF_H */

