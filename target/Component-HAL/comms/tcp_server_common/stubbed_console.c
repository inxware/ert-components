/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_console.c
 * In this file, the target-specific console interface functions provided for EHS are defined.
 *
 * todo2022 - this file should be moved to the common code - it doesn't have targete specific stuff in it and can provide console over other non TCPIP transports too.
 *
 * @author: inx limited
 *
 */

/*todo - this file could be moved to the common code area???*/

/**
 * @page Verification Verification report
 * @section target_console
 * @anchor target_console
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on 2007-10-12
 * Last modified on $Date$
 *
 * This file contained the following derogations to the MISRA standard.
 * @li Rule 16.1 Variable arguments shall not be used. Variable arguments are permitted for console output functions.
 */

/**
 * Provides access to the target-specific declarations of header files
 */

#include <stdarg.h>	
#include "globals.h"

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */
/*****************************************************************************/
/* Declare prototypes of local functions */
/*****************************************************************************/
/* Variables defined with file-scope */
/*****************************************************************************/
/* Variables defined with global-scope */

/*****************************************************************************/
/* Function definitions */

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
ehs_uint32 EhsConsoleGetLine(ehs_char *buff, ehs_uint16 size)
{
    return 0;
}

/**EHSH_LOG_ERROR("Writing to file %s",appPath );
 * Print text into the console output. This function blocks as long
 * as necessary to send the output.
 *
 * @param fmt Format specifier for output (as per printf)
 * 
 * todo2024 - this should return void as we never check the result.
 */
EHS_MEMORY_ATTRIB ehs_uint16 EhsConsolePrintf(const ehs_char *fmt, ...) /*lint !e960 Allowable derrogation to MISRA 16.1. Variable args permitted */
{
#ifndef EHS_MEMORY_ATTRIB
    /* format the message into a chunk of memory allocated especially */
    va_list args;
    va_start(args, fmt);
    printf(fmt, args); /*lint !e534 Not interested in the return value */
#endif
    return 1;
}


/**
 * Check whether console input is available
 *
 * @return true if one or more characters is available
 */
ehs_bool EhsConsoleInputHit()
{
    ehs_bool bRet = EHS_FALSE;
    return bRet;
}

/**
 * Check whether a complete line is available for reading. This occurs when console input contains
 * either a line feed or a carriage return character.
 *
 * @return true if a line is available
 */
ehs_bool EhsConsoleLineReady()
{
    ehs_bool bRet = EHS_FALSE;
    return bRet;
}
/**
 * Read a file from the console input into persistent storage (file system or flash).
 *
 * @param nSize Size to download. If we don't know how big the file is,
 * we can pass in EHS_UINT32_MAX. If the file size is zero, we create an
 * empty file.
 * @param name Name to store file as. This will conform to the EHS filename
 * standard (i.e. 8.3 with leading letter, only letters and digits)
 *
 * @return true if load is successful
 */
ehs_bool EhsConsoleToFile(ehs_uint32 nSize, const ehs_char *name)
{
    ehs_bool bRet = EHS_FALSE;
    return bRet;
}
