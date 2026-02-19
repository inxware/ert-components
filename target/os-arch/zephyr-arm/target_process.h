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
 * services and related OS interactions are defined here. This file should only
 * be included by hal_process.h
 *
 * Zephyr RTOS implementation using k_mutex, k_thread APIs.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_PROCESS_H
#define EHS_TARGET_PROCESS_H

#ifndef EHS_HAL_PROCESS_H
#error "This file should only be included by hal_process.h"
#endif

#include <zephyr/kernel.h>

typedef void *pMutexRef;
typedef void *xTaskHandle;
/** Function prototype for thread functions */
typedef void (*lwip_thread_fn)(void *arg);

/*****************************************************************************/
/* Included target API files */

/* Define macros  */

/* Zephyr uses k_mutex for mutual exclusion.
 * The EhsTPMutexClass is a pointer to a k_mutex struct, matching the
 * pattern used in ESP32 (pointer to pthread_mutex_t).
 *
 * Note: Zephyr's k_mutex supports priority inheritance and is re-entrant
 * by default (via k_mutex_lock with K_FOREVER timeout).
 */

/**
 * Lock a mutex
 */
#define EhsTPMutex_lock(pMutexRef)                                             \
  k_mutex_lock((struct k_mutex *)pMutexRef, K_FOREVER)
/**
 * Release a mutex
 */
#define EhsTPMutex_unlock(pMutexRef)                                           \
  k_mutex_unlock((struct k_mutex *)pMutexRef)

/**
 * Called by the TCP/IP thread to see if it is an orphan process
 * Always false on Zephyr MCU targets (no parent process concept).
 */
#define EhsTgtProcess_isOrphan(x) (EHS_FALSE)

/*****************************************************************************/
/* Define types */

/* Zephyr thread handle - pointer to k_thread struct */
typedef struct k_thread* EhsTPThread;

/**
 * Definition of mutex type - pointer to k_mutex struct
 */
typedef struct k_mutex* EhsTPMutexStruct;

/**
 * Target specific exit function
 * @param[in] exitCode Exit code
 */
void EhsTargetExit(ehs_uint16);

ehs_bool EhsTP_shellExecuteStdout(char *sZstdout, const char *szCmd,
                                  int max_buffer_len);

/**
 * @brief Reboot the device
 */
void EhsTargetReboot(void);

#endif /* EHS_TARGET_PROCESS_H */
