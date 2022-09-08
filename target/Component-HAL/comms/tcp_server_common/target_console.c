/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file target_console.c
 * In this file, the target-specific console interface functions provided for EHS are defined.
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
#define EHS_TARGET_CODE

/*****************************************************************************/
/* Included files */
#include "target.h"
#include "globals.h"
#include <stdarg.h>
#include "messages.h"
#include "hal_string.h"
#include "console_queue.h"
#include "console.h" // needed for buffer status flag

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */
/*****************************************************************************/
/* Declare prototypes of local functions */
/*****************************************************************************/
/* Variables defined with file-scope */
/*****************************************************************************/
/* Variables defined with global-scope */

/**
 * Contains input from the console.
 * Pointer initialised by EhsTargetInitSharedMemory, queue initialised by EhsTargetInit
 */
EhsConsoleQueueType* EhsTgtConsoleInputQueueRef = NULL;

/**
 * Contains output to the console
 * Pointer initialised by EhsTargetInitSharedMemory, queue initialised by EhsTargetInit
 */
EhsConsoleQueueType* EhsTgtConsoleOutputQueueRef = NULL;

/**
 * Contains input from the console
 */
EhsConsoleQueueType EhsTgtConsoleInputQueue;

/**
 * Contains output to the console
 * Initialised by EhsHSys_init
 */
EhsConsoleQueueType EhsTgtConsoleOutputQueue;

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
    ehs_uint32 nLineSize;
    ehs_uint32 nRead = 0u;

    EhsTPMutex_lock(EhsTPMutex_consoleInputQueue);
    if (EhsTgtConsoleInputQueueRef && size > 0 )   // don't bother of we only have small chunk of data it wont be a full reading.
    {
        //EHSH_LOG_ERROR("EhsConsoleGetLine %d - max size",size );
        nLineSize = EhsConsoleQueue_peek(EhsTgtConsoleInputQueueRef,EHS_CHAR_LF);
        if (0u == nLineSize)
        {
            nLineSize = EhsConsoleQueue_peek(EhsTgtConsoleInputQueueRef,EHS_CHAR_CR); /* We'll accept either type of CR */
        }

        if (nLineSize > 0) // also wait for smallest chunk..
        {
            if (nLineSize > size)
            {
                nLineSize = size;
            }
            nRead = EhsConsoleQueue_pop(EhsTgtConsoleInputQueueRef,(ehs_uint8*)buff,nLineSize);
        }
    }
    EhsTPMutex_unlock(EhsTPMutex_consoleInputQueue);
    return nRead;
}

/**EHSH_LOG_ERROR("Writing to file %s",appPath );
 * Print text into the console output. This function blocks as long
 * as necessary to send the output.
 *
 * @param fmt Format specifier for output (as per printf)
 */
ehs_uint16 EhsConsolePrintf(const ehs_char* fmt, ...) /*lint !e960 Allowable derrogation to MISRA 16.1. Variable args permitted */
{

#if 1
    ehs_char szBuffer[EHS_STRING_LENGTH_MAX];
    ehs_uint16 nLen = EHS_STRING_LENGTH_MAX;
    ehs_char *pBuff;
    ehs_uint32 nBuff;
    ehs_uint32 nPushed;

    /* format the message into a chunk of memory allocated especially */
    va_list args;
    va_start(args,fmt);
    vsnprintf(szBuffer,(size_t)EHS_STRING_LENGTH_MAX,fmt,args); /*lint !e534 Not interested in the return value */
    va_end(args);
    /* keep pushing the message until it's all gone */
    nBuff = EhsStrlen(szBuffer);
    pBuff = szBuffer;
    do
    {
        /* always print error messages on stdout */
        if (1 || pBuff[0] == '*')
        {
            fflush(stdout); /*lint !e534 Safe to ignore return value here */
        }
        nPushed = EhsConsoleQueue_push(EhsTgtConsoleOutputQueueRef,(ehs_uint8*)pBuff,nBuff);
        if (nPushed == 0u)
        {
            /* can't push any more - we'll reset the contents of the queue */
            EhsConsoleQueue_reset(EhsTgtConsoleOutputQueueRef);
        }
        pBuff += nPushed;
        nBuff -= nPushed;
    }
    while (nBuff > 0u);
    EhsConsoleQueue_push(EhsTgtConsoleOutputQueueRef,(ehs_uint8*)"\n",1);
    return nLen;
#endif
}

/**
 * Check whether console input is available
 *
 * @return true if one or more characters is available
 */
ehs_bool EhsConsoleInputHit()
{
    ehs_bool bRet = EHS_FALSE;
    if (EhsTgtConsoleInputQueueRef != NULL) bRet=!EhsConsoleQueue_isEmpty(EhsTgtConsoleInputQueueRef);
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
    EhsTPMutex_lock(EhsTPMutex_consoleInputQueue);
    ehs_bool bRet = (0u < (EhsConsoleQueue_peek(EhsTgtConsoleInputQueueRef,EHS_CHAR_CR) +
                           EhsConsoleQueue_peek(EhsTgtConsoleInputQueueRef,EHS_CHAR_LF)));
    EhsTPMutex_unlock(EhsTPMutex_consoleInputQueue);
    return bRet;
}

/**
 * Read a file from the console input into persistent storage.
 *
 * @param nSize Size to download. If we don't know how big the file is,
 * we can pass in EHS_UINT32_MAX. If the file size is zero, we create an
 * empty file.
 * @param name Name to store file as. This will conform to the EHS filename
 * standard (i.e. 8.3 with leading letter, only letters and digits)
 *
 * @return true if load is successful
 */
ehs_bool EhsConsoleToFile(ehs_uint32 nSize, const ehs_char* name)
{
    ehs_uint8 pInBuff[EHS_FILE_BUFF_SIZE];
    ehs_char appPath[EHS_MAXPATHLENGTH];
    ehs_FILE* pOut;
    ehs_sint32 nSizeRemaining = nSize; /* size of the file remaining to write */
    EhsTickType tTimeOfLastRead = EhsCurrentTime();
    //EhsTickType debugFirstTime = EhsCurrentTime();
    ehs_uint32 nRead = 0; /* bytes to read or bytes that have been read */
    ehs_bool bRet = EHS_TRUE; /* assume success */

    /* open the output file */
    Ehs_AppMkdir("temp");
    EhsStrcpy(appPath,"temp/"); /* We always write to this directory */
    EhsStrcat(appPath,name);
    //pOut = Ehs_AppFopen(appPath,"wb");
    pOut = Ehs_AppBaseFopen(appPath,"wb"); /* open the file temp - should create entire path...*/

    /* read data until:
       1. there is nothing left to read OR
       2. no data has happened for a while
       */
    while (((EhsCurrentTime() - tTimeOfLastRead) < EHS_TIMEOUT_READ_FILE) && (nSizeRemaining > 0u))
    {
#ifdef EHS_LWIP
        EhsSleep(50);
#else
        EhsSleep(5); //rate limiting is to allow other threads to run.
#endif //#ifdef EHS_LWIP 

        nRead = (EHS_FILE_BUFF_SIZE < nSizeRemaining)?EHS_FILE_BUFF_SIZE:nSizeRemaining;
        EhsTPMutex_lock(EhsTPMutex_consoleInputQueue);
        nRead = EhsConsoleQueue_pop(EhsTgtConsoleInputQueueRef,pInBuff,nRead);
        EhsTPMutex_unlock(EhsTPMutex_consoleInputQueue);
        //if (nRead >0) {
        //	char printit[1025];
        //	strncpy(printit,pInBuff,nRead);
        //	printit[nRead]='\0';
        //}
        if (nRead > 0)
        {
            tTimeOfLastRead = EhsCurrentTime();
            nSizeRemaining -= nRead;

            /* we must read in all of the data whether or not the file was
             * opened successfully - hence we check here whether the pointer
             * is valid
             */
            if (bRet && pOut)
            {
                if (EhsFwrite(pInBuff,sizeof(unsigned char),nRead,pOut) != nRead)
                {
                    /* write failed. Tell the world, but keep listening for input data */
                    EhsError(EHS_MSG_ERROR_FILE_NOT_WRITE(name));
                    bRet = EHS_FALSE;
                }
            }
        }
    }

    if (pOut)
    {
        if (nSizeRemaining > 0)
        {
            /* If we have a fragement of the last file we need to push this back on the queue so it can be joined with it's relevent data to parse */
            //EhsConsoleQueue_push(EhsTgtConsoleInputQueueRef,(ehs_uint8*)buff,nLineSize);
        }
        if (EhsFclose(pOut) == 0 )
        {
            if ((nSizeRemaining > 0u) && (nSize > 0u))
            {
                /* timeout must have occured */
                EhsError(EHS_MSG_ERROR_FILE_TIMEOUT(name, nSizeRemaining, nSize));
                bRet = EHS_FALSE;
            }
        }
        else
        {
            /* file failed to close */
            EhsError(EHS_MSG_ERROR_FILE_NOT_CLOSE(name));
            bRet = EHS_FALSE;
        }
    }
    else
    {
        EhsError(EHS_MSG_ERROR_FILE_NOT_OPEN(name));
        bRet = EHS_FALSE;
    }

    return bRet;
}
