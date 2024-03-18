/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file target_process.c
 * Maps HAL process operations onto underlying process operations
 *
 * @author: inx limited
 *
 */


/**
 * @page Verification Verification report
 * @section hal_graphics_font
 * @anchor hal_graphics_font
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on
 * Last modified on $Date:$
 *
 * This file contained no derogations to the MISRA standard.
 */

/*****************************************************************************/
/* Included files */
//#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_MEMORY
#include "hal_logger.h"
#include "hal_process.h"
#include "target.h"
#include "globals.h"

/* #define NTDDI_WINXP
#define _WIN32_WINNT 0x0501

is defined in target_config.h
*/

//#define WINVER 0x0A00
//#define _WIN32_WINNT 0x0A00


#include <windows.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <shlwapi.h>
#include <shellapi.h>

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

struct EhsTPMutexStruct
{
    CRITICAL_SECTION hMutex;
};

/*
 *
 * typedefs
 */


/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope - all of these should be created dynamically using a thread create function*/

struct EhsTPMutexStruct EhsLMutex_fbIO;
struct EhsTPMutexStruct EhsLMutex_socketClient;
struct EhsTPMutexStruct EhsLMutex_viewport;
struct EhsTPMutexStruct EhsLMutex_mem;
struct EhsTPMutexStruct EhsLMutex_consoleQueue;
struct EhsTPMutexStruct EhsLMutex_consoleInputQueue;
struct EhsTPMutexStruct EhsLMutex_eventQueue;
struct EhsTPMutexStruct EhsLMutex_fb_thread_counter;
/*****************************************************************************/
/* Variables defined with global-scope */

EhsTPMutexClass EhsTPMutex_fb_thread_counter;


/**
 * Mutex resource used to control access to the function block inputs and outputs
 */
EhsTPMutexClass EhsTPMutex_fbIO;

/* Used by the socket library to stop double closes  on FDs
 */
EhsTPMutexClass EhsTPMutex_socketClient;

/**
 * Mutex resource used to control access to viewport
 */
EhsTPMutexClass EhsTPMutex_viewport;

/**
 * Mutex resource used to create memory allocation
 */
EhsTPMutexClass EhsTPMutex_mem;

/**
 * Mutex resource used to control access to the console queue
 */
EhsTPMutexClass EhsTPMutex_consoleQueue;

/**
 * Mutex resource used to control access to the console queue
 */
EhsTPMutexClass EhsTPMutex_consoleInputQueue;

/**
 * Mutex resource used to control access to the event queue
 */
EhsTPMutexClass EhsTPMutex_eventQueue;



/*****************************************************************************/
/* Function definitions */

/**
 * Initialise the mutexes
 * @todo handle cases where InitializeCritical... returns false
 *
 */
WINBASEAPI BOOL WINAPI
InitializeCriticalSectionAndSpinCount(LPCRITICAL_SECTION,DWORD);

void EhsTPMutex_init(void)
{
    EHSH_LOG_ENTER("EhsTPMutex_init()");
    InitializeCriticalSectionAndSpinCount(&(EhsLMutex_viewport.hMutex), 1000 /* spin count */
                                          /* spin count is needed in SMP systems - indicates how long to 'spin' before calling
                                           * more expensive wait function. Should be proportional to maximum lock time
                                           */);
    EhsTPMutex_viewport = &EhsLMutex_viewport;
    EHSH_LOG_INFO("EhsTPMuxtex_viewport = %x",EhsTPMutex_viewport);

    InitializeCriticalSectionAndSpinCount(&(EhsLMutex_fbIO.hMutex),100);
    EhsTPMutex_fbIO = &EhsLMutex_fbIO;
    EHSH_LOG_INFO("EhsTPMutex_fbIO = %x",EhsTPMutex_fbIO);

    InitializeCriticalSectionAndSpinCount(&(EhsLMutex_socketClient.hMutex),100);
    EhsTPMutex_socketClient = &EhsLMutex_socketClient;
    EHSH_LOG_INFO("EhsTPMutex_socketClient = %x",EhsTPMutex_socketClient);

    InitializeCriticalSectionAndSpinCount(&(EhsLMutex_mem.hMutex),100);
    EhsTPMutex_mem = &EhsLMutex_mem;
    EHSH_LOG_INFO("EhsTPMutex_mem = %x",EhsTPMutex_mem);

    InitializeCriticalSectionAndSpinCount(&(EhsLMutex_consoleQueue.hMutex),100);
    EhsTPMutex_consoleQueue = &EhsLMutex_consoleQueue;
    EHSH_LOG_INFO("EhsTPMutex_consoleQueue = %x",EhsTPMutex_consoleQueue);

    InitializeCriticalSectionAndSpinCount(&(EhsLMutex_consoleInputQueue.hMutex),100);
    EhsTPMutex_consoleInputQueue = &EhsLMutex_consoleInputQueue;
    EHSH_LOG_INFO("EhsTPMutex_consoleInputQueue = %x",EhsTPMutex_consoleInputQueue);

    InitializeCriticalSectionAndSpinCount(&(EhsLMutex_eventQueue.hMutex),100);
    EhsTPMutex_eventQueue = &EhsLMutex_eventQueue;

    InitializeCriticalSectionAndSpinCount(&(EhsLMutex_fb_thread_counter.hMutex),100);
    EhsTPMutex_fb_thread_counter = &EhsLMutex_fb_thread_counter;

    EHSH_LOG_INFO("EhsTPMutex_eventQueue = %x",EhsTPMutex_eventQueue);
    EHSH_LOG_EXIT("EhsTPMutex_init()");
}

/**
 * Shutdown the mutexes
 */
void EhsTPMutex_term(void)
{
    EHSH_LOG_ENTER("EhsTPMutex_term()");
    DeleteCriticalSection(&(EhsTPMutex_viewport->hMutex));
    DeleteCriticalSection(&(EhsTPMutex_fbIO->hMutex));
    DeleteCriticalSection(&(EhsTPMutex_socketClient->hMutex));
    DeleteCriticalSection(&(EhsTPMutex_mem->hMutex));
    DeleteCriticalSection(&(EhsTPMutex_consoleQueue->hMutex));
    DeleteCriticalSection(&(EhsTPMutex_consoleInputQueue->hMutex));
    DeleteCriticalSection(&(EhsTPMutex_fb_thread_counter->hMutex));
    EHSH_LOG_EXIT("EhsTPMutex_term()");
}

/**
 * Lock a mutex to indicate the start of a region where we perform exclusive handling by a thread/process
 *
 * @param[in] pMutexRef Indicates the identity of the mutex we are locking
 */
void EhsTPMutex_lock(EhsTPMutexClass pMutexRef)
{
    EHSH_LOG_ENTER("EhsTPMutex_lock(%x)",pMutexRef);
    EnterCriticalSection(&(pMutexRef->hMutex));
    EHSH_LOG_EXIT("EhsTPMutex_lock()");
}

/**
 * Release a mutex indicating the end of a unique access region
 *
 * @param[in] pMutexRef Indicates the identity of the mutex we are releasing
 */
void EhsTPMutex_unlock(EhsTPMutexClass pMutexRef)
{
    EHSH_LOG_ENTER("EhsTPMutex_unlock(%x)",pMutexRef);
    LeaveCriticalSection(&(pMutexRef->hMutex));
    EHSH_LOG_EXIT("EhsTPMutex_unlock()");
}

/**
 * Execute a function from a function block in a separate thread
 */
EHS_GLOBAL ehs_bool EhsHThread_execute(EhsGeneralThreadFuncType* pfRun, void* context)
{
    EHSH_LOG_ENTER("EhsTPThread_execute(%x,%x)",pfRun, context);
    CreateThread(NULL,0,pfRun,context,0,NULL);
    EHSH_LOG_EXIT("EhsTPThread_execute");
}

#ifdef EHS_RE_INTRODUCE_THREAD_HANDLES

int EhsTPThread_terminate(EhsTPThread thread)
{
    DWORD dwExitCode=0;
    return TerminateThread(
               thread,
               dwExitCode);
}
#endif

/**
 * Execute a command using the system's shell. This function will block until the command has
 * completed.
 * @param[in] szCmd Command to execute
 */
void EhsTP_shellExecute(const ehs_char* szCmd)
{
    char szData1[EHS_STRING_LENGTH_MAX] = {'\0'};
    char* s1 = &szData1[0];
    char* s2;
    wchar_t szWData1[EHS_STRING_LENGTH_MAX] = {'\0'};
    wchar_t *ws1 = &szWData1[0];
    wchar_t szWData2[EHS_STRING_LENGTH_MAX] = {'\0'};
    wchar_t *ws2 = &szWData2[0];
    SHELLEXECUTEINFO ShExecInfo = {0};
    EHSH_LOG_ENTER("EhsTP_shellExecute(%s)",szCmd);

    /* split command line into s1 (executable) and s2 (parameters) */
    s1 = szCmd;
    s2 = EhsGetWordFromString(szData1, s1);

    /* convert s1 and s2 into wide character strings */
    mbstowcs(ws1, szData1, EHS_STRING_LENGTH_MAX);
    if (s2)
    {
        mbstowcs(ws2, s2, EHS_STRING_LENGTH_MAX);
    }
    else
    {
        ws2 = L'\0';
    }

    /* execute s1 with s2 as parameter */
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd = NULL;
    ShExecInfo.lpVerb = L"open";
    ShExecInfo.lpFile = ws1;
    ShExecInfo.lpParameters = ws2;
    ShExecInfo.lpDirectory = NULL;
    ShExecInfo.nShow = SW_HIDE;
    ShExecInfo.hInstApp = NULL;
    ShellExecuteEx(&ShExecInfo);
    WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
    EHSH_LOG_EXIT("EhsTP_shellExecute()");
}

void EhsTargetReboot( void )
{
    while (0);
    EHSH_LOG_ERROR("Reboot Not Implemented on this target!");
}