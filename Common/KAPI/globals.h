/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file globals.h
 * Declarations of EHS-wide globals.
 *
 * @author: inx limited
 *
 */

/**
 * @page Verification Verification report
 * @section hal_process_h
 * @anchor hal_process_h
 * @subsection misra MISRA compliance:
 * Demonstrated MISRA compliant on
 * Last modified on $Date$
 *
 * This file contained the following derogations to the MISRA standard:
 * - Rule 19.12 Multiple use of #/## operators in macro definition. This is used in a debug build of EHS
 * - Rule 19.13 Use of #/## operator. This is used only in a debug build of EHS
 */

#ifndef EHS_GLOBALS_H
#define EHS_GLOBALS_H

/* Globals need ehs types and we should preferentially #include globals.h*/
#include "target_types.h"
#include "ehs_types.h"

/* EHS_VOLATILE: expands to 'volatile' on targets whose mutex implementation does
 * not guarantee compiler memory-barrier semantics (e.g. no-op mutexes on bare-metal
 * MCUs, cooperative RTOS targets).  Targets backed by real POSIX pthread_mutex or
 * equivalent (GNU/Linux, ESP32 IDF, Zephyr) define this empty in target_types.h. */
#ifndef EHS_VOLATILE
#define EHS_VOLATILE volatile
#endif


/*********************** TARGET_INDEPENDENT TYPES FIRST AS THESE MAY BE USED IN TARGET SPECIFIC HEADERS ************************************************ */

#ifdef __cplusplus
 #define EHS_GLOBAL extern "C" /**< allows target-specific definition of global (i.e extern) types */
//needed to export variables in C++ (not as C?)
#define EHS_KERNEL_PROVIDED extern 
/* This is for C to C external compatability */
#define EHS_EXTERN extern   
#define EHS_CPP_EXPORT export extern 
//needed to import variables in C++ (clang C doesn't like extern for exported variables in C )
 #define EHS_C_CPP_EXPORT extern
 /* Most languages don't need anything to make function prototypes accessible, but theis macro allows 
      for cases we may want to export compnent APIs (e.g. as DLLs)*/
 #define EHS_COMPONENT_API_EXPORT  
#else
 /* In C if we want to import a global variable from another filewe use just extern */
 #define EHS_GLOBAL extern
 #define EHS_KERNEL_PROVIDED extern
 #define EHS_EXTERN extern 
 #define EHS_CPP_EXPORT
 #define EHS_C_CPP_EXPORT
 #define EHS_COMPONENT_API_EXPORT  
#endif

/* This is needed when compilig with C++ in combination with export */

#define EHS_SINT32_MAX 2147483647
#define EHS_SINT32_MIN -2147483648
#define EHS_UINT32_MAX 4294967295u
#define EHS_UINT32_MIN 0u
#define EHS_SINT16_MAX 32767
#define EHS_SINT16_MIN -32768
#define EHS_UINT16_MAX 65535u
#define EHS_UINT16_MIN 0u
#define EHS_UINT8_MAX 255u
#define EHS_UINT8_MIN 0u
#define EHS_SINT8_MAX 127
#define EHS_SINT8_MIN -128

#define EHS_MAGIC_NUMBER_16 0xf0f0
#define EHS_MAGIC_NUMBER_32 0xf0f0f0f0
#define EHS_MAGIC_NUMBER EHS_MAGIC_NUMBER_16

/* character constants */
#define EHS_CHAR_LF (0xau)
#define EHS_CHAR_CR (0xdu)
#define EHS_CHAR_ESC (0x1bu)

#define EHS_PARAM_SEPARATOR 0x01 /**< Used to separate fields in parameters. Only used in indirect paramter files currently, but should be used in SODL too for ert0 ? */

/* define special values */
#define EHS_FUNCTION_ID_START 0 /**< Id of the start event */
#define EHS_TRIG_ID_START 0 /**< Id of the triggering event */

/* Canonical EHS runtime directory structure */

#define EHS_DEFAULT_SODL_NAME "t.sdl" /**< Defines the standard name for anonymous SODL files */
#define EHS_DEFAULT_CONFIG_NAME "t.cnf" /**< Defines the standard name for the EHS config file */
#define EHS_SODL_EXTENSION "sdl"
#define EHS_SODL_EXT_SIZE 3

/**
 * Defines the states that the debugger can be in
 * todo2022 - this shouldn't be in the debug headers
 */
typedef enum
{
	EHS_DEBUG_OFF, /**< No debug messages are generated */
	EHS_DEBUG_ON, /**< Debug messages are generated for every trigger and every data flow */
	EHS_DEBUG_MONITOR
/**< Debug messages are generated only for monitored triggers and events */
} EhsDebugModeType;

/*****************************************************************************/
/* Define types */

/* ehs_FILE is defined in target_file.h */

/*****************************************************************************/
/* Declare global variables */

/**
 * Defines the global state for EHS.
 */
EHS_EXTERN EhsKEStateType EhsKEState;

/**
 * If true, indicates that the main scheduler engine is to run in single step mode.
 */
EHS_EXTERN ehs_bool EhsSingleStepFlag;

/**
 * If true, indicates that EHS is running in debug mode
 */
EHS_EXTERN EhsDebugModeType EhsDebugMode;

/**
 * Indicates the sequence number of the current debug message
 */
EHS_EXTERN ehs_uint32 EhsDebugSequenceNumber;

/**
 * Initialize all global variables to the correct state for startup
 */
/// These have moved to ehs_main.c  EHS_GLOBAL void EhsKSys_init(void);




/******************************************************END OF COMMON GLOBALS ************************************************ */

/*****************************************************************************/
/* Included target specifics headers */

#include "target.h"   /* Includes /platform/<TARGET>/target_config.h, target_types.h */

/*****************************************************************************/
/* Define macros for C/C++ types  */

/**************************************************** OVERRIDABLE BY TARGETS GO BELOW ************************************** */


/* Defines system-wide, target-independent limits */
#ifdef EHS_UNITY3D_CONFIG
/* todo2024 the following should be done in hte platform config.mk file (or something common to all unity apps */
	#define EHS_STRING_LENGTH_MAX 2*2047
	#warning "EHS_UNITY3D_CONFIG is configured for this platform using the default value of 2*2047" 
#endif //EHS_UNITY3D_CONFIG

#ifndef EHS_STRING_LENGTH_MAX
#define EHS_STRING_LENGTH_MAX 2047 /*@todo This needs to be killed off and proper dynamic allocation used - at least for edges */
/* Presumably the folloing is because the MSVC build doesn't build against a kernel library */
/* We also don't need this check for the kernel that shouldn't care about strings */
#if !defined(EHS_MSVC) && !defined(EHS_KERNEL_BUILD)
   
	#warning "EHS_STRING_LENGTH_MAX is not configured for this platform using the default value of 2047"
#endif
#endif //EHS_STRING_LENGTH_MAX

#ifndef MAX_FILENAME_LEN
#define MAX_FILENAME_LEN EHS_STRING_LENGTH_MAX
#endif

#ifndef MAX_PARAM_STR_LEN
// TODO2024: We are problably only doble this for low memory devices where.
#define MAX_PARAM_STR_LEN (EHS_STRING_LENGTH_MAX*2)
#endif

/* The following is used to define the maxim string length of a widget name or widget type string */
#ifndef EHS_OBJECT_NAME_MAX_SIZE
#define EHS_OBJECT_NAME_MAX_SIZE 64 
//todo we should probably make this smaller now that we store it for qt widgets, (we don't for lVGL ...)
#endif

/* define file names and size limits */

/* @todo These should move to FILE HAL code too */
#ifndef EHS_FILENAME_SIZE
#define EHS_FILENAME_SIZE EHS_TD_FILES_MAX_FILENAME /**< Defines the size of the part of filename before the '.' */
#endif
#define EHS_FILEEXT_SIZE 3 /**< Used only for console Defines the size of the part of the filename after the '.' */
#define EHS_FILESTRING_SIZE (EHS_FILENAME_SIZE + EHS_FILEEXT_SIZE + 2) /**< 2 comes from 1 character separator + 1 null character */
#define EHS_DEFAULT_FILENAME "default.txt" /**< Used when an invalid filename is given */

#ifndef EHS_MAXPATHLENGTH
#define EHS_MAXPATHLENGTH 2048
#endif
#ifndef EHS_MAX_URL_LENGTH
#define EHS_MAX_URL_LENGTH 4096
#endif
/* General timing constants */

#ifndef EHS_CONSOLE_BUFFER_CONTINUE_PAUSE_US
#define EHS_CONSOLE_BUFFER_CONTINUE_PAUSE_US 5000u /**< How long to wait before trying to write more data to the console in micro seconds*/
#endif

#define EHS_CONSOLE_BUFFER_MAX_RETRIES 5u /**< How many times to try to write to the console before giving up and dropping all data */
#define EHS_CONSOLE_BUFFER_MAX_TOTAL_TRIES 20u /**< How many times to try to contiue writing a before giving up and dropping remaing data */
/**
 * How long to wait before polling console input (when no application is executing)
 */
#define EHS_IDLE_PERIOD_US 5000u
#define EHS_IDLE_PERIOD EHS_TIME_ms(200u)

/**
 * How long to wait between reading chunks of input file before concluding that
 * nothing else is coming.
 * Note this may be used in the component code console ATM.
 */
#ifndef EHS_TIMEOUT_READ_FILE
#define EHS_TIMEOUT_READ_FILE EHS_TIME_s(10u)
#endif
/**
 * How long to wait where no event input is available (i.e. how long it takes
 * application to wake up after a new event first appears
 */
#ifndef EHS_WAKEUP_TIME
#define EHS_WAKEUP_TIME EHS_TIME_ms(20u)
#endif

#define EHS_KINDER_EHS_PROCESSING
#ifdef EHS_KINDER_EHS_PROCESSING
#ifndef EHS_KIND_SLEEP_TIME
/**
 * How long EHS will wait between executing groups or chunks of a group, which then bail..
 * e.g. infinite loops might want to be broken up a bit !!!
 *
 * ~todo - This is temporary - we should revert to EhsKEGroup_execute_x that has deadline exit and doesn't hog CPU in tight loops!
 */
#define EHS_KIND_SLEEP_TIME EHS_TIME_ms(20u)
#endif
#endif

/**
 * Checks whether x (a pointer to an object) is the final entry in a table.
 * The final entry is initialized with {0}, which sets all fields to zero.
 * Casting the first part of the structure to void* means that we can check
 * whether this is initialized to NULL.
 */
#define EHS_TABLE_END(x) (NULL == *(void**)(x))

/* define application size limits */
#ifndef EHS_GROUP_DEFAULT_PERIOD
/* todo2023 separate these from the global type defs etc. and make them overidable from profile types and platform config.mk files. 
            Most of these should only be visible to the kernel (i.e. not part of the KAPI).
*/
/* Group Processing granularity and allocation paramters */
/* Define default group processing granularity - note this is very long because we don't usually have any contention on processing.*/
/* EHS_GROUP_DEFAULT_PERIOD is the time period over which a group of event proccess will get a new allocation */
#define EHS_GROUP_DEFAULT_PERIOD		EHS_TIME_ms(1000)
#endif

#ifndef EHS_GROUP_DEFAULT_ALLOCATION
/* This is the maxium amount of queue processing time for the groupd of event processes before the group stops processing */
#define EHS_GROUP_DEFAULT_ALLOCATION	EHS_TIME_ms( 800)
#endif


/*****************************************************************************/
/* Declare function prototypes  */

/**************************************************** Macro Utilities ************************************** */

// Internal macro. DO NOT USE THIS!
#define _EHS_MACRO_STRINGIFY(x) #x

/**
 * @brief Turn any random characters or macro into a string
 * @code {.C}
 * #include <stdio.h>
 * #define A hello_world!
 * #define B "hello_world!"
 * #define C test_func
 *
 * printf(EHS_MACRO_STRINGIFY(A)); // hello_world!
 * printf(EHS_MACRO_STRINGIFY(B)); // "hello_world!"
 * printf(EHS_MACRO_STRINGIFY(C)); // test_func
 * printf(EHS_MACRO_STRINGIFY(A_MACRO_NOT_DEFINED)); // A_MACRO_NOT_DEFINED
 * @endcode
 * 
 */
#define EHS_MACRO_STRINGIFY(x) _EHS_MACRO_STRINGIFY(x)

/*****************************************************************************/
/* Declare macro utilities  */

#endif /* EHS_GLOBALS */
