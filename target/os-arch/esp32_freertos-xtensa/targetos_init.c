/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file targetos_init.c
 * Initialization/reset/termination code for the OS
 *
 * @author: inx limited
 *
 */

/**
 * @page Verification Verification report
 * @section targetos_init
 * @anchor targetos_init
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on
 * Last modified on $Date:$
 *
 * This file contained no derogations to the MISRA standard.
 */

/*****************************************************************************/
/* Included files */
#define EHS_TARGET_CODE /* Ensure header files include target-internal values */

//#define EHSL_MODULE_ID EHSH_LOG_MODULE_UNDEFINED

#include "globals.h"
#include "hal.h"
#include "hal_file.h"
#include "hal_string.h"
#include "hal_logger.h"
/* Stuff that should be moved to the a more specific OS specific targetos_init header file*/
/* todo2022 remove the spam from copying this from a linux os-arch */
/*
#include "targetos_init.h"
#include "callback_queue.h"
#include "blockref_table.h"
#include "hal_process.h"
#include "unistd.h"

#include "sys/types.h"
#include "netinet/in.h"
#include "linux/if.h"
#include "net/if.h" //todo2022 remove this
#include "sys/ioctl.h" //todo2022 remove this
#include "sys/types.h" //todo2022 remove this
// Network HW access
#include "sys/socket.h" //todo2022 remove this
#include "arpa/inet.h" //todo2022 remove this
#include "sys/param.h" //todo2022 remove this
//File system HW access
#include "sys/types.h" //todo2022 remove this
#ifndef EHS_ANDROID
#include "sys/statvfs.h"
#else
#include "sys/statfs.h"
#endif
// For system CPU usage
#include "sys/time.h"
#include "sys/resource.h"
#include "sys/sysinfo.h"
*/
/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */

/*****************************************************************************/
/* Variables defined with global-scope */

/*****************************************************************************/
/* Function definitions */

#include "esp_log.h"
#ifdef TAG
#undef TAG
#endif
#ifndef TAG
#define TAG "targetos_init"
#endif
/**
 * Perform necessary Operating system setup upon system initialisation
 */
#ifdef EHS_DEBUG_TCPIP_CONSOLE
    #include "console_queue.h"
    #include "target_mem.h"
    //static ehs_uint8 consoleQueueInputBuffer[EHS_DEBUG_CONSOLE_BUFFER_SIZE];
    //static ehs_uint8 consoleQueueOutputBuffer[EHS_DEBUG_CONSOLE_BUFFER_SIZE];
    extern EhsConsoleQueueType EhsTgtConsoleInputQueue;
    extern EhsConsoleQueueType EhsTgtConsoleOutputQueue;
    static void EhsTOS_ConsoleQueue_init(){
        //EhsTgtConsoleInputQueue.xQueue=consoleQueueInputBuffer;
        //EhsTgtConsoleOutputQueue.xQueue=consoleQueueOutputBuffer;
        EhsTgtConsoleInputQueue.xQueue=(ehs_uint8*)EhsTMem_alloc(EHS_DEBUG_CONSOLE_BUFFER_SIZE);
        EhsTgtConsoleOutputQueue.xQueue=(ehs_uint8*)EhsTMem_alloc(EHS_DEBUG_CONSOLE_BUFFER_SIZE);
    }
#else //#ifdef EHS_DEBUG_TCPIP_CONSOLE
    static void EhsTOS_ConsoleQueue_init(){
    }
#endif //#else #ifdef EHS_DEBUG_TCPIP_CONSOLE
void EhsTOsSys_init(void)
{
    EhsTOS_ConsoleQueue_init();
    EhsTPMutex_init();
    ESP_LOGI(TAG, "EHS inited");
}

/*
 * @todo needs extending to add interface to be interogated
 */
EHS_GLOBAL void EhsTOS_GetMACandIPaddr(ehs_char * buf,ehs_char * bufIP)
{
    EhsStrcpy(buf,"n/a");
    EhsStrcpy(bufIP,"n/a");
}

/* Some timeval functions - gnu suggested code - but obvious*/



/*
 * get RAM Usage
 */

typedef  struct timeval timeval_t;

ehs_bool get_cpu_ram_info(ehs_uint16 *cpu_usage_percent,ehs_uint32 * RAM_Size, ehs_uint32 * RAM_Used, ehs_uint32 * RAM_Free)
{
}

/*
 * Get Disk statistics - doesn't work for mingw
 */

ehs_bool get_dir_stats(ehs_uint32 * Size, ehs_uint32 * Used, ehs_uint32 * Free,
                       ehs_char * path)
{
    #if 0
    struct statfs sbuf;
    if (statfs(path, &sbuf) >= 0)
    {

        *Free = ((unsigned long long) sbuf.f_bsize * (unsigned long long)sbuf.f_bavail) / 1024;
        *Size = ((unsigned long long)sbuf.f_frsize * (unsigned long long)sbuf.f_blocks) / 1024;
        *Used = *Size - *Free;
    }
    else     // some warning nunbers..
    {
        *Free = 0;
        *Size = 0;
        *Used = 0;
        ret = EHS_FALSE;
    }
    return ret;
    #else
    return EHS_FALSE;
    #endif
}

/*
 * key value pairs in file separated by '='
 * get next value, reads in next line and returns a value trimmed right of white space
 *
 */
void getNextValue(ehs_char * dst, ehs_FILE *pFile, char * buffer)
{
    char *ptr;

    if (EhsFgets(buffer, EHS_STRING_LENGTH_MAX, pFile))  	 // read one line from file
    {
        ptr = strstr(buffer, "=");
        ptr++;	// char after '='
        ptr = EhsStrTrimR(ptr); /* consume trailing spaces */
        EhsStrcat(dst, ptr);
    }
}

void getOSVersion(ehs_char * dst)
{
    //@todo - use shell command uname -a, instead of reading from file /etc/lsb-release ?
    EhsStrcpy(dst,"Unknown");
    ehs_char * buffer;
    buffer = EhsHMem_tempAlloc(EHS_STRING_LENGTH_MAX);
    if (buffer == NULL)
        return;	// lazy return


    size_t result;
    ehs_char * c;
    ehs_char * ptr;
    ehs_char * ptrEOL;
    //todo2024 why not use malloc for these too to avoid two large stack variables
    ehs_char szKey[EHS_STRING_LENGTH_MAX];
    ehs_char tmp[EHS_STRING_LENGTH_MAX];
    long lSize = 200;
    EhsStrcpy(szKey, "DISTRIB_CODENAME=");


    ehs_FILE *pFile =EhsFopen("/etc/lsb-release","r");
    if (pFile)
    {
        EhsStrcpy(dst,"");

        for (int i=0; i<3; i++)
        {
            getNextValue(dst,pFile,buffer);
            if (i < 2)
            {
                EhsStrcat(dst, " ");	// insert some separators
            }
        }

        EhsFclose(pFile);
    }
    EhsHMem_tempFree(buffer);
}

#ifdef EHS_DEVMAN_MON_SUPPORT
ehs_bool GetDevmanBASEURL(ehs_char *szUrl)
{
    return EHS_FALSE;
}
#endif


/* updated dynamic and static data
 * The "what" parameter can be used 0: get everything, 1 get just static 2 just dynamic
 *
 * @todo Currently we update everything anyway
 * */
ehs_bool EhsTOsSys_UpdateEnvironment(EhsMetaDataType * pEhsMetaData, ehs_uint8 what)
{
    
    ehs_uint32 tempint;

    if (EhsStrlen(pEhsMetaData->zUserDirectory))
    {
        // get disk space in user directory
        ehs_char szTemp[EHS_STRING_LENGTH_MAX]; //todo2024 why do we use a buffer here and not just use pEhsMetaData->zUserDirectory?
        EhsStrcpy(szTemp,pEhsMetaData->zUserDirectory);
        get_dir_stats(&pEhsMetaData->nUserSpaceTotal_KB,&pEhsMetaData->nUserSpaceUsed_KB,&tempint,szTemp);
    }
    else
    {
        EHSH_LOG_WARNING("User Directory has not been set, no disk stats available.");
    }
    EhsTOS_GetMACandIPaddr(pEhsMetaData->zDeviceID,pEhsMetaData->zDeviceIPAddr);
    get_cpu_ram_info(&(pEhsMetaData->CPUUsage), &(pEhsMetaData->RAMTotal_KB),&(pEhsMetaData->RAMUsed_KB),&(pEhsMetaData->RAMAvail_KB));
    //getOSVersion(pEhsMetaData->OSVersion);
    return EHS_FALSE;
}

/**
 * Called to shutdown the target prior to shutting down EHS
 */
void EhsTOsSys_term(void)
{
    // Leave the mutexes to the OS EhsTPMutex_term();
}

/**
 * Perform setup before loading in a new application
 */
void EhsTOsApp_init(void)
{
//	#ifdef EHS_GUI_SUPPORT //@todo and this (as above)
//            EhsGtkKeypressCallback = NULL;
//        #endif
}

/**
 * Called to clean up after the execution of an application in order to make
 * sure everything has been stopped.
 */
void EhsTOsApp_term(void)
{
}


/**
 * Perform necessary target setup per application execution
 */
void EhsTOsApp_reset(void)
{
}



/* 	This needs to be done for all the other targets too */

void EhsBinSearchPath(void )
{
}
