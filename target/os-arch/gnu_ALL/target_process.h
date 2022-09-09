/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file target_process.h
 * The target-specific declarations required to support the HAL for process services
 * and related OS interations are defined here.
 * This file should only be included by hal_process.h
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_PROCESS_H
#define EHS_TARGET_PROCESS_H

/* Check we are being included in a way we like! */
#ifndef EHS_HAL_PROCESS_H
#error "This file should only be included by hal_process.h"
#endif

/* OK we do ned to include this here anyway (because we use macros...) into it */

#define _GNU_SOURCE /* Needed for pthread_mutexattr_settype */
/* check out the GPL situtation of _GNU_SOURCE */
#ifndef __USE_UNIX98
    #define  __USE_UNIX98
#endif
#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif

#include <pthread.h>


/******************************** storage class specified for parameter     *********************************************/
/* Define macros  */

#ifndef EHS_MINGW
    #define EhsTPThread_yield() pthread_yield();	/* Yield thread */
#else
//#define EhsTPThread_yield() Sleep(0);	/* Yield thread? */
    #define EhsTPThread_yield() //don't need this for windows?
#endif

/**
 * @brief Lock a mutex to indicate the start of a region where we perform exclusive handling by a thread/process
 *
 * @param[in] pMutexRef Indicates the identity of the mutex we are locking
 */
//#define EhsTPMutex_lock(pMutexRef) {fprintf(stderr,"lock %x\n",pMutexRef);pthread_mutex_lock((pthread_mutex_t *)pMutexRef);}
#define EhsTPMutex_lock(pMutexRef) pthread_mutex_lock((pthread_mutex_t *)pMutexRef)

/**
 * @brief Conditional semaphore broadcase
 * 
 */
#define EhsTPCondition_broadcast(ConditionRef) pthread_cond_broadcast((pthread_cond_t*)ConditionRef)

/**
 * Try Lock a mutex to indicate the start of a region where we perform exclusive handling by a thread/process
 *
 * @param[in] pMutexRef Indicates the identity of the mutex we are locking
 */
//#define EhsTPMutex_trylock(pMutexRef) {fprintf(stderr,"lock %x\n",pMutexRef);pthread_mutex_trylock((pthread_mutex_t *)pMutexRef);}
#define EhsTPMutex_trylock(pMutexRef) pthread_mutex_trylock((pthread_mutex_t *)pMutexRef)
/**
 * Release a mutex indicating the end of a unique access region
 *
 * @param[in] pMutexRef Indicates the identity of the mutex we are releasing
 */
//#define EhsTPMutex_unlock(pMutexRef) {fprintf(stderr,"unlock%x\n",pMutexRef);pthread_mutex_unlock((pthread_mutex_t *)pMutexRef);}
#define EhsTPMutex_unlock(pMutexRef) pthread_mutex_unlock((pthread_mutex_t *)pMutexRef)

/**
 * Target specific exit function
 * completed.
 * @param[in] szCmd Command to execute
 */
void EhsTargetExit(ehs_uint16);


/**
 * Execute a command using the system's shell. This function will block until the command has
 * completed.
 * @param[in] szCmd Command to execute
 */
#define EhsTP_shellExecute(szCmd) system(szCmd)
ehs_bool EhsTP_shellExecuteStdout(char* sZstdout,const char * szCmd, int max_buffer_len) ;

/**
 * Called by the TCP/IP thread to see if it is an orphan process
 *
 * @return true if the current parent process Id is different to the original parent process id
 * and this process is the TCP/IP process
 */
#define EhsTgtProcess_isOrphan(x) (EHS_FALSE)


/*****************************************************************************/
/* Define types */

// no we don't need to do this here  typedef pthread_t EhsTPThread;

/**
 * Definition of mutex type
 */
//pthread_mutex_t* EhsTPMutexStruct;
/*
  Defintion of the condition semaphore
*/
//pthread_cond_t* EhsTPConditionStruct;

/*****************************************************************************/
/* Declare global variables */


/*****************************************************************************/
/* Declare function prototypes  */


#endif /* EHS_TARGET_STRING_H */
