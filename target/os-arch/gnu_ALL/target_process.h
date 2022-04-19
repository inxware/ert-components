/** @file target_process.h
 * The target-specific declarations required to support the HAL for process services
 * and related OS interations are defined here.
 * This file should only be included by hal_process.h
 *
 * @author: inx limited
 * @version: $Revision: 1650 $
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
//#ifndef EHS_MINGW
#define _GNU_SOURCE /* Needed for pthread_mutexattr_settype */
/* check out the GPL situtation of _GNU_SOURCE */
#ifndef __USE_UNIX98
#define  __USE_UNIX98
#endif
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

//@todo pthread_yield seems t be lost onpthread.h  with the above macros. UNIX98 problem?
#include <pthread.h>


/* define types */
//#endif


/********************************storage class specified for parameter ‘bcmp’*********************************************/
/* Define macros  */

#ifndef EHS_MINGW
#define EhsTPThread_yield() pthread_yield();	/* Yield thread */
#else
//#define EhsTPThread_yield() Sleep(0);	/* Yield thread */
#define EhsTPThread_yield() //don't need this for windows?
#endif

/**
 * Lock a mutex to indicate the start of a region where we perform exclusive handling by a thread/process
 *
 * @param[in] pMutexRef Indicates the identity of the mutex we are locking
 */
//#define EhsTPMutex_lock(pMutexRef) {fprintf(stderr,"lock %x\n",pMutexRef);pthread_mutex_lock((pthread_mutex_t *)pMutexRef);}
#define EhsTPMutex_lock(pMutexRef) pthread_mutex_lock((pthread_mutex_t *)pMutexRef)

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

typedef pthread_t EhsTPThread;

/**
 * Definition of mutex type
 */
typedef pthread_mutex_t* EhsTPMutexStruct;

/*****************************************************************************/
/* Declare global variables */


/*****************************************************************************/
/* Declare function prototypes  */


#endif /* EHS_TARGET_STRING_H */
