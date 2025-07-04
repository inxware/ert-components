/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_process.h
 * The target-specific declarations required to support the HAL for process services
 * and related OS interations are defined here.
 * This file should only be included by hal_process.h
 *
 * @author: inx limited
 *
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
