/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
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

#include "globals.h"
#include "ehs_fb_types.h" //needed for the pfRun type
#include "target_process.h"

/*****************************************************************************/
/* Define macros  */

/*****************************************************************************/
/* Define types */

/** Define a target independent mutexclass prototype: 
 * EhsTPMutexClass is an opaque pointer to pthread_mutex_t* EhsTPMutexStruct
 */

typedef struct EhsTPMutexStruct * EhsTPMutexClass;

/* This is a conditional sempahpore - usally posix thread type. also defined as an opaque pointer */

typedef struct EhsTPConditionStruct * EhsTPConditionClass;

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
EHS_EXTERN ehs_bool* EhsHSys_initCompleteRef;

/**
 * Mutex resource used to control access to the function block inputs and outputs
 * Initialise this mutex
 */
EHS_EXTERN EhsTPMutexClass EhsTPMutex_fbIO;

/* socket  client function block */
EHS_EXTERN EhsTPMutexClass EhsTPMutex_socketClient;

/**
 * Mutex resource used to serialise devman url requests for curl and devman server happiness
 */
EHS_EXTERN EhsTPMutexClass EhsTPMutex_devman_request;

/**
 * Mutex resource used to control access to viewport
 */
EHS_EXTERN EhsTPMutexClass EhsTPMutex_viewport;

/**
 * Mutex resource used to control access to memory allocation
 */
EHS_EXTERN EhsTPMutexClass EhsTPMutex_mem;

/**
 * Mutex resource used to control access to the console queue
 */
EHS_EXTERN EhsTPMutexClass EhsTPMutex_consoleQueue;

/**
 * Mutex resource used to control access to the console input queue
 */
EHS_EXTERN EhsTPMutexClass EhsTPMutex_consoleInputQueue;

/**
 * Mutex resource used to control access to the event queue
 */
EHS_EXTERN EhsTPMutexClass EhsTPMutex_eventQueue;

/*The table of widgets*/
EHS_EXTERN EhsTPMutexClass EhsTPMutex_widgetTable;
#ifdef EHS_DEVMAN_SUPPORT
   EHS_EXTERN EhsTPMutexClass EhsTPMutex_devmanPlayerData;
   EHS_EXTERN EhsTPMutexClass EhsTPMutex_devmanInterface;
   EHS_EXTERN EhsTPMutexClass EhsTPMutex_devmanMiscBuffers;
#endif //EHS_DEVMAN_SUPPORT
#ifdef EHS_COMPONENT_NETWORKING_SUPPORT
   EHS_EXTERN EhsTPMutexClass EhsTPMutex_UrlGet;
#endif //EHS_COMPONENT_NETWORKING_SUPPORT

/**
 * Mutex resource used to control access to the playManager data
 */
// why here? 
EHS_EXTERN EhsTPMutexClass EhsTPMutex_playManager;

/**
 * Mutex resource used to control access to the Modbus Master shared resources
 */
EHS_EXTERN EhsTPMutexClass EhsTPMutex_MBMaster;

/**
 * Mutex resource used to control access to the MQTT shared resources
 */
EHS_EXTERN EhsTPMutexClass EhsTPMutex_subMQTT;
EHS_EXTERN EhsTPMutexClass EhsTPMutex_pubMQTT;

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
void EhsTargetInit(void);

/**
 * Perform setup before loading in a new application
 */
void EhsTargetApplicationInit(void);

/**
 * Reset application-specific parts of target
 */
void EhsTargetApplicationReset(void);

/* Thread functions */


typedef EhsThreadFuncReturnType (*EhsGeneralThreadFuncType)(void* context);

/*
 * Function instance specific version of the thread creator
 * This is use for Function block facing HAL API.
 * It simply calls the target specific function EhsHThread_execute()
 * */

#ifndef EHS_SKIP_COMPONENT_ONLY_HAL
// This means the target will choose a default value
#define EHS_THREAD_USE_DEFAULT_STACK_SIZE -1 

/* This is for non function block facing calls - it just calls EhsHThread_execute() below and should be removed unless we seperate the scope one day */
ehs_bool EhsTPThread_execute(EhsThreadFuncType pfRun, struct EhsFunctionInstanceDataStruct* context, ehs_sint16 priority, ehs_sint32 stackSize, ehs_char * _szThreadName);

/**
 * Execute a function from a function block in a separate thread - generic
 */
ehs_bool EhsHThread_execute(EhsGeneralThreadFuncType pfRun, void * context, ehs_sint16 priority, ehs_sint32 stackSize, ehs_char * _stackName);
void EhsTPThread_exit();
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
void EhsTPMutex_lock(EhsTPMutexClass pMutexRef);
#endif

#ifndef EhsTPMutex_unlock
/**
 * Release a mutex indicating the end of a unique access region
 *
 * @param[in] pMutexRef Indicates the identity of the mutex we are releasing
 */
void EhsTPMutex_unlock(EhsTPMutexClass pMutexRef);
#endif

#ifndef EhsTPMutex_init
/**
 * Initialise the mutexes
 *
 */
void EhsTPMutex_init(void);
#endif

#ifndef EhsTPMutex_term
/**
 * Shutdown the mutexes
 */
void EhsTPMutex_term(void);
#endif

 ehs_bool EhsProcessInitMutex(EhsTPMutexClass *reftoMutex); // note this will only work once!
 ehs_bool EhsProcessInitCond(EhsTPConditionClass * refToCond); // note this will only work once!

/**
 * Dynamically allocate and initialise a new mutex.
 * The mutex is heap-allocated; release with EhsHMutex_destroy().
 * Returns EHS_TRUE on success.
 */
ehs_bool EhsHMutex_create(EhsTPMutexClass* ref);

/**
 * Destroy and free a heap-allocated mutex created with EhsHMutex_create().
 * Sets *ref to NULL on return.
 */
void EhsHMutex_destroy(EhsTPMutexClass* ref);

/**
 * Dynamically allocate and initialise a new condition variable.
 * Must be released with EhsHCond_destroy().
 * Returns EHS_TRUE on success.
 */
ehs_bool EhsHCond_create(EhsTPConditionClass* ref);

/**
 * Destroy and free a heap-allocated condition variable created with EhsHCond_create().
 * Sets *ref to NULL on return.
 */
void EhsHCond_destroy(EhsTPConditionClass* ref);

#ifndef EhsTPCondition_signal
/**
 * Wake one thread waiting on a condition variable.
 * The associated mutex must be held by the caller.
 */
void EhsTPCondition_signal(EhsTPConditionClass ConditionRef);
#endif

#ifndef EhsTPCondition_wait
/**
 * Atomically release MutexRef and block until ConditionRef is signalled.
 * Re-acquires MutexRef before returning.
 */
void EhsTPCondition_wait(EhsTPConditionClass ConditionRef, EhsTPMutexClass MutexRef);
#endif


#ifndef EhsTgtProcess_isOrphan
/**
 * Called by the TCP/IP thread to see if it is an orphan process
 *
 * @return true if the current parent process Id is different to the original parent process id
 * and this process is the TCP/IP process
 */
ehs_bool EhsTgtProcess_isOrphan(void);
#endif

#endif //

#ifndef EhsTP_shellExecute
/**
 * Execute a command using the system's shell. This function will block until the command has
 * completed.
 * @param[in] szCmd Command to execute
 */
void EhsTP_shellExecute(const ehs_char* szCmd);
#endif

#ifndef EhsExit
/**
 * Exit from EHS. Exact behaviour of this function is target defined.
 */
void EhsExit(ehs_uint16 exitCode);
#endif //ifndef EHS_SKIP_COMPONENT_ONLY_HAL

/**
 * Best-effort remaining-stack query for the calling thread, in bytes. Returns -1 if this
 * target has no cheap way to determine it - callers must treat -1 as "unknown", not as a
 * huge/small number. Where available (e.g. FreeRTOS's uxTaskGetStackHighWaterMark()) this
 * is the historical *low-water mark* since the thread started, not a live reading of stack
 * used right now - a conservative but not perfect proxy.
 */
ehs_sint32 EhsHProcess_getStackRemaining(void);


#endif /* EHS_HAL_PROCESS_H */


