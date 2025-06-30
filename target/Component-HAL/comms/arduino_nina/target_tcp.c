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

/**
 * @page Verification Verification report
 * @section target_tcp
 * @anchor target_tcp
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on 2007-10-12
 * Last modified on $Date$
 *
 * This file contained no derogations to the MISRA standard.
 *
 * Note it is necessary to replace <sys/types.h> with <types.h> to lint this file successfully.
 */


/**
 * Provides access to the target-specific declarations of header files
 */
#define EHS_TARGET_CODE

/*****************************************************************************/
/* Included files */

#include <string.h> /* required for memset */
#include "target_tcp.h"
#include "hal_string.h"
#ifdef EHS_DEBUG_TCPIP_CONSOLE
#include "console_server.h"
#endif



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
 * A fault has been detected with TCP/IP. Get an error message corresponding to the current problem.
 *
 * @param[in] nErrCode Code of the error for which we require the error text
 * @return pointer to the error text
 */
const ehs_char * EhsTgtTcp_getErrorText(ehs_char *buff, EhsTgtTcpErrType nErrCode)
{
    EhsStrcpy(buff,"DISABLED ERROR CODE TRANSLATION");
    return buff;
}

/**
 * Target-specific initialisation of Tcpip. Also waits until the main
 * EHS thread is up and running.
 *
 * @return true if initialisation was successful
 */
ehs_bool EhsTgtTcp_init(void)
{
    /* no special initialisation action is required */
    return EHS_TRUE;
}

/**
 * Target-specific shutdown for the TCP/IP connection
 */
void EhsTgtTcp_term(void)
{
    /* no specific action required here */
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
    ehs_sint32 nDataReceived=0;
    
    return nDataReceived;
}

int EhsTgtTcp_select(EhsTgtTcpSocketType xRxSocket)
{

    int32_t retval = -1;
    
    return (retval);
}


ehs_bool EhsSvcTgtTcp_closeConnection(EhsTgtTcpSocketType EhsSvcTcpSocketConnection)
{
    ehs_bool bSuccess = EHS_FALSE; /* assume operation failed */
   
    return bSuccess; // return success if already closed too
}
