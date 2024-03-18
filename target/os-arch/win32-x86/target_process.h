/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file target_process.h
 * The target-specific declarations required to support the HAL for process services
 * and related OS interations are defined here.
 * This file should only be included by hal_process.h
 *
 * @author: inx limited
 *
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
/**
 * @brief Reboot the device
 */
void EhsTargetReboot( void );

#endif /* EHS_TARGET_STRING_H */
