/*
 * SPDX-FileCopyrightText: 2013 Armink
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * SPDX-FileContributor: 2016-2021 Espressif Systems (Shanghai) CO LTD
 */
/*
 * FreeModbus Libary: ESP32 Port
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * IF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id: portevent.c v 1.60 2013/08/13 15:07:05 Armink add Master Functions$
 */

/* ----------------------- Modbus includes ----------------------------------*/

#include <stdatomic.h>
#include "mb_m.h"
#include "mbport.h"
#include "mbconfig.h"

#include "port.h"
#include "mbport.h"

#if MB_MASTER_RTU_ENABLED || MB_MASTER_ASCII_ENABLED || MB_MASTER_TCP_ENABLED

/* ----------------------- Start implementation -----------------------------*/

mbBOOL
xMBMasterPortEventInit( void )
{
    return TRUE;
}

mbBOOL MB_PORT_ISR_ATTR
xMBMasterPortEventPost( eMBMasterEventEnum eEvent)
{
    return TRUE;
}

mbBOOL
xMBMasterPortEventGet(xMBMasterEventType *peEvent)
{
    mbBOOL xEventHappened = FALSE;
    return xEventHappened;
}

eMBMasterEventEnum
xMBMasterPortFsmWaitConfirmation( eMBMasterEventEnum eEventMask, mbULONG ulTimeout)
{
    return 0;
}

uint64_t xMBMasterPortGetTransactionId( )
{
    return 0;
}

// This function is initialize the OS resource for modbus master.
void vMBMasterOsResInit( void )
{
    ;
}

/**
 * This function is take Mobus Master running resource.
 * Note:The resource is define by Operating System.
 *
 * @param lTimeOut the waiting time.
 *
 * @return resource take result
 */
mbBOOL xMBMasterRunResTake( mbLONG lTimeOut )
{
    return TRUE;
}

/**
 * This function is release Modbus Master running resource.
 * Note:The resource is define by Operating System. If you not use OS this function can be empty.
 */
void vMBMasterRunResRelease( void )
{
    ;
}

/**
 * This is modbus master respond timeout error process callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 *
 * @param ucDestAddress destination salve address
 * @param pucPDUData PDU buffer data
 * @param ucPDULength PDU buffer length
 *
 */
void vMBMasterErrorCBRespondTimeout(mbUCHAR ucDestAddress, const mbUCHAR* pucPDUData, mbUSHORT ucPDULength)
{
    ;
}

/**
 * This is modbus master receive data error process callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 *
 * @param ucDestAddress destination salve address
 * @param pucPDUData PDU buffer data
 * @param ucPDULength PDU buffer length
 */
void vMBMasterErrorCBReceiveData(mbUCHAR ucDestAddress, const mbUCHAR* pucPDUData, mbUSHORT ucPDULength)
{
    ;
}

/**
 * This is modbus master execute function error process callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 * @param ucDestAddress destination salve address
 * @param pucPDUData PDU buffer data
 * @param ucPDULength PDU buffer length
 *
 */
void vMBMasterErrorCBExecuteFunction(mbUCHAR ucDestAddress, const mbUCHAR* pucPDUData, mbUSHORT ucPDULength)
{
    ;
}

/**
 * This is modbus master request process success callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system. Do not execute too much waiting process.
 */
void vMBMasterCBRequestSuccess( void ) 
{
    ;
}

/**
 * This function is wait for modbus master request finish and return result.
 * Waiting result include request process success, request respond timeout,
 * receive data error and execute function error.You can use the above callback function.
 * @note If you are use OS, you can use OS's event mechanism. Otherwise you have to run
 * much user custom delay for waiting.
 *
 * @return request error code
 */
eMBMasterReqErrCode eMBMasterWaitRequestFinish( void ) {
    eMBMasterReqErrCode eErrStatus = MB_MRE_NO_ERR;
    return eErrStatus;
}

void vMBMasterPortEventClose(void)
{
    ;
}

#endif
