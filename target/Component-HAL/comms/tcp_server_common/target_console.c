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
#define EHS_TARGET_CODE

/* This file implements the console, so it needs the kernel-only prototypes */
#define EHS_CONSOLE_IMPLEMENTATION

/*****************************************************************************/
/* Included files */
#include "globals.h"

#ifdef EHS_NANOPRINTF_SUPPORT
/* @TODO - move implementation of functions in nanoprintf.h into nanoprintf.c so, 
 *         we can include nanoprintf.h header in the traget_string.h without circular 
 *         dependency isseue which manifest as duplicated function implementation.
 *         At the moment nanoprintf/nanoprintf.h needs to be called before traget_string.h
 *         in order to override EhsVsnprintf when EHS_NANOPRINTF_SUPPORT enabled, so until
 *         functions in nanoprintf.h are moved to .c file , leave this here!
 */
#define NANOPRINTF_IMPLEMENTATION

#include "nanoprintf/nanoprintf.h"
#endif // EHS_NANOPRINTF_SUPPORT

#include <stdarg.h>

/* Set the Logger module before ANY header that pulls in hal_logger.h - messages.h does,
 * and hal_logger.h latches EHSL_MODULE_ID to UNDEFINED if it isn't set yet. */
#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_CONSOLE
#include "hal_logger.h"
#include "messages.h"
#include "hal_string.h"
#include "hal_process.h"
#include "hal_file.h"
#include "hal_time.h"
#include "console_queue.h"
#include "console.h" // needed for buffer status flag

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

#ifndef EHS_DEBUG_CONSOLE_BUFFER_SIZE
#error "EHS_DEBUG_CONSOLE_BUFFER_SIZE must be set by the platform config"
#endif

/* EHS_CONSOLE_QUEUE_INDEX masks with (size-1), so the ring must be a power of two. */
#if (EHS_DEBUG_CONSOLE_BUFFER_SIZE & (EHS_DEBUG_CONSOLE_BUFFER_SIZE - 1)) != 0
#error "EHS_DEBUG_CONSOLE_BUFFER_SIZE must be a power of two"
#endif

/**< Overflow flag record - emitted when a message had to be dropped entirely */
#define EHS_CONSOLE_OVERFLOW_RECORD (EHS_FLAG_CONSOLE_CONSOLE_OVERFLOW EHS_MSG_END_OF_MESSAGE)
#define EHS_CONSOLE_OVERFLOW_RECORD_LEN (EHS_FLAG_CONSOLE_CONSOLE_OVERFLOW_LEN + EHS_MSG_END_OF_MESSAGE_LEN)

/* Space kept free so a flag record can always be pushed alongside or instead of a message.
 * Sized for the longest flag record (the truncation flag), not the shortest. */
#define EHS_CONSOLE_FLAG_RESERVE EHS_FLAG_CONSOLE_TRUNCATED_MAX_LEN

/* Longest message EhsConsolePrintf will stage, including its record terminator. A message
 * bigger than the ring can never be pushed atomically, so it would have to be split across
 * retries and would reach the tools as a torn record. Derive from whatever ring size the
 * platform configured rather than requiring the ring to grow to EHS_STRING_LENGTH_MAX. */
#define EHS_CONSOLE_RING_MSG_MAX (EHS_DEBUG_CONSOLE_BUFFER_SIZE - EHS_CONSOLE_FLAG_RESERVE)
#if EHS_CONSOLE_RING_MSG_MAX < EHS_STRING_LENGTH_MAX
#define EHS_CONSOLE_MSG_MAX EHS_CONSOLE_RING_MSG_MAX
#else
#define EHS_CONSOLE_MSG_MAX EHS_STRING_LENGTH_MAX
#endif

/* Anything this small can't carry a useful record plus its terminator. */
#if EHS_CONSOLE_MSG_MAX < 16u
#error "EHS_DEBUG_CONSOLE_BUFFER_SIZE is too small to carry a console record"
#endif

/**< Chunk size used to discard the tail of an over-long console command line */
#define EHS_CONSOLE_DISCARD_CHUNK 32u

/*****************************************************************************/
/* Declare prototypes of local functions */

/**
 * Append an unsigned decimal to a buffer, returning the new length. Local so a flag
 * record doesn't depend on which snprintf alias a given target happens to define.
 * @param pBuf   Buffer to append to - caller guarantees room for 10 digits
 * @param nLen   Current length of pBuf
 * @param nValue Value to append
 * @return new length of pBuf
 */
EHS_LOCAL ehs_uint32 EhsL_appendUint(ehs_char* pBuf, ehs_uint32 nLen, ehs_uint32 nValue);
/*****************************************************************************/
/* Variables defined with file-scope */
/*****************************************************************************/
/* Variables defined with global-scope */


/**
 * Contains input from the console.
 * Pointer initialised by EhsTargetInitSharedMemory, queue initialised by EhsTargetInit
 */
EhsConsoleQueueType *EhsTgtConsoleInputQueueRef = NULL;

/**
 * Contains output to the console
 * Pointer initialised by EhsTargetInitSharedMemory, queue initialised by EhsTargetInit
 */
EhsConsoleQueueType *EhsTgtConsoleOutputQueueRef = NULL;

/**
 * Set once a record has been dropped, so a run of drops emits a single overflow marker
 * rather than one per record. Cleared by the next record that is successfully queued.
 */
EHS_LOCAL volatile ehs_bool bConsoleLossFlagged = EHS_FALSE;

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
 * A line longer than "size" is truncated, and the remainder is discarded rather than
 * left in the queue to be parsed as a further command. The returned text is always
 * NUL-terminated within "size" bytes.
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
    if (EhsTgtConsoleInputQueueRef && size > 1u) // don't bother of we only have small chunk of data it wont be a full reading.
    {
        // EHSH_LOG_ERROR("EhsConsoleGetLine %d - max size",size );
        nLineSize = EhsConsoleQueue_peek(EhsTgtConsoleInputQueueRef, EHS_CHAR_LF);
        if (0u == nLineSize)
        {
            nLineSize = EhsConsoleQueue_peek(EhsTgtConsoleInputQueueRef, EHS_CHAR_CR); /* We'll accept either type of CR */
        }

        if (nLineSize > 0) // also wait for smallest chunk..
        {
            ehs_uint32 nDiscard = 0u; /* tail of an over-long line, dropped below */

            if (nLineSize > (ehs_uint32)(size - 1u))
            {
                nDiscard = nLineSize - (ehs_uint32)(size - 1u);
                nLineSize = (ehs_uint32)(size - 1u);
            }
            nRead = EhsConsoleQueue_pop(EhsTgtConsoleInputQueueRef, (ehs_uint8 *)buff, nLineSize);
            buff[nRead] = (ehs_char)'\0';

            while (nDiscard > 0u)
            {
                ehs_uint8 scratch[EHS_CONSOLE_DISCARD_CHUNK];
                ehs_uint32 nChunk = (nDiscard > EHS_CONSOLE_DISCARD_CHUNK) ? EHS_CONSOLE_DISCARD_CHUNK : nDiscard;
                ehs_uint32 nPopped = EhsConsoleQueue_pop(EhsTgtConsoleInputQueueRef, scratch, nChunk);
                if (0u == nPopped)
                {
                    break;
                }
                nDiscard -= nPopped;
            }
        }
    }
    EhsTPMutex_unlock(EhsTPMutex_consoleInputQueue);
    return nRead;
}

/**
 * Append an unsigned decimal to a buffer, returning the new length.
 */
EHS_LOCAL ehs_uint32 EhsL_appendUint(ehs_char* pBuf, ehs_uint32 nLen, ehs_uint32 nValue)
{
    ehs_char szDigits[10]; /* widest ehs_uint32 is 4294967295 */
    ehs_uint32 nDigits = 0u;

    do
    {
        szDigits[nDigits] = (ehs_char)('0' + (nValue % 10u));
        nDigits++;
        nValue /= 10u;
    } while ((nValue > 0u) && (nDigits < (ehs_uint32)sizeof(szDigits)));

    while (nDigits > 0u)
    {
        nDigits--;
        pBuf[nLen] = szDigits[nDigits];
        nLen++;
    }
    return nLen;
}

/**
 * Print text into the console output.
 *
 * Never blocks. The record is queued whole or dropped whole, decided once by
 * EhsConsoleQueue_pushRecord - there is no retry, no wait and no queue flush, so this is
 * safe to call from the EHS event thread and from small-stack handlers.
 *
 * Loss policy is drop-newest: already-queued records survive, this one does not, and a
 * '**Z' marker is emitted from the reserve so the reader sees the gap.
 *
 * A message longer than EHS_CONSOLE_MSG_MAX cannot be queued atomically, so it is cut
 * there and followed by a '**T' record giving the delivered and formatted lengths.
 * See docs/ert-porting-guide.md § "Console queue behaviour and sizing".
 *
 * @param fmt Format specifier for output (as per printf)
 *
 * todo2024 - this should return void as we never check the result.
 */
EHS_MEMORY_ATTRIB ehs_uint16 EhsConsolePrintf(const ehs_char *fmt, ...) /*lint !e960 Allowable derrogation to MISRA 16.1. Variable args permitted */
{
    ehs_char szBuffer[EHS_CONSOLE_MSG_MAX];
    ehs_char szFlag[EHS_CONSOLE_FLAG_RESERVE];
    ehs_uint32 nBuff;
    ehs_uint32 nFlag = 0u;
    int nFormatted;
    va_list args;

    if (NULL == EhsTgtConsoleOutputQueueRef)
    {
        return 0; /* console not set up yet, or this target has none */
    }

    va_start(args, fmt);
    /* one byte held back for the record terminator appended below */
    nFormatted = EhsVsnprintf(szBuffer, (size_t)(EHS_CONSOLE_MSG_MAX - 1u), fmt, args);
    va_end(args);

    nBuff = EhsStrlen(szBuffer);
    szBuffer[nBuff] = (ehs_char)EHS_CHAR_LF;
    nBuff++;

    #if !defined(EHS_ESP32_SUPPORT)
        fflush(stdout); /*lint !e534 Safe to ignore return value here */
    #endif

    /* EhsVsnprintf returns the length it would have written, so a value at or beyond the
     * buffer size means the record was cut. Build "**T<delivered>,<formatted>"; a
     * formatted count of 0 means the count itself was unusable. */
    if ((nFormatted < 0) || ((ehs_uint32)nFormatted >= (EHS_CONSOLE_MSG_MAX - 1u)))
    {
        ehs_uint32 nTotal = (nFormatted < 0) ? 0u : (ehs_uint32)nFormatted;
        EhsMemcpy(szFlag,(void*)EHS_FLAG_CONSOLE_TRUNCATED,EHS_FLAG_CONSOLE_TRUNCATED_LEN);
        nFlag = EhsL_appendUint(szFlag,EHS_FLAG_CONSOLE_TRUNCATED_LEN,nBuff - 1u);
        szFlag[nFlag] = (ehs_char)',';
        nFlag++;
        nFlag = EhsL_appendUint(szFlag,nFlag,nTotal);
        szFlag[nFlag] = (ehs_char)EHS_CHAR_LF;
        nFlag++;
        EHSH_LOG_ERROR("Console record truncated - sent %u of %u bytes (queue used=%u/%u left=%u)",
                       (unsigned int)(nBuff - 1u),(unsigned int)nTotal,
                       (unsigned int)EhsConsoleQueue_length(EhsTgtConsoleOutputQueueRef),
                       (unsigned int)EhsConsoleQueue_maxSize(), (unsigned int)EhsConsoleQueue_space(EhsTgtConsoleOutputQueueRef));
    }

    /* Keep the reserve free so a failure notice always fits; spend it only on one. */
    if (EhsConsoleQueue_pushRecord(EhsTgtConsoleOutputQueueRef,(const ehs_uint8*)szBuffer,
                                   nBuff,EHS_CONSOLE_FLAG_RESERVE))
    {
        bConsoleLossFlagged = EHS_FALSE;
#ifdef EHS_CONSOLE_QUEUE_STATS
        /* Same shape as the drop/truncate lines above, but for the normal, non-dropped
         * path - lets you compare buffer state during quiet periods against right before
         * an overflow, not just see the overflow after the fact. Gated (unlike those two
         * error lines) since this fires on every successful console record, not just the
         * rare failure cases. */
        EHSH_LOG_INFO("Console record sent (%u bytes, used=%u/%u left=%u)",
                      (unsigned int)nBuff, (unsigned int)EhsConsoleQueue_length(EhsTgtConsoleOutputQueueRef),
                      (unsigned int)EhsConsoleQueue_maxSize(), (unsigned int)EhsConsoleQueue_space(EhsTgtConsoleOutputQueueRef));
#endif
        if (nFlag > 0u)
        {
            (void)EhsConsoleQueue_pushRecord(EhsTgtConsoleOutputQueueRef,
                                             (const ehs_uint8*)szFlag,nFlag,0u);
        }
    }
    else if (!bConsoleLossFlagged)
    {
        /* Drop-newest: queued records survive, this one does not. One marker per loss run
         * tells the reader there is a gap; repeating it would just fill the reserve. */
        (void)EhsConsoleQueue_pushRecord(EhsTgtConsoleOutputQueueRef,
                                         (const ehs_uint8*)EHS_CONSOLE_OVERFLOW_RECORD,
                                         EHS_CONSOLE_OVERFLOW_RECORD_LEN,0u);
        bConsoleLossFlagged = EHS_TRUE;
        EHSH_LOG_ERROR("Console full - dropping records (%u bytes wanted, used=%u/%u left=%u)",
                       (unsigned int)nBuff, (unsigned int)EhsConsoleQueue_length(EhsTgtConsoleOutputQueueRef),
                       (unsigned int)EhsConsoleQueue_maxSize(), (unsigned int)EhsConsoleQueue_space(EhsTgtConsoleOutputQueueRef));
    }
    return 0;
}

/**
 * Check whether console input is available
 *
 * @return true if one or more characters is available
 */
ehs_bool EhsConsoleInputHit()
{
    ehs_bool bRet = EHS_FALSE;
    if (EhsTgtConsoleInputQueueRef != NULL)
        bRet = !EhsConsoleQueue_isEmpty(EhsTgtConsoleInputQueueRef);
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
    ehs_bool bRet = (0u < (EhsConsoleQueue_peek(EhsTgtConsoleInputQueueRef, EHS_CHAR_CR) +
                           EhsConsoleQueue_peek(EhsTgtConsoleInputQueueRef, EHS_CHAR_LF)));
    EhsTPMutex_unlock(EhsTPMutex_consoleInputQueue);
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
    ehs_uint8 pInBuff[EHS_FILE_BUFF_SIZE];
    ehs_char appPath[EHS_MAXPATHLENGTH];
    ehs_FILE *pOut;
    ehs_sint32 nSizeRemaining = nSize; /* size of the file remaining to write */
    EhsTickType tTimeOfLastRead = EhsCurrentTime();
    ehs_uint32 nRead = 0;     /* bytes to read or bytes that have been read */
    ehs_bool bRet = EHS_TRUE; /* assume success */

#ifdef INX_SODL_IN_FLASH
 /* ignore doing any directory stuff as we are probably using the tiny file system (not littleFS) */
    pOut = Ehs_Fopen(name, "wb"); /* No Directories for SODL so just */
#else
    /* open the output file */
    Ehs_AppMkdir("temp");
    EhsStrcpy(appPath, "temp/"); /* We always write to this directory */
    EhsStrcat(appPath, name);
    pOut = Ehs_AppBaseFopen(appPath, "wb"); /* open the file temp - should create entire path...*/
#endif
    /* Read data until:
       1. there is nothing left to read OR
       2. no data has happened for a while
    */
    while (((EhsCurrentTime() - tTimeOfLastRead) < EHS_TIMEOUT_READ_FILE) && (nSizeRemaining > 0u))
    {
        // todo the delay here should just be a global default that can be overriden in platform config files.
#ifdef EHS_LWIP
        // @TODO - this may need to be reduced to improve speed of transfer for esp32
        EhsSleepUs(50000); // @Xiaosheng - is this for writing to file.
#else
        EhsSleep(5); // delay for 5 clock cycles. so that file transfer is fast.
#endif // #ifdef EHS_LWIP

        nRead = (EHS_FILE_BUFF_SIZE < nSizeRemaining) ? EHS_FILE_BUFF_SIZE : nSizeRemaining;
        EhsTPMutex_lock(EhsTPMutex_consoleInputQueue);
        nRead = EhsConsoleQueue_pop(EhsTgtConsoleInputQueueRef, pInBuff, nRead);
        EhsTPMutex_unlock(EhsTPMutex_consoleInputQueue);

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
                if (EhsFwrite(pInBuff, sizeof(unsigned char), nRead, pOut) != nRead)
                {
                    /* write failed. Tell the world, but keep listening for input data */
                    EHSH_LOG_ERROR(EHS_MSG_ERROR_FILE_NOT_WRITE(name));
                    bRet = EHS_FALSE;
                }
            }
        }
    }

    if (pOut)
    {
        if (nSizeRemaining > 0)
        {
            /* If we have a fragement of the last file we need to push this back on the queue so it can be joined with 
                it's relevent data to parse */
            // EhsConsoleQueue_push(EhsTgtConsoleInputQueueRef,(ehs_uint8*)buff,nLineSize);
        }
        if (EhsFclose(pOut) == 0)
        {
            if ((nSizeRemaining > 0u) && (nSize > 0u))
            {
                /* timeout must have occured */
                EHSH_LOG_ERROR(EHS_MSG_ERROR_FILE_TIMEOUT(name, nSizeRemaining, nSize));
                bRet = EHS_FALSE;
            }
        }
        else
        {
            /* file failed to close */
            EHSH_LOG_ERROR(EHS_MSG_ERROR_FILE_NOT_CLOSE(name));
            bRet = EHS_FALSE;
        }
    }
    else
    {
        EHSH_LOG_ERROR(EHS_MSG_ERROR_FILE_NOT_OPEN(name));
        bRet = EHS_FALSE;
    }

    return bRet;
}
