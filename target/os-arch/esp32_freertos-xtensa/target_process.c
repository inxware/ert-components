/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
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
#define EHS_TARGET_CODE /* Ensure header files include target-internal values */

//#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_PROCESS

#include <errno.h>
#include <stdio.h>

#include "globals.h"
#include "hal-api.h"
#include "target_process.h"
#include "FreeRTOSConfig.h"


#include "esp_log.h"
#include "esp_system.h"
#ifdef TAG
#undef TAG
#endif

/********************************   Private Types                           *********************************************/
/* Define types  */
typedef pthread_t EhsTPThread;


/* Ensure header files include target-internal values */
#define EHS_TARGET_CODE 
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
EHS_LOCAL pthread_mutex_t EhsL_fbIO;

/* socket client double close prevention */
EHS_LOCAL pthread_mutex_t EhsL_socketClient;

/**
 * Mutex devman requests
 */
EHS_LOCAL pthread_mutex_t EhsL_devman_request;

/**
 * Mutex resource used to control access to memory allocation
 */
EHS_LOCAL pthread_mutex_t EhsL_mem;

/**
 * Mutex resource used to control access to the console queue
 */
EHS_LOCAL pthread_mutex_t EhsL_consoleQueue;

/**
 * Mutex resource used to control access to the console input queue
 */
EHS_LOCAL pthread_mutex_t EhsL_consoleInputQueue;

/**
 * Mutex resource used to control access to the event queue
 */
EHS_LOCAL pthread_mutex_t EhsL_eventQueue;
/**
 * Mutex for counting how many application spawned threads are running.
 */
EHS_LOCAL pthread_mutex_t EhsLMutex_fb_thread_counter;

/*
  INtended to be used for various threads??
  NOT USED!
*/
//EHS_LOCAL pthread_mutex_t EhsL_globalTimer;

#ifdef EHS_NETWORKING_SUPPORT
/**
 * Mutex resource used to control access to the UrlGet Function Block
 */
EHS_LOCAL pthread_mutex_t EhsL_UrlGet;
#endif


#ifdef EHS_GUI_SUPPORT
/**
 * Mutex resource used to control access to the GUI Widget table
 */
EHS_LOCAL pthread_mutex_t EhsL_widgetTable;

/**
 * Mutex resource used to control access to viewport
 */
EHS_LOCAL pthread_mutex_t EhsL_viewport;
#endif

#ifdef EHS_DEVMAN_SUPPORT
/**
 * Mutex resource used to control access to the devman player data
 */
EHS_LOCAL pthread_mutex_t EhsL_devmanPlayerData;

/**
 * Mutex resource used to control access to shared information in playManager
 */
EHS_LOCAL pthread_mutex_t EhsL_playManager;

/**
 * Mutex resource used to control access to shared information in playManager
 */
EHS_LOCAL pthread_mutex_t EhsL_devmanMiscBuffers;
#endif

/**
 * Mutex resource used to control access to shared information in Modbus Master
 */
EHS_LOCAL pthread_mutex_t EhsL_MBMaster;

/**
 * Mutex resource used to control access to shared information in MQTT
 */
EHS_LOCAL pthread_mutex_t EhsL_subMQTT;
EHS_LOCAL pthread_mutex_t EhsL_pubMQTT;

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
 * Mutex resource ufor serialising devman server requests and keeping libcurl off it's discomfort path
 */
EhsTPMutexClass EhsTPMutex_devman_request;

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

/** Reference to PID of parent process */
pid_t* EhsT_pidParent;

/** reference to PID of TCP/IP process */
pid_t* EhsT_pidTcpIp;


/*****************************************************************************/
/* Function definitions */

/**
 * Exit from EHS. Need to release shared memory
 * @todo Tell TCP/IP thread it's time to finish
 * @todo this should not exist - we only need a direct call to the HAL one.
 */
void EhsTargetExit(ehs_uint16 exitCode)
{
    printf("EhsTargetExit %d\n",exitCode);
}

/**
 * Initialise the mutexes (Call this only once!)
 * @todo move all of the function block threads to their init functions and tear down function when implemented
 */
void EhsTPMutex_init(void)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    /* set up the posix(ish) ,utex type depending in the flavour of the platform toolchain */

    /* Set some fairly safe (minimal deadlock) mutex parameters */
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    // OR    pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);

    //memset(&EhsL_globalTimer,0,sizeof(pthread_mutex_t));
    // not used pthread_mutex_init(&EhsL_globalTimer,&attr);

    EhsTPMutex_fbIO = (EhsTPMutexClass)&EhsL_fbIO; 
    //memset(&EhsL_fbIO,0,sizeof(pthread_mutex_t));
    pthread_mutex_init(&EhsL_fbIO,&attr);
    
    EhsTPMutex_devman_request = (EhsTPMutexClass)&EhsL_devman_request;
    //memset(&EhsL_devman_request,0,sizeof(pthread_mutex_t));
    pthread_mutex_init(&EhsL_devman_request,&attr);
/* FreeRTOS version 
    memset(&EhsL_viewport,0,sizeof(pthread_mutex_t));
    sys_mutex_new(&EhsL_viewport);
    memset(&EhsL_mem,0,sizeof(pthread_mutex_t));
    sys_mutex_new(&EhsL_mem);
    memset(&EhsL_consoleQueue,0,sizeof(pthread_mutex_t));
    sys_mutex_new(&EhsL_consoleQueue);
    memset(&EhsL_consoleInputQueue,0,sizeof(pthread_mutex_t));
    sys_mutex_new(&EhsL_consoleInputQueue);
    memset(&EhsL_eventQueue,0,sizeof(pthread_mutex_t));
    sys_mutex_new(&EhsL_eventQueue);
    memset(&EhsLMutex_fb_thread_counter,0,sizeof(pthread_mutex_t));
    sys_mutex_new(&EhsLMutex_fb_thread_counter);
    memset(&EhsL_widgetTable,0,sizeof(pthread_mutex_t));
    sys_mutex_new(&EhsL_widgetTable);
    memset(&EhsL_devmanMiscBuffers,0,sizeof(pthread_mutex_t));
    sys_mutex_new(&EhsL_devmanMiscBuffers);
    memset(&EhsL_devmanPlayerData,0,sizeof(pthread_mutex_t));
    sys_mutex_new(&EhsL_devmanPlayerData);
    memset(&EhsL_playManager,0,sizeof(pthread_mutex_t));
    sys_mutex_new(&EhsL_playManager);
    memset(&EhsL_globalTimer,0,sizeof(pthread_mutex_t));
    sys_mutex_new(&EhsL_globalTimer);
*/

    EhsTPMutex_mem = (EhsTPMutexClass)&EhsL_mem;
    //memset(&EhsL_mem,0,sizeof(pthread_mutex_t));
    pthread_mutex_init(&EhsL_mem,&attr);
    
    EhsTPMutex_consoleQueue = (EhsTPMutexClass)&EhsL_consoleQueue;
    //memset(&EhsL_consoleQueue,0,sizeof(pthread_mutex_t));
    pthread_mutex_init(&EhsL_consoleQueue,&attr);
    
    EhsTPMutex_consoleInputQueue = (EhsTPMutexClass)&EhsL_consoleInputQueue;
    //memset(&EhsL_consoleInputQueue,0,sizeof(pthread_mutex_t));
    pthread_mutex_init(&EhsL_consoleInputQueue,&attr);
    
    EhsTPMutex_eventQueue = (EhsTPMutexClass)&EhsL_eventQueue;
    //memset(&EhsL_eventQueue,0,sizeof(pthread_mutex_t));
    pthread_mutex_init(&EhsL_eventQueue,&attr);
    
    EhsTPMutex_fb_thread_counter = (EhsTPMutexClass)&EhsLMutex_fb_thread_counter;
    //memset(&EhsLMutex_fb_thread_counter,0,sizeof(pthread_mutex_t));
    pthread_mutex_init(&EhsLMutex_fb_thread_counter,&attr);
    
#ifdef EHS_NETWORKING_SUPPORT
    EhsTPMutex_UrlGet = (EhsTPMutexClass)&EhsL_UrlGet;
    pthread_mutex_init(&EhsL_UrlGet,&attr);
#endif

#define EHS_NETWORKING_X_SUPPORT //todo we need to separate the basic and advanced networking at some point - but for now assume all targets have basic
#ifdef EHS_NETWORKING_X_SUPPORT
    EhsTPMutex_socketClient = (EhsTPMutexClass)&EhsL_socketClient;
    pthread_mutex_init(&EhsL_socketClient,&attr);
#endif

#ifdef EHS_GUI_SUPPORT
    EhsTPMutex_widgetTable = (EhsTPMutexClass)&EhsL_widgetTable;
    //memset(&EhsL_widgetTable,0,sizeof(pthread_mutex_t));
    pthread_mutex_init(&EhsL_widgetTable,&attr);

    EhsTPMutex_viewport = (EhsTPMutexClass)&EhsL_viewport;
    //memset(&EhsL_viewport,0,sizeof(pthread_mutex_t));
    pthread_mutex_init(&EhsL_viewport,&attr);
#endif
#ifdef EHS_DEVMAN_SUPPORT
    EhsTPMutex_devmanPlayerData = (EhsTPMutexClass)&EhsL_devmanPlayerData;
    //memset(&EhsL_devmanPlayerData,0,sizeof(pthread_mutex_t));
    pthread_mutex_init(&EhsL_devmanPlayerData,&attr);
    
    EhsTPMutex_playManager = (EhsTPMutexClass)&EhsL_playManager;
    //memset(&EhsL_playManager,0,sizeof(pthread_mutex_t));
    pthread_mutex_init(&EhsL_playManager,&attr);

    EhsTPMutex_devmanMiscBuffers = (EhsTPMutexClass)&EhsL_devmanMiscBuffers; 
    //memset(&EhsL_devmanMiscBuffers,0,sizeof(pthread_mutex_t));
    pthread_mutex_init(&EhsL_devmanMiscBuffers,&attr);
#endif

    EhsTPMutex_MBMaster = (EhsTPMutexClass)&EhsL_MBMaster;
    //memset(&EhsL_MBMaster,0,sizeof(pthread_mutex_t));
    pthread_mutex_init(&EhsL_MBMaster,&attr);

    EhsTPMutex_subMQTT = (EhsTPMutexClass)&EhsL_subMQTT;
    //memset(&EhsL_subMQTT,0,sizeof(pthread_mutex_t));
    pthread_mutex_init(&EhsL_subMQTT,&attr);

    EhsTPMutex_pubMQTT = (EhsTPMutexClass)&EhsL_pubMQTT;
    //memset(&EhsL_pubMQTT,0,sizeof(pthread_mutex_t));
    pthread_mutex_init(&EhsL_pubMQTT,&attr);

    // not used: EhsTPMutex_globalTimer =(EhsTPMutexClass)&EhsL_globalTimer;

    pthread_mutexattr_destroy(&attr);
}

/**
 * Shutdown the mutexes - not actually used/.
 *
 * Trust: Called after mutexes are not being polled
 * @todo We should actually check if it's a good idea to do this. We never call this function ....
 */
void EhsTPMutex_term(void)  //@todo and these need to gp too when we have the teardown compliment to instorage class specified for parameter ‘bcmp’it is implemented.
{
    if (EhsTPMutex_fbIO) pthread_mutex_destroy((pthread_mutex_t *) EhsTPMutex_fbIO);
    EhsTPMutex_fbIO = NULL;

    if (EhsTPMutex_socketClient) pthread_mutex_destroy((pthread_mutex_t *) EhsTPMutex_socketClient);
    EhsTPMutex_socketClient = NULL;

    if (EhsTPMutex_viewport) pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_viewport);
    EhsTPMutex_viewport = NULL;
    if (EhsTPMutex_mem) pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_mem);
    EhsTPMutex_mem = NULL;
    if (EhsTPMutex_consoleQueue) pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_consoleQueue);
    EhsTPMutex_consoleQueue = NULL;
    if (EhsTPMutex_consoleInputQueue) pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_consoleInputQueue);
    EhsTPMutex_consoleInputQueue = NULL;
    if (EhsTPMutex_eventQueue) pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_eventQueue);
    EhsTPMutex_eventQueue = NULL;
    if (EhsTPMutex_fb_thread_counter) pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_fb_thread_counter);
    EhsTPMutex_fb_thread_counter = NULL;
    if (EhsTPMutex_widgetTable) pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_widgetTable);
    EhsTPMutex_widgetTable = NULL;

    if (EhsTPMutex_MBMaster) pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_MBMaster);
    EhsTPMutex_MBMaster = NULL; 

    if (EhsTPMutex_subMQTT) pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_subMQTT);
    EhsTPMutex_subMQTT = NULL;

    if (EhsTPMutex_pubMQTT) pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_pubMQTT);
    EhsTPMutex_pubMQTT = NULL;

#ifdef EHS_DEVMAN_SUPPORT
    if (EhsTPMutex_devmanPlayerData) pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_devmanPlayerData);
    EhsTPMutex_devmanPlayerData = NULL;
    if (EhsTPMutex_playManager) pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_playManager);
    EhsTPMutex_playManager = NULL;
    /* Not destroying the core devman monitor */
#endif

#ifdef EHS_NETWORKING_SUPPORT
    if (EhsTPMutex_UrlGet) pthread_mutex_destroy((pthread_mutex_t *)EhsTPMutex_UrlGet);
    EhsTPMutex_UrlGet = NULL; 
#endif    
}


/**
 * Execute a function from a function block in a separate thread
 * priority should be 0 - 100 and is capped at these extremes typically for Posix
 */
// todo2022 review if we want to use the absolute max (how cooperative is EHS? and does this override interrupts or any drivers?)
// previously we have set this to one above the TCPIP_THREAD_PROCESS priority
#ifndef tskIDLE_PRIORITY
#define tskIDLE_PRIORITY 0
#endif
#define CONFIG_MAIN_THREAD_PRIORITY tskIDLE_PRIORITY+3//(configMAX_PRIORITIES-1) //TCPIP_THREAD_PRIO + 1

//@todo this function should allow values below -100 to revert sched other scheduling - and adopt the processe's default native values

/* FreeRTOS Method 
ehs_bool EhsHThread_execute(EhsGeneralThreadFuncType pfRun, void* context,ehs_sint16 priority)
{
    ESP_LOGI(TAG, "EhsHThread_execute");
    EhsTPThread thread;
    thread = sys_thread_new("ehs", (lwip_thread_fn)pfRun, context, 2000, CONFIG_MAIN_THREAD_PRIORITY);
    return (thread != NULL);
}
*/

//@todo this function should allow values below -100 to revert sched other scheduling - and adopt the processe's default native values
ehs_bool EhsHThread_execute(EhsGeneralThreadFuncType pfRun, void* context, ehs_sint16 priority, ehs_sint32 stackSize)
{
    EhsTPThread thread;
    pthread_attr_t tattr_param;
    struct sched_param param;
    int ret;
#ifdef EHS_USE_AGGRESSIVE_REALTIME_FEATURES
    int maxpri=20;//sched_get_priority_max(SCHED_RR);
    int minpri=0;//sched_get_priority_min(SCHED_RR);
#else
    int maxpri=20;//sched_get_priority_max(SCHED_OTHER);
    int minpri=0;//sched_get_priority_min(SCHED_OTHER);
#endif

    /*Todo thread priorities should be made relative to Current priority */
    /* We will assume althese are done under sched_other for linux */
    /* initialized with default attributes */
    ret = pthread_attr_init(&tattr_param);
    param.sched_priority =maxpri+priority;
    param.sched_priority=(param.sched_priority<minpri) ? minpri : param.sched_priority;
    param.sched_priority=(param.sched_priority>maxpri) ? maxpri : param.sched_priority;
#ifdef EHS_USE_AGGRESSIVE_REALTIME_FEATURES
   // pthread_attr_setschedpolicy(&tattr_param, SCHED_RR);
#else
  //  pthread_attr_setschedpolicy(&tattr_param, SCHED_OTHER); // use linux scheeduling
#endif
    //ret = pthread_attr_setschedparam(&tattr_param,&param);

    pthread_attr_setdetachstate(&tattr_param,PTHREAD_CREATE_DETACHED);
    //@todo if we want real time we need to make this only for very short functions - otherwise it will kill drivers and crash linux.
#ifdef EHS_USE_AGGRESSIVE_REALTIME_FEATURES
   // pthread_attr_setscope(&tattr_param, PTHREAD_SCOPE_SYSTEM); /* system-wide contention for linux - CAREFUL - THIS WILL KILL divers eventually under heavy load*/
#else
   // pthread_attr_setscope(&tattr_param, PTHREAD_SCOPE_PROCESS); /* system-wide contention for linux*/
#endif
    //   int getpriority(int which, int who);
    /* @todo we through the above away and use null for now. This needs sorting when EHS SCHED_RR is re-instated */
    /* cast pfRun to return void* with one arg of void* */

    /* @todo : We need to clone the instance data here, this required the size of the object to be known outside of the component, and housekeeping (garbage collection) is required for terminated threads that do not use a terminate or proper completion exit path..*/
    ret=pthread_create(&thread,&tattr_param,(void*(*)(void*))pfRun,context);
    pthread_attr_destroy(&tattr_param);
    switch ( ret )
    {
    case	0 : // this is good
        break;
    case 	EAGAIN :
        EHSH_LOG_ERROR("ERROR Could not create thread: no resources");
        break;
    case	EINVAL :
        EHSH_LOG_ERROR("ERROR Could not create thread: attribute information invalid");
        break;
    case	EPERM  :
        EHSH_LOG_ERROR("ERROR Could not create thread: Client does not have necessary permissions");
        break;
    default:
        EHSH_LOG_ERROR("ERROR Could not create thread: Unknown Error");;
    }
    //ret = pthread_attr_destroy(&tattr);
    return (ret == 0);
}


void EhsTPThread_exit()
{

}

/* These are for the Devman process that gets some pass throughOS data in a fussy way  */ 
EHS_LOCAL pthread_mutex_t EhsProcess_mutexDevmanNewMiscDLData = PTHREAD_MUTEX_INITIALIZER ;
/* Returns a pointer to struct member it is called with */
 ehs_bool EhsProcessInitMutex(EhsTPMutexClass *reftoMutex) 
 {
    if (*reftoMutex == NULL ) {
        *reftoMutex = (EhsTPMutexClass*)&EhsProcess_mutexDevmanNewMiscDLData;
    }
    else {
        EHSH_LOG_ERROR("Refused to Assig mutexDevmanNewMiscDLData Twice!");
    }
 } // note this will provide the same mutex to all callers - only expecting one call at initi time...


EHS_LOCAL pthread_cond_t condDevmanNewMiscDLData = PTHREAD_COND_INITIALIZER;
 ehs_bool EhsProcessInitCond(EhsTPConditionClass * refToCond)
 { 
    if (*refToCond == NULL ) {
        *refToCond = (EhsTPConditionClass*)&condDevmanNewMiscDLData;
    }
    else {
        EHSH_LOG_ERROR("Refised to assigning mutexDevmanNewMiscDLData Twice!");
    }

 } 


/**
 * Execute a function from a function block in a separate thread
 * returns
 */

ehs_bool EhsTPThread_ChangeThisPriority(ehs_sint16 priority)
{
    return EHS_FALSE;
}

#ifdef EHS_RE_INTRODUCE_THREAD_HANDLES
/* Kill a thread !!! don't use if you can avoid !!! */
int EhsTPThread_terminate(EhsTPThread  thread)
{
    return 0;
}
#endif

/* Does a shell execute, returning stdout ( only up to available size ).
 * @todo - make a version that pumps out more data
 */
ehs_bool EhsTP_shellExecuteStdout(char* sZstdout,const char * szCmd, int max_buffer_len)
{
    return EHS_FALSE;
}


void EhsTargetReboot( void )
{
    esp_restart();
}
