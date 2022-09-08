/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file target_process-deleteme.h
 * The target-specific declarations required to support the HAL for process services
 * and related OS interations are defined here.
 * This file should only be included by hal_process.h
 *
 * @author: inx limited
 *
 */

//Try reverting this to the previous windows version
#ifndef EHS_TARGET_PROCESS_H
#define EHS_TARGET_PROCESS_H

#ifndef EHS_HAL_PROCESS_H
#error "This file should only be included by hal_process.h"
#endif

/*****************************************************************************/
/* Included files */
#include <pthread.h>
/*****************************************************************************/
/* Define macros  */

/* define types */
typedef unsigned long int EhsTPThread;

/**
 * Lock a mutex to indicate the start of a region where we perform exclusive handling by a thread/process
 * @todo Provide an implementation for EhsOsMutex_lock
 *
 * @param[in] pMutexRef Indicates the identity of the mutex we are locking
 */
//#define EhsTPMutex_lock(pMutexRef) {fprintf(stderr,"lock %x\n",pMutexRef);pthread_mutex_lock((pthread_mutex_t *)pMutexRef);}
#define EhsTPMutex_lock(pMutexRef) {pthread_mutex_lock((pthread_mutex_t *)pMutexRef);}

/**
 * Release a mutex indicating the end of a unique access region
 * @todo Provide an implmentation for EhsOsMutex_unlock.
 *
 * @param[in] pMutexRef Indicates the identity of the mutex we are releasing
 */
//#define EhsTPMutex_unlock(pMutexRef) {fprintf(stderr,"unlock%x\n",pMutexRef);pthread_mutex_unlock((pthread_mutex_t *)pMutexRef);}
#define EhsTPMutex_unlock(pMutexRef) {pthread_mutex_unlock((pthread_mutex_t *)pMutexRef);}

/**
 * Execute a command using the system's shell. This function will block until the command has
 * completed.
 * @param[in] szCmd Command to execute
 */
#define EhsTP_shellExecute(szCmd) system(szCmd)

/**
 * Called by the TCP/IP thread to see if it is an orphan process
 *
 * @return true if the current parent process Id is different to the original parent process id
 * and this process is the TCP/IP process
 */
#define EhsTgtProcess_isOrphan(x) (EHS_FALSE)


/*****************************************************************************/
/* Define types */

/**
 * Definition of mutex type
 */
typedef pthread_mutex_t* EhsTPMutexStruct;

/*****************************************************************************/
/* Declare global variables */


/*****************************************************************************/
/* Declare function prototypes  */

#endif /* EHS_TARGET_STRING_H */
