/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
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

/*****************************************************************************/
/* Run state machine */
typedef enum ehs_startupmode_enum {EHSMETADATA_NODEBUGONSTARTS=0,EHSMETADATA_DEBUGONRESTART,EHSMETADATA_DEBUGONSTART} ehs_startupmode_t;

/* Target tree implemented */
EHS_GLOBAL ehs_bool EhsTPlatformReady(void (*target_loop_iteration)(void*),void * target_env_blob) ;

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

// Don't extern - we'll pass pointers EHS_GLOBAL /*extern*/ EhsMetaDataType EhsMetaData;

/*****************************************************************************/
/* Declare function prototypes  */

/* @todo move all these functions to function specific hal.h and C files .. ha'h should hust be a convenience include for HAL*/

/**
 * Sets a pointer to a function which gets called after the app has attempted to load.
 * An integer passed to a function represents app status IDs defined in hal.h
 */
EHS_GLOBAL void EhsHSetAppLoadStatusCallback(void (*callback)(ehs_uint32));

/**
 * Notifies about app loading status by passing app status IDs defined in hal.h
 */
EHS_GLOBAL void EhsHAppLoadStatusNotify(ehs_uint32 status);

/**
 * EHS & App Meta data getter setter Functions
 *
 * @todo these should be moved to a hal_sysinfo.h file
 */
EHS_GLOBAL void EhsHMetaUpdateStatic();
EHS_GLOBAL void EhsHMetaUpdateDynamic();
EHS_GLOBAL void EhsHMetaUpdateNetwork();
EHS_GLOBAL const ehs_char* EhsHMetaGetToolboxHashes();
EHS_GLOBAL const ehs_char* EhsHMetaGetInstPath(); /* Called to return the installation directory of EHS. */
EHS_GLOBAL const ehs_char* EhsHMetaGetUserPath(); /* Called to return the user directory. */
EHS_GLOBAL const ehs_char* EhsHMetaGetAppsPath(); /* Called to return a nonstandard Apps root path, return NULL if there isn't one set*/
EHS_GLOBAL void EhsHMetaSetAppsPath(ehs_char* path); /* Set a non standard Apps path.*/

/*
EHS_GLOBAL const ehs_char* EhsHMetaAppLiveDefaultDir();
EHS_GLOBAL const ehs_char* EhsHSetMetaAppLiveDefaultDir(ehs_uint8 which);
*/
EHS_GLOBAL const ehs_char* EhsHMetaGetHWID(); /* Return the HW-based device ID */
EHS_GLOBAL ehs_sint16 EhsHMetaGetNetworkMode(); /* Return the HW-based network mode id e.g. DHCP or Static */
EHS_GLOBAL const ehs_char* EhsHMetaGetIPAddr(); /* Return the HW-based IP address */
EHS_GLOBAL const ehs_char* EhsHMetaGetGateway(); /* Return the HW-based gateway */
EHS_GLOBAL const ehs_char* EhsHMetaGetMask(); /* Return the HW-based mask */
EHS_GLOBAL const ehs_char* EhsHMetaGetDNS1(); /* Return the HW-based DNS1 */
EHS_GLOBAL void EhsHMetaSetHWID(const char * value); /* Set the HW-based device ID */
EHS_GLOBAL void EhsHMetaSetIPAddr(const char * value); /* Set the HW-based ip address */
EHS_GLOBAL void EhsHMetaSetInstPath(const char * value); /* Set the HW-based device ID */
EHS_GLOBAL const ehs_char* EhsHMetaGetEHSVersion(); /* Return the HW-based device ID */
EHS_GLOBAL const ehs_uint32 EhsHMetaGetRAMAvail();
EHS_GLOBAL const ehs_uint32 EhsHMetaGetRAMTotal();
EHS_GLOBAL const ehs_uint32 EhsHMetaGetRAMUsedEHS_kB();
EHS_GLOBAL const ehs_uint32 EhsHMetaGetStorAvail();
EHS_GLOBAL const ehs_uint32 EhsHMetaGetStorUsed();
EHS_GLOBAL const ehs_uint32 EhsHMetaGetStorTotal();
EHS_GLOBAL const ehs_uint16 EhsHMetaGetCPUUsage();
EHS_GLOBAL const ehs_sint16 EhsHMetaGetCPUTemp();
EHS_GLOBAL const ehs_uint32 EhsHMetaGetSysAvail();
EHS_GLOBAL const ehs_uint32 EhsHMetaGetSysTotal();
EHS_GLOBAL const ehs_uint32 EhsHMetaGetSysUsed();
EHS_GLOBAL const ehs_char* EhsHMetaGetVersion();
EHS_GLOBAL const ehs_char* EhsHMetaGetBuildDate();
EHS_GLOBAL const ehs_char* EhsHMetaGetTargetVariant();
EHS_GLOBAL const ehs_char* EhsHMetaGetEHSStartDate();
EHS_GLOBAL const ehs_char * EhsHMetaGetOSVersion();
EHS_GLOBAL  ehs_char * EhsHMetaGetMiscAppNamePtr(); // returns writeable pointer.
EHS_GLOBAL const ehs_uint16 EhsHMetaGetMiscAppCPUUsage();
EHS_GLOBAL const ehs_uint32 EhsHMetaGetMiscAppRAMUsed_kB();

/* and the apps meta data */

EHS_GLOBAL const ehs_uint32 EhsHAppMetaGetBuildNumber();
EHS_GLOBAL const ehs_char * EhsHAppMetaGetVersionNumber();
EHS_GLOBAL const ehs_char * EhsHAppMetaGetAppDate() ;
EHS_GLOBAL const ehs_char * EhsHAppMetaGetAppName() ; /* @todo what is the difference of this and EhsHMetaAppSetCurrent? - one is sourced from parser and one from SODL?*/
EHS_GLOBAL void EhsHMetaAppSetCurrent(ehs_char * App);
EHS_GLOBAL const ehs_char* EhsHMetaAppGetCurrent();
EHS_GLOBAL void EhsHMetaSetNextAppToRun(ehs_char * App);
EHS_GLOBAL const ehs_char* EhsHMetaGetNextAppToRun();

EHS_GLOBAL ehs_startupmode_t EhsHAppMetaGetDebugOnStartMode() ;
EHS_GLOBAL ehs_bool EhsHMetaGetStartWithoutApp();
EHS_GLOBAL void EhsHMetaSetStartWithoutApp(ehs_bool enable);
ehs_bool EhsHRequestEHSInterrupt(); /* return boolean if a command other than EHS_CONTINUE is present */
EHS_GLOBAL Ehs_ConsoleCommand_Type EhsHFSMGetInternallyRequestedCommand();
EHS_GLOBAL ehs_bool EhsHFSMSetInternallyRequestedCommand(Ehs_ConsoleCommand_Type state );
EHS_GLOBAL void EhsHFSMForceInternallyRequestedCommand(Ehs_ConsoleCommand_Type state );

/**
 * Called to initialise the HAL during system initialisation time
 */
EHS_GLOBAL void EhsHSys_init();
EHS_GLOBAL void EhsHStoreArgInfo(ehs_uint32 argc,ehs_char ** argv,ehs_char * start_dir);
EHS_GLOBAL void EhsHWaitForTarget(Ehs_ConsoleCommand_Type (*target_loop_iteration)(void *),void * target_env_blob,ehs_sint16 timeout); /* blocks with 100ms poll for target to become ready */

/**
 * Called to shutdown the HAL prior to shutting down EHS
 *
 */
EHS_GLOBAL void EhsHSys_term(void);

/**
 * Shut down everything
 */
//@todo refact name to have an H in it!
EHS_GLOBAL void EhsExit(ehs_uint16);

/**
 * Called to prepare the HAL for loading in a new application
 */
EHS_GLOBAL void EhsHApp_init(void);

/**
 * Called to prepare the HAL to start executing an application (or to reset
 * an existing application)
 */
EHS_GLOBAL void EhsHApp_reset(void);

/**
 * Called to activate the thread to show the app
 */
EHS_GLOBAL void EhsHApp_show(void);

/**
 * Called to clean up after the execution of an application in order to make
 * sure everything has been stopped.
 */
EHS_GLOBAL void EhsHApp_term(void);

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
EHS_GLOBAL const ehs_char* EhsHCSoundGetVers();


#endif


#endif /* EHS_HAL_H */
