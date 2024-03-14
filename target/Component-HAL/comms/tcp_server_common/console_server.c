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

// #define EHSL_MODULE_ID (EHSH_LOG_MODULE_HAL_NETWORK) /* define before hal_logger.h */
#include "hal_logger.h"

#include "target_types.h"
#include "target_tcp.h"
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

/**
 * Socket used for listening for new connections
 */
EHS_LOCAL EhsTgtTcpSocketType EhsSvcTcpSocketListen;

/**
 * Socket used for the main connection between EHS and its client
 */
EHS_LOCAL EhsTgtTcpSocketType EhsSvcTcpSocketConnection;

/*****************************************************************************/
/* Provide configurable logging support */
#ifdef EHS_TGT_TCP_LOG_ENABLED

/**
 * Write log data to the log file.
 */
EHS_LOCAL void EhsSvcTcp_log(const ehs_char *params, ...); /*lint !e960 Variable arguments required to support generalized printf */

/**
 * Log (potentially) binary data to the log file
 *
 * @param pData pointer to the data to log
 * @param nDataReceived amount of data to log
 */
EHS_LOCAL void EhsSvcTcp_logBinaryData(const ehs_uint8 *pData, ehs_uint32 nDataReceived);

/**
 * How long line lengths should be when displaying data
 */
#define EHS_SVC_TCP_LOG_LINELEN 80u

/**
 * File that receives all log data
 */
EHS_LOCAL ehs_FILE *EhsSvcTcpLogfile;

/**
 * Write log data to the log file.
 */
void EhsSvcTcp_log(const ehs_char *params, ...) /*lint !e960 Variable arguments required to support generalized printf */
{
    /* Disabling as getting errors - perhaps we need to check the file handle? */
#if 0
    va_list argptr;
    ehs_uint32 nTime_us = EhsTgtTimer_tickTous(EhsCurrentTime());
    fprintf(EhsSvcTcpLogfile,"%03d.%03d %03d\t", nTime_us / 1000000u, (nTime_us / 1000u) % 1000u, nTime_us % 1000u);
    va_start (argptr,params);
    vfprintf(EhsSvcTcpLogfile,params,argptr); /*lint !e534 Ignoring return value of function - simply not of interest here */
    va_end(argptr);
    fflush(EhsSvcTcpLogfile);
#endif
}

/**
 * Log (potentially) binary data to the log file
 *
 * @param pData pointer to the data to log
 * @param nDataReceived amount of data to log
 */
void EhsSvcTcp_logBinaryData(const ehs_uint8 *pData, ehs_uint32 nDataReceived)
{
    ehs_uint8 lLen = 0u; /* length of the line output so far */
    ehs_uint32 i;        /* index into the data received */

    for (i = 0u; i < nDataReceived; i++)
    {
        ehs_uint8 cOut = pData[i]; /* character received */

        /* begin new lines with a tab */
        if (lLen == 0u)
        {
            EhsFputc('\t', EhsSvcTcpLogfile);
        }

        /* filter out funny (i.e. control) characters */
        cOut = (((cOut > 0x20u) && (cOut < 0x7fu)) || (cOut == EHS_CHAR_LF)) ? cOut : ((ehs_uint8)('.'));

        /* output the character */
        EhsFputc((ehs_sint32)cOut, EhsSvcTcpLogfile);

        /* break the line if it exceeds EHS_SVC_TCP_LOG_LINELEN characters */
        lLen = (cOut == EHS_CHAR_LF) ? 0u : (lLen + 1u);
        if (lLen > EHS_SVC_TCP_LOG_LINELEN)
        {
            EhsFputc((ehs_sint32)EHS_CHAR_LF, EhsSvcTcpLogfile);
            lLen = 0u;
        }
    }

    /* force a write of the file */
    fflush(EhsSvcTcpLogfile);
}

/**
 * Log the name of the EhsTgtTcpSocketType error that's just occured with the name of
 * the calling function
 *
 * @param[in] szCaller Console server function and the sockets function it called.
 * @param[in] nErrCode Error code that gives rise to this error message
 */

void EhsSvcTcp_logSocketError(const ehs_char *szCaller, EhsTgtTcpErrType nErrCode)
{
    ehs_char szError[EHS_STRING_LENGTH_MAX];
    szError[0] = '\0';
    const ehs_sint8 OK_ZERO = EhsTgtTcp_getErrorText(szError, nErrCode);
    if (OK_ZERO == 0)
    {
        EhsSvcTcp_log("**Error: TCP/IP error (%-d) in %s: %s\n", nErrCode, szCaller, szError);
    }
    else
    {
        EhsSvcTcp_log("**Unidentifid Error: TCP/IP error (%-d) in %s\n", nErrCode, szCaller);
    }
}

#else
/* not logging, so stub out calls to log functions */
#define EhsSvcTcp_log(x, ...)
#define EhsSvcTcp_logBinaryData(a, b)
#endif

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
    EhsTgtTcpSockAddrInTypeWindows connection; /* used to construct the address of the EhsTgtTcpSocketType we're creating */
    ehs_bool ret = EHS_FALSE;                  /* assume initialisation fails */
    // ehs_uint8 retVal;

    /* set up connection ready for bind */
    //printf("OC1\n");
    memset(&connection, 0, sizeof(connection));
    //printf("OC2\n");
    connection.sin_family = AF_INET;                  // ipv4 only
    connection.sin_addr.s_addr = htonl(INADDR_ANY);   // listen on any interface
    connection.sin_port = htons(EHS_TGT_TCP_PORTNUM); // use our magic port number
    //printf("OC3\n");
    /* Create EhsTgtTcpSocketType */
    EhsSvcTcpSocketListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //printf("OC4\n");
    if (EhsSvcTcpSocketListen == EHS_TGT_TCP_INVALID_SOCKET)
    {
        //printf("Could not Open debugger socket - invalid socket");
        EhsSvcTcp_logSocketError("EhsSvcTcp_openConnection.EhsTgtTcpSocketType", EhsTgtTcp_getErrorCode(EHS_TRUE));
        return ret;
    }

    //PBB 2023-03-21 we set this so that we can get back our listening socket if the server is restarted
    //PBB 2023-04-05 we must use the system type int or this command fails
    const int so_reuseaddr = 1;
    const int result=setsockopt(EhsSvcTcpSocketListen, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr, sizeof(so_reuseaddr));
    if(result!=0){
        //printf("Warning: Could not set SO_REUSEADDR\n");
        EhsSvcTcp_logSocketError("EhsSvcTcp_openConnection.setsockopt", EhsTgtTcp_getErrorCode(EHS_TRUE));
    }

    /* Bind EhsTgtTcpSocketType */
    if (bind(EhsSvcTcpSocketListen, (EhsTgtTcpSockAddrType *)&connection, (ehs_sint32)sizeof(connection)) == EHS_TGT_TCP_SOCKET_ERROR)
    {
        ///printf("Could not bind debugger socket\n");
        EhsSvcTcp_logSocketError("EhsSvcTcp_openConnection.bind", EhsTgtTcp_getErrorCode(EHS_TRUE));
    }
    /* Listen for incoming connections. Allows for 1 pending connection only */
    else if (listen(EhsSvcTcpSocketListen, 1) == EHS_TGT_TCP_SOCKET_ERROR)
    {
        //printf("Could not listen on opened socket - client already connected");
        EhsSvcTcp_logSocketError("EhsSvcTcp_openConnection.listen", EhsTgtTcp_getErrorCode(EHS_TRUE));
    }
    else
    {
        ret = EHS_TRUE;
    }

    /* We seem to run this whatever the socket is ...
     * this sets the socket to use linger (l_onoff = 1) but have a linger time of 0 (l_linger = 0)
     * and is required in order for the socket to close immediately after close() is called
     * It is recommended to use these settings for SO_LINGER rather than turn linger off (l_onoff = 0)
     * which may have implementation specific consequences - ie might use TIME_WAIT before releasing socket
     */
#ifdef EHS_ENABLE_SOLINGER

    if (EhsSvcTcpSocketListen)
    {
        EhsLinger Option;
        Option.l_onoff = 1;
        Option.l_linger = 0;
        retVal = setsockopt(EhsSvcTcpSocketListen, SOL_SOCKET, SO_LINGER, (const char *)&Option, sizeof(EhsLinger));
    }
#endif
    //printf("XXX Socket exit\n");
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

    //	while (!ret) {
    ret = EhsSvcTgtTcp_closeConnection(EhsSvcTcpSocketConnection);
    //		EhsSleep(EHS_TIME_us(EHS_TGT_TCP_SUSPENDTIME_us));
    //	}

    //	ret = close(EhsSvcTcpSocketListen);

    //	ret = close(EhsSvcTcpSocketConnection);

    return ret;
}

/**
 * Set up the TCP/IP connection and start waiting for a client to connect.
 */
ehs_bool EhsSvcTcp_waitForClient()
{
    // ehs_uint8 retVal;

    EhsSvcTcp_log("***Waiting for client\n");
    EhsSvcTcpSocketConnection = EHS_TGT_TCP_INVALID_SOCKET;
    while (EhsSvcTcpSocketConnection == EHS_TGT_TCP_INVALID_SOCKET)
    {
        EhsSvcTcpSocketConnection = accept(EhsSvcTcpSocketListen, NULL, NULL);
        if (EhsSvcTcpSocketConnection == EHS_TGT_TCP_INVALID_SOCKET)
        {
            EhsSvcTcp_logSocketError("EhsSvcTcp_waitForClient.accept (ignoring)", EhsTgtTcp_getErrorCode(EHS_TRUE));
            return EHS_FALSE;
        }
        // todo the delay here should just be a global default that can be overriden in platform config files.
        //  we have no idea why these are here fi they are conlated with other paltform switches.
#ifdef EHS_LWIP
        // accept is blocking on lwip
#else
        EhsSleepUs(500000); //* Don#t loop too quick
#endif
    }
    return EHS_TRUE;
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
    ehs_sint32 nDataReceived;
    /* amount of data received by TCP/IP */
    ehs_uint8 *pData = bBuffIn;
    ehs_bool bDisconnect = EHS_FALSE; /* assume we want to stay connected */
    nDataReceived = EhsTgtTcp_recvNonblock(EhsSvcTcpSocketConnection, pData, EHS_TGT_TCP_IN_BUFF_SIZE);
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

#ifdef EHS_LWIP
            EhsSleepUs(20000LL); // 20ms?
            //EhsSleep(20); //todo2023 - what is 20 this time? Presume ticks
#else  // #ifdef EHS_LWIP
            EhsSleepUs(EHS_TGT_TCP_SUSPENDTIME_us);
#endif // #else #ifdef EHS_LWIP
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
#ifdef EHS_LWIP
        // Wait 100ms if there is a socket error
        EhsSleepUs(100000LL);
#endif
    }
    else
    {
        /* we didn't receive anything, but it's ok */
        bDisconnect = EHS_FALSE;
#ifdef EHS_LWIP
        EhsSleepUs(10000);
#endif
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
            // EHSH_LOG_INFO("QPop: in = %d, out = %d\n", EhsTgtConsoleOutputQueueRef->uInIdx, EhsTgtConsoleOutputQueueRef->uOutIdx);
            nDataSize = (ehs_sint32)EhsConsoleQueue_pop(EhsTgtConsoleOutputQueueRef, bBuffOut, EHS_TGT_TCP_OUT_BUFF_SIZE);
            EhsSvcTcp_log("***Sent %d bytes\n", nDataSize);
            // printf("***Sent %d bytes\n", nDataSize);
            EhsSvcTcp_logBinaryData((const ehs_uint8 *)bBuffOut, (ehs_uint32)nDataSize);
            // bBuffOut[nDataSize] = '\0';
            /***** send bBuff to TCP/IP ****/
            if (EhsSvcTcpSocketConnection)
            {
                if (EHS_TGT_TCP_SOCKET_ERROR == send((EhsTgtTcpSocketType)EhsSvcTcpSocketConnection, (const ehs_char *)bBuffOut, (ehs_sint32)nDataSize, (ehs_sint32)0))
                {
                    EhsSvcTcp_logSocketError("EhsSvcTcp_send.send", EhsTgtTcp_getErrorCode(EHS_TRUE));
                    bConnected = EHS_FALSE;
                }
                else
                {
                    // printf("PBB console_server:497 %d\n",nDataSize);
                }
            }
            else
            {
                EHSH_LOG_ERROR("EhsSvcTcpSocketConnection is NULL");
                bConnected = EHS_FALSE;
            }
        }
        else
        {
            // printf("PBB 506 queue is empty\n");
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
    ehs_bool init;
    ehs_uint32 failCount = 0;
#ifdef EHS_TGT_TCP_LOG_ENABLED
    EhsSvcTcpLogfile = Ehs_SysFopen("ehs_tcpip.log", "w"); /* make this in the current directory  */
#endif
    /* wait for initialisation to finish */
    EhsSvcTcp_log("Server started\n");
    //printf("Socket Server Started..\n");
    while (!(*EhsHSys_initCompleteRef))
    {
        EhsSleep(EhsTgtTimer_usToTick(EHS_TGT_TCP_SUSPENDTIME_us));
        EhsSvcTcp_log("waiting for EHS initialization to complete\n");
    }
    //printf("VVV-1\n");
    init = EhsTgtTcp_init();
    EhsSvcTcp_log("Initialization complete\n");
    //printf("Socket Server Started..\n");
    //printf("VVV-2 init=%d\n",init);
    if (init)
    {
        init = EhsSvcTcp_openConnection();
        //printf("VVV-3 init=%d\n",init);
        if (init)
            EhsSvcTcp_log("Connection open\n");
        else
            EHSH_LOG_ERROR("Debug socket not open");
    }
    //printf("SS 2\n");
    while (init) /* loop for each client connection */
    {
        /* tcpip initialisation */
        ehs_bool ClientConnected = EHS_TRUE;
        ehs_uint32 TestUsageCount = 0u;
        //printf("SS 3\n");
        if (EhsSvcTcp_waitForClient() == EHS_FALSE)
        {
            failCount++;
            if (failCount > 10)
            {
                EHSH_LOG_ERROR("Exiting debug listening socket");
                break; /* Block until we get a client or bail if we get something nasty*/
            }
            else
            {
                EhsSleepUs(50000); /* pause a bit before trying again to be nice */
                continue;
            }
        }
        failCount = 0;
        EhsSvcTcp_log("***Client connected\n");
        //printf("ClientConnected = %d\n", ClientConnected);
        while (ClientConnected) /*shunt data in and out of client and detect if we close somehow */
        {
            TestUsageCount++;

            // todo2022 is there a way to compare vlues in preprocessor?
            // #if (EHS_TGT_TCP_IN_BUFF_SIZE > (EHS_DEBUG_CONSOLE_BUFFER_SIZE - EHS_FILE_BUFF_SIZE )) // we eill assume the EHS_FILE_BUFF_SIZE is always bigger that what need to red the next command.
            // #error "Your console queue size is smaller than the socket buffer + space for maximum remaining data from previous reads ()"
            // #endif
            //  @todo - mutex should be applied here rather than inside of 'EhsSvcTcp_receive', as length of the queue may change by the time it reaches 'push' in EhsSvcTcp_receive.
            //  it wasn't put here because of a while loop that is inside of 'EhsSvcTcp_receive' and could potetnially block for longer time. (or does it?)
            if (EhsConsoleQueue_space(EhsTgtConsoleInputQueueRef) >= EHS_TGT_TCP_IN_BUFF_SIZE) // only read data if there is enough space in the queue
            {

                ClientConnected = EhsSvcTcp_receive();
                // int size;
                // if (size = EhsConsoleQueue_length(EhsTgtConsoleInputQueueRef) > 0) {
                // }
            }
            else
            {
            }
#ifdef EHS_NXP_SUPPORT
            // PBB 2022-10-13 This sleep is important or else on the NXP
            // you get stuck in a loop because the console queue is full but without a sleep
            // it can't be cleared by the other threads
            EhsSleepUs((EHS_TGT_TCP_SUSPENDTIME_us*2)); // todo this should be changed to use a mcro so we know what this time actually is
#else
            EhsSleepUs((EHS_TGT_TCP_SUSPENDTIME_us)); /* Make this more friendly when there is a connection & data transfer */

            // EHS_TGT_TCP_SUSPENDTIME_us
#endif
            if (ClientConnected)
            {
                ClientConnected = EhsSvcTcp_send();
            }

            if (EhsTgtProcess_isOrphan()) /* todo this needs implementing */
            {
                EhsSvcTgtTcp_closeConnection(EhsSvcTcpSocketConnection);
                init = EHS_FALSE;
                break;
            }
        }
        init = EhsSvcTgtTcp_closeConnection(EhsSvcTcpSocketConnection);
        EhsSvcTcp_log("UsageCount %d\n", TestUsageCount);
        //printf("UsageCount closed %d\n", TestUsageCount);
    }
    //printf ("Existing console server\n");
    EhsTgtTcp_term();
    EhsHThread_exit();
}
