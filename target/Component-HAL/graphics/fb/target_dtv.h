/** @file target_dtv.h
 * The target-specific declarations required to support the HAL for DTV services
 * are defined here. This file should only be included by hal_dtv.h
 * 
 * @author: inx limited
 * @version: $Revision: 4459 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
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



/*****************************************************************************/
/* Define types */


/*****************************************************************************/
/* Declare global variables */


/*****************************************************************************/
/* Declare function prototypes  */

#endif /* EHS_TARGET_DTV_H */
