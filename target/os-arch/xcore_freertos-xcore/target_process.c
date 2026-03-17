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
 * fwk_rtos exposes a POSIX pthread layer over FreeRTOS, so this file is
 * largely the same as the ESP32 port with esp_restart() replaced.
 *
 * @author: inx limited
 */

#define EHS_TARGET_CODE

#include <errno.h>
#include <stdio.h>

#include "globals.h"
#include "hal-api.h"
#include "target_process.h"

typedef pthread_t EhsTPThread;

/* ---- Private mutex storage -------------------------------------------- */

EHS_LOCAL pthread_mutex_t EhsL_fbIO;
EHS_LOCAL pthread_mutex_t EhsL_socketClient;
EHS_LOCAL pthread_mutex_t EhsL_devman_request;
EHS_LOCAL pthread_mutex_t EhsL_mem;
EHS_LOCAL pthread_mutex_t EhsL_consoleQueue;
EHS_LOCAL pthread_mutex_t EhsL_consoleInputQueue;
EHS_LOCAL pthread_mutex_t EhsL_eventQueue;
EHS_LOCAL pthread_mutex_t EhsLMutex_fb_thread_counter;

#ifdef EHS_NETWORKING_SUPPORT
EHS_LOCAL pthread_mutex_t EhsL_UrlGet;
#endif

#ifdef EHS_GUI_SUPPORT
EHS_LOCAL pthread_mutex_t EhsL_widgetTable;
EHS_LOCAL pthread_mutex_t EhsL_viewport;
#endif

#ifdef EHS_DEVMAN_SUPPORT
EHS_LOCAL pthread_mutex_t EhsL_devmanPlayerData;
EHS_LOCAL pthread_mutex_t EhsL_playManager;
EHS_LOCAL pthread_mutex_t EhsL_devmanMiscBuffers;
#endif

EHS_LOCAL pthread_mutex_t EhsL_MBMaster;
EHS_LOCAL pthread_mutex_t EhsL_subMQTT;
EHS_LOCAL pthread_mutex_t EhsL_pubMQTT;

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

pid_t *EhsT_pidParent;
pid_t *EhsT_pidTcpIp;

/* ----------------------------------------------------------------------- */

void EhsTargetExit(ehs_uint16 exitCode)
{
    printf("EhsTargetExit %d\n", exitCode);
}

void EhsTPMutex_init(void)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

    EhsTPMutex_fbIO = (EhsTPMutexClass)&EhsL_fbIO;
    pthread_mutex_init(&EhsL_fbIO, &attr);

    EhsTPMutex_devman_request = (EhsTPMutexClass)&EhsL_devman_request;
    pthread_mutex_init(&EhsL_devman_request, &attr);

    EhsTPMutex_mem = (EhsTPMutexClass)&EhsL_mem;
    pthread_mutex_init(&EhsL_mem, &attr);

    EhsTPMutex_consoleQueue = (EhsTPMutexClass)&EhsL_consoleQueue;
    pthread_mutex_init(&EhsL_consoleQueue, &attr);

    EhsTPMutex_consoleInputQueue = (EhsTPMutexClass)&EhsL_consoleInputQueue;
    pthread_mutex_init(&EhsL_consoleInputQueue, &attr);

    EhsTPMutex_eventQueue = (EhsTPMutexClass)&EhsL_eventQueue;
    pthread_mutex_init(&EhsL_eventQueue, &attr);

    EhsTPMutex_fb_thread_counter = (EhsTPMutexClass)&EhsLMutex_fb_thread_counter;
    pthread_mutex_init(&EhsLMutex_fb_thread_counter, &attr);

    EhsTPMutex_socketClient = (EhsTPMutexClass)&EhsL_socketClient;
    pthread_mutex_init(&EhsL_socketClient, &attr);

#ifdef EHS_NETWORKING_SUPPORT
    EhsTPMutex_UrlGet = (EhsTPMutexClass)&EhsL_UrlGet;
    pthread_mutex_init(&EhsL_UrlGet, &attr);
#endif

#ifdef EHS_GUI_SUPPORT
    EhsTPMutex_widgetTable = (EhsTPMutexClass)&EhsL_widgetTable;
    pthread_mutex_init(&EhsL_widgetTable, &attr);

    EhsTPMutex_viewport = (EhsTPMutexClass)&EhsL_viewport;
    pthread_mutex_init(&EhsL_viewport, &attr);
#endif

#ifdef EHS_DEVMAN_SUPPORT
    EhsTPMutex_devmanPlayerData = (EhsTPMutexClass)&EhsL_devmanPlayerData;
    pthread_mutex_init(&EhsL_devmanPlayerData, &attr);

    EhsTPMutex_playManager = (EhsTPMutexClass)&EhsL_playManager;
    pthread_mutex_init(&EhsL_playManager, &attr);

    EhsTPMutex_devmanMiscBuffers = (EhsTPMutexClass)&EhsL_devmanMiscBuffers;
    pthread_mutex_init(&EhsL_devmanMiscBuffers, &attr);
#endif

    EhsTPMutex_MBMaster = (EhsTPMutexClass)&EhsL_MBMaster;
    pthread_mutex_init(&EhsL_MBMaster, &attr);

    EhsTPMutex_subMQTT = (EhsTPMutexClass)&EhsL_subMQTT;
    pthread_mutex_init(&EhsL_subMQTT, &attr);

    EhsTPMutex_pubMQTT = (EhsTPMutexClass)&EhsL_pubMQTT;
    pthread_mutex_init(&EhsL_pubMQTT, &attr);

    pthread_mutexattr_destroy(&attr);
}

void EhsTPMutex_term(void)
{
    if (EhsTPMutex_fbIO)        { pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_fbIO);        EhsTPMutex_fbIO        = NULL; }
    if (EhsTPMutex_socketClient){ pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_socketClient); EhsTPMutex_socketClient = NULL; }
    if (EhsTPMutex_viewport)    { pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_viewport);     EhsTPMutex_viewport     = NULL; }
    if (EhsTPMutex_mem)         { pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_mem);          EhsTPMutex_mem          = NULL; }
    if (EhsTPMutex_consoleQueue){ pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_consoleQueue); EhsTPMutex_consoleQueue = NULL; }
    if (EhsTPMutex_consoleInputQueue){ pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_consoleInputQueue); EhsTPMutex_consoleInputQueue = NULL; }
    if (EhsTPMutex_eventQueue)  { pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_eventQueue);   EhsTPMutex_eventQueue   = NULL; }
    if (EhsTPMutex_fb_thread_counter){ pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_fb_thread_counter); EhsTPMutex_fb_thread_counter = NULL; }
    if (EhsTPMutex_widgetTable) { pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_widgetTable);  EhsTPMutex_widgetTable  = NULL; }
    if (EhsTPMutex_MBMaster)    { pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_MBMaster);     EhsTPMutex_MBMaster     = NULL; }
    if (EhsTPMutex_subMQTT)     { pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_subMQTT);      EhsTPMutex_subMQTT      = NULL; }
    if (EhsTPMutex_pubMQTT)     { pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_pubMQTT);      EhsTPMutex_pubMQTT      = NULL; }

#ifdef EHS_DEVMAN_SUPPORT
    if (EhsTPMutex_devmanPlayerData)  { pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_devmanPlayerData);  EhsTPMutex_devmanPlayerData  = NULL; }
    if (EhsTPMutex_playManager)       { pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_playManager);       EhsTPMutex_playManager       = NULL; }
#endif

#ifdef EHS_NETWORKING_SUPPORT
    if (EhsTPMutex_UrlGet) { pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_UrlGet); EhsTPMutex_UrlGet = NULL; }
#endif
}

#ifndef tskIDLE_PRIORITY
#define tskIDLE_PRIORITY 0
#endif
#define CONFIG_MAIN_THREAD_PRIORITY (tskIDLE_PRIORITY + 3)

ehs_bool EhsHThread_execute(EhsGeneralThreadFuncType pfRun, void *context,
                             ehs_sint16 priority, ehs_sint32 stackSize)
{
    EhsTPThread thread;
    pthread_attr_t tattr_param;
    struct sched_param param;
    int maxpri = 20;
    int minpri = 0;
    int ret;

    ret = pthread_attr_init(&tattr_param);
    param.sched_priority = maxpri + priority;
    if (param.sched_priority < minpri) param.sched_priority = minpri;
    if (param.sched_priority > maxpri) param.sched_priority = maxpri;

    pthread_attr_setdetachstate(&tattr_param, PTHREAD_CREATE_DETACHED);

    ret = pthread_create(&thread, &tattr_param, (void *(*)(void *))pfRun, context);
    pthread_attr_destroy(&tattr_param);

    switch (ret)
    {
    case 0:
        break;
    case EAGAIN:
        EHSH_LOG_ERROR("ERROR Could not create thread: no resources");
        break;
    case EINVAL:
        EHSH_LOG_ERROR("ERROR Could not create thread: invalid attributes");
        break;
    case EPERM:
        EHSH_LOG_ERROR("ERROR Could not create thread: insufficient permissions");
        break;
    default:
        EHSH_LOG_ERROR("ERROR Could not create thread: unknown error");
        break;
    }
    return (ret == 0);
}

void EhsTPThread_exit(void) { }

EHS_LOCAL pthread_mutex_t EhsProcess_mutexDevmanNewMiscDLData = PTHREAD_MUTEX_INITIALIZER;

ehs_bool EhsProcessInitMutex(EhsTPMutexClass *reftoMutex)
{
    if (*reftoMutex == NULL)
    {
        *reftoMutex = (EhsTPMutexClass *)&EhsProcess_mutexDevmanNewMiscDLData;
    }
    else
    {
        EHSH_LOG_ERROR("Refused to assign mutexDevmanNewMiscDLData twice!");
    }
    return EHS_TRUE;
}

EHS_LOCAL pthread_cond_t condDevmanNewMiscDLData = PTHREAD_COND_INITIALIZER;

ehs_bool EhsProcessInitCond(EhsTPConditionClass *refToCond)
{
    if (*refToCond == NULL)
    {
        *refToCond = (EhsTPConditionClass *)&condDevmanNewMiscDLData;
    }
    else
    {
        EHSH_LOG_ERROR("Refused to assign condDevmanNewMiscDLData twice!");
    }
    return EHS_TRUE;
}

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
    /* TODO: implement XMOS reboot via rtos_support or watchdog */
    printf("EhsTargetReboot: reboot requested (not yet implemented on xcore)\n");
    while (1) { }
}
