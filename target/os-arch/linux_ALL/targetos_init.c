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
//#include "targetos_init_specific.h" - we don't need this after all
#define EHS_TARGET_CODE /* Ensure header files include target-internal values */

//#define EHSL_MODULE_ID EHSH_LOG_MODULE_UNDEFINED

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
#include "hal_target_sys_stat.h"

#ifdef EHS_PERIPHERALS_GPIO_SUPPORT
#include "inx_gpio.h"
#endif


/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

#ifndef TASK_COMM_LEN
#define TASK_COMM_LEN 16
// not found in linux/sched.h usually
#endif

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
#ifdef EHS_MINGW
    EhsTgtTimer_init();
#endif

#ifdef EHS_PERIPHERALS_GPIO_SUPPORT
    EhsTOsInitGpio();
#endif

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
#endif

/* function used for searching IP address */
EHS_LOCAL ehs_bool EhsTOS_FindIp(const struct ifaddrs* ifa, const char* interface_name, ehs_char * bufIP)
{
    char addressBuffer[INET_ADDRSTRLEN];
    char addressBuffer6[INET6_ADDRSTRLEN];
    void * tmpAddrPtr;
    struct ifaddrs * ifAddrStruct;
    ehs_bool bSuccess = EHS_FALSE;

    if (ifa != NULL && interface_name != NULL && getifaddrs(&ifAddrStruct) != -1)
    {
        for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
        {
            addressBuffer[0]='\0';/* avoid unititialised strcpy */
            addressBuffer6[0]='\0';
            //if (ifa->ifa_addr == NULL) continue; // do we want to skip unconnected ethernet ports? still would like MAC address
            if (EhsStrcmp(ifa->ifa_name, interface_name) == 0)   /* Have we found the IP address we are using for the MAC address? */
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
                    tmpAddrPtr = &((struct sockaddr_in *) ifa->ifa_addr)->sin_addr;

                    //addressBuffer6[0]='\0';
                    inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer6,INET6_ADDRSTRLEN);
                    EHSH_LOG_INFO("%s IPv6 Address %s\n", ifa->ifa_name, addressBuffer6);
                    // EhsSprintf(bufIP,"%s",addressBuffer); /* todo IPV6 support */
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
        if (ifAddrStruct!=NULL){
            freeifaddrs(ifAddrStruct);
        }
    }
    else
    {
        //do nothing we put in n/A as a default
    }

    return bSuccess;
}


/*
 * @todo needs extending to add interface to be interogated
 */
EHS_GLOBAL void EhsTOS_GetMACandIPaddr(ehs_char * buf, ehs_char * bufIP)
{
#ifdef EHS_BUGGY_LINUX_NETWORKING_API
    union IPconverter IPaddr;
#else
    struct in_addr addr;
#endif
    struct ifreq buffer;
    ehs_char * ipbuf;
    struct ifaddrs * ifa;

    memset(&buffer, 0x00, sizeof(buffer));
#if defined(EHS_USE_WIFI_INTERFACE)
    strcpy(buffer.ifr_name, "wlan0");
#elif defined(EHS_ANDROID)
    // I guess this is because most of our android boards are using ethernet
    strcpy(buffer.ifr_name, "eth0");
#else
    strcpy(buffer.ifr_name, "enp2s0"); //This is the default one we try first, then we iterate other options below
#endif
// @TODO - check other network interface for android ?
#ifndef EHS_ANDROID
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
                                    strcpy(buffer.ifr_name,"wlp0s20f3");
                                    if (ioctl(s, SIOCGIFHWADDR, &buffer) != -1 )
                                    {
                                        EhsSprintf(buf, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X", (unsigned char)buffer.ifr_hwaddr.sa_data[0],(unsigned char)buffer.ifr_hwaddr.sa_data[1],(unsigned char)buffer.ifr_hwaddr.sa_data[2],(unsigned char)buffer.ifr_hwaddr.sa_data[3],(unsigned char)buffer.ifr_hwaddr.sa_data[4],(unsigned char)buffer.ifr_hwaddr.sa_data[5]);
                                    }
                                    else
                                    {
                                        EhsStrcpy(buf,"na-xx-xx-xx-xx-xx");
                                    }
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

    EhsStrcpy(bufIP, "N/A"); // might not get it...

    // find ip address for interface from which mac address was extracted
    ehs_bool success = EhsTOS_FindIp(ifa, buffer.ifr_name, bufIP);

    if(success == EHS_FALSE){
        // find ip from any availble interface
        #ifdef EHS_USE_WIFI_INTERFACE // getting ip from wifi interface failed, try to get it from ethernet instead
        EhsTOS_FindIp(ifa, "eth0", bufIP);
        #else
        EhsTOS_FindIp(ifa, "wlan0", bufIP);
        #endif
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

int timeval_subtract ( struct timeval *result,struct timeval *x,struct timeval *y)
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
 * @brief Get CPU & RAM Usage of the eRT process
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
    static timeval_t Last_timeVal; // 0 at start of program is valid time interval
    static  timeval_t Last_userTimeUsed= {.tv_sec=0,.tv_usec=0}; //is this portable - is ansi c99?
    static  timeval_t Last_sysTimeUsed= {.tv_sec=0,.tv_usec=0};
    unsigned long long ms_elapsed=0;
    timeval_t diff_userTimeUsed= {.tv_sec=0,.tv_usec=0};
    timeval_t diff_sysTimeUsed= {.tv_sec=0,.tv_usec=0};
    timeval_t temp_timeVal_time;
    timeval_t diff_time= {.tv_sec=0,.tv_usec=0};
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

    if (ms_elapsed > 100)
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
        if (ms_elapsed > 100)
        {
            Last_userTimeUsed.tv_sec = rusage.ru_utime.tv_sec;
            Last_userTimeUsed.tv_usec = rusage.ru_utime.tv_usec;
            Last_sysTimeUsed.tv_sec = rusage.ru_stime.tv_sec;
            Last_sysTimeUsed.tv_usec = rusage.ru_stime.tv_usec;
            //}
            //do the calculation in ms to avoid overflow
            *cpu_usage_percent = last_cpu_usage_percent = (ehs_uint16)(
                                (
                                    ( diff_userTimeUsed.tv_sec  + diff_sysTimeUsed.tv_sec  ) * 100000 // 100000 = 1000ms*100(%) 
                                +   ( diff_userTimeUsed.tv_usec + diff_sysTimeUsed.tv_usec ) / 10
                                ) / (ms_elapsed)); // 10 = 1000us/100(%).
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
//#ifndef EHS_ANDROID
//#ifndef EFAULT
//#define EFAULT -1
//#endif
    if (sysinfo(&info) != EFAULT)
    {

        *RAM_Size = (ehs_uint32) ((info.totalram * info.mem_unit) / 1024);
        *RAM_Free = (ehs_uint32) (((info.freeram + info.freehigh
                                    + info.bufferram) * info.mem_unit) / 1024);
        //		+ info.bufferram) * info.mem_unit) / 1024)));
    }
    else
    {
//#endif
        EHSH_LOG_WARNING("Could not retrieve CPU and RAM info\n");
        *RAM_Size = (ehs_uint32) 0;
        *RAM_Free = (ehs_uint32) 0;
//#ifndef EHS_ANDROID
    }
//#endif
    //+rusage->ru_nswap;
    ret = EHS_TRUE; //@todo tidy up with error trapping (currently not used).
    return ret;
}

/* @brief get the (first found) process ID of a process name from /proc/xxx */
#define EHS_MAX_PROC_ENTRYS_TO_SEARCH 1000000
ehs_uint32  get_procid_from_procname(ehs_char * procname)
{
     ehs_uint32 i=1; // we don'#'t care about 0 as this is (init)
     ehs_uint32 ret=0;
     ehs_uint32 found=0;
     ehs_char proc_path[EHS_STRING_LENGTH_MAX];
     ehs_char proc_name_test[EHS_STRING_LENGTH_MAX];
     ehs_FILE * procfile;
     //printf("Checking for %s..\n",procname);
     do {
        //EhsSprintf(proc_path,"/proc/%d/cmdline",i); // this has args and spam in it.
        EhsSprintf(proc_path,"/proc/%d/comm",i); // this is usually limited to 16 bytes (or what ever TASK_COMM_LEN is set to )
        //printf("Checking %s\n",proc_path);
        if (procfile = EhsFopen(proc_path,"r")) {
            //printf("Opening Proc...%d\n",i);
            found++;
            EhsFgets(proc_name_test,EHS_STRING_LENGTH_MAX,procfile);
            EhsStrTrimR(proc_name_test); // /proc/*/cmdline adds a new line for us, so just in casse (though we are using comm instead)...
            //printf("[%s]=[%s]\n",procname,proc_name_test);
            //if (EhsStrcmp(procname,proc_name_test)==0) { //,EHS_STRING_LENGTH_MAX)==0) {
            if (EhsStrncmp(procname,proc_name_test,TASK_COMM_LEN-1)==0) {
            //if (EhsStrstr(procname,proc_name_test)!=NULL) {
                ret=i;
                //printf("!!!!!!! Found /proc/%d for %s\n",i,proc_name_test);
            }
            EhsFclose(procfile);
        }
        i++;
        //printf("Another...%d & %d \n");
     } while (ret == 0 && (found < EHS_MAX_PROC_ENTRYS_TO_SEARCH) && (i < EHS_MAX_PROC_ENTRYS_TO_SEARCH));
     return ret;
}

/* This gets the miscelaneous information for an arbitrary process given by process ID */
/* Linux only uses the following /proc/[[*]/stat info */
/*
/proc/[pid]/stat
          Status information about the process.  This is used by ps(1).
          It is defined in the kernel source file fs/proc/array.c.

          The fields, in order, with their proper scanf(3) format speci‐
          fiers, are listed below.  Whether or not certain of these
          fields display valid information is governed by a ptrace
          access mode PTRACE_MODE_READ_FSCREDS | PTRACE_MODE_NOAUDIT
          check (refer to ptrace(2)).  If the check denies access, then
          the field value is displayed as 0.  The affected fields are
          indicated with the marking [PT].

          (1) pid  %d
                    The process ID.

          (2) comm  %s
                    The filename of the executable, in parentheses.
                    This is visible whether or not the executable is
                    swapped out.

          (3) state  %c
                    One of the following characters, indicating process
                    state:

                    R  Running

                    S  Sleeping in an interruptible wait

                    D  Waiting in uninterruptible disk sleep

                    Z  Zombie

                    T  Stopped (on a signal) or (before Linux 2.6.33)
                       trace stopped

                    t  Tracing stop (Linux 2.6.33 onward)

                    W  Paging (only before Linux 2.6.0)

                    X  Dead (from Linux 2.6.0 onward)

                    x  Dead (Linux 2.6.33 to 3.13 only)

                    K  Wakekill (Linux 2.6.33 to 3.13 only)

                    W  Waking (Linux 2.6.33 to 3.13 only)

                    P  Parked (Linux 3.9 to 3.13 only)

                    I  Idle (Linux 4.14 onward)

          (4) ppid  %d
                    The PID of the parent of this process.

          (5) pgrp  %d
                    The process group ID of the process.

          (6) session  %d
                    The session ID of the process.

          (7) tty_nr  %d
                    The controlling terminal of the process.  (The minor
                    device number is contained in the combination of
                    bits 31 to 20 and 7 to 0; the major device number is
                    in bits 15 to 8.)

          (8) tpgid  %d
                    The ID of the foreground process group of the con‐
                    trolling terminal of the process.

          (9) flags  %u
                    The kernel flags word of the process.  For bit mean‐
                    ings, see the PF_* defines in the Linux kernel
                    source file include/linux/sched.h.  Details depend
                    on the kernel version.

                    The format for this field was %lu before Linux 2.6.

          (10) minflt  %lu
                    The number of minor faults the process has made
                    which have not required loading a memory page from
                    disk.

          (11) cminflt  %lu
                    The number of minor faults that the process's
                    waited-for children have made.

          (12) majflt  %lu
                    The number of major faults the process has made
                    which have required loading a memory page from disk.

          (13) cmajflt  %lu
                    The number of major faults that the process's
                    waited-for children have made.

          (14) utime  %lu
                    Amount of time that this process has been scheduled
                    in user mode, measured in clock ticks (divide by
                    sysconf(_SC_CLK_TCK)).  This includes guest time,
                    guest_time (time spent running a virtual CPU, see
                    below), so that applications that are not aware of
                    the guest time field do not lose that time from
                    their calculations.

          (15) stime  %lu
                    Amount of time that this process has been scheduled
                    in kernel mode, measured in clock ticks (divide by
                    sysconf(_SC_CLK_TCK)).

          (16) cutime  %ld
                    Amount of time that this process's waited-for chil‐
                    dren have been scheduled in user mode, measured in
                    clock ticks (divide by sysconf(_SC_CLK_TCK)).  (See
                    also times(2).)  This includes guest time,
                    cguest_time (time spent running a virtual CPU, see
                    below).

          (17) cstime  %ld
                    Amount of time that this process's waited-for chil‐
                    dren have been scheduled in kernel mode, measured in
                    clock ticks (divide by sysconf(_SC_CLK_TCK)).

          (18) priority  %ld
                    (Explanation for Linux 2.6) For processes running a
                    real-time scheduling policy (policy below; see
                    sched_setscheduler(2)), this is the negated schedul‐
                    ing priority, minus one; that is, a number in the
                    range -2 to -100, corresponding to real-time priori‐
                    ties 1 to 99.  For processes running under a non-
                    real-time scheduling policy, this is the raw nice
                    value (setpriority(2)) as represented in the kernel.
                    The kernel stores nice values as numbers in the
                    range 0 (high) to 39 (low), corresponding to the
                    user-visible nice range of -20 to 19.

                    Before Linux 2.6, this was a scaled value based on
                    the scheduler weighting given to this process.

          (19) nice  %ld
                    The nice value (see setpriority(2)), a value in the
                    range 19 (low priority) to -20 (high priority).

          (20) num_threads  %ld
                    Number of threads in this process (since Linux 2.6).
                    Before kernel 2.6, this field was hard coded to 0 as
                    a placeholder for an earlier removed field.

          (21) itrealvalue  %ld
                    The time in jiffies before the next SIGALRM is sent
                    to the process due to an interval timer.  Since ker‐
                    nel 2.6.17, this field is no longer maintained, and
                    is hard coded as 0.

          (22) starttime  %llu
                    The time the process started after system boot.  In
                    kernels before Linux 2.6, this value was expressed
                    in jiffies.  Since Linux 2.6, the value is expressed
                    in clock ticks (divide by sysconf(_SC_CLK_TCK)).

                    The format for this field was %lu before Linux 2.6.

          (23) vsize  %lu
                    Virtual memory size in bytes.

          (24) rss  %ld
                    Resident Set Size: number of pages the process has
                    in real memory.  This is just the pages which count
                    toward text, data, or stack space.  This does not
                    include pages which have not been demand-loaded in,
                    or which are swapped out.

          (25) rsslim  %lu
                    Current soft limit in bytes on the rss of the
                    process; see the description of RLIMIT_RSS in
                    getrlimit(2).

          (26) startcode  %lu  [PT]
                    The address above which program text can run.

          (27) endcode  %lu  [PT]
                    The address below which program text can run.

          (28) startstack  %lu  [PT]
                    The address of the start (i.e., bottom) of the
                    stack.

          (29) kstkesp  %lu  [PT]
                    The current value of ESP (stack pointer), as found
                    in the kernel stack page for the process.

          (30) kstkeip  %lu  [PT]
                    The current EIP (instruction pointer).

          (31) signal  %lu
                    The bitmap of pending signals, displayed as a deci‐
                    mal number.  Obsolete, because it does not provide
                    information on real-time signals; use
                    /proc/[pid]/status instead.

          (32) blocked  %lu
                    The bitmap of blocked signals, displayed as a deci‐
                    mal number.  Obsolete, because it does not provide
                    information on real-time signals; use
                    /proc/[pid]/status instead.

          (33) sigignore  %lu
                    The bitmap of ignored signals, displayed as a deci‐
                    mal number.  Obsolete, because it does not provide
                    information on real-time signals; use
                    /proc/[pid]/status instead.

          (34) sigcatch  %lu
                    The bitmap of caught signals, displayed as a decimal
                    number.  Obsolete, because it does not provide
                    information on real-time signals; use
                    /proc/[pid]/status instead.

          (35) wchan  %lu  [PT]
                    This is the "channel" in which the process is wait‐
                    ing.  It is the address of a location in the kernel
                    where the process is sleeping.  The corresponding
                    symbolic name can be found in /proc/[pid]/wchan.

          (36) nswap  %lu
                    Number of pages swapped (not maintained).

          (37) cnswap  %lu
                    Cumulative nswap for child processes (not main‐
                    tained).

          (38) exit_signal  %d  (since Linux 2.1.22)
                    Signal to be sent to parent when we die.

          (39) processor  %d  (since Linux 2.2.8)
                    CPU number last executed on.

          (40) rt_priority  %u  (since Linux 2.5.19)
                    Real-time scheduling priority, a number in the range
                    1 to 99 for processes scheduled under a real-time
                    policy, or 0, for non-real-time processes (see
                    sched_setscheduler(2)).

          (41) policy  %u  (since Linux 2.5.19)
                    Scheduling policy (see sched_setscheduler(2)).
                    Decode using the SCHED_* constants in linux/sched.h.

                    The format for this field was %lu before Linux
                    2.6.22.

          (42) delayacct_blkio_ticks  %llu  (since Linux 2.6.18)
                    Aggregated block I/O delays, measured in clock ticks
                    (centiseconds).

          (43) guest_time  %lu  (since Linux 2.6.24)
                    Guest time of the process (time spent running a vir‐
                    tual CPU for a guest operating system), measured in
                    clock ticks (divide by sysconf(_SC_CLK_TCK)).

          (44) cguest_time  %ld  (since Linux 2.6.24)
                    Guest time of the process's children, measured in
                    clock ticks (divide by sysconf(_SC_CLK_TCK)).

          (45) start_data  %lu  (since Linux 3.3)  [PT]
                    Address above which program initialized and unini‐
                    tialized (BSS) data are placed.

          (46) end_data  %lu  (since Linux 3.3)  [PT]
                    Address below which program initialized and unini‐
                    tialized (BSS) data are placed.

          (47) start_brk  %lu  (since Linux 3.3)  [PT]
                    Address above which program heap can be expanded
                    with brk(2).

          (48) arg_start  %lu  (since Linux 3.5)  [PT]
                    Address above which program command-line arguments
                    (argv) are placed.

          (49) arg_end  %lu  (since Linux 3.5)  [PT]
                    Address below program command-line arguments (argv)
                    are placed.

          (50) env_start  %lu  (since Linux 3.5)  [PT]
                    Address above which program environment is placed.

          (51) env_end  %lu  (since Linux 3.5)  [PT]
                    Address below which program environment is placed.

          (52) exit_code  %d  (since Linux 3.5)  [PT]
                    The thread's exit status in the form reported by
                    waitpid(2).
*/
ehs_bool get_cpu_ram_info_misc(ehs_uint16 *cpu_usage_percent, ehs_uint32 * RAM_Used, ehs_uint32 procid)
{
    ehs_char proc_path[EHS_STRING_LENGTH_MAX];
    ehs_bool ret = EHS_FALSE;
    ehs_bool scanOK;
    static ehs_uint16 last_cpu_usage_percent=0; //remember the last one if we can't get a value.
    static timeval_t Last_timeVal; // 0 at start of program is valid time interval
    static  ehs_uint64 Last_userTimeUsed=0; 
    static  ehs_uint64 Last_sysTimeUsed=0;
    ehs_uint64 cpu_usage = 0; // in ticks
    ehs_uint64 ram_usage = 0; // in bytes?
    ehs_uint64 ms_elapsed=0; /* we might want to change this to us to ms*/
    timeval_t diff_userTimeUsed= {.tv_sec=0,.tv_usec=0};
    timeval_t diff_sysTimeUsed= {.tv_sec=0,.tv_usec=0};
    timeval_t temp_timeVal_time;
    timeval_t diff_time= {.tv_sec=0,.tv_usec=0};
    if (procid) {
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

        if (ms_elapsed > 100)
        {
            Last_timeVal.tv_sec = temp_timeVal_time.tv_sec;
            Last_timeVal.tv_usec = temp_timeVal_time.tv_usec;
        }
        //printf ("procid = %d\n",procid);
        EhsSprintf(proc_path,"/proc/%d/stat",procid);
        ehs_FILE* procfile = EhsFopen(proc_path,"r");
        if (procfile) {
            ehs_uint64 New_userTimeUsed;// = Last_userTimeUsed;
            ehs_uint64 New_sysTimeUsed;// = Last_sysTimeUsed;
#ifndef EHS_ANDROID
// Debian seems to fit this format (not the above)
            scanOK = EhsFscanf(procfile,"%*d %*s %*c %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %*lu %lu %lu %*ld %*ld %*ld %*ld %*ld %*ld %*llu %*lu %lu"
                ,&New_userTimeUsed,&New_sysTimeUsed,&ram_usage);
#else 
// as specification above
            scanOK = EhsFscanf(procfile,"%*d %*s %*c %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %*lu %lu %lu %*ld %*ld %*ld %*ld %*llu %*lu %lu"
                ,&New_userTimeUsed,&New_sysTimeUsed,&ram_usage);
#endif

            EhsFclose(procfile);
            printf("scanOK=%d=%d(EHS_EOF), CPU-U=%d, CPU-S=%d, RAM = %d:",
                scanOK,EHS_EOF,Last_userTimeUsed,Last_sysTimeUsed,ram_usage);
            
            if (scanOK > 0) ret =EHS_TRUE;    
            if (New_userTimeUsed > 0 || New_sysTimeUsed > 0 ) {
                cpu_usage = ((New_userTimeUsed - Last_userTimeUsed) + (New_sysTimeUsed - Last_sysTimeUsed )); // ms of CPU usage since last
            }
            EHSH_LOG_ERROR("[cpu time in ticks = %d (tick=%u)]",cpu_usage,sysconf(_SC_CLK_TCK));
            if (ms_elapsed > 100)
            {
                *cpu_usage_percent = 
                last_cpu_usage_percent = 
                    (ehs_uint16)(((cpu_usage)*10000/sysconf(_SC_CLK_TCK)) )/ (ms_elapsed/10);
            }
            else ret = EHS_FALSE;

            Last_sysTimeUsed= New_sysTimeUsed;
            Last_userTimeUsed= New_userTimeUsed;
            /* Calulcate RAM from arg X*/
            *RAM_Used = (ehs_uint32)((ram_usage*sysconf(_SC_PAGESIZE))/1024); // return value in kB.
        }
    }
    if (ret == EHS_FALSE) {
        EHSH_LOG_WARNING("Could not retrieve CPU and RAM info\n");
        *RAM_Used = (ehs_uint32) 0;
        *cpu_usage_percent = (ehs_uint16) 0;
    }
    return ret;
}



/*
 * Get Disk statistics - doesn't work for mingw
 */

ehs_bool get_dir_stats(ehs_uint32 * Size, ehs_uint32 * Used, ehs_uint32 * Free,
                       /* todo make const */ ehs_char * path)
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
        //EHSH_LOG_WARNING(statvfs Failed for %s\n",path);
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
    if(what == EHS_OS_ENV_NETWORK_ID){
        // Update ip address
        EhsTOS_GetMACandIPaddr(pEhsMetaData->zDeviceID,pEhsMetaData->zDeviceIPAddr);
        // @TODO - Update network meta data using traget api, instead of using dummy values
        pEhsMetaData->nDeviceNetworkMode = EHS_NET_DHCP_MODE_ID;
        EhsStrcpy(pEhsMetaData->zDeviceGateway, "0.0.0.0");
        EhsStrcpy(pEhsMetaData->zDeviceMask, "0.0.0.0");
        EhsStrcpy(pEhsMetaData->zDeviceDNS1, "0.0.0.0");
        return EHS_TRUE; // return here, we only update network specific os env
    }

    ehs_uint32 tempint;

    if (what < 2) {
        #ifndef INX_SODL_IN_FLASH 
        //ehs_char szTemp[EHS_STRING_LENGTH_MAX]; //todo2024 why do we use a buffer here and not just use pEhsMetaData->zUserDirectory?
        if (EhsStrlen(pEhsMetaData->zUserDirectory))
        {
            // get disk space in user directory
            //EhsStrcpy(szTemp,pEhsMetaData->zUserDirectory);
            get_dir_stats(&pEhsMetaData->nUserSpaceTotal_KB,&pEhsMetaData->nUserSpaceUsed_KB,&tempint,&pEhsMetaData->zUserDirectory);
        }
        else
        {
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
    
    EhsTOS_GetMACandIPaddr(pEhsMetaData->zDeviceID,pEhsMetaData->zDeviceIPAddr);
       
    get_cpu_ram_info(&(pEhsMetaData->CPUUsage), &(pEhsMetaData->RAMTotal_KB),&(pEhsMetaData->RAMUsed_KB),&(pEhsMetaData->RAMAvail_KB));
    get_cpu_ram_info_misc(&(pEhsMetaData->MiscAppCPUUsage),&(pEhsMetaData->MiscAppRAMUsed_KB),pEhsMetaData->MiscAppProcId);
    pEhsMetaData->CPUTemp = EhsTGetCpuTemp();
    /* Get the Linux Distr version */
    getOSVersion(pEhsMetaData->OSVersion);
    return EHS_FALSE;
}

/**
 * Called to shutdown the target prior to shutting down EHS
 */
void EhsTOsSys_term(void)
{
    // Leave the mutexes to the OS EhsTPMutex_term();
    #ifdef EHS_PERIPHERALS_GPIO_SUPPORT
    EhsTOsTermGpio();
    #endif
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
