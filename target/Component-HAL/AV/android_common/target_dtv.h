/** @file target_dtv.h
 * The target-specific declarations required to support the HAL for DTV services
 * are defined here. This file should only be included by hal_dtv.h
 *
 * @author: inx limited
 * @version: $Revision: 1242 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 *
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

#ifndef EHS_TARGET_DTV_H
#define EHS_TARGET_DTV_H

//#ifndef EHS_HAL_DTV_H
//#error "This file should only be included by hal_dtv.h"
//#endif

/*****************************************************************************/
/* Included files */

/*****************************************************************************/
/* Define macros  */



/*****************************************************************************/
/* Define types */


/*****************************************************************************/
/* Declare global variables */


/*****************************************************************************/
/* Declare function prototypes  */

void EhsTDPlayback_android_register (const ehs_char* id, void* pPvrPlay);

#endif /* EHS_TARGET_DTV_H */
