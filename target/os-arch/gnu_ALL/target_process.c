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

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#ifndef EHS_MINGW

//#define _GNU_SOURCE
//#define  __USE_UNIX98
//#include <pthread.h> //done in this header
#include <sys/resource.h>
#ifndef EHS_ANDROID
#include <bits/pthreadtypes.h>
#endif
#endif
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
	exit((EhsTargetIntType)exitCode); /*lint !e586 Target specific derogation - exit can be permitted; */
}

/**
 * Initialise the mutexes
 * @todo move all of the function block threads to their init functions and tear down function when implemented
 */
EHS_GLOBAL void EhsTPMutex_init(void) {
	pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
	//pthread_mutexattr_t * attr_address=&attr;
#ifndef EHS_MINGW
/* Set some fairly safe (minimal deadlock) mutex parameters */
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
//#ifdef PTHREAD_PRIO_INHERIT
#ifndef EHS_ANDROID
	pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);
#else

#endif
//#endif

#else //MINGW
	/* Set some fairly safe (minimal deadlock) mutex parameters */
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		//pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);
	//attr_address=NULL;
#endif
	memset(&EhsL_fbIO,0,sizeof(pthread_mutex_t));
	pthread_mutex_init(&EhsL_fbIO,&attr);
	
    memset(&EhsL_devman_request,0,sizeof(pthread_mutex_t));
    pthread_mutex_init(&EhsL_devman_request,&attr);
	
    memset(&EhsL_viewport,0,sizeof(pthread_mutex_t));
	pthread_mutex_init(&EhsL_viewport,&attr);
	memset(&EhsL_mem,0,sizeof(pthread_mutex_t));
	pthread_mutex_init(&EhsL_mem,&attr);
	memset(&EhsL_consoleQueue,0,sizeof(pthread_mutex_t));
	pthread_mutex_init(&EhsL_consoleQueue,&attr);
	memset(&EhsL_consoleInputQueue,0,sizeof(pthread_mutex_t));
	pthread_mutex_init(&EhsL_consoleInputQueue,&attr);
	memset(&EhsL_eventQueue,0,sizeof(pthread_mutex_t));
	pthread_mutex_init(&EhsL_eventQueue,&attr);
	memset(&EhsLMutex_fb_thread_counter,0,sizeof(pthread_mutex_t));
	pthread_mutex_init(&EhsLMutex_fb_thread_counter,&attr);
	memset(&EhsL_widgetTable,0,sizeof(pthread_mutex_t));
	pthread_mutex_init(&EhsL_widgetTable,&attr);
	memset(&EhsL_devmanMiscBuffers,0,sizeof(pthread_mutex_t));
	pthread_mutex_init(&EhsL_devmanMiscBuffers,&attr);
	memset(&EhsL_devmanPlayerData,0,sizeof(pthread_mutex_t));
	pthread_mutex_init(&EhsL_devmanPlayerData,&attr);

	memset(&EhsL_playManager,0,sizeof(pthread_mutex_t));
	pthread_mutex_init(&EhsL_playManager,&attr);

	memset(&EhsL_globalTimer,0,sizeof(pthread_mutex_t));
	pthread_mutex_init(&EhsL_globalTimer,&attr);

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
    
    pthread_mutexattr_destroy(&attr);
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
	pthread_attr_t tattr_param;
	struct sched_param param;
	int ret;
#ifdef EHS_USE_AGGRESSIVE_REALTIME_FEATURES
	int maxpri=sched_get_priority_max(SCHED_RR);
	int minpri=sched_get_priority_min(SCHED_RR);
#else
	int maxpri=sched_get_priority_max(SCHED_OTHER);
	int minpri=sched_get_priority_min(SCHED_OTHER);
#endif

/*Todo thread priorities should be made relative to Current priority */
	/* We will assume althese are done under sched_other for linux */
	/* initialized with default attributes */
	ret = pthread_attr_init(&tattr_param);
	param.sched_priority =maxpri+priority;
	param.sched_priority=(param.sched_priority<minpri) ? minpri : param.sched_priority;
	param.sched_priority=(param.sched_priority>maxpri) ? maxpri : param.sched_priority;
#ifdef EHS_USE_AGGRESSIVE_REALTIME_FEATURES
	pthread_attr_setschedpolicy(&tattr_param, SCHED_RR);
#else
	pthread_attr_setschedpolicy(&tattr_param, SCHED_OTHER); // use linux scheeduling
#endif
	ret = pthread_attr_setschedparam(&tattr_param,&param);

	pthread_attr_setdetachstate(&tattr_param,PTHREAD_CREATE_DETACHED);
	//@todo if we want real time we need to make this only for very short functions - otherwise it will kill drivers and crash linux.
#ifdef EHS_USE_AGGRESSIVE_REALTIME_FEATURES
	pthread_attr_setscope(&tattr_param, PTHREAD_SCOPE_SYSTEM); /* system-wide contention for linux - CAREFUL - THIS WILL KILL divers eventually under heavy load*/
#else
	pthread_attr_setscope(&tattr_param, PTHREAD_SCOPE_PROCESS); /* system-wide contention for linux*/
#endif
    //   int getpriority(int which, int who);
	/* @todo we through the above away and use null for now. This needs sorting when EHS SCHED_RR is re-instated */
	/* cast pfRun to return void* with one arg of void* */

	/* @todo : We need to clone the instance data here, this required the size of the object to be known outside of the component, and housekeeping (garbage collection) is required for terminated threads that do not use a terminate or proper completion exit path..*/
	//printf("INFO:Creating Thread for func %x\n",pfRun);
	ret=pthread_create(&thread,&tattr_param,(void*(*)(void*))pfRun,context);
	pthread_attr_destroy(&tattr_param);
	switch ( ret ) {
	case	0 : break;//printf("INFO: Created Thread OK\n");break;
	case 	EAGAIN : EHSH_LOG_ERROR("ERROR Could not create thread: no resources\n");break;
	case	EINVAL :EHSH_LOG_ERROR("ERROR Could not create thread: attribute information invalid\n");break;
	case	EPERM  :EHSH_LOG_ERROR("ERROR Could not create thread: Client does not have necessary permissions");break;
	default: EHSH_LOG_ERROR("ERROR Could not create thread: Unknown Error");;
	}
	//ret = pthread_attr_destroy(&tattr);
	//printf("INFO:Created Thread\n");
	return thread;
}

int exit_blob; //nonsense pthread uses

EHS_GLOBAL void EhsTPThread_exit() {
	pthread_exit((void*)&exit_blob);
}


/**
 * Execute a function from a function block in a separate thread
 * returns
 */

ehs_bool EhsTPThread_ChangeThisPriority(ehs_sint16 priority) {
//#ifndef EHS_MINGW
	pthread_attr_t tattr_param;
	struct sched_param param;
	int ret;
	EhsTPThread thread=(pthread_t) pthread_self();
	int maxpri=sched_get_priority_max(SCHED_RR);
	int minpri=sched_get_priority_min(SCHED_RR);

	//ret = pthread_attr_init(&tattr_param);
        memset(&param, 0, sizeof(param));
	param.sched_priority =maxpri+priority;
	param.sched_priority=(param.sched_priority<minpri) ? minpri : param.sched_priority;
	param.sched_priority=(param.sched_priority>maxpri) ? maxpri : param.sched_priority;
	//pthread_setschedpolicy(thread, SCHED_RR);
        ret = pthread_setschedparam(thread,SCHED_RR,&param);
        if (ret !=0) {
            EHSH_LOG_WARNING("WARNING: Could not set ehs.exe to real-time. Running as SCHED_OTHER. Run ehs as root to use SCHED_RR\n");
        }
        return (ret == 0);
//#else
//        return EHS_TRUE;
//#endif
}


/* Kill a thread !!! don't use if you can avoid !!! */
int EhsTPThread_terminate(EhsTPThread  thread) {
#ifndef EHS_ANDROID
	return pthread_cancel(thread);
#else
	return 0;/* @todo this needs sorting out for Android - but is not generally used..*/
#endif
}

/* Does a shell execute, returning stdout ( only up to available size ).
 * @todo - make a version that pumps out more data
 */
ehs_bool EhsTP_shellExecuteStdout(char* sZstdout,const char * szCmd, int max_buffer_len)  {

    // Not mutexing as popen is expected to be thread safe as long as no parent thread has a stream open

	FILE *command_p = popen(szCmd, "r");

	  if (!command_p)
	  {
		sZstdout[0] = '\0';
	    return EHS_FALSE;
	  }
	  else {
		  ehs_char *line_p = fgets(sZstdout, max_buffer_len, command_p);
		  pclose(command_p);
		  return EHS_TRUE;
	  }

}

