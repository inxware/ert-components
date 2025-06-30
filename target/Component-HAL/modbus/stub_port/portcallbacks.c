#include "port.h"
#include "mb.h"
#include "mb_m.h"
#include "mbframe.h"
#include "mbutils.h"
#include "portcallbacks.h"
#include <stdbool.h>

// mbUSHORT gMBRegValues[EHS_CONFIG_FMB_CHANNEL_LIMIT] = {0};

bool EhsModbusReadEvent(const uint32_t channel,const int32_t value);
bool EhsModbusGetValue(const uint32_t channel,uint16_t* pValue);
ehs_sint32 EhsHMBSlaveGetValue(ehs_sint32 register_type, ehs_sint32 address, ehs_sint32 *pValue);
/****************************************************************************************
*
*****************************************************************************************/

eMBErrorCode eMBRegInputCB( mbUCHAR * pucRegBuffer, mbUSHORT usAddress, mbUSHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    return eStatus;
}
/****************************************************************************************
*
*****************************************************************************************/
eMBErrorCode eMBRegHoldingCB( mbUCHAR * pucRegBuffer, mbUSHORT usAddress, mbUSHORT usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode eStatus = MB_ENOERR;
    return eStatus;
}
/****************************************************************************************
*
*****************************************************************************************/
eMBErrorCode eMBRegCoilsCB( mbUCHAR * pucRegBuffer, mbUSHORT usAddress, mbUSHORT usNCoils, eMBRegisterMode eMode )
{
    eMBErrorCode eStatus = MB_ENOERR;
    return eStatus;
}
/****************************************************************************************
*
*****************************************************************************************/
eMBErrorCode eMBRegDiscreteCB( mbUCHAR * pucRegBuffer, mbUSHORT usAddress, mbUSHORT usNDiscrete )
{
    eMBErrorCode eStatus = MB_ENOERR;
    return eStatus;
}
/****************************************************************************************
*
*****************************************************************************************/

/****************************************************************************************
* MODBUS Master Callbacks
*****************************************************************************************/
#if 1// MB_MASTER_RTU_ENABLED || MB_MASTER_ASCII_ENABLED || MB_MASTER_TCP_ENABLED

eMBErrorCode eMBMasterRegDiscreteCB(mbUCHAR * pucRegBuffer, mbUSHORT usAddress,
                            mbUSHORT usNDiscrete)
{
    eMBErrorCode error = MB_ENOERR;
    return error;
}

eMBErrorCode eMBMasterRegCoilsCB(mbUCHAR* pucRegBuffer, mbUSHORT usAddress,
        mbUSHORT usNCoils, eMBRegisterMode eMode)
{
    eMBErrorCode error = MB_ENOERR;
    return error;
}

eMBErrorCode eMBMasterRegHoldingCB(mbUCHAR * pucRegBuffer, mbUSHORT usAddress,
        mbUSHORT usNRegs, eMBRegisterMode eMode)
{
    eMBErrorCode error = MB_ENOERR;
    return error;
}

eMBErrorCode eMBMasterRegInputCB(mbUCHAR * pucRegBuffer, mbUSHORT usAddress,
                                mbUSHORT usNRegs)
{
    eMBErrorCode error = MB_ENOERR;
    return error;
}

#endif