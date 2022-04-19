//Try reverting this to the previous windows version
/** @file target_process.h
 * The target-specific declarations required to support the HAL for process services
 * and related OS interations are defined here.
 * This file should only be included by hal_process.h
 *
 * @author: inx limited
 * @version: $Revision: 1692 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 *
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

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
