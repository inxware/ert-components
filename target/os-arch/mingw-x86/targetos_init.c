/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file targetos_init.c
 * Initialization/reset/termination code for the OS
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
#include "targetos_init.h"
#include "callback_queue.h"
#include "blockref_table.h"
#include "console_server.h"
#include "hal-api.h"
#include "ehs_types.h"
/* OS Headers */
#include <unistd.h>
#include <windows.h>
#include <iphlpapi.h>
#include <winsock.h>
//#include <ws2tcpip.h>
#include <stdint.h>
#include "hal_target_sys_stat.h"


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
    EhsTgtTimer_init();
    /* start TCP/IP server thread */
}


/* / Fetches the MAC address and prints it
static void EhsTOS_GetMACaddress(unsigned char * MACData)
{


  UUID uuid;
  UuidCreateSequential( &uuid );    // Ask OS to create UUID

  for (int i=2; i<8; i++)  // Bytes 2 through 7 inclusive
                           // are MAC address
    MACData[i - 2] = uuid.Data4[i];

  //PrintMACaddress(MACData);         // Print MAC address
}
*/

static void EhsTOS_GetMACandIPaddr(ehs_char * buf, ehs_char * bufIP )
{
    IP_ADAPTER_INFO AdapterInfo[16];
    unsigned char * MACData;
    DWORD dwBufLen = sizeof(AdapterInfo);
// Call GetAdapterInfo
    DWORD dwStatus = GetAdaptersInfo(
                         AdapterInfo,
                         &dwBufLen);

// Verify return value is valid, no buffer overflow
    if (dwStatus == ERROR_SUCCESS)
    {
// Contains pointer to current adapter info
        PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; //@todo check for validity
        MACData = pAdapterInfo->Address;
        //do {
        EhsSprintf(buf,"%02X-%02X-%02X-%02X-%02X-%02X",MACData[0], MACData[1], MACData[2], MACData[3], MACData[4],MACData[5]);
        EhsSprintf(bufIP,"%s",pAdapterInfo->IpAddressList.IpAddress.String);
    }
    else
    {
        EhsStrcpy(buf,"MACID_UNKOWN");
        EhsStrcpy(bufIP,"IP ADDRESS_UNKOWN");
    }
}


ehs_bool EhsTOsSys_UpdateEnvironment(EhsMetaDataType * pEhsMetaData,ehs_uint8 which)
{
    if(which == EHS_OS_ENV_NETWORK_ID){
        // Update ip address
        EhsTOS_GetMACandIPaddr(pEhsMetaData->zDeviceID,pEhsMetaData->zDeviceIPAddr);
        // @TODO - Update network meta data using traget api, instead of using dummy values
        pEhsMetaData->nDeviceNetworkMode = EHS_NET_DHCP_MODE_ID;
        EhsStrcpy(pEhsMetaData->zDeviceGateway, "0.0.0.0");
        EhsStrcpy(pEhsMetaData->zDeviceMask, "0.0.0.0");
        EhsStrcpy(pEhsMetaData->zDeviceDNS1, "0.0.0.0");
        return EHS_TRUE; // return here, we only update network specific os env
    }

    pEhsMetaData->nUserSpaceUsed_KB=0; //@todo here
    pEhsMetaData->nUserSpaceTotal_KB=0;
    //EhsStrcpy(pEhsMetaData->zDeviceIPAddr,"unknown");  // if we are networked get IP address here
    //EhsStrcpy(pEhsMetaData->zDeviceID,"none"); //@todo here
    pEhsMetaData->CPUTemp = EhsTGetCpuTemp();
    EhsTOS_GetMACandIPaddr(pEhsMetaData->zDeviceID,pEhsMetaData->zDeviceIPAddr);
    //EHSH_LOG_INFO("\nDEVICE_ID=%s\n",pEhsMetaData->zDeviceID); // @todo note this can get any network ID, not a specific one
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


/* Mingw - @todo merge this
 *
 *
void EhsBinSearchPath(void ) {
 ehs_char * path_var [4024]; // @ todo make this a temporatry var.  USE: EhsHMem_tempAlloc EhsHMem_tempFree
 char szCwd[1024] = {'\0'};

 strncpy (path_var,"PATH=",4024);
 getcwd(szCwd, 1024);
 strncat(path_var,szCwd,4024); // @todo this should be made more robust - absolute path?? !!
 strncat(path_var,";",4024);
 strncat(path_var,getenv("PATH"),4024);
 putenv (path_var); //path_var
}
 */
/* 	This needs to be done for all the other targets too */

void EhsBinSearchPath(void )
{
    // Linux target uses run_ehs.sh script to set the environment variables.
    // putenv doesn't seem to do it properly.
    /*
    ehs_char* szPathVar = (ehs_char*)EhsHMem_tempAlloc(4024 * sizeof(ehs_char));
    ehs_char* szCwd = (ehs_char*)EhsHMem_tempAlloc(1024 * sizeof(ehs_char));

    // set PATH environment variable
    strncpy (szPathVar,"PATH=",4024);
    getcwd(szCwd, 1024);
    strncat(szPathVar,szCwd,4024);
    if (getenv("PATH")) {
    	strncat(szPathVar,":",4024);
    	strncat(szPathVar,getenv("PATH"),4024);
    }
    //putenv (szPathVar); //path_var

    // set LUA_PATH environment variable
    strncpy (szPathVar,"LUA_PATH='",4024);
    strncat(szPathVar,szCwd,4024);
    strncat(szPathVar,"/../share/lua/5.1/?.lua;;'",4024);

    // set LUA_CPATH environment variable
    strncpy (szPathVar,"LUA_CPATH='",4024);
    strncat(szPathVar,szCwd,4024);
    strncat(szPathVar,"/../lib/lua/5.1/?.so;;'",4024);

    EhsHMem_tempFree(szPathVar);
    EhsHMem_tempFree(szCwd);
    */
}
