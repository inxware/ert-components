#ifndef _HAL_TARGET_SYS_STAT_H_ 
#define _HAL_TARGET_SYS_STAT_H_ 

#include "target_types.h"

ehs_sint32 EhsTGetCpuTemp();
ehs_sint32 EhsTGetCpuUsagePercent();
ehs_sint32 EhsTGetRamUsagePercent();

#endif//_HAL_TARGET_SYS_STAT_H_ 