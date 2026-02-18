/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file hal_logger.c
 * Implementation of the logger support
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
#define EHSL_MODULE_ID (EHSH_LOG_MODULE_LOGGER) /* define before hal_logger.h */

#include "globals.h"
#include "hal_logger.h"
#include "hal_file.h"
#include "hal_time.h"
#include "hal_string.h"
#include "messages.h"

//#ifdef EHS_ANDROID  this is only needed in the hal_logger.h file?
//#include <android/log.h>
//#endif

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/* With/Without file logging code */
//#define EHS_RUNTIME_FILELOGGER_ENABLED - @todo this should be specifed in the platform
/* With/Without STDIO logging code */
#define EHS_LOG_TO_STDIO

/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */
#ifdef EHS_RUNTIME_LOGGER_ENABLED
ehs_FILE* EhsLLogFile=NULL;
//ehs_bool bLogToFile = EHS_TRUE; //we will just use the file handle status

/* set to max logging before the init function is called */
EhsHLoggerLogLevel nLogLevel = EHSH_LOG_DEFAULT_LEVEL; 

//Set the maximum global verbosoty to all if there is on config.mk override.
#ifndef EHSH_LOG_MAX_GLOBAL_LEVEL
#define EHSH_LOG_MAX_GLOBAL_LEVEL EHSH_LOG_LEVEL_ALL
#endif



/* NOTE THIS NEEDS TO BE KEPT IN SYNC WITH THE ENUM TYPE IN THE HEADER!!!!*/
/*
 * typedef enum {
	EHSH_LOG_MODULE_UNDEFINED,
	EHSH_LOG_MODULE_KERNEL,
	EHSH_LOG_MODULE_GRAPHICS,
	EHSH_LOG_MODULE_LOGGER,
	EHSH_LOG_MODULE_HAL_MEMORY,
	EHSH_LOG_MODULE_HAL_PROCESS,
	EHSH_LOG_MODULE_HAL_STRING,
	EHSH_LOG_MODULE_TGT_VIEWPORT,
	EHSH_LOG_MODULE_HAL_NETWORK,
	EHSH_LOG_MODULE_HAL_DEVMANMON,
	EHSH_LOG_MODULE_HAL_FILE,
	EHS_LOG_MODULE_QUANTITY
} EhsHLoggerModuleId;
 *
 */

/* The following are names that prefixed on the error messages (presumably) */
/* NOTE THIS NEEDS TO BE KEPT IN SYNC WITH THE ENUM TYPE IN THE HEADER!!!!*/

ehs_char* EhsLModuleNames[] =
{
    "Undefined",
    "kernel",
    "Graphics",
    "Logger",
    "HalMemory",
    "HalProcess",
    "HalString",
    "TgtViewport",
    "Network",
    "Devman",
    "file",
    NULL
};
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

    // Not verbose mode - defaults to ERROR only per module.
    // Override individual modules by defining EHS_LOG_LEVEL_<Module> in config.mk, e.g.:
    //   DEFS += EHS_LOG_LEVEL_GRAPHICS=0x07    (ERROR|WARNING|INFO)
    //   DEFS += EHS_LOG_LEVEL_NETWORK=0x04     (INFO only)

#ifndef EHS_LOG_LEVEL_UNDEFINED
#define EHS_LOG_LEVEL_UNDEFINED    EHSH_LOG_LEVEL_ERROR
#endif
#ifndef EHS_LOG_LEVEL_KERNEL
#define EHS_LOG_LEVEL_KERNEL       EHSH_LOG_LEVEL_ERROR
#endif
#ifndef EHS_LOG_LEVEL_GRAPHICS
#define EHS_LOG_LEVEL_GRAPHICS     EHSH_LOG_LEVEL_ERROR
#endif
#ifndef EHS_LOG_LEVEL_LOGGER
#define EHS_LOG_LEVEL_LOGGER       EHSH_LOG_LEVEL_ERROR
#endif
#ifndef EHS_LOG_LEVEL_HALMEMORY
#define EHS_LOG_LEVEL_HALMEMORY    EHSH_LOG_LEVEL_ERROR
#endif
#ifndef EHS_LOG_LEVEL_HALPROCESS
#define EHS_LOG_LEVEL_HALPROCESS   EHSH_LOG_LEVEL_ERROR
#endif
#ifndef EHS_LOG_LEVEL_HALSTRING
#define EHS_LOG_LEVEL_HALSTRING    EHSH_LOG_LEVEL_ERROR
#endif
#ifndef EHS_LOG_LEVEL_TGTVIEWPORT
#define EHS_LOG_LEVEL_TGTVIEWPORT  EHSH_LOG_LEVEL_ERROR
#endif
#ifndef EHS_LOG_LEVEL_NETWORK
#define EHS_LOG_LEVEL_NETWORK      EHSH_LOG_LEVEL_ERROR
#endif
#ifndef EHS_LOG_LEVEL_DEVMAN
#define EHS_LOG_LEVEL_DEVMAN       EHSH_LOG_LEVEL_ERROR
#endif
#ifndef EHS_LOG_LEVEL_FILE
#define EHS_LOG_LEVEL_FILE         EHSH_LOG_LEVEL_ERROR
#endif

    void EhsHSetLogLevels()
    {
        EhsHLogger_setLogLevel("Undefined",   EHS_LOG_LEVEL_UNDEFINED);
        EhsHLogger_setLogLevel("Kernel",      EHS_LOG_LEVEL_KERNEL);
        EhsHLogger_setLogLevel("Graphics",    EHS_LOG_LEVEL_GRAPHICS);
        EhsHLogger_setLogLevel("Logger",      EHS_LOG_LEVEL_LOGGER);
        EhsHLogger_setLogLevel("HalMemory",   EHS_LOG_LEVEL_HALMEMORY);
        EhsHLogger_setLogLevel("HalProcess",  EHS_LOG_LEVEL_HALPROCESS);
        EhsHLogger_setLogLevel("HalString",   EHS_LOG_LEVEL_HALSTRING);
        EhsHLogger_setLogLevel("TgtViewport", EHS_LOG_LEVEL_TGTVIEWPORT);
        EhsHLogger_setLogLevel("Network",     EHS_LOG_LEVEL_NETWORK);
        EhsHLogger_setLogLevel("Devman",      EHS_LOG_LEVEL_DEVMAN);
        EhsHLogger_setLogLevel("file",        EHS_LOG_LEVEL_FILE);
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

/**
 * Indicates the current log level for each moduel
 */
EhsHLoggerLogLevel EhsHLoggerModuleLogLevel[EHS_LOG_MODULE_QUANTITY];

/* Variables defined with global-scope */
#ifdef EHS_RUNTIME_LOGGER_ENABLED
/**
 * Temporary string used by EHSH_LOG_ macros to write message
 */
ehs_char EhsHLogger_Msg[EHSH_LOG_MAX_MSG];
#endif

/*****************************************************************************/
/* Function definitions */

ehs_char* EhsHLogger_Buffer()
{
#ifdef EHS_RUNTIME_LOGGER_ENABLED
    return EhsHLogger_Msg;
#else
    return NULL;
#endif
}

/**
 * Initialise the logger subsystem
 */
void EhsHLogger_init()
{
#ifdef EHS_RUNTIME_LOGGER_ENABLED
    ehs_uint16 nId;
    //nLogLevel = EHSH_LOG_DEFAULT_LEVEL; @todo this should be set by something sensible.
#ifdef EHS_RUNTIME_FILELOGGER_ENABLED
//@todo need to add file renaming function here to keep a rolling list of old log files.
    EhsLLogFile = Ehs_SysFopen(EHSH_LOG_FILENAME,"w"); /* if sysdata doesn't exist create in default directory */
    if (!EhsLLogFile)
    {
        //nLogLevel = 0; /* no logging to file */
        EhsConsolePrintf("**Error: Can't open log file\n");
    }
    else
    {
        fprintf(EhsLLogFile,"********************************************\n");
        fprintf(EhsLLogFile,"time,log level,module,filename:line,message\n");
    }
#else
    EhsLLogFile = NULL;
#endif//FILE LOGGER

    /* Continue anyway and reset log levels */
    for (nId = 0; nId < EHS_LOG_MODULE_QUANTITY; nId++)
    {
        EhsHLoggerModuleLogLevel[nId] = EHSH_LOG_DEFAULT_LEVEL;
    }
    /* consistency check between EhsLModuleNames and EhsHLoggerModuleId */
    for (nId = 0; EhsLModuleNames[nId]; nId++ )
    {
    }
    if (nId != EHS_LOG_MODULE_QUANTITY)
    {
        EHSH_LOG_ERROR(EHSH_LOGGER_INVALID_NAMES_TABLE);
    }
#endif
}

/**
 * Record a log entry.
 * This wil ltypically by called from a header macro that will have picked up the nModule name from the #define above the includes.
 * \yhe nLevel would be set by another macro:
 */
void EhsHLogger_log(EhsHLoggerModuleId nModule, EhsHLoggerLogLevel nLevel, const ehs_char* szFilename, ehs_uint32 nLine, const ehs_char* szMsg)
{
#ifdef EHS_RUNTIME_LOGGER_ENABLED

    /* Note this function must be quick as it can be called from blocking functions */

    /* if no valid module value is provided we switch to undefined - and log this... i.e. default is on*/
    if (nModule < EHSH_LOG_MODULE_UNDEFINED) nModule = EHSH_LOG_MODULE_UNDEFINED;
    if (nModule > EHS_LOG_MODULE_QUANTITY) nModule = EHSH_LOG_MODULE_UNDEFINED;

    ehs_uint32 time=0;
    //EhsStdioPrintf("LOGGING time=%d,level=%s nMod=%d nLevel%d] %s",0,"somelevel", nModule,nLevel,szMsg);
    const char* szLevel;
    //const char* szModule;
    #ifdef EHSH_LOG_MAX_GLOBAL_LEVEL
    if (!(nLevel & EHSH_LOG_MAX_GLOBAL_LEVEL)) /* Check the global filter first */
    {
        goto end;
    }
    #endif
     if (!(nLevel & nLogLevel)) /* Check the module filter*/
    {
        goto end;
    }

    switch (nLevel)
    {
    case EHSH_LOG_LEVEL_ERROR:
        szLevel = "Error";
        break;
    case EHSH_LOG_LEVEL_WARNING:
        szLevel = "Warning";
        break;
    case EHSH_LOG_LEVEL_INFO:
        szLevel = "Info";
        break;
    case EHSH_LOG_LEVEL_ENTER:
        szLevel = "Enter";
        break;
    case EHSH_LOG_LEVEL_EXIT:
        szLevel = "Leave";
        break;
    default:
        szLevel = "undefined";
        break;
    }
    if (nModule < EHS_LOG_MODULE_QUANTITY)
    {
        //szModule = EhsLModuleNames[nModule];
    }
    else
    {
        //szModule = "undefined";
        nModule = EHSH_LOG_MODULE_UNDEFINED;
    }
#ifdef EHS_RUNTIME_FILELOGGER_ENABLED
    if (EhsLLogFile) { /* Don't want all that crashing do we!! */ //@todo need to put this in sysinfo/var
        time=EhsTgtTimer_now(); /* Note this is in ticks not real time....*/
        EhsFprintf(EhsLLogFile, "%u,",time);
        EhsFprintf(EhsLLogFile, "%s,",szLevel);
        EhsFprintf(EhsLLogFile, "%s,",EhsLModuleNames[nModule]);
        EhsFprintf(EhsLLogFile, "%s:%d,",szFilename,nLine);
        EhsFprintf(EhsLLogFile, "\"%s\"\n",szMsg);
        EhsFflush(EhsLLogFile);
    }
#endif

#ifdef EHS_LOG_TO_STDIO
    if (szLevel && nModule > 0 && nModule < 11 && szFilename && nLine && szMsg)   //to do make this nModule check better!
    {
        EhsStdioPrintf("\n[%u][%s][%s][%s]:%d:\"%s\"",time,szLevel,EhsLModuleNames[nModule],szFilename,nLine,szMsg);
    }
    else
    {
        if (szMsg)
        {
        }
        else
        {
        }
    }
#endif
end:
    ;
#endif
}

/** todo remove this - just use the enums in the module names???
 * Indicate what we want to log
 * @param[in] szModule Name of the module to log
 * @param[in] nLevels Set of the levels we wish to log (or'd together)
 */

ehs_bool EhsHLogger_setLogLevel(const ehs_char* szModule, EhsHLoggerLogLevel nLevels)
{
#ifdef EHS_RUNTIME_LOGGER_ENABLED
    ehs_uint16 nId;
    ehs_bool bRet = EHS_FALSE; /* we haven't found the module yet */

    for (nId = 0; nId < EHS_LOG_MODULE_QUANTITY; nId++)
    {
        if (EhsStrcmp(szModule, EhsLModuleNames[nId]) == 0)
        {
            EhsHLoggerModuleLogLevel[nId] = nLevels;
            EHSH_LOG_INFO("Setting Log level %s to %d",szModule,nLevels);
            bRet = EHS_TRUE;
            break;
        }
    }
    if (!bRet) EHSH_LOG_ERROR("Could not set log level %x for module %s",nLevels,szModule);
    return bRet;
#endif
    return EHS_TRUE;
}
