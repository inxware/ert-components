/** @file globals.h
 * Declarations of EHS-wide globals.
 * 
 * @author: inx limited
 * @version: $Revision: 5462 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
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

#ifndef EHS_GLOBALS
#define EHS_GLOBALS

/*****************************************************************************/
/* Included files */

#include "target_config.h" /* special include required before macro definitions */
#include "ehs_types.h"
/*****************************************************************************/
/* Define macros  */

/* Defines system-wide, target-independent limits */
#ifdef EHS_UNITY3D_CONFIG
	#define EHS_STRING_LENGTH_MAX 2*2047
#endif //EHS_UNITY3D_CONFIG

#ifndef EHS_STRING_LENGTH_MAX
	#define EHS_STRING_LENGTH_MAX 2047 /*@todo This needs to be killed off and proper dynamic allocation used - at least for edges */
#endif //EHS_STRING_LENGTH_MAX

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

#define EHS_PARAM_SEPARATOR 0x01 /**< Used to separate fields in parameters */

/* define special values */
#define EHS_FUNCTION_ID_START 0 /**< Id of the start event */
#define EHS_TRIG_ID_START 0 /**< Id of the triggering event */

/* Canonical EHS runtime directory structure */

#define EHS_DEFAULT_SODL_NAME "t.sdl" /**< Defines the standard name for anonymous SODL files */
#define EHS_DEFAULT_CONFIG_NAME "t.cnf" /**< Defines the standard name for the EHS config file */
#define EHS_SODL_EXTENSION "sdl"
#define EHS_SODL_EXT_SIZE 3

/* define file names and size limits */

/* @todo These should move to FILE HAL code too */
#define EHS_FILENAME_SIZE 1024 /**< Defines the size of the part of filename before the '.' */
#define EHS_FILEEXT_SIZE 3 /**< Used only for console Defines the size of the part of the filename after the '.' */
#define EHS_FILESTRING_SIZE (EHS_FILENAME_SIZE + EHS_FILEEXT_SIZE + 2) /**< 2 comes from 1 character separator + 1 null character */
#define EHS_DEFAULT_FILENAME "default.txt" /**< Used when an invalid filename is given */
#define EHS_MAXPATHLENGTH 2048
#define EHS_MAX_URL_LENGTH 4096
/* General timing constants */

/**
 * How long to wait before polling console input (when no application is executing)
 */
#define EHS_IDLE_PERIOD EHS_TIME_ms(200u) 

/**
 * How long to wait between reading chunks of input file before concluding that
 * nothing else is coming
 */
#define EHS_TIMEOUT_READ_FILE EHS_TIME_s(10u)

/**
 * How long to wait where no event input is available (i.e. how long it takes
 * application to wake up after a new event first appears
 */
#define EHS_WAKEUP_TIME EHS_TIME_ms(20u)

#define EHS_KINDER_EHS_PROCESSING
#ifdef EHS_KINDER_EHS_PROCESSING
/**
 * How long EHS will wait between executing groups or chunks of a group, which then bail..
 * e.g. infinite loops might want to be broken up a bit !!!
 *
 * ~todo - This is temporary - we should revert to EhsKEGroup_execute_x that has deadline exit and doesn't hog CPU in tight loops!
 */
#define EHS_KIND_SLEEP_TIME EHS_TIME_ms(20u)
#endif

/**
 * Checks whether x (a pointer to an object) is the final entry in a table.
 * The final entry is initialized with {0}, which sets all fields to zero.
 * Casting the first part of the structure to void* means that we can check
 * whether this is initialized to NULL.
 */
#define EHS_TABLE_END(x) (NULL == *(void**)(x))

/* define application size limits */
#define EHS_GROUP_DEFAULT_PERIOD		EHS_TIME_ms(1000)
#define EHS_GROUP_DEFAULT_ALLOCATION	EHS_TIME_ms( 800)

/**
 * Defines the states that the debugger can be in
 */
typedef enum {
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
 */EHS_GLOBAL EhsKEStateType EhsKEState;

/**
 * If true, indicates that the main scheduler engine is to run in single step mode.
 */EHS_GLOBAL ehs_bool EhsSingleStepFlag;

/**
 * If true, indicates that EHS is running in debug mode
 */EHS_GLOBAL EhsDebugModeType EhsDebugMode;

/**
 * Indicates the sequence number of the current debug message
 */EHS_GLOBAL ehs_uint32 EhsDebugSequenceNumber;

/**
 * Initialize all global variables to the correct state for startup
 */
/// These have moved to ehs_main.c  EHS_GLOBAL void EhsKSys_init(void);


/*****************************************************************************/
/* Declare function prototypes  */

#endif /* EHS_GLOBALS */
