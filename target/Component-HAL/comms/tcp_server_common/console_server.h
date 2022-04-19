/** @file console_server.h
 * This file contains all of the definitions for the TCP/IP console server
 * 
 * @author: inx limited
 * @version: $Revision: 3643 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

#ifndef EHS_CONSOLE_SERVER_H
#define EHS_CONSOLE_SERVER_H

#include "target_types.h"
#include "target_tcp.h"

//@todo - remove use header type linger in socket.h
typedef struct {
  int l_onoff;
  int l_linger;
} EhsLinger;

/**
 * Thread function handling Tcpip connections to EHS
 * @param[in] pData Dummy parameter, ignored.
 */
EHS_GLOBAL EhsThreadFuncReturnType EhsSvcTcp_server(void* pData);
EHS_GLOBAL ehs_bool EhsSvcTcp_closeConnection(void);

/**
 * Close the TCP/IP connection.
 *
 * @return True if connection was closed successfully.
 */


//#define EHS_TGT_TCP_LOG_ENABLED
#ifdef EHS_TGT_TCP_LOG_ENABLED
/**
 * Log the name of the EhsTgtTcpSocketType error that's just occured with the name of
 * the calling function
 *
 * @param[in] szCaller Console server function and the sockets function it called.
 * @param[in] nErrCode Error code that gives rise to this error message
 */
EHS_GLOBAL void EhsSvcTcp_logSocketError(const ehs_char* szCaller, EhsTgtTcpErrType nErrCode);
#else
#define EhsSvcTcp_logSocketError(x,y) /**< Dummy stub to allow us to ignore this function */
#endif

#endif /* EHS_CONSOLE_SERVER_H */
