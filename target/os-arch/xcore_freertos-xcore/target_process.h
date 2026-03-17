/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_process.h
 * Thread and mutex interface for XMOS xcore.ai FreeRTOS targets.
 * fwk_rtos provides a POSIX pthread layer over FreeRTOS, so the same
 * pthread-based interface used on ESP32 applies here.
 * Should only be included by hal_process.h.
 *
 * @author: inx limited
 */

#ifndef EHS_TARGET_PROCESS_H
#define EHS_TARGET_PROCESS_H

#ifndef EHS_HAL_PROCESS_H
#error "This file should only be included by hal_process.h"
#endif

typedef void *pMutexRef;
typedef void *xTaskHandle;
typedef void (*lwip_thread_fn)(void *arg);

#define PTHREAD_COND_INITIALIZER  0
#define PTHREAD_MUTEX_INITIALIZER 0

#define pthread_mutex_lock(x)   do { } while (0)
#define pthread_mutex_unlock(x) do { } while (0)
#define pthread_cond_broadcast(x) do { } while (0)
#define pthread_mutex_destroy(x) do { } while (0)

#define EhsTPMutex_lock(pMutexRef)   pthread_mutex_lock((pthread_mutex_t *)pMutexRef)
#define EhsTPMutex_unlock(pMutexRef) pthread_mutex_unlock((pthread_mutex_t *)pMutexRef)
#define EhsTgtProcess_isOrphan(x)    (EHS_FALSE)

typedef pthread_t EhsTPThread;
typedef pthread_mutex_t *EhsTPMutexStruct;

void EhsTargetExit(ehs_uint16);
ehs_bool EhsTP_shellExecuteStdout(char *sZstdout, const char *szCmd, int max_buffer_len);
void EhsTargetReboot(void);

#endif /* EHS_TARGET_PROCESS_H */
