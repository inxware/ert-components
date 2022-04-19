/** @file targetos_init.c
 * Initialization/reset/termination code for the OS
 *
 * @author: inx limited
 * @version: $Revision: 3935 $
 * @date: $Date$
 * 
 * Copyright (c) inx limited, 2008. All rights reserved.
 */

/**
 * @page Verification Verification report
 * @section hal_graphics_font
 * @anchor hal_graphics_font
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on 
 * Last modified on $Date:$
 *
 * This file contained no derogations to the MISRA standard.
 */

/*****************************************************************************/
/* Included files */
#define EHS_TARGET_CODE /* Ensure header files include target-internal values */

#include <stdlib.h>

#include "targetos_init.h"
#include "callback_queue.h"
#include "blockref_table.h"
#include "hal_process.h"
#include "console_server.h"

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */

/*****************************************************************************/
/* Variables defined with global-scope */

extern EhsBlockRefType EhsBlockRefTable_windows[];
extern EhsCallbackQueueType EhsWindowsKeypressCallback;


/*****************************************************************************/
/* Function definitions */

/** 
 * Perform necessary Operating system setup upon system initialisation
 */
void EhsTOsSys_init(void)
{
	EhsTPMutex_init();
	EhsTgtTimer_init();
	
	EhsToolkitTable_addTable(EhsBlockRefTable_windows);
	
	/* start TCP/IP server thread */
	// moved to common code : EhsHThread_execute(EhsSvcTcp_server,NULL,-90);
}

/**
 * Called to shutdown the target prior to shutting down EHS
 */
void EhsTOsSys_term(void)
{
	// Leave the mutexes to the OS EhsTPMutex_term();
}

/**
 * Perform setup before loading in a new application
 */
void EhsTOsApp_init(void)
{
	EhsWindowsKeypressCallback = NULL;
}

/**
 * Called to clean up after the execution of an application in order to make
 * sure everything has been stopped.
 */
void EhsTOsApp_term(void)
{
}

/**
 * Perform necessary target setup per application execution
 */
void EhsTOsApp_reset(void)
{
}

/* 	This needs to be done for all the other targets too */
void EhsBinSearchPath(void ) {
 ehs_char path_var[4024] = {'\0'}; // @ todo make this a temporatry var.  USE: EhsHMem_tempAlloc EhsHMem_tempFree 
 ehs_char szCwd[1024] = {'\0'};

 strncpy (path_var,"PATH=",4024);
 getcwd(szCwd, 1024);
 strncat(path_var,szCwd,4024); // @todo this should be made more robust - absolute path?? !!
 strncat(path_var,";",4024);
 strncat(path_var,getenv("PATH"),4024);
 putenv (path_var); //path_var
 printf(getenv("PATH"));
}
