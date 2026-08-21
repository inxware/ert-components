/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_process.h
 * The target-specific declarations required to support the HAL for process
 * services and related OS interations are defined here. This file should only
 * be included by hal_process.h
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_PROCESS_H
#define EHS_TARGET_PROCESS_H

#ifndef EHS_HAL_PROCESS_H
#error "This file should only be included by hal_process.h"
#endif


typedef void *pMutexRef;
typedef void *xTaskHandle;
/** Function prototype for thread functions */
typedef void (*lwip_thread_fn)(void *arg);

/*****************************************************************************/
/* Included target API files */

#include <pthread.h>
/**
 * Lock a mutex to indicate the start of a region where we perform exclusive
 * handling by a thread/process
 *
 * @param[in] pMutexRef Indicates the identity of the mutex we are locking
 */
//#define EhsTPMutex_lock(pMutexRef) {fprintf(stderr,"lock
//%x\n",pMutexRef);pthread_mutex_lock((pthread_mutex_t *)pMutexRef);}
#define EhsTPMutex_lock(pMutexRef)                                             \
  pthread_mutex_lock((pthread_mutex_t *)pMutexRef)
/**
 * Release a mutex indicating the end of a unique access region
 *
 * @param[in] pMutexRef Indicates the identity of the mutex we are releasing
 */
//#define EhsTPMutex_unlock(pMutexRef)
//{fprintf(stderr,"unlock%x\n",pMutexRef);pthread_mutex_unlock((pthread_mutex_t
//*)pMutexRef);}
#define EhsTPMutex_unlock(pMutexRef)                                           \
  pthread_mutex_unlock((pthread_mutex_t *)pMutexRef)
/**
 * Called by the TCP/IP thread to see if it is an orphan process
 *
 * @return true if the current parent process Id is different to the original
 * parent process id and this process is the TCP/IP process
 */
#define EhsTgtProcess_isOrphan(x) (EHS_FALSE)

// typedef xTaskHandle EhsTPThread;
// typedef sys_mutex_t pthread_cond_t;
// typedef sys_mutex_t pthread_mutex_t;
/*****************************************************************************/
/* Define types */

typedef pthread_t EhsTPThread;

/**
 * Definition of mutex type
 */
typedef pthread_mutex_t* EhsTPMutexStruct;

/**
 * Target specific exit function
 * completed.
 * @param[in] szCmd Command to execute
 */
void EhsTargetExit(ehs_uint16);

ehs_bool EhsTP_shellExecuteStdout(char *sZstdout, const char *szCmd,
                                  int max_buffer_len);

/**
 * @brief Reboot the device
 */
void EhsTargetReboot( void );
/* EHS_TARGET_PROCESS_H */
#endif
