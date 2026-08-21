/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_process.c
 * Zephyr RTOS implementation of process/thread/mutex management.
 * Uses Zephyr kernel APIs: k_mutex, k_thread.
 *
 * @author: inx limited
 *
 */

/*****************************************************************************/
/* Included files */
#define EHS_TARGET_CODE /* Ensure header files include target-internal values */

#include "globals.h"
#include "hal-api.h"
#include "target_process.h"
#include <zephyr/kernel.h>
#include <zephyr/sys/reboot.h>

#include <errno.h>
#include <stdio.h>

/********************************   Private Types   *************************/

/*****************************************************************************/
/* Variables defined with file-scope */
/*
 * Private scoped allocations of each mutex.
 * The public references are published via the header.
 * Zephyr k_mutex instances - statically allocated.
 */

EHS_LOCAL struct k_mutex EhsL_fbIO;
EHS_LOCAL struct k_mutex EhsL_socketClient;
EHS_LOCAL struct k_mutex EhsL_devman_request;
EHS_LOCAL struct k_mutex EhsL_mem;
EHS_LOCAL struct k_mutex EhsL_consoleQueue;
EHS_LOCAL struct k_mutex EhsL_consoleInputQueue;
EHS_LOCAL struct k_mutex EhsL_eventQueue;
EHS_LOCAL struct k_mutex EhsL_fb_thread_counter;

#ifdef EHS_NETWORKING_SUPPORT
EHS_LOCAL struct k_mutex EhsL_UrlGet;
#endif

#ifdef EHS_GUI_SUPPORT
EHS_LOCAL struct k_mutex EhsL_widgetTable;
EHS_LOCAL struct k_mutex EhsL_viewport;
#endif

#ifdef EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER
EHS_LOCAL struct k_mutex EhsL_devmanPlayerData;
EHS_LOCAL struct k_mutex EhsL_playManager;
EHS_LOCAL struct k_mutex EhsL_devmanMiscBuffers;
#endif

EHS_LOCAL struct k_mutex EhsL_MBMaster;
EHS_LOCAL struct k_mutex EhsL_subMQTT;
EHS_LOCAL struct k_mutex EhsL_pubMQTT;

/*****************************************************************************/
/* Variables defined with global-scope */

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

#ifdef EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER
EhsTPMutexClass EhsTPMutex_playManager;
#endif

EhsTPMutexClass EhsTPMutex_MBMaster;
EhsTPMutexClass EhsTPMutex_subMQTT;
EhsTPMutexClass EhsTPMutex_pubMQTT;

/* pid_t not available in Zephyr's picolibc; these variables are unused on MCU targets */
/* TODO: We should be able to delete these because we are in a Zephyr only file anyway?*/
#ifndef EHS_ZEPHYR_RTOS
EHS_GLOBAL pid_t* EhsT_pidParent;
EHS_GLOBAL pid_t* EhsT_pidTcpIp;
#endif

/*****************************************************************************/
/* Function definitions */

/**
 * Exit from EHS. On Zephyr MCU, this triggers a system reboot.
 */
void EhsTargetExit(ehs_uint16 exitCode)
{
    printf("EhsTargetExit %d\n", exitCode);
    sys_reboot(SYS_REBOOT_COLD);
}

/**
 * Helper macro to initialise a Zephyr mutex and assign the public pointer
 */
#define INIT_MUTEX(pub, priv) do { \
    k_mutex_init(&(priv)); \
    (pub) = (EhsTPMutexClass)&(priv); \
} while(0)

/**
 * Initialise the mutexes (Call this only once!)
 * Zephyr k_mutex supports priority inheritance and is recursive-safe
 * when locked multiple times from the same thread.
 */
EHS_GLOBAL void EhsTPMutex_init(void)
{
    INIT_MUTEX(EhsTPMutex_fbIO, EhsL_fbIO);
    INIT_MUTEX(EhsTPMutex_devman_request, EhsL_devman_request);
    INIT_MUTEX(EhsTPMutex_mem, EhsL_mem);
    INIT_MUTEX(EhsTPMutex_consoleQueue, EhsL_consoleQueue);
    INIT_MUTEX(EhsTPMutex_consoleInputQueue, EhsL_consoleInputQueue);
    INIT_MUTEX(EhsTPMutex_eventQueue, EhsL_eventQueue);
    INIT_MUTEX(EhsTPMutex_fb_thread_counter, EhsL_fb_thread_counter);
    INIT_MUTEX(EhsTPMutex_socketClient, EhsL_socketClient);

#ifdef EHS_NETWORKING_SUPPORT
    INIT_MUTEX(EhsTPMutex_UrlGet, EhsL_UrlGet);
#endif

#ifdef EHS_GUI_SUPPORT
    INIT_MUTEX(EhsTPMutex_widgetTable, EhsL_widgetTable);
    INIT_MUTEX(EhsTPMutex_viewport, EhsL_viewport);
#endif

#ifdef EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER
    INIT_MUTEX(EhsTPMutex_devmanPlayerData, EhsL_devmanPlayerData);
    INIT_MUTEX(EhsTPMutex_playManager, EhsL_playManager);
    INIT_MUTEX(EhsTPMutex_devmanMiscBuffers, EhsL_devmanMiscBuffers);
#endif

    INIT_MUTEX(EhsTPMutex_MBMaster, EhsL_MBMaster);
    INIT_MUTEX(EhsTPMutex_subMQTT, EhsL_subMQTT);
    INIT_MUTEX(EhsTPMutex_pubMQTT, EhsL_pubMQTT);
}

/**
 * Shutdown the mutexes. Not typically called on MCU targets.
 */
void EhsTPMutex_term(void)
{
    /* Zephyr k_mutex does not have a destroy/deinit function -
     * mutexes are statically allocated and persist for device lifetime.
     * Set public pointers to NULL for safety. */
    EhsTPMutex_fbIO = NULL;
    EhsTPMutex_socketClient = NULL;
    EhsTPMutex_viewport = NULL;
    EhsTPMutex_mem = NULL;
    EhsTPMutex_consoleQueue = NULL;
    EhsTPMutex_consoleInputQueue = NULL;
    EhsTPMutex_eventQueue = NULL;
    EhsTPMutex_fb_thread_counter = NULL;
    EhsTPMutex_widgetTable = NULL;
    EhsTPMutex_MBMaster = NULL;
    EhsTPMutex_subMQTT = NULL;
    EhsTPMutex_pubMQTT = NULL;

#ifdef EHS_DEVMAN_SUPPORT
    EhsTPMutex_devmanPlayerData = NULL;
#endif

#ifdef EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER
    EhsTPMutex_playManager = NULL;
#endif

#ifdef EHS_NETWORKING_SUPPORT
    EhsTPMutex_UrlGet = NULL;
#endif
}

/**
 * Default thread stack size for dynamically spawned threads on Zephyr.
 * Zephyr requires statically allocated stacks. For dynamic thread creation
 * we use a pool of pre-allocated stacks.
 *
 * SIZE THIS DELIBERATELY - the pool is stacks x slots of BSS and it is
 * allocated WHETHER OR NOT the slots are ever used. On a part where
 * CONFIG_COMMON_LIBC_MALLOC_ARENA_SIZE=-1 (nRF9151), every unused slot is
 * taken straight out of the malloc arena that EhsTMem_alloc/hal_mem.c hands to
 * the EHS kernel for SODL parsing and function-block instances - so an
 * over-generous pool shows up as "**Error: Out of memory" when an app loads,
 * with nothing to point at the thread pool as the cause.
 *
 * The default of 8 x 4096 = 32 KB was fine on parts with RAM to spare. Both
 * are overridable from a platform config.mk via DEFS. Running out of slots is
 * handled gracefully (EhsHThread_execute logs and returns EHS_FALSE), so trim
 * to what the target actually spawns plus a margin. Typical users: the Wi-Fi
 * station SM, the Lucid console server, and any FB that starts a thread.
 *
 * Do not trim the STACK size below 4096 without checking the Wi-Fi station
 * thread: its scan-result handler puts a 2047-byte ehs_char[EHS_STRING_LENGTH_MAX]
 * on the stack.
 */
#ifndef EHS_ZEPHYR_DEFAULT_STACK_SIZE
#define EHS_ZEPHYR_DEFAULT_STACK_SIZE 4096
#endif
#ifndef EHS_ZEPHYR_MAX_DYNAMIC_THREADS
#define EHS_ZEPHYR_MAX_DYNAMIC_THREADS 8
#endif

/* Pool of thread stacks and thread structs for dynamic thread creation */
static K_THREAD_STACK_ARRAY_DEFINE(ehs_thread_stacks, EHS_ZEPHYR_MAX_DYNAMIC_THREADS, EHS_ZEPHYR_DEFAULT_STACK_SIZE);
static struct k_thread ehs_thread_data[EHS_ZEPHYR_MAX_DYNAMIC_THREADS];
static bool ehs_thread_in_use[EHS_ZEPHYR_MAX_DYNAMIC_THREADS];
static struct k_mutex ehs_thread_pool_mutex;
static bool ehs_thread_pool_init = false;

static int ehs_alloc_thread_slot(void)
{
    if (!ehs_thread_pool_init) {
        k_mutex_init(&ehs_thread_pool_mutex);
        for (int i = 0; i < EHS_ZEPHYR_MAX_DYNAMIC_THREADS; i++) {
            ehs_thread_in_use[i] = false;
        }
        ehs_thread_pool_init = true;
    }

    k_mutex_lock(&ehs_thread_pool_mutex, K_FOREVER);
    for (int i = 0; i < EHS_ZEPHYR_MAX_DYNAMIC_THREADS; i++) {
        if (!ehs_thread_in_use[i]) {
            ehs_thread_in_use[i] = true;
            k_mutex_unlock(&ehs_thread_pool_mutex);
            return i;
        }
    }
    k_mutex_unlock(&ehs_thread_pool_mutex);
    return -1;
}

/**
 * Execute a function in a separate thread.
 * On Zephyr, threads require pre-allocated stacks. We use a small pool.
 *
 * @param pfRun     Thread entry function
 * @param context   Argument passed to the thread
 * @param priority  EHS priority (-100 to 100, mapped to Zephyr priorities)
 * @param stackSize Stack size (ignored on Zephyr - uses pool default)
 * @return EHS_TRUE on success, EHS_FALSE on failure
 */
EHS_GLOBAL ehs_bool EhsHThread_execute(EhsGeneralThreadFuncType pfRun, void* context, ehs_sint16 priority, ehs_sint32 stackSize, ehs_char * _szThreadname)
{
    int slot = ehs_alloc_thread_slot();
    if (slot < 0) {
        EHSH_LOG_ERROR("ERROR Could not create thread: no free thread slots");
        return EHS_FALSE;
    }

    /* Map EHS priority to Zephyr cooperative/preemptive priority.
     * Zephyr: lower number = higher priority.
     * Preemptive priorities: 0 (highest) to CONFIG_NUM_PREEMPT_PRIORITIES-1
     * We map EHS priority (higher = more important) to Zephyr range. */
    int zephyr_prio = 7 - (priority / 15); /* rough mapping to 0-14 range */
    if (zephyr_prio < 0) zephyr_prio = 0;
    if (zephyr_prio > 14) zephyr_prio = 14;

    k_thread_create(&ehs_thread_data[slot],
                    ehs_thread_stacks[slot],
                    EHS_ZEPHYR_DEFAULT_STACK_SIZE,
                    (k_thread_entry_t)pfRun,
                    context, NULL, NULL,
                    zephyr_prio, 0, K_NO_WAIT);

    /* Name the thread. The caller always passes one and it used to be dropped
     * on the floor, so every EHS dynamic thread - the Wi-Fi station state
     * machine, the Lucid console server, any FB thread - was anonymous in a
     * fault dump: "Current thread: 0x20011a80 (unknown)", which is the one
     * piece of information you actually need. Needs CONFIG_THREAD_NAME=y (set
     * in target.mk); without it k_thread_name_set is a harmless no-op. */
    if (_szThreadname != NULL)
    {
        k_thread_name_set(&ehs_thread_data[slot], (const char *)_szThreadname);
    }
    else
    {
        k_thread_name_set(&ehs_thread_data[slot], "ehs_dyn");
    }

    return EHS_TRUE;
}

EHS_GLOBAL void EhsTPThread_exit(void)
{
    /* On Zephyr, a thread exits by returning from its entry function.
     * We could call k_thread_abort(k_current_get()) but that's not
     * recommended. For now this is a no-op. */
}

/* Devman mutex/cond support - simplified for Zephyr */
EHS_LOCAL struct k_mutex EhsProcess_mutexDevmanNewMiscDLData;

ehs_bool EhsProcessInitMutex(EhsTPMutexClass *reftoMutex)
{
    if (*reftoMutex == NULL) {
        k_mutex_init(&EhsProcess_mutexDevmanNewMiscDLData);
        *reftoMutex = (EhsTPMutexClass)&EhsProcess_mutexDevmanNewMiscDLData;
    } else {
        EHSH_LOG_ERROR("Refused to assign mutexDevmanNewMiscDLData twice!");
    }
    return EHS_TRUE;
}

EHS_LOCAL struct k_condvar condDevmanNewMiscDLData;

ehs_bool EhsProcessInitCond(EhsTPConditionClass *refToCond)
{
    if (*refToCond == NULL) {
        k_condvar_init(&condDevmanNewMiscDLData);
        *refToCond = (EhsTPConditionClass)&condDevmanNewMiscDLData;
    } else {
        EHSH_LOG_ERROR("Refused to assign condDevmanNewMiscDLData twice!");
    }
    return EHS_TRUE;
}

ehs_bool EhsTPThread_ChangeThisPriority(ehs_sint16 priority)
{
    /* TODO: implement via k_thread_priority_set(k_current_get(), ...) */
    return EHS_FALSE;
}

#ifdef EHS_RE_INTRODUCE_THREAD_HANDLES
int EhsTPThread_terminate(EhsTPThread thread)
{
    /* k_thread_abort() can be used but is dangerous */
    return 0;
}
#endif

ehs_bool EhsTP_shellExecuteStdout(char* sZstdout, const char *szCmd, int max_buffer_len)
{
    /* Shell execution not available on MCU targets */
    return EHS_FALSE;
}

void EhsTargetReboot(void)
{
    EHSH_LOG_INFO("Target Rebooting...");
    EhsApplicationReset();
    sys_reboot(SYS_REBOOT_COLD);
}

ehs_sint32 EhsHProcess_getStackRemaining(void)
{
    /* Zephyr has k_thread_stack_space_get() (needs CONFIG_THREAD_STACK_INFO) but it isn't
     * wired up on this target yet - not implemented rather than guessed. */
    return -1;
}
