/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file target_process.c
 * description
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
/* Ensure header files include target-internal values */
#define EHS_TARGET_CODE 

#include <Arduino.h>
#include <mbed.h>

#include "target.h"
#include "hal-api.h"
#include "target_process.h"


/********************************   Private Types                           *********************************************/
/* Define types  */
typedef rtos::Thread EhsTPThread;
typedef rtos::Mutex EhsTPMutex; // non-recursive !!!


/* Set the Debug level for this module */
//#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_PROCESS

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */


/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */
/* 
The following are the private scoped allocations of each mutex we are going to use.
   The public references to these are published in the target_process.h header        
   */

/**
 * Mutex resource used to control access to the function block inputs and outputs
 */
EHS_LOCAL EhsTPMutex EhsL_fbIO;

/**
 * Mutex resource used to control access to memory allocation
 */
EHS_LOCAL EhsTPMutex EhsL_mem;

/**
 * Mutex resource used to control access to the console queue
 */
EHS_LOCAL EhsTPMutex EhsL_consoleQueue;

/**
 * Mutex resource used to control access to the console input queue
 */
EHS_LOCAL EhsTPMutex EhsL_consoleInputQueue;

/**
 * Mutex resource used to control access to the event queue
 */
EHS_LOCAL EhsTPMutex EhsL_eventQueue;
/**
 * Mutex for counting how many application spawned threads are running.
 */
EHS_LOCAL EhsTPMutex EhsLMutex_fb_thread_counter;

/*
  INtended to be used for various threads??
  NOT USED!
*/
//EHS_LOCAL EhsTPMutex EhsL_globalTimer;

#ifdef EHS_NETWORKING_SUPPORT
/**
 * Mutex resource used to control access to the UrlGet Function Block
 */
EHS_LOCAL EhsTPMutex EhsL_UrlGet;

#endif

/* socket client double close prevention */
EHS_LOCAL EhsTPMutex EhsL_socketClient;

#ifdef EHS_GUI_SUPPORT
/**
 * Mutex resource used to control access to the GUI Widget table
 */
EHS_LOCAL EhsTPMutex EhsL_widgetTable;

/**
 * Mutex resource used to control access to viewport
 */
EHS_LOCAL EhsTPMutex EhsL_viewport;

#endif

#ifdef EHS_DEVMAN_SUPPORT
/**
 * Mutex resource used to control access to the devman player data
 */
EHS_LOCAL EhsTPMutex EhsL_devmanPlayerData;

/**
 * Mutex resource used to control access to shared information in playManager
 */
EHS_LOCAL EhsTPMutex EhsL_playManager;

/**
 * Mutex resource used to control access to shared information in playManager
 */
EHS_LOCAL EhsTPMutex EhsL_devmanMiscBuffers;

/**
 * Mutex devman requests
 */
EHS_LOCAL EhsTPMutex EhsL_devman_request;
#endif

/**
 * Mutex resource used to control access to shared information in Modbus Master
 */
EHS_LOCAL EhsTPMutex EhsL_MBMaster;

/**
 * Mutex resource used to control access to shared information in MQTT
 */
EHS_LOCAL EhsTPMutex EhsL_subMQTT;
EHS_LOCAL EhsTPMutex EhsL_pubMQTT;

/*****************************************************************************/
/* Variables defined with global-scope */

/* The following are  pointers to the above mutex variables which are published via the header file */
/* These are more convenient labels that are used publicly outside of this file                     */
/**
 * Mutex resource used to control access to the function block inputs and outputs
 */
EhsTPMutexClass EhsTPMutex_fbIO;

/* 
    Mutes to avoid Socket client double close prevention - Used in function blocks only
*/
EhsTPMutexClass EhsTPMutex_socketClient;

/**
 * Mutex resource used to control access to viewport
 */
EhsTPMutexClass EhsTPMutex_viewport;

/**
 * Mutex resource used to control access to memory allocation
 */
EhsTPMutexClass EhsTPMutex_mem;

/**
 * Mutex resource used to control access to the console queue
 */
EhsTPMutexClass EhsTPMutex_consoleQueue;

/**
 * Mutex resource used to control access to the console input queue
 */
EhsTPMutexClass EhsTPMutex_consoleInputQueue;

/**
 * Mutex resource used to control access to the event queue(s)
 */
EhsTPMutexClass EhsTPMutex_eventQueue;


 /**
 * Mutex for counting how many application spawned threads are running.
 */
EhsTPMutexClass EhsTPMutex_fb_thread_counter;


EhsTPMutexClass EhsTPMutex_widgetTable;

/*
 * Mutexes for timers
 * Timers might be added in threads
 * 
 * NOT USED!
 */

//EhsTPMutexClass EhsTPMutex_globalTimer;

#ifdef EHS_NETWORKING_SUPPORT
/**
 * Mutex resource used to control access to the devman player data
 */
EhsTPMutexClass EhsTPMutex_UrlGet;
#endif

#ifdef EHS_DEVMAN_SUPPORT
xx
/**
 * Mutex resource ufor serialising devman server requests and keeping libcurl off it's discomfort path
 */
EhsTPMutexClass EhsTPMutex_devman_request;

/**
 * Mutex resource used to control access to the devman player data
 */
EhsTPMutexClass EhsTPMutex_devmanPlayerData;

/**
 * Used for the devman core App interface block
 */
// delete me: EhsTPMutexClass  EhsTPMutex_devmanInterface;

/**
  * Used for the mutexing the DL and UP biffers for devman comms
  */
EhsTPMutexClass  EhsTPMutex_devmanMiscBuffers;

#endif

#ifdef EHS_MEDIA_SUPPORT
/**
 * Mutex resource used to control access to the playManager shared resources
 */
EhsTPMutexClass EhsTPMutex_playManager;
#endif

/**
 * Mutex resource used to control access to the Modbus Master shared resources
 */
EhsTPMutexClass EhsTPMutex_MBMaster;

/**
 * Mutex resource used to control access to the MQTT shared resources
 */
EhsTPMutexClass EhsTPMutex_subMQTT;
EhsTPMutexClass EhsTPMutex_pubMQTT;


/*****************************************************************************/
/* Function definitions */

void EhsTgtMutex_lock(void* pMutexRef)
{
    if(pMutexRef) ((EhsTPMutex*)pMutexRef)->lock();
}
/**
 * Release a mutex indicating the end of a unique access region
 *
 * @param[in] pMutexRef Indicates the identity of the mutex we are releasing
 */
void EhsTgtMutex_unlock(void* pMutexRef)
{
    if(pMutexRef) ((EhsTPMutex*)pMutexRef)->unlock();
}

/**
 * Exit from EHS. Need to release shared memory
 * @todo Tell TCP/IP thread it's time to finish
 * @todo this should not exist - we only need a direct call to the HAL one.
 */
void EhsTargetExit(ehs_uint16 exitCode)
{
    exit((EhsTargetIntType)exitCode); /*lint !e586 Target specific derogation - exit can be permitted; */
}

/**
 * Initialise the mutexes (Call this only once!)
 * @todo move all of the function block threads to their init functions and tear down function when implemented
 */
EHS_GLOBAL void EhsTPMutex_init(void)
{
    EhsTPMutex_fbIO = (EhsTPMutexClass)&EhsL_fbIO; 
    EhsTPMutex_mem = (EhsTPMutexClass)&EhsL_mem;
    EhsTPMutex_consoleQueue = (EhsTPMutexClass)&EhsL_consoleQueue;
    EhsTPMutex_consoleInputQueue = (EhsTPMutexClass)&EhsL_consoleInputQueue;    
    EhsTPMutex_eventQueue = (EhsTPMutexClass)&EhsL_eventQueue;
    EhsTPMutex_fb_thread_counter = (EhsTPMutexClass)&EhsLMutex_fb_thread_counter;
    EhsTPMutex_socketClient = (EhsTPMutexClass)&EhsL_socketClient;
#ifdef EHS_NETWORKING_SUPPORT
    EhsTPMutex_UrlGet = (EhsTPMutexClass)&EhsL_UrlGet;
#endif

#ifdef EHS_GUI_SUPPORT
    EhsTPMutex_widgetTable = (EhsTPMutexClass)&EhsL_widgetTable;
    EhsTPMutex_viewport = (EhsTPMutexClass)&EhsL_viewport;
#endif
#ifdef EHS_DEVMAN_SUPPORT
    EhsTPMutex_devman_request = (EhsTPMutexClass)&EhsL_devman_request;
    EhsTPMutex_devmanPlayerData = (EhsTPMutexClass)&EhsL_devmanPlayerData;
    EhsTPMutex_devmanMiscBuffers = (EhsTPMutexClass)&EhsL_devmanMiscBuffers;
 #endif
 #ifdef EHS_MEDIA_SUPPORT   
    EhsTPMutex_playManager = (EhsTPMutexClass)&EhsL_playManager;
#endif

    EhsTPMutex_MBMaster = (EhsTPMutexClass)&EhsL_MBMaster;

    EhsTPMutex_subMQTT = (EhsTPMutexClass)&EhsL_subMQTT;
    EhsTPMutex_pubMQTT = (EhsTPMutexClass)&EhsL_pubMQTT;
}

/**
 * Shutdown the mutexes - not actually used/.
 *
 * Trust: Called after mutexes are not being polled
 * @todo We should actually check if it's a good idea to do this. We never call this function ....
 */
void EhsTPMutex_term(void)  //@todo and these need to gp too when we have the teardown compliment to instorage class specified for parameter ‘bcmp’it is implemented.
{
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
#ifdef EHS_MEDIA_SUPPORT
    EhsTPMutex_playManager = NULL;
#endif
#ifdef EHS_NETWORKING_SUPPORT
    EhsTPMutex_UrlGet = NULL; 
#endif 
}


/**
 * Execute a function from a function block in a separate thread
 * priority should be 0 - 100 and is capped at these extremes typically for Posix
 */

//@todo this function should allow values below -100 to revert sched other scheduling - and adopt the processe's default native values
EHS_GLOBAL ehs_bool EhsHThread_execute(EhsGeneralThreadFuncType pfRun, void* context, ehs_sint16 priority, ehs_sint32 stackSize)
{
    uint32_t ssize = ( stackSize > 0 ) ? (uint32_t)stackSize : OS_STACK_SIZE;
    EhsTPThread * t = new EhsTPThread((osPriority)priority, ssize, nullptr, nullptr);
    t->start( mbed::callback([=]() { pfRun(context); }) );
    EhsStdioPrintf("EhsHThread_execute (id=%d) pri=%d stk=%d\n", t->get_id(), priority, (int)ssize);
    return EHS_TRUE;
}

/**
 * @brief Returns current thread ID
 */
unsigned int EhsTargetThreadID()
{
    return (unsigned int)rtos::ThisThread::get_id();
}

EHS_GLOBAL void EhsTPThread_exit()
{
    osThreadId_t const current_thread_id = rtos::ThisThread::get_id();
    EhsStdioPrintf("EhsTPThread_exit (id=%d)\n", (int)current_thread_id);
    // @TODO make sure to delete thread, may need to keep a list of thread objects
    // and compare the thread id
}

/* Returns a pointer to struct member it is called with */
 ehs_bool EhsProcessInitMutex(EhsTPMutexClass *reftoMutex) 
 {
    return EHS_TRUE;
 } // note this will provide the same mutex to all callers - only expecting one call at initi time...


ehs_bool EhsProcessInitCond(EhsTPConditionClass * refToCond)
 { 
    return EHS_FALSE; // not used
 } 

/**
 * Execute a function from a function block in a separate thread
 * returns
 */

ehs_bool EhsTPThread_ChangeThisPriority(ehs_sint16 priority)
{
    return EHS_FALSE; // not used
}


/* Kill a thread !!! don't use if you can avoid !!! */
int EhsTPThread_terminate(EhsTPThread  thread)
{
    return 0; // not used
}


/* Does a shell execute, returning stdout ( only up to available size ).
 * @todo - make a version that pumps out more data
 */
ehs_bool EhsTP_shellExecuteStdout(char* sZstdout,const char * szCmd, int max_buffer_len)
{

    // Not mutexing as popen is expected to be thread safe as long as no parent thread has a stream open

    /*FILE *command_p = popen(szCmd, "r");

    if (!command_p)
    {
        sZstdout[0] = '\0';
        return EHS_FALSE;
    }
    else
    {
        ehs_char *line_p = fgets(sZstdout, max_buffer_len, command_p);
        pclose(command_p);
        return EHS_TRUE;
    }*/

}


void EhsTargetReboot( void )
{
    
}
