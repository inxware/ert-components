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

#include "ehs_types.h"
#include "hal.h"
#include "hal_file.h"
#include "hal_string.h"
#include "hal_logger.h"
#include "esp_heap_caps.h"
#include "esp_mac.h"
#include "esp_netif.h"
#include "lwip/inet.h"

#include "esp_log.h"
#ifdef TAG
#undef TAG
#endif
#ifndef TAG
#define TAG "targetos_init"
#endif
#include "esp_ota_ops.h"
#include "esp_err.h"

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */
#define HASH_MOD_ID 10000000000
#define HASH_SEED_ID 19
#define PRIME_1 56893
#define PRIME_2 70607
#define HASH_NUM_LENGTH (10)
#define HASH_ID_LENGTH (HASH_NUM_LENGTH + 2) // XX-XXXX-XXXX

/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */

/*****************************************************************************/
/* Variables defined with global-scope */

/*****************************************************************************/
/* Function definitions */

// @TODO - move these to common code
static ehs_uint64 EhsStringHash(const char *s, const ehs_uint64 length, const ehs_uint64 seed)
{
	ehs_uint64 hash = seed;
	while (*s) {
		hash = (hash * PRIME_1) ^ (s[0] * PRIME_2);
		s++;
	}
	return hash % length;
}

// Generates unique id in XX-XXXX-XXXX format. Both buffers must be at least (HASH_ID_LENGTH+1) in size !
static void EhsDeviceIdGen(const char *str, char *id)
{
    if(str == NULL || id == NULL){
        return;
    }
    ehs_uint64 divisor = 1;
    ehs_uint64 hash = EhsStringHash(str, HASH_MOD_ID, HASH_SEED_ID);
    ehs_uint64 temp = hash;

    // find the divisor to get the most significant digit
    while (temp / divisor >= 10) {
        divisor *= 10;
    }

    for(ehs_uint8 i = 0; i < HASH_ID_LENGTH; i++){
        if(i == 2 || i == 7){ // 2nd and 7th char is '-' in format XX-XXXX-XXXX
			id[i] = '-';
            continue;
		}
        if(divisor != 0){
            id[i] = (ehs_uint8)(48 + (temp / divisor));
            temp %= divisor;
            divisor /= 10;
        }else{
            id[i] = '0';
        }
    }

    id[HASH_ID_LENGTH] = '\0'; // make sure id is NULL terminated
}

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
    esp_ota_mark_app_valid_cancel_rollback();
    ESP_LOGI(TAG, "EHS inited");
}

/*
 * @todo needs extending to add interface to be interogated
 */
EHS_GLOBAL void EhsTOS_GetMACandIPaddr(ehs_char * buf, ehs_char * bufIP)
{
    // get mac address
    uint8_t mac_addr[6] = {0};
    esp_mac_type_t mac_type = ESP_MAC_EFUSE_FACTORY; // Use the efuse which was burnt by Espressif in production
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_read_mac(mac_addr, mac_type));
    EhsSprintf(buf,"%X:%X:%X:%X:%X:%X", mac_addr[0], mac_addr[1], mac_addr[2],
                                        mac_addr[3], mac_addr[4], mac_addr[5]);
    // get ip address 
    esp_netif_ip_info_t ip_info;
    #if TARGET_USE_WIFI
    esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_STA_DEF"), &ip_info);
    #elif TARGET_USE_ETHERNET
    esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("ETH_DEF"), &ip_info);
    #endif
    inet_ntoa_r(ip_info.ip.addr, (char*)bufIP, 16);
}

/* Some timeval functions - gnu suggested code - but obvious*/



/*
 * get RAM Usage
 */

typedef  struct timeval timeval_t;

ehs_bool get_cpu_ram_info(ehs_uint16* cpu_usage_percent, ehs_uint32 * RAM_Size, ehs_uint32 * RAM_Used, ehs_uint32 * RAM_Free)
{
    // TODO - complete remaining 
    multi_heap_info_t info;
    heap_caps_get_info(&info, MALLOC_CAP_8BIT);
    *cpu_usage_percent = 0; // @todo
    *RAM_Size = info.total_allocated_bytes+info.total_free_bytes;
    *RAM_Used = info.total_allocated_bytes;
    *RAM_Free = info.total_free_bytes;
    
    return EHS_TRUE;
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

//Method of checking the version number string read from file
//#define XSTR(x) STR(x)
//#define STR(x) #x
//#pragma message "TARGET_OS_VERSION_STRING" XSTR(TARGET_OS_VERSION_STRING)
//#error TARGET_OS_VERSION_STRING

void getOSVersion(ehs_char * dst)
{
    if (dst == NULL) return;
    #ifndef TARGET_OS_VERSION_STRING
    #define TARGET_OS_VERSION_STRING "Unknown"
    #endif
    EhsStrcpy(dst,TARGET_OS_VERSION_STRING);
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
    // todo2024 lets try and avoid having this on the stack. If this is an init function we can use the heap as we will free straigh away before any fragmentation.
    ehs_char szTemp[EHS_STRING_LENGTH_MAX]; //todo2024 why do we use a buffer here and not just use pEhsMetaData->zUserDirectory?
    ehs_uint32 tempint;
    if (what < 2 ) {
        if (EhsStrlen(pEhsMetaData->zUserDirectory))
        {
            // get disk space in user directory
            EhsStrcpy(szTemp,pEhsMetaData->zUserDirectory);
            get_dir_stats(&pEhsMetaData->nUserSpaceTotal_KB,&pEhsMetaData->nUserSpaceUsed_KB,&tempint,szTemp);
        }
        else
        {
            EHSH_LOG_WARNING("User Directory has not been set, no disk stats available.");
        }
    }

    EhsTOS_GetMACandIPaddr(/*mac=*/szTemp, pEhsMetaData->zDeviceIPAddr); 
    
    // generate XX-XXXX-XXXX format unique id
    EhsDeviceIdGen(szTemp, pEhsMetaData->zDeviceID);

    get_cpu_ram_info(&(pEhsMetaData->CPUUsage), &(pEhsMetaData->RAMTotal_KB),&(pEhsMetaData->RAMUsed_KB),&(pEhsMetaData->RAMAvail_KB));

    getOSVersion(pEhsMetaData->zVersion);

 // todo 2024  we should move this to the target_file.c init function:
    /* Note for the esp32 we have created a partition called appdata which is referenced as /appdata/ so we are just using / here to access these in the root of that partition.*/
    EhsHMetaSetAppsPath("/");// We are using a partition just for apps on this to support 

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
