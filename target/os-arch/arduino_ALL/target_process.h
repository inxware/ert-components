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


/******************************** storage class specified for parameter     *********************************************/
/* Define macros  */


//#define EhsTPThread_yield() Sleep(0);	/* Yield thread? */
  #define EhsTPThread_yield() //don't need this for windows?




/**
 * @brief Conditional semaphore broadcase
 * 
 */
#define EhsTPCondition_broadcast(ConditionRef) //pthread_cond_broadcast((pthread_cond_t*)ConditionRef)

/**
 * Try Lock a mutex to indicate the start of a region where we perform exclusive handling by a thread/process
 *
 * @param[in] pMutexRef Indicates the identity of the mutex we are locking
 */
//#define EhsTPMutex_trylock(pMutexRef) {fprintf(stderr,"lock %x\n",pMutexRef);pthread_mutex_trylock((pthread_mutex_t *)pMutexRef);}
#define EhsTPMutex_trylock(pMutexRef) //pthread_mutex_trylock((pthread_mutex_t *)pMutexRef)

/**
 * @brief Lock a mutex to indicate the start of a region where we perform exclusive handling by a thread/process
 *
 * @param[in] pMutexRef Indicates the identity of the mutex we are locking
 */
void EhsTgtMutex_lock(void* pMutexRef);
#define EhsTPMutex_lock(pMutexRef) EhsTgtMutex_lock(pMutexRef)

/**
 * Release a mutex indicating the end of a unique access region
 *
 * @param[in] pMutexRef Indicates the identity of the mutex we are releasing
 */
void EhsTgtMutex_unlock(void* pMutexRef);
#define EhsTPMutex_unlock(pMutexRef) EhsTgtMutex_unlock(pMutexRef)

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

/**
 * @brief Reboot the device
 */
void EhsTargetReboot( void );

/**
 * @brief Returns current thread ID
 */
unsigned int EhsTargetThreadID();

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
