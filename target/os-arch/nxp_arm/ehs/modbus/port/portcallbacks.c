#include "heatrod_config.h"
#include "port.h"
#include "mb.h"
#include "mbframe.h"
#include "mbutils.h"
bool EhsModbusReadEvent(const uint32_t channel,const int32_t value);
bool EhsModbusGetValue(const uint32_t channel,uint16_t* pValue);
/****************************************************************************************
*
*****************************************************************************************/

eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex = usAddress;
	USHORT value=0;
	while( usNRegs > 0 )
	{
		if(EhsModbusGetValue(iRegIndex,&value)){
			eStatus=MB_ENOREG;
			break;
		}else{
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
eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    //return MB_ENOREG;
	eMBErrorCode eStatus = MB_ENOERR;
	int iRegIndex=usAddress;
		switch(eMode)
		{
		/* Pass current register values to the protocol stack. */
		case MB_REG_READ:
			while(usNRegs > 0)
			{
				USHORT value=0;
				if(EhsModbusGetValue(iRegIndex,&value)){
					//error occurred, tell no reg
					eStatus=MB_ENOREG;
					break;
				}else{
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
				USHORT value=0;
				value = *pucRegBuffer++ << 8;
				value |= *pucRegBuffer++;
				EhsModbusReadEvent(iRegIndex,value);
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
eMBErrorCode eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
   // return MB_ENOREG;
    eMBErrorCode eStatus = MB_ENOERR;
    int iRegIndex=usAddress;
	USHORT value=0;
	switch(eMode)
	{
		case MB_REG_READ:
		{
			while(usNCoils > 0)
			{
				if(EhsModbusGetValue(iRegIndex,&value)){
					eStatus = MB_ENOREG;
					break;
				}else{
					UCHAR ucResult = (UCHAR)value;
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
				UCHAR ucResult = xMBUtilGetBits(pucRegBuffer, iRegIndex - usAddress, 1);
				EhsModbusReadEvent(iRegIndex,ucResult);
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
eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    //return MB_ENOREG;
	eMBErrorCode eStatus = MB_ENOERR;
	int  iRegIndex=usAddress;
	USHORT value=0;
	while(usNDiscrete > 0)
	{
		if(EhsModbusGetValue(iRegIndex,&value)){
			eStatus = MB_ENOREG;
			break;
		}else{
			UCHAR ucResult = (UCHAR)value;
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
