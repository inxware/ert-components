/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file hal_process.h
 * In this file, all of the hardware abstraction layer declarations relating to process control
 * and related OS interactions are given.
 *
 * @author: inx limited
 *
 */

/**
 * @page Verification Verification report
 * @section hal_process_h
 * @anchor hal_process_h
 * @subsection misra MISRA compliance:
 * Demonstrated MISRA compliant on
 * Last modified on $Date$
 *
 * This file contained the following derogations to the MISRA standard.
 */

#ifndef EHS_HAL_PROCESS_H
#define EHS_HAL_PROCESS_H

/*****************************************************************************/
/* Included files */

#include "ehs_types.h"
#include "ehs_fb_types.h" //needed for the pfRun type
#include "target_process.h"

/*****************************************************************************/
/* Define macros  */

/*****************************************************************************/
/* Define types */

/** Define a target independent mutexclass prototype: 
 * EhsTPMutexClass is an opaque pointer to pthread_mutex_t* EhsTPMutexStruct
 */

typedef struct EhsTPMutexStruct *EhsTPMutexClass;

/* This is a conditional sempahpore - usally posix thread type. also defined as an opaque pointer */

typedef struct EhsTPConditionStruct *EhsTPConditionClass;

/*****************************************************************************/
/* Declare global variables */
/* Publish the mutex pointers anaonymously as class variables.
   only code in the target_process.c file needs to know what is in them.
*/

/**
 * Flag to indicate that EhsTgtInit has completed. This is required so that other threads
 * can see that EHS is now ready to work with them. This must be set to false by the main function.
 * This is declared as a pointer to allow shared memory systems to point to some kind of shared
 * memory.
 */
EHS_GLOBAL ehs_bool* EhsHSys_initCompleteRef;

/**
 * Mutex resource used to control access to the function block inputs and outputs
 * Initialise this mutex
 */
EHS_GLOBAL EhsTPMutexClass EhsTPMutex_fbIO;

/* socket  client function block */
EHS_GLOBAL EhsTPMutexClass EhsTPMutex_socketClient;

/**
 * Mutex resource used to serialise devman url requests for curl and devman server happiness
 */
EHS_GLOBAL EhsTPMutexClass EhsTPMutex_devman_request;

/**
 * Mutex resource used to control access to viewport
 */
EHS_GLOBAL EhsTPMutexClass EhsTPMutex_viewport;

/**
 * Mutex resource used to control access to memory allocation
 */
EHS_GLOBAL EhsTPMutexClass EhsTPMutex_mem;

/**
 * Mutex resource used to control access to the console queue
 */
EHS_GLOBAL EhsTPMutexClass EhsTPMutex_consoleQueue;

/**
 * Mutex resource used to control access to the console input queue
 */
EHS_GLOBAL EhsTPMutexClass EhsTPMutex_consoleInputQueue;

/**
 * Mutex resource used to control access to the event queue
 */
EHS_GLOBAL EhsTPMutexClass EhsTPMutex_eventQueue;

/*The table of widgets*/
EHS_GLOBAL EhsTPMutexClass EhsTPMutex_widgetTable;
#ifdef EHS_DEVMAN_SUPPORT
EHS_GLOBAL    EhsTPMutexClass EhsTPMutex_devmanPlayerData;
EHS_GLOBAL    EhsTPMutexClass EhsTPMutex_devmanInterface;
EHS_GLOBAL    EhsTPMutexClass EhsTPMutex_devmanMiscBuffers;
#endif //EHS_DEVMAN_SUPPORT
#ifdef EHS_COMPONENT_NETWORKING_SUPPORT
EHS_GLOBAL    EhsTPMutexClass EhsTPMutex_UrlGet;
#endif //EHS_COMPONENT_NETWORKING_SUPPORT

/**
 * Mutex resource used to control access to the playManager data
 */
EHS_GLOBAL EhsTPMutexClass EhsTPMutex_playManager;

/*****************************************************************************/
/* Declare function prototypes
 *
 * @todo these need to be moved
 *
 * */

void EhsSleepUs(ehs_uint32 tSleepTime);

/**
 * Perform necessary target set-up
 */
EHS_GLOBAL void EhsTargetInit(void);

/**
 * Perform setup before loading in a new application
 */
EHS_GLOBAL void EhsTargetApplicationInit(void);

/**
 * Reset application-specific parts of target
 */
EHS_GLOBAL void EhsTargetApplicationReset(void);

/* Thread functions */


typedef EhsThreadFuncReturnType (*EhsGeneralThreadFuncType)(void* context);

/*
 * Function instance specific version of the thread creator
 * This is use for Function block facing HAL API.
 * It simply calls the target specific function EhsHThread_execute()
 * */

#ifndef EHS_SKIP_COMPONENT_ONLY_HAL

EHS_GLOBAL ehs_bool EhsTPThread_execute(EhsThreadFuncType pfRun, struct EhsFunctionInstanceDataStruct* context,ehs_sint16 priority) ;

/**
 * Execute a function from a function block in a separate thread - generic
 */
EHS_GLOBAL ehs_bool EhsHThread_execute(EhsGeneralThreadFuncType pfRun, void * context,ehs_sint16 priority) ;
EHS_GLOBAL void EhsTPThread_exit();
#define EhsHThread_yield() EhsTPThread_yield();	// Yield thread
#define EhsHThread_exit() EhsTPThread_exit();return 0l	//< Value that can be safely used for returning from thread functions

#ifdef EHS_RE_INTRODUCE_THREAD_HANDLES
/*
 *
 *Shutdown a thread  with what ever terms the thread is configured for - use at your peril!
 */
int EhsTPThread_terminate(EhsTPThread thread);
#endif
/*
 * Calling this function chnges the priority of the calling thread (0-100).
 */

ehs_bool EhsTPThread_ChangeThisPriority(ehs_sint16 priority);
//#endif

/* within target_process.h, process functions are normally #def'd to their stddef equivalents.
 * If the target cannot use the standard version, the #define is removed from target_process,
 * and the below function is used. The declarations below also provide a secondary
 * purpose - they show the prototype of the function in question.
 */

#ifndef EhsTPMutex_lock
/**
 * Lock a mutex to indicate the start of a region where we perform exclusive handling by a thread/process
 *
 * @param[in] pMutexRef Indicates the identity of the mutex we are locking
 */
EHS_GLOBAL void EhsTPMutex_lock(EhsTPMutexClass pMutexRef);
#endif

#ifndef EhsTPMutex_unlock
/**
 * Release a mutex indicating the end of a unique access region
 *
 * @param[in] pMutexRef Indicates the identity of the mutex we are releasing
 */
EHS_GLOBAL void EhsTPMutex_unlock(EhsTPMutexClass pMutexRef);
#endif

#ifndef EhsTPMutex_init
/**
 * Initialise the mutexes
 *
 */
EHS_GLOBAL void EhsTPMutex_init(void);
#endif

#ifndef EhsTPMutex_term
/**
 * Shutdown the mutexes
 */
void EhsTPMutex_term(void);
#endif

 ehs_bool EhsProcessInitMutex(EhsTPMutexClass *reftoMutex); // note this will only work once!
 ehs_bool EhsProcessInitCond(EhsTPConditionClass * refToCond); // note this will only work once!


#ifndef EhsTgtProcess_isOrphan
/**
 * Called by the TCP/IP thread to see if it is an orphan process
 *
 * @return true if the current parent process Id is different to the original parent process id
 * and this process is the TCP/IP process
 */
EHS_GLOBAL ehs_bool EhsTgtProcess_isOrphan(void);
#endif

#endif //

#ifndef EhsTP_shellExecute
/**
 * Execute a command using the system's shell. This function will block until the command has
 * completed.
 * @param[in] szCmd Command to execute
 */
EHS_GLOBAL void EhsTP_shellExecute(const ehs_char* szCmd);
#endif

#ifndef EhsExit
/**
 * Exit from EHS. Exact behaviour of this function is target defined.
 */
EHS_GLOBAL void EhsExit(ehs_uint16 exitCode);
#endif //ifndef EHS_SKIP_COMPONENT_ONLY_HAL


#endif /* EHS_HAL_PROCESS_H */


