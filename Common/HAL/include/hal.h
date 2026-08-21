/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file hal.h
 * Declares the interface for the general hal functions.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_HAL_H
#define EHS_HAL_H

#include "globals.h"
/* \todo we need to overcome what we should iunclude for globals. ehs_types.h - we are getting recursion */

#include "hal_process.h"
#include "hal_time.h"
#include "hal_bootstate.h"
// We don't include this because it has a dependency on hal.h
// #include "hal_file.h"
/*****************************************************************************/
/* Define macros  */

#ifndef EHS_IP_ADDR_LENGTH_MAX
#define EHS_IP_ADDR_LENGTH_MAX EHS_STRING_LENGTH_MAX
#endif

#ifndef EHS_DEVICE_ID_LENGTH_MAX
#define EHS_DEVICE_ID_LENGTH_MAX EHS_STRING_LENGTH_MAX
#endif

#ifndef EHS_NET_MAC_ID_LENGTH_MAX
#define EHS_NET_MAC_ID_LENGTH_MAX EHS_STRING_LENGTH_MAX
#endif

#ifndef EHS_INST_ROOT_DIR_LENGTH_MAX
#define EHS_INST_ROOT_DIR_LENGTH_MAX EHS_STRING_LENGTH_MAX
#endif

#ifndef EHS_APPS_DIR_LENGTH_MAX
#define EHS_APPS_DIR_LENGTH_MAX EHS_STRING_LENGTH_MAX
#endif

#ifndef EHS_USER_DIR_LENGTH_MAX
#define EHS_USER_DIR_LENGTH_MAX EHS_STRING_LENGTH_MAX
#endif

#ifndef EHS_VERSION_LENGTH_MAX
#define EHS_VERSION_LENGTH_MAX EHS_STRING_LENGTH_MAX
#endif

#ifndef EHS_BUILD_DATE_LENGTH_MAX
#define EHS_BUILD_DATE_LENGTH_MAX EHS_STRING_LENGTH_MAX
#endif

#ifndef EHS_START_DATE_LENGTH_MAX
#define EHS_START_DATE_LENGTH_MAX EHS_STRING_LENGTH_MAX
#endif

#ifndef EHS_TARGET_VARIANT_LENGTH_MAX
#define EHS_TARGET_VARIANT_LENGTH_MAX EHS_STRING_LENGTH_MAX
#endif

#ifndef EHS_MODULE_LIST_LENGTH_MAX
#define EHS_MODULE_LIST_LENGTH_MAX EHS_STRING_LENGTH_MAX
#endif

/* Platform-agnostic reset-reason enum. Targets map their native reset cause onto one
 * of these values via EhsHMetaSetLastResetReason so code that cares about the reason
 * (logging, boot-time diagnostics, fallback decisions) can branch on the enum rather
 * than parse strings. Use EhsHResetReasonToString for display. */
typedef enum {
    EHS_RESET_REASON_UNKNOWN = 0,
    EHS_RESET_REASON_POWER_ON,          /* cold boot / power cycle */
    EHS_RESET_REASON_SOFTWARE,          /* deliberate restart from code (e.g. OTA) */
    EHS_RESET_REASON_EXTERNAL,          /* external reset pin / debugger */
    EHS_RESET_REASON_PANIC,             /* CPU exception / panic handler */
    EHS_RESET_REASON_INT_WATCHDOG,      /* interrupt watchdog */
    EHS_RESET_REASON_TASK_WATCHDOG,     /* task watchdog */
    EHS_RESET_REASON_OTHER_WATCHDOG,    /* any other hardware watchdog */
    EHS_RESET_REASON_BROWNOUT,          /* supply voltage dip */
    EHS_RESET_REASON_DEEP_SLEEP_WAKE,   /* resumed from deep sleep */
    EHS_RESET_REASON_SDIO,
    EHS_RESET_REASON_USB,
    EHS_RESET_REASON_JTAG,
    EHS_RESET_REASON_MAX
} EhsResetReasonType;

/* App status id */
#define EHS_APP_LOAD_FAILED      0x00
#define EHS_APP_LOAD_SUCCESFULL  0x01
#define EHS_APP_LOAD_STARTED     0x02
#define EHS_APP_LOAD_RESTARTING  0x04

/* OS Enviroment Update id */
#define EHS_OS_ENV_STATIC_ID     1
#define EHS_OS_ENV_DYNAMIC_ID    2
#define EHS_OS_ENV_NETWORK_ID    3

/* Network mode id */
#define EHS_NET_DHCP_MODE_ID     0
#define EHS_NET_STATIC_MODE_ID   1

/*****************************************************************************/
/* Define types */
typedef enum {
    EHSTHREADNAME_MCU_FAST_HP_THR = 0,
    EHSTHREADNAME_MCU_FAST_LP_THR,
    EHSTHREADNAME_EHS_THR,
    EHSTHREADNAME_MCU_SLOW_HP_THR,
    EHSTHREADNAME_MCU_SLOW_LP_THR,
    EHSTHREADNAME_EHS_CONSOLE_THR,
    EHSTHREADNAME_MAX
} ehs_threadname_t;

/*****************************************************************************/
/* Run state machine */
typedef enum ehs_startupmode_enum {EHSMETADATA_NODEBUGONSTARTS=0,EHSMETADATA_DEBUGONRESTART,EHSMETADATA_DEBUGONSTART} ehs_startupmode_t;

/* Target tree implemented */
ehs_bool EhsTPlatformReady(void (*target_loop_iteration)(void*),void * target_env_blob) ;

/*@todo this could HAL? - should be a new EHS object file.
//todo2024 this is quite a lot of RAM with all these strings! Do we want this to be more dynamic just-enough allocations or just smaller allocations?
//todo also consider #defing out some of these wanot apply to MCU type targets like the Devman pass through buffers etc. It shouldn't really be here any way is it's too dynamic... 
 * Also should be made private - but some target OS stuff needs changing to use setters getters */
typedef struct EhsMetaDataType
{
    /* The following are truly global - not per EHS */
#ifndef EHS_TARGET_NO_MAIN_ARGS
    ehs_char zArgv0[EHS_STRING_LENGTH_MAX]; // contains the calling coommand.
    ehs_char zArgv1[EHS_STRING_LENGTH_MAX]; // contains the calling coommand.
#endif
    ehs_bool bStartWithoutApp; // a flag used for starting the eRT without running SODL
    /* Network specifc data */ 
    ehs_sint16 nDeviceNetworkMode; // Static (0), DHCP (1)
    ehs_char zDeviceIPAddr[EHS_IP_ADDR_LENGTH_MAX]; // if we are networked get IP address here
    ehs_char zDeviceGateway[EHS_IP_ADDR_LENGTH_MAX]; // if we are networked gateway here
    ehs_char zDeviceMask[EHS_IP_ADDR_LENGTH_MAX]; // if we are networked mask address here
    ehs_char zDeviceDNS1[EHS_IP_ADDR_LENGTH_MAX]; // if we are networked dns1 address here

    ehs_char zDeviceID[EHS_DEVICE_ID_LENGTH_MAX]; // Use the Hardware ID (e.g. MAC address).
    ehs_char zDeviceNetMacId[EHS_NET_MAC_ID_LENGTH_MAX]; // Use the Hardware ID (e.g. MAC address).
    ehs_char zEhsStartedDirectory[EHS_STRING_LENGTH_MAX]; // contains the cwd when EHS was first started


    /* The following are for a specific instance of EHS */
    time_t DynamicUpdateTime; //Time stamp of the last Dynamic update to the structure
    ehs_bool bStaticUpdate; //flag to identify static elements are valid
    ehs_uint32 nRepoID;
    ehs_char zVersion[EHS_VERSION_LENGTH_MAX];
    ehs_char zBuildDate[EHS_BUILD_DATE_LENGTH_MAX];
    ehs_char zEHSStartDate[EHS_START_DATE_LENGTH_MAX];
    ehs_char zTargetVariant[EHS_TARGET_VARIANT_LENGTH_MAX];
    ehs_char zModuleList[EHS_MODULE_LIST_LENGTH_MAX]; //@todo this may need to be made larger?

    /* The remainder is environment information - but with potentially EHS instance specific information*/
    ehs_startupmode_t DebugOnStart;
#ifndef INX_SODL_IN_FLASH
#ifndef EHS_EXCLUDE_OS_VERSION
    ehs_char OSVersion[EHS_STRING_LENGTH_MAX]; // This is the linux distro version of the host ! NOT USED !
#endif
    ehs_char zInstallRootDirectory[EHS_INST_ROOT_DIR_LENGTH_MAX]; // Path to ehs/.
    ehs_char zAppsDirectory[EHS_APPS_DIR_LENGTH_MAX]; // Root Path to App directory as we sometimes want this in it's own partition.
    ehs_char AppCurrentLive[EHS_STRING_LENGTH_MAX]; //Canonical Application Name of current Live App.
    ehs_char NextAppToRun[EHS_STRING_LENGTH_MAX]; //Canonical Application Name of next App to run. //@todo - ensure initialised as an empty string
    // #endif possibly we would need the following if SODL is flash and we still have a file system.
    ehs_char zUserDirectory[EHS_USER_DIR_LENGTH_MAX]; // Root Path to user directory, may be in <user home>/userdata or if root user <install dir>/userdata.
#endif
    ehs_uint32 PairedOrganisationID; /*  This is info provided by devman that indicates what organisation ID a device is paired with - this is enumerated for security.... */
    ehs_uint8 PairedOrganisationIDRequested; /* 0: no pairing data updates, 1 pairing data valid, 2 pairing data pending, 3 paring data invalid*/

    ehs_uint32 RAMUsed_KB;
    ehs_uint32 RAMTotal_KB;
    ehs_uint32 RAMAvail_KB;
    ehs_uint32 nUserSpaceUsed_KB;
    ehs_uint32 nUserSpaceTotal_KB;
    //ehs_uint32 nUserSpaceAvail_KB;
    ehs_uint32 nSysSpaceUsed_KB;
    ehs_uint32 nSysSpaceTotal_KB;
    //ehs_uint32 nUserSpaceAvail_KB;
    ehs_uint16 CPUUsage; /* % CPU usage by EHS */
    ehs_sint16 CPUTemp; /* CPU temperature in Celcius */
    /* If we are montitoring some other app then this is its info */
    ehs_uint32 MiscAppProcId; // This isthe proc ID for the app to monitor - set to 0 if we are not monitoring
    ehs_uint32 MiscAppRAMUsed_KB;
    ehs_uint32 MiscAppCPUUsage;
    /* Thread Loop Time */
    ehs_uint32 ThreadLoopTimeMin[EHSTHREADNAME_MAX];
    ehs_uint32 ThreadLoopTimeMax[EHSTHREADNAME_MAX];
    ehs_uint32 ThreadLoopTimeAvg[EHSTHREADNAME_MAX];

    ehs_bool NewDevmanMiscDLData;
    ehs_bool NewDevmanMiscULData; // probably don't need this because we send everything always
    ehs_bool devmanPingFail ;
    time_t devmanLastGoodPing;
    EhsTPConditionClass condDevmanNewMiscDLData; // opaque pointer to condition mutext - some mutexes we've decided to store here rather than with the global ones. No idea why...
    EhsTPMutexClass mutexDevmanNewMiscDLData; // opawue pointer to an ordnary mutex
    Ehs_ConsoleCommand_Type InternallyRequestedCommand;
#ifdef EHS_DEVMAN_SUPPORT
    ehs_char MiscAppProcName[EHS_STRING_LENGTH_MAX];
    ehs_char zDevmanMiscDLDataType[EHS_STRING_LENGTH_MAX];
    ehs_char zDevmanMiscDLData[EHS_STRING_LENGTH_MAX];
    ehs_char zDevmanNewMiscDLData[EHS_STRING_LENGTH_MAX];
    ehs_char zDevmanMiscULData[EHS_STRING_LENGTH_MAX];
#endif
    ehs_char zSysInfo[EHS_STRING_LENGTH_MAX];
    /* Reset reason captured at boot by the target layer. Stored as a platform-agnostic
     * enum so the kernel can classify it (normal vs abnormal) and Lucid can display
     * a human-readable string via EhsHResetReasonToString. */
    EhsResetReasonType eLastResetReason;
    /* EHS state machine */
} EhsMetaDataType;

/***************************
 *
 * EHS code version information struct
 *
 *****************************/


// Only access this through functions extern EhsMetaDataType EhsMetaData; @todo above struct can be moved below deck

/*****************************************************************************/
/* Declare global variables */

// Don't extern - we'll pass pointers /*extern*/ EhsMetaDataType EhsMetaData;

/*****************************************************************************/
/* Declare function prototypes  */

/* @todo move all these functions to function specific hal.h and C files .. ha'h should hust be a convenience include for HAL*/

/**
 * Sets a pointer to a function which gets called after the app has attempted to load.
 * An integer passed to a function represents app status IDs defined in hal.h
 */
void EhsHSetAppLoadStatusCallback(void (*callback)(ehs_uint32));

/**
 * Notifies about app loading status by passing app status IDs defined in hal.h
 */
void EhsHAppLoadStatusNotify(ehs_uint32 status);

/**
 * EHS & App Meta data getter setter Functions
 *
 * @todo these should be moved to a hal_sysinfo.h file
 */
void EhsHMetaUpdateStatic();
void EhsHMetaUpdateDynamic();
void EhsHMetaUpdateNetwork();
const ehs_char* EhsHMetaGetToolboxHashes();
const ehs_char* EhsHMetaGetInstPath(); /* Called to return the installation directory of EHS. */
const ehs_char* EhsHMetaGetUserPath(); /* Called to return the user directory. */
const ehs_char* EhsHMetaGetAppsPath(); /* Called to return a nonstandard Apps root path, return NULL if there isn't one set*/
void EhsHMetaSetAppsPath(ehs_char* path); /* Set a non standard Apps path.*/

/*
const ehs_char* EhsHMetaAppLiveDefaultDir();
const ehs_char* EhsHSetMetaAppLiveDefaultDir(ehs_uint8 which);
*/
const ehs_char* EhsHMetaGetHWID(); /* Return the HW-based device ID */
ehs_sint16 EhsHMetaGetNetworkMode(); /* Return the HW-based network mode id e.g. DHCP or Static */
const ehs_char* EhsHMetaGetIPAddr(); /* Return the HW-based IP address */
const ehs_char* EhsHMetaGetGateway(); /* Return the HW-based gateway */
const ehs_char* EhsHMetaGetMask(); /* Return the HW-based mask */
const ehs_char* EhsHMetaGetDNS1(); /* Return the HW-based DNS1 */
void EhsHMetaSetHWID(const char * value); /* Set the HW-based device ID */
void EhsHMetaSetIPAddr(const char * value); /* Set the HW-based ip address */
void EhsHMetaSetInstPath(const char * value); /* Set the HW-based device ID */
const ehs_char* EhsHMetaGetEHSVersion(); /* Return the HW-based device ID */
const ehs_uint32 EhsHMetaGetRAMAvail();
const ehs_uint32 EhsHMetaGetRAMTotal();
const ehs_uint32 EhsHMetaGetRAMUsedEHS_kB();
const ehs_uint32 EhsHMetaGetStorAvail();
const ehs_uint32 EhsHMetaGetStorUsed();
const ehs_uint32 EhsHMetaGetStorTotal();
const ehs_uint16 EhsHMetaGetCPUUsage();
const ehs_sint16 EhsHMetaGetCPUTemp();
const ehs_uint32 EhsHMetaGetSysAvail();
const ehs_uint32 EhsHMetaGetSysTotal();
const ehs_uint32 EhsHMetaGetSysUsed();
const ehs_char* EhsHMetaGetVersion();
const ehs_char* EhsHMetaGetBuildDate();
const ehs_char* EhsHMetaGetTargetVariant();
const ehs_char* EhsHMetaGetEHSStartDate();
const ehs_char * EhsHMetaGetOSVersion();
 ehs_char * EhsHMetaGetMiscAppNamePtr(); // returns writeable pointer.
const ehs_uint16 EhsHMetaGetMiscAppCPUUsage();
const ehs_uint32 EhsHMetaGetMiscAppRAMUsed_kB();
const ehs_uint32 EhsHMetaGetThreadLoopTimeMin(ehs_threadname_t threadname);
const ehs_uint32 EhsHMetaGetThreadLoopTimeMax(ehs_threadname_t threadname);
const ehs_uint32 EhsHMetaGetThreadLoopTimeAvg(ehs_threadname_t threadname);

void EhsHMetaUpdateThreadLoopTime(ehs_threadname_t threadname, ehs_uint32 time);
void EhsHStatisticsLoopStart(ehs_threadname_t threadname);
void EhsHStatisticsLoopEnd(ehs_threadname_t threadname);

/* and the apps meta data */

const ehs_uint32 EhsHAppMetaGetBuildNumber();
const ehs_char * EhsHAppMetaGetVersionNumber();
const ehs_char * EhsHAppMetaGetAppDate() ;
const ehs_char * EhsHAppMetaGetAppName() ; /* @todo what is the difference of this and EhsHMetaAppSetCurrent? - one is sourced from parser and one from SODL?*/
void EhsHMetaAppSetCurrent(ehs_char * App);
const ehs_char* EhsHMetaAppGetCurrent();

/* Last reset reason — captured once at boot by the target layer */
void EhsHMetaSetLastResetReason(EhsResetReasonType reason);
EhsResetReasonType EhsHMetaGetLastResetReason();
/* Static classifier + stringifier. Both safe to call on unset/unknown values. */
ehs_bool EhsHResetReasonIsAbnormal(EhsResetReasonType reason);
const ehs_char* EhsHResetReasonToString(EhsResetReasonType reason);

/* True if the reset reason is consistent with a software-caused crash of the app
 * (panic / task-wdt / int-wdt / other-wdt). Brownout and external-reset are
 * intentionally excluded — they're not the app's fault. */
ehs_bool EhsHResetReasonIsAppCrash(EhsResetReasonType reason);

/* Policy: should the current app be deleted in response to this reset reason?
 * Today returns EhsHResetReasonIsAppCrash(reason) directly. A future version
 * will consult a consecutive-crash counter (RTC / NVRAM) and only return
 * TRUE after N consecutive crashes — see porting guide TODO. Keep callers
 * using this helper rather than calling IsAppCrash directly so the policy
 * change is a single-site edit. */
ehs_bool EhsHShouldDeleteAppForCrashReason(EhsResetReasonType reason);

/* Persistent boot-state flag API — full prototypes in hal_bootstate.h, included
 * above so kernel-side callers (ehs_main.c) only need hal-api.h. */

void EhsHMetaSetNextAppToRun(ehs_char * App);
const ehs_char* EhsHMetaGetNextAppToRun();

ehs_startupmode_t EhsHAppMetaGetDebugOnStartMode() ;
ehs_bool EhsHMetaGetStartWithoutApp();
void EhsHMetaSetStartWithoutApp(ehs_bool enable);
ehs_bool EhsHRequestEHSInterrupt(); /* return boolean if a command other than EHS_CONTINUE is present */
Ehs_ConsoleCommand_Type EhsHFSMGetInternallyRequestedCommand();
ehs_bool EhsHFSMSetInternallyRequestedCommand(Ehs_ConsoleCommand_Type state );
void EhsHFSMForceInternallyRequestedCommand(Ehs_ConsoleCommand_Type state );

/**
 * Called to initialise the HAL during system initialisation time
 */
void EhsHSys_init();
void EhsHStoreArgInfo(ehs_uint32 argc,ehs_char ** argv,ehs_char * start_dir);
void EhsHWaitForTarget(Ehs_ConsoleCommand_Type (*target_loop_iteration)(void *),void * target_env_blob,ehs_sint16 timeout); /* blocks with 100ms poll for target to become ready */

/**
 * Called to shutdown the HAL prior to shutting down EHS
 *
 */
void EhsHSys_term(void);

/**
 * Shut down everything
 */
//@todo refact name to have an H in it!
void EhsExit(ehs_uint16);

/**
 * Called to prepare the HAL for loading in a new application
 */
void EhsHApp_init(void);

/**
 * Called to prepare the HAL to start executing an application (or to reset
 * an existing application)
 */
void EhsHApp_reset(void);

/**
 * Called by the kernel at the start of app teardown (EhsApplicationReset) and
 * again at the end. Gives the HAL a chance to pause/resume any persistent
 * background work that holds pointers into app memory — render threads,
 * DMA engines, event dispatchers, etc. — so they can't dereference memory
 * that the kernel is about to free. Must be paired; must be safe to call on
 * a target with no such workers (default impl is a no-op).
 */
void EhsHApp_quiesce(void);
void EhsHApp_resume(void);

/**
 * Called to activate the thread to show the app
 */
void EhsHApp_show(void);

/**
 * Called to clean up after the execution of an application in order to make
 * sure everything has been stopped.
 */
void EhsHApp_term(void);

/* Date in W3c format */
ehs_bool EhsHSysUpdateDate(ehs_char * datestring);


/* Devman connectivity monitoring */
ehs_bool EhsHMetaGetMissedPing() ;
/* returns the time in seconds since the last successful ping */
time_t EhsHMetaGetCPUMissedPingTime() ;
void EhsHMetaResetMissedPingTime();
void EhsHMetaSetMissedPing();


#ifndef EHS_SKIP_COMPONENT_ONLY_HAL


/* Device Pairing Support */
void EhsHAppMetaRequestPairedOrganisation(); /* request a new pairing value from server */
void EhsHSysUpdatePairedOrganisation(ehs_uint32 id); /* set to a valid value */
ehs_uint8 EhsHSysGetPairedOrganisationStatus();/* test if value id valid */
ehs_uint32 EhsHSysGetPairedOrganisation();/* get the value */

ehs_bool EhsHSysSetResetTime(ehs_char * datestring);

/* User data management */
/* We may have variations on this that will delete only directories within */
ehs_bool EhsHFileClearUserData(ehs_char *time_string_floating_point);

/* EHS Lifecycle */
ehs_bool EhsHSysRestart(); /* hard EHS restart - OS level - restarts with the default app */
ehs_bool EhsHSysReboot();


/* Devman Data TO SHARE */
ehs_bool EhsHMetaIsNewDevmanMiscDLData() ;
void EhsHMetaSetNewDevmanMiscDLDataNew(ehs_bool val);
ehs_char* EhsHMetaGetPtrToDevmanMiscDLData();
ehs_char* EhsHMetaGetPtrToDevmanMiscDLDataType();
void      EhsHMetaSetDevmanMiscDLData(const ehs_char* zMiscInfo);
EhsTPMutexClass EhsHMetaGetDevmanMiscDLDataMutex();
EhsTPConditionClass EhsHMetaGetDevmanMiscDLDataSemaphor();
void EhsHMetaGetCpyDevmanNewMiscDLData(ehs_char * );
ehs_char*  EhsHMetaGetDevmanNewMiscDLDataPtr() ;
/* upload Data */
ehs_bool EhsHMetaIsNewDevmanMiscULData() ;
void EhsHMetaSetNewDevmanMiscULDataNew(ehs_bool val) ;
ehs_char* EhsHMetaGetPtrToDevmanMiscULData();
ehs_char* EhsHMetaGetPtrToDevmanMiscULDataType();
void EhsHMetaSetDevmanMiscULDataType(const ehs_char* zMiscInfo);
void EhsHMetaSetDevmanMiscULData(const ehs_char* zMiscInfo);
void EhsHMetaGetCpyDevmanMiscULData(ehs_char* zMiscInfo);

const ehs_char* EhsHMetaGetSysInfo();
void EhsHMetaSetSysInfo(const ehs_char* zSysInfo);
/*
 * Populates the Hal Environment variables
 *  - User Directory
 *
 */
ehs_bool EhsHOsSys_UpdateEnvironment();

/*
 * Get the version of CSound Daemon
 *
 */
const ehs_char* EhsHCSoundGetVers();


#endif


#endif /* EHS_HAL_H */
