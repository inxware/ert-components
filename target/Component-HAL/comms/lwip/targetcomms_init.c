/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file targetcomms_init.c
 * Initialization/reset/termination code for comms
 *
 * @author: inx limited
 *
 */

/**
 * @page Verification Verification report
 * @section targetcomms_init
 * @anchor targetcomms_init
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on
 * Last modified on $Date:$
 *
 * This file contained no derogations to the MISRA standard.
 */

/*****************************************************************************/
/* Included files */
#define EHS_TARGET_CODE /* Ensure header files include target-internal values */
#include "targetcomms_init.h"
#include "hal_console.h"
#include "console_queue.h"

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */

/*****************************************************************************/
/* Variables defined with global-scope */
EHS_GLOBAL EhsConsoleQueueType EhsTgtConsoleInputQueue;
EHS_GLOBAL EhsConsoleQueueType EhsTgtConsoleOutputQueue;
/*****************************************************************************/
/* Function definitions */

/**
 * Perform necessary Operating system setup upon system initialisation
 */
void EhsTCommsSys_init(void)
{
#ifdef EHS_DEBUG_TCPIP_CONSOLE
    EhsTgtConsoleInputQueueRef = &EhsTgtConsoleInputQueue;
    EhsTgtConsoleOutputQueueRef = &EhsTgtConsoleOutputQueue;
    EhsConsoleQueue_reset(EhsTgtConsoleInputQueueRef);
    EhsConsoleQueue_reset(EhsTgtConsoleOutputQueueRef);
#endif
}

/**
 * Called to shutdown the target prior to shutting down EHS
 */
void EhsTCommsSys_term(void)
{
}

/**
 * Perform setup before loading in a new application
 */
void EhsTCommsApp_init(void)
{
}

/**
 * Called to clean up after the execution of an application in order to make
 * sure everything has been stopped.
 */
void EhsTCommsApp_term(void)
{
}


/**
 * Perform necessary target setup per application execution
 */
void EhsTCommsApp_reset(void)
{
}
