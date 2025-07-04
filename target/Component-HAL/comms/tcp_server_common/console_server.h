/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file console_server.h
 * This file contains all of the definitions for the TCP/IP console server
 *
 * @author: inx limited
 *
 */

#ifndef EHS_CONSOLE_SERVER_H
#define EHS_CONSOLE_SERVER_H

#include "target_types.h"

// This is not needed in the header as the console should have any TCP 
// #include "target_tcp.h"

//@todo - remove use header type linger in socket.h
typedef struct
{
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
