/** @file hal_console.h
 * In this file, all of the hardware abstraction layer declarations provided for EHS console I/O are given.
 * 
 * @author: inx limited
 * @version: $Revision: 2780 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

#ifndef EHS_HAL_CONSOLE_H
#define EHS_HAL_CONSOLE_H

/*****************************************************************************/
/* Included files */

#include "console_queue.h"
#include "target_types.h"
#ifdef EHS_COMMS_TASK
#include "target_console.h"
#endif

/*****************************************************************************/
/* Define macros  */


/*****************************************************************************/
/* Define types */


/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */


/**
 * Read a line of text from the console up to the specified size.
 * Returned text includes the terminating \n.
 * This function only returns content if an entire line has been read
 * (i.e. if a \n is available). Partially received lines are buffered
 * until the end of line character has been received).
 *
 * @param buff Buffer to read the line into. Must be at least "size" long
 * @param size Size of buff
 * @return number of characters read (or zero if no line is available)
 */
#if defined(EHS_SKIP_GNULIBRARIES)
    #if defined(EHS_COMMS_TASK)
        EHS_GLOBAL ehs_uint32 EhsConsoleGetLine(ehs_char *buff, ehs_uint16 size);
    #else
        inline ehs_uint32 EhsConsoleGetLine(ehs_char *buff, ehs_uint16 size) {return 0;}
    #endif
#else
    EHS_GLOBAL ehs_uint32 EhsConsoleGetLine(ehs_char *buff, ehs_uint16 size);
#endif
/**
 * Print text into the console output. This function blocks as long
 * as necessary to send the output.
 *
 * @param fmt Format specifier for output (as per printf)
 */
#ifdef EHS_SKIP_GNULIBRARIES
    #ifdef EHS_COMMS_TASK
        EHS_GLOBAL ehs_uint16 EhsConsolePrintf(const ehs_char* fmt, ...);
    #else
        #ifndef EhsConsolePrintf
            #define EhsConsolePrintf
        #endif //EhsConsolePrintf
    #endif
#else
#ifndef EhsConsolePrintf
EHS_GLOBAL ehs_uint16 EhsConsolePrintf(const ehs_char* fmt, ...); /*lint !e960 Variable arguments required to support generalized printf */
#endif
#endif
/**
 * Check whether console input is available
 *
 * @return true if one or more characters is available
 */
#ifdef EHS_SKIP_GNULIBRARIES
    #ifdef EHS_COMMS_TASK
        EHS_GLOBAL ehs_bool EhsConsoleInputHit(void);
    #else
        inline ehs_bool EhsConsoleInputHit() {return EHS_TRUE;}
    #endif
#else
EHS_GLOBAL ehs_bool EhsConsoleInputHit(void);
#endif
/**
 * Check whether a complete line is available for reading
 *
 * @return true if a line is available
 */
#ifdef EHS_SKIP_GNULIBRARIES
    #ifdef EHS_COMMS_TASK
        EHS_GLOBAL ehs_bool EhsConsoleLineReady(void);
    #else
        #define EhsConsoleLineReady
    #endif
#else
EHS_GLOBAL ehs_bool EhsConsoleLineReady(void);
#endif
/**
 * Read a file from the console input into persistent storage.
 *
 * @param size Size to download. If zero, we don't know how big it is
 * keep inputting until there is at least 10 seconds where no input has
 * been received.
 * @param name Name to store file as. This will conform to the EHS filename
 * standard (i.e. 8.3 with leading letter, only letters and digits)
 *
 * @return true if load is successful
 */
#ifdef EHS_SKIP_GNULIBRARIES
    #ifdef EHS_COMMS_TASK
        EHS_GLOBAL ehs_bool EhsConsoleToFile(ehs_uint32 size, const ehs_char* name);        
    #else
        #define EhsConsoleToFile
    #endif
#else
EHS_GLOBAL ehs_bool EhsConsoleToFile(ehs_uint32 size, const ehs_char* name);
#endif
/**
 * Contains input from the console.
 * Initialised by EhsHSys_init
 */
EHS_GLOBAL EhsConsoleQueueType* EhsTgtConsoleInputQueueRef;

/**
 * Contains output to the console
 * Initialised by EhsHSys_init
 */
EHS_GLOBAL EhsConsoleQueueType* EhsTgtConsoleOutputQueueRef;

#endif /* EHS_HAL_CONSOLE_H */
