/** @file target_process.h
 * The target-specific declarations required to support the HAL for process services
 * and related OS interations are defined here.
 * This file should only be included by hal_process.h
 *
 * @author: inx limited
 * @version: $Revision: 1242 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 *
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

#ifndef EHS_HAL_XML_MINIMAL_H
#define EHS_HAL_XML_MINIMAL_H

/*****************************************************************************/
/* Included files */
#include "ehs_types.h"
#include "hal_string.h"
#include "hal_mem.h"


/*****************************************************************************/
/* Define macros  */

/*****************************************************************************/
/* Define types */

/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */

/* Some cheap XML tag readers */
ehs_char * Ehs_ReadXMLTag(ehs_char* XML, ehs_char * tag);
ehs_char * Ehs_ReadXMLTagAttrib(ehs_char** pAttrib, ehs_char* XML, ehs_char * tag);
ehs_bool Ehs_ReadXMLAttribValue(ehs_char* result, ehs_char* XML, ehs_char * name);
ehs_char * Ehs_CopyXMLTagElement(ehs_char * element, ehs_char* XML, ehs_uint32 max,
		ehs_bool trim_end_too);



#endif /* EHS_HAL_XML_MINIMAL_H */
