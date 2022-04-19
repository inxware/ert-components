/** @file target_process.h
 * The target-specific declarations required to support the HAL for process services
 * and related OS interations are defined here.
 * This file should only be included by hal_process.h
 *
 * @author: inx limited
 * @version: $Revision: 1650 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 *
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

#ifndef EHS_TARGET_PROCESS_H
#define EHS_TARGET_PROCESS_H

#ifndef EHS_HAL_PROCESS_H
#error "This file should only be included by hal_process.h"
#endif

/*****************************************************************************/
/* Included files */
#include <windows.h>
/*****************************************************************************/
/* Define macros  */

/**
 * Returns true if the TCP/IP process is an orphan. For the PC, the TCP/IP work
 * is done in a thread, which cannot be orphaned by the process stopping
 */
#define EhsTgtProcess_isOrphan() (EHS_FALSE)

/*****************************************************************************/
/* Define types */
typedef HANDLE  EhsTPThread; //@todo only needed for lcd is this safe?
/**
 * type declaration for the mutex - these aren't yet implemented
 */

struct EhsTPMutexStruct;

/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */

#endif /* EHS_TARGET_STRING_H */
