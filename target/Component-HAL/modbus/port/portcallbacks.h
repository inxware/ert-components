#ifndef _PORT_CALLBACKS_H_ 
#define _PORT_CALLBACKS_H_ 

#include "target_mbconfig.h"

// extern mbUSHORT gMBRegValues[EHS_CONFIG_FMB_CHANNEL_LIMIT];

extern ehs_bool gbEhsMBMasterFlag;
extern ehs_bool gbEhsMBInitialised;
extern ehs_bool gbEhsMBTCPIPFlag;

ehs_sint32 EhsHMBSlaveGetValue(ehs_sint32 register_type, ehs_sint32 address, ehs_sint32 *pValue);
ehs_sint32 EhsHMBSlaveSetValue(ehs_sint32 register_type, ehs_sint32 address, ehs_sint32 value);

typedef enum
{
    MB_REG_TYPE_COIL = 0,
    MB_REG_TYPE_DISCRETE,
    MB_REG_TYPE_INPUT,
    MB_REG_TYPE_HOLDING
} eMBRegisterType;

ehs_sint32 EhsHMBMasterWriteGetValue(ehs_sint32 device_id, eMBRegisterType register_type, ehs_sint32 address, ehs_sint32 *pValue);
ehs_sint32 EhsHMBMasterReadSetValue(ehs_sint32 device_id, eMBRegisterType register_type, ehs_sint32 address, ehs_sint32 value);

ehs_sint32 EhsHMBMasterReadError(ehs_sint32 device_id, eMBRegisterType register_type, ehs_sint32 address, ehs_sint32 error);
ehs_sint32 EhsHMBMasterWriteError(ehs_sint32 device_id, eMBRegisterType register_type, ehs_sint32 address, ehs_sint32 error);

#endif//_PORT_CALLBACKS_H_ 
