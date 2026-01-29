/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_main.c
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
#include <unistd.h>

#include "globals.h"

#ifdef   EHS_DEBUG_TCPIP_CONSOLE
#include "console_server.h"
#endif
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


/* linux (and gnu) is always ready as soon as main is run */
ehs_bool EhsTPlatformReady(void (*target_loop_iteration)(void*),void * target_env_blob)
{
    return EHS_TRUE;
}

/**
 * Main entry point to the application.
 * @return Integer representing exit code of application.
 *
 */

EhsTargetIntType main(int argc, ehs_char ** argv )
{
#if defined(EHS_TEST_FUNC_OVERRIDE) && defined(EHS_TEST_FUNC_NO_ERT_INIT)
    // Bare metal mode: Run test immediately and exit
    extern void EHS_TEST_FUNC_NAME(void);
    //EHSH_LOG_INFO("EHS Bare Metal Test: Running %s\n", #EHS_TEST_FUNC_NAME);
    EHS_TEST_FUNC_NAME();
    EHSH_LOG_INFO("Test completed\n");
    return 0;
#endif

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
    sigaction(SIGPIPE, &(struct sigaction)
    {
        SIG_IGN
    }, NULL);
#endif

#ifdef EHS_TEST_FUNC_OVERRIDE
    // Test mode with full init: Run test instead of EhsMain
    extern void EHS_TEST_FUNC_NAME(void);
    //EHSH_LOG_INFO("EHS Test Mode: Running %s\n", #EHS_TEST_FUNC_NAME);
    EHS_TEST_FUNC_NAME();
    EHSH_LOG_INFO("Test completed\n");
#else
    // Normal production mode
    EhsMain(NULL,NULL); /* doesn't return in this version */
#endif

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
