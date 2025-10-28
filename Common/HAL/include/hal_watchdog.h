/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/*
 * hal_watchdog.h
 */

#ifndef _HAL_WATCHDOG_H_
#define _HAL_WATCHDOG_H_
#include "ehs_fb_types.h"
#include "target_watchdog.h"
void EhsHWatchdogDisable( void ) {
    EhsTWatchdogDisable();
}
void EhsHWatchdogEnable() {
    EhsTWatchdogEnable();
}
void EhsHWatchdogKick( void ) {
    EhsTWatchdogKick();
}
void EhsHWatchdogModify( ehs_uint32 period_us ) {
    EhsTWatchdogModify(period_us);
}
#endif//_HAL_WATCHDOG_H_