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
 *
 * xcore has no POSIX pthreads.  Mutexes are backed by FreeRTOS recursive
 * semaphores; threads are FreeRTOS tasks.
 *
 * WORKAROUND: This header contains no FreeRTOS #includes even though the
 * types below (pMutexRef, EhsTPThread) are really FreeRTOS SemaphoreHandle_t
 * and TaskHandle_t.  The reason is that hal_process.h pulls this file into
 * every .c translation unit, but FreeRTOS headers are only on the compiler
 * include path when building with the xcore SDK (i.e. in target_process.c).
 * The workaround is safe today only because FreeRTOS defines both handle
 * types as void* internally, so substituting void* here is ABI-compatible.
 *
 * The proper fix would be to add the FreeRTOS include paths to the global
 * compiler flags (or isolate the handle types behind a dedicated opaque
 * header that is part of the SDK-aware build), removing the need for these
 * void* stand-ins entirely.
 *
 * EhsTPMutex_lock / EhsTPMutex_unlock are implemented as real functions in
 * target_process.c (prototypes are declared in hal_process.h).  Other
 * target ports shadow those prototypes with macros; this port does not.
 *
 * Should only be included by hal_process.h.
 *
 * @author: inx limited
 */

#ifndef EHS_TARGET_PROCESS_H
#define EHS_TARGET_PROCESS_H

#ifndef EHS_HAL_PROCESS_H
#error "This file should only be included by hal_process.h"
#endif

/* void* stand-in for FreeRTOS SemaphoreHandle_t — see WORKAROUND note above. */
typedef void *pMutexRef;
typedef void (*lwip_thread_fn)(void *arg);

/* Stubs retained for any shared code that conditionally references these */
#define PTHREAD_COND_INITIALIZER  0
#define PTHREAD_MUTEX_INITIALIZER 0
#define pthread_mutex_destroy(x)  do { } while (0)
#define pthread_cond_broadcast(x) do { } while (0)

#define EhsTgtProcess_isOrphan(x) (EHS_FALSE)

/* void* stand-in for FreeRTOS TaskHandle_t — see WORKAROUND note above. */
typedef void *EhsTPThread;

void EhsTargetExit(ehs_uint16);
ehs_bool EhsTP_shellExecuteStdout(char *sZstdout, const char *szCmd, int max_buffer_len);
void EhsTargetReboot(void);

#endif /* EHS_TARGET_PROCESS_H */
