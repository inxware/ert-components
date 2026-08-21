/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file targetos_init.c
 * OS initialisation / environment / termination for QNX Neutrino AArch64.
 *
 * Porting notes vs linux_ALL/targetos_init.c
 * ------------------------------------------
 * 1. sys/sysinfo.h (Linux sysinfo struct) is absent on QNX.
 *    Total / free RAM is derived from POSIX sysconf(_SC_PHYS_PAGES) and
 *    sysconf(_SC_AVPHYS_PAGES) instead.
 *
 * 2. /proc/<pid>/comm and /proc/<pid>/stat are Linux-specific.
 *    get_procid_from_procname() and get_cpu_ram_info_misc() return stub
 *    values on QNX.  A QNX-native implementation would use procfs devctl
 *    calls (DCMD_PROC_STATUS / DCMD_PROC_INFO from <sys/procfs.h>).
 *    @TODO: implement QNX procfs-based CPU/RAM tracking.
 *
 * 3. /etc/lsb-release is absent on QNX.
 *    getOSVersion() uses confstr(_CS_RELEASE, ...) which returns the
 *    QNX Neutrino release string (e.g. "8.0.0").
 *
 * 4. All POSIX networking APIs (getifaddrs, socket, ioctl, inet_ntop,
 *    statvfs, getrusage, gettimeofday) are provided by QNX libsocket /
 *    libc and work identically.
 *
 * 5. pthreads are part of QNX libc since SDP 7.1 — no -lpthread needed.
 *
 * @author: inx limited
 */

#define EHS_TARGET_CODE

#include "ifaddrs.h"
#include "globals.h"
#include "targetos_init.h"
#include "callback_queue.h"
#include "blockref_table.h"
#include "hal_process.h"
#include "hal_target_sys_stat.h"

/* POSIX / standard headers */
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/param.h>
#include <sys/statvfs.h>
#include <sys/time.h>
#include <sys/resource.h>

/* QNX: memory info via sysconf instead of sysinfo() */
#include <stdlib.h>
#include <confstr.h>  /* _CS_RELEASE */

#ifdef EHS_PERIPHERALS_GPIO_SUPPORT
#include "inx_gpio.h"
#endif

/*****************************************************************************/
/* Macros */

union IPconverter
{
    struct { unsigned char a, b, c, d; } b;
    int i;
};

/*****************************************************************************/
/* Functions */

#ifdef EHS_DEBUG_TCPIP_CONSOLE
    extern EhsConsoleQueueType EhsTgtConsoleInputQueue;
    extern EhsConsoleQueueType EhsTgtConsoleOutputQueue;
    static void EhsTOS_ConsoleQueue_init(void)
    {
        EhsTgtConsoleInputQueue.xQueue  = EhsTMem_alloc(EHS_DEBUG_CONSOLE_BUFFER_SIZE);
        EhsTgtConsoleOutputQueue.xQueue = EhsTMem_alloc(EHS_DEBUG_CONSOLE_BUFFER_SIZE);
    }
#else
    static void EhsTOS_ConsoleQueue_init(void) {}
#endif

void EhsTOsSys_init(void)
{
    EhsTOS_ConsoleQueue_init();
    EhsTPMutex_init();

#ifdef EHS_PERIPHERALS_GPIO_SUPPORT
    EhsTOsInitGpio();
#endif
}

/* -------------------------------------------------------------------------
 * Network helpers
 * ------------------------------------------------------------------------- */

EHS_LOCAL ehs_bool EhsTOS_FindIp(const struct ifaddrs *ifa,
                                  const char *interface_name,
                                  ehs_char *bufIP)
{
    char addressBuffer[INET_ADDRSTRLEN];
    char addressBuffer6[INET6_ADDRSTRLEN];
    void *tmpAddrPtr;
    struct ifaddrs *ifAddrStruct;
    ehs_bool bSuccess = EHS_FALSE;

    if (interface_name != NULL && getifaddrs(&ifAddrStruct) != -1)
    {
        for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
        {
            addressBuffer[0]  = '\0';
            addressBuffer6[0] = '\0';
            if (ifa->ifa_addr == NULL) continue;
            if (EhsStrcmp(ifa->ifa_name, interface_name) == 0)
            {
                if (ifa->ifa_addr->sa_family == AF_INET)
                {
                    tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
                    inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
                    EHSH_LOG_INFO("%s IPv4 Address %s", ifa->ifa_name, addressBuffer);
                    if (EhsStrlen(addressBuffer) > 0)
                        EhsStrcpy(bufIP, addressBuffer);
                }
                else if (ifa->ifa_addr->sa_family == AF_INET6)
                {
                    tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
                    inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer6, INET6_ADDRSTRLEN);
                    EHSH_LOG_INFO("%s IPv6 Address %s", ifa->ifa_name, addressBuffer6);
                }

                if (EhsStrlen(addressBuffer) == 0)
                {
                    if (EhsStrlen(addressBuffer6) > 0)
                    {
                        EhsStrcpy(bufIP, addressBuffer6);
                        bSuccess = EHS_TRUE;
                        break;
                    }
                }
                else
                {
                    bSuccess = EHS_TRUE;
                    break;
                }
            }
        }
        if (ifAddrStruct != NULL)
            freeifaddrs(ifAddrStruct);
    }
    return bSuccess;
}

EHS_GLOBAL void EhsTOS_GetMACandIPaddr(ehs_char *buf, ehs_char *bufIP)
{
    struct ifreq buffer;
    struct ifaddrs *ifa = NULL;

    memset(&buffer, 0x00, sizeof(buffer));

#if defined(EHS_USE_WIFI_INTERFACE)
    strcpy(buffer.ifr_name, "wlan0");
#else
    /* RPi4 QNX: Broadcom GMAnet interface is typically "bcmgenet0"
     * or "eth0" depending on the QNX BSP build.  Try eth0 first. */
    strcpy(buffer.ifr_name, "eth0");
#endif

    int s = socket(PF_INET, SOCK_DGRAM, 0);
    if (s >= 0)
    {
        if (ioctl(s, SIOCGIFHWADDR, &buffer) != -1)
        {
            EhsSprintf(buf, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X",
                       (unsigned char)buffer.ifr_hwaddr.sa_data[0],
                       (unsigned char)buffer.ifr_hwaddr.sa_data[1],
                       (unsigned char)buffer.ifr_hwaddr.sa_data[2],
                       (unsigned char)buffer.ifr_hwaddr.sa_data[3],
                       (unsigned char)buffer.ifr_hwaddr.sa_data[4],
                       (unsigned char)buffer.ifr_hwaddr.sa_data[5]);
        }
        else
        {
            /* @TODO: try bcmgenet0 or query interface list */
            EhsStrcpy(buf, "na-xx-xx-xx-xx-xx");
        }
        close(s);
    }
    else
    {
        EhsStrcpy(buf, "na-xx-xx-xx-xx-xx");
    }

    EhsStrcpy(bufIP, "N/A*");
    ehs_bool success = EhsTOS_FindIp(ifa, buffer.ifr_name, bufIP);
    if (success == EHS_FALSE)
    {
#ifdef EHS_USE_WIFI_INTERFACE
        EhsTOS_FindIp(ifa, "eth0", bufIP);
#else
        EhsTOS_FindIp(ifa, "wlan0", bufIP);
#endif
    }
}

/* -------------------------------------------------------------------------
 * CPU / RAM info
 *
 * QNX does not provide sysinfo() or /proc/<pid>/stat in Linux format.
 * CPU time is tracked via getrusage() (POSIX); total/free RAM uses sysconf.
 * -------------------------------------------------------------------------*/

typedef struct timeval timeval_t;

static int timeval_subtract(struct timeval *result,
                             struct timeval *x,
                             struct timeval *y)
{
    if (x->tv_usec < y->tv_usec)
    {
        int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
        y->tv_usec -= 1000000 * nsec;
        y->tv_sec  += nsec;
    }
    if (x->tv_usec - y->tv_usec > 1000000)
    {
        int nsec = (x->tv_usec - y->tv_usec) / 1000000;
        y->tv_usec += 1000000 * nsec;
        y->tv_sec  -= nsec;
    }
    result->tv_sec  = x->tv_sec  - y->tv_sec;
    result->tv_usec = x->tv_usec - y->tv_usec;
    return x->tv_sec < y->tv_sec;
}

ehs_bool get_cpu_ram_info(ehs_uint16 *cpu_usage_percent,
                           ehs_uint32 *RAM_Size,
                           ehs_uint32 *RAM_Used,
                           ehs_uint32 *RAM_Free)
{
    struct rusage rusage;
    ehs_bool ret = EHS_FALSE;
    static ehs_uint16 last_cpu_usage_percent = 0;
    static timeval_t  Last_timeVal;
    static timeval_t  Last_userTimeUsed = { .tv_sec = 0, .tv_usec = 0 };
    static timeval_t  Last_sysTimeUsed  = { .tv_sec = 0, .tv_usec = 0 };
    unsigned long long ms_elapsed = 0;
    timeval_t diff_userTimeUsed = { .tv_sec = 0, .tv_usec = 0 };
    timeval_t diff_sysTimeUsed  = { .tv_sec = 0, .tv_usec = 0 };
    timeval_t temp_timeVal_time;
    timeval_t diff_time = { .tv_sec = 0, .tv_usec = 0 };

    gettimeofday(&temp_timeVal_time, NULL);

    if (Last_timeVal.tv_sec && Last_timeVal.tv_usec)
    {
        timeval_subtract(&diff_time, &temp_timeVal_time, &Last_timeVal);
        ms_elapsed = (unsigned long long)diff_time.tv_sec * 1000
                   + (unsigned long long)diff_time.tv_usec / 1000;
    }
    else
    {
        ms_elapsed = 0;
        Last_timeVal.tv_sec  = temp_timeVal_time.tv_sec;
        Last_timeVal.tv_usec = temp_timeVal_time.tv_usec;
    }

    if (ms_elapsed > 100)
    {
        Last_timeVal.tv_sec  = temp_timeVal_time.tv_sec;
        Last_timeVal.tv_usec = temp_timeVal_time.tv_usec;
    }

    if (getrusage(RUSAGE_SELF, &rusage) >= 0)
    {
        *RAM_Used = (ehs_uint32)(rusage.ru_maxrss);
        timeval_subtract(&diff_userTimeUsed, &rusage.ru_utime, &Last_userTimeUsed);
        timeval_subtract(&diff_sysTimeUsed,  &rusage.ru_stime, &Last_sysTimeUsed);
        if (ms_elapsed > 100)
        {
            Last_userTimeUsed.tv_sec  = rusage.ru_utime.tv_sec;
            Last_userTimeUsed.tv_usec = rusage.ru_utime.tv_usec;
            Last_sysTimeUsed.tv_sec   = rusage.ru_stime.tv_sec;
            Last_sysTimeUsed.tv_usec  = rusage.ru_stime.tv_usec;
            *cpu_usage_percent = last_cpu_usage_percent = (ehs_uint16)(
                (   (diff_userTimeUsed.tv_sec  + diff_sysTimeUsed.tv_sec)  * 100000
                  + (diff_userTimeUsed.tv_usec + diff_sysTimeUsed.tv_usec) / 10
                ) / ms_elapsed);
        }
        else
        {
            *cpu_usage_percent = last_cpu_usage_percent;
        }
        ret = EHS_TRUE;
    }
    else
    {
        EHSH_LOG_WARNING("Could not retrieve CPU and RAM info");
        *RAM_Used          = 0;
        *cpu_usage_percent = 0;
    }

    /* QNX: use POSIX sysconf for memory totals instead of sysinfo() */
    long phys_pages  = sysconf(_SC_PHYS_PAGES);
    long avph_pages  = sysconf(_SC_AVPHYS_PAGES);
    long page_size   = sysconf(_SC_PAGESIZE);

    if (phys_pages > 0 && page_size > 0)
    {
        *RAM_Size = (ehs_uint32)(((unsigned long long)phys_pages * (unsigned long long)page_size) / 1024);
        *RAM_Free = (avph_pages > 0)
                  ? (ehs_uint32)(((unsigned long long)avph_pages * (unsigned long long)page_size) / 1024)
                  : 0;
    }
    else
    {
        EHSH_LOG_WARNING("Could not retrieve RAM size from sysconf");
        *RAM_Size = 0;
        *RAM_Free = 0;
    }

    return ret;
}

/**
 * get_cpu_ram_info_misc — per-process CPU/RAM from /proc/<pid>/stat.
 *
 * @TODO: implement using QNX procfs (DCMD_PROC_STATUS devctl on /proc/<pid>/as).
 * For now returns 0 so the runtime starts cleanly.
 */
ehs_bool get_cpu_ram_info_misc(ehs_uint16 *cpu_usage_percent,
                                ehs_uint32 *RAM_Used,
                                ehs_uint32 procid)
{
    (void)procid;
    *cpu_usage_percent = 0;
    *RAM_Used          = 0;
    return EHS_FALSE;
}

/**
 * get_procid_from_procname — find process ID by name.
 *
 * @TODO: implement using QNX procfs (iterate /proc, read devctl DCMD_PROC_INFO).
 * Returns 0 (not found) for now.
 */
ehs_uint32 get_procid_from_procname(ehs_char *procname)
{
    (void)procname;
    return 0;
}

/* -------------------------------------------------------------------------
 * Disk stats (POSIX statvfs — identical on QNX and Linux)
 * ------------------------------------------------------------------------- */

ehs_bool get_dir_stats(ehs_uint32 *Size, ehs_uint32 *Used, ehs_uint32 *Free,
                       ehs_char *path)
{
    struct statvfs sbuf;
    ehs_bool ret = EHS_TRUE;

    if (statvfs(path, &sbuf) >= 0)
    {
        *Free = (ehs_uint32)(((unsigned long long)sbuf.f_bsize
                               * (unsigned long long)sbuf.f_bavail) / 1024);
        *Size = (ehs_uint32)(((unsigned long long)sbuf.f_frsize
                               * (unsigned long long)sbuf.f_blocks) / 1024);
        *Used = *Size - *Free;
    }
    else
    {
        *Free = 0;
        *Size = 0;
        *Used = 0;
        ret   = EHS_FALSE;
    }
    return ret;
}

/* -------------------------------------------------------------------------
 * OS version string
 * QNX provides the release string via confstr(_CS_RELEASE, ...) — this
 * returns something like "8.0.0".
 * -------------------------------------------------------------------------*/

void getOSVersion(ehs_char *dst)
{
    EhsStrcpy(dst, "QNX Neutrino ");
    char release[64];
    release[0] = '\0';
    confstr(_CS_RELEASE, release, sizeof(release));
    EhsStrcat(dst, release);
}

/* -------------------------------------------------------------------------
 * EhsTOsSys_UpdateEnvironment — update metadata (IP, MAC, CPU, RAM, ...)
 * ------------------------------------------------------------------------- */

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

    if (what < 2)
    {
#ifndef INX_SODL_IN_FLASH
        if (EhsStrlen(pEhsMetaData->zUserDirectory))
        {
            get_dir_stats(&pEhsMetaData->nUserSpaceTotal_KB,
                          &pEhsMetaData->nUserSpaceUsed_KB,
                          &tempint,
                          &pEhsMetaData->zUserDirectory);
        }
        else
        {
            EHSH_LOG_WARNING("User Directory has not been set, no disk stats available.");
        }
#endif
#ifdef EHS_DEVMAN_SUPPORT
        if (pEhsMetaData->MiscAppProcName[0] != 0)
        {
            pEhsMetaData->MiscAppProcId =
                get_procid_from_procname(pEhsMetaData->MiscAppProcName);
        }
#endif
    }

    EhsTOS_GetMACandIPaddr(pEhsMetaData->zDeviceID, pEhsMetaData->zDeviceIPAddr);

    get_cpu_ram_info(&pEhsMetaData->CPUUsage,
                     &pEhsMetaData->RAMTotal_KB,
                     &pEhsMetaData->RAMUsed_KB,
                     &pEhsMetaData->RAMAvail_KB);

    get_cpu_ram_info_misc(&pEhsMetaData->MiscAppCPUUsage,
                          &pEhsMetaData->MiscAppRAMUsed_KB,
                          pEhsMetaData->MiscAppProcId);

    pEhsMetaData->CPUTemp = EhsTGetCpuTemp();

    getOSVersion(pEhsMetaData->OSVersion);

    return EHS_FALSE;
}

/* -------------------------------------------------------------------------
 * Lifecycle callbacks
 * ------------------------------------------------------------------------- */

void EhsTOsSys_term(void)
{
#ifdef EHS_PERIPHERALS_GPIO_SUPPORT
    EhsTOsTermGpio();
#endif
}

void EhsTOsApp_init(void) {}
void EhsTOsApp_term(void) {}
void EhsTOsApp_reset(void) {}

void EhsBinSearchPath(void) {}
