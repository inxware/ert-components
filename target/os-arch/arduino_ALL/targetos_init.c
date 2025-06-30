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
#include <mbed.h>
#include <mbed_stats.h>
#include "targetos_init.h"
#include "target_network.h"
#include "ehs_types.h"
#include "callback_queue.h"
#include "hal_process.h"


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

/**
 * Perform necessary Operating system setup upon system initialisation
 */
#ifdef EHS_DEBUG_TCPIP_CONSOLE
    extern EhsConsoleQueueType EhsTgtConsoleInputQueue;
    extern EhsConsoleQueueType EhsTgtConsoleOutputQueue;
    static void EhsTOS_ConsoleQueue_init(){
        EhsTgtConsoleInputQueue.xQueue=EhsTMem_alloc(EHS_DEBUG_CONSOLE_BUFFER_SIZE);
        EhsTgtConsoleOutputQueue.xQueue=EhsTMem_alloc(EHS_DEBUG_CONSOLE_BUFFER_SIZE);
    }
#else //#ifdef EHS_DEBUG_TCPIP_CONSOLE
    static void EhsTOS_ConsoleQueue_init(){
    }
#endif //#else #ifdef EHS_DEBUG_TCPIP_CONSOLE

void EhsTOsSys_init(void)
{
    EhsTOS_ConsoleQueue_init();
    EhsTPMutex_init();
}

/*
 * @todo needs extending to add interface to be interogated
 */
EHS_GLOBAL void EhsTOS_GetMACandIPaddr(ehs_char* buf, ehs_char* bufIP)
{
#ifdef EHS_COMPONENT_NETWORKING_SUPPORT
    EhsTgtNetworkGetMAC(buf, EHS_NET_MAC_ID_LENGTH_MAX);
    EhsTgtNetworkGetIPv4(bufIP, EHS_IP_ADDR_LENGTH_MAX);
#else
    EhsStrcpy(buf, "N/A");
    EhsStrcpy(bufIP, "N/A");
#endif
    
}

ehs_bool get_cpu_ram_info(ehs_uint16 * cpu_usage_percent, ehs_uint32 * RAM_Size_KB, ehs_uint32 * RAM_Used_KB, ehs_uint32 * RAM_Free_KB)
{
#if defined(MBED_CPU_STATS_ENABLED) && defined(___THIS_DOES_NOTHING_ATM)
    us_timestamp_t cpu_usage = 0;
    mbed_stats_cpu_t stats;
    mbed_stats_cpu_get(&stats);
    // Calculate CPU usage as an integer percentage
    us_timestamp_t uptime = stats.uptime;
    us_timestamp_t idle_time = stats.idle_time;
    if ((uptime + idle_time) != 0) {
        cpu_usage = (uptime * 100) / (uptime + idle_time); // Integer-based percentage
    }
    *cpu_usage_percent = (ehs_uint16)cpu_usage;
#else
    *cpu_usage_percent = 0;
#endif
    mbed_stats_heap_t heap_stats;
    mbed_stats_heap_get(&heap_stats);
    *RAM_Size_KB = (ehs_uint32)((heap_stats.reserved_size-heap_stats.current_size)/1000);
    *RAM_Used_KB = (ehs_uint32)(heap_stats.current_size/1000);
    *RAM_Free_KB = *RAM_Size_KB - *RAM_Used_KB;
    return EHS_TRUE;
}


ehs_bool get_cpu_ram_info_misc(ehs_uint16 *cpu_usage_percent, ehs_uint32 * RAM_Used, ehs_uint32 procid)
{
    *cpu_usage_percent = 0;
    *RAM_Used = 0;
    return EHS_FALSE;
}

/*
 * Get Disk statistics
 */

ehs_bool get_dir_stats(ehs_uint32 * SizeKB, ehs_uint32 * UsedKB, ehs_uint32 * FreeKB, const ehs_char * path)
{
    ehs_uint64 Size = 0, Used = 0, Free = 0;
    // get sizes in bytes
    ehs_bool ret = EhsTgtFilesystem_Get_FS_Sizes(&Size, &Used, &Free);
    *SizeKB = (ehs_uint32) (Size/1000);
    *UsedKB = (ehs_uint32) (Used/1000);
    *FreeKB = (ehs_uint32) (Free/1000);
    return ret;
}

void getOSVersion(ehs_char * dst)
{
    // @TODO
    EhsStrcpy(dst,"Unknown");
}

ehs_sint16 EhsTGetCpuTemp()
{
    // @TODO
    // https://github.com/DeimosHall/RP2040_CPU_Temperature/blob/main/src/CPU.cpp
    return 0;
}

/* updated dynamic and static data
 * The "what" parameter can be used 0: get everything, 1 get just static 2 just dynamic
 *
 * @todo Currently we update everything anyway
 * */
ehs_bool EhsTOsSys_UpdateEnvironment(EhsMetaDataType * pEhsMetaData, ehs_uint8 what)
{
    EhsTOS_GetMACandIPaddr(pEhsMetaData->zDeviceID, pEhsMetaData->zDeviceIPAddr);

    if(what == EHS_OS_ENV_NETWORK_ID){
        // @TODO - Update network meta data using traget api, instead of using dummy values
        pEhsMetaData->nDeviceNetworkMode = EHS_NET_DHCP_MODE_ID;
        EhsStrcpy(pEhsMetaData->zDeviceGateway, "0.0.0.0");
        EhsStrcpy(pEhsMetaData->zDeviceMask, "0.0.0.0");
        EhsStrcpy(pEhsMetaData->zDeviceDNS1, "0.0.0.0");
        return EHS_TRUE; // return here, we only update network specific os env
    }

    if (what < 2) {
        #ifndef INX_SODL_IN_FLASH 
        //ehs_char szTemp[EHS_STRING_LENGTH_MAX]; //todo2024 why do we use a buffer here and not just use pEhsMetaData->zUserDirectory?
        if (EhsStrlen(pEhsMetaData->zUserDirectory)){
            // get disk space in user directory
            ehs_uint32 tempint;
            //EhsStrcpy(szTemp,pEhsMetaData->zUserDirectory);
            get_dir_stats(&pEhsMetaData->nUserSpaceTotal_KB, &pEhsMetaData->nUserSpaceUsed_KB, &tempint, pEhsMetaData->zUserDirectory);
        } else {
            EHSH_LOG_WARNING("User Directory has not been set, no disk stats available.");
        }
        #endif
        /* todo we may want the rtinfo function block to work, even without Devman, but saving memory for now*/
        #ifdef EHS_DEVMAN_SUPPORT
        if (pEhsMetaData->MiscAppProcName[0] != 0) {
            pEhsMetaData->MiscAppProcId=get_procid_from_procname(pEhsMetaData->MiscAppProcName);
        }
        #endif
    }
       
    get_cpu_ram_info(&(pEhsMetaData->CPUUsage), &(pEhsMetaData->RAMTotal_KB),&(pEhsMetaData->RAMUsed_KB),&(pEhsMetaData->RAMAvail_KB));
    ehs_uint16 temp_cpu_misc = 0;
    get_cpu_ram_info_misc(&(temp_cpu_misc),&(pEhsMetaData->MiscAppRAMUsed_KB),pEhsMetaData->MiscAppProcId);
    pEhsMetaData->MiscAppCPUUsage = (ehs_uint32)temp_cpu_misc;
    pEhsMetaData->CPUTemp = EhsTGetCpuTemp();

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
