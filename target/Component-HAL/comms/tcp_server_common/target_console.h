/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_console.h
 * The target-specific declarations required to support the HAL for viewport services
 * are defined here. This file should only be included by hal_viewport.h
 *
 * @author: inx limited
 *
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

// declared in console_hal.h EHS_GLOBAL EhsConsoleQueueType* EhsTgtConsoleInputQueueRef;

/**
 * Contains output to the console
 * Initialised by EhsHSys_init
 */
// declared in console_hal.h EHS_GLOBAL EhsConsoleQueueType* EhsTgtConsoleOutputQueueRef = NULL;

#endif /* EHS_TARGET_CODE */
#endif /* EHS_TARGET_CONSOLE_H */
