/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
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



#define NIPQUAD(addr) \
        ((unsigned char *)&addr)[0], \
        ((unsigned char *)&addr)[1], \
        ((unsigned char *)&addr)[2], \
        ((unsigned char *)&addr)[3]

/**
 * A fault has been detected with TCP/IP. Get an error message corresponding to the current problem.
 *
 * @param[in] nErrCode Code of the error for which we require the error text
 * @return pointer to the error text
 */
const ehs_char * EhsTgtTcp_getErrorText(ehs_char *buff, EhsTgtTcpErrType nErrCode)
{
#if 0
    int szError;
    if (buff != NULL)
    {
        /* Warning there is a GNU specific version of the following that returns a pointer instead of an error code
         * This may only result in a compiler warning so has been disabled.
         */
        szError = strerror_r(nErrCode,buff,EHS_STRING_LENGTH_MAX);
        return szError;
    }
    else return -1;
#else
    EhsStrcpy(buff,"DISABLED ERROR CODE TRANSLATION");
#endif
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
    ehs_sint32 nDataReceived;
    int optval;
    socklen_t optlen = sizeof(optval);
    memset(pData, 0, (size_t)nData); /*lint !e534 Safe to ignore return value here */
    nDataReceived=lwip_recv(xRxSocket, pData, nData,MSG_DONTWAIT);
    if(errno==ENOTCONN)
    {
        nDataReceived = EHS_TGT_TCP_SOCKET_ERROR;
    }
    else
    {
        //check socket is not dead, this is LWIP specific
        if(nDataReceived<0)
        {
            nDataReceived=lwip_getsockopt(xRxSocket, SOL_SOCKET, SO_ERROR, &optval, &optlen);
            if(errno == EWOULDBLOCK)
            {
                nDataReceived = 0;
            }
            else
            {
                nDataReceived = EHS_TGT_TCP_SOCKET_ERROR;
            }
            //so people don't call us in a loop forever
            //todo2023 what is 10 in us or something?
            vTaskDelay(10);
        }
    }
    return nDataReceived;
}

int EhsTgtTcp_select(EhsTgtTcpSocketType xRxSocket)
{
    fd_set readset;
    int32_t retval = -1;
    struct timeval timeout;
    timeout.tv_sec=10;
    timeout.tv_usec=0;
    FD_ZERO(&readset);
    FD_SET(xRxSocket, &readset);

    if (lwip_select(xRxSocket+1, (fd_set *)&readset, 0, 0, &timeout) == 1)
    {
        if (FD_ISSET(xRxSocket, &readset))
        {
            retval = xRxSocket;
        }
    }
    return (retval);
}


ehs_bool EhsSvcTgtTcp_closeConnection(EhsTgtTcpSocketType EhsSvcTcpSocketConnection)
{
    ehs_bool bSuccess = EHS_FALSE; /* assume operation failed */
    int retVal;

    //	retVal = shutdown(EhsSvcTcpSocketConnection,EHS_TGT_TCP_SD_BOTH);

    //	EhsSleep(EHS_TIME_us(EHS_TGT_TCP_SUSPENDTIME_us));
    if ( EhsSvcTcpSocketConnection != EHS_TGT_TCP_INVALID_SOCKET)
    {
        retVal = EHS_TGT_TCP_CLOSE_SOCKET(EhsSvcTcpSocketConnection);

        if (retVal != 0)
        {
            if (EhsTgtTcp_getErrorCode(EHS_TRUE) != EHS_TGT_TCP_ERR_NOTCONN)
            {
#ifdef EHS_TGT_TCP_LOG_ENABLED
                EhsSvcTcp_logSocketError("EhsSvcTcp_closeConnection.shutdown", EhsTgtTcp_getErrorCode(EHS_FALSE));
#endif
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

    }

    return bSuccess; // return success if already closed too
}
