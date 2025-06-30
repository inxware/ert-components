/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file console_server.c
 * Contains the TcpIp server thread used for accessing EHS.
 * Note symbols begining EhsSvcTcp... are defined here and
 * symbols that start with EhsTgtTcp are defined in the target-specific
 * part.
 *
 * @author: inx limited
 *
 */

#define EHS_TARGET_CODE

#include <WiFiNINA.h>

// #define EHSL_MODULE_ID (EHSH_LOG_MODULE_HAL_NETWORK) /* define before hal_logger.h */
#include "hal_logger.h"

#include "target_types.h"
#include "target_tcp.h"
#include "target_network.h"
#include "console_server.h"
#include "console.h" // needed for buffer status flag
#include "globals.h"
#include "hal_console.h"
#include "hal-api.h"

#if EHS_DEBUG_CONSOLE_BUFFER_SIZE < EHS_TGT_TCP_IN_BUFF_SIZE
#error "EHS_DEBUG_CONSOLE_BUFFER_SIZE must be larger or equal to EHS_TGT_TCP_IN_BUFF_SIZE"
#endif

#if EHS_DEBUG_CONSOLE_BUFFER_SIZE < EHS_TGT_TCP_OUT_BUFF_SIZE
#error "EHS_DEBUG_CONSOLE_BUFFER_SIZE must be larger or equal to EHS_TGT_TCP_OUT_BUFF_SIZE"
#endif

/*****************************************************************************/
/* Declare prototypes of local functions */

/**
 * Send data from EHS to the target.
 *
 * @return true if we remain connected to the target
 */
EHS_LOCAL ehs_bool EhsSvcTcp_send(void);

/**
 * Receive data from the target, then pass it on to EHS
 *
 * @return True if the client didn't request to disconnect.
 */
EHS_LOCAL ehs_bool EhsSvcTcp_receive(void);

/**
 * Replace all <esc>E in pData with <esc>, check for existance of
 * <esc>D - indicates that we've received a disconnect instruction
 *
 * @param pData Pointer to the received data
 * @param nSize Amount of data to process
 * @param pbDisconnect True if the disconnect has been received
 * @return size of data now in pData (<= nSize)
 */
EHS_LOCAL ehs_uint32 EhsSvcTcp_expandEscapes(ehs_uint8 *pData, ehs_uint32 nSize, ehs_bool *pbDisconnect);

/**
 * Set up the TCP/IP connection and start waiting for a client to connect.
 */
EHS_LOCAL ehs_bool EhsSvcTcp_waitForClient(void);

/**
 * Open a new TCP/IP connection. This involves setting up a EhsTgtTcpSocketType, binding it
 * then listening for incoming connections.
 *
 * @return True if connection was opened successfully.
 */
EHS_LOCAL ehs_bool EhsSvcTcp_openConnection(void);

/*****************************************************************************/
/* Variables defined with file-scope */
/*
 * Just use server object for the console server
 */
WiFiServer gConsoleServer(EHS_TGT_TCP_PORTNUM);
WiFiClient gConsoleClient;

/* not logging, so stub out calls to log functions */
#define EhsSvcTcp_log(...)
#define EhsSvcTcp_logBinaryData(a, b)


/*****************************************************************************/
/* Function definitions */

/**
 * Open a new TCP/IP connection. This involves setting up a EhsTgtTcpSocketType, binding it
 * then listening for incoming connections.
 *
 * @return True if connection was opened successfully.
 */

ehs_bool EhsSvcTcp_openConnection(void)
{
    ehs_bool ret = EHS_FALSE;
    EhsTPMutex_lock(EhsTPMutex_socketClient);
    if(WiFi.status() == WL_CONNECTED){
        gConsoleServer.begin();
        ret = EHS_TRUE;
    }
    EhsTPMutex_unlock(EhsTPMutex_socketClient);
    return ret;
}

/**
 * Close the TCP/IP connection.
 *
 * @return True if connection was closed successfully.
 */
ehs_bool EhsSvcTcp_closeConnection(void)
{
    ehs_bool ret = EHS_FALSE; /* assume close failed */
    EhsStdioPrintf("Close Console Server ...\n"); 
    return ret;
}

/**
 * Set up the TCP/IP connection and start waiting for a client to connect.
 */
ehs_bool EhsSvcTcp_waitForClient()
{
    ehs_bool ret = EHS_FALSE;
    EhsTPMutex_lock(EhsTPMutex_socketClient);
    gConsoleClient = gConsoleServer.available();
    gConsoleClient.setTimeout(2000); // default 1000ms
    if(gConsoleClient){
        ret = EHS_TRUE;
    }
    EhsTPMutex_unlock(EhsTPMutex_socketClient);
    return ret;
}

/**
 * Replace all <esc>E in pData with <esc>, check for existance of
 * <esc>D - indicates that we've received a disconnect instruction
 *
 * @param pData Pointer to the received data
 * @param nSize Amount of data to process
 * @param pbDisconnect True if the disconnect has been received
 * @return size of data now in pData (<= nSize)
 */

// TODO "PMLD need to change this function to only change the first escape (to read the command) and then it needs to be called from a mnore logical place rather than on the whole received buffer"

// We shouls also write up what the format is in a doc as it is a tool API too.
ehs_uint32 EhsSvcTcp_expandEscapes(ehs_uint8 *pData, ehs_uint32 nSize, ehs_bool *pbDisconnect)
{
    static ehs_bool bLastCharEsc = EHS_FALSE; /* true if the last character of the last packet was 'ESC' */
    /**@todo handle unlikely situation where last packet received ended with ESC, and then we lost the connection */
    ehs_uint32 nCount = 0u;
    while (nCount < nSize)
    {
        if (bLastCharEsc)
        {
            switch (pData[nCount])
            {
            case 'D':
                *pbDisconnect = EHS_TRUE;
                /* if <esc> was last character of last packet, nSize is zero, otherwise it's nCount-1 */
                nSize = (nCount > 0u) ? (nCount - 1u) : 0u;
                break;
            case 'E':
                /* overwrite <esc>E with just <esc>. How to do this depends... */
                /* if E is the first character of the buffer, <esc> was last character of last packet & was ignored */
                if (nCount == 0u)
                {
                    pData[nCount] = 0x1bu;
                    nCount++; /* ensure that we don't "redetect" this escape after the switch statement */
                }
                else
                {
                    /* move pData contents down one cell to overwrite E
                     * e.g if we have 12 entries (nSize = 12) and pCurrent (& nCount)
                     * points to entry 7, we want to move to 7, from 8, entries 8-11
                     * (i.e. len = 4 = (nSize-1) - nCount */
                    nSize--;
                    memmove((void *)(&(pData[nCount])), (const void *)(&(pData[nCount + 1u])), (size_t)(nSize - nCount));
                    // pData[nSize]=0;;// we probably want to set the now repeated last char to a safe value (that isn't a command...)
                }
                break;
            default: /* shouldn't happen */
                EhsSvcTcp_log("Unexpected escape sequence <esc>\0x%x\n", pData[nCount]);
            }
            bLastCharEsc = EHS_FALSE;
            // break;// we don't want to do all the buffer, as we would bugger later binary data...
        }

        if (pData[nCount] == EHS_CHAR_ESC)
        {
            bLastCharEsc = EHS_TRUE;
            /* If the last character in the current buffer is escape - defer processing it until later */
            if ((nCount + 1u) >= nSize)
            {
                nSize--;
            }
        }
        else
            bLastCharEsc = EHS_FALSE;
        nCount++;
    }
    return nSize;
}

/**
 * Receive data from the debugger, then pass it on to EHS
 *
 * @return True if the client didn't request to disconnect.
 */
ehs_bool EhsSvcTcp_receive(void)
{
    ehs_uint8 bBuffIn[EHS_TGT_TCP_IN_BUFF_SIZE]; /* buffer for incoming data */
    ehs_sint32 nDataReceived = EHS_TGT_TCP_SOCKET_ERROR;
    /* amount of data received by TCP/IP */
    ehs_uint8 *pData = bBuffIn;
    ehs_bool bDisconnect = EHS_FALSE; /* assume we want to stay connected */
    //nDataReceived = EhsTgtTcp_recvNonblock(EhsSvcTcpSocketConnection, pData, EHS_TGT_TCP_IN_BUFF_SIZE);
    EhsTPMutex_lock(EhsTPMutex_socketClient);
    if(gConsoleClient.connected()){
        if(gConsoleClient.available()){
            nDataReceived = gConsoleClient.read(pData, EHS_TGT_TCP_IN_BUFF_SIZE);
            if(nDataReceived < 0){
                nDataReceived = 0;
            }
        }else{
            nDataReceived = 0;
        }
    }
    EhsTPMutex_unlock(EhsTPMutex_socketClient);
    if (nDataReceived > 0) /* did we receive data? */
    {
        // do this in the queue pop/push - EhsConsole_buffer_empty = EHS_FALSE; //stop any further reads until this is set back to true.Assuming single threaded write so no mutexing.
        nDataReceived = (ehs_sint32)EhsSvcTcp_expandEscapes(pData, (ehs_uint32)nDataReceived, &bDisconnect);
        if (bDisconnect)
        {
            EhsSvcTcp_log("***Client instructed a disconnect\n");
        }
        EhsSvcTcp_logBinaryData(pData, (ehs_uint32)nDataReceived);

        /* pass received data onto EHS */
        if (EhsTgtConsoleInputQueueRef)
        {
            while (nDataReceived > 0)
            {
                /* push data onto the queue until no data remains */
                EhsTPMutex_lock(EhsTPMutex_consoleInputQueue);
                ehs_sint32 nDataPushed = EhsConsoleQueue_push(EhsTgtConsoleInputQueueRef, pData, (ehs_uint32)nDataReceived);
                EhsTPMutex_unlock(EhsTPMutex_consoleInputQueue);
                EhsSvcTcp_logBinaryData(pData, (ehs_uint32)nDataPushed);

                /* advance pointer to start of data that hasn't yet been pushed */
                pData += nDataPushed;
                nDataReceived = (nDataReceived - nDataPushed);
                /* If EHS couldn't handle everything, give it a chance to consume the data that's on the queue already */
                if (nDataReceived > 0L)
                {
                    // EhsSleep(EHS_TIME_us(5000LL));//EHS_TGT_TCP_SUSPENDTIME_us));
                    // todo the delay here should just be a global default that can be overriden in platform config files.
                    // we have no idea why these are here fi they are conlated with other paltform switches.

                    EhsSleep(EHS_TIME_ms(5));
                }
            }
        }
    }
    else if (nDataReceived == EHS_TGT_TCP_SOCKET_ERROR)
    {
        // PBB 2022-04-14 in my testing nDataReceive==-1 and EhsTgtTcp_getErrorCode(EHS_TRUE)==11
        // indicates the other side has disconnected so we should disconnect as well
        // if(EhsTgtTcp_getErrorCode(EHS_TRUE) != EHS_TGT_TCP_ERR_AGAIN)
        //{
        EhsSvcTcp_logSocketError("EhsSvcTcp_receive.recv", EhsTgtTcp_getErrorCode(EHS_FALSE));
        bDisconnect = EHS_TRUE;
        //}
        EhsSleep(EHS_TIME_ms(100));
    }
    else
    {
        /* we didn't receive anything, but it's ok */
        bDisconnect = EHS_FALSE;
        EhsSleep(EHS_TIME_ms(10));
    }

    return (!bDisconnect); /*lint !e960 This is not an implicity type conversion */
}

/**
 * Send data from EHS to the target.
 *
 * @return true if we remain connected to the target
 */
ehs_bool EhsSvcTcp_send()
{
    ehs_bool bConnected = EHS_TRUE; /* assume that we stay connected */
    if (EhsTgtConsoleOutputQueueRef != NULL)
    {
        /* do we have any data to send? */
        if (!EhsConsoleQueue_isEmpty(EhsTgtConsoleOutputQueueRef))
        {
            ehs_uint8 bBuffOut[EHS_TGT_TCP_OUT_BUFF_SIZE];
            ehs_sint32 nDataSize;

            /* send one bufferful of data. This prevents EHS from monopolising a conversation. */
            nDataSize = (ehs_sint32)EhsConsoleQueue_pop(EhsTgtConsoleOutputQueueRef, bBuffOut, EHS_TGT_TCP_OUT_BUFF_SIZE);
            EhsTPMutex_lock(EhsTPMutex_socketClient);
            /***** send bBuff to TCP/IP ****/
            if (gConsoleClient.connected())
            {
                size_t ret = gConsoleClient.write((const ehs_char *)bBuffOut, (ehs_sint32)nDataSize);
                if(ret != nDataSize){
                    // @TODO - error not all data sent
                }
                if(!gConsoleClient.connected()){
                    bConnected = EHS_FALSE;
                }
            }
            else
            {
                bConnected = EHS_FALSE;
            }
            EhsTPMutex_unlock(EhsTPMutex_socketClient);
        }
    }
    else
    {
        EHSH_LOG_ERROR("EhsTgtConsoleOutputQueueRef is NULL");
        bConnected = EHS_FALSE;
    }
    return bConnected;
}

/**
 * Thread function handling Tcpip connections to EHS.
 * Once a client is connected, alternately poll to see if data has been received by TCP/IP,
 * then if there's outgoing data waiting in the outbound queue.
 * @param[in] pData dummy parameter, ignored.
 */
EhsThreadFuncReturnType EhsSvcTcp_server(void *pData)
{
    /* wait for initialisation to finish */
    EhsStdioPrintf("Console Server Started\n");
    while (!(*EhsHSys_initCompleteRef))
    {
        EhsSleep(EhsTgtTimer_usToTick(EHS_TGT_TCP_SUSPENDTIME_us));
    }
    if (EhsTgtTcp_init() == EHS_FALSE)
    {   
        EhsStdioPrintf("Debug socket not open\n");
    }
    EhsStdioPrintf("Console Server Initialization Complete\n");
    ehs_bool init = EHS_FALSE, running = EHS_TRUE, clientConnected = EHS_FALSE;
    ehs_uint32 failCount = 0, testUsageCount = 0u;
    while (running) /* loop for each client connection */
    {
        // @TODO - run Serial Console

        // Initalise tcpip server
        if(init == EHS_FALSE){
            init = EhsSvcTcp_openConnection();
            EhsSleep(EHS_TIME_ms(50));
            continue;
        }else{
            if(EhsTgtNetworkConnected() == EHS_FALSE){
                EhsStdioPrintf("Network N/A\n");
                // TODO - have a counter before teardown
                continue;
            }
        }
        if(clientConnected == EHS_FALSE){
            clientConnected = EhsSvcTcp_waitForClient();
            EhsSleep(EHS_TIME_ms(50));
            continue;
        }

        // todo2022 is there a way to compare vlues in preprocessor?
        // #if (EHS_TGT_TCP_IN_BUFF_SIZE > (EHS_DEBUG_CONSOLE_BUFFER_SIZE - EHS_FILE_BUFF_SIZE )) // we eill assume the EHS_FILE_BUFF_SIZE is always bigger that what need to red the next command.
        // #error "Your console queue size is smaller than the socket buffer + space for maximum remaining data from previous reads ()"
        // #endif
        //  @todo - mutex should be applied here rather than inside of 'EhsSvcTcp_receive', as length of the queue may change by the time it reaches 'push' in EhsSvcTcp_receive.
        //  it wasn't put here because of a while loop that is inside of 'EhsSvcTcp_receive' and could potetnially block for longer time. (or does it?)
        if (EhsConsoleQueue_space(EhsTgtConsoleInputQueueRef) >= EHS_TGT_TCP_IN_BUFF_SIZE) // only read data if there is enough space in the queue
        {
            clientConnected = EhsSvcTcp_receive();
        }

        EhsSleep(EhsTgtTimer_usToTick(EHS_TGT_TCP_SUSPENDTIME_us)); /* Make this more friendly when there is a connection & data transfer */

        if (clientConnected == EHS_TRUE)
        {
            clientConnected = EhsSvcTcp_send();
        }

        if(clientConnected == EHS_FALSE){
            // tidy up clinet if disconnected
            EhsTPMutex_lock(EhsTPMutex_socketClient);
            gConsoleClient.stop();
            EhsTPMutex_unlock(EhsTPMutex_socketClient);
        }
    }
    EhsTgtTcp_term();
    EhsHThread_exit();
}
