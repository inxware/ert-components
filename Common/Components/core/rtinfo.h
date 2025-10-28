/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * fileFunctions.h
 *
 * function prototypes for rtinfo.c - deals with all types
 *
 * @author: inx limited
 *
 */

#ifndef EHS_RTINFO_FUNCTIONS_H
#define EHS_RTINFO_FUNCTIONS_H
#include "ehs_fb_types.h"

// @TODO - this function is now deprecated, move it to deprecated. Use API2 RuntimeInfo2 instead!

/******************************************************************************/
/* Declare the block entries */
#define EHS_FB_NAME_RuntimeInfo "RuntimeInfo"
#define EHS_FB_ID_RuntimeInfo 0x895B

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(runtimeinfo);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(runtimeinfo);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(runtimeinfo_updatestatic);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(runtimeinfo_updatedynamic);
//EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(runtimeinfo_request_pairing_id);	//@todo why is this only in .c ?




/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(runtimeinfo)


#endif /* EHS_RTINFO_FUNCTIONS_H */
