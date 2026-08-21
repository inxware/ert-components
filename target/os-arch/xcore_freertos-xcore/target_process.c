/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_process.c
 * Thread and mutex implementation for XMOS xcore.ai FreeRTOS targets.
 *
 * xcore has no POSIX pthread layer.  This file uses native FreeRTOS APIs:
 *   - Mutexes: xSemaphoreCreateRecursiveMutexStatic / xSemaphoreTakeRecursive
 *   - Threads: xTaskCreate  (EhsGeneralThreadFuncType is cast to TaskFunction_t;
 *              both take a single void* arg — return value from EHS function is
 *              discarded by the FreeRTOS wrapper)
 *
 * Condition variables (used only by the DevMan misc-DL-data path) are stubbed
 * as zero-item queues; add full signalling semantics when that path is ported.
 *
 * @author: inx limited
 */

#define EHS_TARGET_CODE

#include <stdio.h>
#include <stdint.h>

/* FreeRTOS headers — only included here, not in target_process.h, so that
 * the SDK include paths do not need to be visible to every translation unit. */
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"

#include "globals.h"
#include "hal-api.h"
#include "target_process.h"

/* ---- Full struct definitions ------------------------------------------- */
/*
 * hal_process.h forward-declares these as opaque pointer types:
 *   typedef struct EhsTPMutexStruct    * EhsTPMutexClass;
 *   typedef struct EhsTPConditionStruct* EhsTPConditionClass;
 *
 * The full definitions live here (not in target_process.h) so that FreeRTOS
 * headers do not need to be on the include path for every .c file.
 */

struct EhsTPMutexStruct {
    SemaphoreHandle_t handle;
    StaticSemaphore_t storage;
};

struct EhsTPConditionStruct {
    QueueHandle_t queue;  /* zero-payload queue used as a signal */
};

/* ---- Private mutex storage -------------------------------------------- */

EHS_LOCAL struct EhsTPMutexStruct EhsL_fbIO;
EHS_LOCAL struct EhsTPMutexStruct EhsL_socketClient;
EHS_LOCAL struct EhsTPMutexStruct EhsL_devman_request;
EHS_LOCAL struct EhsTPMutexStruct EhsL_mem;
EHS_LOCAL struct EhsTPMutexStruct EhsL_consoleQueue;
EHS_LOCAL struct EhsTPMutexStruct EhsL_consoleInputQueue;
EHS_LOCAL struct EhsTPMutexStruct EhsL_eventQueue;
EHS_LOCAL struct EhsTPMutexStruct EhsLMutex_fb_thread_counter;

#ifdef EHS_NETWORKING_SUPPORT
EHS_LOCAL struct EhsTPMutexStruct EhsL_UrlGet;
#endif

#ifdef EHS_GUI_SUPPORT
EHS_LOCAL struct EhsTPMutexStruct EhsL_widgetTable;
EHS_LOCAL struct EhsTPMutexStruct EhsL_viewport;
#endif

#ifdef EHS_DEVMAN_SUPPORT
EHS_LOCAL struct EhsTPMutexStruct EhsL_devmanPlayerData;
EHS_LOCAL struct EhsTPMutexStruct EhsL_playManager;
EHS_LOCAL struct EhsTPMutexStruct EhsL_devmanMiscBuffers;
#endif

EHS_LOCAL struct EhsTPMutexStruct EhsL_MBMaster;
EHS_LOCAL struct EhsTPMutexStruct EhsL_subMQTT;
EHS_LOCAL struct EhsTPMutexStruct EhsL_pubMQTT;

/* ---- Public mutex handles --------------------------------------------- */

EhsTPMutexClass EhsTPMutex_fbIO;
EhsTPMutexClass EhsTPMutex_socketClient;
EhsTPMutexClass EhsTPMutex_devman_request;
EhsTPMutexClass EhsTPMutex_viewport;
EhsTPMutexClass EhsTPMutex_mem;
EhsTPMutexClass EhsTPMutex_consoleQueue;
EhsTPMutexClass EhsTPMutex_consoleInputQueue;
EhsTPMutexClass EhsTPMutex_eventQueue;
EhsTPMutexClass EhsTPMutex_fb_thread_counter;
EhsTPMutexClass EhsTPMutex_widgetTable;

#ifdef EHS_NETWORKING_SUPPORT
EhsTPMutexClass EhsTPMutex_UrlGet;
#endif

#ifdef EHS_DEVMAN_SUPPORT
EhsTPMutexClass EhsTPMutex_devmanPlayerData;
EhsTPMutexClass EhsTPMutex_devmanMiscBuffers;
#endif

#ifdef EHS_MEDIA_SUPPORT
EhsTPMutexClass EhsTPMutex_playManager;
#endif

EhsTPMutexClass EhsTPMutex_MBMaster;
EhsTPMutexClass EhsTPMutex_subMQTT;
EhsTPMutexClass EhsTPMutex_pubMQTT;

/* ----------------------------------------------------------------------- */

/* Helper macro: create a static recursive mutex and assign the public handle */
#define INIT_MUTEX(pub, priv) \
    do { \
        (priv).handle = xSemaphoreCreateRecursiveMutexStatic(&(priv).storage); \
        (pub) = (EhsTPMutexClass)&(priv); \
    } while (0)

/* Helper macro: delete a mutex and clear the public handle */
#define TERM_MUTEX(pub) \
    do { \
        if ((pub) != NULL) { \
            vSemaphoreDelete(((struct EhsTPMutexStruct *)(pub))->handle); \
            (pub) = NULL; \
        } \
    } while (0)

/* ----------------------------------------------------------------------- */

void EhsTargetExit(ehs_uint16 exitCode)
{
    printf("EhsTargetExit %d\n", exitCode);
}

/* ---- Mutex lock / unlock ---------------------------------------------- */
/*
 * Implemented as real functions rather than macros so that FreeRTOS headers
 * do not need to be included by every translation unit.
 * hal_process.h declares the prototypes; this port does not define macros
 * that would shadow them (unlike the ESP32 / Zephyr ports which do).
 */

void EhsTPMutex_lock(EhsTPMutexClass pMutexRef)
{
    xSemaphoreTakeRecursive(((struct EhsTPMutexStruct *)pMutexRef)->handle,
                            portMAX_DELAY);
}

void EhsTPMutex_unlock(EhsTPMutexClass pMutexRef)
{
    xSemaphoreGiveRecursive(((struct EhsTPMutexStruct *)pMutexRef)->handle);
}

/**
 * Initialise all EHS mutexes using static FreeRTOS recursive semaphores.
 * Must be called once before any thread uses an EhsTPMutex_* handle.
 */
void EhsTPMutex_init(void)
{
    INIT_MUTEX(EhsTPMutex_fbIO,                EhsL_fbIO);
    INIT_MUTEX(EhsTPMutex_devman_request,       EhsL_devman_request);
    INIT_MUTEX(EhsTPMutex_mem,                  EhsL_mem);
    INIT_MUTEX(EhsTPMutex_consoleQueue,         EhsL_consoleQueue);
    INIT_MUTEX(EhsTPMutex_consoleInputQueue,    EhsL_consoleInputQueue);
    INIT_MUTEX(EhsTPMutex_eventQueue,           EhsL_eventQueue);
    INIT_MUTEX(EhsTPMutex_fb_thread_counter,    EhsLMutex_fb_thread_counter);
    INIT_MUTEX(EhsTPMutex_socketClient,         EhsL_socketClient);
    INIT_MUTEX(EhsTPMutex_MBMaster,             EhsL_MBMaster);
    INIT_MUTEX(EhsTPMutex_subMQTT,              EhsL_subMQTT);
    INIT_MUTEX(EhsTPMutex_pubMQTT,              EhsL_pubMQTT);

#ifdef EHS_NETWORKING_SUPPORT
    INIT_MUTEX(EhsTPMutex_UrlGet,               EhsL_UrlGet);
#endif

#ifdef EHS_GUI_SUPPORT
    INIT_MUTEX(EhsTPMutex_widgetTable,          EhsL_widgetTable);
    INIT_MUTEX(EhsTPMutex_viewport,             EhsL_viewport);
#endif

#ifdef EHS_DEVMAN_SUPPORT
    INIT_MUTEX(EhsTPMutex_devmanPlayerData,     EhsL_devmanPlayerData);
    INIT_MUTEX(EhsTPMutex_playManager,          EhsL_playManager);
    INIT_MUTEX(EhsTPMutex_devmanMiscBuffers,    EhsL_devmanMiscBuffers);
#endif
}

/**
 * Release all EHS mutexes.
 */
void EhsTPMutex_term(void)
{
    TERM_MUTEX(EhsTPMutex_fbIO);
    TERM_MUTEX(EhsTPMutex_socketClient);
    TERM_MUTEX(EhsTPMutex_viewport);
    TERM_MUTEX(EhsTPMutex_mem);
    TERM_MUTEX(EhsTPMutex_consoleQueue);
    TERM_MUTEX(EhsTPMutex_consoleInputQueue);
    TERM_MUTEX(EhsTPMutex_eventQueue);
    TERM_MUTEX(EhsTPMutex_fb_thread_counter);
    TERM_MUTEX(EhsTPMutex_widgetTable);
    TERM_MUTEX(EhsTPMutex_MBMaster);
    TERM_MUTEX(EhsTPMutex_subMQTT);
    TERM_MUTEX(EhsTPMutex_pubMQTT);

#ifdef EHS_DEVMAN_SUPPORT
    TERM_MUTEX(EhsTPMutex_devmanPlayerData);
    TERM_MUTEX(EhsTPMutex_playManager);
#endif

#ifdef EHS_NETWORKING_SUPPORT
    TERM_MUTEX(EhsTPMutex_UrlGet);
#endif
}

/* ---- Thread creation --------------------------------------------------- */

#ifndef tskIDLE_PRIORITY
#define tskIDLE_PRIORITY 0
#endif

/* Default base priority for EHS tasks (one above idle + 3 headroom) */
#define EHS_XCORE_BASE_PRIORITY  (tskIDLE_PRIORITY + 3u)

/* Minimum stack in words for an EHS task */
#define EHS_XCORE_MIN_STACK_WORDS  (configMINIMAL_STACK_SIZE * 4u)

/**
 * Spawn a new FreeRTOS task for the given EHS thread function.
 *
 * EhsGeneralThreadFuncType is int(*)(void*); FreeRTOS TaskFunction_t is
 * void(*)(void*).  The cast is safe on xcore.ai (same ABI, return value
 * from the EHS function is discarded by the FreeRTOS scheduler).
 *
 * stackSize is in bytes (EHS convention); xTaskCreate takes words.
 * priority is an EHS relative value; mapped to FreeRTOS absolute priority.
 */
ehs_bool EhsHThread_execute(EhsGeneralThreadFuncType pfRun, void *context,
                             ehs_sint16 priority, ehs_sint32 stackSize,ehs_char * _szThreadname)
{
    UBaseType_t freertos_priority;
    uint32_t stack_words;
    BaseType_t result;

    freertos_priority = (UBaseType_t)((ehs_sint32)EHS_XCORE_BASE_PRIORITY + priority);
    if (freertos_priority < 1u)                     freertos_priority = 1u;
    if (freertos_priority >= configMAX_PRIORITIES)  freertos_priority = configMAX_PRIORITIES - 1u;

    stack_words = (stackSize > 0) ? (uint32_t)((uint32_t)stackSize / sizeof(StackType_t))
                                  : EHS_XCORE_MIN_STACK_WORDS;
    if (stack_words < EHS_XCORE_MIN_STACK_WORDS)    stack_words = EHS_XCORE_MIN_STACK_WORDS;
    // not the pcnmane (arg22) can be null which is supported in modern FreeRTOS
    result = xTaskCreate((TaskFunction_t)pfRun,
                         ehs_char * _szThreadname,
                         (configSTACK_DEPTH_TYPE)stack_words,
                         context,
                         freertos_priority,
                         NULL);

    if (result != pdPASS)
    {
        EHSH_LOG_ERROR("EhsHThread_execute: xTaskCreate failed (%s)",_szThreadname); // assume logger will translate NULL into "NULL" OK for %s
        return EHS_FALSE;
    }
    return EHS_TRUE;
}

void EhsTPThread_exit(void) { }

/* ---- DevMan misc-DL-data mutex / condition (one-shot init) ------------ */

EHS_LOCAL struct EhsTPMutexStruct    EhsProcess_mutexDevmanNewMiscDLData;
EHS_LOCAL struct EhsTPConditionStruct EhsProcess_condDevmanNewMiscDLData;

ehs_bool EhsProcessInitMutex(EhsTPMutexClass *reftoMutex)
{
    if (*reftoMutex == NULL)
    {
        EhsProcess_mutexDevmanNewMiscDLData.handle =
            xSemaphoreCreateRecursiveMutexStatic(
                &EhsProcess_mutexDevmanNewMiscDLData.storage);
        *reftoMutex = (EhsTPMutexClass)&EhsProcess_mutexDevmanNewMiscDLData;
    }
    else
    {
        EHSH_LOG_ERROR("Refused to assign mutexDevmanNewMiscDLData twice!");
    }
    return EHS_TRUE;
}

ehs_bool EhsProcessInitCond(EhsTPConditionClass *refToCond)
{
    if (*refToCond == NULL)
    {
        /* Zero-item-size queue: used for signalling only (no data payload). */
        EhsProcess_condDevmanNewMiscDLData.queue = xQueueCreate(1u, 0u);
        *refToCond = (EhsTPConditionClass)&EhsProcess_condDevmanNewMiscDLData;
    }
    else
    {
        EHSH_LOG_ERROR("Refused to assign condDevmanNewMiscDLData twice!");
    }
    return EHS_TRUE;
}

/* ----------------------------------------------------------------------- */

ehs_bool EhsTPThread_ChangeThisPriority(ehs_sint16 priority)
{
    (void)priority;
    return EHS_FALSE;
}

ehs_bool EhsTP_shellExecuteStdout(char *sZstdout, const char *szCmd, int max_buffer_len)
{
    (void)sZstdout;
    (void)szCmd;
    (void)max_buffer_len;
    return EHS_FALSE;
}

void EhsTargetReboot(void)
{
    /* TODO: implement XMOS reboot via rtos_support watchdog */
    printf("EhsTargetReboot: reboot requested (not yet implemented on xcore)\n");
    while (1) { }
}

ehs_sint32 EhsHProcess_getStackRemaining(void)
{
    /* No cheap stack-remaining primitive wired up for this target. */
    return -1;
}
