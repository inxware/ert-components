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

#define EHSL_MODULE_ID EHSH_LOG_MODULE_UNDEFINED

/* Stuff that should be moved to the a more specific OS specific targetos_init header file*/
#ifndef EHS_ANDROID
#include "ifaddrs.h"
#endif

#include "targetos_init.h"
#include "callback_queue.h"
#include "blockref_table.h"
#include "hal_process.h"
//#include "console_server.h"
#include "ehs_types.h"
/* OS Headers */
#include "unistd.h"

/* OS Specific Stuff */
#include "sys/types.h"
#include "netinet/in.h"
#include "linux/if.h"
#include "net/if.h"
#include "sys/ioctl.h"
#include "sys/types.h"
// Network HW access
#include "sys/socket.h"
#include "arpa/inet.h"
#include "sys/param.h"
//File system HW access
#include "sys/types.h"
#ifndef EHS_ANDROID
#include "sys/statvfs.h"
#else
#include "sys/statfs.h"
#endif
// For system CPU usage
#include "sys/time.h"
#include "sys/resource.h"
#include "sys/sysinfo.h"
#include "unistd.h"
/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */

/*****************************************************************************/
/* Variables defined with global-scope */
#ifdef EHS_GUI_SUPPORT
//extern EhsBlockRefType EhsBlockRefTable_gtk[];
//extern EhsCallbackQueueType EhsGtkKeypressCallback;
#endif
/*****************************************************************************/
/* Function definitions */

/**
 * Perform necessary Operating system setup upon system initialisation
 */
void EhsTOsSys_init(void)
{
    EhsTPMutex_init();
#ifdef EHS_MINGW
    EhsTgtTimer_init();
#endif

//        #ifdef EHS_GUI_SUPPORT //@todo this shoulf be in the common areas an gtk replaced with gui?
//	Don't need this for gtk support: EhsToolkitTable_addTable(EhsBlockRefTable_gtk);
//       #endif
    /* start TCP/IP server thread */
    /*@todo we should have another thread starter for non component threads such as this */
    // - moved to Common code : EhsHThread_execute((EhsGeneralThreadFuncType)EhsSvcTcp_server,NULL,-90); // start with low priority for debugging portal
}

#define EHS_BUGGY_LINUX_NETWORKING_API

#ifdef EHS_BUGGY_LINUX_NETWORKING_API
union IPconverter
{
    struct
    {
        unsigned char a, b, c, d;
    } b;
    int i;
};
#endif


#ifdef EHS_ANDROID /* we need our own implementation of this for Android as the NDK misses it out */
#include "target_net.h"
#define EHS_USE_WIFI_INTERFACE
#endif


/*
 * @todo needs extending to add interface to be interogated
 */
EHS_GLOBAL void EhsTOS_GetMACandIPaddr(ehs_char * buf,ehs_char * bufIP)
{
#ifdef EHS_BUGGY_LINUX_NETWORKING_API
    union IPconverter IPaddr;
#else
    struct in_addr addr;
#endif
    struct ifreq buffer;
    ehs_char * ipbuf;

    struct ifaddrs * ifAddrStruct;
    struct ifaddrs * ifa;
    void * tmpAddrPtr;
    char addressBuffer[INET_ADDRSTRLEN];
    char addressBuffer6[INET6_ADDRSTRLEN];

    memset(&buffer, 0x00, sizeof(buffer));
#ifdef EHS_USE_WIFI_INTERFACE
    strcpy(buffer.ifr_name, "wlan0");
#else
    strcpy(buffer.ifr_name, "enp2s0"); //@todo the interface name should be a parameter
#endif
#ifdef EHS_ANDROID
    // @TODO - check other network interface for android
    strcpy(buffer.ifr_name, "eth0");
#else //EHS_ANDROID
    int s = socket(PF_INET, SOCK_DGRAM, 0);
    EHSH_LOG_INFO("IFR NAME = %s",buffer.ifr_name);
    if (ioctl(s, SIOCGIFHWADDR, &buffer) != -1 )
    {
        // don't duplucate this strning munching as it'sthe same for each case
        EhsSprintf(buf, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X", (unsigned char)buffer.ifr_hwaddr.sa_data[0],(unsigned char)buffer.ifr_hwaddr.sa_data[1],(unsigned char)buffer.ifr_hwaddr.sa_data[2],(unsigned char)buffer.ifr_hwaddr.sa_data[3],(unsigned char)buffer.ifr_hwaddr.sa_data[4],(unsigned char)buffer.ifr_hwaddr.sa_data[5]);
    }
    else
    {
        strcpy(buffer.ifr_name, "eth0"); //@todo hacked for 64 bit debian 9.4
        if (ioctl(s, SIOCGIFHWADDR, &buffer) != -1 )
        {
            EhsSprintf(buf, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X", (unsigned char)buffer.ifr_hwaddr.sa_data[0],(unsigned char)buffer.ifr_hwaddr.sa_data[1],(unsigned char)buffer.ifr_hwaddr.sa_data[2],(unsigned char)buffer.ifr_hwaddr.sa_data[3],(unsigned char)buffer.ifr_hwaddr.sa_data[4],(unsigned char)buffer.ifr_hwaddr.sa_data[5]);
        }
        else
        {
            strcpy(buffer.ifr_name, "enp4s0"); //@todo hacked for 64 bit debian 9.4
            if (ioctl(s, SIOCGIFHWADDR, &buffer) != -1 )
            {
                EhsSprintf(buf, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X", (unsigned char)buffer.ifr_hwaddr.sa_data[0],(unsigned char)buffer.ifr_hwaddr.sa_data[1],(unsigned char)buffer.ifr_hwaddr.sa_data[2],(unsigned char)buffer.ifr_hwaddr.sa_data[3],(unsigned char)buffer.ifr_hwaddr.sa_data[4],(unsigned char)buffer.ifr_hwaddr.sa_data[5]);
            }
            else
            {
                strcpy(buffer.ifr_name, "enp3s0"); //@todo hacked for 64 bit debian 9.4
                if (ioctl(s, SIOCGIFHWADDR, &buffer) != -1 )
                {
                    EhsSprintf(buf, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X", (unsigned char)buffer.ifr_hwaddr.sa_data[0],(unsigned char)buffer.ifr_hwaddr.sa_data[1],(unsigned char)buffer.ifr_hwaddr.sa_data[2],(unsigned char)buffer.ifr_hwaddr.sa_data[3],(unsigned char)buffer.ifr_hwaddr.sa_data[4],(unsigned char)buffer.ifr_hwaddr.sa_data[5]);
                }
                else
                {
                    strcpy(buffer.ifr_name, "eno1"); //@todo hacked for 64 bit debian 9.4 NUC7s
                    if (ioctl(s, SIOCGIFHWADDR, &buffer) != -1 )
                    {
                        EhsSprintf(buf, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X", (unsigned char)buffer.ifr_hwaddr.sa_data[0],(unsigned char)buffer.ifr_hwaddr.sa_data[1],(unsigned char)buffer.ifr_hwaddr.sa_data[2],(unsigned char)buffer.ifr_hwaddr.sa_data[3],(unsigned char)buffer.ifr_hwaddr.sa_data[4],(unsigned char)buffer.ifr_hwaddr.sa_data[5]);
                    }
                    else
                    {
                        strcpy(buffer.ifr_name, "eno0"); //@todo hacked for 64 bit debian 9.4 NUC7s
                        if (ioctl(s, SIOCGIFHWADDR, &buffer) != -1 )
                        {
                            EhsSprintf(buf, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X", (unsigned char)buffer.ifr_hwaddr.sa_data[0],(unsigned char)buffer.ifr_hwaddr.sa_data[1],(unsigned char)buffer.ifr_hwaddr.sa_data[2],(unsigned char)buffer.ifr_hwaddr.sa_data[3],(unsigned char)buffer.ifr_hwaddr.sa_data[4],(unsigned char)buffer.ifr_hwaddr.sa_data[5]);
                        }
                        else
                        {
                            strcpy(buffer.ifr_name, "enp1s0"); //@todo hacked for 64 bit debian 9.4
                            if (ioctl(s, SIOCGIFHWADDR, &buffer) != -1 )
                            {
                                EhsSprintf(buf, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X", (unsigned char)buffer.ifr_hwaddr.sa_data[0],(unsigned char)buffer.ifr_hwaddr.sa_data[1],(unsigned char)buffer.ifr_hwaddr.sa_data[2],(unsigned char)buffer.ifr_hwaddr.sa_data[3],(unsigned char)buffer.ifr_hwaddr.sa_data[4],(unsigned char)buffer.ifr_hwaddr.sa_data[5]);
                            }
                            else
                            {
                                strcpy(buffer.ifr_name, "enp0s0"); //@todo hacked for 64 bit debian 9.4
                                if (ioctl(s, SIOCGIFHWADDR, &buffer) != -1 )
                                {
                                    EhsSprintf(buf, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X", (unsigned char)buffer.ifr_hwaddr.sa_data[0],(unsigned char)buffer.ifr_hwaddr.sa_data[1],(unsigned char)buffer.ifr_hwaddr.sa_data[2],(unsigned char)buffer.ifr_hwaddr.sa_data[3],(unsigned char)buffer.ifr_hwaddr.sa_data[4],(unsigned char)buffer.ifr_hwaddr.sa_data[5]);
                                }
                                else
                                {
                                    EhsStrcpy(buf,"n/a");
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    close(s);
    //EHSH_LOG_INFO("XXXXXXXXXXXXX MAC ID = %s",buf);
#endif //EHS_ANDROID

    /* and get the IP address */

#define USE_MOST_GENERIC
#if defined ( USE_MOST_GENERIC )

    EhsStrcpy(bufIP, "n/a-e"); // might not get it...

    if (getifaddrs(&ifAddrStruct) != -1)
    {
        for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
        {
            addressBuffer[0]='\0';/* avoid unititialised strcpy */
            addressBuffer6[0]='\0';
            //if (ifa->ifa_addr == NULL) continue; // do we want to skip unconnected ethernet ports? still would like MAC address
            if (EhsStrcmp(ifa->ifa_name, buffer.ifr_name) == 0)   /*@todo "eth0" should be made variable and target specific */
            {
                if (ifa ->ifa_addr->sa_family == AF_INET)   // check it is IP4
                {
                    // is a valid IP4 Address
                    tmpAddrPtr = &((struct sockaddr_in *) ifa->ifa_addr)->sin_addr;

                    inet_ntop(AF_INET, tmpAddrPtr, addressBuffer,INET_ADDRSTRLEN);

                    EHSH_LOG_INFO("%s IPv4 Address %s\n", ifa->ifa_name, addressBuffer);
                    // EhsSprintf(bufIP,"%u.%u.%u.%u",IPaddr.b.c,IPaddr.b.d,IPaddr.b.a,IPaddr.b.b); //hack for buggy endian stuff in linux
                    if (EhsStrlen(addressBuffer) > 0 )
                        EhsStrcpy(bufIP, addressBuffer);
                }
                else if (ifa->ifa_addr->sa_family == AF_INET6)     // check it is IP6
                {
                    // is a valid IP6 Address
                    tmpAddrPtr
                        = &((struct sockaddr_in *) ifa->ifa_addr)->sin_addr;

                    //addressBuffer6[0]='\0';
                    inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer6,INET6_ADDRSTRLEN);
                    EHSH_LOG_INFO("%s IPv6 Address %s\n", ifa->ifa_name, addressBuffer6);
                    // EhsSprintf(bufIP,"%s",addressBuffer); //hack for buggy endian stuff in linux


                }

                /* Choose the IPV4 address as a preference*/
                if (EhsStrlen(addressBuffer) == 0 )
                {
                    if (EhsStrlen(addressBuffer6) > 0 )
                    {
                        EhsStrcpy(bufIP, addressBuffer6);
                        break;
                    }
                }
                else break; // if we found our preferred interface then break

            }

        }
        if (ifAddrStruct!=NULL)
            freeifaddrs(ifAddrStruct);
    }
    else
    {
        //do nothing we put in n/A as a default
    }

#else
    //Could use : ioctl(<socketfd>, SIOCGIFCONF, (struct ifconf)&buffer);
#ifdef EHS_BUGGY_LINUX_NETWORKING_API
    IPaddr.i=gethostid();
    EhsSprintf(bufIP,"%u.%u.%u.%u",IPaddr.b.c,IPaddr.b.d,IPaddr.b.a,IPaddr.b.b); //hack for buggy endian stuff in linux
#else
    addr.s_addr=gethostid();
    ipbuf = inet_ntoa (addr); // returns a static buffer.
    EhsStrcpy(bufIP,ipbuf); // copy to the target buffer
#endif
#endif
}

/* Some timeval functions - gnu suggested code - but obvious*/

int
timeval_subtract ( struct timeval *result,struct timeval *x,struct timeval *y)
{
    /* Perform the carry for the later subtraction by updating y. */
    if (x->tv_usec < y->tv_usec)
    {
        int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
        y->tv_usec -= 1000000 * nsec;
        y->tv_sec += nsec;
    }
    if (x->tv_usec - y->tv_usec > 1000000)
    {
        int nsec = (x->tv_usec - y->tv_usec) / 1000000;
        y->tv_usec += 1000000 * nsec;
        y->tv_sec -= nsec;
    }

    /* Compute the time remaining to wait.
       tv_usec is certainly positive. */
    result->tv_sec = x->tv_sec - y->tv_sec;
    result->tv_usec = x->tv_usec - y->tv_usec;

    /* Return 1 if result is negative. */
    return x->tv_sec < y->tv_sec;
}


/*
 * get RAM Usage
 */

typedef  struct timeval timeval_t;

ehs_bool get_cpu_ram_info(ehs_uint16 *cpu_usage_percent,ehs_uint32 * RAM_Size, ehs_uint32 * RAM_Used, ehs_uint32 * RAM_Free)
{
    /*	Manual Linux method
    	char buf[30];
           snprintf(buf, 30, "/proc/%u/statm", (unsigned)getpid());
           FILE* pf = fopen(buf, "r");
           if (pf) {
               unsigned size; //       total program size
               unsigned resident;//   resident set size
               unsigned share;//      shared pages
               unsigned text;//       text (code)
               unsigned lib;//        library
               unsigned data;//       data/stack
               unsigned dt;//         dirty pages (unused in Linux 2.6)
           }
           fclose(pf);
    */
    /*
     *   struct timeval ru_utime; // user time used
    struct timeval ru_stime; // system time used
    long   ru_maxrss;        // maximum resident set size
    long   ru_ixrss;         // integral shared memory size
    long   ru_idrss;         // integral unshared data size
    long   ru_isrss;         // integral unshared stack size
    long   ru_minflt;        // page reclaims
    long   ru_majflt;        // page faults
    long   ru_nswap;         // swaps
    long   ru_inblock;       // block input operations
    long   ru_oublock;       // block output operations
    long   ru_msgsnd;        // messages sent
    long   ru_msgrcv;        // messages received
    long   ru_nsignals;      // signals received
    long   ru_nvcsw;         // voluntary context switches
    long   ru_nivcsw;        // involuntary context switches
     *
     */
    struct sysinfo info;
    struct rusage rusage;
    ehs_bool ret = EHS_FALSE;
    static ehs_uint16 last_cpu_usage_percent=0; //remember the last one if we can't get a value.
    /*
    clock_t tick_diff_since_last = 0;
    clock_t temp_timeVal_ticks = 0;
    */

    static timeval_t Last_timeVal; // 0 at start of program is valid time interval
    static  timeval_t Last_userTimeUsed= {.tv_sec=0,.tv_usec=0}; //is this portable - is ansi c99?
    static  timeval_t Last_sysTimeUsed= {.tv_sec=0,.tv_usec=0};
    unsigned long long ms_elapsed=0;
    timeval_t diff_userTimeUsed= {.tv_sec=0,.tv_usec=0};
    timeval_t diff_sysTimeUsed= {.tv_sec=0,.tv_usec=0};
    timeval_t temp_timeVal_time;
    timeval_t diff_time= {.tv_sec=0,.tv_usec=0};
    /*
    temp_timeVal_ticks=clock (); // we'll use the time since the process started - OOPs this is CPU on process not time...
    tick_diff_since_last=temp_timeVal_ticks-Last_timeVal_ticks;
    Last_timeVal_ticks=temp_timeVal_ticks;
    */
    gettimeofday(&temp_timeVal_time, NULL);

    if (Last_timeVal.tv_sec && Last_timeVal.tv_usec)
    {
        timeval_subtract(&diff_time, &temp_timeVal_time,&Last_timeVal);
        ms_elapsed = diff_time.tv_sec*1000+diff_time.tv_usec/1000;
    }
    else
    {
        ms_elapsed=0;
        Last_timeVal.tv_sec = temp_timeVal_time.tv_sec;
        Last_timeVal.tv_usec = temp_timeVal_time.tv_usec;
    }

    if (ms_elapsed > 20)
    {
        Last_timeVal.tv_sec = temp_timeVal_time.tv_sec;
        Last_timeVal.tv_usec = temp_timeVal_time.tv_usec;
    }
    /*
     *Size=	(sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGESIZE))/1024;
     *Used= rusage->ru_idrss+rusage->ru_isrss;
     // *Free=sysconf(_SC_AVPHYS_PAGES); //@todo should remove the disk cache from this. );  should remove the disk cache from this.
     */

    if (getrusage(RUSAGE_SELF, &rusage) >= 0)
    {
        *RAM_Used = (ehs_uint32)(rusage.ru_maxrss);//ru_idrss+rusage->ru_isrss);
        //@todo  we are gnu here so we will just substract these numbers..
        timeval_subtract(&diff_userTimeUsed, &rusage.ru_utime,
                         &Last_userTimeUsed);
        timeval_subtract(&diff_sysTimeUsed, &rusage.ru_stime, &Last_sysTimeUsed);
        if (ms_elapsed > 20)
        {
            Last_userTimeUsed.tv_sec = rusage.ru_utime.tv_sec;
            Last_userTimeUsed.tv_usec = rusage.ru_utime.tv_usec;
            Last_sysTimeUsed.tv_sec = rusage.ru_stime.tv_sec;
            Last_sysTimeUsed.tv_usec = rusage.ru_stime.tv_usec;
            //}
            //do the calculation in ms to avoid overflow
            //if (ms_elapsed>20) { //avoid div 0.
            *cpu_usage_percent = last_cpu_usage_percent
                                 = (ehs_uint16)(((diff_userTimeUsed.tv_sec + diff_sysTimeUsed.tv_sec)
                                                 * 1000 + (diff_userTimeUsed.tv_usec
                                                         + diff_sysTimeUsed.tv_usec) / 10) / (ms_elapsed)); // 10 = 1000us/100%.
            //*cpu_usage_percent=*cpu_usage_percent*100;
        }
        else     // this is actually redundant - see out time check if..
        {
            /*EHSH_LOG_INFO("Too short CPU measure interval, returning last...\n");*/
            *cpu_usage_percent = last_cpu_usage_percent;
            //leave all the last values as they were for next time.
        }
    }
    else
    {
        EHSH_LOG_WARNING("Could not retrieve CPU and RAM info\n");
        *RAM_Used = (ehs_uint32) 0;
        *cpu_usage_percent = (ehs_uint16) 0;
    }
#ifndef EHS_ANDROID
#ifndef EFAULT
#define EFAULT -1
#endif
    if (sysinfo(&info) != EFAULT)
    {

        *RAM_Size = (ehs_uint32) ((info.totalram * info.mem_unit) / 1024);
        *RAM_Free = (ehs_uint32) (((info.freeram + info.freehigh
                                    + info.bufferram) * info.mem_unit) / 1024);
        //		+ info.bufferram) * info.mem_unit) / 1024)));
        //EHSH_LOG_WARNING("Got  RAM =%d Free High=%d\n",(ehs_uint32) ((info.totalram * info.mem_unit) / 1024),(ehs_uint32) (((info.freeram + info.freehigh + info.bufferram) * info.mem_unit) / 1024));
    }
    else
    {
#endif
        EHSH_LOG_WARNING("Could not retrieve CPU and RAM info\n");
        *RAM_Size = (ehs_uint32) 0;
        *RAM_Free = (ehs_uint32) 0;
#ifndef EHS_ANDROID
    }
#endif
    //+rusage->ru_nswap;
    ret = EHS_TRUE; //@todo tidy up with error trapping (currently not used).
    return ret;

}

/*
 * Get Disk statistics - doesn't work for mingw
 */

ehs_bool get_dir_stats(ehs_uint32 * Size, ehs_uint32 * Used, ehs_uint32 * Free,
                       ehs_char * path)
{
#ifndef EHS_ANDROID
    struct statvfs sbuf;
#else
    struct statfs sbuf;
#endif
    ehs_bool ret = EHS_TRUE;
#ifndef EHS_ANDROID
    if (statvfs(path, &sbuf) >= 0)
    {
#else
    if (statfs(path, &sbuf) >= 0)
    {
#endif

        *Free = ((unsigned long long) sbuf.f_bsize * (unsigned long long)sbuf.f_bavail) / 1024;
        *Size = ((unsigned long long)sbuf.f_frsize * (unsigned long long)sbuf.f_blocks) / 1024;
        *Used = *Size - *Free;
    }
    else     // some warning nunbers..
    {
        /*EHSH_LOG_WARNING*/printf("statvfs Failed for %s\n",path);
        *Free = 0;
        *Size = 0;
        *Used = 0;
        ret = EHS_FALSE;
    }
    return ret;
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

/* updated dynamic and static data
 * The "what" parameter can be used 0: get everything, 1 get just static 2 just dynamic
 *
 * @todo Currently we update everything anyway
 * */
ehs_bool EhsTOsSys_UpdateEnvironment(EhsMetaDataType * pEhsMetaData, ehs_uint8 what)
{
    ehs_char szTemp[EHS_STRING_LENGTH_MAX];
    ehs_uint32 tempint;

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

    //EhsStrcpy(pEhsMetaData->zDeviceIPAddr,"unknown");  // if we are networked get IP address here
    //EhsStrcpy(pEhsMetaData->zDeviceID,"none"); //@todo here
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
