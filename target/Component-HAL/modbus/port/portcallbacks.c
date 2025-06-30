#include "port.h"
#include "mb.h"
#include "mb_m.h"
#include "mbframe.h"
#include "mbutils.h"
#include "portcallbacks.h"

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
    int             iRegIndex = usAddress;
    // mbUSHORT value=0;
    ehs_sint32 value=0;
    //printf("eMBRegInputCB\n");
    while( usNRegs > 0 )
    {
        // if(EhsModbusGetValue(iRegIndex,&value))
        if(EhsHMBSlaveGetValue(MB_REG_TYPE_INPUT, iRegIndex, &value))
        {
            eStatus=MB_ENOREG;
            break;
        }
        else
        {
            *pucRegBuffer++ = ( unsigned char )( value >> 8 );
            *pucRegBuffer++ = ( unsigned char )( value & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    return eStatus;
}
/****************************************************************************************
*
*****************************************************************************************/
eMBErrorCode eMBRegHoldingCB( mbUCHAR * pucRegBuffer, mbUSHORT usAddress, mbUSHORT usNRegs, eMBRegisterMode eMode )
{
    //printf("eMBRegHoldingCB\n");
    //return MB_ENOREG;
    eMBErrorCode eStatus = MB_ENOERR;
    int iRegIndex=usAddress;
    switch(eMode)
    {
    /* Pass current register values to the protocol stack. */
    case MB_REG_READ:
        while(usNRegs > 0)
        {
            mbUSHORT value=0;
            // if(EhsModbusGetValue(iRegIndex,&value))
            if(EhsHMBSlaveGetValue(MB_REG_TYPE_HOLDING, iRegIndex, &value))
            {
                //error occurred, tell no reg
                eStatus=MB_ENOREG;
                break;
            }
            else
            {
                //else update the reg buffer
                //original code
                //copies the 16 bit value in byte backwards
                *pucRegBuffer++ = (unsigned char)(value >> 8);
                *pucRegBuffer++ = (unsigned char)(value & 0xFF);

                iRegIndex++;
                usNRegs--;
            }
        }
        break;

    /* Update current register values with new values from the
     * protocol stack. */
    case MB_REG_WRITE:
        while(usNRegs > 0)
        {
            mbUSHORT value=0;
            value = *pucRegBuffer++ << 8;
            value |= *pucRegBuffer++;
            // EhsModbusReadEvent(iRegIndex,value);
            if (EhsHMBSlaveSetValue(MB_REG_TYPE_HOLDING, iRegIndex, value)) eStatus = MB_ENOREG;
            else eStatus = MB_ENOERR;
            //pucRegBuffer = pucRegBuffer + sizeof(usRegHoldingBuf[iRegIndex]);
            iRegIndex++;
            usNRegs--;
        }
    }
    return eStatus;
}
/****************************************************************************************
*
*****************************************************************************************/
eMBErrorCode eMBRegCoilsCB( mbUCHAR * pucRegBuffer, mbUSHORT usAddress, mbUSHORT usNCoils, eMBRegisterMode eMode )
{
    //printf("eMBRegCoilsCB\n");
    // return MB_ENOREG;
    eMBErrorCode eStatus = MB_ENOERR;
    int iRegIndex=usAddress;
    mbUSHORT value=0;
    switch(eMode)
    {
    case MB_REG_READ:
    {
        while(usNCoils > 0)
        {
            // if(EhsModbusGetValue(iRegIndex,&value))
            if(EhsHMBSlaveGetValue(MB_REG_TYPE_COIL, iRegIndex, &value))
            {
                eStatus = MB_ENOREG;
                break;
            }
            else
            {
                mbUCHAR ucResult = (mbUCHAR)value;
                xMBUtilSetBits(pucRegBuffer, iRegIndex-usAddress, 1, ucResult);
                iRegIndex++;
                usNCoils--;
            }
        }
        break;
    }
    case MB_REG_WRITE:
    {
        while ( usNCoils > 0 )
        {
            mbUCHAR ucResult = xMBUtilGetBits(pucRegBuffer, iRegIndex - usAddress, 1);
            // EhsModbusReadEvent(iRegIndex,ucResult);
            if (EhsHMBSlaveSetValue(MB_REG_TYPE_COIL, iRegIndex, ucResult)) eStatus = MB_ENOREG;
            else eStatus = MB_ENOERR;
            iRegIndex++;
            usNCoils--;
        }
        break;
    }
    }
    return eStatus;
}
/****************************************************************************************
*
*****************************************************************************************/
eMBErrorCode eMBRegDiscreteCB( mbUCHAR * pucRegBuffer, mbUSHORT usAddress, mbUSHORT usNDiscrete )
{
    //printf("eMBRegDiscreteCB\n");
    //return MB_ENOREG;
    eMBErrorCode eStatus = MB_ENOERR;
    int  iRegIndex=usAddress;
    mbUSHORT value=0;
    while(usNDiscrete > 0)
    {
        // if(EhsModbusGetValue(iRegIndex,&value))
        if(EhsHMBSlaveGetValue(MB_REG_TYPE_DISCRETE, iRegIndex, &value))
        {
            eStatus = MB_ENOREG;
            break;
        }
        else
        {
            mbUCHAR ucResult = (mbUCHAR)value;
            xMBUtilSetBits(pucRegBuffer, iRegIndex-usAddress, 1, ucResult);
            iRegIndex++;
            usNDiscrete--;
        }

    }
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
    int iRegIndex= usAddress;
    mbUCHAR value = 0;
    
    while (usNDiscrete > 0)
    {
        value = xMBUtilGetBits(pucRegBuffer++, iRegIndex - usAddress, 1);
        if (EhsHMBMasterReadSetValue(ucMBMasterGetDestAddress(), MB_REG_TYPE_DISCRETE, iRegIndex, value))
        {
            error = MB_ENOREG;
            break;
        }
        iRegIndex++;
        usNDiscrete--;
    }
    // MB_MASTER_CHECK((master_interface_ptr != NULL),
    //                 ESP_ERR_INVALID_STATE,
    //                 "Master interface is not correctly initialized.");
    // MB_MASTER_CHECK((master_interface_ptr->master_reg_cb_discrete != NULL),
    //                 ESP_ERR_INVALID_STATE,
    //                 "Master interface is not correctly initialized.");
    // error = master_interface_ptr->master_reg_cb_discrete(pucRegBuffer, usAddress, usNDiscrete);
    return error;
}

eMBErrorCode eMBMasterRegCoilsCB(mbUCHAR* pucRegBuffer, mbUSHORT usAddress,
        mbUSHORT usNCoils, eMBRegisterMode eMode)
{
    eMBErrorCode error = MB_ENOERR;
    // MB_MASTER_CHECK((master_interface_ptr != NULL),
    //                 ESP_ERR_INVALID_STATE,
    //                 "Master interface is not correctly initialized.");
    // MB_MASTER_CHECK((master_interface_ptr->master_reg_cb_coils != NULL),
    //                 ESP_ERR_INVALID_STATE,
    //                 "Master interface is not correctly initialized.");
    // error = master_interface_ptr->master_reg_cb_coils(pucRegBuffer, usAddress,
    //                                                    usNCoils, eMode);
    mbUSHORT iRegIndex;
    mbUSHORT value = 0;
    usAddress--; // The address is already + 1
    iRegIndex = usAddress;
    switch (eMode)
    {
        case MB_REG_WRITE:
        {
            while (usNCoils > 0)
            {
                value = 0;
                if (EhsHMBMasterWriteGetValue(ucMBMasterGetDestAddress(), MB_REG_TYPE_COIL, iRegIndex, &value))
                {
                    error = MB_ENOREG;
                    break;
                }
                mbUCHAR ucResult = (mbUCHAR) value;
                xMBUtilSetBits(pucRegBuffer++, iRegIndex - usAddress, 1, ucResult);
                iRegIndex++;
                usNCoils--;
            }
            break;
        }
        case MB_REG_READ:
        {
            while (usNCoils > 0)
            {
                mbUCHAR ucResult = xMBUtilGetBits(pucRegBuffer++, iRegIndex - usAddress, 1);
                if (EhsHMBMasterReadSetValue(ucMBMasterGetDestAddress(), MB_REG_TYPE_COIL, iRegIndex, ucResult))
                {
                    error = MB_ENOREG;
                    break;
                }
                iRegIndex++;
                usNCoils--;
            }
            break;
        }
    }
    return error;
}

eMBErrorCode eMBMasterRegHoldingCB(mbUCHAR * pucRegBuffer, mbUSHORT usAddress,
        mbUSHORT usNRegs, eMBRegisterMode eMode)
{
    eMBErrorCode error = MB_ENOERR;
    usAddress--; // The address is already + 1
    int iRegIndex = usAddress;
    mbUSHORT value = 0;

    switch (eMode)
    {
        case MB_REG_WRITE:
        {
            while (usNRegs > 0)
            {
                value = 0;
                if (EhsHMBMasterWriteGetValue(ucMBMasterGetDestAddress(), MB_REG_TYPE_HOLDING, iRegIndex, &value))
                {
                    error = MB_ENOREG;
                    break;
                }
                *pucRegBuffer++ = (mbUCHAR) (value >> 8);
                *pucRegBuffer++ = (mbUCHAR) (value & 0xFF);

                iRegIndex++;
                usNRegs--;
            }
            break;
        }
        case MB_REG_READ:
        {
            while (usNRegs > 0)
            {
                value = 0;
                value = *pucRegBuffer++ << 8;
                value |= *pucRegBuffer++;
                if (EhsHMBMasterReadSetValue(ucMBMasterGetDestAddress(), MB_REG_TYPE_HOLDING, iRegIndex, value))
                {
                    error = MB_ENOREG;
                    break;
                }
                iRegIndex++;
                usNRegs--;
            }
            break;
        }
    }
    // MB_MASTER_CHECK((master_interface_ptr != NULL),
    //                 ESP_ERR_INVALID_STATE,
    //                 "Master interface is not correctly initialized.");
    // MB_MASTER_CHECK((master_interface_ptr->master_reg_cb_holding != NULL),
    //                 ESP_ERR_INVALID_STATE,
    //                 "Master interface is not correctly initialized.");
    // error = master_interface_ptr->master_reg_cb_holding(pucRegBuffer, usAddress,
    //                                                     usNRegs, eMode);
    return error;
}

eMBErrorCode eMBMasterRegInputCB(mbUCHAR * pucRegBuffer, mbUSHORT usAddress,
                                mbUSHORT usNRegs)
{
    eMBErrorCode error = MB_ENOERR;
    int iRegIndex = usAddress;

    mbUCHAR value = 0;
    while (usNRegs > 0)
    {
        value = 0;
        value = *pucRegBuffer++ << 8;
        value |= *pucRegBuffer++;
        if (EhsHMBMasterReadSetValue(ucMBMasterGetDestAddress(), MB_REG_TYPE_INPUT, iRegIndex, value))
        {
            error = MB_ENOREG;
            break;
        }
        iRegIndex++;
        usNRegs--;
    }
    // MB_MASTER_CHECK((master_interface_ptr != NULL),
    //                 ESP_ERR_INVALID_STATE,
    //                 "Master interface is not correctly initialized.");
    // MB_MASTER_CHECK((master_interface_ptr->master_reg_cb_input != NULL),
    //                 ESP_ERR_INVALID_STATE,
    //                 "Master interface is not correctly initialized.");
    // error = master_interface_ptr->master_reg_cb_input(pucRegBuffer, usAddress, usNRegs);
    return error;
}

#endif