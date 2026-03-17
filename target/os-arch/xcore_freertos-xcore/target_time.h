/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_time.h
 * Timer definitions for XMOS xcore.ai FreeRTOS targets.
 * Should only be included by hal_time.h.
 *
 * EhsTickType uses microsecond resolution (EHS_TICKS_PER_S = 1 000 000).
 * EhsTgtTimer_now() is implemented using xTaskGetTickCount() scaled to
 * microseconds.  Accuracy is limited to 1 FreeRTOS tick (~1 ms at
 * configTICK_RATE_HZ=1000); good enough for the initial xcore bring-up.
 *
 * @author: inx limited
 */

#ifndef EHS_TARGET_TIME_H
#define EHS_TARGET_TIME_H

#ifndef EHS_HAL_TIME_H
#error "This file should only be included by hal_time.h"
#endif

#include <time.h>
#include "globals.h"

/* EHS system clock resolution: 1 MHz (1 tick == 1 microsecond) */
#define EHS_TICKS_PER_S   (1000000u)
#define EHS_uS_PER_TICK   (1u)

#define EHS_TICKTYPE_MAX      0x7ffffffffffffffful
#define EHS_TICKTYPE_INVALID  0xfffffffffffffffful

#define EhsTgtTimer_usToTick(us) \
    (EHS_uS_PER_TICK <= (us) ? ((EhsTickType)((EhsTickType)(us) / (EhsTickType)EHS_uS_PER_TICK)) : 1)

#define EhsTgtTimer_msToTick(ms) \
    ((EhsTickType)((EhsTickType)(ms) * 1000u / (EhsTickType)EHS_mS_PER_TICK))

#ifndef EHS_MINGW
#define EhsTgtTimer_tickTous(tick) ((ehs_uint64)((tick) * EHS_uS_PER_TICK))
#else
#define EhsTgtTimer_tickTous(tick) (tick)
#endif

typedef ehs_uint64 EhsTickType;

#endif /* EHS_TARGET_TIME_H */
