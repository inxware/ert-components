/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file targetos_init.c
 * OS initialisation/reset/termination for XMOS xcore.ai FreeRTOS targets.
 * Ported from esp32_freertos-xtensa; ESP-IDF dependencies removed.
 *
 * @author: inx limited
 */

#define EHS_TARGET_CODE

#include <stdio.h>
#include "globals.h"
#include "hal.h"
#include "hal_file.h"
#include "hal_string.h"
#include "hal_logger.h"
#include "hal_target_sys_stat.h"

#define TAG "targetos_init"

void EhsTOsSys_init(void)
{
    EhsTPMutex_init();
    printf("[%s] EHS inited\n", TAG);
}

EHS_GLOBAL void EhsTOS_GetMACandIPaddr(ehs_char *buf, ehs_char *bufIP)
{
    EhsStrcpy(buf,   "n/a");
    EhsStrcpy(bufIP, "n/a");
}

ehs_bool get_cpu_ram_info(ehs_uint16 *cpu_usage_percent,
                          ehs_uint32 *RAM_Size,
                          ehs_uint32 *RAM_Used,
                          ehs_uint32 *RAM_Free)
{
    (void)cpu_usage_percent;
    (void)RAM_Size;
    (void)RAM_Used;
    (void)RAM_Free;
    return EHS_FALSE;
}

ehs_bool get_dir_stats(ehs_uint32 *Size, ehs_uint32 *Used, ehs_uint32 *Free,
                       ehs_char *path)
{
    (void)Size;
    (void)Used;
    (void)Free;
    (void)path;
    return EHS_FALSE;
}

void getNextValue(ehs_char *dst, ehs_FILE *pFile, char *buffer)
{
    char *ptr;
    if (EhsFgets(buffer, EHS_STRING_LENGTH_MAX, pFile))
    {
        ptr = strstr(buffer, "=");
        ptr++;
        ptr = EhsStrTrimR(ptr);
        EhsStrcat(dst, ptr);
    }
}

void getOSVersion(ehs_char *dst)
{
    if (dst == NULL) return;
#ifndef TARGET_OS_VERSION_STRING
#define TARGET_OS_VERSION_STRING "Unknown"
#endif
    EhsStrcpy(dst, TARGET_OS_VERSION_STRING);
}

ehs_bool EhsTOsSys_UpdateEnvironment(EhsMetaDataType *pEhsMetaData, ehs_uint8 what)
{
    if (what == EHS_OS_ENV_NETWORK_ID)
    {
        EhsTOS_GetMACandIPaddr(pEhsMetaData->zDeviceID, pEhsMetaData->zDeviceIPAddr);
        pEhsMetaData->nDeviceNetworkMode = EHS_NET_DHCP_MODE_ID;
        EhsStrcpy(pEhsMetaData->zDeviceGateway, "0.0.0.0");
        EhsStrcpy(pEhsMetaData->zDeviceMask,    "0.0.0.0");
        EhsStrcpy(pEhsMetaData->zDeviceDNS1,    "0.0.0.0");
        return EHS_TRUE;
    }

    ehs_uint32 tempint;
    if (EhsStrlen(pEhsMetaData->zUserDirectory))
    {
        get_dir_stats(&pEhsMetaData->nUserSpaceTotal_KB,
                      &pEhsMetaData->nUserSpaceUsed_KB,
                      &tempint,
                      pEhsMetaData->zUserDirectory);
    }
    else
    {
        EHSH_LOG_WARNING("User Directory has not been set, no disk stats available.");
    }
    EhsTOS_GetMACandIPaddr(pEhsMetaData->zDeviceID, pEhsMetaData->zDeviceIPAddr);
    get_cpu_ram_info(&(pEhsMetaData->CPUUsage),
                     &(pEhsMetaData->RAMTotal_KB),
                     &(pEhsMetaData->RAMUsed_KB),
                     &(pEhsMetaData->RAMAvail_KB));
    pEhsMetaData->CPUTemp = EhsTGetCpuTemp();
    return EHS_FALSE;
}

void EhsTOsSys_term(void)   { }
void EhsTOsApp_init(void)   { }
void EhsTOsApp_term(void)   { }
void EhsTOsApp_reset(void)  { }
void EhsBinSearchPath(void) { }
