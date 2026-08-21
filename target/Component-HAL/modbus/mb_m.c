/*
 * SPDX-FileCopyrightText: 2013 Armink
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * SPDX-FileContributor: 2016-2021 Espressif Systems (Shanghai) CO LTD
 */
/*
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id: mbrtu_m.c,v 1.60 2013/08/20 11:18:10 Armink Add Master Functions $
 */

/* ----------------------- System includes ----------------------------------*/
#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/

#include "mb_m.h"
#include "mbconfig.h"
#include "mbframe.h"
#include "mbproto.h"
#include "mbfunc.h"

#include "mbport.h"
#if MB_MASTER_RTU_ENABLED
#include "mbrtu.h"
#endif
#if MB_MASTER_ASCII_ENABLED
#include "mbascii.h"
#endif
#if MB_MASTER_TCP_ENABLED
#include "mbtcp.h"
#include "mbtcp_m.h"
#endif

#include "hal-api.h"
#include "hal_process.h"
#include "hal_hashmap.h"
#include "target_process.h"
#include "portcallbacks.h"

#if MB_MASTER_RTU_ENABLED || MB_MASTER_ASCII_ENABLED || MB_MASTER_TCP_ENABLED

#ifndef MB_PORT_HAS_CLOSE
#define MB_PORT_HAS_CLOSE 1
#endif

#ifndef EHS_MB_HASH_MAP_SIZE
#define EHS_MB_HASH_MAP_SIZE 20
#endif


typedef struct eMBDataStruct {
    volatile mbUCHAR  device_id;
    volatile mbUSHORT reg_type;
    volatile mbUSHORT reg_addr;
    volatile mbUSHORT data;
    volatile mbBOOL   update;
} eMBDataStruct;

/* ----------------------- Static variables ---------------------------------*/

static _Atomic mbUCHAR ucMBMasterDestAddress;
static _Atomic mbBOOL xMBRunInMasterMode = FALSE;
static volatile _Atomic eMBMasterErrorEventType eMBMasterCurErrorType;
static volatile _Atomic mbUSHORT usMasterSendPDULength;
static volatile eMBMode eMBMasterCurrentMode;
static volatile _Atomic mbBOOL xMBCtrlThreadRunning = FALSE;

/*------------------------ Shared variables ---------------------------------*/

volatile mbUCHAR ucMasterSndBuf[MB_SERIAL_BUF_SIZE];
volatile mbUCHAR ucMasterRcvBuf[MB_SERIAL_BUF_SIZE];
volatile _Atomic eMBMasterTimerMode eMasterCurTimerMode;
volatile _Atomic mbBOOL xFrameIsBroadcast = FALSE;

EHS_VOLATILE EhsHashMap* gMBMasterReadHashMap = NULL;
EHS_VOLATILE EhsHashMap* gMBMasterWriteHashMap = NULL;

static enum
{
    STATE_ENABLED,
    STATE_DISABLED,
    STATE_NOT_INITIALIZED
} eMBState = STATE_NOT_INITIALIZED;

/* Functions pointer which are initialized in eMBInit( ). Depending on the
 * mode (RTU or ASCII) the are set to the correct implementations.
 * Using for Modbus Master,Add by Armink 20130813
 */
static peMBFrameSend peMBMasterFrameSendCur;
static pvMBFrameStart pvMBMasterFrameStartCur;
static pvMBFrameStop pvMBMasterFrameStopCur;
static peMBFrameReceive peMBMasterFrameReceiveCur;
static pvMBFrameClose pvMBMasterFrameCloseCur;

/* Callback functions required by the porting layer. They are called when
 * an external event has happend which includes a timeout or the reception
 * or transmission of a character.
 * Using for Modbus Master,Add by Armink 20130813
 */
mbBOOL( *pxMBMasterFrameCBByteReceived ) ( void );

mbBOOL( *pxMBMasterFrameCBTransmitterEmpty ) ( void );

mbBOOL( *pxMBMasterPortCBTimerExpired ) ( void );

mbBOOL( *pxMBMasterFrameCBReceiveFSMCur ) ( void );

mbBOOL( *pxMBMasterFrameCBTransmitFSMCur ) ( void );

/* An array of Modbus functions handlers which associates Modbus function
 * codes with implementing functions.
 */
static xMBFunctionHandler xMasterFuncHandlers[MB_FUNC_HANDLERS_MAX] = {
#if MB_FUNC_OTHER_REP_SLAVEID_ENABLED > 0
    {MB_FUNC_OTHER_REPORT_SLAVEID, eMBFuncReportSlaveID},
#endif
#if MB_FUNC_READ_INPUT_ENABLED > 0
    {MB_FUNC_READ_INPUT_REGISTER, eMBMasterFuncReadInputRegister},
#endif
#if MB_FUNC_READ_HOLDING_ENABLED > 0
    {MB_FUNC_READ_HOLDING_REGISTER, eMBMasterFuncReadHoldingRegister},
#endif
#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED > 0
    {MB_FUNC_WRITE_MULTIPLE_REGISTERS, eMBMasterFuncWriteMultipleHoldingRegister},
#endif
#if MB_FUNC_WRITE_HOLDING_ENABLED > 0
    {MB_FUNC_WRITE_REGISTER, eMBMasterFuncWriteHoldingRegister},
#endif
#if MB_FUNC_READWRITE_HOLDING_ENABLED > 0
    {MB_FUNC_READWRITE_MULTIPLE_REGISTERS, eMBMasterFuncReadWriteMultipleHoldingRegister},
#endif
#if MB_FUNC_READ_COILS_ENABLED > 0
    {MB_FUNC_READ_COILS, eMBMasterFuncReadCoils},
#endif
#if MB_FUNC_WRITE_COIL_ENABLED > 0
    {MB_FUNC_WRITE_SINGLE_COIL, eMBMasterFuncWriteCoil},
#endif
#if MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED > 0
    {MB_FUNC_WRITE_MULTIPLE_COILS, eMBMasterFuncWriteMultipleCoils},
#endif
#if MB_FUNC_READ_DISCRETE_INPUTS_ENABLED > 0
    {MB_FUNC_READ_DISCRETE_INPUTS, eMBMasterFuncReadDiscreteInputs},
#endif
};

/* ----------------------- Start implementation -----------------------------*/
#if MB_MASTER_TCP_ENABLED
eMBErrorCode
eMBMasterTCPInit( mbUSHORT ucTCPPort )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    if( ( eStatus = eMBMasterTCPDoInit( ucTCPPort ) ) != MB_ENOERR ) {
        eMBState = STATE_DISABLED;
    }
    else if( !xMBMasterPortEventInit(  ) ) {
        /* Port dependent event module initialization failed. */
        eStatus = MB_EPORTERR;
    } else {
        pvMBMasterFrameStartCur = eMBMasterTCPStart;
        pvMBMasterFrameStopCur = eMBMasterTCPStop;
        peMBMasterFrameReceiveCur = eMBMasterTCPReceive;
        peMBMasterFrameSendCur = eMBMasterTCPSend;
        pxMBMasterPortCBTimerExpired = xMBMasterTCPTimerExpired;
        pvMBMasterFrameCloseCur = MB_PORT_HAS_CLOSE ? vMBMasterTCPPortClose : NULL;
        ucMBMasterDestAddress = MB_TCP_PSEUDO_ADDRESS;
        eMBMasterCurrentMode = MB_TCP;
        eMBState = STATE_DISABLED;

        // initialize the OS resource for modbus master.
        vMBMasterOsResInit();
        if (xMBMasterPortTimersInit(MB_MASTER_TIMEOUT_MS_RESPOND * MB_TIMER_TICS_PER_MS) != TRUE)
        {
            eStatus = MB_EPORTERR;
        }

    }
    return eStatus;
}
#endif

eMBErrorCode
eMBMasterSerialInit( eMBMode eMode, mbUCHAR ucPort, mbULONG ulBaudRate, eMBParity eParity )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    switch (eMode)
    {
#if MB_MASTER_RTU_ENABLED > 0
    case MB_RTU:
        pvMBMasterFrameStartCur = eMBMasterRTUStart;
        pvMBMasterFrameStopCur = eMBMasterRTUStop;
        peMBMasterFrameSendCur = eMBMasterRTUSend;
        peMBMasterFrameReceiveCur = eMBMasterRTUReceive;
        pvMBMasterFrameCloseCur = MB_PORT_HAS_CLOSE ? vMBMasterPortClose : NULL;
        pxMBMasterFrameCBByteReceived = xMBMasterRTUReceiveFSM;
        pxMBMasterFrameCBTransmitterEmpty = xMBMasterRTUTransmitFSM;
        pxMBMasterPortCBTimerExpired = xMBMasterRTUTimerExpired;
        eMBMasterCurrentMode = eMode;

        eStatus = eMBMasterRTUInit(ucPort, ulBaudRate, eParity);
        break;
#endif
#if MB_MASTER_ASCII_ENABLED > 0
    case MB_ASCII:
        pvMBMasterFrameStartCur = eMBMasterASCIIStart;
        pvMBMasterFrameStopCur = eMBMasterASCIIStop;
        peMBMasterFrameSendCur = eMBMasterASCIISend;
        peMBMasterFrameReceiveCur = eMBMasterASCIIReceive;
        pvMBMasterFrameCloseCur = MB_PORT_HAS_CLOSE ? vMBMasterPortClose : NULL;
        pxMBMasterFrameCBByteReceived = xMBMasterASCIIReceiveFSM;
        pxMBMasterFrameCBTransmitterEmpty = xMBMasterASCIITransmitFSM;
        pxMBMasterPortCBTimerExpired = xMBMasterASCIITimerT1SExpired;
        eMBMasterCurrentMode = eMode;

        eStatus = eMBMasterASCIIInit(ucPort, ulBaudRate, eParity );
        break;
#endif
    default:
        eStatus = MB_EINVAL;
        break;
    }

    if (eStatus == MB_ENOERR)
    {
        if (!xMBMasterPortEventInit())
        {
            /* port dependent event module initalization failed. */
            eStatus = MB_EPORTERR;
        }
        else
        {
            eMBState = STATE_DISABLED;
        }
        /* initialize the OS resource for modbus master. */
        vMBMasterOsResInit();
    }
    return eStatus;
}

eMBErrorCode
eMBMasterClose( void )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    if( eMBState == STATE_DISABLED )
    {
        if( pvMBMasterFrameCloseCur != NULL )
        {
            pvMBMasterFrameCloseCur(  );
        }
    }
    else
    {
        eStatus = MB_EILLSTATE;
    }
    return eStatus;
}

eMBErrorCode
eMBMasterEnable( void )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    if( eMBState == STATE_DISABLED )
    {
        /* Activate the protocol stack. */
        pvMBMasterFrameStartCur(  );
        /* Release the resource, because it created in busy state */
        //vMBMasterRunResRelease( );
        eMBState = STATE_ENABLED;
    }
    else
    {
        eStatus = MB_EILLSTATE;
    }
    return eStatus;
}

eMBErrorCode
eMBMasterDisable( void )
{
    eMBErrorCode    eStatus;

    if( eMBState == STATE_ENABLED )
    {
        pvMBMasterFrameStopCur(  );
        eMBState = STATE_DISABLED;
        eStatus = MB_ENOERR;
    }
    else if( eMBState == STATE_DISABLED )
    {
        eStatus = MB_ENOERR;
    }
    else
    {
        eStatus = MB_EILLSTATE;
    }
    return eStatus;
}

eMBErrorCode
eMBMasterPoll( void )
{
    static mbUCHAR    *ucMBSendFrame = NULL;
    static mbUCHAR    *ucMBRcvFrame = NULL;
    static mbUCHAR    ucRcvAddress;
    static mbUCHAR    ucFunctionCode;
    static mbUSHORT   usLength;
    static eMBException eException;
    static uint64_t xCurTransactionId = 0;
    int             i;
    int             j;
    eMBErrorCode    eStatus = MB_ENOERR;
    xMBMasterEventType      xEvent;
    eMBMasterErrorEventType errorType;

    /* Check if the protocol stack is ready. */
    if( eMBState != STATE_ENABLED ) {
        return MB_EILLSTATE;
    }

    /* Check if there is a event available. If not return control to caller.
     * Otherwise we will handle the event. */
    if ( xMBMasterPortEventGet( &xEvent ) == TRUE ) {
        switch( xEvent.eEvent ) {
            // In some cases it is possible that more than one event set
            // together (even from one subset mask) than process them consistently
            case EV_MASTER_READY:
                MODBUS_LOGD(MB_PORT_TAG, "%" PRIu64 ":EV_MASTER_READY", xEvent.xTransactionId);
                vMBMasterSetErrorType( EV_ERROR_INIT );
                vMBMasterRunResRelease( );
                break;
            case EV_MASTER_FRAME_TRANSMIT:
                MODBUS_LOGD(MB_PORT_TAG, "%" PRIu64 ":EV_MASTER_FRAME_TRANSMIT", xEvent.xTransactionId);
                /* Master is busy now. */
                vMBMasterGetPDUSndBuf( &ucMBSendFrame );
                MODBUS_LOG_BUFFER_HEX_LEVEL("POLL transmit buffer", (void*)ucMBSendFrame, usMBMasterGetPDUSndLength(), MODBUS_LOG_DEBUG);
                eStatus = peMBMasterFrameSendCur( ucMBMasterGetDestAddress(), ucMBSendFrame, usMBMasterGetPDUSndLength() );
                if (eStatus != MB_ENOERR) {
                    vMBMasterSetErrorType(EV_ERROR_RECEIVE_DATA);
                    ( void ) xMBMasterPortEventPost( EV_MASTER_ERROR_PROCESS );
                    MODBUS_LOGE( MB_PORT_TAG, "%" PRIu64 ":Frame send error = %d", xEvent.xTransactionId, (unsigned)eStatus );
                }
                xCurTransactionId = xEvent.xTransactionId;
                break;
            case EV_MASTER_FRAME_SENT:
                if (xCurTransactionId == xEvent.xTransactionId) {
                    MODBUS_LOGD( MB_PORT_TAG, "%" PRIu64 ":EV_MASTER_FRAME_SENT", xEvent.xTransactionId );
                    MODBUS_LOG_BUFFER_HEX_LEVEL("POLL sent buffer", (void*)ucMBSendFrame, usMBMasterGetPDUSndLength(), MODBUS_LOG_DEBUG);
                }
                break;
            case EV_MASTER_FRAME_RECEIVED:
                MODBUS_LOGD( MB_PORT_TAG, "%" PRIu64 ":EV_MASTER_FRAME_RECEIVED", xEvent.xTransactionId );
                eStatus = peMBMasterFrameReceiveCur( &ucRcvAddress, &ucMBRcvFrame, &usLength);
                if (xCurTransactionId == xEvent.xTransactionId) {
                    MB_PORT_CHECK(ucMBSendFrame, MB_EILLSTATE, "Send buffer initialization fail.");
                    // Check if the frame is for us. If not ,send an error process event.
                    if ( ( eStatus == MB_ENOERR ) && ( ( ucRcvAddress == ucMBMasterGetDestAddress() )
                                                    || ( ucRcvAddress == MB_TCP_PSEUDO_ADDRESS) ) ) {
                        if ( ( ucMBRcvFrame[MB_PDU_FUNC_OFF]  & ~MB_FUNC_ERROR ) == ( ucMBSendFrame[MB_PDU_FUNC_OFF] ) ) {
                            MODBUS_LOGD(MB_PORT_TAG, "%" PRIu64 ": Packet data received successfully (%u).", xEvent.xTransactionId, (unsigned)eStatus);
                            MODBUS_LOG_BUFFER_HEX_LEVEL("POLL receive buffer", (void*)ucMBRcvFrame, (uint16_t)usLength, MODBUS_LOG_DEBUG);
                            ( void ) xMBMasterPortEventPost( EV_MASTER_EXECUTE );
                        } else {
                            MODBUS_LOGE( MB_PORT_TAG, "Drop incorrect frame, receive_func(%u) != send_func(%u)",
                                            ucMBRcvFrame[MB_PDU_FUNC_OFF], ucMBSendFrame[MB_PDU_FUNC_OFF]);
                            vMBMasterSetErrorType(EV_ERROR_RECEIVE_DATA);
                            ( void ) xMBMasterPortEventPost( EV_MASTER_ERROR_PROCESS );
                        }
                    } else {
                        vMBMasterSetErrorType(EV_ERROR_RECEIVE_DATA);
                        ( void ) xMBMasterPortEventPost( EV_MASTER_ERROR_PROCESS );
                        MODBUS_LOGD( MB_PORT_TAG, "%" PRIu64 ": Packet data receive failed (addr=%u)(%u).",
                                               xEvent.xTransactionId, (unsigned)ucRcvAddress, (unsigned)eStatus);
                    }
                } else {
                    // Ignore the `EV_MASTER_FRAME_RECEIVED` event because the respond timeout occurred
                    // and this is likely respond to previous transaction
                    MODBUS_LOGE( MB_PORT_TAG, "Drop data received outside of transaction (%" PRIu64 ")", xEvent.xTransactionId );
                }
                break;
            case EV_MASTER_EXECUTE:
                if (xCurTransactionId == xEvent.xTransactionId) {
                    if ( xMBMasterRequestIsBroadcast() 
                         && (( ucMBMasterGetCommMode() == MB_RTU ) || ( ucMBMasterGetCommMode() == MB_ASCII ) ) ) {
                        ucMBRcvFrame = ucMBSendFrame;
                    }
                    MB_PORT_CHECK(ucMBRcvFrame, MB_EILLSTATE, "receive buffer initialization fail.");
                    MODBUS_LOGD(MB_PORT_TAG, "%" PRIu64 ":EV_MASTER_EXECUTE", xEvent.xTransactionId);
                    ucFunctionCode = ucMBRcvFrame[MB_PDU_FUNC_OFF];
                    eException = MB_EX_ILLEGAL_FUNCTION;
                    /* If receive frame has exception. The receive function code highest bit is 1.*/
                    if (ucFunctionCode & MB_FUNC_ERROR) {
                        eException = (eMBException)ucMBRcvFrame[MB_PDU_DATA_OFF];
                    } else {
                        for ( i = 0; i < MB_FUNC_HANDLERS_MAX; i++ )
                        {
                            /* No more function handlers registered. Abort. */
                            if (xMasterFuncHandlers[i].ucFunctionCode == 0) {
                                break;
                            }
                            if (xMasterFuncHandlers[i].ucFunctionCode == ucFunctionCode) {
                                vMBMasterSetCBRunInMasterMode(TRUE);
                                /* If master request is broadcast,
                                * the master need execute function for all slave.
                                */
                                if ( xMBMasterRequestIsBroadcast() ) {
                                    usLength = usMBMasterGetPDUSndLength();
                                    for(j = 1; j <= MB_MASTER_TOTAL_SLAVE_NUM; j++)
                                    {
                                        vMBMasterSetDestAddress(j);
                                        eException = xMasterFuncHandlers[i].pxHandler(ucMBRcvFrame, &usLength);
                                    }
                                } else {
                                    eException = xMasterFuncHandlers[i].pxHandler( ucMBRcvFrame, &usLength );
                                }
                                vMBMasterSetCBRunInMasterMode( FALSE );
                                break;
                            }
                        }
                    }
                    /* If master has exception, will send error process event. Otherwise the master is idle.*/
                    if ( eException != MB_EX_NONE ) {
                        vMBMasterSetErrorType( EV_ERROR_EXECUTE_FUNCTION );
                        ( void ) xMBMasterPortEventPost( EV_MASTER_ERROR_PROCESS );
                    } else {
                        if ( eMBMasterGetErrorType( ) == EV_ERROR_INIT ) {
                            vMBMasterSetErrorType(EV_ERROR_OK);
                            MODBUS_LOGD( MB_PORT_TAG, "%" PRIu64 ":set event EV_ERROR_OK", xEvent.xTransactionId );
                            ( void ) xMBMasterPortEventPost( EV_MASTER_ERROR_PROCESS );
                        }
                    }
                } else {
                    MODBUS_LOGD( MB_PORT_TAG, "%" PRIu64 ":EV_MASTER_EXECUTE is expired", xEvent.xTransactionId );
                }
                break;
            case EV_MASTER_ERROR_PROCESS:
                if (xCurTransactionId == xEvent.xTransactionId) {
                    MODBUS_LOGD( MB_PORT_TAG, "%" PRIu64 ":EV_MASTER_ERROR_PROCESS", xEvent.xTransactionId);
                    /* Execute specified error process callback function. */
                    errorType = eMBMasterGetErrorType( );
                    vMBMasterGetPDUSndBuf( &ucMBSendFrame );
                    switch ( errorType )
                    {
                        case EV_ERROR_RESPOND_TIMEOUT:
                            vMBMasterErrorCBRespondTimeout( ucMBMasterGetDestAddress( ),
                                    ucMBSendFrame, usMBMasterGetPDUSndLength( ) );
                            break;
                        case EV_ERROR_RECEIVE_DATA:
                            vMBMasterErrorCBReceiveData( ucMBMasterGetDestAddress( ),
                                    ucMBSendFrame, usMBMasterGetPDUSndLength( ) );
                            break;
                        case EV_ERROR_EXECUTE_FUNCTION:
                            vMBMasterErrorCBExecuteFunction( ucMBMasterGetDestAddress( ),
                                    ucMBSendFrame, usMBMasterGetPDUSndLength( ) );
                            break;
                        case EV_ERROR_OK:
                            vMBMasterCBRequestSuccess( );
                            break;
                        default:
                            MODBUS_LOGE( MB_PORT_TAG, "%" PRIu64 ":incorrect error type = %d.", xEvent.xTransactionId, (int)errorType);
                            break;
                    }
                }
                vMBMasterPortTimersDisable( );
                uint64_t xProcTime = xCurTransactionId ? ( xEvent.xPostTimestamp - xCurTransactionId ) : 0;
                MODBUS_LOGD( MB_PORT_TAG, "Transaction (%" PRIu64 "), processing time(us) = %" PRId64, xCurTransactionId, xProcTime );
                xCurTransactionId = 0;
                vMBMasterSetErrorType( EV_ERROR_INIT );
                vMBMasterRunResRelease( );
                break;
            default:
                MODBUS_LOGE( MB_PORT_TAG, "%" PRIu64 ":Unexpected event triggered 0x%02x.", xEvent.xTransactionId, (int)xEvent.eEvent );
                break;
        }
    } else {
        // Something went wrong and task unblocked but there are no any correct events set
        MODBUS_LOGE( MB_PORT_TAG, "%" PRIu64 ": Unexpected event triggered 0x%02x.", xEvent.xTransactionId, (int)xEvent.eEvent );
        eStatus = MB_EILLSTATE;
    }
    return eStatus;
}

/*
 *
 */

ehs_sint32 MBMaster_gen_unique_key(mbUCHAR device_id, mbUSHORT reg_type, mbUSHORT reg_addr) {
    return ((ehs_sint32)(device_id & 0x7F) << 24) | 
           ((ehs_sint32)(reg_type & 0xFF) << 16) | 
           ((ehs_sint32)(reg_addr & 0xFFFF));
}

eMBMasterReqErrCode MBMasterProcessWrite( mbUCHAR device_id, mbUSHORT reg_type, mbUSHORT reg_addr, mbUSHORT data )
{
    eMBMasterReqErrCode mb_error = MB_MRE_MASTER_BUSY;

    switch (reg_type)
    {
        case MB_REG_TYPE_COIL:
        {
            // Convert non-zero data to 0xFF00 to meet the MODBUS specification
            mb_error = eMBMasterReqWriteCoil(device_id, reg_addr, data, (mbLONG)MB_SERIAL_API_RESP_TICS);
            break;
        }
        case MB_REG_TYPE_HOLDING:
        {
            mb_error = eMBMasterReqWriteHoldingRegister(device_id, reg_addr, data, (mbLONG)MB_SERIAL_API_RESP_TICS);
            break;
        }
        default:
        {
            mb_error = MB_MRE_NO_REG;
        }
    }

    return mb_error;
}

eMBMasterReqErrCode MBMasterProcessRead( mbUCHAR device_id, mbUSHORT reg_type, mbUSHORT reg_addr)
{
    eMBMasterReqErrCode mb_error = MB_MRE_MASTER_BUSY;
    switch (reg_type)
    {
        case MB_REG_TYPE_COIL:
        {
            
            mb_error = eMBMasterReqReadCoils(device_id, reg_addr, 1, (mbLONG)MB_SERIAL_API_RESP_TICS);
            break;
        }
        case MB_REG_TYPE_DISCRETE:
        {
            mb_error = eMBMasterReqReadDiscreteInputs(device_id, reg_addr, 1, (mbLONG)MB_SERIAL_API_RESP_TICS);
            break;
        }
        case MB_REG_TYPE_INPUT:
        {
            mb_error = eMBMasterReqReadInputRegister(device_id, reg_addr, 1, (mbLONG)MB_SERIAL_API_RESP_TICS);
            break;
        }
        case MB_REG_TYPE_HOLDING:
        {
            mb_error = eMBMasterReqReadHoldingRegister(device_id, reg_addr, 1, (mbLONG)MB_SERIAL_API_RESP_TICS);
            break;
        }
    }
    return mb_error;
}

EhsThreadFuncReturnType eMBMasterControlThread( void* arg )
{   if(arg){
        // do nothing (for now)
    }
    while( eMBMasterControlThreadRunning() == TRUE ){
        //read master modbus
        if(gMBMasterReadHashMap != NULL){
            mbUCHAR  device_id;
            mbUSHORT reg_type, reg_addr;
            EhsTPMutex_lock(EhsTPMutex_MBMaster);
            ehs_uint32 map_size = EhsHashMapSize(gMBMasterReadHashMap);
            EhsTPMutex_unlock(EhsTPMutex_MBMaster);
            for(ehs_uint32 i = 0; i < map_size; i++){
                mbBOOL bReadUpdate = FALSE, bReadDataFound = FALSE;
                EhsTPMutex_lock(EhsTPMutex_MBMaster);
                eMBDataStruct* pReadData = (eMBDataStruct*)EhsHashMapGetValueByIndex(gMBMasterReadHashMap, i);
                if(pReadData != NULL){
                    device_id = pReadData->device_id;
                    reg_type  = pReadData->reg_type;
                    reg_addr  = pReadData->reg_addr;
                    bReadUpdate = pReadData->update;
                    if(bReadUpdate == TRUE) pReadData->update = FALSE;
                    bReadDataFound = TRUE;
                }
                EhsTPMutex_unlock(EhsTPMutex_MBMaster);
                if(bReadDataFound == TRUE)
                {                    
                    if(bReadUpdate == TRUE){
                        eMBMasterReqErrCode mb_error = MBMasterProcessRead(device_id, reg_type, reg_addr);
                        if (mb_error != MB_MRE_NO_ERR){
                            EhsHMBMasterReadError(device_id, reg_type, reg_addr, mb_error);
                        }
                    }
                }
                EhsSleep(EHS_TIME_ms(1));
            }
        }
        // write master modbus
        if(gMBMasterWriteHashMap != NULL){
            mbUCHAR  device_id = 0;
            mbUSHORT reg_type = 0, reg_addr = 0, data = 0;
            EhsTPMutex_lock(EhsTPMutex_MBMaster);
            ehs_uint32 map_size = EhsHashMapSize(gMBMasterWriteHashMap);
            EhsTPMutex_unlock(EhsTPMutex_MBMaster);
            for(ehs_uint32 i = 0; i < map_size; i++){
                mbBOOL bWriteUpdate = FALSE, bWriteDataFound = FALSE;
                EhsTPMutex_lock(EhsTPMutex_MBMaster);
                eMBDataStruct* pWriteData = (eMBDataStruct*)EhsHashMapGetValueByIndex(gMBMasterWriteHashMap, i);
                if(pWriteData != NULL){
                    device_id = pWriteData->device_id;
                    reg_type  = pWriteData->reg_type;
                    reg_addr  = pWriteData->reg_addr;
                    data  = pWriteData->data;
                    bWriteUpdate = pWriteData->update;
                    if(bWriteUpdate == TRUE) pWriteData->update = FALSE;
                    bWriteDataFound = TRUE;
                }
                EhsTPMutex_unlock(EhsTPMutex_MBMaster);
                if(bWriteDataFound == TRUE)
                {                    
                    if(bWriteUpdate == TRUE){
                        eMBMasterReqErrCode mb_error = MBMasterProcessWrite(device_id, reg_type, reg_addr, data);
                        // if (mb_error != MB_MRE_NO_ERR){
                        // There's no need to check error or not as the function below just indicates that the writing completes
                        EhsHMBMasterWriteError(device_id, reg_type, reg_addr, mb_error);
                        //}
                    }
                }
                EhsSleep(EHS_TIME_ms(1));
            }
        }
        EhsSleep(EHS_TIME_ms(10));
    }
    // free the hash maps
    if(gMBMasterReadHashMap != NULL){
        ehs_uint32 map_size = EhsHashMapSize(gMBMasterReadHashMap);
        for(ehs_uint32 i = 0; i < map_size; i++){
            eMBDataStruct* data = (eMBDataStruct*)EhsHashMapGetValueByIndex(gMBMasterReadHashMap, i);
            free( data );
        }
        EhsHashMapFree(gMBMasterReadHashMap);
        gMBMasterReadHashMap = NULL;
    }
    if(gMBMasterWriteHashMap != NULL){
        ehs_uint32 map_size = EhsHashMapSize(gMBMasterWriteHashMap);
        for(ehs_uint32 i = 0; i < map_size; i++){
            eMBDataStruct* data = (eMBDataStruct*)EhsHashMapGetValueByIndex(gMBMasterWriteHashMap, i);
            free( data );
        }
        EhsHashMapFree(gMBMasterWriteHashMap);
        gMBMasterWriteHashMap = NULL;
    }

    return 1;
}

void eMBMasterControlThreadStart( void )
{
    if(eMBMasterControlThreadRunning() == FALSE) {
        // Only single instance of the thread is required
        atomic_store(&(xMBCtrlThreadRunning), TRUE);
        // initalise modbus master data map
        if(gMBMasterReadHashMap == NULL){
            gMBMasterReadHashMap = EhsCreateHashMap(2);
        }
        if(gMBMasterWriteHashMap == NULL){
            gMBMasterWriteHashMap = EhsCreateHashMap(2);
        }
        // start modbus control thread
        if( EhsHThread_execute(eMBMasterControlThread, NULL, EHS_PRI_MODBUS_MASTER_CTRL, EHS_THREAD_USE_DEFAULT_STACK_SIZE,"modbusms") == EHS_FALSE ) {
            atomic_store(&(xMBCtrlThreadRunning), FALSE);
        }
    }
}

void eMBMasterControlThreadKill( void )
{
    if(eMBMasterControlThreadRunning() == TRUE) {
        atomic_store(&(xMBCtrlThreadRunning), FALSE);
        // @TODO - join the thread
    }
}

mbBOOL eMBMasterControlThreadRunning( void )
{
    return atomic_load(&xMBCtrlThreadRunning);
}

void eMBMasterRead( mbUCHAR device_id, mbSHORT reg_type, mbSHORT reg_addr )
{
    if(eMBMasterControlThreadRunning() == TRUE && gMBMasterReadHashMap != NULL) {
        ehs_sint32 key = MBMaster_gen_unique_key(device_id, reg_type, reg_addr);
        EhsTPMutex_lock(EhsTPMutex_MBMaster);
        eMBDataStruct* pReadData = (eMBDataStruct*)EhsHashMapGetValue(gMBMasterReadHashMap, key);
        if(pReadData == NULL){
            pReadData = (eMBDataStruct*)malloc(sizeof(eMBDataStruct));
            if(pReadData != NULL){
                pReadData->update = FALSE;
                EhsHashMapAppend(gMBMasterReadHashMap, key, (void*) pReadData);
            }
        }
        if(pReadData != NULL){
            pReadData->device_id = device_id;
            pReadData->reg_type  = reg_type;
            pReadData->reg_addr  = reg_addr;
            pReadData->update    = TRUE;
        }
        EhsTPMutex_unlock(EhsTPMutex_MBMaster);
    }
}

void eMBMasterWrite( mbUCHAR device_id, mbSHORT reg_type, mbSHORT reg_addr, mbUSHORT data )
{
    if(eMBMasterControlThreadRunning() == TRUE && gMBMasterWriteHashMap != NULL) {
        ehs_sint32 key = MBMaster_gen_unique_key(device_id, reg_type, reg_addr);
        EhsTPMutex_lock(EhsTPMutex_MBMaster);
        eMBDataStruct* pWriteData = (eMBDataStruct*)EhsHashMapGetValue(gMBMasterWriteHashMap, key);
        if(pWriteData == NULL){
            pWriteData = (eMBDataStruct*)malloc(sizeof(eMBDataStruct));
            if(pWriteData != NULL){
                pWriteData->update = FALSE;
                EhsHashMapAppend(gMBMasterWriteHashMap, key, (void*) pWriteData);
            }
        }
        if(pWriteData != NULL){
            pWriteData->device_id = device_id;
            pWriteData->reg_type  = reg_type;
            pWriteData->reg_addr  = reg_addr;
            pWriteData->data      = data;
            pWriteData->update    = TRUE;
        }
        EhsTPMutex_unlock(EhsTPMutex_MBMaster);
    }
}

// Get whether the Modbus Master is run in master mode.
mbBOOL xMBMasterGetCBRunInMasterMode( void )
{
    return atomic_load(&xMBRunInMasterMode);
}

// Set whether the Modbus Master is run in master mode.
void vMBMasterSetCBRunInMasterMode( mbBOOL IsMasterMode )
{
    atomic_store(&(xMBRunInMasterMode), IsMasterMode);
}

// Get Modbus Master send destination address.
mbUCHAR ucMBMasterGetDestAddress( void )
{
    return atomic_load(&ucMBMasterDestAddress);
}

// Set Modbus Master send destination address.
void vMBMasterSetDestAddress( mbUCHAR Address )
{
    atomic_store(&(ucMBMasterDestAddress), Address);
}

// Get Modbus Master current error event type.
eMBMasterErrorEventType inline eMBMasterGetErrorType( void )
{
    return atomic_load(&eMBMasterCurErrorType);
}

// Set Modbus Master current error event type.
void IRAM_ATTR vMBMasterSetErrorType( eMBMasterErrorEventType errorType )
{
    atomic_store(&(eMBMasterCurErrorType), errorType);
}

/* Get Modbus Master send PDU's buffer address pointer.*/
void vMBMasterGetPDUSndBuf( mbUCHAR ** pucFrame )
{
    *pucFrame = ( mbUCHAR * ) &ucMasterSndBuf[MB_SEND_BUF_PDU_OFF];
}

/* Set Modbus Master send PDU's buffer length.*/
void vMBMasterSetPDUSndLength( mbUSHORT SendPDULength )
{
    atomic_store(&(usMasterSendPDULength), SendPDULength);
}

/* Get Modbus Master send PDU's buffer length.*/
mbUSHORT usMBMasterGetPDUSndLength( void )
{
    return atomic_load(&usMasterSendPDULength);
}

/* Set Modbus Master current timer mode.*/
void vMBMasterSetCurTimerMode( eMBMasterTimerMode eMBTimerMode )
{
    atomic_store(&(eMasterCurTimerMode), eMBTimerMode);
}

/* Get Modbus Master current timer mode.*/
eMBMasterTimerMode MB_PORT_ISR_ATTR xMBMasterGetCurTimerMode( void )
{
    return atomic_load(&eMasterCurTimerMode);
}

/* The master request is broadcast? */
mbBOOL MB_PORT_ISR_ATTR xMBMasterRequestIsBroadcast( void )
{
    return xFrameIsBroadcast;
}

/* The master request is broadcast? */
void vMBMasterRequestSetType( mbBOOL xIsBroadcast )
{
    atomic_store(&(xFrameIsBroadcast), xIsBroadcast);
}

// Get Modbus Master communication mode.
eMBMode ucMBMasterGetCommMode(void)
{
    return eMBMasterCurrentMode;
}

#endif // MB_MASTER_RTU_ENABLED || MB_MASTER_ASCII_ENABLED || MB_MASTER_TCP_ENABLED
