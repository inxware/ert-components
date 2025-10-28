/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/*
 * target_watchdog.h
 */

#ifndef _TARGET_WATCHDOG_H_
#define _TARGET_WATCHDOG_H_

#include "ehs_fb_types.h"

static inline void EhsTWatchdogEnable() {}
static inline void EhsTWatchdogDisable() {}
static inline void EhsTWatchdogKick() {}
static inline void EhsTWatchdogModify( ehs_uint32 period_us ) {}

#endif//_TARGET_WATCHDOG_H_
