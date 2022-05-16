/** @file target_tcp.h
 * This file contains all of the target-specific definitions for using TCP/IP
 *
 * @author: inx limited
 * @version: $Revision: 3760 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 *
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

#ifndef EHS_TARGET_TCP_H
#define EHS_TARGET_TCP_H

#include "target.h"

#ifdef EHS_DEBUG_TCPIP_CONSOLE
#include "console_queue.h"
#endif

#include "sockets.h"

/**
 * Used to perform Logging of TCP/IP data
 */
/*
//#define EHS_TGT_TCP_LOG_ENABLED //default off
@todo this should go into the main debug condfig header file
*/
#define EHS_TGT_TCP_OUT_BUFF_SIZE  64u	/**< Size of buffer to use when sending to TCP/IP */
#define EHS_TGT_TCP_IN_BUFF_SIZE  32u	/**< Size of buffer to use when receiving from TCP/IP */
#define EHS_TGT_TCP_PORTNUM  11425u			/**< Port number to use for receiving TCP/IP connections */

#define EHS_TGT_TCP_LISTENTIME_us 20000u /**< Time to listen (uS) for incoming TCP/IP data */
#define EHS_TGT_TCP_SUSPENDTIME_us 20000u /**< Time to sleep (uS) to allow main EHS thread to do processing */

#define EHS_TGT_TCP_INVALID_SOCKET (-1)	/**< Value returned from socket to indicate failure */
#define EHS_TGT_TCP_SOCKET_ERROR (-1) /**< Value to be returned from socket-using functions to indicate failure */
#define EHS_TGT_TCP_SD_BOTH (2) /**< Indicates that a shutdown is to apply to both read and write channels */

#define EHS_TGT_TCP_CLOSE_SOCKET close /**< Function called to close a socket */

/**
 * Allows the socket type to be hidden
 */
typedef int EhsTgtTcpSocketType;

//typedef struct sockaddr_in EhsTgtTcpSockAddrInType;

typedef struct addrinfo EhsTgtTcpSockAddrInType;


typedef struct sockaddr_in EhsTgtTcpSockAddrInTypeWindows; /*todo rename these */

typedef struct sockaddr EhsTgtTcpSockAddrType;

/*
 * Abstraction of the type used to report error numbers
 */
typedef int EhsTgtTcpErrType;

/* Specific error codes required by console_server */

/**
 * Error code: Socket is not connected
 */
#define EHS_TGT_TCP_ERR_NOTCONN ENOTCONN
/**
 * Error code: try again. Returned by recv()
 */
#define EHS_TGT_TCP_ERR_AGAIN EAGAIN

/**
 * Get the last error detected for the TCP/IP operation.
 *
 * @param[in] bFirstUse True if this is the first use of this call after a socket call, false if
 * we're asking for the same error number again
 */
#define EhsTgtTcp_getErrorCode(x) (errno)
#ifndef EhsTgtTcp_getErrorCode
EHS_GLOBAL EhsTgtTcpErrType EhsTgtTcp_getErrorCode(ehs_bool bFirstUse);
#endif


/**
 * A fault has been detected with TCP/IP. Get an error message corresponding to the current problem.
 *
 * @param[in] nErrCode Code of the error for which we require the error text
 * @return pointer to the error text
 */
const ehs_char *  EhsTgtTcp_getErrorText(ehs_char * buff, EhsTgtTcpErrType nErrCode);

/**
 * Target-specific shutdown for the TCP/IP connection
 */
EHS_GLOBAL void EhsTgtTcp_term(void);

/**
 * Target-specific initialisation of Tcpip. Also waits until the main
 * EHS thread is up and running.
 *
 * @return true if initialisation was successful
 */
EHS_GLOBAL ehs_bool EhsTgtTcp_init(void);

/**
 * Target-specific non-blocking receive of Tcpip.
 *
 * @param[in] xRxSocket Socket to receive data on
 * @param[out] pData Pointer to memory to contain received data
 * @param[in] nData Maximum amount of data to return
 * @return number of bytes returned or EHS_TGT_TCP_SOCKET_ERROR
 */
EHS_GLOBAL ehs_sint32 EhsTgtTcp_recvNonblock(EhsTgtTcpSocketType xRxSocket, ehs_uint8* pData, ehs_uint32 nData);

/*Closes connection * Added by PB 08/08/2008*/

EHS_GLOBAL ehs_bool EhsSvcTgtTcp_closeConnection(EhsTgtTcpSocketType EhsSvcTcpSocketConnection);

#endif /* EHS_TARGET_TCP_H */
