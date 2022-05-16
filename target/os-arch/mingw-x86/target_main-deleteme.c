/** @file 
 * target_main.c
 * This file represents the main entry point for the target. Typically
 * it also incldes an event handler, should the target require one.
 * 
 * @author: inx limited
 * @version: $Revision: 2884 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
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
	printf("server started up\n");
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
	printf("EHS startup\n");
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
