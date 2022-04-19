/** @file 
 * target_main.c
 * This file represents the main entry point for the target. Typically
 * it also incldes an event handler, should the target require one.
 * 
 * @author: inx limited
 * @version: $Revision: 1272 $
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
#include <unistd.h>

#include "target.h"
#ifdef  EHS_COMMS_API_SUPPORT
#include "console_server.h"
#endif
#include "globals.h"
#include "ehs_main.h"
#include "hal-api.h" // required for the meta data storage


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
 *

void* EhsL_server(void* pDummy) {
	printf("server started up\n");
	EhsSvcTcp_server(NULL);
}
*/

/* linux (and gnu) is always ready as soon as main is run */
ehs_bool EhsTPlatformReady(void (*target_loop_iteration)(void*),void * target_env_blob) {
	return EHS_TRUE;
}

/** 
 * Main entry point to the application.
 * @return Integer representing exit code of application.
 *
 */
EhsTargetIntType main(int argc, ehs_char ** argv )
{
	pid_t pID;
//#define EHS_DONT_BUF_STDOUT
#ifdef EHS_DONT_BUF_STDOUT
	setbuf(stdout, NULL);
#endif
	ehs_char buf[EHS_MAXPATHLENGTH];
	getcwd(buf,EHS_MAXPATHLENGTH); // Note this must be implemented for mingw
	EhsHStoreArgInfo(argc,argv,buf);
	EHSH_LOG_INFO("EHS starting up\n");
	signal(SIGTERM,EhsTargetHandleTerm);
	signal(SIGINT, EhsTargetHandleTerm);
#ifdef EHS_CATCH_SIG_IGN_TO_NULL
	/* BAcking out - this is handled in libcurl (possibly) -
	 * It doesn't seem to help when the devman server is busy as things go off line. Perhaps they would be killed otherwise?
	 *  -- a kill would probably be better than the current behaviour of going off line permenantly */
	sigaction(SIGPIPE, &(struct sigaction){SIG_IGN}, NULL);
#endif
	EhsMain(NULL,NULL); /* doesn't return in this version */
	EhsExit(0);
	return 0;
}

/**
 * Handle the SIGTERM signal
 */
void EhsTargetHandleTerm(int sig)
{
	/* Tod we should call some HAL tear down functions here libxml , libcurl etc.*/
	EhsExit(0);
}
