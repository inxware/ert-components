/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/
 
/** @file target_main-deleteme.c
 * This file represents the main entry point for the target. Typically
 * it also incldes an event handler, should the target require one.
 *
 * @author: inx limited
 *
 */

/**
 * @page Verification Verification report
 * @section target_main
 * @anchor target_main
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on 2007-10-12
 * Last modified on $Date$
 *
 * This file contained no derogations to the MISRA standard.
 *
 * Note it is necessary to replace <sys/types.h> with <types.h> to lint this file successfully.
 */

/**
 * Provides access to the target-specific declarations of header files
 */
#define EHS_TARGET_CODE

/*****************************************************************************/
/* Included files */
#include <sys/types.h>
#include <signal.h>
#include "target.h"
#include "console_server.h"
#include "globals.h"
#include "ehs_main.h"
#include "hal_process.h"


/*****************************************************************************/
/* Declare macros and local typedefs used by this file */
/*****************************************************************************/
/* Declare prototypes of local functions */
/*****************************************************************************/
/* Variables defined with file-scope */

/**
 * Handle the SIGTERM signal
 */
EHS_LOCAL void EhsTargetHandleTerm(int);

/*****************************************************************************/
/* Variables defined with global-scope */
/*****************************************************************************/
/* Function definitions */
/* Delete this - no longer used?
void* EhsL_server(void* pDummy) {

	EhsSvcTcp_server(NULL);
}
*/

/**
 * Main entry point to the application.
 * @return Integer representing exit code of application.
 *
 */
EhsTargetIntType main(void)
{
    pid_t pID;
    //SetDllDirectory("../cslib");
    signal(SIGTERM,EhsTargetHandleTerm);
    signal(SIGINT, EhsTargetHandleTerm);
    EhsMain(); /* doesn't return in this version */
    EhsShutdown();

    return 0;
}

/**
 * Handle the SIGTERM signal
 */
void EhsTargetHandleTerm(int sig)
{
    EhsShutdown();
}
