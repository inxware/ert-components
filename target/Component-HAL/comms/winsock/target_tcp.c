/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file target_tcp.c
 * Definitions for target-specific tcp-related functions
 *
 *
 * @author: inx limited
 *
 */

#define EHS_TGT_CODE /* Ensure header files include target-internal values #todo explain this or remove it - seeems random */

#include "target_tcp.h"
#include "console_server.h"

/**
 * A fault has been detected with TCP/IP. Get an error message corresponding to the current problem.
 *
 * @param[in] nErrCode Code of the error for which we require the error text
 * @return pointer to the error text
 */
const ehs_char* EhsTgtTcp_getErrorText(ehs_char* szError, EhsTgtTcpErrType nErrCode)
{
    //const ehs_char* szError;

    switch (nErrCode)
    {
    case WSAEACCES:
        szError = "WSAEACCES: Attempt to connect datagram EhsTgtTcpSocketType to broadcast address failed because setsockopt option SO_BROADCAST is not enabled. ";
        break;
    case WSAEADDRINUSE:
        szError = "WSAEADDRINUSE: A process on the computer is already bound to the same fully-qualified address and the EhsTgtTcpSocketType has not been marked to allow address reuse with SO_REUSEADDR. For example, the IP address and port are bound in the af_inet case). (See the SO_REUSEADDR EhsTgtTcpSocketType option under setsockopt.) ";
        break;
    case WSAEADDRNOTAVAIL:
        szError = "WSAEADDRNOTAVAIL: The specified address is not a valid address for this computer. ";
        break;
    case WSAEAFNOSUPPORT:
        szError = "WSAEAFNOSUPPORT: The specified address family is not supported. ";
        break;
    case WSAECONNABORTED:
        szError = "WSAECONNABORTED: The virtual circuit was terminated due to a time-out or other failure. The application should close the EhsTgtTcpSocketType as it is no longer usable. ";
        break;
    case WSAECONNRESET:
        szError = "WSAECONNRESET: An incoming connection was indicated, but was subsequently terminated by the remote peer prior to accepting the call or the virtual circuit was reset by the remote side executing a hard or abortive close. ";
        break;
    case WSAEFAULT:
        szError = "WSAEFAULT: Possible causes: len parameter too small, addr is not a valid part of the user address space or the Windows EhsTgtTcpSocketTypes implementation was unable to allocate needed resources for its internal operations, or the readfds, writefds, exceptfds, or timeval parameters are not part of the user address space. ";
        break;
    case WSAEHOSTUNREACH:
        szError = "WSAEHOSTUNREACH: The remote host cannot be reached from this host at this time. ";
        break;
    case WSAEINPROGRESS:
        szError = "WSAEINPROGRESS: A blocking Windows EhsTgtTcpSocketTypes 1.1 call is in progress, or the service provider is still processing a callback function. ";
        break;
    case WSAEINTR:
        szError = "WSAEINTR: The (blocking) call was cancelled through WSACancelBlockingCall. ";
        break;
    case WSAEINVAL:
        szError = "WSAEINVAL: Possible causes:\n\tThe listen function was not invoked prior to accept. \nThe EhsTgtTcpSocketType has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a EhsTgtTcpSocketType with SO_OOBINLINE enabled or (for byte stream EhsTgtTcpSocketTypes only) len was zero or negative. \n\tThe EhsTgtTcpSocketType is already bound to an address. \n\tWSAEINVAL The time-out value is not valid, or all three descriptor parameters were null. \n\tThe how parameter is not valid, or is not consistent with the EhsTgtTcpSocketType type. For example, SD_SEND is used with a UNI_RECV EhsTgtTcpSocketType type.";
        break;
    case WSAEISCONN:
        szError = "WSAEISCONN: The EhsTgtTcpSocketType is already connected. ";
        break;
    case WSAEMFILE:
        szError = "WSAEMFILE: No more EhsTgtTcpSocketType descriptors are available, or the queue is nonempty upon entry to accept and there are no descriptors available. ";
        break;
    case WSAEMSGSIZE:
        szError = "WSAEMSGSIZE: The message was too large to fit into the specified buffer and was truncated, or the EhsTgtTcpSocketType is message oriented, and the message is larger than the maximum supported by the underlying transport. ";
        break;
    case WSAENETDOWN:
        szError = "WSAENETDOWN: The network subsystem has failed. ";
        break;
    case WSAENETRESET:
        szError = "WSAENETRESET: The connection has been broken due to the keep-alive activity detecting a failure while the operation was in progress. ";
        break;
    case WSAENOBUFS:
        szError = "WSAENOBUFS: No buffer space is available/Not enough buffers available, too many connections. ";
        break;
    case WSAENOTCONN:
        szError = "WSAENOTCONN: The EhsTgtTcpSocketType is not connected. ";
        break;
    case WSAENOTSOCK:
        szError = "WSAENOTSOCK: One of the descriptor sets contains an entry that is not a EhsTgtTcpSocketType/The descriptor is not a EhsTgtTcpSocketType. ";
        break;
    case WSAEOPNOTSUPP:
        szError = "WSAEOPNOTSUPP: The referenced EhsTgtTcpSocketType is not a type that supports:\n\tconnection-oriented service.\n\tthe listen operation\n\tOOB data ";
        break;
    case WSAEPROCLIM:
        szError = "WSAEPROCLIM: Limit on the number of tasks supported by the Windows EhsTgtTcpSocketTypes implementation has been reached. ";
        break;
    case WSAEPROTONOSUPPORT:
        szError = "WSAEPROTONOSUPPORT: The specified protocol is not supported. ";
        break;
    case WSAEPROTOTYPE:
        szError = "WSAEPROTOTYPE: The specified protocol is the wrong type for this EhsTgtTcpSocketType. ";
        break;
    case WSAESHUTDOWN:
        szError = "WSAESHUTDOWN: The EhsTgtTcpSocketType has been shut down; it is not possible to receive or send on a EhsTgtTcpSocketType after shutdown has been invoked with how set to SD_BOTH. ";
        break;
    case WSAESOCKTNOSUPPORT:
        szError = "WSAESOCKTNOSUPPORT: The specified EhsTgtTcpSocketType type is not supported in this address family. ";
        break;
    case WSAETIMEDOUT:
        szError = "WSAETIMEDOUT: The connection has been dropped because of a network failure or because the peer system failed to respond or because the system on the other end went down without notice. ";
        break;
    case WSAEWOULDBLOCK:
        szError = "WSAEWOULDBLOCK: The EhsTgtTcpSocketType is marked as nonblocking and either:\n\tno connections are present to be accepted. \n\tthe requested operation would block.\n\t SO_LINGER is set to a nonzero time-out value";
        break;
    case WSANOTINITIALISED:
        szError = "WSANOTINITIALISED: A successful WSAStartup call must occur before using this function. ";
        break;
    case WSASYSNOTREADY:
        szError = "WSASYSNOTREADY: Indicates that the underlying network subsystem is not ready for network communication. ";
        break;
    case WSAVERNOTSUPPORTED:
        szError = "WSAVERNOTSUPPORTED: The version of Windows EhsTgtTcpSocketTypes support requested is not provided by this particular Windows EhsTgtTcpSocketTypes implementation. ";
        break;
    default:
        szError = "Unrecognised winsock error";
        break;
    }

    return szError;
}

/**
 * Target-specific initialisation of Tcpip. Also waits until the main
 * EHS thread is up and running.
 *
 * @return true if initialisation was successful
 */
ehs_bool EhsTgtTcp_init(void)
{
    WSADATA wsa_data; /* holds details of Windows EhsTgtTcpSocketTypes implementation. Ignored */
    ehs_bool ret; /* Indicates success or otherwise of the init function */

    /* Initialise Winsock using version 2.2 winsock */
    ehs_sint32 Result = WSAStartup(MAKEWORD(2u,2u), &wsa_data );
    if ( Result == NO_ERROR )
    {
        /* Initialisation was successful */
        ret = EHS_TRUE;
    }
    else
    {
        EhsSvcTcp_logSocketError("EhsSvcTcp_init.WSAStartup", EhsTgtTcp_getErrorCode(EHS_TRUE));
        ret = EHS_FALSE;
    }

    return ret;
}


/**
 * Target-specific shutdown for the TCP/IP connection
 */
void EhsTgtTcp_term(void)
{
    WSACleanup();
}

/**
 * Target-specific non-blocking receive of Tcpip.
 *
 * @param[in] xRxSocket Socket to receive data on
 * @param[out] pData Pointer to memory to contain received data
 * @param[in] nData Maximum amount of data to return
 * @return number of bytes returned or EHS_TGT_TCP_SOCKET_ERROR
 */
ehs_sint32 EhsTgtTcp_recvNonblock(EhsTgtTcpSocketType xRxSocket, ehs_uint8* pData, ehs_uint32 nData)
{
    ehs_sint32 nSelectResp; /* response from select statement */
    ehs_sint32 nDataReceived; /* amount of data received or socket error */
    fd_set xSocketSet; /* EhsTgtTcpSocketTypes to check for readiness */
    struct timeval timeout; /* how long to check input for */

    /* listen for data from TCPIP connection for EHS_TGT_TCP_LISTENTIME_us */
    FD_ZERO(&xSocketSet);
    FD_SET(xRxSocket, &xSocketSet);
    timeout.tv_sec = 0;
    timeout.tv_usec = EHS_TGT_TCP_LISTENTIME_us;

    nSelectResp = select(0,&xSocketSet,NULL,NULL,&timeout);

    /* did we get an answer? */
    if (nSelectResp > 0)
    {
        memset(pData, 0, nData);
        nDataReceived = recv(xRxSocket, (ehs_char*)pData, EHS_TGT_TCP_IN_BUFF_SIZE, 0);
    }
    else if (nSelectResp == EHS_TGT_TCP_SOCKET_ERROR)
    {
        nDataReceived = EHS_TGT_TCP_SOCKET_ERROR;
    }
    else /* select timed out */
    {
        nDataReceived = 0;
    }

    return nDataReceived;
}


/**
 * Get the last error detected for the TCP/IP operation.
 *
 * @param[in] bFirstUse True if this is the first use of this call after a socket call, false if
 * we're asking for the same error number again
 */
EhsTgtTcpErrType EhsTgtTcp_getErrorCode(ehs_bool bFirstUse)
{
    static EhsTgtTcpErrType nErrCode = WSANOTINITIALISED;

    if (bFirstUse)
    {
        nErrCode = WSAGetLastError();
    }

    return nErrCode;
}

/**
 * Close the connection EhsTgtTcpSocketType
 */
ehs_bool EhsSvcTgtTcp_closeConnection(EhsTgtTcpSocketType EhsSvcTcpSocketConnection)
{
    ehs_bool bSuccess = EHS_FALSE; /* assume operation failed */
    if ( EhsSvcTcpSocketConnection != EHS_TGT_TCP_INVALID_SOCKET)
    {
        if (shutdown(EhsSvcTcpSocketConnection,EHS_TGT_TCP_SD_BOTH) != 0)
        {
            if (EhsTgtTcp_getErrorCode(EHS_TRUE) != EHS_TGT_TCP_ERR_NOTCONN)
            {
                EhsSvcTcp_logSocketError("EhsSvcTcp_closeConnection.shutdown", EhsTgtTcp_getErrorCode(EHS_FALSE));
            }
            else
            {
                /* Ignore this warning, as we're already shutdown */
                bSuccess = EHS_TRUE;
            }
        }
        else
        {
            bSuccess = EHS_TRUE;
        }

        if (bSuccess)
        {
            if (EHS_TGT_TCP_CLOSE_SOCKET(EhsSvcTcpSocketConnection) != 0)
            {
                EhsSvcTcp_logSocketError("EhsSvcTcp_closeConnection.closeEhsTgtTcpSocketType", EhsTgtTcp_getErrorCode(EHS_TRUE));
            }
        }
        else
        {
            bSuccess = EHS_TRUE;
        }
    }
    return bSuccess;
}
