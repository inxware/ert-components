/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved.
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
 * with this file. If not, please visit:
 *  <https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file hal.c
 * Controls initialisation of the hardware abstraction layer at various times
 * in the life of EHS.
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

//#define EHSL_MODULE_ID EHSH_LOG_MODULE_KERNEL /**< @todo define a special logger id here */

#include "globals.h"
#include "hal-api.h"
#include "hal.h"

#include "app_data.h" // Needed for the app meta data structure.
#include "include/hal.h"
#include "include/hal_time.h"
#include "targetos_init.h"

#ifdef EHS_COMMS_API_SUPPORT

#ifdef  EHS_DEBUG_TCPIP_CONSOLE
#include "hal_console.h"
#include "console_server.h"
#endif

#include "targetcomms_init.h"
#endif

//@todo the following are a bit specialist for here - should have an AV HAL.
#ifdef EHS_AV_SUPPORT
#ifdef EHS_VIDEO_SUPPORT
#include "targetvideo_init.h"
#endif
#endif

#ifdef EHS_GUI_SUPPORT
#include "hal_gui.h"
#endif


//@todo this probably doesn't need to be here when all the system calls are moved to their respective places.
#if defined(EHS_DEVMAN_SUPPORT) && (EHS_DEVMAN_SUPPORT != EHS_DEVMAN_NONE)
#include "hal_devman.h"
#endif

#ifndef EHS_DEBUG_CONSOLE_THREAD_STACK_SIZE
#define EHS_DEBUG_CONSOLE_THREAD_STACK_SIZE EHS_THREAD_USE_DEFAULT_STACK_SIZE
#endif

/* Use this to set the log level of each component */
/* todo we should have  this overridden for different tarets? */
#ifdef EHS_LOG_LEVEL_VERBOSE
    // Verbose mode...
    void EhsHSetLogLevels()
    {
        EhsHLogger_setLogLevel("Undefined", EHSH_LOG_LEVEL_INFO | EHSH_LOG_LEVEL_WARNING | EHSH_LOG_LEVEL_ERROR);
        EhsHLogger_setLogLevel("Kernel", EHSH_LOG_LEVEL_INFO | EHSH_LOG_LEVEL_WARNING | EHSH_LOG_LEVEL_ERROR);
        EhsHLogger_setLogLevel("Graphics",  EHSH_LOG_LEVEL_INFO | EHSH_LOG_LEVEL_WARNING | EHSH_LOG_LEVEL_ERROR);
        EhsHLogger_setLogLevel("Logger",  EHSH_LOG_LEVEL_INFO | EHSH_LOG_LEVEL_WARNING | EHSH_LOG_LEVEL_ERROR);
        EhsHLogger_setLogLevel("HalMemory",  EHSH_LOG_LEVEL_INFO | EHSH_LOG_LEVEL_WARNING | EHSH_LOG_LEVEL_ERROR);
        EhsHLogger_setLogLevel("HalProcess", EHSH_LOG_LEVEL_INFO | EHSH_LOG_LEVEL_WARNING | EHSH_LOG_LEVEL_ERROR);
        EhsHLogger_setLogLevel("HalString", EHSH_LOG_LEVEL_INFO | EHSH_LOG_LEVEL_WARNING | EHSH_LOG_LEVEL_ERROR);
        EhsHLogger_setLogLevel("TgtViewport", EHSH_LOG_LEVEL_INFO | EHSH_LOG_LEVEL_WARNING | EHSH_LOG_LEVEL_ERROR);
        EhsHLogger_setLogLevel("Network",EHSH_LOG_LEVEL_INFO | EHSH_LOG_LEVEL_WARNING | EHSH_LOG_LEVEL_ERROR);
        EhsHLogger_setLogLevel("Devman", EHSH_LOG_LEVEL_INFO | EHSH_LOG_LEVEL_WARNING | EHSH_LOG_LEVEL_ERROR);
        EhsHLogger_setLogLevel("file", EHSH_LOG_LEVEL_INFO | EHSH_LOG_LEVEL_WARNING | EHSH_LOG_LEVEL_ERROR);
    }
#else
    // Not verbose mode...
    void EhsHSetLogLevels()
    {
        EhsHLogger_setLogLevel("Undefined", EHSH_LOG_LEVEL_ERROR); /*  set the log level if logging enabled in build */
        EhsHLogger_setLogLevel("Kernel", EHSH_LOG_LEVEL_ERROR);
        EhsHLogger_setLogLevel("Graphics",  EHSH_LOG_LEVEL_ERROR);
        EhsHLogger_setLogLevel("Logger",  EHSH_LOG_LEVEL_ERROR);
        EhsHLogger_setLogLevel("HalMemory",  EHSH_LOG_LEVEL_ERROR);
        EhsHLogger_setLogLevel("HalProcess", EHSH_LOG_LEVEL_ERROR); /*  set the log level if logging enabled in build */
        EhsHLogger_setLogLevel("HalString", EHSH_LOG_LEVEL_ERROR);
        EhsHLogger_setLogLevel("TgtViewport", EHSH_LOG_LEVEL_ERROR);
        EhsHLogger_setLogLevel("Network",EHSH_LOG_LEVEL_INFO);
        EhsHLogger_setLogLevel("Devman", EHSH_LOG_LEVEL_ERROR); /*  set the log level if logging enabled in build */
        EhsHLogger_setLogLevel("file", EHSH_LOG_LEVEL_ERROR);//|EHSH_LOG_LEVEL_WARNING|EHSH_LOG_LEVEL_INFO);
    }
#endif

/* Log Levels
EHSH_LOG_LEVEL_ERROR	= 0x01,
EHSH_LOG_LEVEL_WARNING	= 0x02,
EHSH_LOG_LEVEL_INFO		= 0x04,
EHSH_LOG_LEVEL_ENTER	= 0x08,
EHSH_LOG_LEVEL_EXIT		= 0x10
*/

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */


/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Types  defined with file-scope */

/* Private data structs */

/* Variables Required from other components */
extern EhsApplicationMetaDataType EhsApplicationMetaData;

void (*EhsAppLoadedCallback)(ehs_uint32) = NULL;

/*****************************************************************************/
/* Variables defined with global-scope */

/**
 * Flag to indicate that initialisation has been completed for the target.
 */
EHS_LOCAL ehs_bool EhsL_initComplete =EHS_FALSE;
ehs_bool *EhsHSys_initCompleteRef = &EhsL_initComplete;

/*****************************************************************************/
/* Function definitions */

/* NOTE: WE ARE NOW GLOBBERING ALL OF THIS TO NULL AT BOOT TO CLEAR ALL STRINGS */

#ifdef __cplusplus
EhsMetaDataType EhsMetaData=
{
    0,
    EHS_FALSE,
    0,
    0, // was 2 for some reason
    EHS_FALSE,
    EHS_FALSE,
    NULL,// Notes these need to be set
    NULL,
    EHS_FALSE,
    0LL,
    0,
    EHS_FALSE
};
#else
EhsMetaDataType EhsMetaData=
{
    .DynamicUpdateTime=0,
    .bStaticUpdate=EHS_FALSE,
    .PairedOrganisationID=0,
    .PairedOrganisationIDRequested=0, // was 2 for some reason
    .NewDevmanMiscDLData=EHS_FALSE,
    .NewDevmanMiscULData=EHS_FALSE,
    .condDevmanNewMiscDLData = NULL,// Notes these need to be set
    .mutexDevmanNewMiscDLData = NULL,
    .devmanPingFail = EHS_FALSE,
    .devmanLastGoodPing = 0LL,
    .MiscAppProcId=0,
    .bStartWithoutApp=EHS_FALSE
}; //flag to identify static elements are valid;
#endif

/**/
EhsTargetConfigInfoType EhsTargetConfigInfo = {
    .defaultStringDataLen=EHS_DATA_TABLE_STRING_DEFAULT_LENGTH
};

#define EHSVERSIONINFOFILE "version.nfo"

EHS_LOCAL const ehs_char* ehsToolboxHashes = EHS_TOOLBOX_HASHES"0x0";


/**
 * Sets a pointer to a function which gets called after the app has attempted to load.
 * An integer passed to a function represents app status IDs defined in hal.h
 */
void EhsHSetAppLoadStatusCallback(void (*callback)(ehs_uint32))
{
    EhsAppLoadedCallback = callback;
}

/**
 * Notifies about app loading status by passing app status IDs defined in hal.h
 */
void EhsHAppLoadStatusNotify(ehs_uint32 status)
{
    if(EhsAppLoadedCallback != NULL){
        EhsAppLoadedCallback(status);
    }
}

/*
 * Populate the meta data with the start up info and identifiy the install directory
 * This does not work for ehs started from a search path command.
 * @todo cover the case for run from search path.
 */
#define EHS_DEBUG_ROOT_DIR
void EhsHStoreArgInfo(ehs_uint32 argc,ehs_char ** argv,ehs_char * start_dir)
{
#ifndef EHS_TARGET_NO_MAIN_ARGS
    ehs_char buf[EHS_MAXPATHLENGTH];
    ehs_char *temp;
    ehs_uint16 i=0;
    EhsStrcpy(EhsMetaData.zArgv0,argv[0]); // contains the calling command.
    if (argc>1) EhsStrcpy(EhsMetaData.zArgv1,argv[1]);  // contains the calling parameter.
    if (EhsStrcmp(EhsMetaData.zArgv1,"debug_on_restart") )    // @todo make this a better opt reader
    {
        EhsMetaData.DebugOnStart=EHSMETADATA_DEBUGONRESTART;
    }
    else
    {
        EhsMetaData.DebugOnStart=EHSMETADATA_NODEBUGONSTARTS;
    }

    // check if we want to start eRT without running any app
    if (EhsStrcmp(EhsMetaData.zArgv1,"--no-app") == 0) {
        EhsMetaData.bStartWithoutApp = EHS_TRUE;
    }else{
        EhsMetaData.bStartWithoutApp = EHS_FALSE;
    }

    /* Because we are using Opaque processing handles we need to initialise the handles using the target specific process.. */
    EhsProcessInitMutex(&EhsMetaData.mutexDevmanNewMiscDLData); // note this will only work once!
    EhsProcessInitCond(&EhsMetaData.condDevmanNewMiscDLData); // note this will only work once!

    /* ToDo this code should be moved to a taret specific cade area */
#ifdef EHS_MINGW // This method for windows
    printf("***************************************************************\n");
    printf("start_dir=%s\nArgv0=%s\n",&EhsMetaData.zInstallRootDirectory[i],EhsMetaData.zArgv0);
    EhsStrcpy(EhsMetaData.zEhsStartedDirectory,start_dir); // save the PWD - @todo do we need this?
    //EhsStrcpy(EhsMetaData.zInstallRootDirectory,EhsMetaData.zArgv0);
    if (EhsMetaData.zArgv0[1] !=':')   /* Check we haven;t been given an absolute path to the exe (e.g. by XP) */
    {
        EhsStrcpy(EhsMetaData.zInstallRootDirectory,start_dir);
        EhsStrcat(EhsMetaData.zInstallRootDirectory,"\\");
        EhsStrcat(EhsMetaData.zInstallRootDirectory,EhsMetaData.zArgv0); //if not assume we might have a relative  path to the current dir
    }
    else    // we were give an absolute path by windows so chuck away PWD and use this
    {
        EhsStrcpy(EhsMetaData.zInstallRootDirectory,EhsMetaData.zArgv0);
    }
    i=EhsStrlen(EhsMetaData.zInstallRootDirectory);

    if ( (i>0) && ((EhsMetaData.zInstallRootDirectory[i-1]=='/') || (EhsMetaData.zInstallRootDirectory[i-1] == '\\')))
    {
        EhsMetaData.zInstallRootDirectory[i-1]='\0';//gets rid of a trailing in path
        i--;
    }
    // get the directory above.
    while ((i>=0) && (EhsMetaData.zInstallRootDirectory[i] != '\\') && (EhsMetaData.zInstallRootDirectory[i] != '/'))
    {
        i--; //gets rid of ehs.exe in path
    }
    //printf("AA= %s\n",&EhsMetaData.zInstallRootDirectory[i]); // check if we have a dot before the command
    if ( (i>0) && (EhsMetaData.zInstallRootDirectory[i-1]=='.'))
    {
        EhsMetaData.zInstallRootDirectory[i-2]='\0';//gets rid of a trailing in path
        i-=2; //remove the slash also
    }
    if (i>0)
    {
        EhsMetaData.zInstallRootDirectory[i]='\0';//gets rid of bin/ehs.exe in path
    }
    while ((i>=0) && (EhsMetaData.zInstallRootDirectory[i] != '\\') && (EhsMetaData.zInstallRootDirectory[i] != '/'))
    {
        i--;
    }
    //printf("BB= %s\n",EhsMetaData.zInstallRootDirectory);
    if (i>0)
    {
        EhsMetaData.zInstallRootDirectory[i]='\0';//gets rid of bin/ehs.exe in path
    }
    else   // have a stab..
    {
        EhsStrcpy(EhsMetaData.zInstallRootDirectory,"../");
    }

#else


#ifdef EHS_ANDROID
    //EhsStrcpy(EhsMetaData.zInstallRootDirectory,"/sdcard/ehs"); /*@todo -  hardware specific ops here for now */
    //if (EhsTF_mkdir("/sdcard/ehs")) {
    //LOGI("Opened Android data directory OK");
    //}
    //else {
    //LOGI("Failed Opened Android data directory OK");
    //}

#else
    /* Add the command line path if there was one */
    #ifndef INX_SODL_IN_FLASH
    EhsStrcpy(EhsMetaData.zEhsStartedDirectory,start_dir);
    if (EhsMetaData.zArgv0[0]=='/')   /* Check we haven;t been given an absolute path to the exe (e.g. by XP) */
    {
        EhsStrcpy(EhsMetaData.zInstallRootDirectory, EhsMetaData.zArgv0); /* useful for running in GDB */
        temp=EhsStrstr(EhsMetaData.zInstallRootDirectory, "bin/ehs.exe");
        if (temp)
        {
            *temp='\0'; //truncate to the beginning
        }
        else   /* not much we can do here */
        {
            //EhsMetaData.zInstallRootDirectory ;
        }

    }
    else     /* Build path by concatonation */
    {
        EhsStrcpy(EhsMetaData.zInstallRootDirectory, start_dir);

        EhsStrcpy(buf, EhsMetaData.zArgv0);
        i = EhsStrlen(buf);
        while ((i >= 0) && (buf[i] != '\\') && (buf[i] != '/'))
        {
            i--;
        }
        if (i > 0)
        {
            buf[i] = '\0';//gets rid of bin/ehs.exe in path
        }
        else     // have a stab..
        {
            buf[0] = '\0';
        }
        i = EhsStrlen(EhsMetaData.zInstallRootDirectory);
        //remove any trailing /s
        if ((i > 0) && ((EhsMetaData.zInstallRootDirectory[i] == '/')
                        || (EhsMetaData.zInstallRootDirectory[i] == '\\')))
        {
            EhsMetaData.zInstallRootDirectory[i] = '\0';//gets rid of ehs.exe in path
            i--;
        }
        // get the directory above.
        while ((i >= 0) && (EhsMetaData.zInstallRootDirectory[i] != '\\')
                && (EhsMetaData.zInstallRootDirectory[i] != '/'))
        {
            i--;
        }
        if (i > 0)
        {
            EhsMetaData.zInstallRootDirectory[i] = '\0';//gets rid of bin/ehs.exe in path
        }
        else     // have a stab..
        {
            EhsStrcpy(EhsMetaData.zInstallRootDirectory, ".." EHS_TD_FILES_SEPARATOR_STR);
        }
        /* Add the command path */
        EhsStrcat(EhsMetaData.zInstallRootDirectory, EHS_TD_FILES_SEPARATOR_STR);
        if (buf[0] != '.')
        {
            EhsStrcat(EhsMetaData.zInstallRootDirectory, buf);
        }
        else
        {
            EhsStrcat(EhsMetaData.zInstallRootDirectory, &buf[1]);
        }
    } /* end of normal concatenation method */
    #endif //   #ifndef INX_SODL_IN_FLASH
#endif
#endif
#ifdef EHS_DEBUG_ROOT_DIR
#ifndef INX_SODL_IN_FLASH
    //EHSH_LOG_INFO("argv0=%s",EhsMetaData.zArgv0);
    //EHSH_LOG_INFO("startdir=%s",start_dir);
    EHSH_LOG_INFO("argv0=%s", EhsMetaData.zArgv0);
    EHSH_LOG_INFO("startdir=%s", start_dir);
    EHSH_LOG_INFO("Final install dir=%s", EhsMetaData.zInstallRootDirectory);
#endif
#endif
#endif // EHS_TARGET_NO_MAIN_ARGS
    //EHSH_LOG_INFO("Done all sys init code");
    /* And set the start time stamp */
    EhsHGetdateTime(EhsMetaData.zEHSStartDate,EHS_START_DATE_LENGTH_MAX,EHS_TRUE, 0);
    //EHSH_LOG_INFO("Done all sys init code");
}

void EhsHInitEhsMetaData() {
#ifndef INX_SODL_IN_FLASH
    //EhsMemset(&EhsMetaData,0,sizeof(EhsMetaData)); // This does something bad...
    EhsMetaData.zAppsDirectory[0]='\0'; // just in case the aboe doesn't do what we expect ;(
#endif
}

/* Version and system status information */
void EhsHMetaUpdateStatic()
{
    EhsTOsSys_UpdateEnvironment(&EhsMetaData,EHS_OS_ENV_STATIC_ID);
    EhsMetaData.bStaticUpdate=EHS_TRUE;
}
void EhsHMetaUpdateDynamic()
{
    EhsTOsSys_UpdateEnvironment(&EhsMetaData,EHS_OS_ENV_DYNAMIC_ID);
}

void EhsHMetaUpdateNetwork()
{
    EhsTOsSys_UpdateEnvironment(&EhsMetaData,EHS_OS_ENV_NETWORK_ID);
}

const ehs_char* EhsHMetaGetInstPath()
{
#ifndef INX_SODL_IN_FLASH
    return EhsMetaData.zInstallRootDirectory;
#else
    return NULL;
#endif
}
const ehs_char* EhsHMetaGetToolboxHashes(){
    return ehsToolboxHashes;
}
const ehs_char* EhsHMetaGetUserPath()
{
#ifndef INX_SODL_IN_FLASH
    return EhsMetaData.zUserDirectory;
#else
    return NULL;
#endif
}
const ehs_char* EhsHMetaGetAppsPath()
{
#ifndef INX_SODL_IN_FLASH
    if ( EhsMetaData.zAppsDirectory[0] != '\0' )
        return EhsMetaData.zAppsDirectory;
    else
        return NULL;
#else
    return NULL;
#endif
}

void EhsHMetaSetAppsPath(ehs_char* path)
{
#ifndef INX_SODL_IN_FLASH
    EhsStrcpy(EhsMetaData.zAppsDirectory,path);
#endif
}

const ehs_char* EhsHMetaGetHWID()
{
    return EhsMetaData.zDeviceID;
}

ehs_sint16 EhsHMetaGetNetworkMode()
{
    return EhsMetaData.nDeviceNetworkMode;
}

const ehs_char* EhsHMetaGetIPAddr()
{
    return EhsMetaData.zDeviceIPAddr;
}

const ehs_char* EhsHMetaGetGateway()
{
    return EhsMetaData.zDeviceGateway;
}

const ehs_char* EhsHMetaGetMask()
{
    return EhsMetaData.zDeviceMask;
}

const ehs_char* EhsHMetaGetDNS1()
{
    return EhsMetaData.zDeviceDNS1;
}

void EhsHMetaSetHWID(const char * value)
{
    if (value) EhsStrcpy(EhsMetaData.zDeviceID,value);
}

void EhsHMetaSetInstPath(const char * value)
{
#ifndef INX_SODL_IN_FLASH
    if (value) EhsStrcpy(EhsMetaData.zInstallRootDirectory,value);
#endif
}

void EhsHMetaSetIPAddr(const char * value)
{
    if (value) EhsStrcpy(EhsMetaData.zDeviceIPAddr,value);
}

const ehs_char* EhsHMetaGetEHSVersion()
{

    //printf("EhsMetaData.zVersion =%s\n",EhsMetaData.zVersion);
    return EhsMetaData.zVersion;
}
const ehs_uint32 EhsHMetaGetRAMAvail()
{
    //printf("hsMetaData.RAMAvail_KB =%s",hsMetaData.RAMAvail_KB);
    return EhsMetaData.RAMAvail_KB;
}
const ehs_uint32 EhsHMetaGetRAMUsedEHS_kB()
{
    //printf("EhsMetaData.RAMUsed_KB =%s\n",EhsMetaData.RAMUsed_KB);
    return EhsMetaData.RAMUsed_KB;
}
const ehs_uint32 EhsHMetaGetRAMTotal()
{
    //printf("RAM Total get=%d\n",EhsMetaData.RAMTotal_KB);
    return EhsMetaData.RAMTotal_KB;
}
const ehs_uint32 EhsHMetaGetStorAvail()
{
    //printf("hsHMetaGetStorAvail =%s\n",);
    return (EhsMetaData.nUserSpaceTotal_KB-EhsMetaData.nUserSpaceUsed_KB);
}
const ehs_uint32 EhsHMetaGetStorUsed()
{
    //printf(" =%s\n",);
    return EhsMetaData.nUserSpaceUsed_KB;
}
const ehs_uint32 EhsHMetaGetStorTotal()
{
    //printf(" =%s\n",);
    return EhsMetaData.nUserSpaceTotal_KB;
}

const ehs_uint16 EhsHMetaGetCPUUsage()
{
//	printf("CPU_usage get=%d\n",EhsMetaData.CPUUsage);
    return EhsMetaData.CPUUsage;
}

const ehs_sint16 EhsHMetaGetCPUTemp()
{
//  printf("CPU temperature get=%d\n", EhsMetaData.CPUTemp);
    return EhsMetaData.CPUTemp;
}

const ehs_uint16 EhsHMetaGetMiscAppCPUUsage()
{
    return EhsMetaData.MiscAppCPUUsage;
}

const ehs_uint32 EhsHMetaGetMiscAppRAMUsed_kB()
{
    return EhsMetaData.MiscAppRAMUsed_KB;
}

const ehs_uint32 EhsHMetaGetSysAvail()
{
    return (EhsMetaData.nSysSpaceTotal_KB -EhsMetaData.nSysSpaceUsed_KB);
}

const ehs_uint32 EhsHMetaGetSysTotal()
{
    //printf(" =%s\n",);
    return EhsMetaData.nSysSpaceTotal_KB;
}

const ehs_uint32 EhsHMetaGetSysUsed()
{
    //printf(" =%s\n",);
    return EhsMetaData.nSysSpaceUsed_KB;
}
const ehs_char * EhsHMetaGetVersion()
{
    //printf("EhsMetaData.zVersion =%s\n",EhsMetaData.zVersion);
    return EhsMetaData.zVersion;
}
const ehs_char * EhsHMetaGetBuildDate()
{
    //printf("EhsMetaData.zBuildDate =%s\n",EhsMetaData.zBuildDate);
    return EhsMetaData.zBuildDate;
}
const ehs_char * EhsHMetaGetTargetVariant()
{
    #ifdef EHS_ESP32_SUPPORT
    return "ehs-esp32";
    #else
    return EhsMetaData.zTargetVariant;
    #endif
}
const ehs_char * EhsHMetaGetEHSStartDate()
{
    //printf("EhsMetaData.zEHSStartDate =%s\n",EhsMetaData.zEHSStartDate);
    return EhsMetaData.zEHSStartDate;
}
ehs_startupmode_t EhsHMetaGetDebugOnStartMode()
{
    return EhsMetaData.DebugOnStart;
}

ehs_bool EhsHMetaGetStartWithoutApp()
{
    return EhsMetaData.bStartWithoutApp;
}

void EhsHMetaSetStartWithoutApp(ehs_bool enable)
{
    EhsMetaData.bStartWithoutApp = enable;
}

void EhsHMetaAppSetCurrent(ehs_char * App)
{
    /* todo should check length and return an error */
#ifndef INX_SODL_IN_FLASH
    EhsStrcpy(EhsMetaData.AppCurrentLive, App);
#endif
}

//@todo - needs renaming!
const ehs_char* EhsHMetaAppGetCurrent()
{
#ifndef INX_SODL_IN_FLASH
    return EhsMetaData.AppCurrentLive;
#endif
}

void EhsHMetaSetNextAppToRun(ehs_char * App)
{
    /* todo should check length and return an error */
#ifndef INX_SODL_IN_FLASH
    EhsStrcpy(EhsMetaData.NextAppToRun, App);
#endif
}

const ehs_char* EhsHMetaGetNextAppToRun()
{
#ifndef INX_SODL_IN_FLASH
    return EhsMetaData.NextAppToRun;
#else
    return NULL;
#endif
}

/* Get the Loop time according to which thread */

const ehs_uint32 EhsHMetaGetThreadLoopTimeMin(ehs_threadname_t threadname)
{
    #ifdef EHS_STATISTICS_THREADLOOPTIME_ENABLED
    if (threadname >= EHSTHREADNAME_MAX) return 0;
    return EhsMetaData.ThreadLoopTimeMin[threadname];
    #else
    return 0;
    #endif//EHS_STATISTICS_THREADLOOPTIME_ENABLED
}
const ehs_uint32 EhsHMetaGetThreadLoopTimeMax(ehs_threadname_t threadname)
{
    #ifdef EHS_STATISTICS_THREADLOOPTIME_ENABLED
    if (threadname >= EHSTHREADNAME_MAX) return 0;
    return EhsMetaData.ThreadLoopTimeMax[threadname];
    #else
    return 0;
    #endif//EHS_STATISTICS_THREADLOOPTIME_ENABLED
}
const ehs_uint32 EhsHMetaGetThreadLoopTimeAvg(ehs_threadname_t threadname)
{
    #ifdef EHS_STATISTICS_THREADLOOPTIME_ENABLED
    if (threadname >= EHSTHREADNAME_MAX) return 0;
    return EhsMetaData.ThreadLoopTimeAvg[threadname];
    #else
    return 0;
    #endif//EHS_STATISTICS_THREADLOOPTIME_ENABLED
}

/**
 * @brief Update the thread loop time to the min, max and avg
 *
 * @param threadname The enumeration of the related thread
 * @param time The loop time to be updated
 * @return none
 */
void EhsHMetaUpdateThreadLoopTime(ehs_threadname_t threadname, ehs_uint32 time)
{
    #ifdef EHS_STATISTICS_THREADLOOPTIME_ENABLED
    if (threadname >= EHSTHREADNAME_MAX) return;
    if (EhsMetaData.ThreadLoopTimeAvg[threadname] == 0) EhsMetaData.ThreadLoopTimeAvg[threadname] = time;
    EhsMetaData.ThreadLoopTimeAvg[threadname] = (time + 15*EhsMetaData.ThreadLoopTimeAvg[threadname]) / 16;
    if (EhsMetaData.ThreadLoopTimeMin[threadname] == 0) EhsMetaData.ThreadLoopTimeMin[threadname] = time;
    if (time < EhsMetaData.ThreadLoopTimeMin[threadname]) EhsMetaData.ThreadLoopTimeMin[threadname] = time;
    if (time > EhsMetaData.ThreadLoopTimeMax[threadname]) EhsMetaData.ThreadLoopTimeMax[threadname] = time;
    #endif//EHS_STATISTICS_THREADLOOPTIME_ENABLED
}

static ehs_uint64 gThreadLoopTimeTemp[EHSTHREADNAME_MAX];
void EhsHStatisticsLoopStart(ehs_threadname_t threadname)
{
    #ifdef EHS_STATISTICS_THREADLOOPTIME_ENABLED
    if (threadname >= EHSTHREADNAME_MAX) return;
    gThreadLoopTimeTemp[threadname] = /*esp_timer_get_time();*/EHS_CURRENT_TIME;
    #endif//EHS_STATISTICS_THREADLOOPTIME_ENABLED
}
void EhsHStatisticsLoopEnd(ehs_threadname_t threadname)
{
    #ifdef EHS_STATISTICS_THREADLOOPTIME_ENABLED
    if (threadname >= EHSTHREADNAME_MAX) return;
    gThreadLoopTimeTemp[threadname] = EHS_CURRENT_TIME /*esp_timer_get_time()*/ - gThreadLoopTimeTemp[threadname];
    EhsHMetaUpdateThreadLoopTime(threadname, (ehs_uint32) gThreadLoopTimeTemp[threadname]);
    #endif//EHS_STATISTICS_THREADLOOPTIME_ENABLED
}

/* And the same for the app data. This structure is from the app_data. module */

const ehs_uint32 EhsHAppMetaGetBuildNumber()
{
    //printf(" =%s\n",);
    return EhsApplicationMetaData.nSODLBuildNumber;
}

const ehs_char * EhsHAppMetaGetVersionNumber()
{
    //printf("hsApplicationMetaData.zVersion =%s\n",EhsApplicationMetaData.zVersion);
    return EhsApplicationMetaData.zVersion;
}

const ehs_char * EhsHAppMetaGetAppDate()
{
    //printf("EhsApplicationMetaData.zSODLdate =%s\n",EhsApplicationMetaData.zSODLdate);
    return EhsApplicationMetaData.zSODLdate;
}

const ehs_char * EhsHAppMetaGetAppName()
{
    return EhsApplicationMetaData.zApplicationName;
}

/* The followiing for another process name (not an EHS app)*/
 ehs_char * EhsHMetaGetMiscAppNamePtr()
{
#if defined(EHS_DEVMAN_SUPPORT) && (EHS_DEVMAN_SUPPORT != EHS_DEVMAN_NONE)
    return EhsMetaData.MiscAppProcName;
#else
    return NULL;
#endif
}

/* Retuens true if we have had a failed ping to the primary server */
ehs_bool EhsHMetaGetMissedPing()
{
    return EhsMetaData.devmanPingFail;
}

/* returns the time in seconds since the last successful ping */
time_t EhsHMetaGetCPUMissedPingTime()
{
    return time(NULL) - EhsMetaData.devmanLastGoodPing;
}

void EhsHMetaResetMissedPingTime()
{
    EhsMetaData.devmanPingFail = EHS_FALSE;
    EhsMetaData.devmanLastGoodPing = time(NULL);
}

void EhsHMetaSetMissedPing()
{
    EhsMetaData.devmanPingFail = EHS_TRUE;
}

/* The following are inter app related functions, but get data from the ES environment - not the app environment */
const ehs_char * EhsHAppMetaGetLiveDir()
{
#ifndef INX_SODL_IN_FLASH
    return EhsMetaData.AppCurrentLive;
#else
    return NULL;
#endif
}

extern const ehs_char* EhsHMetaGetSysInfo()
{
    return EhsMetaData.zSysInfo;
}

void EhsHMetaSetSyscInfo(const ehs_char* zSysInfo)
{
    EhsStrcpy(EhsMetaData.zSysInfo, zSysInfo);
}

/* Miscelaneoud data (JSOn usually) to server */
/* Download data */
extern ehs_bool EhsHMetaIsNewDevmanMiscDLData()
{
    return EhsMetaData.NewDevmanMiscDLData;
}

extern void EhsHMetaSetNewDevmanMiscDLDataNew(ehs_bool val)
{
    EhsMetaData.NewDevmanMiscDLData = val;
}

extern ehs_char* EhsHMetaGetPtrToDevmanMiscDLData()
{
#if defined(EHS_DEVMAN_SUPPORT) && (EHS_DEVMAN_SUPPORT != EHS_DEVMAN_NONE)
    return EhsMetaData.zDevmanMiscDLData;
#else
    return NULL;
#endif
}


extern ehs_char* EhsHMetaGetPtrToDevmanMiscDLDataType()
{
#if defined(EHS_DEVMAN_SUPPORT) && (EHS_DEVMAN_SUPPORT != EHS_DEVMAN_NONE)
    return EhsMetaData.zDevmanMiscDLDataType;
#else
    return NULL;
#endif
}

/* Thread safe versions
 * todo:
 * needs to concatenate data into  zDevmanMiscDLData
 * needs to write data to file in changes
 * need  a new function for reading initial data from file.
 * */
void EhsHMetaSetDevmanMiscDLDataType(const ehs_char* zMiscInfo)
{
#if defined(EHS_DEVMAN_SUPPORT) && (EHS_DEVMAN_SUPPORT != EHS_DEVMAN_NONE)
    EhsTPMutex_lock(EhsTPMutex_devmanMiscBuffers);
    EhsStrcpy(EhsMetaData.zDevmanMiscDLDataType, zMiscInfo);
    EhsTPMutex_unlock(EhsTPMutex_devmanMiscBuffers);
#endif //EHS_DEVMAN_SUPPORT
}

/* Set some miscellaneous devman JSON monitor data into two buffers??
  and then signal the semaphore there's some data
*/

extern void EhsHMetaSetDevmanMiscDLData(const ehs_char* zMiscInfo)
{
#if defined(EHS_DEVMAN_SUPPORT) && (EHS_DEVMAN_SUPPORT != EHS_DEVMAN_NONE)
    EhsTPMutex_lock(EhsTPMutex_devmanMiscBuffers);
    EhsStrcpy(EhsMetaData.zDevmanMiscDLData, zMiscInfo); // * todo - this needs to merge JSON - Ideally not using  JSON library for portability
    EhsStrcpy(EhsMetaData.zDevmanNewMiscDLData, zMiscInfo);
    if ( EhsMetaData.mutexDevmanNewMiscDLData != NULL
         && EhsMetaData.condDevmanNewMiscDLData != NULL ) { /*( Only signal if the signals have been set up */
        EhsTPMutex_lock(EhsMetaData.mutexDevmanNewMiscDLData);
        EhsTPCondition_broadcast(EhsMetaData.condDevmanNewMiscDLData);
        EhsTPMutex_unlock(EhsMetaData.mutexDevmanNewMiscDLData);
    }
    EhsTPMutex_unlock(EhsTPMutex_devmanMiscBuffers);
#endif //EHS_DEVMAN_SUPPORT
}

/* Strange place for these?*/
extern EhsTPConditionClass EhsHMetaGetDevmanMiscDLDataSemaphor()
{
    return EhsMetaData.condDevmanNewMiscDLData;

}
extern EhsTPMutexClass EhsHMetaGetDevmanMiscDLDataMutex()
{
    return EhsMetaData.mutexDevmanNewMiscDLData;
}

void EhsHMetaGetCpyDevmanMiscDLData(ehs_char* zMiscInfo)
{
#if defined(EHS_DEVMAN_SUPPORT) && (EHS_DEVMAN_SUPPORT != EHS_DEVMAN_NONE)
    EhsTPMutex_lock(EhsTPMutex_devmanMiscBuffers);
    EhsStrcpy(zMiscInfo,EhsMetaData.zDevmanMiscDLDataType);
    EhsTPMutex_unlock(EhsTPMutex_devmanMiscBuffers);
#endif //EHS_DEVMAN_SUPPORT
}

/* this gets just the new part */
extern void EhsHMetaGetCpyDevmanNewMiscDLData(ehs_char* zMiscInfo)
{
#if defined(EHS_DEVMAN_SUPPORT) && (EHS_DEVMAN_SUPPORT != EHS_DEVMAN_NONE)
    EhsTPMutex_lock(EhsTPMutex_devmanMiscBuffers);
    EhsStrcpy(zMiscInfo,EhsMetaData.zDevmanNewMiscDLData);
    EhsTPMutex_unlock(EhsTPMutex_devmanMiscBuffers);
#endif //EHS_DEVMAN_SUPPORT
}

/* this gets just the new part */
extern ehs_char*  EhsHMetaGetDevmanNewMiscDLDataPtr()
{
#if defined(EHS_DEVMAN_SUPPORT) && (EHS_DEVMAN_SUPPORT != EHS_DEVMAN_NONE)
    return EhsMetaData.zDevmanNewMiscDLData;
#else
    return NULL;
#endif
}


/* upload Data */
extern ehs_bool EhsHMetaIsNewDevmanMiscULData()
{
    return EhsMetaData.NewDevmanMiscULData;
}
/************* Not thread safe */
extern void EhsHMetaSetNewDevmanMiscULDataNew(ehs_bool val)
{
    EhsMetaData.NewDevmanMiscULData = val;
}

extern ehs_char* EhsHMetaGetPtrToDevmanMiscULData()
{
#if defined(EHS_DEVMAN_SUPPORT) && (EHS_DEVMAN_SUPPORT != EHS_DEVMAN_NONE)
    return EhsMetaData.zDevmanMiscULData;
#else
    return NULL;
#endif
}

/********************************/
extern void EhsHMetaSetDevmanMiscULData(const ehs_char* zMiscInfo)
{
#if defined(EHS_DEVMAN_SUPPORT) && (EHS_DEVMAN_SUPPORT != EHS_DEVMAN_NONE)
    EhsTPMutex_lock(EhsTPMutex_devmanMiscBuffers);
    EhsStrcpy(EhsMetaData.zDevmanMiscULData, zMiscInfo);
    EhsTPMutex_unlock(EhsTPMutex_devmanMiscBuffers);
#endif //EHS_DEVMAN_SUPPORT
}

extern void EhsHMetaGetCpyDevmanMiscULData(ehs_char* zMiscInfo)
{
#if defined(EHS_DEVMAN_SUPPORT) && (EHS_DEVMAN_SUPPORT != EHS_DEVMAN_NONE)
    EhsTPMutex_lock(EhsTPMutex_devmanMiscBuffers);
    //	printf("XXXX[%x] = %s\n",zMiscInfo,EhsMetaData.zDevmanMiscULData);
    EhsStrcpy(zMiscInfo,EhsMetaData.zDevmanMiscULData);
    //	printf("XXXX2[%x] = %s\n",zMiscInfo,EhsMetaData.zDevmanMiscULData);
    EhsTPMutex_unlock(EhsTPMutex_devmanMiscBuffers);
#endif //EHS_DEVMAN_SUPPORT
}

void EhsHGetEHSVersionInfo(EhsMetaDataType * EhsMetaData)
{
    ehs_FILE* EhsVersionFile;
    ehs_char temp[64]; //*todo2022 remove this?

    EhsVersionFile = Ehs_SysFopen(EHSVERSIONINFOFILE,"r"); /* if sysdata doesn't exist create in default directory */
    if (!EhsVersionFile)
    {
        ehs_char szParameterFilePath[EHS_MAXPATHLENGTH];
        EhsTF_tryCanonicPath(szParameterFilePath, EHS_RUNTIME_SYSDATA_DIR,EHSVERSIONINFOFILE,EHS_TRUE );
        EHSH_LOG_ERROR("Can't open version info file: %s",szParameterFilePath);
        EhsStrcpy(EhsMetaData->zVersion,"ehs-n/a");
        EhsStrcpy(EhsMetaData->zBuildDate,"ehs-n/a");
        EhsMetaData->nRepoID=-1; //@todo need formal error enumerator for data not present
        EhsStrcpy(EhsMetaData->zTargetVariant,"ehs-n/a");
        EhsStrcpy(EhsMetaData->zModuleList,"ehs-n/a");
    }
    else
    {
        EhsFgets(EhsMetaData->zVersion,EHS_VERSION_LENGTH_MAX,EhsVersionFile);
        EhsFgets(EhsMetaData->zBuildDate,EHS_BUILD_DATE_LENGTH_MAX,EhsVersionFile);
        EhsFgets(temp,64,EhsVersionFile); // skip the build tag hash
        if (EhsStrlen(temp)>0  && EhsStrlen(temp)< 64) EhsMetaData->nRepoID=atoi(temp);
        else
        {
            EhsMetaData->nRepoID=-2; //@todo need formal error enumerator for invalid data
        }
        //EhsFgets(EhsMetaData->zTargetVariant,EHS_STRING_LENGTH_MAX,EhsVersionFile); // skip the build tag hash
        EhsFgets(EhsMetaData->zTargetVariant,EHS_TARGET_VARIANT_LENGTH_MAX,EhsVersionFile);
        EhsFgets(EhsMetaData->zModuleList,EHS_MODULE_LIST_LENGTH_MAX,EhsVersionFile);
        EhsFclose(EhsVersionFile);
    }
}


/* EHS control interface @todo review if a new module should contain this..*/

/* Devman specific bits */
/* Device Pairing */
void EhsHAppMetaRequestPairedOrganisation()
{
    //EhsMetaData.PairedOrganisationID = -1 ;
    EhsMetaData.PairedOrganisationIDRequested=2;
    //return EHS_TRUE;
}
/* 0: no pairing data updates, 1 pairing data valid, 2 pairing data pending, 3 paring data invalid*/
/* set the pairing ID */
void EhsHSysUpdatePairedOrganisation(ehs_uint32 id)
{
    //EhsMetaData.PairedOrganisationIDRequeste=0;
    if (id == 0xFFFF || id == 0)
    {
        EhsMetaData.PairedOrganisationIDRequested=3; /* invalid data */
    }
    else
    {
        EhsMetaData.PairedOrganisationID= id;
        EhsMetaData.PairedOrganisationIDRequested=1; /* valid data */
    }
    //return EHS_TRUE;
}

/* return true of the device is definately paired */
ehs_uint8 EhsHSysGetPairedOrganisationStatus()
{
    return EhsMetaData.PairedOrganisationIDRequested;
}

ehs_uint32 EhsHSysGetPairedOrganisation()
{
    //EhsMetaData.PairedOrganisationIDRequeste=0;
    return EhsMetaData.PairedOrganisationID;
}



/********************** Requires target Sepcific support ***********************/

/* Date in W3c format */
ehs_bool EhsHSysUpdateDate(ehs_char * datestring)
{
    //@todo move code to the target specific implementation
    EHSH_LOG_WARNING("EhsHSysUpdateDate(%s) - Not Implemented", datestring);
    return EHS_TRUE;
}

#if 0
/* expects =setting in json form: { ipaddr="x.x.x.x", gateway="x.x.x.x", mask="x.x.x.x", dns1="x.x.x.x" */
ehs_sint8 EhsSysSetStaticIpV4Addr(ehs_char * json)
{
    const ehs_char ipaddr_label[] = "ipaddr:\"";
    const ehs_char gateway_label[] = "gateway:\"";
    const ehs_char mask_label[] = "mask:\"";
    const ehs_char dns1_label[] = "dns1:\"";
    const ehs_uint8 min_addr_str_len = 7;
    ehs_char zStrIpaddr[ 16];
    ehs_char zStrGateway[ 16];
    ehs_char zStrMask	[ 16];
    ehs_char zStrDns1	[ 16];
    ehs_char * next = json;
    ehs_char * found = NULL;
    if (json == NULL) return -1 ;
    found = EhsStrstr(next, "ipaddr:\"");
    if (found)
    {
        if (EhsStrlen(found) > EhsStrlen(ipaddr_label)+min_addr_str_len)
        {
            EhsStrcpyUIpto UpTo(zStrIpaddr,6)


        }

#endif

        ehs_bool EhsHSysSetResetTime(ehs_char * datestring)
        {
            //@todo
            /* 		devmanCronFile = io.open("../sysdata/devman.cron", "w");
             devmanCronFile:write(time.." /root/ehs/bin/reboot.sh > /dev/null\n"); -- this is unix specific.
             devmanCronFile:close();

             */
            EHSH_LOG_WARNING("EhsHSysSetResetTime(%s) - Not Implemented", datestring);
            return EHS_TRUE;
        }

        /* We may have variations on this that will delete only directories within */
        ehs_bool EhsHFileClearUserData(ehs_char *time_string_floating_point)
        {

            Ehs_UserRm("playlist.smil");
            Ehs_UserRmdir("media");
            /*
             * if string.find(response, "") then
             for temp in string.gmatch(response, "<ClearStorage>(.-)<ClearStorage/>") do
             days = temp;
             end
             if days == "0" then
             --rmdir("../userdata/media"); -- todo this should only remove all content not the directory.*/
            //os.execute("rm -Rf ../userdata/media/*");
            /*else
            days = days - 1;
            print("days="..days);
            os.execute("find ../userdata/ -atime +"..days.." | grep -v '../userdata/$' | xargs rm -rf");
            end
            end
            -- restart ehs
            */
            EHSH_LOG_WARNING("EhsHFileClearUserData(%s) - Not Fully Implemented", time_string_floating_point);
            return EHS_TRUE;
        }

        /* Sets a flag that EHS should close and restart (forcibly if necessary via WDT). */

        ehs_bool EhsHSysReLoad()
        {
            return EHS_TRUE;
        }

        /* EHS FSM control functions - to be used by function blocks or target specific components to handle host app lifecycles requirements */

        ehs_bool EhsHRequestEHSInterrupt()
        {
            if (EhsMetaData.InternallyRequestedCommand !=	EHS_CONTINUE)
            {
                return EHS_TRUE;
            }
            else
            {
                return EHS_FALSE;
            }
        }




        /* This returns the requested state and sets it back to the default "continue" command */
        Ehs_ConsoleCommand_Type EhsHFSMGetInternallyRequestedCommand()
        {
            Ehs_ConsoleCommand_Type state = EhsMetaData.InternallyRequestedCommand;
            EhsMetaData.InternallyRequestedCommand=	EHS_CONTINUE;
            return state;
        }


        /* Tell EHS to change state
         * This can bounce the request and return false if it thinks the current request is more important
         */
        ehs_bool EhsHFSMSetInternallyRequestedCommand(Ehs_ConsoleCommand_Type state)
        {
            if (EhsMetaData.InternallyRequestedCommand == EHS_CONTINUE)   //Any other request is more important!
            {
                EhsMetaData.InternallyRequestedCommand = state;
                EHSH_LOG_INFO("Setting command to run next time");
                return EHS_TRUE;
            }
            else
            {
                return EHS_FALSE;
            }
        }

        /* don't check the current state is not EHS_CONTINUE */
        void EhsHFSMForceInternallyRequestedCommand(Ehs_ConsoleCommand_Type state )
        {

            EhsMetaData.InternallyRequestedCommand = state;
        }



        /**
         * Shut down everything
         * @todo remove this function there are loads if EHS shutdowns!!
         *
         */
        void EhsExit(ehs_uint16 exitCode)
        {
            // todo attempt to do nice things with toolkits (e.g.g shutdown gtk, close sockets etc.)
            static ehs_bool bExited = EHS_FALSE; // don't let EhsHSys_term() run twice!
#ifdef EHS_DEBUG_TCPIP_CONSOLE // todo2022 this should be a higher level comms close down - not just the console?
            EhsSvcTcp_closeConnection();
#endif //EHS_COMMS_API_SUPPORT
            if (!bExited)
            {
                bExited = EHS_TRUE;
                EhsHSys_term();
            }
            EHSH_LOG_WARNING("Terminated System - exiting with exit code %d",exitCode);

            EhsTargetExit(exitCode);
            //*todo we should add a flag that the OS uses not to respawn EHS when it runs persistently.
        }


        ehs_bool EhsHSysRestart()
        {
            //@todo
            EhsExit(0);
            return EHS_TRUE;
        }

        ehs_bool EhsHSysReboot()
        {
            //@todo
            EHSH_LOG_ERROR("EhsHSysReboot() - Not Implemented");
            // Use Exec here so that a bash environment is not started. (this should run the reboot script in /bin.
            return EHS_TRUE;
        }

        /*********************** End of System services that can be called from components ********************************/


        /**
         * Called to initialise the HAL during system initialisation time
         * @todo this should be in a separate kernel HAL - not exposed to components.
         * @todo - complex init sequence that is required to be run in predefined order, need to add a "init level" to each part of initialisation and test to see of at correct level before run next part of initialisation
         */



        /**
         * \brief Populates OS/install meta data for the EHS instance.
         *
         */
        ehs_bool EhsHOsSys_UpdateEnvironment()
        {
            return EhsHUpdateFilePathEnvironment(&EhsMetaData);
        }

        /**
         * Called to shutdown the HAL prior to shutting down EHS
         *
         * @todo the following should be common for all targets
         * @todo the component specific ones need to be moved to toolboxes
         */
        void EhsHSys_term(void)
        {

            /* Shutdown the application & component threads*/
            EhsHApp_term();
            EHSH_LOG_INFO("Terminated Application");
            /* Shutdown the system */
            EhsTOsSys_term();
            EHSH_LOG_INFO("Terminated Application");
#ifdef EHS_COMMS_API_SUPPORT
            EhsTCommsSys_term();
#endif //EHS_COMMS_API_SUPPORT
#ifdef EHS_GUI_SUPPORT
            EhsTGfxSys_term();
            EHSH_LOG_INFO("Terminated Graphics Subsystem");
#endif
#ifdef EHS_AV_SUPPORT
#ifdef EHS_VIDEO_SUPPORT
            EhsTVideoSys_term();
            EHSH_LOG_INFO("Terminated A/V Subsystem");
#endif
#endif
        }

        void EhsHSys_init()
        {
            EhsHMem_init(); /* this function has no pre-requisites: it should be called first */

            /* Create basic file syste if the installer hasn't */
            EhsFInitFileSystem();

            EhsHLogger_init(); /* initialise the system Logger */
            /* EHSH_LOG_LEVEL_ERROR|EHSH_LOG_LEVEL_WARNING|EHSH_LOG_LEVEL_INFO|EHSH_LOG_LEVEL_ENTER|EHSH_LOG_LEVEL_EXIT */
            EhsHSetLogLevels();
            EhsHInitEhsMetaData(); //Make sure EHS_DEVMAN_SUPPORTthe pltform Meta data file is all blanked out
#ifdef EHS_NETWORKING_SUPPORT
            EhsHURLGlobalInit();
#endif

            EhsTOsSys_init(); /* initialise the Operating System */

#ifdef EHS_DEBUG_TCPIP_CONSOLE
            //printf("Starting TCPIP CONSOLE thread\n");
            EhsHThread_execute(EhsSvcTcp_server, NULL, EHS_PRI_TCP_IP_CONSOLE, EHS_DEBUG_CONSOLE_THREAD_STACK_SIZE);//-90); //////// CHANGES ONLYA
            //printf("Started TCPIP CONSOLE thread\n");
#endif
            EhsHGetEHSVersionInfo(&EhsMetaData); /*Populate the version information table */
            /* Path info is defined at first start before any cds etc */
            EhsHOsSys_UpdateEnvironment(); /*Call generic OS code e.g. user data path */
            EhsTOsSys_UpdateEnvironment(&EhsMetaData,0); /*Call target specific Dynamic infor e.g. Network ID, etc. */
#ifdef EHS_COMMS_API_SUPPORT
            EhsTCommsSys_init();
#endif //EHS_COMMS_API_SUPPORT
            EhsBinSearchPath(); /* Add a search path to bin (@todo Still needed?) */

            //@todo ALL THIS SHOULD BE MOVED TO THE MODULE INITIALISATION CODE
#if defined(EHS_DEVMAN_SUPPORT) && (EHS_DEVMAN_SUPPORT != EHS_DEVMAN_NONE)
            DevmanMon_init(); // Non Component devman functions are considered part of the HAL
#endif
#ifdef EHS_LUA_SUPPORT
//#ifdef LUA scripting component
            EhsHLuaInit() ; // sets up the environment path for its utilities
#endif

            //@todo move these to the function-block-space initialisers
#ifdef EHS_GUI_SUPPORT
            // @todo move to widget initialisation
            EhsTGfxSys_init();
#endif
#ifdef EHS_AV_SUPPORT
#ifdef EHS_VIDEO_SUPPORT
            EhsTVideoSys_init();
#endif
#endif
            /* set flag to indicate to other threads that EHS is now in business */

            EhsMetaData.PairedOrganisationIDRequested=0; //was 2 for some reason?
#if defined(EHS_DEVMAN_SUPPORT) && (EHS_DEVMAN_SUPPORT != EHS_DEVMAN_NONE)
            EhsHMetaSetNewDevmanMiscDLDataNew(EHS_FALSE);
            ehs_char* data = EhsHMetaGetPtrToDevmanMiscDLData();
            data[0]='\0';
            data = EhsHMetaGetPtrToDevmanMiscDLDataType();
            data[0]='\0';
#endif
            EhsL_initComplete = EHS_TRUE;
        }


        /**
         * Called to prepare the HAL for loading in a new application.
         * @todo any of the below which are purely used in function blocks should be moved to
         * function block initialisation system. FB shred init system TBC!
         */
        void EhsHApp_init()
        {
            EhsTOsApp_init(); /* Target Operating System initialisation */
#ifdef EHS_COMMS_API_SUPPORT
            EhsTCommsApp_init();/* Console comms interface - for debugging */
#endif //EHS_COMMS_API_SUPPORT
#ifdef EHS_GUI_SUPPORT
            EhsTGfxApp_init();
#ifndef EHS_DONT_USE_BASIC_FONTS
            EhsGraphicsFontTable_init();
#endif
            EhsWidgetViewport_default_config();//Clear the old viewport parameters in case the new one has no widget for it
#endif
#ifdef EHS_AV_SUPPORT
#ifdef EHS_VIDEO_SUPPORT
            EhsTVideoApp_init();
#endif
#endif
        }

        /**
         * Called to prepare the HAL to start executing an application (or to reset
         * an existing application). This needs to preserve the SODL and init data.
         */
        void EhsHApp_reset()
        {
            /* clear up from earlier phase */
            EhsTOsApp_reset(); //doesn't so anything currently
#ifdef EHS_COMMS_API_SUPPORT
            EhsTCommsApp_reset(); //doesn't so anything currently
#endif
#ifdef EHS_GUI_SUPPORT
            EhsTGfxApp_reset(); //doesn't so anything currently
#ifndef EHS_DONT_USE_BASIC_FONTS
            EhsGraphicsFontTable_init(); // the memory is wiped by the garbage collector.
#endif
            EhsWidgetViewport_default_config();//Clear the old viewport parameters in case the new one has no widget for it
#endif
#ifdef EHS_AV_SUPPORT
#ifdef EHS_VIDEO_SUPPORT
            EhsTVideoApp_reset(); //doesn't so anything currently
#endif
#endif
            /* entered new phase - start by switching memory pool - this clears the memory allocated for the app! */
            EhsHMem_switchPool(EHSHM_POOL_APP_DATA); //clears memory
#ifdef EHS_GUI_SUPPORT
            //EhsGraphicsFontTable_init();
#endif
        }


        /* Return when the target signals all is good to go
         * timeout in seconds or -1 for wait for ever
         * todo review if passing void (*target_loop_iteration)(void *),void * target_env_blob is better than just having these as global variables
         * */
        void EhsHWaitForTarget(Ehs_ConsoleCommand_Type (*target_loop_iteration)(void *),void * target_env_blob,ehs_sint16 timeout)
        {
            ehs_sint32 i=0;
            ehs_bool contin=EHS_TRUE;
            while (contin)
            {
                if (EhsTPlatformReady( (void*) target_loop_iteration,target_env_blob) ) break;
                if (timeout>0 && i> timeout*10) break;
                i=1+1;
                EhsSleep(EHS_TIME_ms(100)); //sleep for 100ms
            }
            // where do we do this - if at all - pthread cond destroy
        }



        /**
         * Called to activate the thread to show the app
         */
        void EhsHApp_show(void)
        {
#ifdef EHS_GUI_SUPPORT
            EhsTV_show(&EhsTV);
#endif
        }

        /**
         * Called to clean up after the execution of an application in order to make
         * sure everything has been stopped.
         */
        void EhsHApp_term(void)
        {
            EhsTOsApp_term();
            //EhsTCommsApp_term();  we don't want to kill the comms for an app do we?
            /* This is done by the system shutdown only we don't terminate these during an App term
            	#ifdef EHS_GUI_SUPPORT
            	EhsTGfxApp_term();
            #endif
            #ifdef EHS_AV_SUPPORT
            #ifdef EHS_VIDEO_SUPPORT
            	EhsTVideoApp_term();
            #endif
            #endif
            */
        }

ehs_uint32 EhsConsoleQueue_maxSize(){
    return EHS_DEBUG_CONSOLE_BUFFER_SIZE;
}
