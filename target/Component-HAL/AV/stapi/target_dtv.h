/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_dtv.h
 * The target-specific declarations required to support the HAL for DTV services
 * are defined here. This file should only be included by hal_dtv.h
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_DTV_H
#define EHS_TARGET_DTV_H

#ifndef EHS_HAL_DTV_H
#error "This file should only be included by hal_dtv.h"
#endif

/*****************************************************************************/
/* Included files */

/*****************************************************************************/
/* Define macros  */

/**
 * Longest possible path name (excluding filename, but including separators and
 * terminating zero).
 */
#define EHS_TD_FILES_MAX_PATH 1024

/**
 * Path separator for this target
 */
#define EHS_TD_FILES_SEPARATOR '/'

/**
 * @TODO provide a proper reset function
 */
#define EhsTDPlayback_reset(x) return EHS_TRUE
/*****************************************************************************/
/* Define types */


/*****************************************************************************/
/* Declare global variables */


/*****************************************************************************/
/* Declare function prototypes  */

#endif /* EHS_TARGET_DTV_H */
