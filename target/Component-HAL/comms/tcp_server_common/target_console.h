/** @file target_console.h
 * The target-specific declarations required to support the HAL for viewport services
 * are defined here. This file should only be included by hal_viewport.h
 * 
 * @author: inx limited
 * @version: $Revision: 1238 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

#ifndef EHS_TARGET_CONSOLE_H
#define EHS_TARGET_CONSOLE_H

#ifndef EHS_HAL_CONSOLE_H
#error "This file should only be included by hal_console.h"
#endif

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


/******************************************************************************/
/* Console interface functions */

/* Target-only declarations. These declarations are intended for the target only
 * they should not be visible to the main application */

#ifdef EHS_TARGET_CODE
/**
 * Contains input from the console.
 * Initialised by EhsHSys_init
 */
EHS_GLOBAL EhsConsoleQueueType* EhsTgtConsoleInputQueueRef;

/**
 * Contains output to the console
 * Initialised by EhsHSys_init
 */
EHS_GLOBAL EhsConsoleQueueType* EhsTgtConsoleOutputQueueRef;

#endif /* EHS_TARGET_CODE */
#endif /* EHS_TARGET_CONSOLE_H */
