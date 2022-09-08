/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/
 
/** @file target_process-deleteme.c
 * description
 *
 * @author: inx limited
 *
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
 
 

//Try reverting this to the previous windows version

/*****************************************************************************/
/* Included files */
#define EHS_TARGET_CODE /* Ensure header files include target-internal values */

#include "target.h"
#include "hal_process.h"
#include "target_process.h"
#include <errno.h>
#include <sys/types.h>
#include <pthread.h>
//#include <bits/pthreadtypes.h>

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
 * Mutex resource used to control access to the event queue
 */
EHS_LOCAL pthread_mutex_t EhsL_eventQueue;
EHS_LOCAL pthread_mutex_t EhsLMutex_fb_thread_counter;
EHS_LOCAL pthread_mutex_t EhsL_widgetTable;
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
 * Mutex resource used to control access to the event queue
 */
EhsTPMutexClass EhsTPMutex_eventQueue;
EhsTPMutexClass EhsTPMutex_fb_thread_counter;
EhsTPMutexClass EhsTPMutex_widgetTable;
/*****************************************************************************/
/* Function definitions */

/**
 * Exit from EHS. Need to release shared memory
 * @todo Tell TCP/IP thread it's time to finish
 */
void EhsExit_WithClearedemory(ehs_uint16 exitCode)  // @todo what the hell is this for??
{
    /*
     *
     static ehs_bool bExited = EHS_FALSE;

    if (!bExited)
    {
    	bExited = EHS_TRUE;
    	EhsHSys_term();
    }
    exit((EhsTargetIntType)exitCode); / *lint !e586 Target specific derogation - exit can be permitted; * /
    */
}

/**
 * Initialise the mutexes
 *
 */
EHS_GLOBAL void EhsTPMutex_init(void)
{
    pthread_mutexattr_t attr;
    //pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&EhsL_fbIO,NULL);
    pthread_mutex_init(&EhsL_viewport,&attr);
    pthread_mutex_init(&EhsL_mem,NULL);
    pthread_mutex_init(&EhsL_consoleQueue,NULL);
    pthread_mutex_init(&EhsL_eventQueue,NULL);
    pthread_mutex_init(&EhsLMutex_fb_thread_counter,NULL);
    pthread_mutex_init(&EhsL_widgetTable,NULL);
    EhsTPMutex_fbIO = (EhsTPMutexStruct)&EhsL_fbIO;
    EhsTPMutex_viewport = (EhsTPMutexStruct)&EhsL_viewport;
    EhsTPMutex_mem = (EhsTPMutexStruct)&EhsL_mem;
    EhsTPMutex_consoleQueue = (EhsTPMutexStruct)&EhsL_consoleQueue;
    EhsTPMutex_eventQueue = (EhsTPMutexStruct)&EhsL_eventQueue;
    EhsTPMutex_fb_thread_counter = (EhsTPMutexStruct)&EhsLMutex_fb_thread_counter;
    EhsTPMutex_widgetTable = (EhsTPMutexStruct)&EhsL_widgetTable;
}

/**
 * Shutdown the mutexes
 */
void EhsTPMutex_term(void)
{
    pthread_mutex_destroy(&EhsL_fbIO);
    pthread_mutex_destroy(&EhsL_viewport);
    pthread_mutex_destroy(&EhsL_mem);
    pthread_mutex_destroy(&EhsL_consoleQueue);
    pthread_mutex_destroy(&EhsL_eventQueue);
    pthread_mutex_destroy(&EhsL_widgetTable);
    EhsTPMutex_fbIO = NULL;
    EhsTPMutex_viewport = NULL;
    EhsTPMutex_mem = NULL;
    EhsTPMutex_consoleQueue = NULL;
    EhsTPMutex_eventQueue = NULL;
    EhsTPMutex_fb_thread_counter = NULL;
    EhsTPMutex_widgetTable = NULL;
}

/**
 * Execute a function from a function block in a separate thread
 */

EHS_GLOBAL EhsTPThread EhsTPThread_execute(EhsThreadFuncType* pfRun, struct EhsFunctionInstanceDataStruct* context,ehs_sint16 priority)
{
    EhsTPThread thread;
    /* cast pfRun to return void* with one arg of void* */
    pthread_create(&thread,NULL,(void*(*)(void*))pfRun,context);
    return thread;
}

/* Kill a thread !!! don't use if you can avoid !!! */
int EhsTPThread_terminate(EhsTPThread  thread)
{
    return pthread_cancel(thread);
}

