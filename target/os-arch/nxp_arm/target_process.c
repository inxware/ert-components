/** @file target_process.c
 * description
 *
 * @author: inx limited
 * @version: $Revision: 2731 $
 * @date: $Date$
 *
 * Copyright (c) inx limited, 2007. All rights reserved.
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

#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_PROCESS

#include "target.h"
#include "hal-api.h"
#include "target_process.h"
#include "heatrod_config.h"
#include <errno.h>
#include <stdio.h>
/*****************************************************************************/
/* Declare macros and local typedefs used by this file */


/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */

/**
 * Mutex resource used to control access to the function block inputs and outputs
 */
EHS_LOCAL pthread_mutex_t EhsL_fbIO;

/* socket client double close prevention */
EHS_LOCAL pthread_mutex_t EhsL_socketClient;

/**
 * Mutex devman requests
 */
EHS_LOCAL pthread_mutex_t EhsL_devman_request;

/**
 * Mutex resource used to control access to viewport
 */
EHS_LOCAL pthread_mutex_t EhsL_viewport;

/**
 * Mutex resource used to control access to memory allocation
 */
EHS_LOCAL pthread_mutex_t EhsL_mem;

/**
 * Mutex resource used to control access to the console queue
 */
EHS_LOCAL pthread_mutex_t EhsL_consoleQueue;

/**
 * Mutex resource used to control access to the console input queue
 */
EHS_LOCAL pthread_mutex_t EhsL_consoleInputQueue;

/**
 * Mutex resource used to control access to the event queue
 */
EHS_LOCAL pthread_mutex_t EhsL_eventQueue;
EHS_LOCAL pthread_mutex_t EhsLMutex_fb_thread_counter;
EHS_LOCAL pthread_mutex_t EhsL_widgetTable;

EHS_LOCAL pthread_mutex_t EhsL_globalTimer;


/**
 * Mutex resource used to control access to the devman player data
 */
EHS_LOCAL pthread_mutex_t EhsL_devmanPlayerData;

/**
 * Mutex resource used to control access to the UrlGet Function Block
 */
EHS_LOCAL pthread_mutex_t EhsL_UrlGet;


/**
 * Mutex resource used to control access to shared information in playManager
 */ 
EHS_LOCAL pthread_mutex_t EhsL_playManager;


/**
 * Mutex resource used to control access to shared information in playManager
 */
EHS_LOCAL pthread_mutex_t EhsL_devmanMiscBuffers;

/*****************************************************************************/
/* Variables defined with global-scope */

/** reference to PID of parent process */
EHS_GLOBAL pid_t* EhsT_pidParent;

/** reference to PID of TCP/IP process */
EHS_GLOBAL pid_t* EhsT_pidTcpIp;

/**
 * Mutex resource used to control access to the function block inputs and outputs
 */
EhsTPMutexClass EhsTPMutex_fbIO;


EhsTPMutexClass EhsTPMutex_socketClient;

/**
 * Mutex resource ufor serialising devman server requests and keeping libcurl off it's discomfort path
 */
EhsTPMutexClass EhsTPMutex_devman_request;

/**
 * Mutex resource used to control access to viewport
 */
EhsTPMutexClass EhsTPMutex_viewport;

/**
 * Mutex resource used to control access to memory allocation
 */
EhsTPMutexClass EhsTPMutex_mem;

/**
 * Mutex resource used to control access to the console queue
 */
EhsTPMutexClass EhsTPMutex_consoleQueue;

/**
 * Mutex resource used to control access to the console input queue
 */
EhsTPMutexClass EhsTPMutex_consoleInputQueue;

/**
 * Mutex resource used to control access to the event queue
 */
EhsTPMutexClass EhsTPMutex_eventQueue;
EhsTPMutexClass EhsTPMutex_fb_thread_counter;
EhsTPMutexClass EhsTPMutex_widgetTable;

/*
 * Mutexes for timers
 * Timers might be added in threads
 */

EhsTPMutexClass EhsTPMutex_globalTimer;

/**
 * Mutex resource used to control access to the devman player data
 */
 EhsTPMutexClass EhsTPMutex_devmanPlayerData;

 /**
  * Used for the devman core App interface block
  */
 EhsTPMutexClass  EhsTPMutex_devmanInterface;

 /**
   * Used for the mutexing the DL and UP biffers for devman comms
   */
  EhsTPMutexClass  EhsTPMutex_devmanMiscBuffers;

 /**
  * Mutex resource used to control access to the devman player data
  */
 EhsTPMutexClass EhsTPMutex_UrlGet;


/**
 * Mutex resource used to control access to the playManager shared resources
 */
EhsTPMutexClass EhsTPMutex_playManager;

/**
 * Mutex system execution
 */
 EhsTPMutexClass EhsTPMutex_devmanPlayerData;
   
/*****************************************************************************/
/* Function definitions */

/**
 * Exit from EHS. Need to release shared memory
 * @todo Tell TCP/IP thread it's time to finish
 * @todo this should not exist - we only need a direct call to the HAL one.
 */
void EhsTargetExit(ehs_uint16 exitCode)
{
	printf("PBB EhsTargetExit %d\n",exitCode);
}

/**
 * Initialise the mutexes
 * @todo move all of the function block threads to their init functions and tear down function when implemented
 */
EHS_GLOBAL void EhsTPMutex_init(void) {
	memset(&EhsL_fbIO,0,sizeof(pthread_mutex_t));
	sys_mutex_new(&EhsL_fbIO);
	
    memset(&EhsL_devman_request,0,sizeof(pthread_mutex_t));
    sys_mutex_new(&EhsL_devman_request);
	
    memset(&EhsL_viewport,0,sizeof(pthread_mutex_t));
	sys_mutex_new(&EhsL_viewport);
	memset(&EhsL_mem,0,sizeof(pthread_mutex_t));
	sys_mutex_new(&EhsL_mem);
	memset(&EhsL_consoleQueue,0,sizeof(pthread_mutex_t));
	sys_mutex_new(&EhsL_consoleQueue);
	memset(&EhsL_consoleInputQueue,0,sizeof(pthread_mutex_t));
	sys_mutex_new(&EhsL_consoleInputQueue);
	memset(&EhsL_eventQueue,0,sizeof(pthread_mutex_t));
	sys_mutex_new(&EhsL_eventQueue);
	memset(&EhsLMutex_fb_thread_counter,0,sizeof(pthread_mutex_t));
	sys_mutex_new(&EhsLMutex_fb_thread_counter);
	memset(&EhsL_widgetTable,0,sizeof(pthread_mutex_t));
	sys_mutex_new(&EhsL_widgetTable);
	memset(&EhsL_devmanMiscBuffers,0,sizeof(pthread_mutex_t));
	sys_mutex_new(&EhsL_devmanMiscBuffers);
	memset(&EhsL_devmanPlayerData,0,sizeof(pthread_mutex_t));
	sys_mutex_new(&EhsL_devmanPlayerData);

	memset(&EhsL_playManager,0,sizeof(pthread_mutex_t));
	sys_mutex_new(&EhsL_playManager);

	memset(&EhsL_globalTimer,0,sizeof(pthread_mutex_t));
	sys_mutex_new(&EhsL_globalTimer);

	EhsTPMutex_fbIO = (EhsTPMutexClass)&EhsL_fbIO; // @todo all these need to be moved into the function block code
	EhsTPMutex_devman_request = (EhsTPMutexClass)&EhsL_devman_request;
    EhsTPMutex_socketClient= (EhsTPMutexClass)&EhsL_socketClient;
	EhsTPMutex_viewport = (EhsTPMutexClass)&EhsL_viewport;
	EhsTPMutex_mem = (EhsTPMutexClass)&EhsL_mem;
	EhsTPMutex_consoleQueue = (EhsTPMutexClass)&EhsL_consoleQueue;
	EhsTPMutex_consoleInputQueue = (EhsTPMutexClass)&EhsL_consoleInputQueue;
	EhsTPMutex_eventQueue = (EhsTPMutexClass)&EhsL_eventQueue;
	EhsTPMutex_fb_thread_counter = (EhsTPMutexClass)&EhsLMutex_fb_thread_counter;
	EhsTPMutex_widgetTable = (EhsTPMutexClass)&EhsL_widgetTable;
	EhsTPMutex_devmanPlayerData = (EhsTPMutexClass)&EhsL_devmanPlayerData;
	EhsTPMutex_devmanMiscBuffers = (EhsTPMutexClass)&EhsL_devmanMiscBuffers;
	EhsTPMutex_UrlGet = (EhsTPMutexClass)&EhsL_UrlGet;
	EhsTPMutex_playManager = (EhsTPMutexClass)&EhsL_playManager;
	EhsTPMutex_globalTimer =(EhsTPMutexClass)&EhsL_globalTimer;
}

/**
 * Shutdown the mutexes/.
 *
 * Trust: Called after mutexes are not being polled
 * @todo most of these should not be here - should move to the component handlers
 */
void EhsTPMutex_term(void) {//@todo and these need to gp too when we have the teardown compliment to instorage class specified for parameter ‘bcmp’it is implemented.
	if (EhsTPMutex_fbIO) pthread_mutex_destroy((pthread_mutex_t *) EhsTPMutex_fbIO);
    if (EhsTPMutex_socketClient) pthread_mutex_destroy((pthread_mutex_t *) EhsTPMutex_socketClient);
	if (EhsTPMutex_viewport) pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_viewport);
	if (EhsTPMutex_mem) pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_mem);
	if (EhsTPMutex_consoleQueue) pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_consoleQueue);
	if (EhsTPMutex_consoleInputQueue) pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_consoleInputQueue);
	if (EhsTPMutex_eventQueue) pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_eventQueue);
	if (EhsTPMutex_fb_thread_counter) pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_fb_thread_counter);
	if (EhsTPMutex_widgetTable) pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_widgetTable);
	if (EhsTPMutex_devmanPlayerData) pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_devmanPlayerData);
	if (EhsTPMutex_UrlGet) pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_UrlGet);
	if (EhsTPMutex_playManager) pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_playManager);
	EhsTPMutex_fbIO = NULL;
    EhsTPMutex_socketClient = NULL;
	EhsTPMutex_viewport = NULL;
	EhsTPMutex_mem = NULL;
	EhsTPMutex_consoleQueue = NULL;
	EhsTPMutex_consoleInputQueue = NULL;
	EhsTPMutex_eventQueue = NULL;
	EhsTPMutex_fb_thread_counter = NULL;
	EhsTPMutex_widgetTable = NULL;
	EhsTPMutex_devmanPlayerData = NULL;
	EhsTPMutex_UrlGet = NULL;
	EhsTPMutex_playManager = NULL;
}


/**
 * Execute a function from a function block in a separate thread
 * priority should be 0 - 100 and is capped at these extremes typically for Posix
 */

//@todo this function should allow values below -100 to revert sched other scheduling - and adopt the processe's default native values
EHS_GLOBAL EhsTPThread EhsHThread_execute(EhsGeneralThreadFuncType pfRun, void* context,ehs_sint16 priority) {
	EhsTPThread thread;
	thread = sys_thread_new("ehs", (lwip_thread_fn)pfRun, context, 1200, CONFIG_MAIN_THREAD_PRIORITY);
	return thread;
}

EHS_GLOBAL void EhsTPThread_exit() {
	
}


/**
 * Execute a function from a function block in a separate thread
 * returns
 */

ehs_bool EhsTPThread_ChangeThisPriority(ehs_sint16 priority) {
    return EHS_FALSE;
}


/* Kill a thread !!! don't use if you can avoid !!! */
int EhsTPThread_terminate(EhsTPThread  thread) {
	return 0;
}

/* Does a shell execute, returning stdout ( only up to available size ).
 * @todo - make a version that pumps out more data
 */
ehs_bool EhsTP_shellExecuteStdout(char* sZstdout,const char * szCmd, int max_buffer_len)  {
    return EHS_FALSE;
}

