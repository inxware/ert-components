/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file logger.h
 * Declares the interface for the logger module.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_HAL_LOGGER_H
#define EHS_HAL_LOGGER_H

/*****************************************************************************/
/* Included files */
#include "ehs_types.h"
#include "hal_string.h"
#include "target_config.h"
#include "target_specific.h"
#include "globals.h"
/*****************************************************************************/
/* Define macros  */
/**
 * Filename used for logging
 */
#define EHSH_LOG_FILENAME "ehs_log.000" /* This is to be postpended with an index */

/**
 * Maximum number of entries in a log file
 */
#define EHSH_LOG_MAX_MSG 2048u

/**
 * Maximum number of indexed log files
 */
//#define EHSH_LOG_MAX_MSG 4u


/**
 * Maximum length of a message sent to the logger
 */
//#define EHSH_LOG_MAX_MSG 256u

/**
 * Default log level
 */
#define EHSH_LOG_DEFAULT_LEVEL (EHSH_LOG_LEVEL_ERROR | EHSH_LOG_LEVEL_WARNING)
//#define EHSH_LOG_DEFAULT_LEVEL (EHSH_LOG_DEFAULT_LEVEL | EHSH_LOG_LEVEL_INFO | EHSH_LOG_LEVEL_ENTER | EHSH_LOG_LEVEL_EXIT)


/**
 * All log levels
 */
#define EHSH_LOG_ALL_LEVEL (EHSH_LOG_DEFAULT_LEVEL | EHSH_LOG_LEVEL_INFO | EHSH_LOG_LEVEL_INFO | EHSH_LOG_LEVEL_ENTER| EHSH_LOG_LEVEL_EXIT)

/*****************************************************************************/
/* Define types */

/**
 * Specifies the level at which to log information
 */
typedef enum
{
    EHSH_LOG_LEVEL_ERROR	= 0x01,
    EHSH_LOG_LEVEL_WARNING	= 0x02,
    EHSH_LOG_LEVEL_INFO		= 0x04,
    EHSH_LOG_LEVEL_ENTER	= 0x08,
    EHSH_LOG_LEVEL_EXIT		= 0x10
} EhsHLoggerLogLevel;


/**
 * Specifies module ids for specific logging.
 * *************************************************
 * Don't forget to add
 * module text name in EhsLModuleNames in logger.c
 * **************************************************
 */
typedef enum
{
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
    EHS_LOG_MODULE_QUANTITY /* final entry - used to determine size of EhsLModuleNames */
} EhsHLoggerModuleId;



#ifdef EHS_BUILDOPT_STDIO_MESSAGE_TRACE
#define EHS_TRACE_MESSAGE(x,...)  EhsStdioSimplePrintf(x,##__VA_ARGS__) // THis is for random text
#define EHS_TRACE_FUNCTION(x)   EhsStdioSimplePrintf("%s\n",#x)   //this is for stringifying function names for Enter tracing
#else
#define EHS_TRACE_FUNCTION(x)
#define EHS_TRACE_MESSAGE(...)
#endif


/**
 * Is the current module logging a specific level?
 * @param[in] nLevel logging level to check
 * @return true if current module is logging nLevel
 */

#ifdef EHSL_MODULE_ID
#define EHSH_LOG_CHECK(nLevel) (((nLevel) && EhsHLoggerModuleLogLevel[EHSL_MODULE_ID]) & (nLevel)) //(1) /* bit check against module for the requested log level */
#else
#define EHSL_MODULE_ID EHSH_LOG_MODULE_UNDEFINED
#define EHSH_LOG_CHECK(nLevel) 0
#endif

/**
 * Returns EhsHLogger_Msg buffer
 */
EHS_GLOBAL ehs_char* EhsHLogger_Buffer();


//#define EHSH_LOG_MESSAGEX(nLevel,...) {}
/**
 * Writes debug standard out and any debug console that's listening.
 * Generate a specific log message (if we're logging at that level)
 * @todo the following is not thread safe - try with local variable in curlies
 * @todo the else function here is for debug this needs to be dumped... This F*ing logging code has wasted hours....
 */
#if defined(EHS_RUNTIME_LOGGER_ENABLED) && defined(EHSL_MODULE_ID)
 #ifdef EHS_ANDROID
  #define EHSH_LOG_MESSAGE(nLevel,...) if(EHSH_LOG_CHECK(nLevel)){LOGE(__VA_ARGS__);}
 #else
  /* todo  consider another build option macro for not including the __FILE__ and  __LINE__ test on the log line to save code sizes of debug builds*/
  #define EHSH_LOG_MESSAGE(nLevel,...) if(EHSH_LOG_CHECK(nLevel) && EhsHLogger_Buffer()){EhsSnprintf(EhsHLogger_Buffer(),EHSH_LOG_MAX_MSG,__VA_ARGS__);EhsHLogger_log(EHSL_MODULE_ID,nLevel,__FILE__,__LINE__,EhsHLogger_Buffer());}
 #endif
//else{EhsSprintf(EhsHLogger_Msg,__VA_ARGS__); EhsHLogger_log(EHSL_MODULE_ID,nLevel,__FILE__,__LINE__,EhsHLogger_Msg);}
#else
#define EHSH_LOG_MESSAGE(nLevel,...); {}
#endif
/**
 * Macros intended for users of the logger (require definition of EHSL_LOG_LEVEL_xxx
 */

#ifdef EHS_RUNTIME_LOGGER_ENABLED
#define EHSH_LOG_ERROR(...) 		EHSH_LOG_MESSAGE(EHSH_LOG_LEVEL_ERROR,__VA_ARGS__)
#define EHSH_LOG_WARNING(...) 	EHSH_LOG_MESSAGE(EHSH_LOG_LEVEL_WARNING,__VA_ARGS__)
#define EHSH_LOG_INFO(...) 		EHSH_LOG_MESSAGE(EHSH_LOG_LEVEL_INFO,__VA_ARGS__)
#define EHSH_LOG_ENTER(...) 		EHSH_LOG_MESSAGE(EHSH_LOG_LEVEL_ENTER,__VA_ARGS__)
#define EHSH_LOG_EXIT(...) 		EHSH_LOG_MESSAGE(EHSH_LOG_LEVEL_EXIT,__VA_ARGS__)
#else
#define EHSH_LOG_ERROR(...)
#define EHSH_LOG_WARNING(...)
#define EHSH_LOG_INFO(...)
#define EHSH_LOG_ENTER(...)
#define EHSH_LOG_EXIT(...)

#endif
/*lint -emacro( (534), EhsError ) Ignore return code of printf here */
//#define EhsError(err) {EhsConsolePrintf(err); EHSH_LOG_ERROR(err);}
//#define EhsWarning(err) {EhsConsolePrintf(err); EHSH_LOG_WARNING(err);}
//@todo for legacy support
//#define EhsError(...) EHSH_LOG_ERROR(__VA_ARGS__) //@todo get rid of this version - use the EHSH version
//#define EhsWarning(...) EHSH_LOG_WARNING(__VA_ARGS__) //@todo get rid of this version - use the EHSH version

/* Compile time switched tracing function */
#ifdef EHS_BUILDOPT_STDIO_ENABLE_FUNCTION_TRACING

/**
 * Used in running debugging EHS via tracing calls. This represents a bitmask
 * of the subsystems that are currently being traced. 0 means trace is disabled.
 */
/* TOdo all these trasing functions - at elast the ernel ones, should go in the kernel code-base*/
EHS_GLOBAL ehs_uint32 EhsTraceFlags;

#define EHS_TRACE_FLAG_ATOM 0x8000 /**< Applies to functions that are called at the lowest level: bytes (file handling), tokens (parser), events (kernel) ... */
#define EHS_TRACE_FLAG_ITEM 0x4000 /**< Applies to items at the next level: lines (file handling), statements (parser),...*/
#define EHS_TRACE_FLAG_GROUP 0x2000 /**< Applies to "group" level items: function blocks (parser) ... */
#define EHS_TRACE_FLAG_TOPLEVEL 0x1000

#define EHS_TRACE_FLAG_HAL_FILE 0x0001
#define EHS_TRACE_FLAG_KERNEL 0x0010
#define EHS_TRACE_FLAG_PARSER 0x0020
#define EHS_TRACE_FLAG_GLOBAL 0x0800 /* unspecified functions */

#define EHS_TRACE_FLAG_SET(x) (((x) & EhsTraceFlags) == (x))

/* @todo the following should use __VAR_ARGS__ - */
/**
 * Conditional trace a function with no args @todo these need simplifying to a single __VA_ARG__ type */
/*lint -save -e960 -e961 #/## used only in debug build */
#define EHS_TRACE_FUNC0(subsys,name) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s()\n\r",#name); }
//todo Use the var args istead of all these???
/** Conditional trace a function with one argument */
#define EHS_TRACE_FUNC1(subsys,name,fmt,A) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s("fmt")\n\r",#name,A); }
/** Conditional trace a function with two arguments */
#define EHS_TRACE_FUNC2(subsys,name,fmt,A,B) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s("fmt")\n\r",#name,A,B); }
/** Conditional trace a function with three arguments */
#define EHS_TRACE_FUNC3(subsys,name,fmt,A,B,C) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s("fmt")\n\r",#name,A,B,C); }
/** Conditional trace a function with four arguments */
#define EHS_TRACE_FUNC4(subsys,name,fmt,A,B,C,D) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s("fmt")\n\r",#name,A,B,C,D); }

/** Conditional trace a function with one number argument */
#define EHS_TRACE_FUNCn(subsys,name,A) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s(%d)\n\r",#name,A); }
/** Conditional trace a function with one pointer argument */
#define EHS_TRACE_FUNCp(subsys,name,A) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s(%x)\n\r",#name,A); }
/** Conditional trace a function with one string argument */
#define EHS_TRACE_FUNCs(subsys,name,A) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s(%s)\n\r",#name,A); }
/** Conditional trace a function with arguments: pointer, string */
#define EHS_TRACE_FUNCpp(subsys,name,A,B) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s(%x,%x)\n\r",#name,A,B); }
/** Conditional trace a function with arguments: pointer, string */
#define EHS_TRACE_FUNCps(subsys,name,A,B) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s(%x,%s)\n\r",#name,A,B); }
/** Conditional trace a function with arguments: string, string */
#define EHS_TRACE_FUNCss(subsys,name,A,B) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s(%s,%s)\n\r",#name,A,B); }
/** Conditional trace a function with arguments: pointer, number, pointer */
#define EHS_TRACE_FUNCpnp(subsys,name,A,B,C) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s(%x,%d,%x)\n\r",#name,A,B,C); }
/** Conditional trace a function with arguments: pointer, string, pointer */
#define EHS_TRACE_FUNCpsp(subsys,name,A,B,C) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s(%x,%s,%x)\n\r",#name,A,B,C); }
/** Conditional trace a function with arguments: pointer, number, pointer */
#define EHS_TRACE_FUNCsnp(subsys,name,A,B,C) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s(%s,%d,%x)\n\r",#name,A,B,C); }
/** Conditional trace a function with arguments: pointer, number, number, pointer */
#define EHS_TRACE_FUNCpnnp(subsys,name,A,B,C,D) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s(%x,%d,%d,%x)\n\r",#name,A,B,C,D); }
/*lint restore */
#else
#define EHS_TRACE_FUNC0(subsys,name)
#define EHS_TRACE_FUNC1(subsys,name,fmt,A)
#define EHS_TRACE_FUNC2(subsys,name,fmt,A,B)
#define EHS_TRACE_FUNC3(subsys,name,fmt,A,B,C)
#define EHS_TRACE_FUNC4(subsys,name,fmt,A,B,C,D)
#define EHS_TRACE_FUNCn(subsys,name,A)
#define EHS_TRACE_FUNCp(subsys,name,A)
#define EHS_TRACE_FUNCs(subsys,name,A)
#define EHS_TRACE_FUNCpp(subsys,name,A,B)
#define EHS_TRACE_FUNCps(subsys,name,A,B)
#define EHS_TRACE_FUNCss(subsys,name,A,B)
#define EHS_TRACE_FUNCpnp(subsys,name,A,B,C)
#define EHS_TRACE_FUNCpsp(subsys,name,A,B,C)
#define EHS_TRACE_FUNCsnp(subsys,name,A,B,C)
#define EHS_TRACE_FUNCpnnp(subsys,name,A,B,C,D)

#endif /* EHS_BUILDOPT_STDIO_ENABLE_FUNCTION_TRACING */


/*****************************************************************************/
/* Declare global variables */

/**
 * Indicates the current log level for each module
 */
EHS_GLOBAL EhsHLoggerLogLevel EhsHLoggerModuleLogLevel[];

#ifdef EHS_RUNTIME_LOGGER_ENABLED
/**
 * Temporary string used by EHSH_LOG_ macros to write message
 */
EHS_GLOBAL ehs_char EhsHLogger_Msg[];
#endif
/*****************************************************************************/
/* Declare function prototypes  */

/**
 * Initialise the logger subsystem
 */
EHS_GLOBAL void EhsHLogger_init();

/**
 * Record a log entry
 */
EHS_GLOBAL void EhsHLogger_log(EhsHLoggerModuleId nModule, EhsHLoggerLogLevel nLevel,
                               const ehs_char* szFilename, ehs_uint32 nLine, const ehs_char* szMsg);

/**
 * Indicate what we want to log
 * @param[in] szModule Name of the module to log
 * @param[in] nLevels Set of the levels we wish to log (or'd together)
 */
EHS_GLOBAL ehs_bool EhsHLogger_setLogLevel(const ehs_char* szModule, EhsHLoggerLogLevel nLevels);

#endif /* EHS_HAL_LOGGER_H */
